// The originals our C++ still called (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift_originals.py from generated/. Don't edit it by hand:
// run the tool again.

#include "lifted/originals.h"
#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/log.h"
#include "helpers/clock.h"

#include <bit>
#include <cstdlib>

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted::original {

// sub_82091A60: lifted from generated/ by lift.py.
lifted::Result f82091A60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82091A60 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r12 = 0, r31 = 0;
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
  // addi r31,r3,64
  r31 = uint64_t(int64_t(int64_t(r3) + 64));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258dc58
  // (left out: dead in C++)
  { const lifted::Result o = f8258DC58(base, r1, r13, r3, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8258d9f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258D9F8, LIFTED_ARGS(0x82091A84, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820920D8: lifted from generated/ by lift.py.
lifted::Result f820920D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820920D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // mr r30,r3
  r30 = r3;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r30,392
  r3 = uint64_t(int64_t(int64_t(r30) + 392));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8258dd90
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD90(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32247
  r11 = uint64_t(int64_t(-2113339392));
  // stw r30,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r30));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,8176
  r11 = uint64_t(int64_t(int64_t(r11) + 8176));
  // addi r31,r30,40
  r31 = uint64_t(int64_t(int64_t(r30) + 40));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // lwz r9,4(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 4);
  // std r11,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r11);
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // ble cr6,0x82092140
  if (!cr6.gt) goto loc_82092140;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82092140:
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r8,8(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 8);
  // stw r31,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r31));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // ble cr6,0x82092160
  if (!cr6.gt) goto loc_82092160;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82092160:
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // ld r6,0(r10)
  r6 = REX_LOAD_U64(uint32_t(r10) + 0);
  // ld r7,8(r10)
  r7 = REX_LOAD_U64(uint32_t(r10) + 8);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r31,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r31));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // ld r5,0(r9)
  r5 = REX_LOAD_U64(uint32_t(r9) + 0);
  // ld r4,0(r8)
  r4 = REX_LOAD_U64(uint32_t(r8) + 0);
  // bl 0x82091c48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091C48, LIFTED_ARGS(0x82092184, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8209a080
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209A080, LIFTED_ARGS(0x8209218C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82099B18: lifted from generated/ by lift.py.
void f82099B18(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82099B18 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82099b28
  if (!cr6.eq) goto loc_82099B28;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82099B28:
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r11,21(r10)
  r11 = REX_LOAD_U8(uint32_t(r10) + 21);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82099b40
  if (cr6.eq) goto loc_82099B40;
  // twi 31,r0,22
  lifted::Trap(22);
  // blr
  return;
loc_82099B40:
  // lwz r11,8(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 8);
  // lbz r9,21(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 21);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x82099b5c
  if (cr6.eq) goto loc_82099B5C;
  // lwz r11,4(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 4);
  // b 0x82099b8c
  goto loc_82099B8C;
loc_82099B58:
  // mr r11,r10
  r11 = r10;
loc_82099B5C:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lbz r9,21(r10)
  r9 = REX_LOAD_U8(uint32_t(r10) + 21);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x82099b58
  if (cr6.eq) goto loc_82099B58;
  // b 0x82099b98
  goto loc_82099B98;
loc_82099B70:
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x82099b98
  if (!cr6.eq) goto loc_82099B98;
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
loc_82099B8C:
  // lbz r10,21(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 21);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82099b70
  if (cr6.eq) goto loc_82099B70;
loc_82099B98:
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // blr
  return;
}

// sub_8209A080: lifted from generated/ by lift.py.
uint64_t f8209A080(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8209A080 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r4,r3
  r4 = r3;
  // lwz r11,8(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 8);
  // lwz r10,4(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 4);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x8209a0a4
  if (!cr6.gt) goto loc_8209A0A4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8209A0A4:
  // stw r4,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r4));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x8209a0bc
  if (!cr6.gt) goto loc_8209A0BC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8209A0BC:
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r6,0(r9)
  r6 = REX_LOAD_U64(uint32_t(r9) + 0);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r4,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r4));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // ld r5,0(r11)
  r5 = REX_LOAD_U64(uint32_t(r11) + 0);
  // bl 0x82092ef8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092EF8, LIFTED_ARGS(0x8209A0D8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8209BD50: lifted from generated/ by lift.py.
void f8209BD50(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8209BD50 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, ctr = 0;
  lifted::Vec v0{}, v1{}, v2{}, v3{}, v4{}, v5{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{}, v26{}, v27{}, v28{}, v29{}, v30{}, v31{};
  uint32_t ea{};
  // addi r10,r1,-128
  r10 = uint64_t(int64_t(int64_t(r1) + -128));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // mtctr r11
  ctr = r11;
loc_8209BD5C:
  // ld r11,0(r5)
  r11 = REX_LOAD_U64(uint32_t(r5) + 0);
  // addi r5,r5,8
  r5 = uint64_t(int64_t(int64_t(r5) + 8));
  // std r11,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r11);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x8209bd5c
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8209BD5C;
  // addi r10,r1,-64
  r10 = uint64_t(int64_t(int64_t(r1) + -64));
  // mr r11,r4
  r11 = r4;
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // mtctr r9
  ctr = r9;
loc_8209BD80:
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x8209bd80
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8209BD80;
  // addi r11,r1,-96
  r11 = uint64_t(int64_t(int64_t(r1) + -96));
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // addi r9,r3,48
  r9 = uint64_t(int64_t(int64_t(r3) + 48));
  // lvx128 v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,-128
  r11 = uint64_t(int64_t(int64_t(r1) + -128));
  // lvx128 v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,-80
  r11 = uint64_t(int64_t(int64_t(r1) + -80));
  // vmrghw v13,v11,v0
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), simde_mm_load_si128((simde__m128i*)v11.u32)));
  // vmrglw v11,v11,v0
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_unpacklo_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), simde_mm_load_si128((simde__m128i*)v11.u32)));
  // lvx128 v9,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,-112
  r11 = uint64_t(int64_t(int64_t(r1) + -112));
  // lvx128 v8,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v8.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,-64
  r11 = uint64_t(int64_t(int64_t(r1) + -64));
  // vmrghw v12,v8,v9
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v9.u32), simde_mm_load_si128((simde__m128i*)v8.u32)));
  // vmrglw v7,v8,v9
  simde_mm_store_si128((simde__m128i*)v7.u32, simde_mm_unpacklo_epi32(simde_mm_load_si128((simde__m128i*)v9.u32), simde_mm_load_si128((simde__m128i*)v8.u32)));
  // lvx128 v10,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,-48
  r11 = uint64_t(int64_t(int64_t(r1) + -48));
  // vmrghw v0,v13,v12
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v12.u32), simde_mm_load_si128((simde__m128i*)v13.u32)));
  // vmrglw v13,v13,v12
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_unpacklo_epi32(simde_mm_load_si128((simde__m128i*)v12.u32), simde_mm_load_si128((simde__m128i*)v13.u32)));
  // vmrghw v12,v11,v7
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v7.u32), simde_mm_load_si128((simde__m128i*)v11.u32)));
  // vmrglw v11,v11,v7
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_unpacklo_epi32(simde_mm_load_si128((simde__m128i*)v7.u32), simde_mm_load_si128((simde__m128i*)v11.u32)));
  // lvx128 v9,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,-32
  r11 = uint64_t(int64_t(int64_t(r1) + -32));
  // vmsum4fp128 v7,v10,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v7.f32, simde_mm_dp_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v0.f32), 0xFF));
  // vmsum4fp128 v3,v10,v12
  simde_mm_store_ps(v3.f32, simde_mm_dp_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v12.f32), 0xFF));
  // vmsum4fp128 v2,v10,v11
  simde_mm_store_ps(v2.f32, simde_mm_dp_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v11.f32), 0xFF));
  // vmsum4fp128 v6,v10,v13
  simde_mm_store_ps(v6.f32, simde_mm_dp_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v13.f32), 0xFF));
  // lvx128 v10,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum4fp128 v5,v9,v0
  simde_mm_store_ps(v5.f32, simde_mm_dp_ps(simde_mm_load_ps(v9.f32), simde_mm_load_ps(v0.f32), 0xFF));
  // lvx128 v8,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v8.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum4fp128 v4,v9,v13
  simde_mm_store_ps(v4.f32, simde_mm_dp_ps(simde_mm_load_ps(v9.f32), simde_mm_load_ps(v13.f32), 0xFF));
  // addi r11,r3,16
  r11 = uint64_t(int64_t(int64_t(r3) + 16));
  // vmsum4fp128 v1,v9,v12
  simde_mm_store_ps(v1.f32, simde_mm_dp_ps(simde_mm_load_ps(v9.f32), simde_mm_load_ps(v12.f32), 0xFF));
  // addi r10,r3,32
  r10 = uint64_t(int64_t(int64_t(r3) + 32));
  // vmsum4fp128 v9,v9,v11
  simde_mm_store_ps(v9.f32, simde_mm_dp_ps(simde_mm_load_ps(v9.f32), simde_mm_load_ps(v11.f32), 0xFF));
  // vmsum4fp128 v30,v8,v12
  simde_mm_store_ps(v30.f32, simde_mm_dp_ps(simde_mm_load_ps(v8.f32), simde_mm_load_ps(v12.f32), 0xFF));
  // vmsum4fp128 v28,v10,v0
  simde_mm_store_ps(v28.f32, simde_mm_dp_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v0.f32), 0xFF));
  // vmsum4fp128 v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_dp_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v12.f32), 0xFF));
  // vmsum4fp128 v31,v8,v0
  simde_mm_store_ps(v31.f32, simde_mm_dp_ps(simde_mm_load_ps(v8.f32), simde_mm_load_ps(v0.f32), 0xFF));
  // vmsum4fp128 v29,v8,v13
  simde_mm_store_ps(v29.f32, simde_mm_dp_ps(simde_mm_load_ps(v8.f32), simde_mm_load_ps(v13.f32), 0xFF));
  // vmsum4fp128 v27,v10,v13
  simde_mm_store_ps(v27.f32, simde_mm_dp_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v13.f32), 0xFF));
  // vmsum4fp128 v26,v10,v11
  simde_mm_store_ps(v26.f32, simde_mm_dp_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v11.f32), 0xFF));
  // vmsum4fp128 v8,v8,v11
  simde_mm_store_ps(v8.f32, simde_mm_dp_ps(simde_mm_load_ps(v8.f32), simde_mm_load_ps(v11.f32), 0xFF));
  // vmrghw v0,v7,v3
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v3.u32), simde_mm_load_si128((simde__m128i*)v7.u32)));
  // vmrghw v13,v6,v2
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v2.u32), simde_mm_load_si128((simde__m128i*)v6.u32)));
  // vmrghw v11,v5,v1
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v1.u32), simde_mm_load_si128((simde__m128i*)v5.u32)));
  // vmrghw v10,v4,v9
  simde_mm_store_si128((simde__m128i*)v10.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v9.u32), simde_mm_load_si128((simde__m128i*)v4.u32)));
  // vmrghw v0,v0,v13
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), simde_mm_load_si128((simde__m128i*)v0.u32)));
  // vmrghw v13,v28,v12
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v12.u32), simde_mm_load_si128((simde__m128i*)v28.u32)));
  // vmrghw v12,v11,v10
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v10.u32), simde_mm_load_si128((simde__m128i*)v11.u32)));
  // vmrghw v9,v31,v30
  simde_mm_store_si128((simde__m128i*)v9.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v30.u32), simde_mm_load_si128((simde__m128i*)v31.u32)));
  // vmrghw v11,v27,v26
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v26.u32), simde_mm_load_si128((simde__m128i*)v27.u32)));
  // vmrghw v8,v29,v8
  simde_mm_store_si128((simde__m128i*)v8.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v8.u32), simde_mm_load_si128((simde__m128i*)v29.u32)));
  // stvx v0,r0,r3
  ea = (uint32_t(r3)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmrghw v0,v13,v11
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v11.u32), simde_mm_load_si128((simde__m128i*)v13.u32)));
  // stvx v12,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmrghw v10,v9,v8
  simde_mm_store_si128((simde__m128i*)v10.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)v8.u32), simde_mm_load_si128((simde__m128i*)v9.u32)));
  // stvx v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvx v10,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // blr
  return;
}

// sub_8209F738: lifted from generated/ by lift.py.
lifted::Result f8209F738(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8209F738 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // stw r4,-21568(r11)
  REX_STORE_U32(uint32_t(r11) + -21568, uint32_t(r4));
  // bne cr6,0x8209f774
  if (!cr6.eq) goto loc_8209F774;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // addi r3,r11,2368
  r3 = uint64_t(int64_t(int64_t(r11) + 2368));
  // lwz r10,2368(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 2368);
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f1,7820(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7820);
  f1 = double(temp.f32);
  // mtctr r9
  ctr = r9;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_8209F774:
  // cmpwi cr6,r4,1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 1, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // addi r3,r11,2368
  r3 = uint64_t(int64_t(int64_t(r11) + 2368));
  // lwz r10,2368(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 2368);
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f1,7816(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7816);
  f1 = double(temp.f32);
  // mtctr r9
  ctr = r9;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A2D30: lifted from generated/ by lift.py.
void f820A2D30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820A2D30 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Vec v0{}, v3{}, v4{}, v5{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{};
  lifted::Temp temp{};
  lifted::Vec vTemp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-288(r1)
  ea = -288 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,292
  r11 = uint64_t(int64_t(19136512));
  // lfs f0,0(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // ori r10,r11,16237
  r10 = r11 | 16237;
  // vspltisw v6,0
  simde_mm_store_si128((simde__m128i*)v6.u32, simde_mm_set1_epi32(int(0x0)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8104(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8104);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,96(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // addi r11,r11,2544
  r11 = uint64_t(int64_t(int64_t(r11) + 2544));
  // lvx128 v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2496
  r11 = uint64_t(int64_t(int64_t(r11) + 2496));
  // lvx128 v10,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2560
  r11 = uint64_t(int64_t(int64_t(r11) + 2560));
  // lvx128 v9,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2512
  r11 = uint64_t(int64_t(int64_t(r11) + 2512));
  // lvx128 v8,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v8.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2576
  r11 = uint64_t(int64_t(int64_t(r11) + 2576));
  // lvx128 v7,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v7.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2528
  r11 = uint64_t(int64_t(int64_t(r11) + 2528));
  // lvx128 v5,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v5.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // rlwimi r11,r10,30,1,31
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x7FFFFFFF) | (r11 & 0xFFFFFFFF80000000);
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lfs f12,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f12 = double(temp.f32);
  // fadds f11,f12,f0
  f11 = double(float(f12 + f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,8100(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8100);
  f12 = double(temp.f32);
  // fmuls f12,f11,f12
  f12 = double(float(f11 * f12));
  // fctiwz f12,f12
  f12 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f12) ? int64_t(0x80000000U) : (f12 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f12)))));
  // stfiwx f12,0,r9
  REX_STORE_U32(uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f12)));
  // lwa r11,80(r1)
  r11 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r1) + 80))));
  // std r11,144(r1)
  REX_STORE_U64(uint32_t(r1) + 144, r11);
  // lfd f12,144(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 144)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fnmsubs f0,f12,f13,f0
  f0 = double(float(-std::fma(f12, f13, -f0)));
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // stfs f13,104(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvx128 v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // vmulfp128 v0,v13,v13
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32)));
  // vspltw v4,v13,2
  simde_mm_store_si128((simde__m128i*)v4.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x55));
  // vspltw v13,v13,1
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0xAA));
  // vmulfp128 v12,v0,v13
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v13.f32)));
  // vmsum4fp128 v13,v0,v11
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v11.f32), 0xFF));
  // vspltw v3,v0,3
  simde_mm_store_si128((simde__m128i*)v3.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x0));
  // vmsum4fp128 v11,v12,v10
  simde_mm_store_ps(v11.f32, simde_mm_dp_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32), 0xFF));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmulfp128 v13,v3,v4
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v3.f32), simde_mm_load_ps(v4.f32)));
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
  // vmulfp128 v0,v0,v13
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v13.f32)));
  // stvx v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // vmulfp128 v12,v12,v13
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v13.f32)));
  // vmsum4fp128 v10,v0,v9
  simde_mm_store_ps(v10.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v9.f32), 0xFF));
  // vmulfp128 v11,v0,v13
  simde_mm_store_ps(v11.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v13.f32)));
  // vmsum4fp128 v9,v12,v8
  simde_mm_store_ps(v9.f32, simde_mm_dp_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v8.f32), 0xFF));
  // vmulfp128 v0,v12,v13
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v13.f32)));
  // vmsum4fp128 v13,v11,v7
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v7.f32), 0xFF));
  // stvx v10,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,192
  r11 = uint64_t(int64_t(int64_t(r1) + 192));
  // stvx v9,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f13,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 144);
  f13 = double(temp.f32);
  // lfs f0,96(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 96);
  f0 = double(temp.f32);
  // fadds f13,f13,f0
  f13 = double(float(f13 + f0));
  // vmsum4fp128 v13,v0,v5
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v5.f32), 0xFF));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r7,r1,112
  r7 = uint64_t(int64_t(int64_t(r1) + 112));
  // vupkd3d128 v0,v6,4
  temp.f32 = 3.0f;
  temp.s32 += v6.s16[1];
  vTemp.f32[3] = temp.f32;
  temp.f32 = 3.0f;
  temp.s32 += v6.s16[0];
  vTemp.f32[2] = temp.f32;
  vTemp.f32[1] = 0.0f;
  vTemp.f32[0] = 1.0f;
  v0 = vTemp;
  // addi r9,r1,176
  r9 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r1,208
  r5 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // vpermwi128 v10,v0,174
  simde_mm_store_si128((simde__m128i*)v10.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x51));
  // lfs f0,112(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 112);
  f0 = double(temp.f32);
  // vpermwi128 v9,v0,171
  simde_mm_store_si128((simde__m128i*)v9.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x54));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,176(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 176, temp.u32);
  // lfs f0,192(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 192);
  f0 = double(temp.f32);
  // stvx v13,r0,r7
  ea = (uint32_t(r7)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f13,112(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 112);
  f13 = double(temp.f32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // lfs f13,160(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 160);
  f13 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfs f0,144(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 144, temp.u32);
  // fneg f13,f0
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f13,128(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 128, temp.u32);
  // lvlx v12,0,r11
  temp.u32 = uint32_t(r11);
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // addi r11,r1,240
  r11 = uint64_t(int64_t(int64_t(r1) + 240));
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // lvlx v13,0,r9
  temp.u32 = uint32_t(r9);
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // vrlimi128 v0,v12,4,3
  simde_mm_store_ps(v0.f32, simde_mm_blend_ps(simde_mm_load_ps(v0.f32), simde_mm_permute_ps(simde_mm_load_ps(v12.f32), 57), 4));
  // lvlx v11,0,r8
  temp.u32 = uint32_t(r8);
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // vrlimi128 v13,v11,4,3
  simde_mm_store_ps(v13.f32, simde_mm_blend_ps(simde_mm_load_ps(v13.f32), simde_mm_permute_ps(simde_mm_load_ps(v11.f32), 57), 4));
  // stvx v10,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,256
  r11 = uint64_t(int64_t(int64_t(r1) + 256));
  // vrlimi128 v0,v13,3,2
  simde_mm_store_ps(v0.f32, simde_mm_blend_ps(simde_mm_load_ps(v0.f32), simde_mm_permute_ps(simde_mm_load_ps(v13.f32), 78), 3));
  // stvx v9,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,208
  r11 = uint64_t(int64_t(int64_t(r1) + 208));
  // vpermwi128 v13,v0,143
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x70));
  // vpermwi128 v0,v0,111
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x90));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,224
  r11 = uint64_t(int64_t(int64_t(r1) + 224));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // bl 0x8209bd50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209BD50, LIFTED_ARGS(0x820A2F28, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,288
  r1 = uint64_t(int64_t(int64_t(r1) + 288));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return;
}

// sub_820A8F80: lifted from generated/ by lift.py.
lifted::Result f820A8F80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A8F80 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r5
  r28 = r5;
  // std r6,168(r1)
  REX_STORE_U64(uint32_t(r1) + 168, r6);
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // std r28,160(r1)
  REX_STORE_U64(uint32_t(r1) + 160, r28);
  // lwz r11,160(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 160);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820a8fb8
  if (cr6.eq) goto loc_820A8FB8;
  // lwz r10,168(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 168);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x820a8fbc
  if (cr6.eq) goto loc_820A8FBC;
loc_820A8FB8:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A8FBC:
  // lwz r3,164(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 164);
  // lwz r5,172(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 172);
  // subf r11,r3,r5
  r11 = r5 - r3;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a9000
  if (cr0.eq) goto loc_820A9000;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // subf r11,r5,r11
  r11 = r11 - r5;
  // srawi. r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r30,r6,r3
  r30 = r6 + r3;
  // ble 0x820a8ffc
  if (!cr0.gt) goto loc_820A8FFC;
  // mr r4,r6
  r4 = r6;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7348, LIFTED_ARGS(0x820A8FFC, r31)); r3 = o.r3; f1 = o.f1; }
loc_820A8FFC:
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
loc_820A9000:
  // mr r3,r29
  r3 = r29;
  // std r28,0(r29)
  REX_STORE_U64(uint32_t(r29) + 0, r28);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A9198: lifted from generated/ by lift.py.
lifted::Result f820A9198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9198 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r4,r3
  r4 = r3;
  // lwz r11,8(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 8);
  // lwz r10,4(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 4);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x820a91bc
  if (!cr6.gt) goto loc_820A91BC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A91BC:
  // stw r4,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r4));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x820a91d4
  if (!cr6.gt) goto loc_820A91D4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A91D4:
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r6,0(r9)
  r6 = REX_LOAD_U64(uint32_t(r9) + 0);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r4,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r4));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // ld r5,0(r11)
  r5 = REX_LOAD_U64(uint32_t(r11) + 0);
  // bl 0x820a8f80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A8F80, LIFTED_ARGS(0x820A91F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820AC7F0: lifted from generated/ by lift.py.
lifted::Result f820AC7F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AC7F0 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
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
  // li r3,224
  r3 = uint64_t(int64_t(224));
  // mr r30,r4
  r30 = r4;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x820AC814, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820ac82c
  if (cr0.eq) goto loc_820AC82C;
  // mr r5,r30
  r5 = r30;
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x820acc08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACC08, LIFTED_ARGS(0x820AC828, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820ac830
  goto loc_820AC830;
loc_820AC82C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820AC830:
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

// sub_820AC8A0: lifted from generated/ by lift.py.
lifted::Result f820AC8A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AC8A0 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
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
  // li r3,224
  r3 = uint64_t(int64_t(224));
  // mr r30,r4
  r30 = r4;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x820AC8C4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820ac8dc
  if (cr0.eq) goto loc_820AC8DC;
  // mr r5,r30
  r5 = r30;
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x820acaa8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACAA8, LIFTED_ARGS(0x820AC8D8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820ac8e0
  goto loc_820AC8E0;
loc_820AC8DC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820AC8E0:
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

// sub_820B18B8: lifted from generated/ by lift.py.
lifted::Result f820B18B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820B18B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7914
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,48(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f0 = double(temp.f32);
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // lfs f13,10736(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 10736);
  f13 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f27,324(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 324);
  f27 = double(temp.f32);
  // fmr f30,f27
  f30 = f27;
  // lfs f29,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f29 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // lfs f13,304(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 304);
  f13 = double(temp.f32);
  // stfs f13,328(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 328, temp.u32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x820b1914
  if (!cr6.lt) goto loc_820B1914;
  // fmr f0,f29
  f0 = f29;
loc_820B1914:
  // lbz r10,276(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 276);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x820b1930
  if (!cr6.eq) goto loc_820B1930;
  // lfs f13,328(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 328);
  f13 = double(temp.f32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // fdivs f0,f13,f0
  f0 = double(float(f13 / f0));
  // fmuls f30,f0,f27
  f30 = double(float(f0 * f27));
loc_820B1930:
  // fcmpu cr6,f30,f29
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f30, f29);
  // bge cr6,0x820b1940
  if (!cr6.lt) goto loc_820B1940;
  // fmr f30,f29
  f30 = f29;
  // b 0x820b194c
  goto loc_820B194C;
loc_820B1940:
  // fcmpu cr6,f27,f30
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f27, f30);
  // bge cr6,0x820b194c
  if (!cr6.lt) goto loc_820B194C;
  // fmr f30,f27
  f30 = f27;
loc_820B194C:
  // fneg f31,f30
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f30) ^ 0x8000000000000000));
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // fmr f28,f31
  f28 = f31;
  // bl 0x820b6930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6930, LIFTED_ARGS(0x820B195C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // lfs f0,24(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 24);
  f0 = double(temp.f32);
  // lfs f13,20(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 20);
  f13 = double(temp.f32);
  // lfs f12,16(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 16);
  f12 = double(temp.f32);
  // stfs f12,80(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f29,92(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // stfs f13,84(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // bl 0x820b6948
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6948, LIFTED_ARGS(0x820B1984, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294608, LIFTED_ARGS(0x820B1990, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lfs f12,92(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 92);
  f12 = double(temp.f32);
  // lfs f0,3384(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3384);
  f0 = double(temp.f32);
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lfs f13,3380(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3380);
  f13 = double(temp.f32);
  // fmuls f13,f13,f1
  f13 = double(float(f13 * f1));
  // fmsubs f0,f12,f0,f13
  f0 = double(float(std::fma(f12, f0, -f13)));
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // bge cr6,0x820b19cc
  if (!cr6.lt) goto loc_820B19CC;
  // fneg f13,f27
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f27) ^ 0x8000000000000000));
  // fmr f28,f0
  f28 = f0;
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x820b19e4
  if (!cr6.lt) goto loc_820B19E4;
  // fmr f28,f13
  f28 = f13;
  // b 0x820b19e4
  goto loc_820B19E4;
loc_820B19CC:
  // fcmpu cr6,f0,f30
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f30);
  // ble cr6,0x820b19e4
  if (!cr6.gt) goto loc_820B19E4;
  // fmr f30,f0
  f30 = f0;
  // fcmpu cr6,f0,f27
  cr6 = lifted::CompareFloat(f0, f27);
  // ble cr6,0x820b19e4
  if (!cr6.gt) goto loc_820B19E4;
  // fmr f30,f27
  f30 = f27;
loc_820B19E4:
  // lfs f0,280(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 280);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // blt cr6,0x820b19fc
  if (cr6.lt) goto loc_820B19FC;
  // fmuls f0,f0,f28
  f0 = double(float(f0 * f28));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x820b1a08
  goto loc_820B1A08;
loc_820B19FC:
  // fmuls f0,f0,f30
  lifted::ScalarFloatMode();
  f0 = double(float(f0 * f30));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
loc_820B1A08:
  // fmr f31,f0
  lifted::ScalarFloatMode();
  f31 = f0;
  // stfs f0,304(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 304, temp.u32);
  // fcmpu cr6,f31,f29
  cr6 = lifted::CompareFloat(f31, f29);
  // beq cr6,0x820b1a68
  if (cr6.eq) goto loc_820B1A68;
  // fmr f1,f31
  f1 = f31;
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS(0x820B1A20, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lfs f0,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f0 = double(temp.f32);
  // lfs f12,8(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8);
  f12 = double(temp.f32);
  // fabs f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) & ~0x8000000000000000));
  // fdivs f13,f0,f13
  f13 = double(float(f0 / f13));
  // fsubs f13,f13,f12
  f13 = double(float(f13 - f12));
  // fcmpu cr6,f13,f29
  cr6 = lifted::CompareFloat(f13, f29);
  // bgt cr6,0x820b1a50
  if (cr6.gt) goto loc_820B1A50;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8116(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8116);
  f0 = double(temp.f32);
  // b 0x820b1a5c
  goto loc_820B1A5C;
loc_820B1A50:
  // fdivs f1,f0,f13
  lifted::ScalarFloatMode();
  f1 = double(float(f0 / f13));
  // bl 0x825a8688
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8688, LIFTED_ARGS(0x820B1A58, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
loc_820B1A5C:
  // fcmpu cr6,f31,f29
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f31, f29);
  // bge cr6,0x820b1a68
  if (!cr6.lt) goto loc_820B1A68;
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
loc_820B1A68:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x820b1a7c
  if (cr6.eq) goto loc_820B1A7C;
  // stfs f31,312(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 312, temp.u32);
  // stfs f0,308(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 308, temp.u32);
  // b 0x820b1a84
  goto loc_820B1A84;
loc_820B1A7C:
  // stfs f0,312(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 312, temp.u32);
  // stfs f31,308(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 308, temp.u32);
loc_820B1A84:
  // fneg f13,f27
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f27) ^ 0x8000000000000000));
  // lfs f0,304(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 304);
  f0 = double(temp.f32);
  // stfs f28,316(r31)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r31) + 316, temp.u32);
  // stfs f30,320(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 320, temp.u32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x820b1ab0
  if (cr6.lt) goto loc_820B1AB0;
  // fcmpu cr6,f27,f0
  cr6 = lifted::CompareFloat(f27, f0);
  // bge cr6,0x820b1aac
  if (!cr6.lt) goto loc_820B1AAC;
  // fmr f13,f27
  f13 = f27;
  // b 0x820b1ab0
  goto loc_820B1AB0;
loc_820B1AAC:
  // fmr f13,f0
  lifted::ScalarFloatMode();
  f13 = f0;
loc_820B1AB0:
  // lfs f11,324(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 324);
  f11 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f11,f11,f13
  f11 = double(float(f11 + f13));
  // lwz r10,272(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lfs f12,384(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 384);
  f12 = double(temp.f32);
  // lfs f13,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f13 = double(temp.f32);
  // fmuls f13,f11,f13
  f13 = double(float(f11 * f13));
  // stfs f13,324(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 324, temp.u32);
  // lfs f13,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f13 = double(temp.f32);
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,3116(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 3116, temp.u32);
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7960
  // (left out: dead in C++)
  // (left out: dead in C++)
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

// sub_820B2100: lifted from generated/ by lift.py.
double f820B2100(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820B2100 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r11,272(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 272);
  // lfs f8,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f8 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,180(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 180);
  f0 = double(temp.f32);
  // fsubs f12,f8,f0
  f12 = double(float(f8 - f0));
  // stfs f0,1988(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 1988, temp.u32);
  // stfs f12,2116(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 2116, temp.u32);
  // lfs f13,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // beq cr6,0x820b22c4
  if (cr6.eq) goto loc_820B22C4;
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // beq cr6,0x820b22c4
  if (cr6.eq) goto loc_820B22C4;
  // lwz r10,2376(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 2376);
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // beq cr6,0x820b224c
  if (cr6.eq) goto loc_820B224C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // beq cr6,0x820b21d4
  if (cr6.eq) goto loc_820B21D4;
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bgt cr6,0x820b22c4
  if (cr6.gt) goto loc_820B22C4;
  // lwz r10,2252(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 2252);
  // lfs f12,180(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 180);
  f12 = double(temp.f32);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x820b21c8
  if (!cr6.gt) goto loc_820B21C8;
  // lfs f12,1432(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1432);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,1800(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1800);
  f0 = double(temp.f32);
  // fadds f0,f0,f12
  f0 = double(float(f0 + f12));
  // lfs f12,1064(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1064);
  f12 = double(temp.f32);
  // lfs f11,696(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 696);
  f11 = double(temp.f32);
  // fadds f11,f12,f11
  f11 = double(float(f12 + f11));
  // fabs f12,f0
  f12 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // lfs f0,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // fabs f11,f11
  f11 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f11) & ~0x8000000000000000));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // fabs f10,f12
  f10 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) & ~0x8000000000000000));
  // fabs f11,f11
  f11 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f11) & ~0x8000000000000000));
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // ble cr6,0x820b21c4
  if (!cr6.gt) goto loc_820B21C4;
  // fabs f13,f12
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) & ~0x8000000000000000));
  // fdivs f12,f13,f11
  f12 = double(float(f13 / f11));
  // fcmpu cr6,f12,f0
  cr6 = lifted::CompareFloat(f12, f0);
  // ble cr6,0x820b21c8
  if (!cr6.gt) goto loc_820B21C8;
loc_820B21C4:
  // fmr f12,f0
  lifted::ScalarFloatMode();
  f12 = f0;
loc_820B21C8:
  // stfs f12,1988(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 1988, temp.u32);
  // fsubs f0,f8,f12
  f0 = double(float(f8 - f12));
  // b 0x820b2244
  goto loc_820B2244;
loc_820B21D4:
  // lfs f12,696(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 696);
  f12 = double(temp.f32);
  // lfs f0,1064(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1064);
  f0 = double(temp.f32);
  // fadds f0,f0,f12
  f0 = double(float(f0 + f12));
  // lfs f11,1432(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1432);
  f11 = double(temp.f32);
  // lfs f12,1800(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1800);
  f12 = double(temp.f32);
  // fadds f12,f12,f11
  f12 = double(float(f12 + f11));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bne cr6,0x820b21fc
  if (!cr6.eq) goto loc_820B21FC;
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // beq cr6,0x820b2208
  if (cr6.eq) goto loc_820B2208;
loc_820B21FC:
  // fadds f11,f12,f0
  lifted::ScalarFloatMode();
  f11 = double(float(f12 + f0));
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // bne cr6,0x820b2210
  if (!cr6.eq) goto loc_820B2210;
loc_820B2208:
  // fmr f0,f8
  lifted::ScalarFloatMode();
  f0 = f8;
  // fmr f12,f8
  f12 = f8;
loc_820B2210:
  // fadds f12,f12,f0
  lifted::ScalarFloatMode();
  f12 = double(float(f12 + f0));
  // lfs f13,180(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 180);
  f13 = double(temp.f32);
  // fdivs f0,f0,f12
  f0 = double(float(f0 / f12));
  // fsubs f0,f8,f0
  f0 = double(float(f8 - f0));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x820b2238
  if (cr6.lt) goto loc_820B2238;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x820b223c
  if (!cr6.gt) goto loc_820B223C;
loc_820B2238:
  // fmr f0,f13
  lifted::ScalarFloatMode();
  f0 = f13;
loc_820B223C:
  // stfs f0,1988(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 1988, temp.u32);
  // fsubs f0,f8,f0
  f0 = double(float(f8 - f0));
loc_820B2244:
  // stfs f0,2116(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 2116, temp.u32);
  // b 0x820b22c4
  goto loc_820B22C4;
loc_820B224C:
  // lfs f9,180(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 180);
  f9 = double(temp.f32);
  // lwz r11,2252(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 2252);
  // fsubs f8,f8,f9
  f8 = double(float(f8 - f9));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x820b22bc
  if (!cr6.gt) goto loc_820B22BC;
  // lfs f0,80(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 80);
  f0 = double(temp.f32);
  // fabs f1,f0
  f1 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // lfs f13,1400(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1400);
  f13 = double(temp.f32);
  // lfs f0,1768(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1768);
  f0 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // lfs f12,1032(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 1032);
  f12 = double(temp.f32);
  // lfs f13,664(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 664);
  f13 = double(temp.f32);
  // fadds f13,f12,f13
  f13 = double(float(f12 + f13));
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // fabs f7,f0
  f7 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // bl 0x820b0340
  // (left out: dead in C++)
  f1 = f820B0340(base, r1, r3, f1);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,7140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // fmuls f13,f7,f0
  f13 = double(float(f7 * f0));
  // fmuls f13,f1,f13
  f13 = double(float(f1 * f13));
  // fadds f9,f13,f9
  f9 = double(float(f13 + f9));
  // fsubs f8,f8,f13
  f8 = double(float(f8 - f13));
  // fcmpu cr6,f9,f0
  cr6 = lifted::CompareFloat(f9, f0);
  // ble cr6,0x820b22b0
  if (!cr6.gt) goto loc_820B22B0;
  // fmr f9,f0
  f9 = f0;
loc_820B22B0:
  // fcmpu cr6,f8,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f8, f0);
  // bge cr6,0x820b22bc
  if (!cr6.lt) goto loc_820B22BC;
  // fmr f8,f0
  f8 = f0;
loc_820B22BC:
  // stfs f9,1988(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r3) + 1988, temp.u32);
  // stfs f8,2116(r3)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + 2116, temp.u32);
loc_820B22C4:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return f1;
}

// sub_820B5670: lifted from generated/ by lift.py.
lifted::Result f820B5670(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_820B5670 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-96(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -96, std::bit_cast<uint64_t>(f29));
  // stfd f30,-88(r1)
  REX_STORE_U64(uint32_t(r1) + -88, std::bit_cast<uint64_t>(f30));
  // stfd f31,-80(r1)
  REX_STORE_U64(uint32_t(r1) + -80, std::bit_cast<uint64_t>(f31));
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r24,r5
  r24 = r5;
  // mr r27,r6
  r27 = r6;
  // bl 0x820b0ee8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B0EE8, LIFTED_ARGS(0x820B569C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r31,416
  r26 = uint64_t(int64_t(int64_t(r31) + 416));
  // li r5,1472
  r5 = uint64_t(int64_t(1472));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B56B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // stw r29,272(r31)
  REX_STORE_U32(uint32_t(r31) + 272, uint32_t(r29));
  // addi r11,r31,2144
  r11 = uint64_t(int64_t(int64_t(r31) + 2144));
  // mr r10,r25
  r10 = r25;
  // stb r9,2624(r31)
  REX_STORE_U8(uint32_t(r31) + 2624, uint8_t(r9));
loc_820B56C8:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x820b56e8
  if (cr6.eq) goto loc_820B56E8;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // beq cr6,0x820b56e8
  if (cr6.eq) goto loc_820B56E8;
  // lwz r9,272(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 272);
  // addi r8,r31,2016
  r8 = uint64_t(int64_t(int64_t(r31) + 2016));
  // addi r9,r9,608
  r9 = uint64_t(int64_t(int64_t(r9) + 608));
  // b 0x820b56f4
  goto loc_820B56F4;
loc_820B56E8:
  // lwz r9,272(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 272);
  // addi r8,r31,1888
  r8 = uint64_t(int64_t(int64_t(r31) + 1888));
  // addi r9,r9,336
  r9 = uint64_t(int64_t(int64_t(r9) + 336));
loc_820B56F4:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // blt cr6,0x820b56c8
  if (cr6.lt) goto loc_820B56C8;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r3,528
  r3 = uint64_t(int64_t(528));
  // lfs f12,400(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 400);
  f12 = double(temp.f32);
  // lfs f13,672(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 672);
  f13 = double(temp.f32);
  // fadds f13,f13,f12
  f13 = double(float(f13 + f12));
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // lfs f0,8680(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8680);
  f0 = double(temp.f32);
  // fmadds f0,f13,f0,f12
  f0 = double(float(std::fma(f13, f0, f12)));
  // stfs f0,52(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x820B5738, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820b5748
  if (cr0.eq) goto loc_820B5748;
  // bl 0x820b7928
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7928, LIFTED_ARGS(0x820B5744, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820b574c
  goto loc_820B574C;
loc_820B5748:
  // mr r3,r25
  r3 = r25;
loc_820B574C:
  // stw r3,336(r31)
  REX_STORE_U32(uint32_t(r31) + 336, uint32_t(r3));
  // bl 0x820b77d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B77D0, LIFTED_ARGS(0x820B5754, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f1,52(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f1 = double(temp.f32);
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // bl 0x820b6698
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6698, LIFTED_ARGS(0x820B5760, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // addi r4,r11,64
  r4 = uint64_t(int64_t(int64_t(r11) + 64));
  // bl 0x820b7280
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7280, LIFTED_ARGS(0x820B5770, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // addi r4,r11,96
  r4 = uint64_t(int64_t(int64_t(r11) + 96));
  // bl 0x820b67e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B67E0, LIFTED_ARGS(0x820B5780, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // addi r28,r11,10624
  r28 = uint64_t(int64_t(int64_t(r11) + 10624));
  // addi r4,r28,-4
  r4 = uint64_t(int64_t(int64_t(r28) + -4));
  // bl 0x8259d5a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D5A8, LIFTED_ARGS(0x820B5794, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // lfs f1,8696(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8696);
  f1 = double(temp.f32);
  // bl 0x820b6950
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6950, LIFTED_ARGS(0x820B57A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f30,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f30 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f30,80(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f30,88(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // stfs f30,92(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // lfs f0,10892(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10892);
  f0 = double(temp.f32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // bl 0x820b6640
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6640, LIFTED_ARGS(0x820B57D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,496
  r3 = uint64_t(int64_t(496));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x820B57D8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820b57f4
  if (cr0.eq) goto loc_820B57F4;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // addi r5,r11,1776
  r5 = uint64_t(int64_t(int64_t(r11) + 1776));
  // addi r4,r11,1760
  r4 = uint64_t(int64_t(int64_t(r11) + 1760));
  // bl 0x820bb188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BB188, LIFTED_ARGS(0x820B57F0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820b57f8
  goto loc_820B57F8;
loc_820B57F4:
  // mr r3,r25
  r3 = r25;
loc_820B57F8:
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // stw r3,344(r31)
  REX_STORE_U32(uint32_t(r31) + 344, uint32_t(r3));
  // lfs f1,1792(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1792);
  f1 = double(temp.f32);
  // bl 0x820b9660
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9660, LIFTED_ARGS(0x820B5808, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // lfs f1,1796(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1796);
  f1 = double(temp.f32);
  // bl 0x820b9658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9658, LIFTED_ARGS(0x820B5818, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // lfs f1,1792(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1792);
  f1 = double(temp.f32);
  // bl 0x820b9678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9678, LIFTED_ARGS(0x820B5828, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // lfs f1,1796(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1796);
  f1 = double(temp.f32);
  // bl 0x820b9670
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9670, LIFTED_ARGS(0x820B5838, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r27
  r5 = r27;
  // lwz r4,344(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 344);
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // bl 0x820b9570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9570, LIFTED_ARGS(0x820B5848, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stfs f30,108(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // stw r3,340(r31)
  REX_STORE_U32(uint32_t(r31) + 340, uint32_t(r3));
  // lfs f31,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f31,96(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // stfs f31,104(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // lfs f29,10600(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10600);
  f29 = double(temp.f32);
  // stfs f29,100(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // bl 0x820b7d68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D68, LIFTED_ARGS(0x820B5874, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // lfs f1,8712(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8712);
  f1 = double(temp.f32);
  // bl 0x820b7d70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D70, LIFTED_ARGS(0x820B5884, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // mr r4,r30
  r4 = r30;
  // bl 0x820b7cf0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7CF0, LIFTED_ARGS(0x820B5894, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D38, LIFTED_ARGS(0x820B58A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D58, LIFTED_ARGS(0x820B58AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D18, LIFTED_ARGS(0x820B58B8, r31)); r3 = o.r3; f1 = o.f1; }
  // oris r30,r27,1792
  r30 = r27 | 117440512;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // mr r4,r30
  r4 = r30;
  // bl 0x820b7cc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7CC8, LIFTED_ARGS(0x820B58C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D28, LIFTED_ARGS(0x820B58D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D48, LIFTED_ARGS(0x820B58E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r4,256
  r4 = uint64_t(int64_t(16777216));
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D18, LIFTED_ARGS(0x820B58EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1472
  r5 = uint64_t(int64_t(1472));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stfs f30,280(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 280, temp.u32);
  // mr r3,r26
  r3 = r26;
  // stfs f30,284(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 284, temp.u32);
  // stfs f30,292(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 292, temp.u32);
  // stb r25,3132(r31)
  REX_STORE_U8(uint32_t(r31) + 3132, uint8_t(r25));
  // stfs f30,296(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 296, temp.u32);
  // stb r25,276(r31)
  REX_STORE_U8(uint32_t(r31) + 276, uint8_t(r25));
  // stfs f30,304(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 304, temp.u32);
  // stfs f30,324(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 324, temp.u32);
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B591C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,1888
  r3 = uint64_t(int64_t(int64_t(r31) + 1888));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B592C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,2016
  r3 = uint64_t(int64_t(int64_t(r31) + 2016));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B593C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,144
  r5 = uint64_t(int64_t(144));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,2204
  r3 = uint64_t(int64_t(int64_t(r31) + 2204));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B594C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,708
  r11 = uint64_t(int64_t(int64_t(r31) + 708));
  // li r10,4
  r10 = uint64_t(int64_t(4));
loc_820B5954:
  // stfs f31,-64(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + -64, temp.u32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stfs f30,0(r11)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,368
  r11 = uint64_t(int64_t(int64_t(r11) + 368));
  // bne 0x820b5954
  if (!cr0.eq) goto loc_820B5954;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // stfs f31,1984(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 1984, temp.u32);
  // stfs f31,2112(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 2112, temp.u32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f0 = double(temp.f32);
  // lfs f11,480(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 480);
  f11 = double(temp.f32);
  // lfs f13,752(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 752);
  f13 = double(temp.f32);
  // fadds f13,f13,f11
  f13 = double(float(f13 + f11));
  // lfs f11,1836(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1836);
  f11 = double(temp.f32);
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // lfs f12,10888(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 10888);
  f12 = double(temp.f32);
  // fmuls f10,f13,f0
  f10 = double(float(f13 * f0));
  // lfs f13,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f13 = double(temp.f32);
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // stfs f11,2968(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 2968, temp.u32);
  // lfs f11,1840(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1840);
  f11 = double(temp.f32);
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // fnmsubs f12,f10,f12,f31
  f12 = double(float(-std::fma(f10, f12, -f31)));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,2972(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2972, temp.u32);
  // lfs f0,1836(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 1836);
  f0 = double(temp.f32);
  // fdivs f0,f0,f12
  f0 = double(float(f0 / f12));
  // stfs f0,2976(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2976, temp.u32);
  // lfs f13,1840(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 1840);
  f13 = double(temp.f32);
  // fdivs f13,f13,f12
  f13 = double(float(f13 / f12));
  // stfs f13,2980(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2980, temp.u32);
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // stfs f0,2976(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2976, temp.u32);
  // fabs f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) & ~0x8000000000000000));
  // stfs f13,2980(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2980, temp.u32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x820b59f0
  if (!cr6.lt) goto loc_820B59F0;
  // fmr f0,f30
  f0 = f30;
  // b 0x820b59fc
  goto loc_820B59FC;
loc_820B59F0:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x820b59fc
  if (!cr6.gt) goto loc_820B59FC;
  // fmr f0,f31
  f0 = f31;
loc_820B59FC:
  // stfs f0,2976(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2976, temp.u32);
  // fcmpu cr6,f13,f30
  cr6 = lifted::CompareFloat(f13, f30);
  // bge cr6,0x820b5a10
  if (!cr6.lt) goto loc_820B5A10;
  // fmr f13,f30
  f13 = f30;
  // b 0x820b5a1c
  goto loc_820B5A1C;
loc_820B5A10:
  // fcmpu cr6,f13,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f13, f31);
  // ble cr6,0x820b5a1c
  if (!cr6.gt) goto loc_820B5A1C;
  // fmr f13,f31
  f13 = f31;
loc_820B5A1C:
  // mr r3,r31
  r3 = r31;
  // stfs f13,2980(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2980, temp.u32);
  // stb r24,2249(r31)
  REX_STORE_U8(uint32_t(r31) + 2249, uint8_t(r24));
  // bl 0x820b4aa8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B4AA8, LIFTED_ARGS(0x820B5A2C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b13c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B13C8, LIFTED_ARGS(0x820B5A30, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b4420
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B4420, LIFTED_ARGS(0x820B5A38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,172(r29)
  r9 = REX_LOAD_U32(uint32_t(r29) + 172);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r31,620
  r10 = uint64_t(int64_t(int64_t(r31) + 620));
  // stw r9,2376(r31)
  REX_STORE_U32(uint32_t(r31) + 2376, uint32_t(r9));
  // lfs f13,180(r29)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 180);
  f13 = double(temp.f32);
  // stfs f13,1988(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1988, temp.u32);
  // lfs f13,180(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 180);
  f13 = double(temp.f32);
  // fsubs f13,f31,f13
  f13 = double(float(f31 - f13));
  // stfs f13,2116(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2116, temp.u32);
  // lfs f13,1832(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 1832);
  f13 = double(temp.f32);
  // fsubs f13,f31,f13
  f13 = double(float(f31 - f13));
  // lfs f0,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // fmadds f0,f13,f0,f31
  f0 = double(float(std::fma(f13, f0, f31)));
  // stfs f0,2936(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2936, temp.u32);
  // lfs f0,1832(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 1832);
  f0 = double(temp.f32);
  // fsubs f0,f31,f0
  f0 = double(float(f31 - f0));
  // fnmsubs f0,f0,f29,f31
  f0 = double(float(-std::fma(f0, f29, -f31)));
  // stfs f0,2940(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2940, temp.u32);
loc_820B5A84:
  // stw r25,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r25));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,368
  r10 = uint64_t(int64_t(int64_t(r10) + 368));
  // bne 0x820b5a84
  if (!cr0.eq) goto loc_820B5A84;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lfs f0,140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 140);
  f0 = double(temp.f32);
  // stfs f0,2000(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2000, temp.u32);
  // lfs f0,144(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 144);
  f0 = double(temp.f32);
  // stfs f0,2128(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2128, temp.u32);
  // lwz r10,1268(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 1268);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x820b5af4
  if (!cr6.eq) goto loc_820B5AF4;
  // lfs f0,1568(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1568);
  f0 = double(temp.f32);
  // stfs f0,2288(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2288, temp.u32);
  // lfs f11,1600(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1600);
  f11 = double(temp.f32);
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // lfs f13,2328(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 2328);
  f13 = double(temp.f32);
  // lfs f12,2336(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 2336);
  f12 = double(temp.f32);
  // fadds f12,f12,f13
  f12 = double(float(f12 + f13));
  // stfs f30,2320(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 2320, temp.u32);
  // stfs f13,2324(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2324, temp.u32);
  // stfs f12,2312(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 2312, temp.u32);
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // stfs f0,2304(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2304, temp.u32);
  // lfs f0,1544(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1544);
  f0 = double(temp.f32);
  // lfs f13,1504(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1504);
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,2308(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2308, temp.u32);
loc_820B5AF4:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f29,-96(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -96)));
  // lfd f30,-88(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -88)));
  // lfd f31,-80(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -80)));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820B5B08: lifted from generated/ by lift.py.
lifted::Result f820B5B08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_820B5B08 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-96(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -96, std::bit_cast<uint64_t>(f29));
  // stfd f30,-88(r1)
  REX_STORE_U64(uint32_t(r1) + -88, std::bit_cast<uint64_t>(f30));
  // stfd f31,-80(r1)
  REX_STORE_U64(uint32_t(r1) + -80, std::bit_cast<uint64_t>(f31));
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // bl 0x820b0ee8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B0EE8, LIFTED_ARGS(0x820B5B30, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r31,416
  r26 = uint64_t(int64_t(int64_t(r31) + 416));
  // li r5,1472
  r5 = uint64_t(int64_t(1472));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B5B44, r31)); r3 = o.r3; f1 = o.f1; }
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // stw r29,272(r31)
  REX_STORE_U32(uint32_t(r31) + 272, uint32_t(r29));
  // addi r11,r31,2144
  r11 = uint64_t(int64_t(int64_t(r31) + 2144));
  // mr r10,r27
  r10 = r27;
  // stb r24,2624(r31)
  REX_STORE_U8(uint32_t(r31) + 2624, uint8_t(r24));
loc_820B5B5C:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x820b5b7c
  if (cr6.eq) goto loc_820B5B7C;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // beq cr6,0x820b5b7c
  if (cr6.eq) goto loc_820B5B7C;
  // lwz r9,272(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 272);
  // addi r8,r31,2016
  r8 = uint64_t(int64_t(int64_t(r31) + 2016));
  // addi r9,r9,608
  r9 = uint64_t(int64_t(int64_t(r9) + 608));
  // b 0x820b5b88
  goto loc_820B5B88;
loc_820B5B7C:
  // lwz r9,272(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 272);
  // addi r8,r31,1888
  r8 = uint64_t(int64_t(int64_t(r31) + 1888));
  // addi r9,r9,336
  r9 = uint64_t(int64_t(int64_t(r9) + 336));
loc_820B5B88:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // blt cr6,0x820b5b5c
  if (cr6.lt) goto loc_820B5B5C;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // lfs f12,400(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 400);
  f12 = double(temp.f32);
  // lfs f13,672(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 672);
  f13 = double(temp.f32);
  // fadds f13,f13,f12
  f13 = double(float(f13 + f12));
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // lfs f0,8680(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8680);
  f0 = double(temp.f32);
  // fmadds f0,f13,f0,f12
  f0 = double(float(std::fma(f13, f0, f12)));
  // stfs f0,52(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // bl 0x820b77d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B77D0, LIFTED_ARGS(0x820B5BCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f1,52(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f1 = double(temp.f32);
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // bl 0x820b6698
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6698, LIFTED_ARGS(0x820B5BD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // addi r4,r11,64
  r4 = uint64_t(int64_t(int64_t(r11) + 64));
  // bl 0x820b7280
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7280, LIFTED_ARGS(0x820B5BE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // addi r4,r11,96
  r4 = uint64_t(int64_t(int64_t(r11) + 96));
  // bl 0x820b67e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B67E0, LIFTED_ARGS(0x820B5BF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // addi r25,r11,10624
  r25 = uint64_t(int64_t(int64_t(r11) + 10624));
  // addi r4,r25,-4
  r4 = uint64_t(int64_t(int64_t(r25) + -4));
  // bl 0x8259d5a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D5A8, LIFTED_ARGS(0x820B5C0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // lfs f1,8696(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8696);
  f1 = double(temp.f32);
  // bl 0x820b6950
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6950, LIFTED_ARGS(0x820B5C1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f30,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f30 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f30,80(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f30,88(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // stfs f30,92(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // lfs f0,10892(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10892);
  f0 = double(temp.f32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // bl 0x820b6640
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6640, LIFTED_ARGS(0x820B5C48, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820b5c58
  if (cr0.eq) goto loc_820B5C58;
  // bl 0x820b9448
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9448, LIFTED_ARGS(0x820B5C58, r31)); r3 = o.r3; f1 = o.f1; }
loc_820B5C58:
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // stw r27,340(r31)
  REX_STORE_U32(uint32_t(r31) + 340, uint32_t(r27));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820b5c80
  if (cr0.eq) goto loc_820B5C80;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820B5C7C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r27,344(r31)
  REX_STORE_U32(uint32_t(r31) + 344, uint32_t(r27));
loc_820B5C80:
  // li r3,496
  r3 = uint64_t(int64_t(496));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x820B5C88, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820b5ca4
  if (cr0.eq) goto loc_820B5CA4;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // addi r5,r11,1776
  r5 = uint64_t(int64_t(int64_t(r11) + 1776));
  // addi r4,r11,1760
  r4 = uint64_t(int64_t(int64_t(r11) + 1760));
  // bl 0x820bb188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BB188, LIFTED_ARGS(0x820B5CA0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820b5ca8
  goto loc_820B5CA8;
loc_820B5CA4:
  // mr r3,r27
  r3 = r27;
loc_820B5CA8:
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // stw r3,344(r31)
  REX_STORE_U32(uint32_t(r31) + 344, uint32_t(r3));
  // lfs f1,1792(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1792);
  f1 = double(temp.f32);
  // bl 0x820b9660
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9660, LIFTED_ARGS(0x820B5CB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // lfs f1,1796(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1796);
  f1 = double(temp.f32);
  // bl 0x820b9658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9658, LIFTED_ARGS(0x820B5CC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // lfs f1,1792(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1792);
  f1 = double(temp.f32);
  // bl 0x820b9678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9678, LIFTED_ARGS(0x820B5CD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // lfs f1,1796(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1796);
  f1 = double(temp.f32);
  // bl 0x820b9670
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9670, LIFTED_ARGS(0x820B5CE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r28
  r5 = r28;
  // lwz r4,344(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 344);
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // bl 0x820b9570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9570, LIFTED_ARGS(0x820B5CF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stfs f30,108(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // stw r3,340(r31)
  REX_STORE_U32(uint32_t(r31) + 340, uint32_t(r3));
  // lfs f31,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f31,96(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // stfs f31,104(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // lfs f29,10600(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10600);
  f29 = double(temp.f32);
  // stfs f29,100(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // bl 0x820b7d68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D68, LIFTED_ARGS(0x820B5D24, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // lfs f1,8712(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8712);
  f1 = double(temp.f32);
  // bl 0x820b7d70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D70, LIFTED_ARGS(0x820B5D34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // mr r4,r30
  r4 = r30;
  // bl 0x820b7cf0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7CF0, LIFTED_ARGS(0x820B5D44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D38, LIFTED_ARGS(0x820B5D50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D58, LIFTED_ARGS(0x820B5D5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D18, LIFTED_ARGS(0x820B5D68, r31)); r3 = o.r3; f1 = o.f1; }
  // oris r30,r28,1792
  r30 = r28 | 117440512;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // mr r4,r30
  r4 = r30;
  // bl 0x820b7cc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7CC8, LIFTED_ARGS(0x820B5D78, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D28, LIFTED_ARGS(0x820B5D84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D48, LIFTED_ARGS(0x820B5D90, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r4,256
  r4 = uint64_t(int64_t(16777216));
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // bl 0x820b7d18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7D18, LIFTED_ARGS(0x820B5D9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r3,r31,352
  r3 = uint64_t(int64_t(int64_t(r31) + 352));
  // stfs f30,280(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 280, temp.u32);
  // addi r30,r11,14224
  r30 = uint64_t(int64_t(int64_t(r11) + 14224));
  // stfs f30,284(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 284, temp.u32);
  // stfs f30,292(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 292, temp.u32);
  // stb r27,3132(r31)
  REX_STORE_U8(uint32_t(r31) + 3132, uint8_t(r27));
  // mr r4,r30
  r4 = r30;
  // stfs f30,296(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 296, temp.u32);
  // stfs f30,304(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 304, temp.u32);
  // stb r27,276(r31)
  REX_STORE_U8(uint32_t(r31) + 276, uint8_t(r27));
  // stfs f30,324(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 324, temp.u32);
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820B5DD0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,384
  r3 = uint64_t(int64_t(int64_t(r31) + 384));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820B5DDC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1472
  r5 = uint64_t(int64_t(1472));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B5DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,1888
  r3 = uint64_t(int64_t(int64_t(r31) + 1888));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B5DFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,2016
  r3 = uint64_t(int64_t(int64_t(r31) + 2016));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B5E0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,144
  r5 = uint64_t(int64_t(144));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,2204
  r3 = uint64_t(int64_t(int64_t(r31) + 2204));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820B5E1C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,708
  r11 = uint64_t(int64_t(int64_t(r31) + 708));
  // li r10,4
  r10 = uint64_t(int64_t(4));
loc_820B5E24:
  // stfs f31,-64(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + -64, temp.u32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stfs f30,0(r11)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,368
  r11 = uint64_t(int64_t(int64_t(r11) + 368));
  // bne 0x820b5e24
  if (!cr0.eq) goto loc_820B5E24;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // stfs f31,1984(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 1984, temp.u32);
  // stfs f31,2112(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 2112, temp.u32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f0 = double(temp.f32);
  // lfs f11,480(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 480);
  f11 = double(temp.f32);
  // lfs f13,752(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 752);
  f13 = double(temp.f32);
  // fadds f13,f13,f11
  f13 = double(float(f13 + f11));
  // lfs f11,1836(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1836);
  f11 = double(temp.f32);
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // lfs f12,10888(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 10888);
  f12 = double(temp.f32);
  // fmuls f10,f13,f0
  f10 = double(float(f13 * f0));
  // lfs f13,0(r25)
  temp.u32 = REX_LOAD_U32(uint32_t(r25) + 0);
  f13 = double(temp.f32);
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // stfs f11,2968(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 2968, temp.u32);
  // lfs f11,1840(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1840);
  f11 = double(temp.f32);
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // fnmsubs f12,f10,f12,f31
  f12 = double(float(-std::fma(f10, f12, -f31)));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,2972(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2972, temp.u32);
  // lfs f0,1836(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 1836);
  f0 = double(temp.f32);
  // fdivs f0,f0,f12
  f0 = double(float(f0 / f12));
  // stfs f0,2976(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2976, temp.u32);
  // lfs f13,1840(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 1840);
  f13 = double(temp.f32);
  // fdivs f13,f13,f12
  f13 = double(float(f13 / f12));
  // stfs f13,2980(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2980, temp.u32);
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // stfs f0,2976(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2976, temp.u32);
  // fabs f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) & ~0x8000000000000000));
  // stfs f13,2980(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2980, temp.u32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x820b5ec0
  if (!cr6.lt) goto loc_820B5EC0;
  // fmr f0,f30
  f0 = f30;
  // b 0x820b5ecc
  goto loc_820B5ECC;
loc_820B5EC0:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x820b5ecc
  if (!cr6.gt) goto loc_820B5ECC;
  // fmr f0,f31
  f0 = f31;
loc_820B5ECC:
  // stfs f0,2976(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2976, temp.u32);
  // fcmpu cr6,f13,f30
  cr6 = lifted::CompareFloat(f13, f30);
  // bge cr6,0x820b5ee0
  if (!cr6.lt) goto loc_820B5EE0;
  // fmr f13,f30
  f13 = f30;
  // b 0x820b5eec
  goto loc_820B5EEC;
loc_820B5EE0:
  // fcmpu cr6,f13,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f13, f31);
  // ble cr6,0x820b5eec
  if (!cr6.gt) goto loc_820B5EEC;
  // fmr f13,f31
  f13 = f31;
loc_820B5EEC:
  // mr r3,r31
  r3 = r31;
  // stfs f13,2980(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2980, temp.u32);
  // stb r24,2249(r31)
  REX_STORE_U8(uint32_t(r31) + 2249, uint8_t(r24));
  // bl 0x820b4aa8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B4AA8, LIFTED_ARGS(0x820B5EFC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b13c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B13C8, LIFTED_ARGS(0x820B5F00, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b4420
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B4420, LIFTED_ARGS(0x820B5F08, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,172(r29)
  r9 = REX_LOAD_U32(uint32_t(r29) + 172);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r31,620
  r10 = uint64_t(int64_t(int64_t(r31) + 620));
  // stw r9,2376(r31)
  REX_STORE_U32(uint32_t(r31) + 2376, uint32_t(r9));
  // lfs f13,180(r29)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 180);
  f13 = double(temp.f32);
  // stfs f13,1988(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1988, temp.u32);
  // lfs f13,180(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 180);
  f13 = double(temp.f32);
  // fsubs f13,f31,f13
  f13 = double(float(f31 - f13));
  // stfs f13,2116(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2116, temp.u32);
  // lfs f13,1832(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 1832);
  f13 = double(temp.f32);
  // fsubs f13,f31,f13
  f13 = double(float(f31 - f13));
  // lfs f0,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // fmadds f0,f13,f0,f31
  f0 = double(float(std::fma(f13, f0, f31)));
  // stfs f0,2936(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2936, temp.u32);
  // lfs f0,1832(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 1832);
  f0 = double(temp.f32);
  // fsubs f0,f31,f0
  f0 = double(float(f31 - f0));
  // fnmsubs f0,f0,f29,f31
  f0 = double(float(-std::fma(f0, f29, -f31)));
  // stfs f0,2940(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2940, temp.u32);
loc_820B5F54:
  // stw r27,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r27));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,368
  r10 = uint64_t(int64_t(int64_t(r10) + 368));
  // bne 0x820b5f54
  if (!cr0.eq) goto loc_820B5F54;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // lfs f0,140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 140);
  f0 = double(temp.f32);
  // stfs f0,2000(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2000, temp.u32);
  // lfs f0,144(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 144);
  f0 = double(temp.f32);
  // stfs f0,2128(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2128, temp.u32);
  // lwz r10,1268(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 1268);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x820b5fc4
  if (!cr6.eq) goto loc_820B5FC4;
  // lfs f0,1568(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1568);
  f0 = double(temp.f32);
  // stfs f0,2288(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2288, temp.u32);
  // lfs f11,1600(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1600);
  f11 = double(temp.f32);
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // lfs f13,2328(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 2328);
  f13 = double(temp.f32);
  // lfs f12,2336(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 2336);
  f12 = double(temp.f32);
  // fadds f12,f12,f13
  f12 = double(float(f12 + f13));
  // stfs f30,2320(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 2320, temp.u32);
  // stfs f13,2324(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 2324, temp.u32);
  // stfs f12,2312(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 2312, temp.u32);
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // stfs f0,2304(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2304, temp.u32);
  // lfs f0,1544(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1544);
  f0 = double(temp.f32);
  // lfs f13,1504(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 1504);
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,2308(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2308, temp.u32);
loc_820B5FC4:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f29,-96(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -96)));
  // lfd f30,-88(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -88)));
  // lfd f31,-80(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -80)));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820B5FD8: lifted from generated/ by lift.py.
lifted::Result f820B5FD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r8, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_820B5FD8 (lifted)");
  uint64_t r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7914
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f29,f1
  lifted::ScalarFloatMode();
  f29 = f1;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // fmr f11,f3
  f11 = f3;
  // clrlwi. r9,r4,24
  r9 = uint32_t(r4) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // stb r10,276(r31)
  REX_STORE_U8(uint32_t(r31) + 276, uint8_t(r10));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,316(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 316);
  f0 = double(temp.f32);
  // lfs f13,320(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 320);
  f13 = double(temp.f32);
  // fmuls f10,f0,f2
  f10 = double(float(f0 * f2));
  // lfs f27,7136(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7136);
  f27 = double(temp.f32);
  // fmuls f8,f13,f2
  f8 = double(float(f13 * f2));
  // beq 0x820b6030
  if (cr0.eq) goto loc_820B6030;
  // fmr f29,f27
  f29 = f27;
loc_820B6030:
  // lis r30,-32256
  r30 = uint64_t(int64_t(-2113929216));
  // clrlwi. r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lfs f2,2608(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 2608);
  f2 = double(temp.f32);
  // beq 0x820b6044
  if (cr0.eq) goto loc_820B6044;
  // fmr f29,f2
  f29 = f2;
loc_820B6044:
  // lfs f0,48(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // lfs f13,10736(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 10736);
  f13 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f31,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f31 = double(temp.f32);
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // fcmpu cr6,f12,f31
  cr6 = lifted::CompareFloat(f12, f31);
  // bge cr6,0x820b606c
  if (!cr6.lt) goto loc_820B606C;
  // fmr f12,f31
  f12 = f31;
loc_820B606C:
  // lfs f13,1048(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1048);
  f13 = double(temp.f32);
  // addi r7,r11,336
  r7 = uint64_t(int64_t(int64_t(r11) + 336));
  // lfs f9,680(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 680);
  f9 = double(temp.f32);
  // addi r3,r31,2988
  r3 = uint64_t(int64_t(int64_t(r31) + 2988));
  // fadds f9,f13,f9
  f9 = double(float(f13 + f9));
  // lfs f13,696(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 696);
  f13 = double(temp.f32);
  // lfs f7,1064(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1064);
  f7 = double(temp.f32);
  // fadds f7,f7,f13
  f7 = double(float(f7 + f13));
  // lfs f0,328(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 328);
  f0 = double(temp.f32);
  // lfs f3,484(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 484);
  f3 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f13 = double(temp.f32);
  // fmuls f9,f9,f13
  f9 = double(float(f9 * f13));
  // fmuls f1,f7,f13
  f1 = double(float(f7 * f13));
  // fmuls f13,f0,f11
  f13 = double(float(f0 * f11));
  // fadds f0,f0,f12
  f0 = double(float(f0 + f12));
  // fdivs f0,f13,f0
  f0 = double(float(f13 / f0));
  // fdivs f0,f2,f0
  f0 = double(float(f2 / f0));
  // fmuls f30,f0,f10
  f30 = double(float(f0 * f10));
  // fmuls f28,f0,f8
  f28 = double(float(f0 * f8));
  // bl 0x820b0668
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B0668, LIFTED_ARGS(0x820B60C0, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f1,f9
  lifted::ScalarFloatMode();
  f1 = f9;
  // bl 0x820b3e98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B3E98, LIFTED_ARGS(0x820B60C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,304(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 304);
  f0 = double(temp.f32);
  // lfs f13,92(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 92);
  f13 = double(temp.f32);
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // lfs f13,280(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 280);
  f13 = double(temp.f32);
  // fsubs f0,f29,f13
  f0 = double(float(f29 - f13));
  // stfs f1,3108(r31)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r31) + 3108, temp.u32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x820b6104
  if (!cr6.gt) goto loc_820B6104;
  // fcmpu cr6,f12,f31
  cr6 = lifted::CompareFloat(f12, f31);
  // ble cr6,0x820b60f4
  if (!cr6.gt) goto loc_820B60F4;
  // fmr f30,f28
  f30 = f28;
loc_820B60F4:
  // fcmpu cr6,f0,f30
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f30);
  // ble cr6,0x820b6120
  if (!cr6.gt) goto loc_820B6120;
  // fmr f0,f30
  f0 = f30;
  // b 0x820b6120
  goto loc_820B6120;
loc_820B6104:
  // fcmpu cr6,f12,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f12, f31);
  // bge cr6,0x820b6110
  if (!cr6.lt) goto loc_820B6110;
  // fmr f30,f28
  f30 = f28;
loc_820B6110:
  // fneg f12,f30
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f30) ^ 0x8000000000000000));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x820b6120
  if (!cr6.lt) goto loc_820B6120;
  // fmr f0,f12
  f0 = f12;
loc_820B6120:
  // fadds f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 + f0));
  // stfs f0,280(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 280, temp.u32);
  // fcmpu cr6,f0,f27
  cr6 = lifted::CompareFloat(f0, f27);
  // bge cr6,0x820b6138
  if (!cr6.lt) goto loc_820B6138;
  // fmr f0,f27
  f0 = f27;
  // b 0x820b6148
  goto loc_820B6148;
loc_820B6138:
  // lfs f13,2608(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 2608);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x820b6148
  if (!cr6.gt) goto loc_820B6148;
  // fmr f0,f13
  f0 = f13;
loc_820B6148:
  // stfs f0,280(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 280, temp.u32);
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7960
  // (left out: dead in C++)
  // (left out: dead in C++)
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

// sub_820B6498: lifted from generated/ by lift.py.
lifted::Result f820B6498(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820B6498 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // bl 0x820afae8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AFAE8, LIFTED_ARGS(0x820B64AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stb r11,2456(r31)
  REX_STORE_U8(uint32_t(r31) + 2456, uint8_t(r11));
  // bl 0x820b13c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B13C8, LIFTED_ARGS(0x820B64BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b4420
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B4420, LIFTED_ARGS(0x820B64C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b6328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6328, LIFTED_ARGS(0x820B64CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r31,416
  r10 = uint64_t(int64_t(int64_t(r31) + 416));
  // lfs f0,7132(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7132);
  f0 = double(temp.f32);
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820B64DC:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x820b64ec
  if (!cr6.gt) goto loc_820B64EC;
  // fmr f0,f13
  f0 = f13;
loc_820B64EC:
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,368
  r10 = uint64_t(int64_t(int64_t(r10) + 368));
  // bne 0x820b64dc
  if (!cr0.eq) goto loc_820B64DC;
  // lwz r11,340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 340);
  // mr r3,r31
  r3 = r31;
  // fneg f0,f0
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,144(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 144, temp.u32);
  // bl 0x820b1790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B1790, LIFTED_ARGS(0x820B650C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b6430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6430, LIFTED_ARGS(0x820B6510, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b18b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B18B8, LIFTED_ARGS(0x820B6514, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b1b00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B1B00, LIFTED_ARGS(0x820B651C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820afbd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AFBD8, LIFTED_ARGS(0x820B6524, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820afd30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AFD30, LIFTED_ARGS(0x820B652C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b1fb8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B1FB8, LIFTED_ARGS(0x820B6530, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b2100
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B2100, LIFTED_ARGS(0x820B6538, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b22d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B22D8, LIFTED_ARGS(0x820B653C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b24a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B24A8, LIFTED_ARGS(0x820B6540, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b2808
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B2808, LIFTED_ARGS(0x820B6544, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r11,r31,704
  r11 = uint64_t(int64_t(int64_t(r31) + 704));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // lfs f0,2612(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2612);
  f0 = double(temp.f32);
loc_820B6554:
  // stfs f0,-4(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,368
  r11 = uint64_t(int64_t(int64_t(r11) + 368));
  // bne 0x820b6554
  if (!cr0.eq) goto loc_820B6554;
  // addi r10,r31,620
  r10 = uint64_t(int64_t(int64_t(r31) + 620));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_820B6574:
  // stw r29,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r29));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,368
  r10 = uint64_t(int64_t(int64_t(r10) + 368));
  // bne 0x820b6574
  if (!cr0.eq) goto loc_820B6574;
  // lwz r11,272(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 272);
  // mr r30,r29
  r30 = r29;
  // lwz r10,2368(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 2368);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lfs f0,140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 140);
  f0 = double(temp.f32);
  // stfs f0,2000(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2000, temp.u32);
  // lfs f0,144(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 144);
  f0 = double(temp.f32);
  // stfs f0,2128(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2128, temp.u32);
  // ble cr6,0x820b65e0
  if (!cr6.gt) goto loc_820B65E0;
loc_820B65A8:
  // mr r3,r31
  r3 = r31;
  // bl 0x820b0718
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B0718, LIFTED_ARGS(0x820B65B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b0838
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B0838, LIFTED_ARGS(0x820B65B8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b3f80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B3F80, LIFTED_ARGS(0x820B65BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b0908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B0908, LIFTED_ARGS(0x820B65C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b2748
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B2748, LIFTED_ARGS(0x820B65CC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b4778
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B4778, LIFTED_ARGS(0x820B65D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,2368(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 2368);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x820b65a8
  if (cr6.lt) goto loc_820B65A8;
loc_820B65E0:
  // mr r3,r31
  r3 = r31;
  // bl 0x820b2a48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B2A48, LIFTED_ARGS(0x820B65E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b2cc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B2CC0, LIFTED_ARGS(0x820B65F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b13c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B13C8, LIFTED_ARGS(0x820B65F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b4420
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B4420, LIFTED_ARGS(0x820B6600, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b1b00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B1B00, LIFTED_ARGS(0x820B6608, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820b35c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B35C8, LIFTED_ARGS(0x820B6610, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b3788
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B3788, LIFTED_ARGS(0x820B6614, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b39c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B39C8, LIFTED_ARGS(0x820B6618, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820afda8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AFDA8, LIFTED_ARGS(0x820B661C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820afe90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AFE90, LIFTED_ARGS(0x820B6620, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,2516(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 2516);
  f0 = double(temp.f32);
  // stfs f0,2520(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 2520, temp.u32);
  // stb r29,2444(r31)
  REX_STORE_U8(uint32_t(r31) + 2444, uint8_t(r29));
  // stb r29,2445(r31)
  REX_STORE_U8(uint32_t(r31) + 2445, uint8_t(r29));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820B7998: lifted from generated/ by lift.py.
lifted::Result f820B7998(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820B7998 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // stw r11,288(r31)
  REX_STORE_U32(uint32_t(r31) + 288, uint32_t(r11));
  // bl 0x820b6930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6930, LIFTED_ARGS(0x820B79C0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,32
  r30 = uint64_t(int64_t(int64_t(r31) + 32));
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820bb758
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BB758, LIFTED_ARGS(0x820B79D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // bl 0x820b6930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6930, LIFTED_ARGS(0x820B79D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820bb758
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BB758, LIFTED_ARGS(0x820B79E4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820B81C8: lifted from generated/ by lift.py.
lifted::Result f820B81C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820B81C8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
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
  // bl 0x825a7900
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-416(r1)
  ea = -416 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // lfs f27,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f27 = double(temp.f32);
  // stfs f27,140(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
  // stfs f27,156(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 156, temp.u32);
  // stfs f27,172(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 172, temp.u32);
  // bl 0x820b8cb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B8CB0, LIFTED_ARGS(0x820B8204, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r29
  r6 = r29;
  // mr r5,r31
  r5 = r31;
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r27,r3
  r27 = r3;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // bl 0x820b7ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7ED0, LIFTED_ARGS(0x820B821C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f23,7140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f23 = double(temp.f32);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x820b82cc
  if (cr0.eq) goto loc_820B82CC;
  // lwz r11,184(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 184);
  // lwz r3,16(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16);
  // rlwinm r29,r11,0,5,7
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x7000000;
  // bl 0x820b6890
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6890, LIFTED_ARGS(0x820B823C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // mr r4,r3
  r4 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820b82a4
  if (cr6.eq) goto loc_820B82A4;
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294608, LIFTED_ARGS(0x820B8254, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // fabs f31,f1
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f1) & ~0x8000000000000000));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820b82a4
  if (cr6.eq) goto loc_820B82A4;
  // lwz r3,16(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16);
  // lwz r11,516(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 516);
  // clrlwi. r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x820b82a4
  if (cr0.eq) goto loc_820B82A4;
  // rlwinm. r11,r11,31,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820b82a4
  if (cr0.eq) goto loc_820B82A4;
  // bl 0x820b6898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6898, LIFTED_ARGS(0x820B8280, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8696(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8696);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x820b82a4
  if (!cr6.lt) goto loc_820B82A4;
  // fcmpu cr6,f31,f23
  cr6 = lifted::CompareFloat(f31, f23);
  // ble cr6,0x820b82a4
  if (!cr6.gt) goto loc_820B82A4;
  // li r25,1
  r25 = uint64_t(int64_t(1));
loc_820B82A4:
  // lwz r11,260(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 260);
  // and. r11,r11,r29
  r11 = r11 & r29;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820b82cc
  if (cr0.eq) goto loc_820B82CC;
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfs f1,176(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 176);
  f1 = double(temp.f32);
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294430, LIFTED_ARGS(0x820B82C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b7b60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7B60, LIFTED_ARGS(0x820B82CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_820B82CC:
  // lwz r3,16(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16);
  // bl 0x820b6898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6898, LIFTED_ARGS(0x820B82D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f22,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f22 = double(temp.f32);
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x820b8538
  if (cr6.eq) goto loc_820B8538;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r29,r27,56
  r29 = uint64_t(int64_t(int64_t(r27) + 56));
  // mr r26,r31
  r26 = r31;
  // lfd f24,10904(r9)
  f24 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r9) + 10904)));
  // lfs f28,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f28 = double(temp.f32);
  // lfs f25,8124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8124);
  f25 = double(temp.f32);
loc_820B8300:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // stfs f27,80(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // lwz r10,256(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 256);
  // stfs f27,84(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // rlwinm r31,r11,0,5,7
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x7000000;
  // stfs f27,88(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // stfs f27,92(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stfs f27,108(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // and. r11,r10,r31
  r11 = r10 & r31;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820b852c
  if (cr0.eq) goto loc_820B852C;
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // bne cr6,0x820b852c
  if (!cr6.eq) goto loc_820B852C;
  // rlwinm. r11,r31,0,7,7
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 0) & 0x1000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820b8348
  if (cr0.eq) goto loc_820B8348;
  // lwz r3,16(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16);
  // bl 0x820b8d00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B8D00, LIFTED_ARGS(0x820B8340, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,4(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f0 = double(temp.f32);
  // stfs f0,-52(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + -52, temp.u32);
loc_820B8348:
  // addi r28,r29,-24
  r28 = uint64_t(int64_t(int64_t(r29) + -24));
  // lwz r3,16(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16);
  // addi r27,r29,-56
  r27 = uint64_t(int64_t(int64_t(r29) + -56));
  // mr r5,r28
  r5 = r28;
  // mr r4,r27
  r4 = r27;
  // bl 0x820b6c78
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6C78, LIFTED_ARGS(0x820B8360, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f26,f1
  lifted::ScalarFloatMode();
  f26 = f1;
  // fcmpu cr6,f26,f27
  cr6 = lifted::CompareFloat(f26, f27);
  // ble cr6,0x820b852c
  if (!cr6.gt) goto loc_820B852C;
  // mr r3,r31
  r3 = r31;
  // bl 0x820bc530
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC530, LIFTED_ARGS(0x820B8374, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f1,f25
  lifted::ScalarFloatMode();
  f0 = double(float(f1 + f25));
  // fmuls f0,f0,f23
  f0 = double(float(f0 * f23));
  // fcmpu cr6,f0,f27
  cr6 = lifted::CompareFloat(f0, f27);
  // blt cr6,0x820b8394
  if (cr6.lt) goto loc_820B8394;
  // fcmpu cr6,f0,f28
  cr6 = lifted::CompareFloat(f0, f28);
  // ble cr6,0x820b8398
  if (!cr6.gt) goto loc_820B8398;
  // fmr f0,f28
  f0 = f28;
  // b 0x820b8398
  goto loc_820B8398;
loc_820B8394:
  // fmr f0,f27
  lifted::ScalarFloatMode();
  f0 = f27;
loc_820B8398:
  // fadds f0,f0,f28
  lifted::ScalarFloatMode();
  f0 = double(float(f0 + f28));
  // mr r4,r28
  r4 = r28;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // fmuls f30,f0,f26
  f30 = double(float(f0 * f26));
  // fmr f1,f30
  f1 = f30;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294430, LIFTED_ARGS(0x820B83B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820B83BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,20(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 20);
  // bl 0x820b9668
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9668, LIFTED_ARGS(0x820B83C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // bl 0x820bc540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC540, LIFTED_ARGS(0x820B83D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,280(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 280);
  // fadds f31,f1,f31
  lifted::ScalarFloatMode();
  f31 = double(float(f1 + f31));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820b83ec
  if (cr6.eq) goto loc_820B83EC;
  // lwz r3,20(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 20);
  // bl 0x820b9668
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9668, LIFTED_ARGS(0x820B83E8, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
loc_820B83EC:
  // fmr f2,f22
  lifted::ScalarFloatMode();
  f2 = f22;
  // fmr f1,f24
  f1 = f24;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS(0x820B83F8, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f29,f1
  lifted::ScalarFloatMode();
  f29 = f1;
  // rlwinm. r11,r31,0,5,5
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 0) & 0x4000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x820b842c
  if (!cr0.eq) goto loc_820B842C;
  // mr r3,r31
  r3 = r31;
  // bl 0x820bc538
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC538, LIFTED_ARGS(0x820B840C, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f29
  lifted::ScalarFloatMode();
  f0 = double(float(f29));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // fmuls f1,f1,f0
  f1 = double(float(f1 * f0));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294430, LIFTED_ARGS(0x820B8420, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820B842C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820B842C:
  // fcmpu cr6,f31,f27
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f31, f27);
  // ble cr6,0x820b84b0
  if (!cr6.gt) goto loc_820B84B0;
  // mr r6,r28
  r6 = r28;
  // lwz r4,16(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 16);
  // mr r5,r27
  r5 = r27;
  // fmr f2,f31
  f2 = f31;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // fmr f1,f30
  f1 = f30;
  // bl 0x820b6d48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6D48, LIFTED_ARGS(0x820B8450, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820bc548
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC548, LIFTED_ARGS(0x820B8458, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // rlwinm. r11,r31,0,6,6
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 0) & 0x2000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820b8490
  if (cr0.eq) goto loc_820B8490;
  // bl 0x82294330
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294330, LIFTED_ARGS(0x820B8470, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,240
  r3 = uint64_t(int64_t(int64_t(r1) + 240));
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294430, LIFTED_ARGS(0x820B8480, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294330
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294330, LIFTED_ARGS(0x820B848C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820b84b0
  goto loc_820B84B0;
loc_820B8490:
  // bl 0x82294320
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294320, LIFTED_ARGS(0x820B8494, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,224
  r3 = uint64_t(int64_t(int64_t(r1) + 224));
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294430, LIFTED_ARGS(0x820B84A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294320
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294320, LIFTED_ARGS(0x820B84B0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820B84B0:
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,16(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16);
  // bl 0x820b6958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6958, LIFTED_ARGS(0x820B84BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r27
  r5 = r27;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,16(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16);
  // bl 0x820b69d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B69D8, LIFTED_ARGS(0x820B84CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16);
  // fmr f1,f28
  lifted::ScalarFloatMode();
  f1 = f28;
  // bl 0x820b75b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B75B8, LIFTED_ARGS(0x820B84D8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820b8a48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B8A48, LIFTED_ARGS(0x820B84DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x820b852c
  if (cr0.eq) goto loc_820B852C;
  // lwz r11,240(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 240);
  // mr r4,r27
  r4 = r27;
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // stw r11,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r11));
  // stw r31,240(r30)
  REX_STORE_U32(uint32_t(r30) + 240, uint32_t(r31));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // rlwinm r11,r11,0,8,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFF80FFFFFF;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // stfs f26,64(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f26);
  REX_STORE_U32(uint32_t(r31) + 64, temp.u32);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820B8514, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r29,-40
  r4 = uint64_t(int64_t(int64_t(r29) + -40));
  // addi r3,r31,32
  r3 = uint64_t(int64_t(int64_t(r31) + 32));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820B8520, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // addi r3,r31,48
  r3 = uint64_t(int64_t(int64_t(r31) + 48));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820B852C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820B852C:
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r29,r29,80
  r29 = uint64_t(int64_t(int64_t(r29) + 80));
  // bne 0x820b8300
  if (!cr0.eq) goto loc_820B8300;
loc_820B8538:
  // addi r1,r1,416
  r1 = uint64_t(int64_t(int64_t(r1) + 416));
  // addi r12,r1,-64
  r12 = uint64_t(int64_t(int64_t(r1) + -64));
  // bl 0x825a794c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BB758: lifted from generated/ by lift.py.
lifted::Result f820BB758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820BB758 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-608(r1)
  ea = -608 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BB798, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // bne cr6,0x820bb80c
  if (!cr6.eq) goto loc_820BB80C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,112(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 112);
  f13 = double(temp.f32);
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // beq cr6,0x820bb80c
  if (cr6.eq) goto loc_820BB80C;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x820bb688
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BB688, LIFTED_ARGS(0x820BB7C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f13,372(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 372);
  f13 = double(temp.f32);
  // lfs f0,112(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 112);
  f0 = double(temp.f32);
  // addi r5,r1,256
  r5 = uint64_t(int64_t(int64_t(r1) + 256));
  // addi r4,r1,368
  r4 = uint64_t(int64_t(int64_t(r1) + 368));
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfs f0,372(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 372, temp.u32);
  // bl 0x820b99d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B99D0, LIFTED_ARGS(0x820BB7E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r30
  r4 = r30;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BB7FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,10920
  r11 = uint64_t(int64_t(int64_t(r11) + 10920));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // b 0x820bb828
  goto loc_820BB828;
loc_820BB80C:
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // mr r4,r30
  r4 = r30;
  // lwz r5,0(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BB828, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BB828:
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BB840, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820bae78
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BAE78, LIFTED_ARGS(0x820BB848, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820bac00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BAC00, LIFTED_ARGS(0x820BB850, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,608
  r1 = uint64_t(int64_t(int64_t(r1) + 608));
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

// sub_820BBE30: lifted from generated/ by lift.py.
lifted::Result f820BBE30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820BBE30 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  // stwu r1,-1632(r1)
  ea = -1632 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x820bd1c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BD1C8, LIFTED_ARGS(0x820BBE4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r5,r10,32
  r5 = uint64_t(int64_t(int64_t(r10) + 32));
  // addi r4,r11,32
  r4 = uint64_t(int64_t(int64_t(r11) + 32));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // bl 0x820bc848
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC848, LIFTED_ARGS(0x820BBE68, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r31,24
  r4 = uint64_t(int64_t(int64_t(r31) + 24));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x820bd508
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BD508, LIFTED_ARGS(0x820BBE74, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bbe84
  if (cr0.eq) goto loc_820BBE84;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820bc050
  goto loc_820BC050;
loc_820BBE84:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_820BBE88:
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x820bce80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BCE80, LIFTED_ARGS(0x820BBE90, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bbef8
  if (cr0.eq) goto loc_820BBEF8;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x820b6888
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6888, LIFTED_ARGS(0x820BBEA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // lwz r4,20(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // bl 0x820bb868
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BB868, LIFTED_ARGS(0x820BBEB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r9,260(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 260);
  // lwz r8,244(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 244);
  // and. r9,r9,r8
  r9 = r9 & r8;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820bc05c
  if (cr0.eq) goto loc_820BC05C;
  // lwz r11,244(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 244);
  // lwz r10,260(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 260);
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820bc05c
  if (cr0.eq) goto loc_820BC05C;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x820bd6e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BD6E0, LIFTED_ARGS(0x820BBEE4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820bc05c
  if (!cr0.eq) goto loc_820BC05C;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x820bbe88
  if (cr6.lt) goto loc_820BBE88;
loc_820BBEF8:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r3,r11,32
  r3 = uint64_t(int64_t(int64_t(r11) + 32));
  // lfs f31,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f31 = double(temp.f32);
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // bl 0x820bab30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BAB30, LIFTED_ARGS(0x820BBF10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,32
  r3 = uint64_t(int64_t(int64_t(r11) + 32));
  // bl 0x820bab30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BAB30, LIFTED_ARGS(0x820BBF20, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r30
  r5 = r30;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822942E0, LIFTED_ARGS(0x820BBF30, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820BBF3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f0,208(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 208);
  f0 = double(temp.f32);
  // lfs f13,208(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 208);
  f13 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfs f13,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // lfs f13,212(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 212);
  f13 = double(temp.f32);
  // lfs f0,212(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 212);
  f0 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // lfs f13,84(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 84);
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // lfs f13,216(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 216);
  f13 = double(temp.f32);
  // lfs f0,216(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 216);
  f0 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // lfs f13,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294170, LIFTED_ARGS(0x820BBF98, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8748(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8748);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x820bbfcc
  if (!cr6.lt) goto loc_820BBFCC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f31,100(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stfs f31,104(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // lfs f0,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820BBFCC, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BBFCC:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294298
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294298, LIFTED_ARGS(0x820BBFD4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_820BBFD8:
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x820bcdd0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BCDD0, LIFTED_ARGS(0x820BBFE4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bc04c
  if (cr0.eq) goto loc_820BC04C;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x820b6888
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6888, LIFTED_ARGS(0x820BBFF4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // lwz r4,20(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // bl 0x820bb868
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BB868, LIFTED_ARGS(0x820BC004, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r9,260(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 260);
  // lwz r8,244(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 244);
  // and. r9,r9,r8
  r9 = r9 & r8;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820bc05c
  if (cr0.eq) goto loc_820BC05C;
  // lwz r11,244(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 244);
  // lwz r10,260(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 260);
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820bc05c
  if (cr0.eq) goto loc_820BC05C;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x820bd6e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BD6E0, LIFTED_ARGS(0x820BC038, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820bc05c
  if (!cr0.eq) goto loc_820BC05C;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 3, so);
  // blt cr6,0x820bbfd8
  if (cr6.lt) goto loc_820BBFD8;
loc_820BC04C:
  // mr r3,r29
  r3 = r29;
loc_820BC050:
  // addi r1,r1,1632
  r1 = uint64_t(int64_t(int64_t(r1) + 1632));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_820BC05C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x820bc050
  goto loc_820BC050;
}

// sub_820BFD30: lifted from generated/ by lift.py.
lifted::Result f820BFD30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_820BFD30 (lifted)");
  uint64_t r5 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7914
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lbz r10,17(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 17);
  // mr r30,r3
  r30 = r3;
  // lfs f31,6200(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 6200);
  f31 = double(temp.f32);
  // lis r29,-32256
  r29 = uint64_t(int64_t(-2113929216));
  // extsb r11,r10
  r11 = uint64_t(int64_t(int8_t(r10)));
  // lfs f27,2612(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 2612);
  f27 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f29,2608(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 2608);
  f29 = double(temp.f32);
  // lfs f30,8708(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 8708);
  f30 = double(temp.f32);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // lfs f28,11220(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 11220);
  f28 = double(temp.f32);
  // blt cr6,0x820bfe18
  if (cr6.lt) goto loc_820BFE18;
  // beq cr6,0x820bfdb8
  if (cr6.eq) goto loc_820BFDB8;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x820bfeb4
  if (!cr6.lt) goto loc_820BFEB4;
  // lha r11,8(r31)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 8))));
  // fmr f2,f31
  f2 = f31;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f0,f28
  f1 = double(float(f0 * f28));
  // bl 0x820b3d10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B3D10, LIFTED_ARGS(0x820BFDB4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820bfeb4
  goto loc_820BFEB4;
loc_820BFDB8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lbz r10,16(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 16);
  // lha r9,8(r31)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 8))));
  // mr r3,r30
  r3 = r30;
  // fmr f2,f31
  lifted::ScalarFloatMode();
  f2 = f31;
  // lfs f0,10152(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10152);
  f0 = double(temp.f32);
  // extsb r11,r10
  r11 = uint64_t(int64_t(int8_t(r10)));
  // mr r10,r9
  r10 = r9;
  // addi r11,r11,10
  r11 = uint64_t(int64_t(int64_t(r11) + 10));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
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
  // fmuls f1,f13,f28
  f1 = double(float(f13 * f28));
  // lfs f13,11200(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 11200);
  f13 = double(temp.f32);
  // fnmsubs f12,f12,f30,f29
  f12 = double(float(-std::fma(f12, f30, -f29)));
  // fmadds f3,f12,f0,f13
  f3 = double(float(std::fma(f12, f0, f13)));
  // bl 0x820b6170
  // (left out: dead in C++)
  { const lifted::Result o = f820B6170(base, r1, r13, r3, r6, r8, r9, f1, f2, f3); r3 = o.r3; f1 = o.f1; }
  // b 0x820bfeb4
  goto loc_820BFEB4;
loc_820BFE18:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lbz r10,16(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 16);
  // lwz r9,0(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lfs f0,10152(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10152);
  f0 = double(temp.f32);
  // extsb r11,r10
  r11 = uint64_t(int64_t(int8_t(r10)));
  // addi r11,r11,10
  r11 = uint64_t(int64_t(int64_t(r11) + 10));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // rlwinm. r10,r9,0,6,6
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x2000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lha r11,8(r31)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 8))));
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fnmsubs f12,f13,f30,f29
  f12 = double(float(-std::fma(f13, f30, -f29)));
  // lfs f13,11200(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 11200);
  f13 = double(temp.f32);
  // fmadds f3,f12,f0,f13
  f3 = double(float(std::fma(f12, f0, f13)));
  // beq 0x820bfe8c
  if (cr0.eq) goto loc_820BFE8C;
  // cmpwi cr6,r11,-32767
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -32767, so);
  // fmr f1,f27
  f1 = f27;
  // bne cr6,0x820bfe70
  if (!cr6.eq) goto loc_820BFE70;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x820bfea4
  goto loc_820BFEA4;
loc_820BFE70:
  // cmpwi cr6,r11,32767
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 32767, so);
  // fmr f2,f31
  lifted::ScalarFloatMode();
  f2 = f31;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bne cr6,0x820bfeac
  if (!cr6.eq) goto loc_820BFEAC;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // b 0x820bfeb0
  goto loc_820BFEB0;
loc_820BFE8C:
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f0,f28
  f1 = double(float(f0 * f28));
loc_820BFEA4:
  // fmr f2,f31
  lifted::ScalarFloatMode();
  f2 = f31;
  // mr r3,r30
  r3 = r30;
loc_820BFEAC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_820BFEB0:
  // bl 0x820b5fd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B5FD8, LIFTED_ARGS(0x820BFEB4, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BFEB4:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r30
  r3 = r30;
  // rlwinm. r11,r11,0,2,2
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820bff10
  if (cr0.eq) goto loc_820BFF10;
  // lbz r11,15(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 15);
  // lfs f0,11200(r29)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 11200);
  f0 = double(temp.f32);
  // lhz r10,6(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 6);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmplwi cr6,r10,32767
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 32767, so);
  // addi r11,r11,10
  r11 = uint64_t(int64_t(int64_t(r11) + 10));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f30
  f13 = double(float(f13 * f30));
  // fmadds f0,f13,f0,f29
  f0 = double(float(std::fma(f13, f0, f29)));
  // fmuls f1,f0,f31
  f1 = double(float(f0 * f31));
  // beq cr6,0x820bff08
  if (cr6.eq) goto loc_820BFF08;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_820BFF08:
  // bl 0x820b03e0
  // (left out: dead in C++)
  f820B03E0(base, r3, r4, f1);
  // b 0x820bff68
  goto loc_820BFF68;
loc_820BFF10:
  // lbz r10,15(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 15);
  // lha r9,6(r31)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 6))));
  // extsb r11,r10
  r11 = uint64_t(int64_t(int8_t(r10)));
  // mr r10,r9
  r10 = r9;
  // addi r11,r11,10
  r11 = uint64_t(int64_t(int64_t(r11) + 10));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
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
  // fmuls f0,f0,f28
  f0 = double(float(f0 * f28));
  // fmuls f12,f13,f30
  f12 = double(float(f13 * f30));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // fsubs f11,f0,f13
  f11 = double(float(f0 - f13));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // fmadds f1,f0,f12,f13
  f1 = double(float(std::fma(f0, f12, f13)));
  // bl 0x820b3e08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B3E08, LIFTED_ARGS(0x820BFF68, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BFF68:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r30
  r3 = r30;
  // rlwinm. r11,r11,0,4,4
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820bffc4
  if (cr0.eq) goto loc_820BFFC4;
  // lbz r11,14(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 14);
  // lfs f0,11200(r29)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 11200);
  f0 = double(temp.f32);
  // lhz r10,4(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 4);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmplwi cr6,r10,32767
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 32767, so);
  // addi r11,r11,10
  r11 = uint64_t(int64_t(int64_t(r11) + 10));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f30
  f13 = double(float(f13 * f30));
  // fmadds f0,f13,f0,f29
  f0 = double(float(std::fma(f13, f0, f29)));
  // fmuls f1,f0,f31
  f1 = double(float(f0 * f31));
  // beq cr6,0x820bffbc
  if (cr6.eq) goto loc_820BFFBC;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_820BFFBC:
  // bl 0x820b0428
  // (left out: dead in C++)
  f820B0428(base, r3, r4, f1);
  // b 0x820c001c
  goto loc_820C001C;
loc_820BFFC4:
  // lbz r10,14(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 14);
  // lha r9,4(r31)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 4))));
  // extsb r11,r10
  r11 = uint64_t(int64_t(int8_t(r10)));
  // mr r10,r9
  r10 = r9;
  // addi r11,r11,10
  r11 = uint64_t(int64_t(int64_t(r11) + 10));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
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
  // fmuls f0,f0,f28
  f0 = double(float(f0 * f28));
  // fmuls f12,f13,f30
  f12 = double(float(f13 * f30));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // fsubs f11,f0,f13
  f11 = double(float(f0 - f13));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // fmadds f1,f0,f12,f13
  f1 = double(float(std::fma(f0, f12, f13)));
  // bl 0x820b3e68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B3E68, LIFTED_ARGS(0x820C001C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820C001C:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r30
  r3 = r30;
  // rlwinm. r11,r11,0,0,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820c0054
  if (cr0.eq) goto loc_820C0054;
  // lhz r11,10(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 10);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // cmplwi cr6,r11,32767
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32767, so);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8696(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8696);
  f0 = double(temp.f32);
  // fmuls f1,f31,f0
  f1 = double(float(f31 * f0));
  // beq cr6,0x820c004c
  if (cr6.eq) goto loc_820C004C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_820C004C:
  // bl 0x820b04c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B04C0, LIFTED_ARGS(0x820C0050, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c006c
  goto loc_820C006C;
loc_820C0054:
  // lha r11,10(r31)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 10))));
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f1,f0
  f1 = double(float(f0));
  // bl 0x820b3e38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B3E38, LIFTED_ARGS(0x820C006C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820C006C:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r30
  r3 = r30;
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // rlwinm. r11,r11,0,22,22
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x200;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x820c0088
  if (!cr0.eq) goto loc_820C0088;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_820C0088:
  // bl 0x820b0478
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B0478, LIFTED_ARGS(0x820C008C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,2249(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 2249);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // beq 0x820c00b0
  if (cr0.eq) goto loc_820C00B0;
  // rlwinm. r11,r11,0,18,18
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820c00d4
  if (cr0.eq) goto loc_820C00D4;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b00b8
  // (left out: dead in C++)
  f820B00B8(base, r3);
  // b 0x820c00d4
  goto loc_820C00D4;
loc_820C00B0:
  // clrlwi. r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x820c00c4
  if (cr0.eq) goto loc_820C00C4;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b0098
  // (left out: dead in C++)
  f820B0098(base, r3);
  // b 0x820c00d4
  goto loc_820C00D4;
loc_820C00C4:
  // rlwinm. r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820c00d4
  if (cr0.eq) goto loc_820C00D4;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b00a8
  // (left out: dead in C++)
  f820B00A8(base, r3);
loc_820C00D4:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // rlwinm. r11,r11,0,15,15
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820c0104
  if (cr0.eq) goto loc_820C0104;
  // lbz r11,2424(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 2424);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x820c00fc
  if (!cr0.eq) goto loc_820C00FC;
  // lfs f0,2420(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 2420);
  f0 = double(temp.f32);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // fcmpu cr6,f0,f27
  cr6 = lifted::CompareFloat(f0, f27);
  // bgt cr6,0x820c0100
  if (cr6.gt) goto loc_820C0100;
loc_820C00FC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_820C0100:
  // stb r11,2424(r30)
  REX_STORE_U8(uint32_t(r30) + 2424, uint8_t(r11));
loc_820C0104:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7960
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820C0118: lifted from generated/ by lift.py.
lifted::Result f820C0118(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820C0118 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lfs f1,6200(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 6200);
  f1 = double(temp.f32);
  // b 0x820b62e8
  { const lifted::Result o = lifted::Call(base, 0x820B62E8, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820C1D08: lifted from generated/ by lift.py.
lifted::Result f820C1D08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820C1D08 (lifted)");
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
  // lwz r11,340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 340);
  // addi r3,r11,32
  r3 = uint64_t(int64_t(int64_t(r11) + 32));
  // bl 0x820bae38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BAE38, LIFTED_ARGS(0x820C1D24, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,5232
  r11 = uint64_t(int64_t(int64_t(r31) + 5232));
  // mr r27,r3
  r27 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r28,r31,436
  r28 = uint64_t(int64_t(int64_t(r31) + 436));
  // addi r29,r31,5328
  r29 = uint64_t(int64_t(int64_t(r31) + 5328));
  // stw r11,2984(r31)
  REX_STORE_U32(uint32_t(r31) + 2984, uint32_t(r11));
loc_820C1D3C:
  // stw r29,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r29));
  // mr r5,r29
  r5 = r29;
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r27
  r3 = r27;
  // lwz r11,80(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 80);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820C1D5C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,96
  r29 = uint64_t(int64_t(int64_t(r29) + 96));
  // addi r28,r28,368
  r28 = uint64_t(int64_t(int64_t(r28) + 368));
  // cmplwi cr6,r30,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 4, so);
  // blt cr6,0x820c1d3c
  if (cr6.lt) goto loc_820C1D3C;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820C20E0: lifted from generated/ by lift.py.
lifted::Result f820C20E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820C20E0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r3,340(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 340);
  // stw r30,6148(r31)
  REX_STORE_U32(uint32_t(r31) + 6148, uint32_t(r30));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820c2114
  if (cr0.eq) goto loc_820C2114;
  // bl 0x820b9448
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B9448, LIFTED_ARGS(0x820C2110, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,340(r31)
  REX_STORE_U32(uint32_t(r31) + 340, uint32_t(r30));
loc_820C2114:
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820c2134
  if (cr0.eq) goto loc_820C2134;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820C2134, r31)); r3 = o.r3; f1 = o.f1; }
loc_820C2134:
  // stw r30,344(r31)
  REX_STORE_U32(uint32_t(r31) + 344, uint32_t(r30));
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

// sub_820C23E8: lifted from generated/ by lift.py.
lifted::Result f820C23E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_820C23E8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-80(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -80, std::bit_cast<uint64_t>(f29));
  // stfd f30,-72(r1)
  REX_STORE_U64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f30));
  // stfd f31,-64(r1)
  REX_STORE_U64(uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // stwu r1,-384(r1)
  ea = -384 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r27,r5
  r27 = r5;
  // addi r4,r30,5232
  r4 = uint64_t(int64_t(int64_t(r30) + 5232));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // li r5,96
  r5 = uint64_t(int64_t(96));
  // mr r26,r6
  r26 = r6;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820C2420, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,212(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 212);
  // lwz r3,336(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 336);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x820b67d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B67D8, LIFTED_ARGS(0x820C2430, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // std r9,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r9);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r11,8(r10)
  REX_STORE_U64(uint32_t(r10) + 8, r11);
  // bl 0x82170f90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82170F90, LIFTED_ARGS(0x820C2458, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lha r6,214(r1)
  r6 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 214))));
  // lha r5,212(r1)
  r5 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 212))));
  // bl 0x820f9768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F9768, LIFTED_ARGS(0x820C246C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // lha r6,82(r1)
  r6 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 82))));
  // lha r5,80(r1)
  r5 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 80))));
  // bl 0x820f9768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F9768, LIFTED_ARGS(0x820C2480, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,176
  r4 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,132(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // stfs f31,116(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stfs f31,180(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 180, temp.u32);
  // bl 0x82294068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294068, LIFTED_ARGS(0x820C24A4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r31,32
  r28 = uint64_t(int64_t(int64_t(r31) + 32));
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // mr r3,r28
  r3 = r28;
  // bl 0x822941b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822941B0, LIFTED_ARGS(0x820C24B4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // bl 0x82294068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294068, LIFTED_ARGS(0x820C24C8, r31)); r3 = o.r3; f1 = o.f1; }
  // fcmpu cr6,f30,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f30, f31);
  // bne cr6,0x820c24d8
  if (!cr6.eq) goto loc_820C24D8;
  // stfs f31,68(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 68, temp.u32);
  // b 0x820c24ec
  goto loc_820C24EC;
loc_820C24D8:
  // addi r4,r1,192
  r4 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294170, LIFTED_ARGS(0x820C24E4, r31)); r3 = o.r3; f1 = o.f1; }
  // fdivs f0,f1,f30
  lifted::ScalarFloatMode();
  f0 = double(float(f1 / f30));
  // stfs f0,68(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 68, temp.u32);
loc_820C24EC:
  // addi r29,r31,48
  r29 = uint64_t(int64_t(int64_t(r31) + 48));
  // lfs f1,68(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 68);
  f1 = double(temp.f32);
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x822940a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940A8, LIFTED_ARGS(0x820C2500, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r4,r29
  r4 = r29;
  // mr r3,r29
  r3 = r29;
  // bl 0x82294028
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294028, LIFTED_ARGS(0x820C2510, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,128
  r5 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82294200
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294200, LIFTED_ARGS(0x820C2520, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,192
  r5 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // stfs f1,64(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r31) + 64, temp.u32);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82294108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294108, LIFTED_ARGS(0x820C2534, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,100(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 100);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // bge cr6,0x820c2554
  if (!cr6.lt) goto loc_820C2554;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,64(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 64);
  f13 = double(temp.f32);
  // lfs f0,7136(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7136);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,64(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 64, temp.u32);
loc_820C2554:
  // lwz r3,336(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 336);
  // bl 0x820b6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6880, LIFTED_ARGS(0x820C255C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r31,16
  r29 = uint64_t(int64_t(int64_t(r31) + 16));
  // mr r4,r3
  r4 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x82293df0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82293DF0, LIFTED_ARGS(0x820C256C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r29
  r3 = r29;
  // stfs f31,20(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 20, temp.u32);
  // bl 0x822941b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822941B0, LIFTED_ARGS(0x820C257C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294170, LIFTED_ARGS(0x820C2588, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x820c25bc
  if (cr6.eq) goto loc_820C25BC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f1,11488(r11)
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 11488)));
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS(0x820C25A4, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // ble cr6,0x820c25e0
  if (!cr6.gt) goto loc_820C25E0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x820c25dc
  goto loc_820C25DC;
loc_820C25BC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f1,11480(r11)
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 11480)));
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS(0x820C25C8, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // bge cr6,0x820c25e0
  if (!cr6.lt) goto loc_820C25E0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_820C25DC:
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
loc_820C25E0:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // lfs f30,10732(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10732);
  f30 = double(temp.f32);
  // beq cr6,0x820c261c
  if (cr6.eq) goto loc_820C261C;
  // lfs f0,48(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 48);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f0,f30
  f13 = double(float(f0 * f30));
  // lfs f12,84(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 84);
  f12 = double(temp.f32);
  // lfs f0,10712(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10712);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfs f13,88(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 88);
  f13 = double(temp.f32);
  // fadds f13,f13,f0
  f13 = double(float(f13 + f0));
  // stfs f13,88(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 88, temp.u32);
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // stfs f0,84(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 84, temp.u32);
loc_820C261C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // addi r29,r11,11200
  r29 = uint64_t(int64_t(int64_t(r11) + 11200));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,8748(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8748);
  f31 = double(temp.f32);
  // beq cr6,0x820c2750
  if (cr6.eq) goto loc_820C2750;
  // lwa r9,1656(r27)
  r9 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r27) + 1656))));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,4648(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 4648);
  f0 = double(temp.f32);
  // lwa r10,1664(r27)
  r10 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r27) + 1664))));
  // lfs f13,2224(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 2224);
  f13 = double(temp.f32);
  // mr r3,r30
  r3 = r30;
  // fdivs f11,f13,f0
  f11 = double(float(f13 / f0));
  // lfs f0,92(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 92);
  f0 = double(temp.f32);
  // lfs f13,96(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 96);
  f13 = double(temp.f32);
  // std r9,144(r1)
  REX_STORE_U64(uint32_t(r1) + 144, r9);
  // lfs f9,9596(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 9596);
  f9 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // std r10,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r10);
  // lfd f10,96(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 96)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // lfs f8,7820(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7820);
  f8 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f7,11472(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11472);
  f7 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f10,f10
  f10 = double(float(f10));
  // lfs f6,11468(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11468);
  f6 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f12,144(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 144)));
  // fcfid f5,f12
  f5 = double(std::bit_cast<int64_t>(f12));
  // lfs f12,10764(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10764);
  f12 = double(temp.f32);
  // fmuls f12,f11,f12
  f12 = double(float(f11 * f12));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f11,f5
  f11 = double(float(f5));
  // fmuls f12,f12,f12
  f12 = double(float(f12 * f12));
  // fsubs f10,f11,f10
  f10 = double(float(f11 - f10));
  // fmuls f11,f12,f31
  f11 = double(float(f12 * f31));
  // lfs f12,11436(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11436);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f12,f10,f12
  f12 = double(float(f10 + f12));
  // lfs f10,9592(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 9592);
  f10 = double(temp.f32);
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadds f7,f12,f7,f10
  f7 = double(float(std::fma(f12, f7, f10)));
  // fmadds f10,f12,f6,f10
  f10 = double(float(std::fma(f12, f6, f10)));
  // lfs f12,10784(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10784);
  f12 = double(temp.f32);
  // fmadds f9,f11,f9,f12
  f9 = double(float(std::fma(f11, f9, f12)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadds f12,f11,f8,f12
  f12 = double(float(std::fma(f11, f8, f12)));
  // fsubs f11,f9,f0
  f11 = double(float(f9 - f0));
  // fsubs f12,f12,f13
  f12 = double(float(f12 - f13));
  // fmuls f11,f11,f7
  f11 = double(float(f11 * f7));
  // fmuls f12,f12,f10
  f12 = double(float(f12 * f10));
  // fmadds f0,f11,f30,f0
  f0 = double(float(std::fma(f11, f30, f0)));
  // stfs f0,92(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 92, temp.u32);
  // fmadds f0,f12,f30,f13
  f0 = double(float(std::fma(f12, f30, f13)));
  // stfs f0,96(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 96, temp.u32);
  // lfs f1,2228(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 2228);
  f1 = double(temp.f32);
  // fnmsubs f0,f1,f1,f1
  f0 = double(float(-std::fma(f1, f1, -f1)));
  // fadds f13,f0,f1
  f13 = double(float(f0 + f1));
  // lfs f0,10260(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10260);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // fsqrts f29,f0
  f29 = double(float(sqrt(f0)));
  // bl 0x820b00c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B00C8, LIFTED_ARGS(0x820C271C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,96(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 96);
  f12 = double(temp.f32);
  // lfs f13,104(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 104);
  f13 = double(temp.f32);
  // lfs f0,10148(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10148);
  f0 = double(temp.f32);
  // fsubs f12,f0,f12
  f12 = double(float(f0 - f12));
  // fdivs f0,f1,f13
  f0 = double(float(f1 / f13));
  // fsqrts f0,f0
  f0 = double(float(sqrt(f0)));
  // fmuls f13,f0,f29
  f13 = double(float(f0 * f29));
  // lfs f0,-28(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + -28);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,100(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 100, temp.u32);
loc_820C2750:
  // lfs f0,48(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 48);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f0,f30
  f13 = double(float(f0 * f30));
  // lfs f10,3544(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 3544);
  f10 = double(temp.f32);
  // lfs f0,3540(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 3540);
  f0 = double(temp.f32);
  // fsubs f0,f0,f10
  f0 = double(float(f0 - f10));
  // lfs f12,292(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 292);
  f12 = double(temp.f32);
  // lfs f11,2432(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 2432);
  f11 = double(temp.f32);
  // fmuls f10,f0,f31
  f10 = double(float(f0 * f31));
  // lfs f0,10820(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10820);
  f0 = double(temp.f32);
  // fmuls f0,f10,f0
  f0 = double(float(f10 * f0));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // lfs f13,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f13 = double(temp.f32);
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fsubs f0,f11,f0
  f0 = double(float(f11 - f0));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x820c279c
  if (!cr6.lt) goto loc_820C279C;
  // fmr f0,f13
  f0 = f13;
loc_820C279C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,2432(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 2432, temp.u32);
  // lfs f13,96(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 96);
  f13 = double(temp.f32);
  // lfs f0,11464(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11464);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfs f13,2428(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 2428);
  f13 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // lfs f13,10812(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x820c27d0
  if (!cr6.lt) goto loc_820C27D0;
  // fmr f0,f13
  f0 = f13;
loc_820C27D0:
  // stfs f0,2428(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 2428, temp.u32);
  // addi r1,r1,384
  r1 = uint64_t(int64_t(int64_t(r1) + 384));
  // lfd f29,-80(r1)
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -80)));
  // lfd f30,-72(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // lfd f31,-64(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820C27E8: lifted from generated/ by lift.py.
lifted::Result f820C27E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820C27E8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-80(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -80, std::bit_cast<uint64_t>(f29));
  // stfd f30,-72(r1)
  REX_STORE_U64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f30));
  // stfd f31,-64(r1)
  REX_STORE_U64(uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // stwu r1,-368(r1)
  ea = -368 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r4
  r27 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r3,r27
  r3 = r27;
  // bl 0x820c4550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C4550, LIFTED_ARGS(0x820C2810, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r30,r28,352
  r30 = uint64_t(int64_t(int64_t(r28) + 352));
  // addi r10,r28,1196
  r10 = uint64_t(int64_t(int64_t(r28) + 1196));
  // addi r29,r28,1288
  r29 = uint64_t(int64_t(int64_t(r28) + 1288));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // addi r11,r28,992
  r11 = uint64_t(int64_t(int64_t(r28) + 992));
  // lfs f13,16(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 16);
  f13 = double(temp.f32);
  // stfs f13,32(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 32, temp.u32);
  // lfs f13,20(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 20);
  f13 = double(temp.f32);
  // stfs f13,36(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
  // lfs f13,24(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 24);
  f13 = double(temp.f32);
  // stfs f13,40(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 40, temp.u32);
  // lfs f13,32(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 32);
  f13 = double(temp.f32);
  // stfs f13,48(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 48, temp.u32);
  // lfs f13,36(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 36);
  f13 = double(temp.f32);
  // stfs f13,52(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // lfs f13,40(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 40);
  f13 = double(temp.f32);
  // stfs f13,56(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 56, temp.u32);
  // lfs f13,596(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 596);
  f13 = double(temp.f32);
  // stfs f13,28(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // lfs f13,532(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 532);
  f13 = double(temp.f32);
  // stfs f13,0(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // lfs f13,536(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 536);
  f13 = double(temp.f32);
  // stfs f13,4(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // lfs f13,540(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 540);
  f13 = double(temp.f32);
  // stfs f13,8(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 8, temp.u32);
  // lfs f13,544(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 544);
  f13 = double(temp.f32);
  // stfs f13,12(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
  // lfs f13,48(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 48);
  f13 = double(temp.f32);
  // stfs f13,80(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 80, temp.u32);
  // lfs f13,52(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 52);
  f13 = double(temp.f32);
  // stfs f13,84(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 84, temp.u32);
  // lfs f13,56(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 56);
  f13 = double(temp.f32);
  // stfs f13,88(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 88, temp.u32);
  // lfs f13,556(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 556);
  f13 = double(temp.f32);
  // stfs f13,1264(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1264, temp.u32);
  // lfs f13,560(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 560);
  f13 = double(temp.f32);
  // stfs f13,1712(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1712, temp.u32);
  // lfs f13,564(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 564);
  f13 = double(temp.f32);
  // stfs f13,1716(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1716, temp.u32);
  // lfs f13,568(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 568);
  f13 = double(temp.f32);
  // stfs f13,1708(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1708, temp.u32);
  // lfs f13,572(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 572);
  f13 = double(temp.f32);
  // stfs f13,1704(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1704, temp.u32);
  // lfs f13,64(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 64);
  f13 = double(temp.f32);
  // stfs f13,1760(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1760, temp.u32);
  // lfs f13,68(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 68);
  f13 = double(temp.f32);
  // stfs f13,1764(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1764, temp.u32);
  // lfs f13,72(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 72);
  f13 = double(temp.f32);
  // stfs f13,1768(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1768, temp.u32);
  // stfs f0,1772(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1772, temp.u32);
  // lfs f13,80(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 80);
  f13 = double(temp.f32);
  // stfs f13,1776(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1776, temp.u32);
  // lfs f13,84(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 84);
  f13 = double(temp.f32);
  // stfs f13,1780(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1780, temp.u32);
  // lfs f13,88(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 88);
  f13 = double(temp.f32);
  // stfs f13,1784(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 1784, temp.u32);
  // stfs f0,1788(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1788, temp.u32);
  // lfs f0,576(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 576);
  f0 = double(temp.f32);
  // stfs f0,1792(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1792, temp.u32);
  // lfs f0,580(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 580);
  f0 = double(temp.f32);
  // stfs f0,1796(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1796, temp.u32);
  // lfs f0,616(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 616);
  f0 = double(temp.f32);
  // stfs f0,316(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 316, temp.u32);
  // lfs f0,620(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 620);
  f0 = double(temp.f32);
  // stfs f0,320(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 320, temp.u32);
  // lfs f0,628(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 628);
  f0 = double(temp.f32);
  // stfs f0,324(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 324, temp.u32);
  // lfs f0,624(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 624);
  f0 = double(temp.f32);
  // stfs f0,328(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 328, temp.u32);
  // lfs f0,304(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 304);
  f0 = double(temp.f32);
  // stfs f0,332(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 332, temp.u32);
  // lfs f0,112(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 112);
  f0 = double(temp.f32);
  // stfs f0,488(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 488, temp.u32);
  // lfs f0,116(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 116);
  f0 = double(temp.f32);
  // stfs f0,492(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 492, temp.u32);
  // lfs f0,96(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 96);
  f0 = double(temp.f32);
  // addi r7,r31,276
  r7 = uint64_t(int64_t(int64_t(r31) + 276));
  // stfs f0,540(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 540, temp.u32);
  // addi r8,r30,324
  r8 = uint64_t(int64_t(int64_t(r30) + 324));
  // lfs f0,96(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 96);
  f0 = double(temp.f32);
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // stfs f0,544(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 544, temp.u32);
  // lfs f0,100(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 100);
  f0 = double(temp.f32);
  // stfs f0,548(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 548, temp.u32);
  // lfs f0,136(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 136);
  f0 = double(temp.f32);
  // stfs f0,596(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 596, temp.u32);
  // lfs f0,140(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 140);
  f0 = double(temp.f32);
  // stfs f0,600(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 600, temp.u32);
  // lfs f0,144(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 144);
  f0 = double(temp.f32);
  // stfs f0,604(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 604, temp.u32);
  // lfs f0,108(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 108);
  f0 = double(temp.f32);
  // stfs f0,484(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 484, temp.u32);
  // lfs f0,104(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 104);
  f0 = double(temp.f32);
  // stfs f0,384(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 384, temp.u32);
  // lfs f0,176(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 176);
  f0 = double(temp.f32);
  // stfs f0,760(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 760, temp.u32);
  // lfs f0,180(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 180);
  f0 = double(temp.f32);
  // stfs f0,764(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 764, temp.u32);
  // lfs f0,160(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 160);
  f0 = double(temp.f32);
  // stfs f0,812(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 812, temp.u32);
  // lfs f0,160(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 160);
  f0 = double(temp.f32);
  // stfs f0,816(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 816, temp.u32);
  // lfs f0,164(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 164);
  f0 = double(temp.f32);
  // stfs f0,820(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 820, temp.u32);
  // lfs f0,200(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 200);
  f0 = double(temp.f32);
  // stfs f0,868(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 868, temp.u32);
  // lfs f0,204(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 204);
  f0 = double(temp.f32);
  // stfs f0,872(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 872, temp.u32);
  // lfs f0,208(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 208);
  f0 = double(temp.f32);
  // stfs f0,876(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 876, temp.u32);
  // lfs f0,172(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 172);
  f0 = double(temp.f32);
  // stfs f0,756(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 756, temp.u32);
  // lfs f0,168(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 168);
  f0 = double(temp.f32);
  // stfs f0,656(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 656, temp.u32);
  // lfs f0,224(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 224);
  f0 = double(temp.f32);
  // stfs f0,1720(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1720, temp.u32);
  // lfs f0,228(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 228);
  f0 = double(temp.f32);
  // stfs f0,1724(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1724, temp.u32);
  // lfs f0,232(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 232);
  f0 = double(temp.f32);
  // stfs f0,1728(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1728, temp.u32);
  // lfs f0,236(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 236);
  f0 = double(temp.f32);
  // stfs f0,1736(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1736, temp.u32);
  // lfs f0,152(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 152);
  f0 = double(temp.f32);
  // stfs f0,456(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 456, temp.u32);
  // lfs f0,156(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 156);
  f0 = double(temp.f32);
  // stfs f0,460(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 460, temp.u32);
  // lfs f0,148(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 148);
  f0 = double(temp.f32);
  // stfs f0,592(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 592, temp.u32);
  // lfs f0,240(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 240);
  f0 = double(temp.f32);
  // stfs f0,1732(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1732, temp.u32);
  // lfs f0,244(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 244);
  f0 = double(temp.f32);
  // stfs f0,1740(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1740, temp.u32);
  // lfs f0,248(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 248);
  f0 = double(temp.f32);
  // stfs f0,1744(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1744, temp.u32);
  // lfs f0,252(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 252);
  f0 = double(temp.f32);
  // stfs f0,1748(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1748, temp.u32);
  // lfs f0,256(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 256);
  f0 = double(temp.f32);
  // stfs f0,1756(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1756, temp.u32);
  // lfs f0,216(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 216);
  f0 = double(temp.f32);
  // stfs f0,728(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 728, temp.u32);
  // lfs f0,220(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 220);
  f0 = double(temp.f32);
  // stfs f0,732(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 732, temp.u32);
  // lfs f0,212(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 212);
  f0 = double(temp.f32);
  // stfs f0,864(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 864, temp.u32);
  // lfs f0,260(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 260);
  f0 = double(temp.f32);
  // stfs f0,1752(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1752, temp.u32);
  // lfs f0,584(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 584);
  f0 = double(temp.f32);
  // stfs f0,1820(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1820, temp.u32);
  // lfs f0,588(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 588);
  f0 = double(temp.f32);
  // stfs f0,1824(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1824, temp.u32);
  // lfs f0,592(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 592);
  f0 = double(temp.f32);
  // stfs f0,1828(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1828, temp.u32);
  // lwz r6,356(r30)
  r6 = REX_LOAD_U32(uint32_t(r30) + 356);
  // stw r6,172(r31)
  REX_STORE_U32(uint32_t(r31) + 172, uint32_t(r6));
  // lwz r6,360(r30)
  r6 = REX_LOAD_U32(uint32_t(r30) + 360);
  // stw r6,176(r31)
  REX_STORE_U32(uint32_t(r31) + 176, uint32_t(r6));
  // lfs f0,364(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 364);
  f0 = double(temp.f32);
  // stfs f0,180(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 180, temp.u32);
  // lfs f0,308(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 308);
  f0 = double(temp.f32);
  // stfs f0,128(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 128, temp.u32);
  // lfs f0,268(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 268);
  f0 = double(temp.f32);
  // stfs f0,132(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 132, temp.u32);
  // lfs f0,284(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 284);
  f0 = double(temp.f32);
  // stfs f0,136(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 136, temp.u32);
  // lfs f0,312(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 312);
  f0 = double(temp.f32);
  // stfs f0,148(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 148, temp.u32);
  // lfs f0,316(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 316);
  f0 = double(temp.f32);
  // stfs f0,308(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 308, temp.u32);
  // lfs f0,264(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 264);
  f0 = double(temp.f32);
  // stfs f0,156(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 156, temp.u32);
  // lfs f0,280(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 280);
  f0 = double(temp.f32);
  // stfs f0,160(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 160, temp.u32);
  // lfs f0,272(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 272);
  f0 = double(temp.f32);
  // stfs f0,140(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 140, temp.u32);
  // lfs f0,288(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 288);
  f0 = double(temp.f32);
  // stfs f0,144(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 144, temp.u32);
  // lfs f0,320(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 320);
  f0 = double(temp.f32);
  // stfs f0,312(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 312, temp.u32);
  // lfs f0,604(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 604);
  f0 = double(temp.f32);
  // stfs f0,1832(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1832, temp.u32);
  // lfs f0,608(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 608);
  f0 = double(temp.f32);
  // stfs f0,1836(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1836, temp.u32);
  // lfs f0,612(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 612);
  f0 = double(temp.f32);
  // stfs f0,1840(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1840, temp.u32);
  // lfs f0,296(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 296);
  f0 = double(temp.f32);
  // stfs f0,164(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 164, temp.u32);
  // lfs f0,300(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 300);
  f0 = double(temp.f32);
  // stfs f0,168(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 168, temp.u32);
loc_820C2B08:
  // lfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // bne 0x820c2b08
  if (!cr0.eq) goto loc_820C2B08;
  // lwz r9,368(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 368);
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // addi r26,r31,1428
  r26 = uint64_t(int64_t(int64_t(r31) + 1428));
  // addi r8,r30,384
  r8 = uint64_t(int64_t(int64_t(r30) + 384));
  // li r7,9
  r7 = uint64_t(int64_t(9));
  // stw r9,1268(r31)
  REX_STORE_U32(uint32_t(r31) + 1268, uint32_t(r9));
  // lfs f29,2612(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 2612);
  f29 = double(temp.f32);
  // mr r9,r26
  r9 = r26;
loc_820C2B40:
  // lfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x820c2b50
  if (!cr6.lt) goto loc_820C2B50;
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
loc_820C2B50:
  // stfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // lfs f0,36(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 36);
  f0 = double(temp.f32);
  // stfs f0,36(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 36, temp.u32);
  // lfs f0,72(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 72);
  f0 = double(temp.f32);
  // stfs f0,72(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 72, temp.u32);
  // lfs f0,108(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 108);
  f0 = double(temp.f32);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // stfs f0,204(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 204, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bne 0x820c2b40
  if (!cr0.eq) goto loc_820C2B40;
  // lfs f0,372(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 372);
  f0 = double(temp.f32);
  // addi r6,r31,1292
  r6 = uint64_t(int64_t(int64_t(r31) + 1292));
  // stfs f0,1536(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1536, temp.u32);
  // addi r5,r11,28
  r5 = uint64_t(int64_t(int64_t(r11) + 28));
  // lfs f0,376(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 376);
  f0 = double(temp.f32);
  // li r9,32
  r9 = uint64_t(int64_t(32));
  // stfs f0,1540(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1540, temp.u32);
  // mr r7,r6
  r7 = r6;
  // lfs f0,380(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 380);
  f0 = double(temp.f32);
  // mr r8,r5
  r8 = r5;
  // stfs f0,1544(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1544, temp.u32);
  // lfs f0,600(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 600);
  f0 = double(temp.f32);
  // stfs f0,1628(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1628, temp.u32);
  // lfs f0,12(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 12);
  f0 = double(temp.f32);
  // stfs f0,1272(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1272, temp.u32);
  // lfs f0,16(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 16);
  f0 = double(temp.f32);
  // stfs f0,1276(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1276, temp.u32);
  // lfs f0,20(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 20);
  f0 = double(temp.f32);
  // stfs f0,1280(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1280, temp.u32);
  // lfs f0,24(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 24);
  f0 = double(temp.f32);
  // stfs f0,1420(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1420, temp.u32);
  // lwz r4,8(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 8);
  // std r4,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r4);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,1288(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1288, temp.u32);
loc_820C2BE8:
  // lfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // bne 0x820c2be8
  if (!cr0.eq) goto loc_820C2BE8;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f13,1280(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1280);
  f13 = double(temp.f32);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lfs f0,11496(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 11496);
  f0 = double(temp.f32);
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // fctiwz f13,f13
  f13 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f13) ? int64_t(0x80000000U) : (f13 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f13)))));
  // stfiwx f13,0,r9
  REX_STORE_U32(uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f13)));
  // lwz r9,80(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // ble cr6,0x820c2c68
  if (!cr6.gt) goto loc_820C2C68;
  // mr r8,r6
  r8 = r6;
  // mr r9,r5
  r9 = r5;
loc_820C2C34:
  // lfs f13,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfs f13,0(r8)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // lfs f13,1280(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1280);
  f13 = double(temp.f32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // fctiwz f13,f13
  f13 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f13) ? int64_t(0x80000000U) : (f13 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f13)))));
  // stfiwx f13,0,r6
  REX_STORE_U32(uint32_t(r6), uint32_t(std::bit_cast<uint64_t>(f13)));
  // lwz r6,80(r1)
  r6 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmpw cr6,r7,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r6), so);
  // blt cr6,0x820c2c34
  if (cr6.lt) goto loc_820C2C34;
loc_820C2C68:
  // lfs f0,156(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 156);
  f0 = double(temp.f32);
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // stfs f0,1548(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1548, temp.u32);
  // addi r9,r10,24
  r9 = uint64_t(int64_t(int64_t(r10) + 24));
  // lfs f0,160(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 160);
  f0 = double(temp.f32);
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // stfs f0,1552(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1552, temp.u32);
  // lfs f0,164(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 164);
  f0 = double(temp.f32);
  // stfs f0,1556(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1556, temp.u32);
  // lfs f0,168(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 168);
  f0 = double(temp.f32);
  // stfs f0,1560(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1560, temp.u32);
  // lwz r6,172(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 172);
  // cntlzw r6,r6
  r6 = uint32_t(r6) == 0 ? 32 : __builtin_clz(uint32_t(r6));
  // rlwinm r6,r6,27,31,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 27) & 0x1;
  // xori r6,r6,1
  r6 = r6 ^ 1;
  // stb r6,1604(r31)
  REX_STORE_U8(uint32_t(r31) + 1604, uint8_t(r6));
  // lfs f0,176(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 176);
  f0 = double(temp.f32);
  // stfs f0,1616(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1616, temp.u32);
  // lfs f0,180(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 180);
  f0 = double(temp.f32);
  // stfs f0,1620(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1620, temp.u32);
  // lfs f0,184(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 184);
  f0 = double(temp.f32);
  // stfs f0,1624(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1624, temp.u32);
  // lfs f0,188(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 188);
  f0 = double(temp.f32);
  // stfs f0,1612(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1612, temp.u32);
  // lwz r6,192(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 192);
  // cntlzw r6,r6
  r6 = uint32_t(r6) == 0 ? 32 : __builtin_clz(uint32_t(r6));
  // rlwinm r6,r6,27,31,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 27) & 0x1;
  // xori r6,r6,1
  r6 = r6 ^ 1;
  // stb r6,1605(r31)
  REX_STORE_U8(uint32_t(r31) + 1605, uint8_t(r6));
  // lfs f0,196(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 196);
  f0 = double(temp.f32);
  // stfs f0,1608(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1608, temp.u32);
loc_820C2CE4:
  // lfs f0,-16(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -16);
  f0 = double(temp.f32);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // stfs f0,-16(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + -16, temp.u32);
  // lfs f0,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // bne 0x820c2ce4
  if (!cr0.eq) goto loc_820C2CE4;
  // lfs f0,44(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 44);
  f0 = double(temp.f32);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stfs f0,160(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 160, temp.u32);
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfs f0,48(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 48);
  f0 = double(temp.f32);
  // mr r3,r31
  r3 = r31;
  // stfs f0,164(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 164, temp.u32);
  // lfs f0,52(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 52);
  f0 = double(temp.f32);
  // stfs f0,168(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 168, temp.u32);
  // lfs f0,56(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 56);
  f0 = double(temp.f32);
  // stfs f0,172(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 172, temp.u32);
  // lfs f0,60(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 60);
  f0 = double(temp.f32);
  // stfs f0,176(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 176, temp.u32);
  // lfs f0,64(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 64);
  f0 = double(temp.f32);
  // stfs f0,180(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 180, temp.u32);
  // lfs f0,68(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 68);
  f0 = double(temp.f32);
  // stfs f0,184(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 184, temp.u32);
  // lfs f0,72(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 72);
  f0 = double(temp.f32);
  // stfs f0,188(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 188, temp.u32);
  // lfs f0,76(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 76);
  f0 = double(temp.f32);
  // stfs f0,192(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 192, temp.u32);
  // bl 0x820aee98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AEE98, LIFTED_ARGS(0x820C2D5C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,224
  r10 = uint64_t(int64_t(int64_t(r1) + 224));
  // addi r11,r29,24
  r11 = uint64_t(int64_t(int64_t(r29) + 24));
  // li r9,4
  r9 = uint64_t(int64_t(4));
loc_820C2D68:
  // lfs f0,-16(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16);
  f0 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stfs f0,-16(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + -16, temp.u32);
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bne 0x820c2d68
  if (!cr0.eq) goto loc_820C2D68;
  // lfs f0,44(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 44);
  f0 = double(temp.f32);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stfs f0,240(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 240, temp.u32);
  // addi r4,r1,208
  r4 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfs f0,48(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 48);
  f0 = double(temp.f32);
  // mr r3,r31
  r3 = r31;
  // stfs f0,244(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 244, temp.u32);
  // lfs f0,52(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 52);
  f0 = double(temp.f32);
  // stfs f0,248(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 248, temp.u32);
  // lfs f0,56(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 56);
  f0 = double(temp.f32);
  // stfs f0,252(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 252, temp.u32);
  // lfs f0,60(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 60);
  f0 = double(temp.f32);
  // stfs f0,256(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 256, temp.u32);
  // lfs f0,64(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 64);
  f0 = double(temp.f32);
  // stfs f0,260(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 260, temp.u32);
  // lfs f0,68(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 68);
  f0 = double(temp.f32);
  // stfs f0,264(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 264, temp.u32);
  // lfs f0,72(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 72);
  f0 = double(temp.f32);
  // stfs f0,268(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 268, temp.u32);
  // lfs f0,76(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 76);
  f0 = double(temp.f32);
  // stfs f0,272(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 272, temp.u32);
  // bl 0x820aee98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AEE98, LIFTED_ARGS(0x820C2DE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,176(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 176);
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // addi r8,r31,192
  r8 = uint64_t(int64_t(int64_t(r31) + 192));
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,6172
  r11 = uint64_t(int64_t(int64_t(r11) + 6172));
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // lwzx r11,r9,r11
  r11 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // addi r9,r11,12
  r9 = uint64_t(int64_t(int64_t(r11) + 12));
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f0 = double(temp.f32);
  // stfs f0,184(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 184, temp.u32);
  // lfs f0,8(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8);
  f0 = double(temp.f32);
  // stfs f0,188(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 188, temp.u32);
loc_820C2E10:
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // bne 0x820c2e10
  if (!cr0.eq) goto loc_820C2E10;
  // lfs f0,76(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 76);
  f0 = double(temp.f32);
  // mr r4,r27
  r4 = r27;
  // stfs f0,256(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 256, temp.u32);
  // mr r3,r31
  r3 = r31;
  // lfs f0,80(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 80);
  f0 = double(temp.f32);
  // stfs f0,260(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 260, temp.u32);
  // bl 0x820c0d28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0D28, LIFTED_ARGS(0x820C2E44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r28
  r5 = r28;
  // mr r4,r27
  r4 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x820c05c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C05C8, LIFTED_ARGS(0x820C2E54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3888(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 3888);
  // rlwinm. r11,r11,31,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820c2e7c
  if (cr0.eq) goto loc_820C2E7C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,84(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 84);
  f13 = double(temp.f32);
  // lfs f12,164(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 164);
  f12 = double(temp.f32);
  // stfs f12,168(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 168, temp.u32);
  // lfs f0,4208(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4208);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,84(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 84, temp.u32);
loc_820C2E7C:
  // lbz r11,80(r27)
  r11 = REX_LOAD_U8(uint32_t(r27) + 80);
  // fmr f0,f29
  lifted::ScalarFloatMode();
  f0 = f29;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,10568(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10568);
  f31 = double(temp.f32);
  // beq cr6,0x820c2ecc
  if (cr6.eq) goto loc_820C2ECC;
  // lfs f0,96(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 96);
  f0 = double(temp.f32);
  // fmuls f1,f0,f31
  f1 = double(float(f0 * f31));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A88A0, LIFTED_ARGS(0x820C2EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,544(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 544);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // lfs f13,388(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 388);
  f13 = double(temp.f32);
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // fneg f30,f0
  f30 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A88A0, LIFTED_ARGS(0x820C2EC0, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lfs f0,388(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 388);
  f0 = double(temp.f32);
  // fmadds f0,f13,f0,f30
  f0 = double(float(std::fma(f13, f0, f30)));
loc_820C2ECC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // lfs f12,3876(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 3876);
  f12 = double(temp.f32);
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // lfs f11,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f11 = double(temp.f32);
  // addi r3,r31,336
  r3 = uint64_t(int64_t(int64_t(r31) + 336));
  // lfs f10,1736(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1736);
  f10 = double(temp.f32);
  // stfs f10,116(r1)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // lfs f30,11200(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11200);
  f30 = double(temp.f32);
  // fnmsubs f13,f13,f30,f12
  f13 = double(float(-std::fma(f13, f30, -f12)));
  // stfs f29,124(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // fmuls f11,f11,f30
  f11 = double(float(f11 * f30));
  // stfs f11,112(r1)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820C2F0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,81(r27)
  r11 = REX_LOAD_U8(uint32_t(r27) + 81);
  // fmr f0,f29
  lifted::ScalarFloatMode();
  f0 = f29;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820c2f54
  if (cr6.eq) goto loc_820C2F54;
  // lfs f0,160(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 160);
  f0 = double(temp.f32);
  // fmuls f1,f0,f31
  f1 = double(float(f0 * f31));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A88A0, LIFTED_ARGS(0x820C2F28, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,816(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 816);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // lfs f13,660(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 660);
  f13 = double(temp.f32);
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // fneg f31,f0
  f31 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A88A0, LIFTED_ARGS(0x820C2F48, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lfs f0,660(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 660);
  f0 = double(temp.f32);
  // fmadds f0,f13,f0,f31
  f0 = double(float(std::fma(f13, f0, f31)));
loc_820C2F54:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,1756(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1756);
  f13 = double(temp.f32);
  // stfs f13,100(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f10,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f10 = double(temp.f32);
  // addi r3,r31,608
  r3 = uint64_t(int64_t(int64_t(r31) + 608));
  // lfs f12,12(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f12 = double(temp.f32);
  // lfs f11,3880(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 3880);
  f11 = double(temp.f32);
  // lfs f13,7144(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7144);
  f13 = double(temp.f32);
  // fmuls f13,f10,f13
  f13 = double(float(f10 * f13));
  // stfs f13,96(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // fnmsubs f13,f12,f30,f11
  f13 = double(float(-std::fma(f12, f30, -f11)));
  // stfs f29,108(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822940E0, LIFTED_ARGS(0x820C2F94, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,1704(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1704);
  f0 = double(temp.f32);
  // stfs f0,1808(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1808, temp.u32);
  // addi r11,r31,1564
  r11 = uint64_t(int64_t(int64_t(r31) + 1564));
  // lfs f0,1712(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1712);
  f0 = double(temp.f32);
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // stfs f0,1812(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1812, temp.u32);
  // mr r9,r26
  r9 = r26;
  // lfs f0,1716(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1716);
  f0 = double(temp.f32);
  // stfs f0,1816(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1816, temp.u32);
  // lfs f0,132(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 132);
  f0 = double(temp.f32);
  // stfs f0,264(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 264, temp.u32);
  // lfs f0,136(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 136);
  f0 = double(temp.f32);
  // stfs f0,268(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 268, temp.u32);
  // lfs f0,148(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 148);
  f0 = double(temp.f32);
  // stfs f0,272(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 272, temp.u32);
  // mtctr r10
  ctr = r10;
loc_820C2FD4:
  // lwz r10,0(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + 0);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x820c2fd4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_820C2FD4;
  // lfs f0,1420(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1420);
  f0 = double(temp.f32);
  // mr r3,r31
  r3 = r31;
  // stfs f0,1424(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1424, temp.u32);
  // lfs f0,1536(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 1536);
  f0 = double(temp.f32);
  // stfs f0,1600(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 1600, temp.u32);
  // bl 0x820af018
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AF018, LIFTED_ARGS(0x820C3000, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820af1c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AF1C0, LIFTED_ARGS(0x820C3008, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,368
  r1 = uint64_t(int64_t(int64_t(r1) + 368));
  // lfd f29,-80(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -80)));
  // lfd f30,-72(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // lfd f31,-64(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820C3608: lifted from generated/ by lift.py.
lifted::Result f820C3608(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_820C3608 (lifted)");
  uint64_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v1{}, v2{}, v3{}, v4{}, v5{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{};
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
  // bl 0x825a7910
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-560(r1)
  ea = -560 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // fmr f26,f1
  lifted::ScalarFloatMode();
  f26 = f1;
  // mr r30,r3
  r30 = r3;
  // fmr f30,f2
  f30 = f2;
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r28,r6
  r28 = r6;
  // mr r27,r7
  r27 = r7;
  // addi r26,r30,5232
  r26 = uint64_t(int64_t(int64_t(r30) + 5232));
  // stw r11,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r11));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // stw r11,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r11));
  // bl 0x82170f90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82170F90, LIFTED_ARGS(0x820C3658, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x82170f90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82170F90, LIFTED_ARGS(0x820C3668, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r26,4
  r10 = uint64_t(int64_t(int64_t(r26) + 4));
  // mr r3,r26
  r3 = r26;
  // lfs f27,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f27 = double(temp.f32);
  // mr r11,r10
  r11 = r10;
  // lhz r10,0(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 0);
  // fcmpu cr6,f30,f27
  cr6 = lifted::CompareFloat(f30, f27);
  // sth r10,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r10));
  // lhz r10,2(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 2);
  // sth r10,2(r11)
  REX_STORE_U16(uint32_t(r11) + 2, uint16_t(r10));
  // bne cr6,0x820c369c
  if (!cr6.eq) goto loc_820C369C;
  // bl 0x820f8578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8578, LIFTED_ARGS(0x820C3698, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c36b0
  goto loc_820C36B0;
loc_820C369C:
  // bl 0x820f8578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8578, LIFTED_ARGS(0x820C36A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // addi r3,r26,16
  r3 = uint64_t(int64_t(int64_t(r26) + 16));
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // bl 0x820fa8f8
  // (left out: dead in C++)
  { const lifted::Result o = f820FA8F8(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_820C36B0:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fcmpu cr6,f30,f27
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f30, f27);
  // addi r3,r1,240
  r3 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfs f31,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // stfs f31,252(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 252, temp.u32);
  // stfs f31,268(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 268, temp.u32);
  // stfs f31,204(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 204, temp.u32);
  // stfs f31,220(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 220, temp.u32);
  // bne cr6,0x820c372c
  if (!cr6.eq) goto loc_820C372C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lha r6,82(r1)
  r6 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 82))));
  // lha r5,80(r1)
  r5 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 80))));
  // bl 0x820f9768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F9768, LIFTED_ARGS(0x820C36E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // lha r6,146(r1)
  r6 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 146))));
  // lha r5,144(r1)
  r5 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 144))));
  // bl 0x820f9768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F9768, LIFTED_ARGS(0x820C36F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lha r31,178(r1)
  r31 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 178))));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lha r29,176(r1)
  r29 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 176))));
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // mr r6,r31
  r6 = r31;
  // mr r5,r29
  r5 = r29;
  // bl 0x820f9768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F9768, LIFTED_ARGS(0x820C3714, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r31
  r6 = r31;
  // mr r5,r29
  r5 = r29;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x820f9768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F9768, LIFTED_ARGS(0x820C3728, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c3770
  goto loc_820C3770;
loc_820C372C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // lfs f29,9844(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 9844);
  f29 = double(temp.f32);
  // fadds f1,f30,f29
  f1 = double(float(f30 + f29));
  // bl 0x820fa8f8
  // (left out: dead in C++)
  { const lifted::Result o = f820FA8F8(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // fsubs f1,f30,f29
  lifted::ScalarFloatMode();
  f1 = double(float(f30 - f29));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x820fa8f8
  // (left out: dead in C++)
  { const lifted::Result o = f820FA8F8(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // bl 0x820fa8f8
  // (left out: dead in C++)
  { const lifted::Result o = f820FA8F8(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // bl 0x820fa8f8
  // (left out: dead in C++)
  { const lifted::Result o = f820FA8F8(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_820C3770:
  // lfs f0,240(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 240);
  f0 = double(temp.f32);
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // lfs f13,256(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 256);
  f13 = double(temp.f32);
  // addi r9,r1,112
  r9 = uint64_t(int64_t(int64_t(r1) + 112));
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stfs f31,172(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 172, temp.u32);
  // beq cr6,0x820c3814
  if (cr6.eq) goto loc_820C3814;
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // lfs f13,260(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 260);
  f13 = double(temp.f32);
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f0,244(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 244);
  f0 = double(temp.f32);
  // addi r10,r1,160
  r10 = uint64_t(int64_t(int64_t(r1) + 160));
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // lfs f13,264(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 264);
  f13 = double(temp.f32);
  // lfs f0,248(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 248);
  f0 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // lfs f13,208(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 208);
  f13 = double(temp.f32);
  // ld r7,0(r11)
  r7 = REX_LOAD_U64(uint32_t(r11) + 0);
  // lfs f0,192(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 192);
  f0 = double(temp.f32);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,160(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 160, temp.u32);
  // lfs f13,212(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 212);
  f13 = double(temp.f32);
  // lfs f0,196(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 196);
  f0 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,164(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 164, temp.u32);
  // lfs f0,200(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 200);
  f0 = double(temp.f32);
  // std r7,0(r9)
  REX_STORE_U64(uint32_t(r9) + 0, r7);
  // lfs f13,216(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 216);
  f13 = double(temp.f32);
  // std r11,8(r9)
  REX_STORE_U64(uint32_t(r9) + 8, r11);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,168(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 168, temp.u32);
  // ld r6,0(r10)
  r6 = REX_LOAD_U64(uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r6,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r6);
  // std r10,8(r8)
  REX_STORE_U64(uint32_t(r8) + 8, r10);
  // b 0x820c3894
  goto loc_820C3894;
loc_820C3814:
  // fsubs f0,f0,f13
  lifted::ScalarFloatMode();
  f0 = double(float(f0 - f13));
  // stfs f0,160(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 160, temp.u32);
  // lfs f13,260(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 260);
  f13 = double(temp.f32);
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfs f0,244(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 244);
  f0 = double(temp.f32);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,164(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 164, temp.u32);
  // lfs f13,264(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 264);
  f13 = double(temp.f32);
  // lfs f0,248(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 248);
  f0 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,168(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 168, temp.u32);
  // lfs f13,208(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 208);
  f13 = double(temp.f32);
  // ld r7,0(r11)
  r7 = REX_LOAD_U64(uint32_t(r11) + 0);
  // lfs f0,192(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 192);
  f0 = double(temp.f32);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // lfs f13,212(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 212);
  f13 = double(temp.f32);
  // lfs f0,196(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 196);
  f0 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // lfs f0,200(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 200);
  f0 = double(temp.f32);
  // std r7,0(r9)
  REX_STORE_U64(uint32_t(r9) + 0, r7);
  // lfs f13,216(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 216);
  f13 = double(temp.f32);
  // std r11,8(r9)
  REX_STORE_U64(uint32_t(r9) + 8, r11);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // ld r6,0(r10)
  r6 = REX_LOAD_U64(uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r6,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r6);
  // std r10,8(r8)
  REX_STORE_U64(uint32_t(r8) + 8, r10);
loc_820C3894:
  // stfs f31,124(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // stfs f31,140(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
  // vspltisw v10,1
  simde_mm_store_si128((simde__m128i*)v10.u32, simde_mm_set1_epi32(int(0x1)));
  // vspltisw v9,0
  simde_mm_store_si128((simde__m128i*)v9.u32, simde_mm_set1_epi32(int(0x0)));
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // addi r6,r1,112
  r6 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r5,r1,128
  r5 = uint64_t(int64_t(int64_t(r1) + 128));
  // lvx128 v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // vmsum3fp128 v12,v13,v13
  lifted::VectorFloatMode();
  simde_mm_store_ps(v12.f32, simde_mm_dp_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32), 0xEF));
  // vpermwi128 v5,v13,135
  simde_mm_store_si128((simde__m128i*)v5.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // vcfsx v7,v10,1
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v10.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vpermwi128 v6,v13,99
  simde_mm_store_si128((simde__m128i*)v6.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r3,r1,352
  r3 = uint64_t(int64_t(int64_t(r1) + 352));
  // lvx128 v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // vpermwi128 v4,v0,99
  simde_mm_store_si128((simde__m128i*)v4.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // vpermwi128 v3,v0,135
  simde_mm_store_si128((simde__m128i*)v3.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vmulfp128 v0,v4,v5
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v4.f32), simde_mm_load_ps(v5.f32)));
  // vrsqrtefp v11,v12
  simde_mm_store_ps(v11.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v12.f32))));
  // vmulfp128 v2,v12,v7
  simde_mm_store_ps(v2.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v7.f32)));
  // vcmpeqfp v8,v12,v9
  simde_mm_store_ps(v8.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v9.f32)));
  // vnmsubfp v0,v3,v6,v0
  simde_mm_store_ps(v0.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v3.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v0.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmulfp128 v4,v11,v11
  simde_mm_store_ps(v4.f32, simde_mm_mul_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v11.f32)));
  // vpermwi128 v3,v0,99
  simde_mm_store_si128((simde__m128i*)v3.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // vnmsubfp v7,v2,v4,v7
  simde_mm_store_ps(v7.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v2.f32), simde_mm_load_ps(v4.f32)), simde_mm_load_ps(v7.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vpermwi128 v4,v0,135
  simde_mm_store_si128((simde__m128i*)v4.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vmulfp128 v6,v6,v4
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v6.f32), simde_mm_load_ps(v4.f32)));
  // vmaddfp v11,v11,v7,v11
  simde_mm_store_ps(v11.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v7.f32)), simde_mm_load_ps(v11.f32)));
  // vcfsx v7,v10,1
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v10.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vnmsubfp v6,v5,v3,v6
  simde_mm_store_ps(v6.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v5.f32), simde_mm_load_ps(v3.f32)), simde_mm_load_ps(v6.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vcfsx v5,v10,1
  simde_mm_store_ps(v5.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v10.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vmulfp128 v4,v13,v11
  simde_mm_store_ps(v4.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v11.f32)));
  // vmsum3fp128 v11,v0,v0
  simde_mm_store_ps(v11.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // vmsum3fp128 v13,v6,v6
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v6.f32), simde_mm_load_ps(v6.f32), 0xEF));
  // vsel v8,v4,v12,v8
  simde_mm_store_si128((simde__m128i*)v8.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)v4.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)v12.u8))));
  // vrsqrtefp v10,v11
  simde_mm_store_ps(v10.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v11.f32))));
  // stvx v8,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmulfp128 v3,v11,v5
  simde_mm_store_ps(v3.f32, simde_mm_mul_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v5.f32)));
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfs f0,124(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 124);
  f0 = double(temp.f32);
  // stfs f0,172(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 172, temp.u32);
  // vrsqrtefp v12,v13
  lifted::VectorFloatMode();
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v13.f32))));
  // lfs f30,112(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 112);
  f30 = double(temp.f32);
  // vmulfp128 v4,v13,v7
  lifted::VectorFloatMode();
  simde_mm_store_ps(v4.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v7.f32)));
  // lfs f29,116(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 116);
  f29 = double(temp.f32);
  // vcmpeqfp v8,v13,v9
  lifted::VectorFloatMode();
  simde_mm_store_ps(v8.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v9.f32)));
  // lfs f28,120(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 120);
  f28 = double(temp.f32);
  // vcmpeqfp v9,v11,v9
  lifted::VectorFloatMode();
  simde_mm_store_ps(v9.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v9.f32)));
  // stfs f30,160(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 160, temp.u32);
  // stfs f29,164(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + 164, temp.u32);
  // stfs f28,168(r1)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r1) + 168, temp.u32);
  // vmulfp128 v1,v10,v10
  lifted::VectorFloatMode();
  simde_mm_store_ps(v1.f32, simde_mm_mul_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v10.f32)));
  // vmulfp128 v2,v12,v12
  simde_mm_store_ps(v2.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v5,v3,v1,v5
  simde_mm_store_ps(v5.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v3.f32), simde_mm_load_ps(v1.f32)), simde_mm_load_ps(v5.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vnmsubfp v7,v4,v2,v7
  simde_mm_store_ps(v7.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v4.f32), simde_mm_load_ps(v2.f32)), simde_mm_load_ps(v7.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v7,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v7.f32)), simde_mm_load_ps(v12.f32)));
  // vmulfp128 v12,v6,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v6.f32), simde_mm_load_ps(v12.f32)));
  // vsel v8,v12,v13,v8
  simde_mm_store_si128((simde__m128i*)v8.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)v13.u8))));
  // vmaddfp v13,v10,v5,v10
  simde_mm_store_ps(v13.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v10.f32), simde_mm_load_ps(v5.f32)), simde_mm_load_ps(v10.f32)));
  // stvx v8,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lfs f0,128(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 128);
  f0 = double(temp.f32);
  // addi r7,r11,-32048
  r7 = uint64_t(int64_t(int64_t(r11) + -32048));
  // stfs f0,112(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f0,132(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 132);
  f0 = double(temp.f32);
  // stfs f0,116(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // lfs f0,136(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 136);
  f0 = double(temp.f32);
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // lfs f0,140(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 140);
  f0 = double(temp.f32);
  // vmulfp128 v0,v0,v13
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v13.f32)));
  // stfs f0,124(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // vsel v9,v0,v11,v9
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)v0.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)v11.u8))));
  // stvx v9,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f0,96(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 96);
  f0 = double(temp.f32);
  // stfs f0,128(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 128, temp.u32);
  // lfs f0,100(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 100);
  f0 = double(temp.f32);
  // stfs f0,132(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // lfs f0,104(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 104);
  f0 = double(temp.f32);
  // stfs f0,136(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 136, temp.u32);
  // lfs f0,108(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 108);
  f0 = double(temp.f32);
  // stfs f0,140(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
  // bl 0x820bfbb8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BFBB8, LIFTED_ARGS(0x820C39F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,320
  r3 = uint64_t(int64_t(int64_t(r1) + 320));
  // bl 0x82293ad8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82293AD8, LIFTED_ARGS(0x820C3A00, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,320
  r4 = uint64_t(int64_t(int64_t(r1) + 320));
  // lwz r3,336(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 336);
  // bl 0x820b71e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B71E8, LIFTED_ARGS(0x820C3A0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x820b13c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B13C8, LIFTED_ARGS(0x820C3A14, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,340(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 340);
  // bl 0x820b7998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7998, LIFTED_ARGS(0x820C3A1C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,272
  r4 = uint64_t(int64_t(int64_t(r1) + 272));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // stfs f30,272(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 272, temp.u32);
  // stfs f29,276(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + 276, temp.u32);
  // fmr f1,f26
  f1 = f26;
  // stfs f28,280(r1)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r1) + 280, temp.u32);
  // stfs f31,284(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 284, temp.u32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294430, LIFTED_ARGS(0x820C3A3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f1,7132(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7132);
  f1 = double(temp.f32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294430, LIFTED_ARGS(0x820C3A50, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,336
  r3 = uint64_t(int64_t(int64_t(r1) + 336));
  // lfs f1,11512(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11512);
  f1 = double(temp.f32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294430, LIFTED_ARGS(0x820C3A64, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,336
  r4 = uint64_t(int64_t(int64_t(r1) + 336));
  // lwz r3,336(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 336);
  // bl 0x820b7238
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7238, LIFTED_ARGS(0x820C3A70, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f27,236(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 236, temp.u32);
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // blt cr6,0x820c3a98
  if (cr6.lt) goto loc_820C3A98;
  // addi r3,r11,23312
  r3 = uint64_t(int64_t(int64_t(r11) + 23312));
  // mr r6,r28
  r6 = r28;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,224
  r4 = uint64_t(int64_t(int64_t(r1) + 224));
  // bl 0x820f8668
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8668, LIFTED_ARGS(0x820C3A94, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c3ad4
  goto loc_820C3AD4;
loc_820C3A98:
  // lhz r10,80(r1)
  r10 = REX_LOAD_U16(uint32_t(r1) + 80);
  // lwz r11,23312(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23312);
  // rotlwi r9,r10,2
  r9 = __builtin_rotateleft32(uint32_t(r10), 2);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwzx r11,r9,r11
  r11 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // lha r10,82(r1)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 82))));
  // mulli r10,r10,84
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(84))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // lfs f0,124(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 124);
  f0 = double(temp.f32);
  // stfs f0,224(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 224, temp.u32);
  // lfs f0,128(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 128);
  f0 = double(temp.f32);
  // stfs f0,228(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 228, temp.u32);
  // lfs f0,132(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 132);
  f0 = double(temp.f32);
  // stfs f0,232(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 232, temp.u32);
  // stfs f31,236(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 236, temp.u32);
loc_820C3AD4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,224(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 224);
  f0 = double(temp.f32);
  // stfs f0,288(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 288, temp.u32);
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // lfs f13,228(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 228);
  f13 = double(temp.f32);
  // lwz r3,336(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 336);
  // stfs f27,300(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 300, temp.u32);
  // lfs f0,10604(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10604);
  f0 = double(temp.f32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,292(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 292, temp.u32);
  // lfs f0,232(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 232);
  f0 = double(temp.f32);
  // stfs f0,296(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 296, temp.u32);
  // bl 0x820b6710
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6710, LIFTED_ARGS(0x820C3B08, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x820b13c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B13C8, LIFTED_ARGS(0x820C3B10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,340(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 340);
  // bl 0x820b7998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7998, LIFTED_ARGS(0x820C3B18, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r30,5328
  r29 = uint64_t(int64_t(int64_t(r30) + 5328));
  // li r31,4
  r31 = uint64_t(int64_t(4));
loc_820C3B20:
  // addi r3,r1,352
  r3 = uint64_t(int64_t(int64_t(r1) + 352));
  // li r5,96
  r5 = uint64_t(int64_t(96));
  // mr r4,r26
  r4 = r26;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820C3B30, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,352
  r4 = uint64_t(int64_t(int64_t(r1) + 352));
  // mr r3,r29
  r3 = r29;
  // bl 0x820c2150
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C2150, LIFTED_ARGS(0x820C3B3C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r29,r29,96
  r29 = uint64_t(int64_t(int64_t(r29) + 96));
  // bne 0x820c3b20
  if (!cr0.eq) goto loc_820C3B20;
  // addi r4,r1,304
  r4 = uint64_t(int64_t(int64_t(r1) + 304));
  // lwz r3,336(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 336);
  // stfs f27,304(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 304, temp.u32);
  // stfs f27,308(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 308, temp.u32);
  // stfs f27,312(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 312, temp.u32);
  // stfs f27,316(r1)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r1) + 316, temp.u32);
  // bl 0x820b7500
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B7500, LIFTED_ARGS(0x820C3B64, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,560
  r1 = uint64_t(int64_t(int64_t(r1) + 560));
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a795c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820C3B78: lifted from generated/ by lift.py.
lifted::Result f820C3B78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820C3B78 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r31,11828(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 11828);
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x820c3bac
  if (cr6.eq) goto loc_820C3BAC;
  // mr r3,r31
  r3 = r31;
  // bl 0x820c21e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C21E0, LIFTED_ARGS(0x820C3BA4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820C3BAC, r31)); r3 = o.r3; f1 = o.f1; }
loc_820C3BAC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,11828(r30)
  REX_STORE_U32(uint32_t(r30) + 11828, uint32_t(r11));
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

// sub_820C3BD0: lifted from generated/ by lift.py.
lifted::Result f820C3BD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820C3BD0 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r3,0
  r3 = uint64_t(int64_t(0));
  // ori r3,r3,43904
  r3 = r3 | 43904;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x820C3BE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r29,r3
  r29 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x820c3c14
  if (cr0.eq) goto loc_820C3C14;
  // mr r30,r29
  r30 = r29;
  // li r31,6
  r31 = uint64_t(int64_t(6));
loc_820C3BF8:
  // mr r3,r30
  r3 = r30;
  // bl 0x820c30d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C30D8, LIFTED_ARGS(0x820C3C00, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r30,r30,6272
  r30 = uint64_t(int64_t(int64_t(r30) + 6272));
  // bge 0x820c3bf8
  if (!cr0.lt) goto loc_820C3BF8;
  // mr r3,r29
  r3 = r29;
  // b 0x820c3c18
  goto loc_820C3C18;
loc_820C3C14:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820C3C18:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // addi r11,r11,11828
  r11 = uint64_t(int64_t(int64_t(r11) + 11828));
  // ori r5,r5,43904
  r5 = r5 | 43904;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820C3C34, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820C5C88: lifted from generated/ by lift.py.
lifted::Result f820C5C88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820C5C88 (lifted)");
  uint64_t r0 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r4,r5
  r4 = r5;
  // addi r11,r30,412
  r11 = uint64_t(int64_t(int64_t(r30) + 412));
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r3,r11,r3
  r3 = r11 + r3;
  // bl 0x82593fc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593FC0, LIFTED_ARGS(0x820C5CB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x820c5d58
  if (!cr6.eq) goto loc_820C5D58;
  // cmplwi cr6,r30,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 7, so);
  // bgt cr6,0x820c5d50
  if (cr6.gt) goto loc_820C5D50;
  // lis r12,-32256
  r12 = uint64_t(int64_t(-2113929216));
  // addi r12,r12,12176
  r12 = uint64_t(int64_t(int64_t(r12) + 12176));
  // lbzx r0,r12,r30
  r0 = REX_LOAD_U8(uint32_t(r12) + uint32_t(r30));
  // lis r12,-32244
  r12 = uint64_t(int64_t(-2113142784));
  // addi r12,r12,23796
  r12 = uint64_t(int64_t(int64_t(r12) + 23796));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r30)) {
  case 0:
  goto loc_820C5CF4;
  case 1:
  goto loc_820C5D00;
  case 2:
  goto loc_820C5D18;
  case 3:
  goto loc_820C5D0C;
  case 4:
  goto loc_820C5D24;
  case 5:
  goto loc_820C5D30;
  case 6:
  goto loc_820C5D3C;
  case 7:
  goto loc_820C5D48;
  default:
  std::abort();
  }
loc_820C5CF4:
  // mr r3,r31
  r3 = r31;
  // bl 0x820db6c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820DB6C0, LIFTED_ARGS(0x820C5CFC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c5d50
  goto loc_820C5D50;
loc_820C5D00:
  // mr r3,r31
  r3 = r31;
  // bl 0x820e0590
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820E0590, LIFTED_ARGS(0x820C5D08, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c5d50
  goto loc_820C5D50;
loc_820C5D0C:
  // mr r3,r31
  r3 = r31;
  // bl 0x820e3890
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820E3890, LIFTED_ARGS(0x820C5D14, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c5d50
  goto loc_820C5D50;
loc_820C5D18:
  // mr r3,r31
  r3 = r31;
  // bl 0x820dbf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820DBF88, LIFTED_ARGS(0x820C5D20, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c5d50
  goto loc_820C5D50;
loc_820C5D24:
  // mr r3,r31
  r3 = r31;
  // bl 0x820dcb20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820DCB20, LIFTED_ARGS(0x820C5D2C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c5d50
  goto loc_820C5D50;
loc_820C5D30:
  // mr r3,r31
  r3 = r31;
  // bl 0x820e89b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820E89B0, LIFTED_ARGS(0x820C5D38, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c5d50
  goto loc_820C5D50;
loc_820C5D3C:
  // mr r3,r31
  r3 = r31;
  // bl 0x820e5a18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820E5A18, LIFTED_ARGS(0x820C5D44, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820c5d50
  goto loc_820C5D50;
loc_820C5D48:
  // mr r3,r31
  r3 = r31;
  // bl 0x820da5d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820DA5D0, LIFTED_ARGS(0x820C5D50, r31)); r3 = o.r3; f1 = o.f1; }
loc_820C5D50:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r11));
loc_820C5D58:
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

// sub_820CC470: lifted from generated/ by lift.py.
void f820CC470(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820CC470 (lifted)");
  // stw r4,20(r3)
  REX_STORE_U32(uint32_t(r3) + 20, uint32_t(r4));
  // blr
  return;
}

// sub_820D09A0: lifted from generated/ by lift.py.
lifted::Result f820D09A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820D09A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // b 0x82295330
  { const lifted::Result o = lifted::Call(base, 0x82295330, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820D3318: lifted from generated/ by lift.py.
uint64_t f820D3318(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820D3318 (lifted)");
  uint64_t r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r7
  r28 = r7;
  // mr r27,r8
  r27 = r8;
  // mr r26,r9
  r26 = r9;
  // mr r25,r10
  r25 = r10;
  // mr r23,r3
  r23 = r3;
  // mr r31,r4
  r31 = r4;
  // std r28,320(r1)
  REX_STORE_U64(uint32_t(r1) + 320, r28);
  // mr r30,r5
  r30 = r5;
  // std r27,328(r1)
  REX_STORE_U64(uint32_t(r1) + 328, r27);
  // mr r29,r6
  r29 = r6;
  // std r26,336(r1)
  REX_STORE_U64(uint32_t(r1) + 336, r26);
  // std r25,344(r1)
  REX_STORE_U64(uint32_t(r1) + 344, r25);
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x820d34b0
  if (cr6.eq) goto loc_820D34B0;
  // lwz r11,388(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 388);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x820d34b0
  if (!cr6.eq) goto loc_820D34B0;
  // lwz r18,412(r23)
  r18 = REX_LOAD_U32(uint32_t(r23) + 412);
  // li r21,0
  r21 = uint64_t(int64_t(0));
  // cmpwi r18,0
  cr0 = lifted::Compare<int32_t>(int32_t(r18), 0, so);
  // ble 0x820d34b0
  if (!cr0.gt) goto loc_820D34B0;
  // lwz r17,404(r1)
  r17 = REX_LOAD_U32(uint32_t(r1) + 404);
  // lwz r19,396(r1)
  r19 = REX_LOAD_U32(uint32_t(r1) + 396);
  // ld r24,352(r1)
  r24 = REX_LOAD_U64(uint32_t(r1) + 352);
loc_820D3384:
  // mr r4,r21
  r4 = r21;
  // mr r3,r23
  r3 = r23;
  // bl 0x82594ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594AC0, LIFTED_ARGS(0x820D3390, r31)); r3 = o.r3; f1 = o.f1; }
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // mr. r20,r3
  r20 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r20), 0, so);
  // ble 0x820d34a4
  if (!cr0.gt) goto loc_820D34A4;
loc_820D339C:
  // mr r5,r22
  r5 = r22;
  // mr r4,r21
  r4 = r21;
  // mr r3,r23
  r3 = r23;
  // bl 0x82594b00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594B00, LIFTED_ARGS(0x820D33AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmpwi cr6,r19,0
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 0, so);
  // blt cr6,0x820d3400
  if (cr6.lt) goto loc_820D3400;
  // lwz r10,408(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 408);
  // cmpw cr6,r19,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r19), int32_t(r10), so);
  // bne cr6,0x820d3400
  if (!cr6.eq) goto loc_820D3400;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // addi r4,r1,360
  r4 = uint64_t(int64_t(int64_t(r1) + 360));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820D33D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r28
  r6 = r28;
  // mr r7,r27
  r7 = r27;
  // mr r8,r26
  r8 = r26;
  // mr r9,r25
  r9 = r25;
  // mr r10,r24
  r10 = r24;
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // mr r5,r29
  r5 = r29;
  // bl 0x820d3250
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3250, LIFTED_ARGS(0x820D33FC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820d3498
  goto loc_820D3498;
loc_820D3400:
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // blt cr6,0x820d3450
  if (cr6.lt) goto loc_820D3450;
  // lwz r10,392(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 392);
  // cmplw cr6,r17,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r17), uint32_t(r10), so);
  // bne cr6,0x820d3450
  if (!cr6.eq) goto loc_820D3450;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // addi r4,r1,360
  r4 = uint64_t(int64_t(int64_t(r1) + 360));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820D3428, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r28
  r6 = r28;
  // mr r7,r27
  r7 = r27;
  // mr r8,r26
  r8 = r26;
  // mr r9,r25
  r9 = r25;
  // mr r10,r24
  r10 = r24;
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // mr r5,r29
  r5 = r29;
  // bl 0x820d3250
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3250, LIFTED_ARGS(0x820D344C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820d3498
  goto loc_820D3498;
loc_820D3450:
  // cmpwi cr6,r19,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r19), -1, so);
  // blt cr6,0x820d3498
  if (cr6.lt) goto loc_820D3498;
  // cmpwi cr6,r17,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r17), -1, so);
  // bne cr6,0x820d3498
  if (!cr6.eq) goto loc_820D3498;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // addi r4,r1,360
  r4 = uint64_t(int64_t(int64_t(r1) + 360));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820D3474, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r28
  r6 = r28;
  // mr r7,r27
  r7 = r27;
  // mr r8,r26
  r8 = r26;
  // mr r9,r25
  r9 = r25;
  // mr r10,r24
  r10 = r24;
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // mr r5,r29
  r5 = r29;
  // bl 0x820d3250
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3250, LIFTED_ARGS(0x820D3498, r31)); r3 = o.r3; f1 = o.f1; }
loc_820D3498:
  // addi r22,r22,1
  r22 = uint64_t(int64_t(int64_t(r22) + 1));
  // cmpw cr6,r22,r20
  cr6 = lifted::Compare<int32_t>(int32_t(r22), int32_t(r20), so);
  // blt cr6,0x820d339c
  if (cr6.lt) goto loc_820D339C;
loc_820D34A4:
  // addi r21,r21,1
  r21 = uint64_t(int64_t(int64_t(r21) + 1));
  // cmpw cr6,r21,r18
  cr6 = lifted::Compare<int32_t>(int32_t(r21), int32_t(r18), so);
  // blt cr6,0x820d3384
  if (cr6.lt) goto loc_820D3384;
loc_820D34B0:
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6eac
  // (left out: dead in C++)
  return r3;
}

// sub_820D3688: lifted from generated/ by lift.py.
double f820D3688(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820D3688 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f1 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // bne cr6,0x820d369c
  if (!cr6.eq) goto loc_820D369C;
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // blr
  return f1;
loc_820D369C:
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // cmplwi cr6,r4,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 1, so);
  // blt cr6,0x820d36d0
  if (cr6.lt) goto loc_820D36D0;
  // beq cr6,0x820d36c8
  if (cr6.eq) goto loc_820D36C8;
  // cmplwi cr6,r4,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 3, so);
  // blt cr6,0x820d36c0
  if (cr6.lt) goto loc_820D36C0;
  // bne cr6,0x820d36d4
  if (!cr6.eq) goto loc_820D36D4;
  // lfs f0,720(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 720);
  f0 = double(temp.f32);
  // b 0x820d36d4
  goto loc_820D36D4;
loc_820D36C0:
  // lfs f0,716(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 716);
  f0 = double(temp.f32);
  // b 0x820d36d4
  goto loc_820D36D4;
loc_820D36C8:
  // lfs f0,712(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 712);
  f0 = double(temp.f32);
  // b 0x820d36d4
  goto loc_820D36D4;
loc_820D36D0:
  // lfs f0,704(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 704);
  f0 = double(temp.f32);
loc_820D36D4:
  // lfs f13,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x820d36e8
  if (!cr6.gt) goto loc_820D36E8;
  // lfs f13,8(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f13 = double(temp.f32);
  // b 0x820d3718
  goto loc_820D3718;
loc_820D36E8:
  // lfs f12,4(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f0
  cr6 = lifted::CompareFloat(f12, f0);
  // bge cr6,0x820d36fc
  if (!cr6.lt) goto loc_820D36FC;
  // lfs f13,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f13 = double(temp.f32);
  // b 0x820d3718
  goto loc_820D3718;
loc_820D36FC:
  // fsubs f0,f0,f13
  lifted::ScalarFloatMode();
  f0 = double(float(f0 - f13));
  // lfs f11,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f11 = double(temp.f32);
  // fsubs f13,f12,f13
  f13 = double(float(f12 - f13));
  // lfs f12,8(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f12 = double(temp.f32);
  // fsubs f12,f11,f12
  f12 = double(float(f11 - f12));
  // fdivs f0,f0,f13
  f0 = double(float(f0 / f13));
  // fmuls f13,f0,f12
  f13 = double(float(f0 * f12));
loc_820D3718:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10568(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10568);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // blr
  return f1;
}

// sub_820D3740: lifted from generated/ by lift.py.
void f820D3740(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_820D3740 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,0(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 0);
  // lfs f0,696(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 696);
  f0 = double(temp.f32);
  // lfs f8,700(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 700);
  f8 = double(temp.f32);
  // lfs f7,24(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 24);
  f7 = double(temp.f32);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lfs f6,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f6 = double(temp.f32);
  // bne cr6,0x820d3768
  if (!cr6.eq) goto loc_820D3768;
  // lfs f12,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f12 = double(temp.f32);
  // lfs f9,4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f9 = double(temp.f32);
  // b 0x820d3770
  goto loc_820D3770;
loc_820D3768:
  // lfs f12,8(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f12 = double(temp.f32);
  // lfs f9,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f9 = double(temp.f32);
loc_820D3770:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,16(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 16);
  f13 = double(temp.f32);
  // lfs f10,20(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f10 = double(temp.f32);
  // lfs f11,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f11 = double(temp.f32);
  // fcmpu cr6,f0,f11
  cr6 = lifted::CompareFloat(f0, f11);
  // bge cr6,0x820d3790
  if (!cr6.lt) goto loc_820D3790;
  // stfs f11,0(r6)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // b 0x820d37cc
  goto loc_820D37CC;
loc_820D3790:
  // fcmpu cr6,f9,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f9, f0);
  // bge cr6,0x820d37a0
  if (!cr6.lt) goto loc_820D37A0;
  // stfs f10,0(r6)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // b 0x820d37cc
  goto loc_820D37CC;
loc_820D37A0:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x820d37b4
  if (!cr6.lt) goto loc_820D37B4;
  // fdivs f0,f0,f12
  f0 = double(float(f0 / f12));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // b 0x820d37c8
  goto loc_820D37C8;
loc_820D37B4:
  // fsubs f0,f0,f12
  lifted::ScalarFloatMode();
  f0 = double(float(f0 - f12));
  // fsubs f12,f9,f12
  f12 = double(float(f9 - f12));
  // fsubs f11,f10,f13
  f11 = double(float(f10 - f13));
  // fdivs f0,f0,f12
  f0 = double(float(f0 / f12));
  // fmadds f0,f0,f11,f13
  f0 = double(float(std::fma(f0, f11, f13)));
loc_820D37C8:
  // stfs f0,0(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
loc_820D37CC:
  // fcmpu cr6,f7,f8
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f7, f8);
  // ble cr6,0x820d37dc
  if (!cr6.gt) goto loc_820D37DC;
  // stfs f6,0(r7)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // blr
  return;
loc_820D37DC:
  // lfs f12,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f8
  cr6 = lifted::CompareFloat(f12, f8);
  // bge cr6,0x820d37f0
  if (!cr6.lt) goto loc_820D37F0;
  // lfs f0,44(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 44);
  f0 = double(temp.f32);
  // b 0x820d3838
  goto loc_820D3838;
loc_820D37F0:
  // lfs f0,32(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 32);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // ble cr6,0x820d3818
  if (!cr6.gt) goto loc_820D3818;
  // fsubs f0,f0,f7
  f0 = double(float(f0 - f7));
  // lfs f12,36(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f12 = double(temp.f32);
  // fsubs f13,f8,f7
  f13 = double(float(f8 - f7));
  // fsubs f12,f12,f6
  f12 = double(float(f12 - f6));
  // fdivs f0,f13,f0
  f0 = double(float(f13 / f0));
  // fmadds f0,f0,f12,f6
  f0 = double(float(std::fma(f0, f12, f6)));
  // b 0x820d3838
  goto loc_820D3838;
loc_820D3818:
  // lfs f0,32(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 32);
  f0 = double(temp.f32);
  // fsubs f10,f8,f0
  f10 = double(float(f8 - f0));
  // lfs f13,36(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f13 = double(temp.f32);
  // fsubs f0,f12,f0
  f0 = double(float(f12 - f0));
  // lfs f11,44(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 44);
  f11 = double(temp.f32);
  // fsubs f12,f11,f13
  f12 = double(float(f11 - f13));
  // fdivs f0,f10,f0
  f0 = double(float(f10 / f0));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
loc_820D3838:
  // stfs f0,0(r7)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // blr
  return;
}

// sub_820D47A8: lifted from generated/ by lift.py.
lifted::Result f820D47A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820D47A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v11{}, v12{}, v13{};
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  lifted::Vec vTemp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e64
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-136(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -136, std::bit_cast<uint64_t>(f29));
  // stfd f30,-128(r1)
  REX_STORE_U64(uint32_t(r1) + -128, std::bit_cast<uint64_t>(f30));
  // stfd f31,-120(r1)
  REX_STORE_U64(uint32_t(r1) + -120, std::bit_cast<uint64_t>(f31));
  // stwu r1,-1552(r1)
  ea = -1552 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // mr r31,r6
  r31 = r6;
  // addi r25,r11,6344
  r25 = uint64_t(int64_t(int64_t(r11) + 6344));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r29,r3
  r29 = r3;
  // addi r6,r11,20464
  r6 = uint64_t(int64_t(int64_t(r11) + 20464));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,384
  r3 = uint64_t(int64_t(int64_t(r1) + 384));
  // addi r4,r11,20456
  r4 = uint64_t(int64_t(int64_t(r11) + 20456));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r5,r11,r25
  r5 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r25));
  // bl 0x825a93d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A93D0, LIFTED_ARGS(0x820D47F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // addi r11,r31,3956
  r11 = uint64_t(int64_t(int64_t(r31) + 3956));
  // mr r30,r26
  r30 = r26;
loc_820D4800:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // beq cr6,0x820d4dac
  if (cr6.eq) goto loc_820D4DAC;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // blt cr6,0x820d4800
  if (cr6.lt) goto loc_820D4800;
  // lwz r11,3732(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3732);
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x820d4d98
  if (!cr6.eq) goto loc_820D4D98;
  // addi r30,r31,3760
  r30 = uint64_t(int64_t(int64_t(r31) + 3760));
  // lwz r4,3744(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 3744);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x82599a20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82599A20, LIFTED_ARGS(0x820D483C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82599af0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82599AF0, LIFTED_ARGS(0x820D4844, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82599c58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82599C58, LIFTED_ARGS(0x820D484C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825998a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825998A0, LIFTED_ARGS(0x820D4854, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // mr r27,r26
  r27 = r26;
  // stw r11,3748(r31)
  REX_STORE_U32(uint32_t(r31) + 3748, uint32_t(r11));
  // bl 0x82599880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82599880, LIFTED_ARGS(0x820D4868, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x820d4abc
  if (!cr0.gt) goto loc_820D4ABC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r29,r26
  r29 = r26;
  // addi r25,r11,20444
  r25 = uint64_t(int64_t(int64_t(r11) + 20444));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r19,r3
  r19 = r3;
  // addi r24,r11,20436
  r24 = uint64_t(int64_t(int64_t(r11) + 20436));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r23,r11,20428
  r23 = uint64_t(int64_t(int64_t(r11) + 20428));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r22,r11,20420
  r22 = uint64_t(int64_t(int64_t(r11) + 20420));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r21,r11,20412
  r21 = uint64_t(int64_t(int64_t(r11) + 20412));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r20,r11,20404
  r20 = uint64_t(int64_t(int64_t(r11) + 20404));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r28,r11,20396
  r28 = uint64_t(int64_t(int64_t(r11) + 20396));
loc_820D48B0:
  // lwz r11,3844(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3844);
  // add r30,r11,r26
  r30 = r11 + r26;
  // mr r11,r28
  r11 = r28;
  // mr r10,r30
  r10 = r30;
loc_820D48C0:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d48e4
  if (cr0.eq) goto loc_820D48E4;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d48c0
  if (cr6.eq) goto loc_820D48C0;
loc_820D48E4:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d4900
  if (!cr0.eq) goto loc_820D4900;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,6720
  r3 = uint64_t(int64_t(int64_t(r31) + 6720));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D48FC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
loc_820D4900:
  // mr r10,r30
  r10 = r30;
  // mr r11,r20
  r11 = r20;
loc_820D4908:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d492c
  if (cr0.eq) goto loc_820D492C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d4908
  if (cr6.eq) goto loc_820D4908;
loc_820D492C:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d4948
  if (!cr0.eq) goto loc_820D4948;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,6784
  r3 = uint64_t(int64_t(int64_t(r31) + 6784));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D4944, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
loc_820D4948:
  // mr r10,r30
  r10 = r30;
  // mr r11,r21
  r11 = r21;
loc_820D4950:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d4974
  if (cr0.eq) goto loc_820D4974;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d4950
  if (cr6.eq) goto loc_820D4950;
loc_820D4974:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d4990
  if (!cr0.eq) goto loc_820D4990;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,6848
  r3 = uint64_t(int64_t(int64_t(r31) + 6848));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D498C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
loc_820D4990:
  // mr r10,r30
  r10 = r30;
  // mr r11,r22
  r11 = r22;
loc_820D4998:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d49bc
  if (cr0.eq) goto loc_820D49BC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d4998
  if (cr6.eq) goto loc_820D4998;
loc_820D49BC:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d49d8
  if (!cr0.eq) goto loc_820D49D8;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,6912
  r3 = uint64_t(int64_t(int64_t(r31) + 6912));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D49D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
loc_820D49D8:
  // mr r10,r30
  r10 = r30;
  // mr r11,r23
  r11 = r23;
loc_820D49E0:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d4a04
  if (cr0.eq) goto loc_820D4A04;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d49e0
  if (cr6.eq) goto loc_820D49E0;
loc_820D4A04:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d4a20
  if (!cr0.eq) goto loc_820D4A20;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,7040
  r3 = uint64_t(int64_t(int64_t(r31) + 7040));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D4A1C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
loc_820D4A20:
  // mr r10,r30
  r10 = r30;
  // mr r11,r24
  r11 = r24;
loc_820D4A28:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d4a4c
  if (cr0.eq) goto loc_820D4A4C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d4a28
  if (cr6.eq) goto loc_820D4A28;
loc_820D4A4C:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d4a68
  if (!cr0.eq) goto loc_820D4A68;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,7104
  r3 = uint64_t(int64_t(int64_t(r31) + 7104));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D4A64, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
loc_820D4A68:
  // mr r10,r30
  r10 = r30;
  // mr r11,r25
  r11 = r25;
loc_820D4A70:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d4a94
  if (cr0.eq) goto loc_820D4A94;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d4a70
  if (cr6.eq) goto loc_820D4A70;
loc_820D4A94:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d4aac
  if (!cr0.eq) goto loc_820D4AAC;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // add r11,r11,r29
  r11 = r11 + r29;
  // stw r11,7200(r31)
  REX_STORE_U32(uint32_t(r31) + 7200, uint32_t(r11));
loc_820D4AAC:
  // addic. r19,r19,-1
  ca = uint32_t(r19) > 0;
  r19 = uint64_t(int64_t(int64_t(r19) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r19), 0, so);
  // addi r26,r26,160
  r26 = uint64_t(int64_t(int64_t(r26) + 160));
  // addi r29,r29,64
  r29 = uint64_t(int64_t(int64_t(r29) + 64));
  // bne 0x820d48b0
  if (!cr0.eq) goto loc_820D48B0;
loc_820D4ABC:
  // vspltisw v0,0
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_set1_epi32(int(0x0)));
  // addi r8,r1,224
  r8 = uint64_t(int64_t(int64_t(r1) + 224));
  // addi r10,r1,288
  r10 = uint64_t(int64_t(int64_t(r1) + 288));
  // addi r11,r1,224
  r11 = uint64_t(int64_t(int64_t(r1) + 224));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // vupkd3d128 v0,v0,4
  temp.f32 = 3.0f;
  temp.s32 += v0.s16[1];
  vTemp.f32[3] = temp.f32;
  temp.f32 = 3.0f;
  temp.s32 += v0.s16[0];
  vTemp.f32[2] = temp.f32;
  vTemp.f32[1] = 0.0f;
  vTemp.f32[0] = 1.0f;
  v0 = vTemp;
  // vpermwi128 v13,v0,234
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x15));
  // vpermwi128 v12,v0,186
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x45));
  // vpermwi128 v11,v0,174
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x51));
  // vpermwi128 v0,v0,171
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x54));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,240
  r8 = uint64_t(int64_t(int64_t(r1) + 240));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,272
  r8 = uint64_t(int64_t(int64_t(r1) + 272));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // mtctr r9
  ctr = r9;
loc_820D4B04:
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x820d4b04
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_820D4B04;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // addi r3,r1,288
  r3 = uint64_t(int64_t(int64_t(r1) + 288));
  // lfs f0,10832(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10832);
  f0 = double(temp.f32);
  // stfs f0,144(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 144, temp.u32);
  // bl 0x820a67f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A67F8, LIFTED_ARGS(0x820D4B34, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r5,r31,6976
  r5 = uint64_t(int64_t(int64_t(r31) + 6976));
  // addi r10,r31,6832
  r10 = uint64_t(int64_t(int64_t(r31) + 6832));
  // addi r30,r31,6992
  r30 = uint64_t(int64_t(int64_t(r31) + 6992));
  // addi r9,r31,6896
  r9 = uint64_t(int64_t(int64_t(r31) + 6896));
  // lfs f30,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f30 = double(temp.f32);
  // addi r11,r31,6768
  r11 = uint64_t(int64_t(int64_t(r31) + 6768));
  // addi r29,r31,7008
  r29 = uint64_t(int64_t(int64_t(r31) + 7008));
  // addi r8,r31,6960
  r8 = uint64_t(int64_t(int64_t(r31) + 6960));
  // addi r28,r31,7024
  r28 = uint64_t(int64_t(int64_t(r31) + 7024));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // ld r7,0(r11)
  r7 = REX_LOAD_U64(uint32_t(r11) + 0);
  // mr r3,r5
  r3 = r5;
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r11,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r11);
  // ld r11,0(r10)
  r11 = REX_LOAD_U64(uint32_t(r10) + 0);
  // stfs f30,6988(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 6988, temp.u32);
  // std r11,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r11);
  // ld r11,8(r10)
  r11 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r11,8(r30)
  REX_STORE_U64(uint32_t(r30) + 8, r11);
  // ld r11,0(r9)
  r11 = REX_LOAD_U64(uint32_t(r9) + 0);
  // stfs f30,7004(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7004, temp.u32);
  // std r11,0(r29)
  REX_STORE_U64(uint32_t(r29) + 0, r11);
  // ld r11,8(r9)
  r11 = REX_LOAD_U64(uint32_t(r9) + 8);
  // std r11,8(r29)
  REX_STORE_U64(uint32_t(r29) + 8, r11);
  // ld r11,0(r8)
  r11 = REX_LOAD_U64(uint32_t(r8) + 0);
  // stfs f30,7020(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7020, temp.u32);
  // std r11,0(r28)
  REX_STORE_U64(uint32_t(r28) + 0, r11);
  // ld r11,8(r8)
  r11 = REX_LOAD_U64(uint32_t(r8) + 8);
  // std r11,8(r28)
  REX_STORE_U64(uint32_t(r28) + 8, r11);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // lfs f13,6980(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 6980);
  f13 = double(temp.f32);
  // lfs f12,6984(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 6984);
  f12 = double(temp.f32);
  // lfs f11,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f11 = double(temp.f32);
  // lfs f31,8124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8124);
  f31 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // stfs f13,6980(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 6980, temp.u32);
  // lfs f0,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f0 = double(temp.f32);
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // lfs f13,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f13 = double(temp.f32);
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f12,6984(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 6984, temp.u32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // stfs f11,0(r30)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // stfs f13,0(r28)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // lfs f10,6996(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 6996);
  f10 = double(temp.f32);
  // lfs f9,7000(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7000);
  f9 = double(temp.f32);
  // fmuls f10,f10,f31
  f10 = double(float(f10 * f31));
  // lfs f0,7012(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7012);
  f0 = double(temp.f32);
  // fmuls f9,f9,f31
  f9 = double(float(f9 * f31));
  // lfs f13,7016(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7016);
  f13 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f12,7028(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7028);
  f12 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // lfs f11,7032(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7032);
  f11 = double(temp.f32);
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // stfs f30,7036(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7036, temp.u32);
  // stfs f10,6996(r31)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r31) + 6996, temp.u32);
  // stfs f9,7000(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 7000, temp.u32);
  // stfs f0,7012(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 7012, temp.u32);
  // stfs f13,7016(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 7016, temp.u32);
  // stfs f12,7028(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 7028, temp.u32);
  // stfs f11,7032(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 7032, temp.u32);
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D4C50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r30
  r3 = r30;
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D4C60, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r29
  r3 = r29;
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D4C70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r28
  r5 = r28;
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r28
  r3 = r28;
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D4C80, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmpwi cr6,r27,7
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 7, so);
  // lfs f29,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f29 = double(temp.f32);
  // stfs f29,6988(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 6988, temp.u32);
  // stfs f29,7004(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7004, temp.u32);
  // stfs f29,7020(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7020, temp.u32);
  // stfs f29,7036(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7036, temp.u32);
  // bne cr6,0x820d4d44
  if (!cr6.eq) goto loc_820D4D44;
  // addi r11,r31,7088
  r11 = uint64_t(int64_t(int64_t(r31) + 7088));
  // addi r5,r31,7168
  r5 = uint64_t(int64_t(int64_t(r31) + 7168));
  // addi r10,r31,7152
  r10 = uint64_t(int64_t(int64_t(r31) + 7152));
  // addi r30,r31,7184
  r30 = uint64_t(int64_t(int64_t(r31) + 7184));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // mr r3,r5
  r3 = r5;
  // std r9,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r9);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r11,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r11);
  // ld r11,0(r10)
  r11 = REX_LOAD_U64(uint32_t(r10) + 0);
  // stfs f30,7180(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7180, temp.u32);
  // std r11,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r11);
  // ld r11,8(r10)
  r11 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r11,8(r30)
  REX_STORE_U64(uint32_t(r30) + 8, r11);
  // lfs f0,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // lfs f13,7172(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7172);
  f13 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f12,7176(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7176);
  f12 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // lfs f11,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f11 = double(temp.f32);
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // lfs f10,7188(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7188);
  f10 = double(temp.f32);
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // lfs f9,7192(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7192);
  f9 = double(temp.f32);
  // fmuls f10,f10,f31
  f10 = double(float(f10 * f31));
  // fmuls f9,f9,f31
  f9 = double(float(f9 * f31));
  // stfs f30,7196(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7196, temp.u32);
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // stfs f13,7172(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 7172, temp.u32);
  // stfs f12,7176(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 7176, temp.u32);
  // stfs f11,0(r30)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f10,7188(r31)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r31) + 7188, temp.u32);
  // stfs f9,7192(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 7192, temp.u32);
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D4D2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r30
  r3 = r30;
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D4D3C, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f29,7180(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7180, temp.u32);
  // stfs f29,7196(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7196, temp.u32);
loc_820D4D44:
  // lwz r11,7200(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 7200);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820d4d98
  if (cr0.eq) goto loc_820D4D98;
  // ld r10,48(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 48);
  // addi r5,r31,7216
  r5 = uint64_t(int64_t(int64_t(r31) + 7216));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r5
  r3 = r5;
  // std r10,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r10);
  // ld r11,56(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 56);
  // std r11,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r11);
  // lfs f0,0(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // lfs f13,7220(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7220);
  f13 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f12,7224(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7224);
  f12 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // stfs f30,7228(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7228, temp.u32);
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // stfs f13,7220(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 7220, temp.u32);
  // stfs f12,7224(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 7224, temp.u32);
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D4D98, r31)); r3 = o.r3; f1 = o.f1; }
loc_820D4D98:
  // addi r1,r1,1552
  r1 = uint64_t(int64_t(int64_t(r1) + 1552));
  // lfd f29,-136(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -136)));
  // lfd f30,-128(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -128)));
  // lfd f31,-120(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -120)));
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
loc_820D4DAC:
  // mulli r11,r30,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(24))));
  // add r29,r11,r31
  r29 = r11 + r31;
  // addi r28,r29,3968
  r28 = uint64_t(int64_t(int64_t(r29) + 3968));
  // addi r27,r29,4000
  r27 = uint64_t(int64_t(int64_t(r29) + 4000));
  // mr r3,r27
  r3 = r27;
  // lwz r4,0(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 0);
  // bl 0x82596f68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82596F68, LIFTED_ARGS(0x820D4DC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,20
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(20))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // mr r5,r27
  r5 = r27;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r11,4048
  r3 = uint64_t(int64_t(int64_t(r11) + 4048));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820D4DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // mr r4,r28
  r4 = r28;
  // stw r11,3972(r29)
  REX_STORE_U32(uint32_t(r29) + 3972, uint32_t(r11));
  // lwz r3,7256(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 7256);
  // bl 0x820c3ce0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C3CE0, LIFTED_ARGS(0x820D4E00, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stw r26,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r26));
  // bne cr6,0x820d4d98
  if (!cr6.eq) goto loc_820D4D98;
  // lwz r11,3724(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3724);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x820d4ed8
  if (!cr6.eq) goto loc_820D4ED8;
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // stw r26,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r26));
  // bl 0x820eebc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EEBC0, LIFTED_ARGS(0x820D4E24, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,352
  r3 = uint64_t(int64_t(int64_t(r1) + 352));
  // bl 0x820eebc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EEBC0, LIFTED_ARGS(0x820D4E2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,192
  r4 = uint64_t(int64_t(int64_t(r1) + 192));
  // lwz r3,11844(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 11844);
  // bl 0x820c4e68
  // (left out: dead in C++)
  r3 = f820C4E68(base, r1, r13, r3, r4, r5);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,6696(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 6696);
  // lwz r9,6688(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 6688);
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r26,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r26));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // addi r30,r1,176
  r30 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lwz r11,144(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 144);
  // stfs f0,160(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 160, temp.u32);
  // stw r10,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r10));
  // stfs f0,164(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 164, temp.u32);
  // stw r9,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r9));
  // stfs f0,168(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 168, temp.u32);
  // stfs f0,172(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 172, temp.u32);
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,188(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 188, temp.u32);
  // lfs f31,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,176(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 176, temp.u32);
  // stfs f31,180(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 180, temp.u32);
  // stfs f31,184(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 184, temp.u32);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820D4EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,208(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 208);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // ld r5,192(r1)
  r5 = REX_LOAD_U64(uint32_t(r1) + 192);
  // rldicr r7,r11,32,63
  r7 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // ld r6,200(r1)
  r6 = REX_LOAD_U64(uint32_t(r1) + 200);
  // ld r8,0(r30)
  r8 = REX_LOAD_U64(uint32_t(r30) + 0);
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // ld r9,8(r30)
  r9 = REX_LOAD_U64(uint32_t(r30) + 8);
  // bl 0x820ee638
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EE638, LIFTED_ARGS(0x820D4EC8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,352
  r3 = uint64_t(int64_t(int64_t(r1) + 352));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x820D4ED0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x820D4ED8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820D4ED8:
  // addi r30,r31,3664
  r30 = uint64_t(int64_t(int64_t(r31) + 3664));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // addi r4,r30,32
  r4 = uint64_t(int64_t(int64_t(r30) + 32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820D4EEC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r31,6712
  r6 = uint64_t(int64_t(int64_t(r31) + 6712));
  // addi r5,r31,6708
  r5 = uint64_t(int64_t(int64_t(r31) + 6708));
  // ld r7,0(r30)
  r7 = REX_LOAD_U64(uint32_t(r30) + 0);
  // addi r4,r31,6704
  r4 = uint64_t(int64_t(int64_t(r31) + 6704));
  // ld r8,8(r30)
  r8 = REX_LOAD_U64(uint32_t(r30) + 8);
  // addi r3,r31,6700
  r3 = uint64_t(int64_t(int64_t(r31) + 6700));
  // ld r9,16(r30)
  r9 = REX_LOAD_U64(uint32_t(r30) + 16);
  // ld r10,24(r30)
  r10 = REX_LOAD_U64(uint32_t(r30) + 24);
  // bl 0x820ed6d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ED6D0, LIFTED_ARGS(0x820D4F10, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r31,4000
  r28 = uint64_t(int64_t(int64_t(r31) + 4000));
  // addi r4,r1,384
  r4 = uint64_t(int64_t(int64_t(r1) + 384));
  // mr r3,r28
  r3 = r28;
  // bl 0x820d09b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D09B8, LIFTED_ARGS(0x820D4F20, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // stw r3,7232(r31)
  REX_STORE_U32(uint32_t(r31) + 7232, uint32_t(r3));
  // addi r27,r11,20388
  r27 = uint64_t(int64_t(int64_t(r11) + 20388));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r29,r31,7236
  r29 = uint64_t(int64_t(int64_t(r31) + 7236));
  // addi r26,r11,20376
  r26 = uint64_t(int64_t(int64_t(r11) + 20376));
loc_820D4F3C:
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // mr r7,r30
  r7 = r30;
  // mr r6,r26
  r6 = r26;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,384
  r3 = uint64_t(int64_t(int64_t(r1) + 384));
  // lwzx r5,r11,r25
  r5 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r25));
  // bl 0x825a93d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A93D0, LIFTED_ARGS(0x820D4F5C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,384
  r4 = uint64_t(int64_t(int64_t(r1) + 384));
  // mr r3,r28
  r3 = r28;
  // bl 0x820d09b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D09B8, LIFTED_ARGS(0x820D4F68, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r3,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r3));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmpwi cr6,r30,6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 6, so);
  // blt cr6,0x820d4f3c
  if (cr6.lt) goto loc_820D4F3C;
  // b 0x820d4d98
  goto loc_820D4D98;
}

// sub_820D4F80: lifted from generated/ by lift.py.
lifted::Result f820D4F80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820D4F80 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v11{}, v12{}, v13{};
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  lifted::Vec vTemp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e64
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-136(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -136, std::bit_cast<uint64_t>(f29));
  // stfd f30,-128(r1)
  REX_STORE_U64(uint32_t(r1) + -128, std::bit_cast<uint64_t>(f30));
  // stfd f31,-120(r1)
  REX_STORE_U64(uint32_t(r1) + -120, std::bit_cast<uint64_t>(f31));
  // stwu r1,-1552(r1)
  ea = -1552 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // mr r31,r6
  r31 = r6;
  // addi r25,r11,6344
  r25 = uint64_t(int64_t(int64_t(r11) + 6344));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r28,r4
  r28 = r4;
  // addi r6,r11,20488
  r6 = uint64_t(int64_t(int64_t(r11) + 20488));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r29,r3
  r29 = r3;
  // addi r4,r11,20456
  r4 = uint64_t(int64_t(int64_t(r11) + 20456));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // addi r3,r1,384
  r3 = uint64_t(int64_t(int64_t(r1) + 384));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r5,r11,r25
  r5 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r25));
  // bl 0x825a93d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A93D0, LIFTED_ARGS(0x820D4FD0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r11,r31,3956
  r11 = uint64_t(int64_t(int64_t(r31) + 3956));
  // mr r30,r27
  r30 = r27;
loc_820D4FDC:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // beq cr6,0x820d5588
  if (cr6.eq) goto loc_820D5588;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // blt cr6,0x820d4fdc
  if (cr6.lt) goto loc_820D4FDC;
  // lwz r11,3732(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3732);
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x820d5574
  if (!cr6.eq) goto loc_820D5574;
  // addi r30,r31,3760
  r30 = uint64_t(int64_t(int64_t(r31) + 3760));
  // lwz r4,3744(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 3744);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x82599a20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82599A20, LIFTED_ARGS(0x820D5018, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82599af0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82599AF0, LIFTED_ARGS(0x820D5020, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82599c58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82599C58, LIFTED_ARGS(0x820D5028, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825998a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825998A0, LIFTED_ARGS(0x820D5030, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // mr r26,r27
  r26 = r27;
  // stw r11,3748(r31)
  REX_STORE_U32(uint32_t(r31) + 3748, uint32_t(r11));
  // bl 0x82599880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82599880, LIFTED_ARGS(0x820D5044, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x820d5298
  if (!cr0.gt) goto loc_820D5298;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r29,r27
  r29 = r27;
  // addi r25,r11,20444
  r25 = uint64_t(int64_t(int64_t(r11) + 20444));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r19,r3
  r19 = r3;
  // addi r24,r11,20436
  r24 = uint64_t(int64_t(int64_t(r11) + 20436));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r23,r11,20428
  r23 = uint64_t(int64_t(int64_t(r11) + 20428));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r22,r11,20420
  r22 = uint64_t(int64_t(int64_t(r11) + 20420));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r21,r11,20412
  r21 = uint64_t(int64_t(int64_t(r11) + 20412));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r20,r11,20404
  r20 = uint64_t(int64_t(int64_t(r11) + 20404));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r28,r11,20396
  r28 = uint64_t(int64_t(int64_t(r11) + 20396));
loc_820D508C:
  // lwz r11,3844(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3844);
  // add r30,r27,r11
  r30 = r27 + r11;
  // mr r11,r28
  r11 = r28;
  // mr r10,r30
  r10 = r30;
loc_820D509C:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d50c0
  if (cr0.eq) goto loc_820D50C0;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d509c
  if (cr6.eq) goto loc_820D509C;
loc_820D50C0:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d50dc
  if (!cr0.eq) goto loc_820D50DC;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,6720
  r3 = uint64_t(int64_t(int64_t(r31) + 6720));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D50D8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
loc_820D50DC:
  // mr r10,r30
  r10 = r30;
  // mr r11,r20
  r11 = r20;
loc_820D50E4:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d5108
  if (cr0.eq) goto loc_820D5108;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d50e4
  if (cr6.eq) goto loc_820D50E4;
loc_820D5108:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d5124
  if (!cr0.eq) goto loc_820D5124;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,6784
  r3 = uint64_t(int64_t(int64_t(r31) + 6784));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D5120, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
loc_820D5124:
  // mr r10,r30
  r10 = r30;
  // mr r11,r21
  r11 = r21;
loc_820D512C:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d5150
  if (cr0.eq) goto loc_820D5150;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d512c
  if (cr6.eq) goto loc_820D512C;
loc_820D5150:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d516c
  if (!cr0.eq) goto loc_820D516C;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,6848
  r3 = uint64_t(int64_t(int64_t(r31) + 6848));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D5168, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
loc_820D516C:
  // mr r10,r30
  r10 = r30;
  // mr r11,r22
  r11 = r22;
loc_820D5174:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d5198
  if (cr0.eq) goto loc_820D5198;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d5174
  if (cr6.eq) goto loc_820D5174;
loc_820D5198:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d51b4
  if (!cr0.eq) goto loc_820D51B4;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,6912
  r3 = uint64_t(int64_t(int64_t(r31) + 6912));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D51B0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
loc_820D51B4:
  // mr r10,r30
  r10 = r30;
  // mr r11,r23
  r11 = r23;
loc_820D51BC:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d51e0
  if (cr0.eq) goto loc_820D51E0;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d51bc
  if (cr6.eq) goto loc_820D51BC;
loc_820D51E0:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d51fc
  if (!cr0.eq) goto loc_820D51FC;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,7040
  r3 = uint64_t(int64_t(int64_t(r31) + 7040));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D51F8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
loc_820D51FC:
  // mr r10,r30
  r10 = r30;
  // mr r11,r24
  r11 = r24;
loc_820D5204:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d5228
  if (cr0.eq) goto loc_820D5228;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d5204
  if (cr6.eq) goto loc_820D5204;
loc_820D5228:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d5244
  if (!cr0.eq) goto loc_820D5244;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r3,r31,7104
  r3 = uint64_t(int64_t(int64_t(r31) + 7104));
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x820D5240, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
loc_820D5244:
  // mr r10,r30
  r10 = r30;
  // mr r11,r25
  r11 = r25;
loc_820D524C:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x820d5270
  if (cr0.eq) goto loc_820D5270;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x820d524c
  if (cr6.eq) goto loc_820D524C;
loc_820D5270:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x820d5288
  if (!cr0.eq) goto loc_820D5288;
  // lwz r11,3908(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3908);
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // add r11,r11,r29
  r11 = r11 + r29;
  // stw r11,7200(r31)
  REX_STORE_U32(uint32_t(r31) + 7200, uint32_t(r11));
loc_820D5288:
  // addic. r19,r19,-1
  ca = uint32_t(r19) > 0;
  r19 = uint64_t(int64_t(int64_t(r19) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r19), 0, so);
  // addi r27,r27,160
  r27 = uint64_t(int64_t(int64_t(r27) + 160));
  // addi r29,r29,64
  r29 = uint64_t(int64_t(int64_t(r29) + 64));
  // bne 0x820d508c
  if (!cr0.eq) goto loc_820D508C;
loc_820D5298:
  // vspltisw v0,0
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_set1_epi32(int(0x0)));
  // addi r8,r1,224
  r8 = uint64_t(int64_t(int64_t(r1) + 224));
  // addi r10,r1,288
  r10 = uint64_t(int64_t(int64_t(r1) + 288));
  // addi r11,r1,224
  r11 = uint64_t(int64_t(int64_t(r1) + 224));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // vupkd3d128 v0,v0,4
  temp.f32 = 3.0f;
  temp.s32 += v0.s16[1];
  vTemp.f32[3] = temp.f32;
  temp.f32 = 3.0f;
  temp.s32 += v0.s16[0];
  vTemp.f32[2] = temp.f32;
  vTemp.f32[1] = 0.0f;
  vTemp.f32[0] = 1.0f;
  v0 = vTemp;
  // vpermwi128 v13,v0,234
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x15));
  // vpermwi128 v12,v0,186
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x45));
  // vpermwi128 v11,v0,174
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x51));
  // vpermwi128 v0,v0,171
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x54));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,240
  r8 = uint64_t(int64_t(int64_t(r1) + 240));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,272
  r8 = uint64_t(int64_t(int64_t(r1) + 272));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // mtctr r9
  ctr = r9;
loc_820D52E0:
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x820d52e0
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_820D52E0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // addi r3,r1,288
  r3 = uint64_t(int64_t(int64_t(r1) + 288));
  // lfs f0,10832(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10832);
  f0 = double(temp.f32);
  // stfs f0,144(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 144, temp.u32);
  // bl 0x820a67f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A67F8, LIFTED_ARGS(0x820D5310, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r5,r31,6976
  r5 = uint64_t(int64_t(int64_t(r31) + 6976));
  // addi r10,r31,6832
  r10 = uint64_t(int64_t(int64_t(r31) + 6832));
  // addi r30,r31,6992
  r30 = uint64_t(int64_t(int64_t(r31) + 6992));
  // addi r9,r31,6896
  r9 = uint64_t(int64_t(int64_t(r31) + 6896));
  // lfs f30,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f30 = double(temp.f32);
  // addi r11,r31,6768
  r11 = uint64_t(int64_t(int64_t(r31) + 6768));
  // addi r29,r31,7008
  r29 = uint64_t(int64_t(int64_t(r31) + 7008));
  // addi r8,r31,6960
  r8 = uint64_t(int64_t(int64_t(r31) + 6960));
  // addi r28,r31,7024
  r28 = uint64_t(int64_t(int64_t(r31) + 7024));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // ld r7,0(r11)
  r7 = REX_LOAD_U64(uint32_t(r11) + 0);
  // mr r3,r5
  r3 = r5;
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r11,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r11);
  // ld r11,0(r10)
  r11 = REX_LOAD_U64(uint32_t(r10) + 0);
  // stfs f30,6988(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 6988, temp.u32);
  // std r11,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r11);
  // ld r11,8(r10)
  r11 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r11,8(r30)
  REX_STORE_U64(uint32_t(r30) + 8, r11);
  // ld r11,0(r9)
  r11 = REX_LOAD_U64(uint32_t(r9) + 0);
  // stfs f30,7004(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7004, temp.u32);
  // std r11,0(r29)
  REX_STORE_U64(uint32_t(r29) + 0, r11);
  // ld r11,8(r9)
  r11 = REX_LOAD_U64(uint32_t(r9) + 8);
  // std r11,8(r29)
  REX_STORE_U64(uint32_t(r29) + 8, r11);
  // ld r11,0(r8)
  r11 = REX_LOAD_U64(uint32_t(r8) + 0);
  // stfs f30,7020(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7020, temp.u32);
  // std r11,0(r28)
  REX_STORE_U64(uint32_t(r28) + 0, r11);
  // ld r11,8(r8)
  r11 = REX_LOAD_U64(uint32_t(r8) + 8);
  // std r11,8(r28)
  REX_STORE_U64(uint32_t(r28) + 8, r11);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // lfs f13,6980(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 6980);
  f13 = double(temp.f32);
  // lfs f12,6984(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 6984);
  f12 = double(temp.f32);
  // lfs f11,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f11 = double(temp.f32);
  // lfs f31,8124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8124);
  f31 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // stfs f13,6980(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 6980, temp.u32);
  // lfs f0,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f0 = double(temp.f32);
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // lfs f13,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f13 = double(temp.f32);
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f12,6984(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 6984, temp.u32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // stfs f11,0(r30)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // stfs f13,0(r28)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // lfs f10,6996(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 6996);
  f10 = double(temp.f32);
  // lfs f9,7000(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7000);
  f9 = double(temp.f32);
  // fmuls f10,f10,f31
  f10 = double(float(f10 * f31));
  // lfs f0,7012(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7012);
  f0 = double(temp.f32);
  // fmuls f9,f9,f31
  f9 = double(float(f9 * f31));
  // lfs f13,7016(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7016);
  f13 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f12,7028(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7028);
  f12 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // lfs f11,7032(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7032);
  f11 = double(temp.f32);
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // stfs f30,7036(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7036, temp.u32);
  // stfs f10,6996(r31)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r31) + 6996, temp.u32);
  // stfs f9,7000(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 7000, temp.u32);
  // stfs f0,7012(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 7012, temp.u32);
  // stfs f13,7016(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 7016, temp.u32);
  // stfs f12,7028(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 7028, temp.u32);
  // stfs f11,7032(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 7032, temp.u32);
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D542C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r30
  r3 = r30;
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D543C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r29
  r3 = r29;
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D544C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r28
  r5 = r28;
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r28
  r3 = r28;
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D545C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmpwi cr6,r26,7
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 7, so);
  // lfs f29,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f29 = double(temp.f32);
  // stfs f29,6988(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 6988, temp.u32);
  // stfs f29,7004(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7004, temp.u32);
  // stfs f29,7020(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7020, temp.u32);
  // stfs f29,7036(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7036, temp.u32);
  // bne cr6,0x820d5520
  if (!cr6.eq) goto loc_820D5520;
  // addi r11,r31,7088
  r11 = uint64_t(int64_t(int64_t(r31) + 7088));
  // addi r5,r31,7168
  r5 = uint64_t(int64_t(int64_t(r31) + 7168));
  // addi r10,r31,7152
  r10 = uint64_t(int64_t(int64_t(r31) + 7152));
  // addi r30,r31,7184
  r30 = uint64_t(int64_t(int64_t(r31) + 7184));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // mr r3,r5
  r3 = r5;
  // std r9,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r9);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r11,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r11);
  // ld r11,0(r10)
  r11 = REX_LOAD_U64(uint32_t(r10) + 0);
  // stfs f30,7180(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7180, temp.u32);
  // std r11,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r11);
  // ld r11,8(r10)
  r11 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r11,8(r30)
  REX_STORE_U64(uint32_t(r30) + 8, r11);
  // lfs f0,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // lfs f13,7172(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7172);
  f13 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f12,7176(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7176);
  f12 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // lfs f11,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f11 = double(temp.f32);
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // lfs f10,7188(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7188);
  f10 = double(temp.f32);
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // lfs f9,7192(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7192);
  f9 = double(temp.f32);
  // fmuls f10,f10,f31
  f10 = double(float(f10 * f31));
  // fmuls f9,f9,f31
  f9 = double(float(f9 * f31));
  // stfs f30,7196(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7196, temp.u32);
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // stfs f13,7172(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 7172, temp.u32);
  // stfs f12,7176(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 7176, temp.u32);
  // stfs f11,0(r30)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f10,7188(r31)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r31) + 7188, temp.u32);
  // stfs f9,7192(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 7192, temp.u32);
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D5508, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r30
  r3 = r30;
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D5518, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f29,7180(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7180, temp.u32);
  // stfs f29,7196(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 7196, temp.u32);
loc_820D5520:
  // lwz r11,7200(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 7200);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820d5574
  if (cr0.eq) goto loc_820D5574;
  // ld r10,48(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 48);
  // addi r5,r31,7216
  r5 = uint64_t(int64_t(int64_t(r31) + 7216));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // mr r3,r5
  r3 = r5;
  // std r10,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r10);
  // ld r11,56(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 56);
  // std r11,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r11);
  // lfs f0,0(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // lfs f13,7220(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7220);
  f13 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f12,7224(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 7224);
  f12 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // stfs f30,7228(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 7228, temp.u32);
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // stfs f13,7220(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 7220, temp.u32);
  // stfs f12,7224(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 7224, temp.u32);
  // bl 0x820ab560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB560, LIFTED_ARGS(0x820D5574, r31)); r3 = o.r3; f1 = o.f1; }
loc_820D5574:
  // addi r1,r1,1552
  r1 = uint64_t(int64_t(int64_t(r1) + 1552));
  // lfd f29,-136(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -136)));
  // lfd f30,-128(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -128)));
  // lfd f31,-120(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -120)));
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
loc_820D5588:
  // mulli r11,r30,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(24))));
  // add r29,r11,r31
  r29 = r11 + r31;
  // mr r4,r28
  r4 = r28;
  // addi r28,r29,4000
  r28 = uint64_t(int64_t(int64_t(r29) + 4000));
  // mr r3,r28
  r3 = r28;
  // bl 0x82596f68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82596F68, LIFTED_ARGS(0x820D55A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,20
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(20))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // mr r5,r28
  r5 = r28;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r11,4048
  r3 = uint64_t(int64_t(int64_t(r11) + 4048));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820D55C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stw r27,3968(r29)
  REX_STORE_U32(uint32_t(r29) + 3968, uint32_t(r27));
  // stw r11,3972(r29)
  REX_STORE_U32(uint32_t(r29) + 3972, uint32_t(r11));
  // bne cr6,0x820d5574
  if (!cr6.eq) goto loc_820D5574;
  // lwz r11,3724(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3724);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x820d56a4
  if (!cr6.eq) goto loc_820D56A4;
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // stw r27,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r27));
  // bl 0x820eebc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EEBC0, LIFTED_ARGS(0x820D55F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,352
  r3 = uint64_t(int64_t(int64_t(r1) + 352));
  // bl 0x820eebc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EEBC0, LIFTED_ARGS(0x820D55F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,192
  r4 = uint64_t(int64_t(int64_t(r1) + 192));
  // lwz r3,11844(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 11844);
  // bl 0x820c4e68
  // (left out: dead in C++)
  r3 = f820C4E68(base, r1, r13, r3, r4, r5);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,6696(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 6696);
  // lwz r9,6688(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 6688);
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r27,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r27));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // addi r30,r1,176
  r30 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lwz r11,144(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 144);
  // stfs f0,160(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 160, temp.u32);
  // stw r10,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r10));
  // stfs f0,164(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 164, temp.u32);
  // stw r9,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r9));
  // stfs f0,168(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 168, temp.u32);
  // stfs f0,172(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 172, temp.u32);
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,188(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 188, temp.u32);
  // lfs f31,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,176(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 176, temp.u32);
  // stfs f31,180(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 180, temp.u32);
  // stfs f31,184(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 184, temp.u32);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820D566C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,208(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 208);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // ld r5,192(r1)
  r5 = REX_LOAD_U64(uint32_t(r1) + 192);
  // rldicr r7,r11,32,63
  r7 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // ld r6,200(r1)
  r6 = REX_LOAD_U64(uint32_t(r1) + 200);
  // ld r8,0(r30)
  r8 = REX_LOAD_U64(uint32_t(r30) + 0);
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // ld r9,8(r30)
  r9 = REX_LOAD_U64(uint32_t(r30) + 8);
  // bl 0x820ee638
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EE638, LIFTED_ARGS(0x820D5694, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,352
  r3 = uint64_t(int64_t(int64_t(r1) + 352));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x820D569C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x820D56A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_820D56A4:
  // addi r30,r31,3664
  r30 = uint64_t(int64_t(int64_t(r31) + 3664));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // addi r4,r30,32
  r4 = uint64_t(int64_t(int64_t(r30) + 32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820D56B8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r31,6712
  r6 = uint64_t(int64_t(int64_t(r31) + 6712));
  // addi r5,r31,6708
  r5 = uint64_t(int64_t(int64_t(r31) + 6708));
  // ld r7,0(r30)
  r7 = REX_LOAD_U64(uint32_t(r30) + 0);
  // addi r4,r31,6704
  r4 = uint64_t(int64_t(int64_t(r31) + 6704));
  // ld r8,8(r30)
  r8 = REX_LOAD_U64(uint32_t(r30) + 8);
  // addi r3,r31,6700
  r3 = uint64_t(int64_t(int64_t(r31) + 6700));
  // ld r9,16(r30)
  r9 = REX_LOAD_U64(uint32_t(r30) + 16);
  // ld r10,24(r30)
  r10 = REX_LOAD_U64(uint32_t(r30) + 24);
  // bl 0x820ed6d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ED6D0, LIFTED_ARGS(0x820D56DC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r31,4000
  r28 = uint64_t(int64_t(int64_t(r31) + 4000));
  // addi r4,r1,384
  r4 = uint64_t(int64_t(int64_t(r1) + 384));
  // mr r3,r28
  r3 = r28;
  // bl 0x820d09b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D09B8, LIFTED_ARGS(0x820D56EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // stw r3,7232(r31)
  REX_STORE_U32(uint32_t(r31) + 7232, uint32_t(r3));
  // addi r27,r11,20388
  r27 = uint64_t(int64_t(int64_t(r11) + 20388));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r29,r31,7236
  r29 = uint64_t(int64_t(int64_t(r31) + 7236));
  // addi r26,r11,20476
  r26 = uint64_t(int64_t(int64_t(r11) + 20476));
loc_820D5708:
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // mr r7,r30
  r7 = r30;
  // mr r6,r26
  r6 = r26;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,384
  r3 = uint64_t(int64_t(int64_t(r1) + 384));
  // lwzx r5,r11,r25
  r5 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r25));
  // bl 0x825a93d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A93D0, LIFTED_ARGS(0x820D5728, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,384
  r4 = uint64_t(int64_t(int64_t(r1) + 384));
  // mr r3,r28
  r3 = r28;
  // bl 0x820d09b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D09B8, LIFTED_ARGS(0x820D5734, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r3,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r3));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmpwi cr6,r30,6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 6, so);
  // blt cr6,0x820d5708
  if (cr6.lt) goto loc_820D5708;
  // b 0x820d5574
  goto loc_820D5574;
}

// sub_820D5890: lifted from generated/ by lift.py.
lifted::Result f820D5890(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820D5890 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x82594050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594050, LIFTED_ARGS(0x820D58A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,20372
  r11 = uint64_t(int64_t(int64_t(r11) + 20372));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r3,r31,32
  r3 = uint64_t(int64_t(int64_t(r31) + 32));
  // stw r10,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r9,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r9));
  // bl 0x820d45b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D45B0, LIFTED_ARGS(0x820D58CC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,3728
  r3 = uint64_t(int64_t(int64_t(r31) + 3728));
  // bl 0x820d3de0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3DE0, LIFTED_ARGS(0x820D58D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r11,r31,6720
  r11 = uint64_t(int64_t(int64_t(r31) + 6720));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // lfs f0,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f0 = double(temp.f32);
loc_820D58E8:
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
  // stfs f0,32(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // bge 0x820d58e8
  if (!cr0.lt) goto loc_820D58E8;
  // addi r10,r31,6976
  r10 = uint64_t(int64_t(int64_t(r31) + 6976));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r10,r10,12
  r10 = uint64_t(int64_t(int64_t(r10) + 12));
loc_820D5910:
  // stfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // bge 0x820d5910
  if (!cr0.lt) goto loc_820D5910;
  // addi r11,r31,7040
  r11 = uint64_t(int64_t(int64_t(r31) + 7040));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
loc_820D592C:
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
  // stfs f0,32(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // bge 0x820d592c
  if (!cr0.lt) goto loc_820D592C;
  // addi r10,r31,7168
  r10 = uint64_t(int64_t(int64_t(r31) + 7168));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,12
  r10 = uint64_t(int64_t(int64_t(r10) + 12));
loc_820D5954:
  // stfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // bge 0x820d5954
  if (!cr0.lt) goto loc_820D5954;
  // mr r3,r31
  r3 = r31;
  // stfs f0,7228(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 7228, temp.u32);
  // bl 0x820d3e98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3E98, LIFTED_ARGS(0x820D5970, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820D6108: lifted from generated/ by lift.py.
lifted::Result f820D6108(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820D6108 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1168(r1)
  ea = -1168 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r30,556
  r10 = uint64_t(int64_t(int64_t(r30) + 556));
loc_820D6120:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // beq cr6,0x820d61b0
  if (cr6.eq) goto loc_820D61B0;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,24
  r10 = uint64_t(int64_t(int64_t(r10) + 24));
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // blt cr6,0x820d6120
  if (cr6.lt) goto loc_820D6120;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r30,420
  r10 = uint64_t(int64_t(int64_t(r30) + 420));
loc_820D6144:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // beq cr6,0x820d61b0
  if (cr6.eq) goto loc_820D61B0;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,24
  r10 = uint64_t(int64_t(int64_t(r10) + 24));
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // blt cr6,0x820d6144
  if (cr6.lt) goto loc_820D6144;
  // lwz r11,196(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 196);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x820d61b0
  if (cr6.eq) goto loc_820D61B0;
  // lwz r11,24(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 24);
  // li r26,-1
  r26 = uint64_t(int64_t(-1));
  // lwz r7,48(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 48);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // cmpwi r7,0
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // lwz r29,32(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 32);
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // rlwinm r9,r29,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,6344
  r11 = uint64_t(int64_t(int64_t(r11) + 6344));
  // lwzx r5,r9,r11
  r5 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // bne 0x820d61b8
  if (!cr0.eq) goto loc_820D61B8;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r6,r10,20488
  r6 = uint64_t(int64_t(int64_t(r10) + 20488));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r4,r10,20456
  r4 = uint64_t(int64_t(int64_t(r10) + 20456));
  // bl 0x825a93d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A93D0, LIFTED_ARGS(0x820D61AC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820d61cc
  goto loc_820D61CC;
loc_820D61B0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820d6230
  goto loc_820D6230;
loc_820D61B8:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r6,r10,20476
  r6 = uint64_t(int64_t(int64_t(r10) + 20476));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r4,r10,20388
  r4 = uint64_t(int64_t(int64_t(r10) + 20388));
  // bl 0x825a93d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A93D0, LIFTED_ARGS(0x820D61CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_820D61CC:
  // cmpwi cr6,r29,18
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 18, so);
  // bge cr6,0x820d61f8
  if (!cr6.lt) goto loc_820D61F8;
  // addi r31,r30,448
  r31 = uint64_t(int64_t(int64_t(r30) + 448));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // bl 0x820d09b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D09B8, LIFTED_ARGS(0x820D61E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // mr r3,r31
  r3 = r31;
  // mr r4,r26
  r4 = r26;
  // bl 0x82597118
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82597118, LIFTED_ARGS(0x820D61F4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,3504(r30)
  REX_STORE_U32(uint32_t(r30) + 3504, uint32_t(r3));
loc_820D61F8:
  // addi r27,r30,496
  r27 = uint64_t(int64_t(int64_t(r30) + 496));
  // addi r31,r30,1140
  r31 = uint64_t(int64_t(int64_t(r30) + 1140));
  // li r28,6
  r28 = uint64_t(int64_t(6));
loc_820D6204:
  // cmpwi cr6,r29,18
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 18, so);
  // stw r27,-404(r31)
  REX_STORE_U32(uint32_t(r31) + -404, uint32_t(r27));
  // bge cr6,0x820d6220
  if (!cr6.lt) goto loc_820D6220;
  // mr r4,r26
  r4 = r26;
  // addi r3,r30,448
  r3 = uint64_t(int64_t(int64_t(r30) + 448));
  // bl 0x82597118
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82597118, LIFTED_ARGS(0x820D621C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r3));
loc_820D6220:
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,464
  r31 = uint64_t(int64_t(int64_t(r31) + 464));
  // bne 0x820d6204
  if (!cr0.eq) goto loc_820D6204;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_820D6230:
  // addi r1,r1,1168
  r1 = uint64_t(int64_t(int64_t(r1) + 1168));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820D63A0: lifted from generated/ by lift.py.
lifted::Result f820D63A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820D63A0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x82594050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594050, LIFTED_ARGS(0x820D63B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // addi r11,r11,20512
  r11 = uint64_t(int64_t(int64_t(r11) + 20512));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r29,r31,176
  r29 = uint64_t(int64_t(int64_t(r31) + 176));
  // mr r3,r29
  r3 = r29;
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r30,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r30));
  // bl 0x820d6010
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D6010, LIFTED_ARGS(0x820D63E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r31,208
  r3 = uint64_t(int64_t(int64_t(r31) + 208));
  // lfs f31,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stfs f31,3500(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 3500, temp.u32);
  // lwz r11,11840(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 11840);
  // stw r30,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r30));
  // stw r11,3508(r31)
  REX_STORE_U32(uint32_t(r31) + 3508, uint32_t(r11));
  // bl 0x825997a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825997A0, LIFTED_ARGS(0x820D6404, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // bl 0x820d34b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D34B8, LIFTED_ARGS(0x820D640C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfs f31,92(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stw r30,3472(r31)
  REX_STORE_U32(uint32_t(r31) + 3472, uint32_t(r30));
  // mr r3,r29
  r3 = r29;
  // lfs f0,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // ld r10,0(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r10,3488(r31)
  REX_STORE_U64(uint32_t(r31) + 3488, r10);
  // std r11,3496(r31)
  REX_STORE_U64(uint32_t(r31) + 3496, r11);
  // stw r30,3504(r31)
  REX_STORE_U32(uint32_t(r31) + 3504, uint32_t(r30));
  // bl 0x820d0730
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D0730, LIFTED_ARGS(0x820D6448, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r31,496
  r29 = uint64_t(int64_t(int64_t(r31) + 496));
  // addi r30,r31,448
  r30 = uint64_t(int64_t(int64_t(r31) + 448));
  // li r28,2
  r28 = uint64_t(int64_t(2));
loc_820D6454:
  // addi r3,r30,-48
  r3 = uint64_t(int64_t(int64_t(r30) + -48));
  // bl 0x820d0730
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D0730, LIFTED_ARGS(0x820D645C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825970a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825970A8, LIFTED_ARGS(0x820D6464, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r3,r29
  r3 = r29;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820D6478, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r30,r30,24
  r30 = uint64_t(int64_t(int64_t(r30) + 24));
  // addi r29,r29,20
  r29 = uint64_t(int64_t(int64_t(r29) + 20));
  // bne 0x820d6454
  if (!cr0.eq) goto loc_820D6454;
  // addi r30,r31,688
  r30 = uint64_t(int64_t(int64_t(r31) + 688));
  // addi r28,r31,536
  r28 = uint64_t(int64_t(int64_t(r31) + 536));
  // li r29,6
  r29 = uint64_t(int64_t(6));
loc_820D6494:
  // mr r3,r28
  r3 = r28;
  // bl 0x820d0730
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D0730, LIFTED_ARGS(0x820D649C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82594970
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594970, LIFTED_ARGS(0x820D64A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x820ce9c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CE9C8, LIFTED_ARGS(0x820D64AC, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r28,r28,24
  r28 = uint64_t(int64_t(int64_t(r28) + 24));
  // addi r30,r30,464
  r30 = uint64_t(int64_t(int64_t(r30) + 464));
  // bne 0x820d6494
  if (!cr0.eq) goto loc_820D6494;
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820D6A70: lifted from generated/ by lift.py.
lifted::Result f820D6A70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820D6A70 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x82594050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594050, LIFTED_ARGS(0x820D6A88, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,20580
  r11 = uint64_t(int64_t(int64_t(r11) + 20580));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r3,r31,112
  r3 = uint64_t(int64_t(int64_t(r31) + 112));
  // stw r10,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r9,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r9));
  // bl 0x820ce928
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CE928, LIFTED_ARGS(0x820D6AAC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820d6948
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D6948, LIFTED_ARGS(0x820D6AB4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820D7428: lifted from generated/ by lift.py.
lifted::Result f820D7428(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820D7428 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x82594050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594050, LIFTED_ARGS(0x820D743C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // addi r11,r11,20628
  r11 = uint64_t(int64_t(int64_t(r11) + 20628));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r30,r31,96
  r30 = uint64_t(int64_t(int64_t(r31) + 96));
  // mr r3,r30
  r3 = r30;
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r27,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r27));
  // bl 0x820d6f88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D6F88, LIFTED_ARGS(0x820D7464, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r31,5280
  r29 = uint64_t(int64_t(int64_t(r31) + 5280));
  // stw r27,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r27));
  // li r28,216
  r28 = uint64_t(int64_t(216));
loc_820D7470:
  // mr r3,r30
  r3 = r30;
  // bl 0x820d0730
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D0730, LIFTED_ARGS(0x820D7478, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x82594970
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594970, LIFTED_ARGS(0x820D7480, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x820ce9c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CE9C8, LIFTED_ARGS(0x820D7488, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r30,r30,24
  r30 = uint64_t(int64_t(int64_t(r30) + 24));
  // addi r29,r29,464
  r29 = uint64_t(int64_t(int64_t(r29) + 464));
  // bne 0x820d7470
  if (!cr0.eq) goto loc_820D7470;
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // bl 0x820d3558
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3558, LIFTED_ARGS(0x820D74A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // mr r3,r31
  r3 = r31;
  // ori r10,r11,40464
  r10 = r11 | 40464;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,11840(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 11840);
  // stwx r11,r31,r10
  REX_STORE_U32(uint32_t(r31) + uint32_t(r10), uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820D78B8: lifted from generated/ by lift.py.
lifted::Result f820D78B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820D78B8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x82594050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594050, LIFTED_ARGS(0x820D78CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r11,r11,20636
  r11 = uint64_t(int64_t(int64_t(r11) + 20636));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // addi r30,r31,304
  r30 = uint64_t(int64_t(int64_t(r31) + 304));
  // li r29,5
  r29 = uint64_t(int64_t(5));
  // stw r27,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r27));
  // mr r28,r30
  r28 = r30;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
loc_820D78F4:
  // mr r3,r28
  r3 = r28;
  // bl 0x820ce928
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CE928, LIFTED_ARGS(0x820D78FC, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r28,r28,464
  r28 = uint64_t(int64_t(int64_t(r28) + 464));
  // bge 0x820d78f4
  if (!cr0.lt) goto loc_820D78F4;
  // addi r28,r31,160
  r28 = uint64_t(int64_t(int64_t(r31) + 160));
  // stw r27,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r27));
  // li r29,6
  r29 = uint64_t(int64_t(6));
loc_820D7914:
  // mr r3,r28
  r3 = r28;
  // bl 0x820d0730
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D0730, LIFTED_ARGS(0x820D791C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82594970
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594970, LIFTED_ARGS(0x820D7924, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x820ce9c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CE9C8, LIFTED_ARGS(0x820D792C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r28,r28,24
  r28 = uint64_t(int64_t(int64_t(r28) + 24));
  // addi r30,r30,464
  r30 = uint64_t(int64_t(int64_t(r30) + 464));
  // bne 0x820d7914
  if (!cr0.eq) goto loc_820D7914;
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // bl 0x820d3840
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3840, LIFTED_ARGS(0x820D7944, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r3,r31
  r3 = r31;
  // lwz r11,11840(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 11840);
  // stw r27,3088(r31)
  REX_STORE_U32(uint32_t(r31) + 3088, uint32_t(r27));
  // stw r11,3092(r31)
  REX_STORE_U32(uint32_t(r31) + 3092, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820D7D38: lifted from generated/ by lift.py.
lifted::Result f820D7D38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820D7D38 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1184(r1)
  ea = -1184 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,3764(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3764);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x820d7d5c
  if (!cr6.eq) goto loc_820D7D5C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820d7e0c
  goto loc_820D7E0C;
loc_820D7D5C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r24,r31,4064
  r24 = uint64_t(int64_t(int64_t(r31) + 4064));
  // addi r28,r11,20388
  r28 = uint64_t(int64_t(int64_t(r11) + 20388));
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // addi r29,r31,4256
  r29 = uint64_t(int64_t(int64_t(r31) + 4256));
  // addi r27,r11,6528
  r27 = uint64_t(int64_t(int64_t(r11) + 6528));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r25,6
  r25 = uint64_t(int64_t(6));
  // addi r26,r11,20652
  r26 = uint64_t(int64_t(int64_t(r11) + 20652));
loc_820D7D80:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r24,48(r29)
  REX_STORE_U32(uint32_t(r29) + 48, uint32_t(r24));
  // mr r3,r29
  r3 = r29;
  // lwz r4,3728(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 3728);
  // bl 0x820ce9f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CE9F8, LIFTED_ARGS(0x820D7D94, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r7,192(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 192);
  // cmpwi r7,0
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // ble 0x820d7dfc
  if (!cr0.gt) goto loc_820D7DFC;
  // lwz r11,7056(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 7056);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x820d7dfc
  if (cr6.eq) goto loc_820D7DFC;
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // mr r6,r26
  r6 = r26;
  // mr r4,r28
  r4 = r28;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r5,r11,r27
  r5 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r27));
  // bl 0x825a93d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A93D0, LIFTED_ARGS(0x820D7DC8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,4016
  r30 = uint64_t(int64_t(int64_t(r31) + 4016));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // bl 0x820d09b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D09B8, LIFTED_ARGS(0x820D7DD8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // stw r4,7060(r31)
  REX_STORE_U32(uint32_t(r31) + 7060, uint32_t(r4));
  // beq cr6,0x820d7dfc
  if (cr6.eq) goto loc_820D7DFC;
  // mr r3,r30
  r3 = r30;
  // bl 0x82597118
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82597118, LIFTED_ARGS(0x820D7DF0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,460(r29)
  REX_STORE_U32(uint32_t(r29) + 460, uint32_t(r3));
  // lwz r11,7056(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 7056);
  // stw r11,436(r29)
  REX_STORE_U32(uint32_t(r29) + 436, uint32_t(r11));
loc_820D7DFC:
  // addic. r25,r25,-1
  ca = uint32_t(r25) > 0;
  r25 = uint64_t(int64_t(int64_t(r25) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // addi r29,r29,464
  r29 = uint64_t(int64_t(int64_t(r29) + 464));
  // bne 0x820d7d80
  if (!cr0.eq) goto loc_820D7D80;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_820D7E0C:
  // addi r1,r1,1184
  r1 = uint64_t(int64_t(int64_t(r1) + 1184));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820D7EE8: lifted from generated/ by lift.py.
uint64_t f820D7EE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820D7EE8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r30,r3
  r30 = r3;
  // addi r11,r31,16
  r11 = uint64_t(int64_t(int64_t(r31) + 16));
  // subf r7,r31,r30
  r7 = r30 - r31;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // lwz r9,0(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r9,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r9));
loc_820D7F18:
  // mr r9,r11
  r9 = r11;
  // add r8,r7,r11
  r8 = r7 + r11;
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // ld r6,0(r9)
  r6 = REX_LOAD_U64(uint32_t(r9) + 0);
  // std r6,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r6);
  // ld r9,8(r9)
  r9 = REX_LOAD_U64(uint32_t(r9) + 8);
  // std r9,8(r8)
  REX_STORE_U64(uint32_t(r8) + 8, r9);
  // bne 0x820d7f18
  if (!cr0.eq) goto loc_820D7F18;
  // addi r11,r31,64
  r11 = uint64_t(int64_t(int64_t(r31) + 64));
  // li r10,3
  r10 = uint64_t(int64_t(3));
loc_820D7F44:
  // mr r9,r11
  r9 = r11;
  // add r8,r11,r7
  r8 = r11 + r7;
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // ld r6,0(r9)
  r6 = REX_LOAD_U64(uint32_t(r9) + 0);
  // std r6,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r6);
  // ld r9,8(r9)
  r9 = REX_LOAD_U64(uint32_t(r9) + 8);
  // std r9,8(r8)
  REX_STORE_U64(uint32_t(r8) + 8, r9);
  // bne 0x820d7f44
  if (!cr0.eq) goto loc_820D7F44;
  // addi r11,r31,112
  r11 = uint64_t(int64_t(int64_t(r31) + 112));
  // li r10,3
  r10 = uint64_t(int64_t(3));
loc_820D7F70:
  // mr r9,r11
  r9 = r11;
  // add r8,r11,r7
  r8 = r11 + r7;
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // ld r6,0(r9)
  r6 = REX_LOAD_U64(uint32_t(r9) + 0);
  // std r6,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r6);
  // ld r9,8(r9)
  r9 = REX_LOAD_U64(uint32_t(r9) + 8);
  // std r9,8(r8)
  REX_STORE_U64(uint32_t(r8) + 8, r9);
  // bne 0x820d7f70
  if (!cr0.eq) goto loc_820D7F70;
  // lwz r7,160(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 160);
  // addi r10,r31,192
  r10 = uint64_t(int64_t(int64_t(r31) + 192));
  // addi r11,r30,180
  r11 = uint64_t(int64_t(int64_t(r30) + 180));
  // subf r8,r30,r31
  r8 = r31 - r30;
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r7,160(r30)
  REX_STORE_U32(uint32_t(r30) + 160, uint32_t(r7));
  // lwz r7,164(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 164);
  // stw r7,164(r30)
  REX_STORE_U32(uint32_t(r30) + 164, uint32_t(r7));
loc_820D7FB4:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r8,r11
  r3 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d7fb4
  if (!cr0.eq) goto loc_820D7FB4;
  // addi r10,r31,384
  r10 = uint64_t(int64_t(int64_t(r31) + 384));
  // addi r11,r30,372
  r11 = uint64_t(int64_t(int64_t(r30) + 372));
  // li r9,3
  r9 = uint64_t(int64_t(3));
loc_820D8020:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r11,r8
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d8020
  if (!cr0.eq) goto loc_820D8020;
  // addi r10,r31,576
  r10 = uint64_t(int64_t(int64_t(r31) + 576));
  // addi r11,r30,564
  r11 = uint64_t(int64_t(int64_t(r30) + 564));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_820D808C:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r8,r11
  r3 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d808c
  if (!cr0.eq) goto loc_820D808C;
  // addi r10,r31,960
  r10 = uint64_t(int64_t(int64_t(r31) + 960));
  // addi r11,r30,948
  r11 = uint64_t(int64_t(int64_t(r30) + 948));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_820D80F8:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r11,r8
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d80f8
  if (!cr0.eq) goto loc_820D80F8;
  // addi r10,r31,1344
  r10 = uint64_t(int64_t(int64_t(r31) + 1344));
  // addi r11,r30,1332
  r11 = uint64_t(int64_t(int64_t(r30) + 1332));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_820D8164:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r11,r8
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d8164
  if (!cr0.eq) goto loc_820D8164;
  // addi r10,r31,1728
  r10 = uint64_t(int64_t(int64_t(r31) + 1728));
  // addi r11,r30,1716
  r11 = uint64_t(int64_t(int64_t(r30) + 1716));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_820D81D0:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r8,r11
  r3 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d81d0
  if (!cr0.eq) goto loc_820D81D0;
  // addi r10,r31,2112
  r10 = uint64_t(int64_t(int64_t(r31) + 2112));
  // addi r11,r30,2100
  r11 = uint64_t(int64_t(int64_t(r30) + 2100));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_820D823C:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r11,r8
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d823c
  if (!cr0.eq) goto loc_820D823C;
  // addi r10,r31,2496
  r10 = uint64_t(int64_t(int64_t(r31) + 2496));
  // addi r11,r30,2484
  r11 = uint64_t(int64_t(int64_t(r30) + 2484));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_820D82A8:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r8,r11
  r3 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d82a8
  if (!cr0.eq) goto loc_820D82A8;
  // addi r10,r31,2880
  r10 = uint64_t(int64_t(int64_t(r31) + 2880));
  // addi r11,r30,2868
  r11 = uint64_t(int64_t(int64_t(r30) + 2868));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_820D8314:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r11,r8
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d8314
  if (!cr0.eq) goto loc_820D8314;
  // addi r10,r31,3264
  r10 = uint64_t(int64_t(int64_t(r31) + 3264));
  // addi r11,r30,3252
  r11 = uint64_t(int64_t(int64_t(r30) + 3252));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_820D8380:
  // lwz r5,-16(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -16);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r6,r10,32
  r6 = uint64_t(int64_t(int64_t(r10) + 32));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r5,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r5));
  // mr r5,r4
  r5 = r4;
  // lwzx r3,r8,r11
  r3 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // lwz r4,-8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + -8);
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r4,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r4);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r4,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r4);
  // lfs f0,16(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f0 = double(temp.f32);
  // addi r10,r10,64
  r10 = uint64_t(int64_t(int64_t(r10) + 64));
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // ld r7,0(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 0);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // std r7,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r7);
  // ld r7,8(r6)
  r7 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r7);
  // bne 0x820d8380
  if (!cr0.eq) goto loc_820D8380;
  // addi r4,r31,3632
  r4 = uint64_t(int64_t(int64_t(r31) + 3632));
  // addi r3,r30,3632
  r3 = uint64_t(int64_t(int64_t(r30) + 3632));
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820D83F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3680(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3680);
  // mr r3,r30
  r3 = r30;
  // stw r11,3680(r30)
  REX_STORE_U32(uint32_t(r30) + 3680, uint32_t(r11));
  // lwz r11,3684(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3684);
  // stw r11,3684(r30)
  REX_STORE_U32(uint32_t(r30) + 3684, uint32_t(r11));
  // lwz r11,3688(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3688);
  // stw r11,3688(r30)
  REX_STORE_U32(uint32_t(r30) + 3688, uint32_t(r11));
  // lwz r11,3692(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3692);
  // stw r11,3692(r30)
  REX_STORE_U32(uint32_t(r30) + 3692, uint32_t(r11));
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

// sub_820D8908: lifted from generated/ by lift.py.
lifted::Result f820D8908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820D8908 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x82594050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594050, LIFTED_ARGS(0x820D8920, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,20660
  r11 = uint64_t(int64_t(int64_t(r11) + 20660));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r3,r31,32
  r3 = uint64_t(int64_t(int64_t(r31) + 32));
  // stw r10,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r9,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r9));
  // bl 0x820d45b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D45B0, LIFTED_ARGS(0x820D8944, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,3728
  r3 = uint64_t(int64_t(int64_t(r31) + 3728));
  // bl 0x820d7c40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D7C40, LIFTED_ARGS(0x820D894C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r11,r31,7072
  r11 = uint64_t(int64_t(int64_t(r31) + 7072));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // lfs f0,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f0 = double(temp.f32);
loc_820D8960:
  // stfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // bge 0x820d8960
  if (!cr0.lt) goto loc_820D8960;
  // addi r11,r31,7136
  r11 = uint64_t(int64_t(int64_t(r31) + 7136));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
loc_820D897C:
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
  // stfs f0,32(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // bge 0x820d897c
  if (!cr0.lt) goto loc_820D897C;
  // mr r3,r31
  r3 = r31;
  // bl 0x820d7ab0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D7AB0, LIFTED_ARGS(0x820D89A0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820D9570: lifted from generated/ by lift.py.
lifted::Result f820D9570(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820D9570 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x82594050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594050, LIFTED_ARGS(0x820D958C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,20736
  r11 = uint64_t(int64_t(int64_t(r11) + 20736));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r30,r31,28
  r30 = uint64_t(int64_t(int64_t(r31) + 28));
  // addi r3,r30,44
  r3 = uint64_t(int64_t(int64_t(r30) + 44));
  // stw r10,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r9,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r9));
  // bl 0x820d3b38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3B38, LIFTED_ARGS(0x820D95B4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r30,56
  r3 = uint64_t(int64_t(int64_t(r30) + 56));
  // bl 0x820d3b38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D3B38, LIFTED_ARGS(0x820D95BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,192
  r3 = uint64_t(int64_t(int64_t(r31) + 192));
  // bl 0x820d8f60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D8F60, LIFTED_ARGS(0x820D95C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820d8b58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D8B58, LIFTED_ARGS(0x820D95CC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820DB290: lifted from generated/ by lift.py.
uint64_t f820DB290(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820DB290 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // addi r4,r30,16
  r4 = uint64_t(int64_t(int64_t(r30) + 16));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x820d7ee8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820D7EE8, LIFTED_ARGS(0x820DB2C0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r30,3712
  r4 = uint64_t(int64_t(int64_t(r30) + 3712));
  // addi r3,r31,3712
  r3 = uint64_t(int64_t(int64_t(r31) + 3712));
  // li r5,60
  r5 = uint64_t(int64_t(60));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820DB2D0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r30,3772
  r4 = uint64_t(int64_t(int64_t(r30) + 3772));
  // addi r3,r31,3772
  r3 = uint64_t(int64_t(int64_t(r31) + 3772));
  // li r5,120
  r5 = uint64_t(int64_t(120));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820DB2E0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r30,3892
  r4 = uint64_t(int64_t(int64_t(r30) + 3892));
  // addi r3,r31,3892
  r3 = uint64_t(int64_t(int64_t(r31) + 3892));
  // li r5,144
  r5 = uint64_t(int64_t(144));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820DB2F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r30,4036
  r4 = uint64_t(int64_t(int64_t(r30) + 4036));
  // addi r3,r31,4036
  r3 = uint64_t(int64_t(int64_t(r31) + 4036));
  // li r5,156
  r5 = uint64_t(int64_t(156));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820DB300, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r30,4192
  r4 = uint64_t(int64_t(int64_t(r30) + 4192));
  // addi r3,r31,4192
  r3 = uint64_t(int64_t(int64_t(r31) + 4192));
  // li r5,52
  r5 = uint64_t(int64_t(52));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820DB310, r31)); r3 = o.r3; f1 = o.f1; }
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
  return r3;
}

// sub_820F0070: lifted from generated/ by lift.py.
lifted::Result f820F0070(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820F0070 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r11,260(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 260);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x820f0110
  if (!cr6.eq) goto loc_820F0110;
  // lwz r11,192(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 192);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x820f0110
  if (!cr6.eq) goto loc_820F0110;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r4,196(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 196);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x8259a9e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259A9E0, LIFTED_ARGS(0x820F00A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820f0110
  if (!cr0.gt) goto loc_820F0110;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r28,r11,26384
  r28 = uint64_t(int64_t(int64_t(r11) + 26384));
loc_820F00C8:
  // lwz r11,84(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 84);
  // mr r4,r28
  r4 = r28;
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x825a98a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A98A8, LIFTED_ARGS(0x820F00D8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820f00fc
  if (!cr0.eq) goto loc_820F00FC;
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,160
  r30 = uint64_t(int64_t(int64_t(r30) + 160));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // blt cr6,0x820f00c8
  if (cr6.lt) goto loc_820F00C8;
  // b 0x820f0100
  goto loc_820F0100;
loc_820F00FC:
  // stw r29,256(r31)
  REX_STORE_U32(uint32_t(r31) + 256, uint32_t(r29));
loc_820F0100:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r11,260(r31)
  REX_STORE_U32(uint32_t(r31) + 260, uint32_t(r11));
  // bl 0x820efb30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EFB30, LIFTED_ARGS(0x820F0110, r31)); r3 = o.r3; f1 = o.f1; }
loc_820F0110:
  // lwz r3,260(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 260);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820F4128: lifted from generated/ by lift.py.
uint64_t f820F4128(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r9, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_820F4128 (lifted)");
  uint64_t r7 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{};
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-152
  r12 = uint64_t(int64_t(int64_t(r1) + -152));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1984(r1)
  ea = -1984 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r22,r3,216
  r22 = uint64_t(int64_t(int64_t(r3) + 216));
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // mr r20,r4
  r20 = r4;
  // fmr f29,f2
  f29 = f2;
  // mr r25,r5
  r25 = r5;
  // fmr f28,f3
  f28 = f3;
  // mr r21,r6
  r21 = r6;
  // mr r19,r9
  r19 = r9;
  // lwz r11,0(r22)
  r11 = REX_LOAD_U32(uint32_t(r22) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bgt cr6,0x820f4170
  if (cr6.gt) goto loc_820F4170;
loc_820F4168:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820f44f4
  goto loc_820F44F4;
loc_820F4170:
  // cmpwi cr6,r21,32
  cr6 = lifted::Compare<int32_t>(int32_t(r21), 32, so);
  // bgt cr6,0x820f4168
  if (cr6.gt) goto loc_820F4168;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // li r10,32
  r10 = uint64_t(int64_t(32));
  // addi r11,r1,236
  r11 = uint64_t(int64_t(int64_t(r1) + 236));
  // lfs f31,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f31 = double(temp.f32);
loc_820F4188:
  // stfs f31,-16(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + -16, temp.u32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stfs f31,0(r11)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,48
  r11 = uint64_t(int64_t(int64_t(r11) + 48));
  // bge 0x820f4188
  if (!cr0.lt) goto loc_820F4188;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // li r24,0
  r24 = uint64_t(int64_t(0));
loc_820F41A8:
  // lwz r11,8(r22)
  r11 = REX_LOAD_U32(uint32_t(r22) + 8);
  // add r11,r24,r11
  r11 = r24 + r11;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r29,80(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 80);
  // cmplwi r29,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq 0x820f4474
  if (cr0.eq) goto loc_820F4474;
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lbz r10,0(r29)
  r10 = REX_LOAD_U8(uint32_t(r29) + 0);
  // mr r9,r11
  r9 = r11;
  // rlwinm r8,r11,24,16,23
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xFF00;
  // rlwimi r9,r11,16,0,15
  r9 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF0000) | (r9 & 0xFFFFFFFF0000FFFF);
  // rlwinm r11,r9,8,0,15
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 8) & 0xFFFF0000;
  // or r11,r11,r8
  r11 = r11 | r8;
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820f4474
  if (cr0.eq) goto loc_820F4474;
loc_820F41E8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r27
  r4 = r27;
  // mr r3,r29
  r3 = r29;
  // bl 0x82294730
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294730, LIFTED_ARGS(0x820F41F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r11,r3,16
  r11 = uint64_t(int64_t(int64_t(r3) + 16));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x820f4448
  if (cr0.eq) goto loc_820F4448;
  // addi r11,r11,44
  r11 = uint64_t(int64_t(int64_t(r11) + 44));
  // mr r26,r10
  r26 = r10;
  // addi r28,r11,16
  r28 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
loc_820F4218:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x820f4440
  if (cr6.eq) goto loc_820F4440;
  // mulli r11,r31,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(48))));
  // lfs f9,0(r25)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r25) + 0);
  f9 = double(temp.f32);
  // addi r10,r1,216
  r10 = uint64_t(int64_t(int64_t(r1) + 216));
  // lfs f8,4(r25)
  temp.u32 = REX_LOAD_U32(uint32_t(r25) + 4);
  f8 = double(temp.f32);
  // lfs f7,8(r25)
  temp.u32 = REX_LOAD_U32(uint32_t(r25) + 8);
  f7 = double(temp.f32);
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // addi r4,r28,8
  r4 = uint64_t(int64_t(int64_t(r28) + 8));
  // mr r3,r30
  r3 = r30;
  // add r10,r11,r10
  r10 = r11 + r10;
loc_820F4244:
  // lfs f0,-4(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + -4);
  f0 = double(temp.f32);
  // lfs f13,0(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f13 = double(temp.f32);
  // fsubs f0,f0,f8
  f0 = double(float(f0 - f8));
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // fsubs f0,f13,f7
  f0 = double(float(f13 - f7));
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // lfs f0,-8(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + -8);
  f0 = double(temp.f32);
  // fsubs f0,f0,f9
  f0 = double(float(f0 - f9));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lvx128 v7,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v7.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // vmsum3fp128 v13,v7,v7
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v7.f32), 0xEF));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f10,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 144);
  f10 = double(temp.f32);
  // fcmpu cr6,f10,f29
  cr6 = lifted::CompareFloat(f10, f29);
  // bgt cr6,0x820f4434
  if (cr6.gt) goto loc_820F4434;
  // vspltisw v12,1
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_set1_epi32(int(0x1)));
  // vrsqrtefp v0,v13
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v13.f32))));
  // vspltisw v9,0
  simde_mm_store_si128((simde__m128i*)v9.u32, simde_mm_set1_epi32(int(0x0)));
  // lvx128 v10,r0,r19
  ea = (uint32_t(r19)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // vcfsx v11,v12,1
  simde_mm_store_ps(v11.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v12.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vcmpeqfp v12,v13,v9
  simde_mm_store_ps(v12.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v9.f32)));
  // vmulfp128 v9,v0,v0
  simde_mm_store_ps(v9.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v0.f32)));
  // vmulfp128 v8,v13,v11
  simde_mm_store_ps(v8.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v11.f32)));
  // vnmsubfp v11,v8,v9,v11
  simde_mm_store_ps(v11.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v8.f32), simde_mm_load_ps(v9.f32)), simde_mm_load_ps(v11.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v0,v0,v11,v0
  simde_mm_store_ps(v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v11.f32)), simde_mm_load_ps(v0.f32)));
  // vmulfp128 v0,v7,v0
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v0.f32)));
  // vsel v12,v0,v13,v12
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)v0.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)v13.u8))));
  // vmsum3fp128 v0,v12,v10
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32), 0xEF));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f0,112(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 112);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f28
  cr6 = lifted::CompareFloat(f0, f28);
  // blt cr6,0x820f4434
  if (cr6.lt) goto loc_820F4434;
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // ld r8,-8(r4)
  r8 = REX_LOAD_U64(uint32_t(r4) + -8);
  // ld r7,0(r4)
  r7 = REX_LOAD_U64(uint32_t(r4) + 0);
  // mr r9,r31
  r9 = r31;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // ld r6,0(r11)
  r6 = REX_LOAD_U64(uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r8,-8(r10)
  REX_STORE_U64(uint32_t(r10) + -8, r8);
  // std r7,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r7);
  // std r6,8(r10)
  REX_STORE_U64(uint32_t(r10) + 8, r6);
  // std r11,16(r10)
  REX_STORE_U64(uint32_t(r10) + 16, r11);
  // stfs f10,24(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 24, temp.u32);
  // ble cr6,0x820f436c
  if (!cr6.gt) goto loc_820F436C;
  // lfs f0,-8(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -8);
  f0 = double(temp.f32);
  // fmuls f11,f30,f30
  f11 = double(float(f30 * f30));
  // lfs f13,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f13 = double(temp.f32);
  // addi r11,r10,-48
  r11 = uint64_t(int64_t(int64_t(r10) + -48));
  // lfs f12,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f12 = double(temp.f32);
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
loc_820F431C:
  // lfs f6,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -8);
  f6 = double(temp.f32);
  // fsubs f6,f0,f6
  f6 = double(float(f0 - f6));
  // stfs f6,96(r1)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // lfs f5,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f5 = double(temp.f32);
  // lfs f6,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f6 = double(temp.f32);
  // fsubs f5,f13,f5
  f5 = double(float(f13 - f5));
  // fsubs f6,f12,f6
  f6 = double(float(f12 - f6));
  // stfs f6,104(r1)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // stfs f5,100(r1)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvx128 v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // vmsum3fp128 v0,v0,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f6,128(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 128);
  f6 = double(temp.f32);
  // fcmpu cr6,f6,f11
  cr6 = lifted::CompareFloat(f6, f11);
  // ble cr6,0x820f4504
  if (!cr6.gt) goto loc_820F4504;
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // addi r11,r11,-48
  r11 = uint64_t(int64_t(int64_t(r11) + -48));
  // bgt 0x820f431c
  if (cr0.gt) goto loc_820F431C;
loc_820F436C:
  // mr r5,r31
  r5 = r31;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // ble cr6,0x820f4424
  if (!cr6.gt) goto loc_820F4424;
  // addi r11,r10,-24
  r11 = uint64_t(int64_t(int64_t(r10) + -24));
loc_820F437C:
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f13,48(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 48);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x820f4424
  if (!cr6.gt) goto loc_820F4424;
  // ld r18,16(r11)
  r18 = REX_LOAD_U64(uint32_t(r11) + 16);
  // addi r9,r1,160
  r9 = uint64_t(int64_t(int64_t(r1) + 160));
  // ld r17,24(r11)
  r17 = REX_LOAD_U64(uint32_t(r11) + 24);
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // ld r16,32(r11)
  r16 = REX_LOAD_U64(uint32_t(r11) + 32);
  // stfs f31,172(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 172, temp.u32);
  // ld r15,40(r11)
  r15 = REX_LOAD_U64(uint32_t(r11) + 40);
  // stfs f31,188(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 188, temp.u32);
  // ld r14,-32(r11)
  r14 = REX_LOAD_U64(uint32_t(r11) + -32);
  // addi r7,r1,160
  r7 = uint64_t(int64_t(int64_t(r1) + 160));
  // std r18,0(r9)
  REX_STORE_U64(uint32_t(r9) + 0, r18);
  // addi r6,r1,176
  r6 = uint64_t(int64_t(int64_t(r1) + 176));
  // std r17,8(r9)
  REX_STORE_U64(uint32_t(r9) + 8, r17);
  // std r16,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r16);
  // fmr f0,f13
  f0 = f13;
  // std r15,8(r8)
  REX_STORE_U64(uint32_t(r8) + 8, r15);
  // std r14,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r14);
  // stfs f0,192(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 192, temp.u32);
  // ld r9,-24(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + -24);
  // addic. r5,r5,-1
  ca = uint32_t(r5) > 0;
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // ld r8,0(r7)
  r8 = REX_LOAD_U64(uint32_t(r7) + 0);
  // ld r7,8(r7)
  r7 = REX_LOAD_U64(uint32_t(r7) + 8);
  // ld r18,0(r6)
  r18 = REX_LOAD_U64(uint32_t(r6) + 0);
  // ld r6,8(r6)
  r6 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r9,24(r11)
  REX_STORE_U64(uint32_t(r11) + 24, r9);
  // ld r9,-16(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + -16);
  // std r9,32(r11)
  REX_STORE_U64(uint32_t(r11) + 32, r9);
  // ld r9,-8(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + -8);
  // std r9,40(r11)
  REX_STORE_U64(uint32_t(r11) + 40, r9);
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // stfs f13,48(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // std r8,-32(r11)
  REX_STORE_U64(uint32_t(r11) + -32, r8);
  // std r7,-24(r11)
  REX_STORE_U64(uint32_t(r11) + -24, r7);
  // std r18,-16(r11)
  REX_STORE_U64(uint32_t(r11) + -16, r18);
  // std r6,-8(r11)
  REX_STORE_U64(uint32_t(r11) + -8, r6);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,-48
  r11 = uint64_t(int64_t(int64_t(r11) + -48));
  // bgt 0x820f437c
  if (cr0.gt) goto loc_820F437C;
loc_820F4424:
  // cmpw cr6,r31,r21
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r21), so);
  // bge cr6,0x820f4434
  if (!cr6.lt) goto loc_820F4434;
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
loc_820F4434:
  // addic. r3,r3,-1
  ca = uint32_t(r3) > 0;
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // addi r4,r4,48
  r4 = uint64_t(int64_t(int64_t(r4) + 48));
  // bne 0x820f4244
  if (!cr0.eq) goto loc_820F4244;
loc_820F4440:
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // bne 0x820f4218
  if (!cr0.eq) goto loc_820F4218;
loc_820F4448:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // lbz r10,0(r29)
  r10 = REX_LOAD_U8(uint32_t(r29) + 0);
  // mr r9,r11
  r9 = r11;
  // rlwinm r8,r11,24,16,23
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xFF00;
  // rlwimi r9,r11,16,0,15
  r9 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF0000) | (r9 & 0xFFFFFFFF0000FFFF);
  // rlwinm r11,r9,8,0,15
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 8) & 0xFFFF0000;
  // or r11,r11,r8
  r11 = r11 | r8;
  // or r11,r11,r10
  r11 = r11 | r10;
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // blt cr6,0x820f41e8
  if (cr6.lt) goto loc_820F41E8;
loc_820F4474:
  // lwz r11,0(r22)
  r11 = REX_LOAD_U32(uint32_t(r22) + 0);
  // addi r23,r23,1
  r23 = uint64_t(int64_t(int64_t(r23) + 1));
  // addi r24,r24,272
  r24 = uint64_t(int64_t(int64_t(r24) + 272));
  // cmplw cr6,r23,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), uint32_t(r11), so);
  // blt cr6,0x820f41a8
  if (cr6.lt) goto loc_820F41A8;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // ble cr6,0x820f44f0
  if (!cr6.gt) goto loc_820F44F0;
  // addi r10,r1,216
  r10 = uint64_t(int64_t(int64_t(r1) + 216));
  // addi r11,r20,24
  r11 = uint64_t(int64_t(int64_t(r20) + 24));
  // mr r9,r31
  r9 = r31;
loc_820F449C:
  // lfs f0,-8(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -8);
  f0 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // lfs f13,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f13 = double(temp.f32);
  // lfs f12,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f12 = double(temp.f32);
  // lfs f11,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f11 = double(temp.f32);
  // stfs f0,-24(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -24, temp.u32);
  // stfs f13,-20(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + -20, temp.u32);
  // stfs f12,-16(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + -16, temp.u32);
  // stfs f11,-12(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + -12, temp.u32);
  // lfs f0,8(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8);
  f0 = double(temp.f32);
  // lfs f13,12(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 12);
  f13 = double(temp.f32);
  // lfs f12,16(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f12 = double(temp.f32);
  // lfs f10,20(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 20);
  f10 = double(temp.f32);
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // stfs f31,-12(r11)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + -12, temp.u32);
  // stfs f0,-8(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -8, temp.u32);
  // stfs f13,-4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // stfs f12,0(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f10,4(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne 0x820f449c
  if (!cr0.eq) goto loc_820F449C;
loc_820F44F0:
  // mr r3,r31
  r3 = r31;
loc_820F44F4:
  // addi r1,r1,1984
  r1 = uint64_t(int64_t(int64_t(r1) + 1984));
  // addi r12,r1,-152
  r12 = uint64_t(int64_t(int64_t(r1) + -152));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ea0
  // (left out: dead in C++)
  return r3;
loc_820F4504:
  // mulli r11,r9,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(48))));
  // addi r9,r1,240
  r9 = uint64_t(int64_t(int64_t(r1) + 240));
  // add r9,r11,r9
  r9 = r11 + r9;
  // lfs f0,-48(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -48);
  f0 = double(temp.f32);
  // fcmpu cr6,f10,f0
  cr6 = lifted::CompareFloat(f10, f0);
  // bge cr6,0x820f4434
  if (!cr6.lt) goto loc_820F4434;
  // addi r9,r1,208
  r9 = uint64_t(int64_t(int64_t(r1) + 208));
  // add r11,r11,r9
  r11 = r11 + r9;
  // ld r9,-8(r10)
  r9 = REX_LOAD_U64(uint32_t(r10) + -8);
  // addi r11,r11,-48
  r11 = uint64_t(int64_t(int64_t(r11) + -48));
  // std r9,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r9);
  // ld r9,0(r10)
  r9 = REX_LOAD_U64(uint32_t(r10) + 0);
  // std r9,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r9);
  // ld r9,8(r10)
  r9 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r9,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r9);
  // ld r9,16(r10)
  r9 = REX_LOAD_U64(uint32_t(r10) + 16);
  // std r9,24(r11)
  REX_STORE_U64(uint32_t(r11) + 24, r9);
  // lfs f0,24(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 24);
  f0 = double(temp.f32);
  // stfs f0,32(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // b 0x820f4434
  goto loc_820F4434;
}

// sub_820F8618: lifted from generated/ by lift.py.
lifted::Result f820F8618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820F8618 (lifted)");
  uint64_t r4 = 0, r5 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295330, LIFTED_ARGS(0x820F864C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
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

// sub_820FEFB0: lifted from generated/ by lift.py.
lifted::Result f820FEFB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820FEFB0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x820b86b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B86B8, LIFTED_ARGS(0x820FEFC0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,23324
  r3 = uint64_t(int64_t(int64_t(r11) + 23324));
  // bl 0x820f6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F6940, LIFTED_ARGS(0x820FEFCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,23312
  r3 = uint64_t(int64_t(int64_t(r11) + 23312));
  // bl 0x820f8618
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8618, LIFTED_ARGS(0x820FEFD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23336(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23336);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820FEFE4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820FFD88: lifted from generated/ by lift.py.
uint64_t f820FFD88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820FFD88 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // cmpwi cr6,r5,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r5), -1, so);
  // bne cr6,0x820ffda8
  if (!cr6.eq) goto loc_820FFDA8;
  // lwz r5,804(r4)
  r5 = REX_LOAD_U32(uint32_t(r4) + 804);
loc_820FFDA8:
  // mulli r11,r5,80
  r11 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(80))));
  // add r4,r11,r4
  r4 = r11 + r4;
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // mr r3,r31
  r3 = r31;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820FFDBC, r31)); r3 = o.r3; f1 = o.f1; }
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
  return r3;
}

// sub_8211AED0: lifted from generated/ by lift.py.
uint64_t f8211AED0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8211AED0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // addi r4,r30,16
  r4 = uint64_t(int64_t(int64_t(r30) + 16));
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r5,64
  r5 = uint64_t(int64_t(64));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // stfs f0,28(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // stfs f0,44(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 44, temp.u32);
  // stfs f0,60(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 60, temp.u32);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8211AF1C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,84
  r11 = uint64_t(int64_t(int64_t(r30) + 84));
  // lfs f0,80(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 80);
  f0 = double(temp.f32);
  // stfs f0,80(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 80, temp.u32);
  // addi r10,r31,84
  r10 = uint64_t(int64_t(int64_t(r31) + 84));
  // addi r9,r30,100
  r9 = uint64_t(int64_t(int64_t(r30) + 100));
  // addi r8,r31,100
  r8 = uint64_t(int64_t(int64_t(r31) + 100));
  // mr r3,r31
  r3 = r31;
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r7,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r7));
  // lwz r7,4(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r7,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r7));
  // lwz r7,8(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r7,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r7));
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // stw r11,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r11));
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // stw r11,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r11));
  // lwz r11,4(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 4);
  // stw r11,4(r8)
  REX_STORE_U32(uint32_t(r8) + 4, uint32_t(r11));
  // lwz r11,8(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 8);
  // stw r11,8(r8)
  REX_STORE_U32(uint32_t(r8) + 8, uint32_t(r11));
  // lwz r11,12(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 12);
  // stw r11,12(r8)
  REX_STORE_U32(uint32_t(r8) + 12, uint32_t(r11));
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

// sub_8211BD10: lifted from generated/ by lift.py.
uint64_t f8211BD10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_8211BD10 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -4096);
  // ld r12,-8192(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -8192);
  // ld r12,-12288(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -12288);
  // ld r12,-16384(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -16384);
  // stwu r1,-16800(r1)
  ea = -16800 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r25,r3
  r25 = r3;
  // mr r24,r4
  r24 = r4;
  // mr r30,r5
  r30 = r5;
  // mr r26,r6
  r26 = r6;
  // mr r23,r7
  r23 = r7;
  // li r11,63
  r11 = uint64_t(int64_t(63));
  // addi r10,r1,336
  r10 = uint64_t(int64_t(int64_t(r1) + 336));
loc_8211BD48:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // addi r10,r10,260
  r10 = uint64_t(int64_t(int64_t(r10) + 260));
  // bge 0x8211bd48
  if (!cr0.lt) goto loc_8211BD48;
  // lwz r10,104(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 104);
  // lwz r11,100(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 100);
  // add r10,r10,r11
  r10 = r10 + r11;
  // addi r31,r10,-1
  r31 = uint64_t(int64_t(int64_t(r10) + -1));
  // b 0x8211bdb4
  goto loc_8211BDB4;
loc_8211BD70:
  // mr r4,r31
  r4 = r31;
  // mr r3,r24
  r3 = r24;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS(0x8211BD7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,72(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 72);
  // cmpwi cr6,r11,63
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 63, so);
  // blt cr6,0x8211bd8c
  if (cr6.lt) goto loc_8211BD8C;
  // li r11,63
  r11 = uint64_t(int64_t(63));
loc_8211BD8C:
  // mulli r11,r11,260
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(260))));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,256(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 256);
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,256(r11)
  REX_STORE_U32(uint32_t(r11) + 256, uint32_t(r9));
  // stwx r3,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r3));
  // lwz r11,100(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 100);
loc_8211BDB4:
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // bge cr6,0x8211bd70
  if (!cr6.lt) goto loc_8211BD70;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r29,r1,336
  r29 = uint64_t(int64_t(int64_t(r1) + 336));
loc_8211BDC4:
  // lwz r28,0(r29)
  r28 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi r28,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq 0x8211be7c
  if (cr0.eq) goto loc_8211BE7C;
  // addi r31,r29,-256
  r31 = uint64_t(int64_t(int64_t(r29) + -256));
loc_8211BDD8:
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmpwi cr6,r23,0
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // beq cr6,0x8211bde8
  if (cr6.eq) goto loc_8211BDE8;
  // mr r26,r27
  r26 = r27;
loc_8211BDE8:
  // lis r10,18258
  r10 = uint64_t(int64_t(1196556288));
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // ori r10,r10,20480
  r10 = r10 | 20480;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8211be54
  if (cr6.eq) goto loc_8211BE54;
  // lis r10,20553
  r10 = uint64_t(int64_t(1346961408));
  // ori r10,r10,17152
  r10 = r10 | 17152;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8211be3c
  if (cr6.eq) goto loc_8211BE3C;
  // lis r10,20562
  r10 = uint64_t(int64_t(1347551232));
  // ori r10,r10,19712
  r10 = r10 | 19712;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8211be3c
  if (cr6.eq) goto loc_8211BE3C;
  // lis r10,21315
  r10 = uint64_t(int64_t(1396899840));
  // ori r10,r10,17664
  r10 = r10 | 17664;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8211be4c
  if (cr6.eq) goto loc_8211BE4C;
  // lis r10,21592
  r10 = uint64_t(int64_t(1415053312));
  // ori r10,r10,21504
  r10 = r10 | 21504;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8211be6c
  if (!cr6.eq) goto loc_8211BE6C;
loc_8211BE3C:
  // mr r5,r26
  r5 = r26;
  // mr r3,r25
  r3 = r25;
  // bl 0x8211a660
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A660, LIFTED_ARGS(0x8211BE48, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8211be6c
  goto loc_8211BE6C;
loc_8211BE4C:
  // mr r5,r4
  r5 = r4;
  // b 0x8211be5c
  goto loc_8211BE5C;
loc_8211BE54:
  // mr r5,r4
  r5 = r4;
  // mr r4,r24
  r4 = r24;
loc_8211BE5C:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r26
  r6 = r26;
  // mr r3,r25
  r3 = r25;
  // bl 0x8211bd10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BD10, LIFTED_ARGS(0x8211BE6C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8211BE6C:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplw cr6,r30,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r28), so);
  // blt cr6,0x8211bdd8
  if (cr6.lt) goto loc_8211BDD8;
loc_8211BE7C:
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r29,r29,260
  r29 = uint64_t(int64_t(int64_t(r29) + 260));
  // cmplwi cr6,r27,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 64, so);
  // blt cr6,0x8211bdc4
  if (cr6.lt) goto loc_8211BDC4;
  // addi r1,r1,16800
  r1 = uint64_t(int64_t(int64_t(r1) + 16800));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return r3;
}

// sub_82128E70: lifted from generated/ by lift.py.
lifted::Result f82128E70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82128E70 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r31,r11,23900
  r31 = uint64_t(int64_t(int64_t(r11) + 23900));
  // mr r22,r5
  r22 = r5;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8258c9d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258C9D0, LIFTED_ARGS(0x82128E94, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // li r25,5
  r25 = uint64_t(int64_t(5));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // sth r25,2(r24)
  REX_STORE_U16(uint32_t(r24) + 2, uint16_t(r25));
  // stw r30,4(r24)
  REX_STORE_U32(uint32_t(r24) + 4, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128EC0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r23,4
  r23 = uint64_t(int64_t(4));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // stw r23,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r23));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128EE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // lwz r10,112(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 112);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128F00, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r28));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r25,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r25));
  // stw r30,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r30));
  // lwz r11,116(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 116);
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82128fa4
  if (cr6.eq) goto loc_82128FA4;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128F24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,56
  r10 = uint64_t(int64_t(56));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r26,6
  r26 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r26,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r26));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128F44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,96
  r10 = uint64_t(int64_t(96));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128F60, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,60
  r10 = uint64_t(int64_t(60));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128F7C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,72
  r10 = uint64_t(int64_t(72));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r26,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r26));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128F98, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // stw r10,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r10));
  // b 0x82129020
  goto loc_82129020;
loc_82128FA4:
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128FA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,56
  r10 = uint64_t(int64_t(56));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r26,6
  r26 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r26,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r26));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128FC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,96
  r10 = uint64_t(int64_t(96));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82128FE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,60
  r10 = uint64_t(int64_t(60));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82129000, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,72
  r10 = uint64_t(int64_t(72));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8212901C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r30));
loc_82129020:
  // li r11,76
  r11 = uint64_t(int64_t(76));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82129034, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r27,2
  r27 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // stw r27,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r27));
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82129054, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r28,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r28));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82129074, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r28,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r28));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82129094, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x821290B4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r26,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r26));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // stw r27,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r27));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x821290D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // stw r25,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r25));
  // stw r23,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r23));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258cb08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CB08, LIFTED_ARGS(0x821290F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // mr r11,r3
  r11 = r3;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // addi r9,r1,124
  r9 = uint64_t(int64_t(int64_t(r1) + 124));
  // lfs f0,2608(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 2608);
  f0 = double(temp.f32);
loc_82129104:
  // stfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // bge 0x82129104
  if (!cr0.lt) goto loc_82129104;
  // lwz r9,80(r29)
  r9 = REX_LOAD_U32(uint32_t(r29) + 80);
  // lfs f13,100(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 100);
  f13 = double(temp.f32);
  // lwz r6,88(r29)
  r6 = REX_LOAD_U32(uint32_t(r29) + 88);
  // lfs f12,96(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 96);
  f12 = double(temp.f32);
  // lwz r8,84(r29)
  r8 = REX_LOAD_U32(uint32_t(r29) + 84);
  // extsw r5,r9
  r5 = uint64_t(int64_t(int32_t(r9)));
  // lwz r7,92(r29)
  r7 = REX_LOAD_U32(uint32_t(r29) + 92);
  // add r9,r6,r9
  r9 = r6 + r9;
  // extsw r4,r8
  r4 = uint64_t(int64_t(int32_t(r8)));
  // stfs f13,20(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // add r8,r7,r8
  r8 = r7 + r8;
  // stfs f12,16(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // extsw r9,r9
  r9 = uint64_t(int64_t(int32_t(r9)));
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // extsw r8,r8
  r8 = uint64_t(int64_t(int32_t(r8)));
  // std r5,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r5);
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // std r4,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r4);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // std r9,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r9);
  // std r8,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r8);
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,96(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 96)));
  // lfd f10,104(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 104)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f13,f13
  f13 = double(float(f13));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // frsp f12,f12
  f12 = double(float(f12));
  // stfs f12,4(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // lfs f9,100(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 100);
  f9 = double(temp.f32);
  // lfs f8,104(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 104);
  f8 = double(temp.f32);
  // frsp f11,f11
  f11 = double(float(f11));
  // stfs f0,32(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stfs f11,24(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // frsp f10,f10
  f10 = double(float(f10));
  // stfs f12,28(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f9,44(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f8,40(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 40, temp.u32);
  // lfs f12,96(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 96);
  f12 = double(temp.f32);
  // lfs f9,108(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 108);
  f9 = double(temp.f32);
  // stw r10,60(r11)
  REX_STORE_U32(uint32_t(r11) + 60, uint32_t(r10));
  // stfs f13,48(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // stfs f10,52(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 52, temp.u32);
  // stfs f0,56(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 56, temp.u32);
  // stfs f12,64(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 64, temp.u32);
  // stfs f9,68(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // lfs f13,108(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 108);
  f13 = double(temp.f32);
  // lfs f12,104(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 104);
  f12 = double(temp.f32);
  // stw r10,84(r11)
  REX_STORE_U32(uint32_t(r11) + 84, uint32_t(r10));
  // stfs f11,72(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 72, temp.u32);
  // stfs f10,76(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // stfs f0,80(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 80, temp.u32);
  // stfs f12,88(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 88, temp.u32);
  // stfs f13,92(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 92, temp.u32);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8212920C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8212922C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x82129238, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,36
  r10 = uint64_t(int64_t(36));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258ca50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CA50, LIFTED_ARGS(0x82129254, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r5,r22
  r5 = r22;
  // mr r4,r24
  r4 = r24;
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82129278, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8213B1E0: lifted from generated/ by lift.py.
lifted::Result f8213B1E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8213B1E0 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r3,0
  r3 = uint64_t(int64_t(0));
  // ori r3,r3,43120
  r3 = r3 | 43120;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x8213B1F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x8213b224
  if (cr0.eq) goto loc_8213B224;
  // mr r29,r31
  r29 = r31;
  // li r30,6
  r30 = uint64_t(int64_t(6));
loc_8213B208:
  // mr r3,r29
  r3 = r29;
  // bl 0x8213ad48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213AD48, LIFTED_ARGS(0x8213B210, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,6160
  r29 = uint64_t(int64_t(int64_t(r29) + 6160));
  // bge 0x8213b208
  if (!cr0.lt) goto loc_8213B208;
  // mr r3,r31
  r3 = r31;
  // b 0x8213b228
  goto loc_8213B228;
loc_8213B224:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8213B228:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // addi r29,r11,24692
  r29 = uint64_t(int64_t(int64_t(r11) + 24692));
  // stw r3,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r3));
  // beq cr6,0x8213b2ac
  if (cr6.eq) goto loc_8213B2AC;
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,43120
  r5 = r5 | 43120;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x8213B24C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_8213B254:
  // mr r3,r28
  r3 = r28;
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0468, LIFTED_ARGS(0x8213B25C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r27,r3
  r27 = r3;
  // add r31,r30,r11
  r31 = r30 + r11;
  // addi r3,r31,6048
  r3 = uint64_t(int64_t(int64_t(r31) + 6048));
  // bl 0x8213f700
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213F700, LIFTED_ARGS(0x8213B270, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r11,500(r31)
  REX_STORE_U32(uint32_t(r31) + 500, uint32_t(r11));
  // bl 0x8213aaa8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213AAA8, LIFTED_ARGS(0x8213B280, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // stwx r28,r30,r11
  REX_STORE_U32(uint32_t(r30) + uint32_t(r11), uint32_t(r28));
  // beq cr6,0x8213b29c
  if (cr6.eq) goto loc_8213B29C;
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r27,160(r11)
  REX_STORE_U32(uint32_t(r11) + 160, uint32_t(r27));
loc_8213B29C:
  // addi r30,r30,6160
  r30 = uint64_t(int64_t(int64_t(r30) + 6160));
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmplwi cr6,r30,43120
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 43120, so);
  // blt cr6,0x8213b254
  if (cr6.lt) goto loc_8213B254;
loc_8213B2AC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8213B2B8: lifted from generated/ by lift.py.
lifted::Result f8213B2B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8213B2B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r31,24692(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 24692);
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8213b2ec
  if (cr6.eq) goto loc_8213B2EC;
  // mr r3,r31
  r3 = r31;
  // bl 0x8213ad98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213AD98, LIFTED_ARGS(0x8213B2E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8213B2EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8213B2EC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,24692(r30)
  REX_STORE_U32(uint32_t(r30) + 24692, uint32_t(r11));
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

// sub_82150350: lifted from generated/ by lift.py.
lifted::Result f82150350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82150350 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
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
  // bl 0x825a7914
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r7,336(r4)
  r7 = REX_LOAD_U32(uint32_t(r4) + 336);
  // mr r31,r5
  r31 = r5;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmplwi r7,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // fmr f29,f31
  f29 = f31;
  // fmr f28,f31
  f28 = f31;
  // fmr f27,f31
  f27 = f31;
  // fmr f13,f31
  f13 = f31;
  // fmr f12,f31
  f12 = f31;
  // beq 0x82150444
  if (cr0.eq) goto loc_82150444;
  // addi r9,r4,80
  r9 = uint64_t(int64_t(int64_t(r4) + 80));
loc_8215039C:
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x82150428
  if (cr6.lt) goto loc_82150428;
  // beq cr6,0x82150418
  if (cr6.eq) goto loc_82150418;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x82150404
  if (cr6.lt) goto loc_82150404;
  // cmplwi cr6,r10,101
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 101, so);
  // beq cr6,0x821503f4
  if (cr6.eq) goto loc_821503F4;
  // cmplwi cr6,r10,102
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 102, so);
  // beq cr6,0x821503e4
  if (cr6.eq) goto loc_821503E4;
  // cmplwi cr6,r10,109
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 109, so);
  // bne cr6,0x82150434
  if (!cr6.eq) goto loc_82150434;
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // rlwinm r10,r6,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfsx f0,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f0 = double(temp.f32);
  // fneg f27,f0
  f27 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // b 0x82150434
  goto loc_82150434;
loc_821503E4:
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // rlwinm r10,r6,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfsx f12,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f12 = double(temp.f32);
  // b 0x82150434
  goto loc_82150434;
loc_821503F4:
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // rlwinm r10,r6,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfsx f13,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f13 = double(temp.f32);
  // b 0x82150434
  goto loc_82150434;
loc_82150404:
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // rlwinm r10,r6,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfsx f0,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f0 = double(temp.f32);
  // fneg f28,f0
  f28 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // b 0x82150434
  goto loc_82150434;
loc_82150418:
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // rlwinm r10,r6,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfsx f29,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f29 = double(temp.f32);
  // b 0x82150434
  goto loc_82150434;
loc_82150428:
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // rlwinm r10,r6,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfsx f31,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f31 = double(temp.f32);
loc_82150434:
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // blt cr6,0x8215039c
  if (cr6.lt) goto loc_8215039C;
loc_82150444:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11532(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11532);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // lfs f30,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f30 = double(temp.f32);
  // fmuls f13,f13,f30
  f13 = double(float(f13 * f30));
  // fdivs f1,f0,f13
  f1 = double(float(f0 / f13));
  // bl 0x825a8688
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8688, LIFTED_ARGS(0x82150464, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f0,f30
  f13 = double(float(f0 * f30));
  // lfs f0,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS(0x8215047C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfs f31,80(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f29,84(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stfs f28,88(r1)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // stfs f27,32(r31)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r31) + 32, temp.u32);
  // lfs f0,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f0 = double(temp.f32);
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // ld r10,0(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 0);
  // ld r9,8(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 8);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11312(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11312);
  f0 = double(temp.f32);
  // fdivs f0,f0,f13
  f0 = double(float(f0 / f13));
  // stfs f0,36(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // std r9,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r9);
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r12,r1,-16
  r12 = uint64_t(int64_t(int64_t(r1) + -16));
  // bl 0x825a7960
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

// sub_8216C2A0: lifted from generated/ by lift.py.
lifted::Result f8216C2A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8216C2A0 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,9612(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8216c310
  if (cr6.lt) goto loc_8216C310;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x8216c2d4
  if (cr6.lt) goto loc_8216C2D4;
  // beq cr6,0x8216c2fc
  if (cr6.eq) goto loc_8216C2FC;
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // bne cr6,0x8216c31c
  if (!cr6.eq) goto loc_8216C31C;
loc_8216C2D4:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x822128a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822128A8, LIFTED_ARGS(0x8216C2E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82212a48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82212A48, LIFTED_ARGS(0x8216C2EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82212b08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82212B08, LIFTED_ARGS(0x8216C2F8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8216c31c
  goto loc_8216C31C;
loc_8216C2FC:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r11,656
  r3 = uint64_t(int64_t(int64_t(r11) + 656));
  // bl 0x82215d58
  // (left out: dead in C++)
  { const lifted::Result o = f82215D58(base, r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8216c31c
  goto loc_8216C31C;
loc_8216C310:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,12
  r3 = uint64_t(int64_t(int64_t(r11) + 12));
  // bl 0x8220b9e8
  // (left out: dead in C++)
  { const lifted::Result o = f8220B9E8(base, r1, r13, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_8216C31C:
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

// sub_821715B8: lifted from generated/ by lift.py.
lifted::Result f821715B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821715B8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r3,5
  r3 = uint64_t(int64_t(327680));
  // ori r3,r3,35912
  r3 = r3 | 35912;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x821715D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,-32012(r10)
  REX_STORE_U32(uint32_t(r10) + -32012, uint32_t(r3));
  // beq 0x821716e8
  if (cr0.eq) goto loc_821716E8;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // mr r7,r11
  r7 = r11;
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // lfs f0,2612(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 2612);
  f0 = double(temp.f32);
loc_821715F8:
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // add r8,r7,r8
  r8 = r7 + r8;
  // addi r7,r7,28
  r7 = uint64_t(int64_t(int64_t(r7) + 28));
  // cmplwi cr6,r7,2156
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 2156, so);
  // stfs f0,16(r8)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 16, temp.u32);
  // stw r11,4(r8)
  REX_STORE_U32(uint32_t(r8) + 4, uint32_t(r11));
  // stw r9,8(r8)
  REX_STORE_U32(uint32_t(r8) + 8, uint32_t(r9));
  // stw r9,24(r8)
  REX_STORE_U32(uint32_t(r8) + 24, uint32_t(r9));
  // stw r11,20(r8)
  REX_STORE_U32(uint32_t(r8) + 20, uint32_t(r11));
  // stw r9,28(r8)
  REX_STORE_U32(uint32_t(r8) + 28, uint32_t(r9));
  // sth r9,12(r8)
  REX_STORE_U16(uint32_t(r8) + 12, uint16_t(r9));
  // sth r9,14(r8)
  REX_STORE_U16(uint32_t(r8) + 14, uint16_t(r9));
  // blt cr6,0x821715f8
  if (cr6.lt) goto loc_821715F8;
  // mr r7,r11
  r7 = r11;
loc_82171630:
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // add r8,r7,r8
  r8 = r7 + r8;
  // stfs f0,2172(r8)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 2172, temp.u32);
  // stw r11,2160(r8)
  REX_STORE_U32(uint32_t(r8) + 2160, uint32_t(r11));
  // stw r9,2164(r8)
  REX_STORE_U32(uint32_t(r8) + 2164, uint32_t(r9));
  // stw r9,2180(r8)
  REX_STORE_U32(uint32_t(r8) + 2180, uint32_t(r9));
  // stw r11,2176(r8)
  REX_STORE_U32(uint32_t(r8) + 2176, uint32_t(r11));
  // stw r9,2184(r8)
  REX_STORE_U32(uint32_t(r8) + 2184, uint32_t(r9));
  // sth r9,2168(r8)
  REX_STORE_U16(uint32_t(r8) + 2168, uint16_t(r9));
  // sth r9,2170(r8)
  REX_STORE_U16(uint32_t(r8) + 2170, uint16_t(r9));
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // add r8,r7,r8
  r8 = r7 + r8;
  // addi r7,r7,28
  r7 = uint64_t(int64_t(int64_t(r7) + 28));
  // cmplwi cr6,r7,84
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 84, so);
  // stw r11,2164(r8)
  REX_STORE_U32(uint32_t(r8) + 2164, uint32_t(r11));
  // blt cr6,0x82171630
  if (cr6.lt) goto loc_82171630;
  // li r8,2244
  r8 = uint64_t(int64_t(2244));
loc_82171674:
  // lwz r7,-32012(r10)
  r7 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stwx r9,r8,r7
  REX_STORE_U32(uint32_t(r8) + uint32_t(r7), uint32_t(r9));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // cmplwi cr6,r8,2308
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 2308, so);
  // blt cr6,0x82171674
  if (cr6.lt) goto loc_82171674;
  // lwz r9,-32012(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stw r11,2364(r9)
  REX_STORE_U32(uint32_t(r9) + 2364, uint32_t(r11));
  // lwz r9,-32012(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stw r11,2368(r9)
  REX_STORE_U32(uint32_t(r9) + 2368, uint32_t(r11));
  // lwz r9,-32012(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stw r11,2372(r9)
  REX_STORE_U32(uint32_t(r9) + 2372, uint32_t(r11));
  // li r9,2336
  r9 = uint64_t(int64_t(2336));
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stw r11,2376(r8)
  REX_STORE_U32(uint32_t(r8) + 2376, uint32_t(r11));
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stw r11,2380(r8)
  REX_STORE_U32(uint32_t(r8) + 2380, uint32_t(r11));
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stw r11,2384(r8)
  REX_STORE_U32(uint32_t(r8) + 2384, uint32_t(r11));
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stw r11,2388(r8)
  REX_STORE_U32(uint32_t(r8) + 2388, uint32_t(r11));
loc_821716C4:
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // add r8,r9,r8
  r8 = r9 + r8;
  // stw r11,-28(r8)
  REX_STORE_U32(uint32_t(r8) + -28, uint32_t(r11));
  // lwz r8,-32012(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + -32012);
  // stwx r11,r9,r8
  REX_STORE_U32(uint32_t(r9) + uint32_t(r8), uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplwi cr6,r9,2364
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 2364, so);
  // blt cr6,0x821716c4
  if (cr6.lt) goto loc_821716C4;
  // lwz r3,-32012(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + -32012);
loc_821716E8:
  // addi r10,r3,2392
  r10 = uint64_t(int64_t(int64_t(r3) + 2392));
  // li r9,300
  r9 = uint64_t(int64_t(300));
  // addis r10,r10,5
  r10 = uint64_t(int64_t(int64_t(r10) + 327680));
  // addi r10,r10,32320
  r10 = uint64_t(int64_t(int64_t(r10) + 32320));
  // mtctr r9
  ctr = r9;
loc_821716FC:
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x821716fc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821716FC;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8217C368: lifted from generated/ by lift.py.
lifted::Result f8217C368(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8217C368 (lifted)");
  uint64_t r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x8217aa98
  { const lifted::Result o = lifted::Call(base, 0x8217AA98, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82294330: lifted from generated/ by lift.py.
void f82294330(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82294330 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r5,r4
  r5 = r4;
  // mr r4,r3
  r4 = r3;
  // b 0x82294068
  { const lifted::Result o = lifted::Call(base, 0x82294068, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return;
}

// sub_82295330: lifted from generated/ by lift.py.
lifted::Result f82295330(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82295330 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // stw r3,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r3));
  // rlwinm. r11,r5,0,16,16
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0x8000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r4,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r4));
  // beq 0x82295374
  if (cr0.eq) goto loc_82295374;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82295374
  if (cr6.eq) goto loc_82295374;
  // lis r3,-16384
  r3 = uint64_t(int64_t(-1073741824));
  // ori r3,r3,13
  r3 = r3 | 13;
loc_8229535C:
  // bl 0x82296e88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296E88, LIFTED_ARGS(0x82295360, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82295364:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_82295374:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r4,r1,124
  r4 = uint64_t(int64_t(int64_t(r1) + 124));
  // addi r3,r1,116
  r3 = uint64_t(int64_t(int64_t(r1) + 116));
  // bl 0x825db6c4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtFreeVirtualMemory, LIFTED_ARGS(0x82295384, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8229535c
  if (cr0.lt) goto loc_8229535C;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82295364
  goto loc_82295364;
}

// sub_822A5B78: lifted from generated/ by lift.py.
uint64_t f822A5B78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822A5B78 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x822a4f60
  { const lifted::Result o = lifted::Call(base, 0x822A4F60, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_822A6008: lifted from generated/ by lift.py.
lifted::Result f822A6008(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822A6008 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // addi r11,r4,3072
  r11 = uint64_t(int64_t(int64_t(r4) + 3072));
  // mr r31,r3
  r31 = r3;
  // rlwinm r29,r11,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // lwzx r28,r29,r31
  r28 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r31));
  // beq cr6,0x822a6108
  if (cr6.eq) goto loc_822A6108;
  // lwz r10,32(r5)
  r10 = REX_LOAD_U32(uint32_t(r5) + 32);
  // mulli r11,r4,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(24))));
  // add r8,r31,r4
  r8 = r31 + r4;
  // lwz r9,48(r5)
  r9 = REX_LOAD_U32(uint32_t(r5) + 48);
  // lwz r27,28(r5)
  r27 = REX_LOAD_U32(uint32_t(r5) + 28);
  // lwz r25,36(r5)
  r25 = REX_LOAD_U32(uint32_t(r5) + 36);
  // lwz r26,40(r5)
  r26 = REX_LOAD_U32(uint32_t(r5) + 40);
  // rlwinm r4,r10,12,20,31
  r4 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFF;
  // add r11,r11,r31
  r11 = r11 + r31;
  // addi r4,r4,512
  r4 = uint64_t(int64_t(int64_t(r4) + 512));
  // addi r11,r11,1024
  r11 = uint64_t(int64_t(int64_t(r11) + 1024));
  // clrlwi r30,r10,3
  r30 = uint32_t(r10) & 0x1FFFFFFF;
  // rlwinm r3,r4,0,19,19
  r3 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 0) & 0x1000;
  // lwz r10,44(r5)
  r10 = REX_LOAD_U32(uint32_t(r5) + 44);
  // rlwinm r7,r9,12,20,31
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 12) & 0xFFF;
  // add r3,r3,r30
  r3 = r3 + r30;
  // addi r7,r7,512
  r7 = uint64_t(int64_t(int64_t(r7) + 512));
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r4,r9,0,3,22
  r4 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x1FFFFE00;
  // rlwinm r7,r7,0,19,19
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0x1000;
  // stw r25,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r25));
  // rlwimi r27,r30,0,10,21
  r27 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x3FFC00) | (r27 & 0xFFFFFFFFFFC003FF);
  // lwz r25,20(r11)
  r25 = REX_LOAD_U32(uint32_t(r11) + 20);
  // add r7,r7,r4
  r7 = r7 + r4;
  // lbz r9,11694(r8)
  r9 = REX_LOAD_U8(uint32_t(r8) + 11694);
  // lwz r4,16(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 16);
  // rlwinm r30,r10,30,28,31
  r30 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0xF;
  // rlwimi r10,r4,0,30,21
  r10 = (__builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 0) & 0xFFFFFFFFFFFFFC03) | (r10 & 0x3FC);
  // lwz r4,12(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 12);
  // rlwimi r7,r25,0,23,31
  r7 = (__builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 0) & 0x1FF) | (r7 & 0xFFFFFFFFFFFFFE00);
  // rlwimi r26,r4,0,1,12
  r26 = (__builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 0) & 0x7FF80000) | (r26 & 0xFFFFFFFF8007FFFF);
  // stw r27,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r27));
  // cmplw cr6,r30,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r9), so);
  // lwz r27,4(r11)
  r27 = REX_LOAD_U32(uint32_t(r11) + 4);
  // rlwimi r3,r27,0,20,20
  r3 = (__builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 0) & 0x800) | (r3 & 0xFFFFFFFFFFFFF7FF);
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r7,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r7));
  // stw r26,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r26));
  // stw r3,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r3));
  // ble cr6,0x822a60d4
  if (!cr6.gt) goto loc_822A60D4;
  // lwz r9,44(r5)
  r9 = REX_LOAD_U32(uint32_t(r5) + 44);
  // rlwinm r9,r9,30,28,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 30) & 0xF;
loc_822A60D4:
  // lwz r7,44(r5)
  r7 = REX_LOAD_U32(uint32_t(r5) + 44);
  // rlwimi r10,r9,2,26,29
  r10 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x3C) | (r10 & 0xFFFFFFFFFFFFFFC3);
  // lbz r8,11720(r8)
  r8 = REX_LOAD_U8(uint32_t(r8) + 11720);
  // rlwinm r7,r7,26,28,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 26) & 0xF;
  // cmplw cr6,r7,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r8), so);
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // bge cr6,0x822a60f4
  if (!cr6.lt) goto loc_822A60F4;
  // mr r8,r7
  r8 = r7;
loc_822A60F4:
  // rlwimi r10,r8,6,22,25
  r10 = (__builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 6) & 0x3C0) | (r10 & 0xFFFFFFFFFFFFFC3F);
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // or r11,r11,r6
  r11 = r11 | r6;
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
loc_822A6108:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // stwx r5,r29,r31
  REX_STORE_U32(uint32_t(r29) + uint32_t(r31), uint32_t(r5));
  // beq cr6,0x822a6178
  if (cr6.eq) goto loc_822A6178;
  // lwz r11,10780(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10780);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822a6128
  if (cr0.eq) goto loc_822A6128;
  // stw r11,8(r28)
  REX_STORE_U32(uint32_t(r28) + 8, uint32_t(r11));
  // b 0x822a6178
  goto loc_822A6178;
loc_822A6128:
  // lwz r11,10784(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10784);
  // lwz r10,0(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 0);
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822a6178
  if (cr0.eq) goto loc_822A6178;
  // lwz r11,13172(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 13172);
  // lwz r3,13168(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 13168);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // blt cr6,0x822a6150
  if (cr6.lt) goto loc_822A6150;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ad790
  // (left out: dead in C++)
  { const lifted::Result o = f822AD790(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_822A6150:
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // rlwimi r11,r28,30,2,31
  r11 = (__builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 30) & 0x3FFFFFFF) | (r11 & 0xFFFFFFFFC0000000);
  // rlwinm r11,r11,0,2,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFBFFFFFFF;
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // addi r10,r3,8
  r10 = uint64_t(int64_t(int64_t(r3) + 8));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // std r11,0(r3)
  REX_STORE_U64(uint32_t(r3) + 0, r11);
  // stw r10,13168(r31)
  REX_STORE_U32(uint32_t(r31) + 13168, uint32_t(r10));
loc_822A6178:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822A8740: lifted from generated/ by lift.py.
uint64_t f822A8740(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822A8740 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r28,r4
  r28 = r4;
  // li r5,592
  r5 = uint64_t(int64_t(592));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x822A8760, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,612(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 612);
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stw r28,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r28));
  // add r11,r11,r31
  r11 = r11 + r31;
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r8,-1
  r8 = uint64_t(int64_t(-65536));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // addi r11,r11,592
  r11 = uint64_t(int64_t(int64_t(r11) + 592));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // stw r9,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r9));
  // stw r8,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r8));
  // stw r7,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r7));
  // lwz r10,28(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 28);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r10,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r10));
  // lwz r9,24(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 24);
  // addi r9,r9,10
  r9 = uint64_t(int64_t(int64_t(r9) + 10));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r30,r9,r11
  r30 = r9 + r11;
  // beq 0x822a87fc
  if (cr0.eq) goto loc_822A87FC;
  // addi r29,r31,192
  r29 = uint64_t(int64_t(int64_t(r31) + 192));
  // addi r31,r31,64
  r31 = uint64_t(int64_t(int64_t(r31) + 64));
  // mr r27,r10
  r27 = r10;
loc_822A87BC:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // clrlwi r11,r11,20
  r11 = uint32_t(r11) & 0xFFF;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r4,r11,r28
  r4 = r11 + r28;
  // cmplw cr6,r29,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r4), so);
  // beq cr6,0x822a87e8
  if (cr6.eq) goto loc_822A87E8;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x822A87E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822A87E8:
  // addic. r27,r27,-1
  ca = uint32_t(r27) > 0;
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r29,r29,12
  r29 = uint64_t(int64_t(int64_t(r29) + 12));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x822a87bc
  if (!cr0.eq) goto loc_822A87BC;
loc_822A87FC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_82594D10: lifted from generated/ by lift.py.
uint64_t f82594D10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82594D10 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r3,r4
  r3 = r4;
  // stw r5,8(r4)
  REX_STORE_U32(uint32_t(r4) + 8, uint32_t(r5));
  // stw r6,12(r4)
  REX_STORE_U32(uint32_t(r4) + 12, uint32_t(r6));
  // b 0x8258bd28
  { const lifted::Result o = lifted::Call(base, 0x8258BD28, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82596920: lifted from generated/ by lift.py.
lifted::Result f82596920(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82596920 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r31,r5
  r31 = r5;
  // bl 0x82594970
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594970, LIFTED_ARGS(0x8259693C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x825963f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825963F0, LIFTED_ARGS(0x82596948, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82596984
  if (cr6.eq) goto loc_82596984;
  // addi r30,r29,56
  r30 = uint64_t(int64_t(int64_t(r29) + 56));
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x82596f68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82596F68, LIFTED_ARGS(0x82596960, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r29,80
  r31 = uint64_t(int64_t(int64_t(r29) + 80));
  // mr r5,r30
  r5 = r30;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82596980, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r31,48(r29)
  REX_STORE_U32(uint32_t(r29) + 48, uint32_t(r31));
loc_82596984:
  // lwz r11,412(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 412);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82596b0c
  if (cr6.eq) goto loc_82596B0C;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f6,2608(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 2608);
  f6 = double(temp.f32);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lfs f11,10712(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 10712);
  f11 = double(temp.f32);
  // lfs f12,7140(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 7140);
  f12 = double(temp.f32);
  // lfs f13,7132(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7132);
  f13 = double(temp.f32);
  // lfs f7,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f7 = double(temp.f32);
loc_825969C0:
  // lwz r11,40(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 40);
  // add r11,r11,r4
  r11 = r11 + r4;
  // lwz r10,428(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 428);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x82596af8
  if (!cr6.eq) goto loc_82596AF8;
  // lwz r6,408(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 408);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r10,0(r6)
  r10 = REX_LOAD_U32(uint32_t(r6) + 0);
  // lwz r7,8(r6)
  r7 = REX_LOAD_U32(uint32_t(r6) + 8);
  // lwz r11,12(r6)
  r11 = REX_LOAD_U32(uint32_t(r6) + 12);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // ble cr6,0x82596af8
  if (!cr6.gt) goto loc_82596AF8;
loc_825969F0:
  // lwz r9,0(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 0);
  // fmr f10,f7
  lifted::ScalarFloatMode();
  f10 = f7;
  // fmr f9,f7
  f9 = f7;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // fmr f8,f7
  f8 = f7;
  // beq 0x82596ac8
  if (cr0.eq) goto loc_82596AC8;
loc_82596A0C:
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // addi r31,r1,80
  r31 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmadds f0,f0,f13,f12
  f0 = double(float(std::fma(f0, f13, f12)));
  // addi r9,r11,4
  r9 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r8,r11,8
  r8 = uint64_t(int64_t(int64_t(r11) + 8));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // lfs f5,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f5 = double(temp.f32);
  // fmadds f5,f5,f13,f12
  f5 = double(float(std::fma(f5, f13, f12)));
  // fctidz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x8000000000000000ULL) : (f0 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r31
  REX_STORE_U32(uint32_t(r31), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lfs f0,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // fmadds f0,f0,f13,f12
  f0 = double(float(std::fma(f0, f13, f12)));
  // fctidz f5,f5
  f5 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f5) ? int64_t(0x8000000000000000ULL) : (f5 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f5)))));
  // fctidz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x8000000000000000ULL) : (f0 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f0)))));
  // lwz r31,80(r1)
  r31 = REX_LOAD_U32(uint32_t(r1) + 80);
  // addi r30,r1,84
  r30 = uint64_t(int64_t(int64_t(r1) + 84));
  // stfiwx f0,0,r30
  REX_STORE_U32(uint32_t(r30), uint32_t(std::bit_cast<uint64_t>(f0)));
  // std r31,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r31);
  // lwz r31,84(r1)
  r31 = REX_LOAD_U32(uint32_t(r1) + 84);
  // addi r30,r1,88
  r30 = uint64_t(int64_t(int64_t(r1) + 88));
  // lfd f0,96(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 96)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // stfiwx f5,0,r30
  REX_STORE_U32(uint32_t(r30), uint32_t(std::bit_cast<uint64_t>(f5)));
  // std r31,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r31);
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f11
  f0 = double(float(f0 * f11));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // fadds f10,f0,f10
  f10 = double(float(f0 + f10));
  // lwz r31,88(r1)
  r31 = REX_LOAD_U32(uint32_t(r1) + 88);
  // lfd f5,104(r1)
  f5 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 104)));
  // fcfid f5,f5
  f5 = double(std::bit_cast<int64_t>(f5));
  // std r31,112(r1)
  REX_STORE_U64(uint32_t(r1) + 112, r31);
  // frsp f5,f5
  f5 = double(float(f5));
  // fmuls f0,f5,f11
  f0 = double(float(f5 * f11));
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fadds f9,f0,f9
  f9 = double(float(f0 + f9));
  // lfd f5,112(r1)
  f5 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 112)));
  // fcfid f5,f5
  f5 = double(std::bit_cast<int64_t>(f5));
  // frsp f5,f5
  f5 = double(float(f5));
  // fmuls f0,f5,f11
  f0 = double(float(f5 * f11));
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // lwz r9,0(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 0);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // fadds f8,f0,f8
  f8 = double(float(f0 + f8));
  // blt cr6,0x82596a0c
  if (cr6.lt) goto loc_82596A0C;
loc_82596AC8:
  // fsubs f0,f6,f10
  lifted::ScalarFloatMode();
  f0 = double(float(f6 - f10));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fsubs f10,f6,f9
  f10 = double(float(f6 - f9));
  // stfs f10,4(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // fsubs f0,f6,f8
  f0 = double(float(f6 - f8));
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // lwz r10,0(r6)
  r10 = REX_LOAD_U32(uint32_t(r6) + 0);
  // addi r5,r5,1
  r5 = uint64_t(int64_t(int64_t(r5) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // cmplw cr6,r5,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r10), so);
  // blt cr6,0x825969f0
  if (cr6.lt) goto loc_825969F0;
loc_82596AF8:
  // lwz r11,412(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 412);
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // addi r4,r4,544
  r4 = uint64_t(int64_t(int64_t(r4) + 544));
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // blt cr6,0x825969c0
  if (cr6.lt) goto loc_825969C0;
loc_82596B0C:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825986B8: lifted from generated/ by lift.py.
lifted::Result f825986B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825986B8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // mr r6,r4
  r6 = r4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825986dc
  if (!cr6.eq) goto loc_825986DC;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // b 0x825986e8
  goto loc_825986E8;
loc_825986DC:
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // srawi r10,r10,4
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0xF) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 4));
loc_825986E8:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82598738
  if (cr6.eq) goto loc_82598738;
  // lwz r9,12(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 12);
  // subf r11,r11,r9
  r11 = r9 - r11;
  // srawi r11,r11,4
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xF) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 4));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82598738
  if (!cr6.lt) goto loc_82598738;
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82598720
  if (cr6.eq) goto loc_82598720;
  // ld r10,0(r6)
  r10 = REX_LOAD_U64(uint32_t(r6) + 0);
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
  // ld r10,8(r6)
  r10 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r10,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r10);
loc_82598720:
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stw r11,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_82598738:
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lwz r5,0(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82598648
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82598648, LIFTED_ARGS(0x82598754, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825998A0: lifted from generated/ by lift.py.
lifted::Result f825998A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825998A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,176(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 176);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x8259c3a8
  { const lifted::Result o = lifted::Call(base, 0x8259C3A8, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8259F770: lifted from generated/ by lift.py.
lifted::Result f8259F770(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_8259F770 (lifted)");
  uint64_t r4 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r5
  r30 = r5;
  // mr r31,r3
  r31 = r3;
  // mr r4,r30
  r4 = r30;
  // mr r29,r6
  r29 = r6;
  // bl 0x8259e6c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259E6C8, LIFTED_ARGS(0x8259F790, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,100(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 100);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8259f83c
  if (cr6.eq) goto loc_8259F83C;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x8211a818
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A818, LIFTED_ARGS(0x8259F7A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r30,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r30));
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS(0x8259F7B4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrldi r9,r29,32
  r9 = r29 & 0xFFFFFFFF;
  // addi r11,r31,80
  r11 = uint64_t(int64_t(int64_t(r31) + 80));
  // lwz r3,100(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 100);
  // addi r10,r31,64
  r10 = uint64_t(int64_t(int64_t(r31) + 64));
  // addi r8,r1,196
  r8 = uint64_t(int64_t(int64_t(r1) + 196));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // std r9,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r9);
  // addi r9,r1,180
  r9 = uint64_t(int64_t(int64_t(r1) + 180));
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r6,4(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r5,8(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r31,12(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lwz r30,0(r10)
  r30 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r29,4(r10)
  r29 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lwz r28,8(r10)
  r28 = REX_LOAD_U32(uint32_t(r10) + 8);
  // lwz r10,12(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 12);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,5408(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 5408);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,176(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 176, temp.u32);
  // stw r7,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r7));
  // stw r6,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r6));
  // stw r5,8(r9)
  REX_STORE_U32(uint32_t(r9) + 8, uint32_t(r5));
  // stw r31,12(r9)
  REX_STORE_U32(uint32_t(r9) + 12, uint32_t(r31));
  // stw r30,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r30));
  // stw r29,4(r8)
  REX_STORE_U32(uint32_t(r8) + 4, uint32_t(r29));
  // stw r28,8(r8)
  REX_STORE_U32(uint32_t(r8) + 8, uint32_t(r28));
  // stw r10,12(r8)
  REX_STORE_U32(uint32_t(r8) + 12, uint32_t(r10));
  // bl 0x8259f700
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259F700, LIFTED_ARGS(0x8259F834, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x8211a8c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A8C8, LIFTED_ARGS(0x8259F83C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8259F83C:
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8259FB30: lifted from generated/ by lift.py.
lifted::Result f8259FB30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259FB30 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7910
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // lfs f0,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lfs f28,8(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8);
  f28 = double(temp.f32);
  // lfs f13,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // lfs f1,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f1 = double(temp.f32);
  // fsubs f30,f0,f31
  f30 = double(float(f0 - f31));
  // lfs f0,4(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 4);
  f0 = double(temp.f32);
  // fsubs f29,f13,f0
  f29 = double(float(f13 - f0));
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS(0x8259FB7C, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f13,f1
  lifted::ScalarFloatMode();
  f13 = f1;
  // lfs f0,12(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f0 = double(temp.f32);
  // lfs f27,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f27 = double(temp.f32);
  // fmr f1,f0
  f1 = f0;
  // frsp f26,f13
  f26 = double(float(f13));
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS(0x8259FB94, r31)); r3 = o.r3; f1 = o.f1; }
  // fmuls f12,f26,f29
  lifted::ScalarFloatMode();
  f12 = double(float(f26 * f29));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f11 = double(temp.f32);
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // lfs f10,8(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8);
  f10 = double(temp.f32);
  // fmuls f10,f10,f30
  f10 = double(float(f10 * f30));
  // lfs f0,10808(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10808);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadds f12,f12,f0,f28
  f12 = double(float(std::fma(f12, f0, f28)));
  // lfs f13,8676(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8676);
  f13 = double(temp.f32);
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // fmuls f10,f10,f30
  f10 = double(float(f10 * f30));
  // fmuls f12,f12,f30
  f12 = double(float(f12 * f30));
  // fmuls f12,f12,f30
  f12 = double(float(f12 * f30));
  // fmuls f9,f12,f31
  f9 = double(float(f12 * f31));
  // frsp f12,f1
  f12 = double(float(f1));
  // fmuls f9,f9,f13
  f9 = double(float(f9 * f13));
  // fmuls f12,f12,f29
  f12 = double(float(f12 * f29));
  // fmadds f0,f12,f0,f27
  f0 = double(float(std::fma(f12, f0, f27)));
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fmadds f0,f0,f13,f9
  f0 = double(float(std::fma(f0, f13, f9)));
  // fmadds f0,f11,f31,f0
  f0 = double(float(std::fma(f11, f31, f0)));
  // fmadds f1,f10,f30,f0
  f1 = double(float(std::fma(f10, f30, f0)));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a795c
  // (left out: dead in C++)
  // (left out: dead in C++)
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

// sub_8259FC18: lifted from generated/ by lift.py.
lifted::Result f8259FC18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259FC18 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r4
  r31 = r4;
  // lfs f0,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lfs f13,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // lfs f1,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f1 = double(temp.f32);
  // fsubs f30,f0,f31
  f30 = double(float(f0 - f31));
  // lfs f0,4(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 4);
  f0 = double(temp.f32);
  // fsubs f29,f13,f0
  f29 = double(float(f13 - f0));
  // lwa r29,8(r30)
  r29 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r30) + 8))));
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS(0x8259FC5C, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f13,f1
  lifted::ScalarFloatMode();
  f13 = f1;
  // lfs f0,12(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f0 = double(temp.f32);
  // lwa r28,8(r31)
  r28 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r31) + 8))));
  // fmr f1,f0
  f1 = f0;
  // frsp f28,f13
  f28 = double(float(f13));
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS(0x8259FC74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r29
  r10 = r29;
  // fmuls f11,f28,f29
  lifted::ScalarFloatMode();
  f11 = double(float(f28 * f29));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // frsp f12,f1
  f12 = double(float(f1));
  // mr r9,r28
  r9 = r28;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // std r9,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r9);
  // lfs f0,10808(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10808);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f12,f12,f29
  f12 = double(float(f12 * f29));
  // lfs f13,8676(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8676);
  f13 = double(temp.f32);
  // lwa r7,8(r31)
  r7 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r31) + 8))));
  // lwa r8,8(r30)
  r8 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r30) + 8))));
  // std r7,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r7);
  // std r8,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r8);
  // lfd f10,80(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // lfd f9,88(r1)
  f9 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f9,f9
  f9 = double(std::bit_cast<int64_t>(f9));
  // frsp f10,f10
  f10 = double(float(f10));
  // frsp f9,f9
  f9 = double(float(f9));
  // lfd f8,96(r1)
  f8 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 96)));
  // fcfid f8,f8
  f8 = double(std::bit_cast<int64_t>(f8));
  // lfd f7,104(r1)
  f7 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 104)));
  // fcfid f7,f7
  f7 = double(std::bit_cast<int64_t>(f7));
  // fmadds f11,f11,f0,f10
  f11 = double(float(std::fma(f11, f0, f10)));
  // fmadds f0,f12,f0,f9
  f0 = double(float(std::fma(f12, f0, f9)));
  // frsp f8,f8
  f8 = double(float(f8));
  // frsp f7,f7
  f7 = double(float(f7));
  // fmuls f11,f11,f30
  f11 = double(float(f11 * f30));
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // fmuls f10,f8,f31
  f10 = double(float(f8 * f31));
  // fmuls f8,f7,f30
  f8 = double(float(f7 * f30));
  // fmuls f11,f11,f30
  f11 = double(float(f11 * f30));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fmuls f10,f10,f31
  f10 = double(float(f10 * f31));
  // fmuls f8,f8,f30
  f8 = double(float(f8 * f30));
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fmuls f12,f11,f13
  f12 = double(float(f11 * f13));
  // fmadds f0,f0,f13,f12
  f0 = double(float(std::fma(f0, f13, f12)));
  // fmadds f0,f10,f31,f0
  f0 = double(float(std::fma(f10, f31, f0)));
  // fmadds f0,f8,f30,f0
  f0 = double(float(std::fma(f8, f30, f0)));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r6
  REX_STORE_U32(uint32_t(r6), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r12,r1,-40
  r12 = uint64_t(int64_t(int64_t(r1) + -40));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A7068: lifted from generated/ by lift.py.
lifted::Result f825A7068(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A7068 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x825a6f60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6F60, LIFTED_ARGS(0x825A7078, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // neg r3,r11
  r3 = uint64_t(int64_t(static_cast<int64_t>(-r11)));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825A70E8: lifted from generated/ by lift.py.
lifted::Result f825A70E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825A70E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825a9b58
  { const lifted::Result o = lifted::Call(base, 0x825A9B58, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825A7410: lifted from generated/ by lift.py.
uint64_t f825A7410(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825A7410 (lifted)");
  uint64_t r0 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, ctr = 0;
  lifted::Cr cr0, cr1, cr6, cr7;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // std r3,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r3);
  // clrlwi r6,r3,29
  r6 = uint32_t(r3) & 0x7;
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // subfic r6,r6,8
  ca = uint32_t(r6) <= 8;
  r6 = static_cast<uint64_t>(8) - r6;
  // beq 0x825a7454
  if (cr0.eq) goto loc_825A7454;
  // cmplw r5,r6
  cr0 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r6), so);
  // ble 0x825a7470
  if (!cr0.gt) goto loc_825A7470;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // addi r4,r4,-1
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  // subf r5,r6,r5
  r5 = r5 - r6;
  // mtctr r6
  ctr = r6;
loc_825A7440:
  // lbzu r6,1(r4)
  ea = 1 + uint32_t(r4);
  r6 = REX_LOAD_U8(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stbu r6,1(r3)
  ea = 1 + uint32_t(r3);
  REX_STORE_U8(ea, uint8_t(r6));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a7440
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7440;
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // addi r4,r4,1
  r4 = uint64_t(int64_t(int64_t(r4) + 1));
loc_825A7454:
  // clrlwi r6,r4,29
  r6 = uint32_t(r4) & 0x7;
  // cmplwi cr6,r6,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 4, so);
  // cmplwi cr1,r6,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // cmplwi cr7,r5,128
  cr7 = lifted::Compare<uint32_t>(uint32_t(r5), 128, so);
  // beq cr6,0x825a75f0
  if (cr6.eq) goto loc_825A75F0;
  // bne cr1,0x825a7720
  if (!cr1.eq) goto loc_825A7720;
  // bge cr7,0x825a74c4
  if (!cr7.lt) goto loc_825A74C4;
loc_825A7470:
  // addi r4,r4,-8
  r4 = uint64_t(int64_t(int64_t(r4) + -8));
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
loc_825A747C:
  // rlwinm r7,r5,29,28,31
  r7 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 29) & 0xF;
  // clrlwi r6,r5,29
  r6 = uint32_t(r5) & 0x7;
  // cmplwi cr1,r7,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq cr1,0x825a74a0
  if (cr1.eq) goto loc_825A74A0;
  // mtctr r7
  ctr = r7;
loc_825A7494:
  // ldu r7,8(r4)
  ea = 8 + uint32_t(r4);
  r7 = REX_LOAD_U64(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stdu r7,8(r3)
  ea = 8 + uint32_t(r3);
  REX_STORE_U64(ea, r7);
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a7494
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7494;
loc_825A74A0:
  // beq cr6,0x825a74bc
  if (cr6.eq) goto loc_825A74BC;
  // addi r3,r3,7
  r3 = uint64_t(int64_t(int64_t(r3) + 7));
  // addi r4,r4,7
  r4 = uint64_t(int64_t(int64_t(r4) + 7));
  // mtctr r6
  ctr = r6;
loc_825A74B0:
  // lbzu r7,1(r4)
  ea = 1 + uint32_t(r4);
  r7 = REX_LOAD_U8(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stbu r7,1(r3)
  ea = 1 + uint32_t(r3);
  REX_STORE_U8(ea, uint8_t(r7));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a74b0
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A74B0;
loc_825A74BC:
  // ld r3,-8(r1)
  r3 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return r3;
loc_825A74C4:
  // clrlwi r6,r3,25
  r6 = uint32_t(r3) & 0x7F;
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // addi r4,r4,-8
  r4 = uint64_t(int64_t(int64_t(r4) + -8));
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // subfic r6,r6,128
  ca = uint32_t(r6) <= 128;
  r6 = static_cast<uint64_t>(128) - r6;
  // beq 0x825a74f4
  if (cr0.eq) goto loc_825A74F4;
  // rlwinm r7,r6,29,3,31
  r7 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 29) & 0x1FFFFFFF;
  // subf r5,r6,r5
  r5 = r5 - r6;
  // mtctr r7
  ctr = r7;
loc_825A74E8:
  // ldu r7,8(r4)
  ea = 8 + uint32_t(r4);
  r7 = REX_LOAD_U64(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stdu r7,8(r3)
  ea = 8 + uint32_t(r3);
  REX_STORE_U64(ea, r7);
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a74e8
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A74E8;
loc_825A74F4:
  // rlwinm r6,r5,25,7,31
  r6 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 25) & 0x1FFFFFF;
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq 0x825a747c
  if (cr0.eq) goto loc_825A747C;
  // addi r10,r5,127
  r10 = uint64_t(int64_t(int64_t(r5) + 127));
  // clrlwi r8,r5,25
  r8 = uint32_t(r5) & 0x7F;
  // rlwinm r10,r10,25,7,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr1,r8,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // clrlwi r10,r10,29
  r10 = uint32_t(r10) & 0x7;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // mtctr r10
  ctr = r10;
loc_825A7524:
  // addi r9,r9,128
  r9 = uint64_t(int64_t(int64_t(r9) + 128));
  // bdnz 0x825a7524
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7524;
  // add r12,r4,r5
  r12 = r4 + r5;
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // subf r11,r9,r12
  r11 = r12 - r9;
  // add r12,r3,r5
  r12 = r3 + r5;
  // mtctr r6
  ctr = r6;
loc_825A7544:
  // ld r6,8(r4)
  r6 = REX_LOAD_U64(uint32_t(r4) + 8);
  // ld r7,16(r4)
  r7 = REX_LOAD_U64(uint32_t(r4) + 16);
  // ld r8,24(r4)
  r8 = REX_LOAD_U64(uint32_t(r4) + 24);
  // std r6,8(r3)
  REX_STORE_U64(uint32_t(r3) + 8, r6);
  // ld r6,32(r4)
  r6 = REX_LOAD_U64(uint32_t(r4) + 32);
  // std r7,16(r3)
  REX_STORE_U64(uint32_t(r3) + 16, r7);
  // ld r7,40(r4)
  r7 = REX_LOAD_U64(uint32_t(r4) + 40);
  // std r8,24(r3)
  REX_STORE_U64(uint32_t(r3) + 24, r8);
  // ld r8,48(r4)
  r8 = REX_LOAD_U64(uint32_t(r4) + 48);
  // std r6,32(r3)
  REX_STORE_U64(uint32_t(r3) + 32, r6);
  // ld r6,56(r4)
  r6 = REX_LOAD_U64(uint32_t(r4) + 56);
  // std r7,40(r3)
  REX_STORE_U64(uint32_t(r3) + 40, r7);
  // ld r7,64(r4)
  r7 = REX_LOAD_U64(uint32_t(r4) + 64);
  // std r8,48(r3)
  REX_STORE_U64(uint32_t(r3) + 48, r8);
  // ld r8,72(r4)
  r8 = REX_LOAD_U64(uint32_t(r4) + 72);
  // std r6,56(r3)
  REX_STORE_U64(uint32_t(r3) + 56, r6);
  // ld r6,80(r4)
  r6 = REX_LOAD_U64(uint32_t(r4) + 80);
  // std r7,64(r3)
  REX_STORE_U64(uint32_t(r3) + 64, r7);
  // ld r7,88(r4)
  r7 = REX_LOAD_U64(uint32_t(r4) + 88);
  // std r8,72(r3)
  REX_STORE_U64(uint32_t(r3) + 72, r8);
  // ld r8,96(r4)
  r8 = REX_LOAD_U64(uint32_t(r4) + 96);
  // std r6,80(r3)
  REX_STORE_U64(uint32_t(r3) + 80, r6);
  // ld r6,104(r4)
  r6 = REX_LOAD_U64(uint32_t(r4) + 104);
  // std r7,88(r3)
  REX_STORE_U64(uint32_t(r3) + 88, r7);
  // ld r7,112(r4)
  r7 = REX_LOAD_U64(uint32_t(r4) + 112);
  // std r8,96(r3)
  REX_STORE_U64(uint32_t(r3) + 96, r8);
  // ld r8,120(r4)
  r8 = REX_LOAD_U64(uint32_t(r4) + 120);
  // std r6,104(r3)
  REX_STORE_U64(uint32_t(r3) + 104, r6);
  // ldu r6,128(r4)
  ea = 128 + uint32_t(r4);
  r6 = REX_LOAD_U64(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // std r7,112(r3)
  REX_STORE_U64(uint32_t(r3) + 112, r7);
  // std r8,120(r3)
  REX_STORE_U64(uint32_t(r3) + 120, r8);
  // stdu r6,128(r3)
  ea = 128 + uint32_t(r3);
  REX_STORE_U64(ea, r6);
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // cmplw r4,r11
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bge 0x825a75d8
  if (!cr0.lt) goto loc_825A75D8;
  // bdnz 0x825a7544
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7544;
  // b 0x825a747c
  goto loc_825A747C;
loc_825A75D8:
  // beq cr1,0x825a75e8
  if (cr1.eq) goto loc_825A75E8;
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmplwi cr1,r8,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
loc_825A75E8:
  // bdnz 0x825a7544
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7544;
  // b 0x825a747c
  goto loc_825A747C;
loc_825A75F0:
  // addi r4,r4,-4
  r4 = uint64_t(int64_t(int64_t(r4) + -4));
  // bge cr7,0x825a7648
  if (!cr7.lt) goto loc_825A7648;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
loc_825A7600:
  // rlwinm r7,r5,30,27,31
  r7 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 30) & 0x1F;
  // clrlwi r6,r5,30
  r6 = uint32_t(r5) & 0x3;
  // cmplwi cr1,r7,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq cr1,0x825a7624
  if (cr1.eq) goto loc_825A7624;
  // mtctr r7
  ctr = r7;
loc_825A7618:
  // lwzu r7,4(r4)
  ea = 4 + uint32_t(r4);
  r7 = REX_LOAD_U32(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r7));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a7618
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7618;
loc_825A7624:
  // beq cr6,0x825a7640
  if (cr6.eq) goto loc_825A7640;
  // addi r3,r3,3
  r3 = uint64_t(int64_t(int64_t(r3) + 3));
  // addi r4,r4,3
  r4 = uint64_t(int64_t(int64_t(r4) + 3));
  // mtctr r6
  ctr = r6;
loc_825A7634:
  // lbzu r7,1(r4)
  ea = 1 + uint32_t(r4);
  r7 = REX_LOAD_U8(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stbu r7,1(r3)
  ea = 1 + uint32_t(r3);
  REX_STORE_U8(ea, uint8_t(r7));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a7634
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7634;
loc_825A7640:
  // ld r3,-8(r1)
  r3 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return r3;
loc_825A7648:
  // clrlwi r6,r3,25
  r6 = uint32_t(r3) & 0x7F;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // subfic r6,r6,128
  ca = uint32_t(r6) <= 128;
  r6 = static_cast<uint64_t>(128) - r6;
  // beq 0x825a7674
  if (cr0.eq) goto loc_825A7674;
  // rlwinm r7,r6,30,2,31
  r7 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 30) & 0x3FFFFFFF;
  // subf r5,r6,r5
  r5 = r5 - r6;
  // mtctr r7
  ctr = r7;
loc_825A7668:
  // lwzu r7,4(r4)
  ea = 4 + uint32_t(r4);
  r7 = REX_LOAD_U32(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r7));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a7668
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7668;
loc_825A7674:
  // rlwinm r6,r5,25,7,31
  r6 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 25) & 0x1FFFFFF;
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq 0x825a7600
  if (cr0.eq) goto loc_825A7600;
  // addi r10,r5,127
  r10 = uint64_t(int64_t(int64_t(r5) + 127));
  // clrlwi r8,r5,25
  r8 = uint32_t(r5) & 0x7F;
  // rlwinm r10,r10,25,7,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr1,r8,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // clrlwi r10,r10,29
  r10 = uint32_t(r10) & 0x7;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // mtctr r10
  ctr = r10;
loc_825A76A4:
  // addi r9,r9,128
  r9 = uint64_t(int64_t(int64_t(r9) + 128));
  // bdnz 0x825a76a4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A76A4;
  // add r12,r4,r5
  r12 = r4 + r5;
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // subf r11,r9,r12
  r11 = r12 - r9;
  // add r12,r3,r5
  r12 = r3 + r5;
  // mtctr r6
  ctr = r6;
loc_825A76C4:
  // li r6,8
  r6 = uint64_t(int64_t(8));
loc_825A76C8:
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // lwz r0,4(r4)
  r0 = REX_LOAD_U32(uint32_t(r4) + 4);
  // lwz r7,8(r4)
  r7 = REX_LOAD_U32(uint32_t(r4) + 8);
  // lwz r8,12(r4)
  r8 = REX_LOAD_U32(uint32_t(r4) + 12);
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // stw r0,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r0));
  // lwzu r0,16(r4)
  ea = 16 + uint32_t(r4);
  r0 = REX_LOAD_U32(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stw r7,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r7));
  // stw r8,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r8));
  // stwu r0,16(r3)
  ea = 16 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r0));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bne 0x825a76c8
  if (!cr0.eq) goto loc_825A76C8;
  // cmplw r4,r11
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bge 0x825a7708
  if (!cr0.lt) goto loc_825A7708;
  // bdnz 0x825a76c4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A76C4;
  // b 0x825a7600
  goto loc_825A7600;
loc_825A7708:
  // beq cr1,0x825a7718
  if (cr1.eq) goto loc_825A7718;
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmplwi cr1,r8,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
loc_825A7718:
  // bdnz 0x825a76c4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A76C4;
  // b 0x825a7600
  goto loc_825A7600;
loc_825A7720:
  // addi r4,r4,-1
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  // bge cr7,0x825a7754
  if (!cr7.lt) goto loc_825A7754;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
loc_825A7730:
  // clrlwi r6,r5,25
  r6 = uint32_t(r5) & 0x7F;
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // mtctr r6
  ctr = r6;
  // beq 0x825a774c
  if (cr0.eq) goto loc_825A774C;
loc_825A7740:
  // lbzu r6,1(r4)
  ea = 1 + uint32_t(r4);
  r6 = REX_LOAD_U8(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stbu r6,1(r3)
  ea = 1 + uint32_t(r3);
  REX_STORE_U8(ea, uint8_t(r6));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a7740
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7740;
loc_825A774C:
  // ld r3,-8(r1)
  r3 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return r3;
loc_825A7754:
  // clrlwi r6,r3,25
  r6 = uint32_t(r3) & 0x7F;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // subfic r6,r6,128
  ca = uint32_t(r6) <= 128;
  r6 = static_cast<uint64_t>(128) - r6;
  // beq 0x825a777c
  if (cr0.eq) goto loc_825A777C;
  // subf r5,r6,r5
  r5 = r5 - r6;
  // mtctr r6
  ctr = r6;
loc_825A7770:
  // lbzu r6,1(r4)
  ea = 1 + uint32_t(r4);
  r6 = REX_LOAD_U8(ea);
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stbu r6,1(r3)
  ea = 1 + uint32_t(r3);
  REX_STORE_U8(ea, uint8_t(r6));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bdnz 0x825a7770
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A7770;
loc_825A777C:
  // rlwinm r6,r5,25,7,31
  r6 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 25) & 0x1FFFFFF;
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq 0x825a7730
  if (cr0.eq) goto loc_825A7730;
  // addi r10,r5,127
  r10 = uint64_t(int64_t(int64_t(r5) + 127));
  // clrlwi r8,r5,25
  r8 = uint32_t(r5) & 0x7F;
  // rlwinm r10,r10,25,7,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr1,r8,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // clrlwi r10,r10,29
  r10 = uint32_t(r10) & 0x7;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // mtctr r10
  ctr = r10;
loc_825A77AC:
  // addi r9,r9,128
  r9 = uint64_t(int64_t(int64_t(r9) + 128));
  // bdnz 0x825a77ac
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A77AC;
  // add r12,r4,r5
  r12 = r4 + r5;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // subf r11,r9,r12
  r11 = r12 - r9;
  // add r12,r3,r5
  r12 = r3 + r5;
  // mtctr r6
  ctr = r6;
loc_825A77CC:
  // li r6,32
  r6 = uint64_t(int64_t(32));
loc_825A77D0:
  // lbz r7,4(r4)
  r7 = REX_LOAD_U8(uint32_t(r4) + 4);
  // lbz r8,3(r4)
  r8 = REX_LOAD_U8(uint32_t(r4) + 3);
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // rlwimi r7,r8,8,16,23
  r7 = (__builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 8) & 0xFF00) | (r7 & 0xFFFFFFFFFFFF00FF);
  // lbz r9,2(r4)
  r9 = REX_LOAD_U8(uint32_t(r4) + 2);
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // rlwimi r7,r9,16,8,15
  r7 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 16) & 0xFF0000) | (r7 & 0xFFFFFFFFFF00FFFF);
  // lbz r10,1(r4)
  r10 = REX_LOAD_U8(uint32_t(r4) + 1);
  // addi r4,r4,4
  r4 = uint64_t(int64_t(int64_t(r4) + 4));
  // rlwimi r7,r10,24,0,7
  r7 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFF000000) | (r7 & 0xFFFFFFFF00FFFFFF);
  // stw r7,1(r3)
  REX_STORE_U32(uint32_t(r3) + 1, uint32_t(r7));
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // bne 0x825a77d0
  if (!cr0.eq) goto loc_825A77D0;
  // cmplw r4,r11
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bge 0x825a7818
  if (!cr0.lt) goto loc_825A7818;
  // bdnz 0x825a77cc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A77CC;
  // b 0x825a7730
  goto loc_825A7730;
loc_825A7818:
  // beq cr1,0x825a7828
  if (cr1.eq) goto loc_825A7828;
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmplwi cr1,r8,0
  cr1 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
loc_825A7828:
  // bdnz 0x825a77cc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A77CC;
  // b 0x825a7730
  goto loc_825A7730;
}

// sub_825BE708: lifted from generated/ by lift.py.
lifted::Result f825BE708(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825BE708 (lifted)");
  uint64_t r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r16,r4
  r16 = r4;
  // mr r15,r5
  r15 = r5;
  // mr r19,r6
  r19 = r6;
  // mr r17,r7
  r17 = r7;
  // ld r11,0(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 0);
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // ld r27,32(r31)
  r27 = REX_LOAD_U64(uint32_t(r31) + 32);
  // ld r28,24(r31)
  r28 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // ld r30,16(r31)
  r30 = REX_LOAD_U64(uint32_t(r31) + 16);
  // ld r29,8(r31)
  r29 = REX_LOAD_U64(uint32_t(r31) + 8);
  // beq cr6,0x825be75c
  if (cr6.eq) goto loc_825BE75C;
  // addi r6,r31,1792
  r6 = uint64_t(int64_t(int64_t(r31) + 1792));
  // li r5,16384
  r5 = uint64_t(int64_t(16384));
  // mr r4,r11
  r4 = r11;
  // bl 0x822ae1d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822AE1D0, LIFTED_ARGS(0x825BE758, r31)); r3 = o.r3; f1 = o.f1; }
  // std r26,0(r31)
  REX_STORE_U64(uint32_t(r31) + 0, r26);
loc_825BE75C:
  // cmpldi cr6,r29,0
  cr6 = lifted::Compare<uint64_t>(r29, 0, so);
  // beq cr6,0x825be77c
  if (cr6.eq) goto loc_825BE77C;
  // addi r6,r31,5888
  r6 = uint64_t(int64_t(int64_t(r31) + 5888));
  // ld r4,8(r31)
  r4 = REX_LOAD_U64(uint32_t(r31) + 8);
  // li r5,17408
  r5 = uint64_t(int64_t(17408));
  // mr r3,r31
  r3 = r31;
  // bl 0x822ae1d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822AE1D0, LIFTED_ARGS(0x825BE778, r31)); r3 = o.r3; f1 = o.f1; }
  // std r26,8(r31)
  REX_STORE_U64(uint32_t(r31) + 8, r26);
loc_825BE77C:
  // cmpldi cr6,r30,0
  cr6 = lifted::Compare<uint64_t>(r30, 0, so);
  // beq cr6,0x825be844
  if (cr6.eq) goto loc_825BE844;
  // li r12,15
  r12 = uint64_t(int64_t(15));
  // rldicr r12,r12,49,14
  r12 = __builtin_rotateleft64(r12, 49) & 0xFFFE000000000000;
  // and r11,r30,r12
  r11 = r30 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be7a8
  if (cr6.eq) goto loc_825BE7A8;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x822aed18
  // (left out: dead in C++)
  { const lifted::Result o = f822AED18(base, r1, r13, r3, r4, r7, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825BE7A8:
  // clrldi r11,r30,52
  r11 = r30 & 0xFFF;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be7f8
  if (cr6.eq) goto loc_825BE7F8;
  // rlwinm r11,r30,0,23,23
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x100;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be7d8
  if (cr6.eq) goto loc_825BE7D8;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r5,10432(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 10432);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x822adc08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADC08, LIFTED_ARGS(0x825BE7D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825BE7D8:
  // clrldi r11,r30,52
  r11 = r30 & 0xFFF;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be7f8
  if (cr6.eq) goto loc_825BE7F8;
  // addi r6,r31,10420
  r6 = uint64_t(int64_t(int64_t(r31) + 10420));
  // li r5,8704
  r5 = uint64_t(int64_t(8704));
  // rldicr r4,r30,52,11
  r4 = __builtin_rotateleft64(r30, 52) & 0xFFF0000000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADE30, LIFTED_ARGS(0x825BE7F8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE7F8:
  // li r12,-4096
  r12 = uint64_t(int64_t(-4096));
  // clrldi r12,r12,20
  r12 = r12 & 0xFFFFFFFFFFF;
  // and r11,r30,r12
  r11 = r30 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be818
  if (cr6.eq) goto loc_825BE818;
  // rldicr r4,r30,20,31
  r4 = __builtin_rotateleft64(r30, 20) & 0xFFFFFFFF00000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ae070
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822AE070, LIFTED_ARGS(0x825BE818, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE818:
  // li r12,31
  r12 = uint64_t(int64_t(31));
  // rldicr r12,r12,44,19
  r12 = __builtin_rotateleft64(r12, 44) & 0xFFFFF00000000000;
  // and r11,r30,r12
  r11 = r30 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be840
  if (cr6.eq) goto loc_825BE840;
  // addi r6,r31,10400
  r6 = uint64_t(int64_t(int64_t(r31) + 10400));
  // li r5,8576
  r5 = uint64_t(int64_t(8576));
  // rldicr r4,r30,15,4
  r4 = __builtin_rotateleft64(r30, 15) & 0xF800000000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADE30, LIFTED_ARGS(0x825BE840, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE840:
  // std r26,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r26);
loc_825BE844:
  // cmpldi cr6,r28,0
  cr6 = lifted::Compare<uint64_t>(r28, 0, so);
  // beq cr6,0x825be8ec
  if (cr6.eq) goto loc_825BE8EC;
  // rlwinm r11,r28,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0x2;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be874
  if (cr6.eq) goto loc_825BE874;
  // lis r11,-256
  r11 = uint64_t(int64_t(-16777216));
  // addi r6,r31,9984
  r6 = uint64_t(int64_t(int64_t(r31) + 9984));
  // li r5,18688
  r5 = uint64_t(int64_t(18688));
  // lis r4,-256
  r4 = uint64_t(int64_t(-16777216));
  // mr r3,r31
  r3 = r31;
  // std r11,11584(r31)
  REX_STORE_U64(uint32_t(r31) + 11584, r11);
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADE30, LIFTED_ARGS(0x825BE874, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE874:
  // rlwinm r11,r28,0,9,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0x7FFFFC;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be894
  if (cr6.eq) goto loc_825BE894;
  // addi r6,r31,10316
  r6 = uint64_t(int64_t(int64_t(r31) + 10316));
  // li r5,8448
  r5 = uint64_t(int64_t(8448));
  // rldicr r4,r28,41,20
  r4 = __builtin_rotateleft64(r28, 41) & 0xFFFFF80000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADE30, LIFTED_ARGS(0x825BE894, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE894:
  // lis r12,-128
  r12 = uint64_t(int64_t(-8388608));
  // clrldi r12,r12,25
  r12 = r12 & 0x7FFFFFFFFF;
  // and r11,r28,r12
  r11 = r28 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be8bc
  if (cr6.eq) goto loc_825BE8BC;
  // addi r6,r31,10240
  r6 = uint64_t(int64_t(int64_t(r31) + 10240));
  // li r5,8192
  r5 = uint64_t(int64_t(8192));
  // rldicr r4,r28,25,15
  r4 = __builtin_rotateleft64(r28, 25) & 0xFFFF000000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADE30, LIFTED_ARGS(0x825BE8BC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE8BC:
  // lis r12,31
  r12 = uint64_t(int64_t(2031616));
  // ori r12,r12,65535
  r12 = r12 | 65535;
  // rldicr r12,r12,39,24
  r12 = __builtin_rotateleft64(r12, 39) & 0xFFFFFF8000000000;
  // and r11,r28,r12
  r11 = r28 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be8e8
  if (cr6.eq) goto loc_825BE8E8;
  // addi r6,r31,10468
  r6 = uint64_t(int64_t(int64_t(r31) + 10468));
  // li r5,8832
  r5 = uint64_t(int64_t(8832));
  // rldicr r4,r28,4,20
  r4 = __builtin_rotateleft64(r28, 4) & 0xFFFFF80000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADE30, LIFTED_ARGS(0x825BE8E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE8E8:
  // std r26,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r26);
loc_825BE8EC:
  // cmpldi cr6,r27,0
  cr6 = lifted::Compare<uint64_t>(r27, 0, so);
  // beq cr6,0x825be960
  if (cr6.eq) goto loc_825BE960;
  // clrldi r11,r27,26
  r11 = r27 & 0x3FFFFFFFFF;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be914
  if (cr6.eq) goto loc_825BE914;
  // addi r6,r31,10552
  r6 = uint64_t(int64_t(int64_t(r31) + 10552));
  // li r5,8960
  r5 = uint64_t(int64_t(8960));
  // rldicr r4,r27,26,37
  r4 = __builtin_rotateleft64(r27, 26) & 0xFFFFFFFFFC000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADE30, LIFTED_ARGS(0x825BE914, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE914:
  // li r12,255
  r12 = uint64_t(int64_t(255));
  // rldicr r12,r12,38,25
  r12 = __builtin_rotateleft64(r12, 38) & 0xFFFFFFC000000000;
  // and r11,r27,r12
  r11 = r27 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be93c
  if (cr6.eq) goto loc_825BE93C;
  // addi r6,r31,10704
  r6 = uint64_t(int64_t(int64_t(r31) + 10704));
  // li r5,9088
  r5 = uint64_t(int64_t(9088));
  // rldicr r4,r27,18,7
  r4 = __builtin_rotateleft64(r27, 18) & 0xFF00000000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADE30, LIFTED_ARGS(0x825BE93C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE93C:
  // li r12,63
  r12 = uint64_t(int64_t(63));
  // rldicr r12,r12,49,14
  r12 = __builtin_rotateleft64(r12, 49) & 0xFFFE000000000000;
  // and r11,r27,r12
  r11 = r27 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825be95c
  if (cr6.eq) goto loc_825BE95C;
  // rldicr r4,r27,9,5
  r4 = __builtin_rotateleft64(r27, 9) & 0xFC00000000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822adef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ADEF0, LIFTED_ARGS(0x825BE95C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE95C:
  // std r26,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r26);
loc_825BE960:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r22,12180(r31)
  r22 = REX_LOAD_U32(uint32_t(r31) + 12180);
  // addi r21,r31,11866
  r21 = uint64_t(int64_t(int64_t(r31) + 11866));
  // addi r18,r11,2384
  r18 = uint64_t(int64_t(int64_t(r11) + 2384));
  // lis r11,-16381
  r11 = uint64_t(int64_t(-1073545216));
  // clrlwi r20,r16,26
  r20 = uint32_t(r16) & 0x3F;
  // ori r23,r11,8705
  r23 = r11 | 8705;
  // lis r11,-16384
  r11 = uint64_t(int64_t(-1073741824));
  // ori r25,r11,24576
  r25 = r11 | 24576;
loc_825BE984:
  // lwz r3,40(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // ble cr6,0x825be99c
  if (!cr6.gt) goto loc_825BE99C;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8229EF48, LIFTED_ARGS(0x825BE99C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BE99C:
  // li r11,8450
  r11 = uint64_t(int64_t(8450));
  // mr r24,r17
  r24 = r17;
  // cmplwi cr6,r17,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r17), 65535, so);
  // stwu r11,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r11));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // stwu r15,4(r27)
  ea = 4 + uint32_t(r27);
  REX_STORE_U32(ea, uint32_t(r15));
  r27 = (r27 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // ble cr6,0x825be9d8
  if (!cr6.gt) goto loc_825BE9D8;
  // rlwinm r11,r16,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r16) | (r16 << 32), 3) & 0xFFFFFFF8;
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // ori r10,r10,65535
  r10 = r10 | 65535;
  // lwzx r11,r11,r18
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r18));
  // divwu r10,r10,r11
  r10 = uint32_t(uint32_t(r11) ? uint32_t(r10) / uint32_t(r11) : 0);
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
  // rlwinm r10,r10,0,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFE;
  // mullw r24,r10,r11
  r24 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
loc_825BE9D8:
  // lwz r11,0(r22)
  r11 = REX_LOAD_U32(uint32_t(r22) + 0);
  // rlwinm r7,r24,16,0,15
  r7 = __builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 16) & 0xFFFF0000;
  // lbz r10,11931(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 11931);
  // rlwinm r9,r19,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r19) | (r19 << 32), 1) & 0xFFFFFFFE;
  // or r30,r7,r20
  r30 = r7 | r20;
  // lwz r8,24(r22)
  r8 = REX_LOAD_U32(uint32_t(r22) + 24);
  // subfic r5,r10,0
  ca = uint32_t(r10) <= 0;
  r5 = static_cast<uint64_t>(0) - r10;
  // mr r7,r11
  r7 = r11;
  // subfe r5,r5,r5
  temp.u8 = (~uint32_t(r5) + uint32_t(r5) < ~uint32_t(r5)) | (~uint32_t(r5) + uint32_t(r5) + ca < ca);
  r5 = ~r5 + r5 + ca;
  ca = temp.u8;
  // lbzx r10,r21,r10
  r10 = REX_LOAD_U8(uint32_t(r21) + uint32_t(r10));
  // add r9,r9,r8
  r9 = r9 + r8;
  // clrlwi r6,r24,8
  r6 = uint32_t(r24) & 0xFFFFFF;
  // rlwinm. r11,r11,0,0,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // clrlwi r11,r5,31
  r11 = uint32_t(r5) & 0x1;
  // rlwimi r10,r11,8,23,23
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0x100) | (r10 & 0xFFFFFFFFFFFFFEFF);
  // rlwinm r11,r7,1,0,1
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 1) & 0xC0000000;
  // clrlwi r26,r10,23
  r26 = uint32_t(r10) & 0x1FF;
  // rlwinm r10,r9,12,20,31
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 12) & 0xFFF;
  // clrlwi r9,r9,3
  r9 = uint32_t(r9) & 0x1FFFFFFF;
  // addi r10,r10,512
  r10 = uint64_t(int64_t(int64_t(r10) + 512));
  // rlwinm r26,r26,0,26,23
  r26 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 0) & 0xFFFFFFFFFFFFFF3F;
  // rlwinm r10,r10,0,19,19
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1000;
  // or r29,r6,r11
  r29 = r6 | r11;
  // add r28,r10,r9
  r28 = r10 + r9;
  // beq 0x825bea64
  if (cr0.eq) goto loc_825BEA64;
  // rlwinm r10,r19,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r19) | (r19 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r9,r24,1,8,30
  r9 = __builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 1) & 0xFFFFFE;
  // add r10,r10,r8
  r10 = r10 + r8;
  // or r29,r9,r11
  r29 = r9 | r11;
  // rlwinm r11,r10,12,20,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFF;
  // clrlwi r10,r10,3
  r10 = uint32_t(r10) & 0x1FFFFFFF;
  // addi r11,r11,512
  r11 = uint64_t(int64_t(int64_t(r11) + 512));
  // ori r30,r30,2048
  r30 = r30 | 2048;
  // rlwinm r11,r11,0,19,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x1000;
  // add r28,r11,r10
  r28 = r11 + r10;
loc_825BEA64:
  // lbz r11,10808(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10808);
  // rlwinm. r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825bea90
  if (!cr0.eq) goto loc_825BEA90;
  // mr r11,r23
  r11 = r23;
  // stwu r11,4(r27)
  ea = 4 + uint32_t(r27);
  REX_STORE_U32(ea, uint32_t(r11));
  r27 = (r27 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r26,4(r27)
  ea = 4 + uint32_t(r27);
  REX_STORE_U32(ea, uint32_t(r26));
  r27 = (r27 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r30,4(r27)
  ea = 4 + uint32_t(r27);
  REX_STORE_U32(ea, uint32_t(r30));
  r27 = (r27 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r28,4(r27)
  ea = 4 + uint32_t(r27);
  REX_STORE_U32(ea, uint32_t(r28));
  r27 = (r27 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r29,4(r27)
  ea = 4 + uint32_t(r27);
  REX_STORE_U32(ea, uint32_t(r29));
  r27 = (r27 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r27
  r11 = r27;
  // b 0x825beb4c
  goto loc_825BEB4C;
loc_825BEA90:
  // addi r3,r31,12976
  r3 = uint64_t(int64_t(int64_t(r31) + 12976));
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // addi r9,r10,16
  r9 = uint64_t(int64_t(int64_t(r10) + 16));
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // ble cr6,0x825beab0
  if (!cr6.gt) goto loc_825BEAB0;
  // bl 0x8229eb10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8229EB10, LIFTED_ARGS(0x825BEAAC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r3
  r10 = r3;
loc_825BEAB0:
  // addi r11,r10,16
  r11 = uint64_t(int64_t(int64_t(r10) + 16));
  // stw r27,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r27));
  // mr r9,r27
  r9 = r27;
  // mr r8,r25
  r8 = r25;
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // mr r6,r23
  r6 = r23;
  // stw r11,12984(r31)
  REX_STORE_U32(uint32_t(r31) + 12984, uint32_t(r11));
  // mr r5,r25
  r5 = r25;
  // lis r4,-32768
  r4 = uint64_t(int64_t(-2147483648));
  // stwu r8,4(r9)
  ea = 4 + uint32_t(r9);
  REX_STORE_U32(ea, uint32_t(r8));
  r9 = (r9 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // li r27,26
  r27 = uint64_t(int64_t(26));
  // mr r11,r9
  r11 = r9;
  // lis r9,-16383
  r9 = uint64_t(int64_t(-1073676288));
  // rlwinm r8,r10,12,20,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFF;
  // ori r3,r9,23041
  r3 = r9 | 23041;
  // lis r9,-16384
  r9 = uint64_t(int64_t(-1073741824));
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r14,25
  r14 = uint64_t(int64_t(25));
  // ori r9,r9,17921
  r9 = r9 | 17921;
  // stwu r6,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r6));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // rlwinm r9,r10,0,3,30
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1FFFFFFE;
  // addi r10,r8,512
  r10 = uint64_t(int64_t(int64_t(r8) + 512));
  // rlwinm r10,r10,0,19,19
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1000;
  // stwu r26,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r26));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // add r10,r10,r9
  r10 = r10 + r9;
  // ori r10,r10,1
  r10 = r10 | 1;
  // stwu r30,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r30));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r28,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r28));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r29,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r29));
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
  // stwu r27,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r27));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r7,80(r1)
  r7 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r14,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r14));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
loc_825BEB4C:
  // subf. r10,r24,r17
  r10 = r17 - r24;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
  // beq 0x825beb74
  if (cr0.eq) goto loc_825BEB74;
  // addi r9,r18,4
  r9 = uint64_t(int64_t(int64_t(r18) + 4));
  // rlwinm r11,r16,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r16) | (r16 << 32), 3) & 0xFFFFFFF8;
  // lwzx r11,r11,r9
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  // subf r9,r11,r24
  r9 = r24 - r11;
  // add r17,r11,r10
  r17 = r11 + r10;
  // add r19,r9,r19
  r19 = r9 + r19;
  // b 0x825be984
  goto loc_825BE984;
loc_825BEB74:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C31D8: lifted from generated/ by lift.py.
lifted::Result f825C31D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C31D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // mr r29,r7
  r29 = r7;
  // mr r26,r10
  r26 = r10;
  // mr r27,r3
  r27 = r3;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r7,r28
  r7 = r28;
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // mtctr r10
  ctr = r10;
loc_825C3204:
  // std r7,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r7);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bdnz 0x825c3204
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825C3204;
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lis r4,-32164
  r4 = uint64_t(int64_t(-2107899904));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // addi r11,r4,1160
  r11 = uint64_t(int64_t(int64_t(r4) + 1160));
  // stw r11,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r11));
  // clrlwi r11,r10,30
  r11 = uint32_t(r10) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x825c323c
  if (cr6.eq) goto loc_825C323C;
  // lis r7,-32164
  r7 = uint64_t(int64_t(-2107899904));
  // addi r7,r7,-520
  r7 = uint64_t(int64_t(int64_t(r7) + -520));
  // stw r7,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r7));
loc_825C323C:
  // li r7,6
  r7 = uint64_t(int64_t(6));
  // stb r5,155(r1)
  REX_STORE_U8(uint32_t(r1) + 155, uint8_t(r5));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // stb r7,154(r1)
  REX_STORE_U8(uint32_t(r1) + 154, uint8_t(r7));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lfs f0,8680(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 8680);
  f0 = double(temp.f32);
  // stfs f0,156(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 156, temp.u32);
  // bne cr6,0x825c32c4
  if (!cr6.eq) goto loc_825C32C4;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // rlwinm r30,r10,30,29,31
  r30 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x7;
  // mr r7,r28
  r7 = r28;
  // stb r11,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r11));
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // rlwinm r11,r11,31,25,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x7F;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stb r11,100(r1)
  REX_STORE_U8(uint32_t(r1) + 100, uint8_t(r11));
  // beq cr6,0x825c32f4
  if (cr6.eq) goto loc_825C32F4;
  // rlwinm r31,r10,27,14,31
  r31 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x3FFFF;
  // mr r10,r28
  r10 = r28;
  // addi r11,r1,104
  r11 = uint64_t(int64_t(int64_t(r1) + 104));
loc_825C328C:
  // subf r5,r10,r30
  r5 = r30 - r10;
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // subfic r5,r5,1
  ca = uint32_t(r5) <= 1;
  r5 = static_cast<uint64_t>(1) - r5;
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // subfe r5,r5,r5
  temp.u8 = (~uint32_t(r5) + uint32_t(r5) < ~uint32_t(r5)) | (~uint32_t(r5) + uint32_t(r5) + ca < ca);
  r5 = ~r5 + r5 + ca;
  ca = temp.u8;
  // clrlwi r3,r5,31
  r3 = uint32_t(r5) & 0x1;
  // addi r5,r3,1
  r5 = uint64_t(int64_t(int64_t(r3) + 1));
  // stb r5,4(r11)
  REX_STORE_U8(uint32_t(r11) + 4, uint8_t(r5));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // lbz r5,100(r1)
  r5 = REX_LOAD_U8(uint32_t(r1) + 100);
  // cmpw cr6,r7,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r5), so);
  // blt cr6,0x825c328c
  if (cr6.lt) goto loc_825C328C;
  // b 0x825c32f4
  goto loc_825C32F4;
loc_825C32C4:
  // rlwinm r11,r10,0,0,0
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x80000000;
  // lis r7,-32768
  r7 = uint64_t(int64_t(-2147483648));
  // rlwinm r5,r10,30,29,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x7;
  // subf r11,r11,r7
  r11 = r7 - r11;
  // rlwinm r10,r10,27,14,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x3FFFF;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // stb r5,100(r1)
  REX_STORE_U8(uint32_t(r1) + 100, uint8_t(r5));
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r11,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r11));
loc_825C32F4:
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // beq cr6,0x825c3304
  if (cr6.eq) goto loc_825C3304;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,152(r1)
  REX_STORE_U8(uint32_t(r1) + 152, uint8_t(r11));
loc_825C3304:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r28,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r28));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r6,r9
  r6 = r9;
  // mr r5,r8
  r5 = r8;
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stb r11,153(r1)
  REX_STORE_U8(uint32_t(r1) + 153, uint8_t(r11));
  // bl 0x825c9c40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C9C40, LIFTED_ARGS(0x825C3328, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c338c
  if (cr6.lt) goto loc_825C338C;
  // addi r31,r27,12
  r31 = uint64_t(int64_t(int64_t(r27) + 12));
  // lwz r30,80(r1)
  r30 = REX_LOAD_U32(uint32_t(r1) + 80);
  // mr r3,r31
  r3 = r31;
  // bl 0x825c2cc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C2CC8, LIFTED_ARGS(0x825C3344, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c3384
  if (cr6.eq) goto loc_825C3384;
  // stw r30,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r30));
  // stw r28,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r28));
  // lwz r11,208(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 208);
  // stw r11,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r11));
  // lwz r11,208(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 208);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c3370
  if (cr6.eq) goto loc_825C3370;
  // stw r3,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r3));
  // b 0x825c3374
  goto loc_825C3374;
loc_825C3370:
  // stw r3,204(r31)
  REX_STORE_U32(uint32_t(r31) + 204, uint32_t(r3));
loc_825C3374:
  // lwz r11,212(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 212);
  // stw r3,208(r31)
  REX_STORE_U32(uint32_t(r31) + 208, uint32_t(r3));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,212(r31)
  REX_STORE_U32(uint32_t(r31) + 212, uint32_t(r11));
loc_825C3384:
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_825C338C:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

lifted::Result e82091A60(uint8_t* base, const lifted::Args& a) {
  return f82091A60(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820920D8(uint8_t* base, const lifted::Args& a) {
  return f820920D8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82099B18(uint8_t* base, const lifted::Args& a) {
  f82099B18(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8209A080(uint8_t* base, const lifted::Args& a) {
  return {f8209A080(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8209BD50(uint8_t* base, const lifted::Args& a) {
  f8209BD50(base, a.r1, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8209F738(uint8_t* base, const lifted::Args& a) {
  return f8209F738(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.lr);
}

lifted::Result e820A2D30(uint8_t* base, const lifted::Args& a) {
  f820A2D30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8F80(uint8_t* base, const lifted::Args& a) {
  return f820A8F80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A9198(uint8_t* base, const lifted::Args& a) {
  return f820A9198(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820AC7F0(uint8_t* base, const lifted::Args& a) {
  return f820AC7F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AC8A0(uint8_t* base, const lifted::Args& a) {
  return f820AC8A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820B18B8(uint8_t* base, const lifted::Args& a) {
  return f820B18B8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e820B2100(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820B2100(base, a.r1, a.r13, a.r[0], a.f[0])};
}

lifted::Result e820B5670(uint8_t* base, const lifted::Args& a) {
  return f820B5670(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e820B5B08(uint8_t* base, const lifted::Args& a) {
  return f820B5B08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e820B5FD8(uint8_t* base, const lifted::Args& a) {
  return f820B5FD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[5], a.f[0], a.f[1], a.f[2]);
}

lifted::Result e820B6498(uint8_t* base, const lifted::Args& a) {
  return f820B6498(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820B7998(uint8_t* base, const lifted::Args& a) {
  return f820B7998(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e820B81C8(uint8_t* base, const lifted::Args& a) {
  return f820B81C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820BB758(uint8_t* base, const lifted::Args& a) {
  return f820BB758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e820BBE30(uint8_t* base, const lifted::Args& a) {
  return f820BBE30(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e820BFD30(uint8_t* base, const lifted::Args& a) {
  return f820BFD30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3]);
}

lifted::Result e820C0118(uint8_t* base, const lifted::Args& a) {
  return f820C0118(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e820C1D08(uint8_t* base, const lifted::Args& a) {
  return f820C1D08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820C20E0(uint8_t* base, const lifted::Args& a) {
  return f820C20E0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820C23E8(uint8_t* base, const lifted::Args& a) {
  return f820C23E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e820C27E8(uint8_t* base, const lifted::Args& a) {
  return f820C27E8(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e820C3608(uint8_t* base, const lifted::Args& a) {
  return f820C3608(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.f[0], a.f[1]);
}

lifted::Result e820C3B78(uint8_t* base, const lifted::Args& a) {
  return f820C3B78(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820C3BD0(uint8_t* base, const lifted::Args& a) {
  return f820C3BD0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820C5C88(uint8_t* base, const lifted::Args& a) {
  return f820C5C88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820CC470(uint8_t* base, const lifted::Args& a) {
  f820CC470(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820D09A0(uint8_t* base, const lifted::Args& a) {
  return f820D09A0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e820D3318(uint8_t* base, const lifted::Args& a) {
  return {f820D3318(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]), a.f[0]};
}

lifted::Result e820D3688(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820D3688(base, a.r[0], a.r[1], a.r[2])};
}

lifted::Result e820D3740(uint8_t* base, const lifted::Args& a) {
  f820D3740(base, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820D47A8(uint8_t* base, const lifted::Args& a) {
  return f820D47A8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820D4F80(uint8_t* base, const lifted::Args& a) {
  return f820D4F80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820D5890(uint8_t* base, const lifted::Args& a) {
  return f820D5890(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820D6108(uint8_t* base, const lifted::Args& a) {
  return f820D6108(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e820D63A0(uint8_t* base, const lifted::Args& a) {
  return f820D63A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820D6A70(uint8_t* base, const lifted::Args& a) {
  return f820D6A70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820D7428(uint8_t* base, const lifted::Args& a) {
  return f820D7428(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820D78B8(uint8_t* base, const lifted::Args& a) {
  return f820D78B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820D7D38(uint8_t* base, const lifted::Args& a) {
  return f820D7D38(base, a.r1, a.r13, a.r[0], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820D7EE8(uint8_t* base, const lifted::Args& a) {
  return {f820D7EE8(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820D8908(uint8_t* base, const lifted::Args& a) {
  return f820D8908(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820D9570(uint8_t* base, const lifted::Args& a) {
  return f820D9570(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820DB290(uint8_t* base, const lifted::Args& a) {
  return {f820DB290(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820F0070(uint8_t* base, const lifted::Args& a) {
  return f820F0070(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820F4128(uint8_t* base, const lifted::Args& a) {
  return {f820F4128(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[6], a.f[0], a.f[1], a.f[2]), a.f[0]};
}

lifted::Result e820F8618(uint8_t* base, const lifted::Args& a) {
  return f820F8618(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820FEFB0(uint8_t* base, const lifted::Args& a) {
  return f820FEFB0(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820FFD88(uint8_t* base, const lifted::Args& a) {
  return {f820FFD88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8211AED0(uint8_t* base, const lifted::Args& a) {
  return {f8211AED0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8211BD10(uint8_t* base, const lifted::Args& a) {
  return {f8211BD10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e82128E70(uint8_t* base, const lifted::Args& a) {
  return f82128E70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8213B1E0(uint8_t* base, const lifted::Args& a) {
  return f8213B1E0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8213B2B8(uint8_t* base, const lifted::Args& a) {
  return f8213B2B8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82150350(uint8_t* base, const lifted::Args& a) {
  return f82150350(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[6], a.r[7]);
}

lifted::Result e8216C2A0(uint8_t* base, const lifted::Args& a) {
  return f8216C2A0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821715B8(uint8_t* base, const lifted::Args& a) {
  return f821715B8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8217C368(uint8_t* base, const lifted::Args& a) {
  return f8217C368(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e82294330(uint8_t* base, const lifted::Args& a) {
  f82294330(base, a.r1, a.r13, a.r[0], a.r[1], a.lr);
  return {a.r[0], a.f[0]};
}

lifted::Result e82295330(uint8_t* base, const lifted::Args& a) {
  return f82295330(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822A5B78(uint8_t* base, const lifted::Args& a) {
  return {f822A5B78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.lr), a.f[0]};
}

lifted::Result e822A6008(uint8_t* base, const lifted::Args& a) {
  return f822A6008(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822A8740(uint8_t* base, const lifted::Args& a) {
  return {f822A8740(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82594D10(uint8_t* base, const lifted::Args& a) {
  return {f82594D10(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.lr), a.f[0]};
}

lifted::Result e82596920(uint8_t* base, const lifted::Args& a) {
  return f82596920(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825986B8(uint8_t* base, const lifted::Args& a) {
  return f825986B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e825998A0(uint8_t* base, const lifted::Args& a) {
  return f825998A0(base, a.r1, a.r13, a.r[0], a.f[0], a.lr);
}

lifted::Result e8259F770(uint8_t* base, const lifted::Args& a) {
  return f8259F770(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e8259FB30(uint8_t* base, const lifted::Args& a) {
  return f8259FB30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259FC18(uint8_t* base, const lifted::Args& a) {
  return f8259FC18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A7068(uint8_t* base, const lifted::Args& a) {
  return f825A7068(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A70E8(uint8_t* base, const lifted::Args& a) {
  return f825A70E8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825A7410(uint8_t* base, const lifted::Args& a) {
  return {f825A7410(base, a.r1, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825BE708(uint8_t* base, const lifted::Args& a) {
  return f825BE708(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825C31D8(uint8_t* base, const lifted::Args& a) {
  return f825C31D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

void c82091A60(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e82091A60); }
void c820920D8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820920D8); }
void c82099B18(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e82099B18); }
void c8209A080(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8209A080); }
void c8209BD50(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8209BD50); }
void c8209F738(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8209F738); }
void c820A2D30(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820A2D30); }
void c820A8F80(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820A8F80); }
void c820A9198(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820A9198); }
void c820AC7F0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820AC7F0); }
void c820AC8A0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820AC8A0); }
void c820B18B8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820B18B8); }
void c820B2100(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820B2100); }
void c820B5670(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820B5670); }
void c820B5B08(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820B5B08); }
void c820B5FD8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820B5FD8); }
void c820B6498(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820B6498); }
void c820B7998(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820B7998); }
void c820B81C8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820B81C8); }
void c820BB758(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820BB758); }
void c820BBE30(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820BBE30); }
void c820BFD30(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820BFD30); }
void c820C0118(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C0118); }
void c820C1D08(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C1D08); }
void c820C20E0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C20E0); }
void c820C23E8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C23E8); }
void c820C27E8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C27E8); }
void c820C3608(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C3608); }
void c820C3B78(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C3B78); }
void c820C3BD0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C3BD0); }
void c820C5C88(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820C5C88); }
void c820CC470(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820CC470); }
void c820D09A0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D09A0); }
void c820D3318(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D3318); }
void c820D3688(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D3688); }
void c820D3740(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D3740); }
void c820D47A8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D47A8); }
void c820D4F80(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D4F80); }
void c820D5890(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D5890); }
void c820D6108(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D6108); }
void c820D63A0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D63A0); }
void c820D6A70(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D6A70); }
void c820D7428(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D7428); }
void c820D78B8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D78B8); }
void c820D7D38(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D7D38); }
void c820D7EE8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D7EE8); }
void c820D8908(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D8908); }
void c820D9570(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820D9570); }
void c820DB290(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820DB290); }
void c820F0070(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820F0070); }
void c820F4128(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820F4128); }
void c820F8618(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820F8618); }
void c820FEFB0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820FEFB0); }
void c820FFD88(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e820FFD88); }
void c8211AED0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8211AED0); }
void c8211BD10(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8211BD10); }
void c82128E70(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e82128E70); }
void c8213B1E0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8213B1E0); }
void c8213B2B8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8213B2B8); }
void c82150350(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e82150350); }
void c8216C2A0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8216C2A0); }
void c821715B8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e821715B8); }
void c8217C368(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8217C368); }
void c82294330(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e82294330); }
void c82295330(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e82295330); }
void c822A5B78(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e822A5B78); }
void c822A6008(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e822A6008); }
void c822A8740(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e822A8740); }
void c82594D10(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e82594D10); }
void c82596920(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e82596920); }
void c825986B8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e825986B8); }
void c825998A0(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e825998A0); }
void c8259F770(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8259F770); }
void c8259FB30(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8259FB30); }
void c8259FC18(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e8259FC18); }
void c825A7068(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e825A7068); }
void c825A70E8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e825A70E8); }
void c825A7410(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e825A7410); }
void c825BE708(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e825BE708); }
void c825C31D8(PPCContext& ctx, uint8_t* base) { Enter(ctx, base, e825C31D8); }

}  // namespace lifted::original
