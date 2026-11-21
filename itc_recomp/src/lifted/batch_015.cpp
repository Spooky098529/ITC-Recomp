// Lift sweep batch 015 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/device_state.h"
#include "schema/predicated_tiling.h"
#include "schema/texture_compression.h"
#include "schema/video_output.h"
#include "schema/xaudio.h"
#include "schema/xaudio_engine.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_015, true, "ITC",
              "Run lift sweep batch 015's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_822B2598: lifted from generated/ by lift.py.
void f822B2598(uint8_t* base, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_822B2598 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::ScalerParameters(base, uint32_t(r4)).source_left_address());
  // stw r11,0(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).source_left_address(), uint32_t(r11));
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, schema::ScalerParameters(base, uint32_t(r4)).source_top_address());
  // stw r11,4(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).source_top_address(), uint32_t(r11));
  // lwz r11,8(r4)
  r11 = guest::LoadU32(base, schema::ScalerParameters(base, uint32_t(r4)).source_right_address());
  // stw r11,8(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).source_right_address(), uint32_t(r11));
  // lwz r11,12(r4)
  r11 = guest::LoadU32(base, schema::ScalerParameters(base, uint32_t(r4)).source_bottom_address());
  // stw r11,12(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).source_bottom_address(), uint32_t(r11));
  // lwz r11,16(r4)
  r11 = guest::LoadU32(base, schema::ScalerParameters(base, uint32_t(r4)).scaled_width_address());
  // stw r11,16(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).scaled_width_address(), uint32_t(r11));
  // lwz r11,20(r4)
  r11 = guest::LoadU32(base, schema::ScalerParameters(base, uint32_t(r4)).scaled_height_address());
  // stw r11,20(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).scaled_height_address(), uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,24(r4)
  r10 = guest::LoadU32(base, schema::ScalerParameters(base, uint32_t(r4)).filter_profile_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x822b25e4
  if (!cr6.eq) goto loc_822B25E4;
  // stw r11,24(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).horizontal_filter_address(), uint32_t(r11));
  // stw r11,40(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).vertical_filter_address(), uint32_t(r11));
  // b 0x822b25f0
  goto loc_822B25F0;
loc_822B25E4:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r10,24(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).horizontal_filter_address(), uint32_t(r10));
  // stw r10,40(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).vertical_filter_address(), uint32_t(r10));
loc_822B25F0:
  // stw r11,28(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).unknown_28_address(), uint32_t(r11));
  // stw r11,32(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).unknown_32_address(), uint32_t(r11));
  // stw r11,36(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).unknown_36_address(), uint32_t(r11));
  // stw r11,44(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).unknown_44_address(), uint32_t(r11));
  // stw r11,48(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).unknown_48_address(), uint32_t(r11));
  // stw r11,52(r5)
  guest::StoreU32(base, schema::ScalerSetUp(base, uint32_t(r5)).unknown_52_address(), uint32_t(r11));
  // blr
  return;
}

// sub_822B4250: lifted from generated/ by lift.py.
lifted::Result f822B4250(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822B4250 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,13428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 13428);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b4290
  if (cr0.eq) goto loc_822B4290;
  // lwz r10,40(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 40);
  // lwz r11,36(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 36);
  // rlwinm r10,r10,2,30,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0x2;
  // clrlwi r9,r11,19
  r9 = uint32_t(r11) & 0x1FFF;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r11,r11,19,19,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 19) & 0x1FFF;
  // add r30,r9,r10
  r30 = r9 + r10;
  // add r29,r11,r10
  r29 = r11 + r10;
  // b 0x822b4298
  goto loc_822B4298;
loc_822B4290:
  // lwz r30,13188(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 13188);
  // lwz r29,13192(r31)
  r29 = guest::LoadU32(base, uint32_t(r31) + 13192);
loc_822B4298:
  // addi r28,r31,13368
  r28 = uint64_t(int64_t(int64_t(r31) + 13368));
  // li r5,56
  r5 = uint64_t(int64_t(56));
  // mr r3,r28
  r3 = r28;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822B42A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,10810(r31)
  r11 = guest::LoadU8(base, uint32_t(r31) + 10810);
  // mr r6,r28
  r6 = r28;
  // ori r11,r11,32
  r11 = r11 | 32;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // stb r11,10810(r31)
  guest::StoreU8(base, uint32_t(r31) + 10810, uint8_t(r11));
  // bl 0x822b2610
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822B2610(base, LIFTED_ARGS(0x822B42C8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822B42D0: lifted from generated/ by lift.py.
lifted::Result f822B42D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822B42D0 (lifted)");
  uint64_t r5 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822b2598
  // (left out: dead in C++)
  f822B2598(base, r4, r5);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822b4250
  // (left out: dead in C++)
  { const lifted::Result o = f822B4250(base, r1, r13, r3, r4, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822B4300: lifted from generated/ by lift.py.
uint64_t f822B4300(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822B4300 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stb r11,15901(r10)
  guest::StoreU8(base, uint32_t(r10) + 15901, uint8_t(r11));
  // blr
  return r3;
}

// sub_822B4318: lifted from generated/ by lift.py.
uint64_t f822B4318(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822B4318 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stb r11,15901(r10)
  guest::StoreU8(base, uint32_t(r10) + 15901, uint8_t(r11));
  // blr
  return r3;
}

// sub_822B4330: lifted from generated/ by lift.py.
lifted::Result f822B4330(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822B4330 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x825aaf08
  // (left out: dead in C++)
  { const lifted::Result o = game::atol(base, LIFTED_ARGS(0x822B4340, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,-1
  r11 = uint64_t(int64_t(int64_t(r3) + -1));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822B4360: lifted from generated/ by lift.py.
uint64_t f822B4360(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822B4360 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r4,r3
  r4 = r3;
  // addi r31,r11,15640
  r31 = uint64_t(int64_t(int64_t(r11) + 15640));
  // li r5,260
  r5 = uint64_t(int64_t(260));
  // mr r3,r31
  r3 = r31;
  // bl 0x825a7f50
  // (left out: dead in C++)
  { const lifted::Result o = game::strncpy(base, LIFTED_ARGS(0x822B4388, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stb r11,260(r31)
  guest::StoreU8(base, uint32_t(r31) + 260, uint8_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_822B43A8: lifted from generated/ by lift.py.
uint64_t f822B43A8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822B43A8 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stb r11,15900(r10)
  guest::StoreU8(base, uint32_t(r10) + 15900, uint8_t(r11));
  // blr
  return r3;
}

// sub_822B43E8: lifted from generated/ by lift.py.
lifted::Result f822B43E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822B43E8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-496(r1)
  ea = -496 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // mr r11,r3
  r11 = r3;
  // addi r10,r10,24688
  r10 = uint64_t(int64_t(int64_t(r10) + 24688));
  // addi r30,r11,6
  r30 = uint64_t(int64_t(int64_t(r11) + 6));
  // mr r27,r4
  r27 = r4;
  // mr r26,r5
  r26 = r5;
  // lis r25,-32038
  r25 = uint64_t(int64_t(-2099642368));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // li r5,54
  r5 = uint64_t(int64_t(54));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,90
  r3 = uint64_t(int64_t(int64_t(r1) + 90));
  // ori r25,r25,7
  r25 = r25 | 7;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 4);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lhz r11,8(r10)
  r11 = guest::LoadU16(base, uint32_t(r10) + 8);
  // sth r11,88(r1)
  guest::StoreU16(base, uint32_t(r1) + 88, uint16_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822B443C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32213
  r10 = uint64_t(int64_t(-2111111168));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r10,r10,17152
  r10 = uint64_t(int64_t(int64_t(r10) + 17152));
  // addi r11,r11,24672
  r11 = uint64_t(int64_t(int64_t(r11) + 24672));
  // li r5,51
  r5 = uint64_t(int64_t(51));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,161
  r3 = uint64_t(int64_t(int64_t(r1) + 161));
  // stw r10,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r10));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r10,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lbz r11,12(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 12);
  // stw r10,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r10));
  // stb r11,160(r1)
  guest::StoreU8(base, uint32_t(r1) + 160, uint8_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822B4480, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32213
  r10 = uint64_t(int64_t(-2111111168));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r10,r10,17176
  r10 = uint64_t(int64_t(int64_t(r10) + 17176));
  // addi r11,r11,24660
  r11 = uint64_t(int64_t(int64_t(r11) + 24660));
  // li r5,54
  r5 = uint64_t(int64_t(54));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,226
  r3 = uint64_t(int64_t(int64_t(r1) + 226));
  // stw r10,212(r1)
  guest::StoreU32(base, uint32_t(r1) + 212, uint32_t(r10));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,216(r1)
  guest::StoreU32(base, uint32_t(r1) + 216, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lhz r11,8(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 8);
  // stw r10,220(r1)
  guest::StoreU32(base, uint32_t(r1) + 220, uint32_t(r10));
  // sth r11,224(r1)
  guest::StoreU16(base, uint32_t(r1) + 224, uint16_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822B44BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32213
  r10 = uint64_t(int64_t(-2111111168));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r10,r10,17200
  r10 = uint64_t(int64_t(int64_t(r10) + 17200));
  // addi r11,r11,24644
  r11 = uint64_t(int64_t(int64_t(r11) + 24644));
  // li r5,49
  r5 = uint64_t(int64_t(49));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,299
  r3 = uint64_t(int64_t(int64_t(r1) + 299));
  // stw r10,280(r1)
  guest::StoreU32(base, uint32_t(r1) + 280, uint32_t(r10));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,284(r1)
  guest::StoreU32(base, uint32_t(r1) + 284, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r10,288(r1)
  guest::StoreU32(base, uint32_t(r1) + 288, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // stw r10,292(r1)
  guest::StoreU32(base, uint32_t(r1) + 292, uint32_t(r10));
  // lhz r10,12(r11)
  r10 = guest::LoadU16(base, uint32_t(r11) + 12);
  // lbz r11,14(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 14);
  // sth r10,296(r1)
  guest::StoreU16(base, uint32_t(r1) + 296, uint16_t(r10));
  // stb r11,298(r1)
  guest::StoreU8(base, uint32_t(r1) + 298, uint8_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822B4508, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32213
  r10 = uint64_t(int64_t(-2111111168));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r10,r10,17248
  r10 = uint64_t(int64_t(int64_t(r10) + 17248));
  // addi r11,r11,24628
  r11 = uint64_t(int64_t(int64_t(r11) + 24628));
  // li r5,51
  r5 = uint64_t(int64_t(51));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,365
  r3 = uint64_t(int64_t(int64_t(r1) + 365));
  // stw r10,348(r1)
  guest::StoreU32(base, uint32_t(r1) + 348, uint32_t(r10));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,352(r1)
  guest::StoreU32(base, uint32_t(r1) + 352, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r10,356(r1)
  guest::StoreU32(base, uint32_t(r1) + 356, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lbz r11,12(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 12);
  // stw r10,360(r1)
  guest::StoreU32(base, uint32_t(r1) + 360, uint32_t(r10));
  // stb r11,364(r1)
  guest::StoreU8(base, uint32_t(r1) + 364, uint8_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822B454C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32213
  r11 = uint64_t(int64_t(-2111111168));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r11,r11,17320
  r11 = uint64_t(int64_t(int64_t(r11) + 17320));
  // addi r31,r1,80
  r31 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,416(r1)
  guest::StoreU32(base, uint32_t(r1) + 416, uint32_t(r11));
loc_822B4560:
  // mr r11,r31
  r11 = r31;
  // mr r10,r11
  r10 = r11;
loc_822B4568:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x822b4568
  if (!cr6.eq) goto loc_822B4568;
  // subf r11,r10,r11
  r11 = r11 - r10;
  // mr r4,r31
  r4 = r31;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // mr r3,r30
  r3 = r30;
  // rotlwi r28,r11,0
  r28 = __builtin_rotateleft32(uint32_t(r11), 0);
  // mr r5,r28
  r5 = r28;
  // bl 0x825aacd0
  // (left out: dead in C++)
  { const lifted::Result o = game::strncmp(base, LIFTED_ARGS(0x822B4594, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b45cc
  if (cr0.eq) goto loc_822B45CC;
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r31,r31,68
  r31 = uint64_t(int64_t(int64_t(r31) + 68));
  // cmplwi cr6,r29,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 5, so);
  // blt cr6,0x822b4560
  if (cr6.lt) goto loc_822B4560;
loc_822B45AC:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r4,r11,17736
  r4 = uint64_t(int64_t(int64_t(r11) + 17736));
loc_822B45B4:
  // mr r5,r26
  r5 = r26;
  // mr r3,r27
  r3 = r27;
  // bl 0x825a7f50
  // (left out: dead in C++)
  { const lifted::Result o = game::strncpy(base, LIFTED_ARGS(0x822B45C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,496
  r1 = uint64_t(int64_t(int64_t(r1) + 496));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_822B45CC:
  // mulli r10,r29,68
  r10 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(68))));
  // addi r9,r1,144
  r9 = uint64_t(int64_t(int64_t(r1) + 144));
  // add r11,r28,r30
  r11 = r28 + r30;
  // addi r3,r11,1
  r3 = uint64_t(int64_t(int64_t(r11) + 1));
  // lwzx r11,r10,r9
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B45E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r25,730
  r25 = uint64_t(int64_t(47841280));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b45ac
  if (cr0.eq) goto loc_822B45AC;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r4,r11,17744
  r4 = uint64_t(int64_t(int64_t(r11) + 17744));
  // b 0x822b45b4
  goto loc_822B45B4;
}

// sub_822B4680: lifted from generated/ by lift.py.
lifted::Result f822B4680(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822B4680 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r31,r3,20128
  r31 = uint64_t(int64_t(int64_t(r3) + 20128));
  // lis r30,-20096
  r30 = uint64_t(int64_t(-1317011456));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r31) - 20128).capture_memory_0_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822b46d4
  if (cr0.eq) goto loc_822B46D4;
  // lwz r11,300(r31)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r31) - 20128).capture_flags_address());
  // srawi r11,r11,30
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3FFFFFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 30));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822b46c8
  if (cr6.lt) goto loc_822B46C8;
  // bne cr6,0x822b46d4
  if (!cr6.eq) goto loc_822B46D4;
  // mr r4,r30
  r4 = r30;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x822B46C4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822b46d4
  goto loc_822B46D4;
loc_822B46C8:
  // mr r4,r3
  r4 = r3;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x825db7a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__MmFreePhysicalMemory, LIFTED_ARGS(0x822B46D4, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B46D4:
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r31) - 20128).capture_memory_1_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822b4710
  if (cr0.eq) goto loc_822B4710;
  // lwz r11,300(r31)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r31) - 20128).capture_flags_address());
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // srawi r11,r11,30
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3FFFFFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 30));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822b4704
  if (cr6.lt) goto loc_822B4704;
  // bne cr6,0x822b4710
  if (!cr6.eq) goto loc_822B4710;
  // mr r4,r30
  r4 = r30;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x822B4700, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822b4710
  goto loc_822B4710;
loc_822B4704:
  // mr r4,r3
  r4 = r3;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x825db7a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__MmFreePhysicalMemory, LIFTED_ARGS(0x822B4710, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B4710:
  // li r5,312
  r5 = uint64_t(int64_t(312));
  // lwz r30,304(r31)
  r30 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r31) - 20128).unknown_20432_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822B4724, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,8
  r11 = uint64_t(int64_t(int64_t(r31) + 8));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r30,304(r31)
  guest::StoreU32(base, schema::DeviceDisplayCapture(base, uint32_t(r31) - 20128).unknown_20432_address(), uint32_t(r30));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // mtctr r10
  ctr = r10;
loc_822B4738:
  // stw r9,0(r11)
  guest::StoreU32(base, schema::DeviceDisplayCapture(base, uint32_t(r11) - 20136).unknown_20136_address(), uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x822b4738
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_822B4738;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822B4E78: lifted from generated/ by lift.py.
void f822B4E78(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822B4E78 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r10,64(r3)
  r10 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_64_address());
  // lwz r9,68(r3)
  r9 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_68_address());
  // lwz r11,292(r3)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_292_address());
  // add r9,r10,r9
  r9 = r10 + r9;
  // rlwinm r8,r11,12,26,31
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 12) & 0x3F;
  // add r10,r9,r10
  r10 = r9 + r10;
  // rlwinm r7,r11,6,26,31
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0x3F;
  // stw r9,68(r3)
  guest::StoreU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_68_address(), uint32_t(r9));
  // addi r9,r8,-1
  r9 = uint64_t(int64_t(int64_t(r8) + -1));
  // cmplw cr6,r7,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r9), so);
  // bne cr6,0x822b4eac
  if (!cr6.eq) goto loc_822B4EAC;
  // lwz r9,76(r3)
  r9 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_76_address());
  // b 0x822b4eb0
  goto loc_822B4EB0;
loc_822B4EAC:
  // lwz r9,72(r3)
  r9 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_72_address());
loc_822B4EB0:
  // lwz r6,68(r3)
  r6 = guest::LoadU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_68_address());
  // cmplw cr6,r10,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r6), so);
  // blt cr6,0x822b4ec4
  if (cr6.lt) goto loc_822B4EC4;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blelr cr6
  if (!cr6.gt) return;
loc_822B4EC4:
  // addi r10,r7,1
  r10 = uint64_t(int64_t(int64_t(r7) + 1));
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // divwu r9,r10,r8
  r9 = uint32_t(uint32_t(r8) ? uint32_t(r10) / uint32_t(r8) : 0);
  // mullw r9,r9,r8
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r8))));
  // subf r10,r9,r10
  r10 = r10 - r9;
  // rlwimi r11,r10,26,0,5
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 26) & 0xFC000000) | (r11 & 0xFFFFFFFF03FFFFFF);
  // stw r11,292(r3)
  guest::StoreU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_292_address(), uint32_t(r11));
  // rlwinm. r10,r11,0,0,5
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFC000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x822b4f00
  if (!cr0.eq) goto loc_822B4F00;
  // li r11,2048
  r11 = uint64_t(int64_t(2048));
  // stw r11,68(r3)
  guest::StoreU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_68_address(), uint32_t(r11));
  // lbz r11,296(r3)
  r11 = guest::LoadU8(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_296_address());
  // ori r11,r11,16
  r11 = r11 | 16;
  // stb r11,296(r3)
  guest::StoreU8(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_296_address(), uint8_t(r11));
  // blr
  return;
loc_822B4F00:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,68(r3)
  guest::StoreU32(base, schema::DeviceDisplayCapture(base, uint32_t(r3)).unknown_68_address(), uint32_t(r11));
  // blr
  return;
}

// sub_822B4F10: lifted from generated/ by lift.py.
lifted::Result f822B4F10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822B4F10 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-2304(r1)
  ea = -2304 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r3
  r26 = r3;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r31,r26,20128
  r31 = uint64_t(int64_t(int64_t(r26) + 20128));
  // lbz r11,296(r31)
  r11 = guest::LoadU8(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20424_address());
  // rlwinm. r10,r11,0,28,28
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822b5184
  if (cr0.eq) goto loc_822B5184;
  // lwz r3,48(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20176_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822b515c
  if (cr0.eq) goto loc_822B515C;
  // rlwinm. r10,r11,0,25,25
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // beq 0x822b4fb8
  if (cr0.eq) goto loc_822B4FB8;
  // rlwinm. r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822b4f5c
  if (cr0.eq) goto loc_822B4F5C;
  // mr r4,r28
  r4 = r28;
  // bl 0x822970a0
  // (left out: dead in C++)
  { const lifted::Result o = game::WaitForSingleObject(base, r1, r13, r3, r4, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822B4F5C:
  // lwz r3,288(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20416_address());
  // bl 0x8229eaf0
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::BlockOnFence(base, r1, r13, r3, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, 0x822B4F64); r3 = o.r3; f1 = o.f1; }
  // lwz r11,292(r31)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20420_address());
  // addi r7,r31,32
  r7 = uint64_t(int64_t(int64_t(r31) + 32));
  // lwz r10,284(r31)
  r10 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20412_address());
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // rlwinm r11,r11,6,26,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0x3F;
  // lwz r9,68(r31)
  r9 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20196_address());
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r27,44(r31)
  guest::StoreU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20172_address(), uint32_t(r27));
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // lwz r5,64(r31)
  r5 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20192_address());
  // rlwinm r10,r10,2,29,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0x4;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stw r9,40(r31)
  guest::StoreU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20168_address(), uint32_t(r9));
  // lwzx r4,r10,r31
  r4 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r31));
  // lwzx r3,r11,r31
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r31));
  // bl 0x82295d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295D50, LIFTED_ARGS(0x822B4FA4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x822b4e78
  // (left out: dead in C++)
  f822B4E78(base, r3);
  // mr r4,r28
  r4 = r28;
  // lwz r3,48(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20176_address());
  // bl 0x822970a0
  // (left out: dead in C++)
  { const lifted::Result o = game::WaitForSingleObject(base, r1, r13, r3, r4, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822B4FB8:
  // lbz r11,296(r31)
  r11 = guest::LoadU8(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20424_address());
  // rlwinm. r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822b505c
  if (!cr0.eq) goto loc_822B505C;
  // lwz r11,292(r31)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20420_address());
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r27,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r27));
  // rlwinm r11,r11,6,26,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0x3F;
  // lwz r4,68(r31)
  r4 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20196_address());
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r31
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r31));
  // bl 0x825b9dd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B9DD8, LIFTED_ARGS(0x822B4FEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,292(r31)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20420_address());
  // rlwinm r11,r11,6,26,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0x3F;
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r31
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r31));
  // bl 0x825b9d30
  // (left out: dead in C++)
  { const lifted::Result o = game::SetEndOfFile(base, r1, r13, r3, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,292(r31)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20420_address());
  // rlwinm r10,r11,6,26,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0x3F;
  // rlwinm r11,r11,12,26,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 12) & 0x3F;
  // addi r30,r10,1
  r30 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bge cr6,0x822b505c
  if (!cr6.lt) goto loc_822B505C;
  // addi r11,r30,2
  r11 = uint64_t(int64_t(int64_t(r30) + 2));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r29,r11,r31
  r29 = r11 + r31;
loc_822B5028:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825b9dd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B9DD8, LIFTED_ARGS(0x822B503C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // bl 0x825b9d30
  // (left out: dead in C++)
  { const lifted::Result o = game::SetEndOfFile(base, r1, r13, r3, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,292(r31)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20420_address());
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // rlwinm r11,r11,12,26,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 12) & 0x3F;
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x822b5028
  if (cr6.lt) goto loc_822B5028;
loc_822B505C:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825dbaa4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__VdGetCurrentDisplayInformation, LIFTED_ARGS(0x822B5064, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,296(r31)
  r11 = guest::LoadU8(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20424_address());
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,21415
  r11 = uint64_t(int64_t(1403453440));
  // bne 0x822b507c
  if (!cr0.eq) goto loc_822B507C;
  // ori r11,r11,8884
  r11 = r11 | 8884;
  // b 0x822b5080
  goto loc_822B5080;
loc_822B507C:
  // ori r11,r11,8885
  r11 = r11 | 8885;
loc_822B5080:
  // lbz r10,101(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 101);
  // lfs f0,172(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 172);
  f0 = double(temp.f32);
  // stw r11,192(r1)
  guest::StoreU32(base, uint32_t(r1) + 192, uint32_t(r11));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lhz r9,168(r1)
  r9 = guest::LoadU16(base, uint32_t(r1) + 168);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lhz r10,52(r31)
  r10 = guest::LoadU16(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20180_address());
  // addi r4,r26,13608
  r4 = uint64_t(int64_t(int64_t(r26) + 13608));
  // stw r27,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r27));
  // addi r3,r1,288
  r3 = uint64_t(int64_t(int64_t(r1) + 288));
  // stw r11,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r9,216(r1)
  guest::StoreU32(base, uint32_t(r1) + 216, uint32_t(r9));
  // stw r10,208(r1)
  guest::StoreU32(base, uint32_t(r1) + 208, uint32_t(r10));
  // lhz r9,170(r1)
  r9 = guest::LoadU16(base, uint32_t(r1) + 170);
  // lhz r10,54(r31)
  r10 = guest::LoadU16(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20182_address());
  // stw r11,196(r1)
  guest::StoreU32(base, uint32_t(r1) + 196, uint32_t(r11));
  // stw r9,220(r1)
  guest::StoreU32(base, uint32_t(r1) + 220, uint32_t(r9));
  // stw r10,212(r1)
  guest::StoreU32(base, uint32_t(r1) + 212, uint32_t(r10));
  // bne cr6,0x822b50e4
  if (!cr6.eq) goto loc_822B50E4;
  // stw r11,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r11));
  // bl 0x822b0ab0
  // (left out: dead in C++)
  { const lifted::Result o = game::ComputeGammaRampPwl(base, LIFTED_ARGS(0x822B50E0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822b50e8
  goto loc_822B50E8;
loc_822B50E4:
  // bl 0x822b0910
  // (left out: dead in C++)
  { const lifted::Result o = game::ComputeGammaRamp(base, LIFTED_ARGS(0x822B50E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B50E8:
  // addi r3,r1,232
  r3 = uint64_t(int64_t(int64_t(r1) + 232));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // li r5,56
  r5 = uint64_t(int64_t(56));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822B50F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,296(r31)
  r11 = guest::LoadU8(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20424_address());
  // rlwinm. r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822b511c
  if (cr0.eq) goto loc_822B511C;
  // lwz r11,292(r31)
  r11 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20420_address());
  // lwz r10,68(r31)
  r10 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20196_address());
  // rlwinm r11,r11,6,26,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0x3F;
  // stw r10,200(r1)
  guest::StoreU32(base, uint32_t(r1) + 200, uint32_t(r10));
  // stw r11,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r11));
  // b 0x822b5128
  goto loc_822B5128;
loc_822B511C:
  // li r11,2048
  r11 = uint64_t(int64_t(2048));
  // stw r27,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r27));
  // stw r11,200(r1)
  guest::StoreU32(base, uint32_t(r1) + 200, uint32_t(r11));
loc_822B5128:
  // addi r7,r31,32
  r7 = uint64_t(int64_t(int64_t(r31) + 32));
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20136_address());
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r27,40(r31)
  guest::StoreU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20168_address(), uint32_t(r27));
  // li r5,2048
  r5 = uint64_t(int64_t(2048));
  // stw r27,44(r31)
  guest::StoreU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20172_address(), uint32_t(r27));
  // addi r4,r1,192
  r4 = uint64_t(int64_t(int64_t(r1) + 192));
  // bl 0x82295d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295D50, LIFTED_ARGS(0x822B5148, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // lwz r3,48(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20176_address());
  // bl 0x822970a0
  // (left out: dead in C++)
  { const lifted::Result o = game::WaitForSingleObject(base, r1, r13, r3, r4, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r3,48(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20128).unknown_20176_address());
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x822B515C, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B515C:
  // mr r30,r27
  r30 = r27;
  // addi r31,r31,8
  r31 = uint64_t(int64_t(int64_t(r31) + 8));
loc_822B5164:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::DeviceDisplayCaptureLoose(base, uint32_t(r31) - 20136).unknown_20136_address());
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x822b5184
  if (cr6.eq) goto loc_822B5184;
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x822B5174, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplwi cr6,r30,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 6, so);
  // blt cr6,0x822b5164
  if (cr6.lt) goto loc_822B5164;
loc_822B5184:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r11,1972(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 1972);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x822b51b4
  if (cr6.eq) goto loc_822B51B4;
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b51e0
  if (cr0.eq) goto loc_822B51E0;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b51e0
  if (cr0.eq) goto loc_822B51E0;
  // b 0x822b51cc
  goto loc_822B51CC;
loc_822B51B4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r11,2032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 2032);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b51e0
  if (cr0.eq) goto loc_822B51E0;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24);
loc_822B51CC:
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // li r3,27
  r3 = uint64_t(int64_t(27));
  // addi r4,r10,24712
  r4 = uint64_t(int64_t(int64_t(r10) + 24712));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B51E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B51E0:
  // mr r3,r26
  r3 = r26;
  // bl 0x822b4680
  // (left out: dead in C++)
  { const lifted::Result o = f822B4680(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // addi r3,r11,-24304
  r3 = uint64_t(int64_t(int64_t(r11) + -24304));
  // bl 0x825dbb74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDeleteSymbolicLink, LIFTED_ARGS(0x822B51F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // mr r11,r27
  r11 = r27;
  // stb r11,15900(r10)
  guest::StoreU8(base, uint32_t(r10) + 15900, uint8_t(r11));
  // addi r1,r1,2304
  r1 = uint64_t(int64_t(int64_t(r1) + 2304));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822B58C0: lifted from generated/ by lift.py.
lifted::Result f822B58C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822B58C0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x822b4f10
  // (left out: dead in C++)
  { const lifted::Result o = f822B4F10(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stb r11,15901(r10)
  guest::StoreU8(base, uint32_t(r10) + 15901, uint8_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822B60C8 game::Device::WorkerNextBlock: lifted from generated/ by lift.py.
lifted::Result f822B60C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822B60C8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r27,r4
  r27 = r4;
  // addi r29,r31,60
  r29 = uint64_t(int64_t(int64_t(r31) + 60));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db9c4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfAcquireSpinLock, LIFTED_ARGS(0x822B60E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r27)
  r30 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lis r11,-16384
  r11 = uint64_t(int64_t(-1073741824));
  // mr r26,r3
  r26 = r3;
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x822b6170
  if (!cr6.eq) goto loc_822B6170;
  // lwz r11,48(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 48);
  // addi r30,r31,64
  r30 = uint64_t(int64_t(int64_t(r31) + 64));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r3,r30
  r3 = r30;
  // stw r11,48(r31)
  guest::StoreU32(base, uint32_t(r31) + 48, uint32_t(r11));
  // bne 0x822b6138
  if (!cr0.eq) goto loc_822B6138;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,56(r31)
  guest::StoreU32(base, uint32_t(r31) + 56, uint32_t(r11));
  // bl 0x825dbbd4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeSetEvent, LIFTED_ARGS(0x822B6130, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,52(r31)
  guest::StoreU32(base, uint32_t(r31) + 52, uint32_t(r28));
  // b 0x822b613c
  goto loc_822B613C;
loc_822B6138:
  // bl 0x825dbbc4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeResetEvent, LIFTED_ARGS(0x822B613C, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B613C:
  // mr r4,r26
  r4 = r26;
  // stw r27,36(r31)
  guest::StoreU32(base, uint32_t(r31) + 36, uint32_t(r27));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db9b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfReleaseSpinLock, LIFTED_ARGS(0x822B614C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r28,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r28));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbbb4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeWaitForSingleObject, LIFTED_ARGS(0x822B6168, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x822b6180
  goto loc_822B6180;
loc_822B6170:
  // mr r4,r26
  r4 = r26;
  // mr r3,r29
  r3 = r29;
  // bl 0x825db9b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfReleaseSpinLock, LIFTED_ARGS(0x822B617C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
loc_822B6180:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822B6188 game::Device::MaskTiledDrawBlock: lifted from generated/ by lift.py.
uint64_t f822B6188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_822B6188 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // mr r3,r4
  r3 = r4;
  // mr r25,r5
  r25 = r5;
  // addi r27,r11,16
  r27 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r26,12(r11)
  r26 = guest::LoadU32(base, uint32_t(r11) + 12);
loc_822B61A8:
  // lwz r4,0(r3)
  r4 = guest::LoadU32(base, uint32_t(r3) + 0);
  // addi r10,r3,4
  r10 = uint64_t(int64_t(int64_t(r3) + 4));
  // cmplw cr6,r10,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r4), so);
  // bge cr6,0x822b6260
  if (!cr6.lt) goto loc_822B6260;
  // addi r28,r26,-1
  r28 = uint64_t(int64_t(int64_t(r26) + -1));
  // addi r11,r10,8
  r11 = uint64_t(int64_t(int64_t(r10) + 8));
loc_822B61C0:
  // lhz r9,2(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 2);
  // mr r7,r28
  r7 = r28;
  // lhz r8,-2(r11)
  r8 = guest::LoadU16(base, uint32_t(r11) + -2);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // lhz r31,-4(r11)
  r31 = guest::LoadU16(base, uint32_t(r11) + -4);
  // lhz r30,0(r11)
  r30 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // rlwinm r29,r9,3,0,28
  r29 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // lwz r5,0(r10)
  r5 = guest::LoadU32(base, uint32_t(r10) + 0);
  // rlwinm r9,r7,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 4) & 0xFFFFFFF0;
  // rotlwi r31,r31,3
  r31 = __builtin_rotateleft32(uint32_t(r31), 3);
  // add r9,r9,r27
  r9 = r9 + r27;
  // rlwinm r8,r8,3,0,28
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // rotlwi r30,r30,3
  r30 = __builtin_rotateleft32(uint32_t(r30), 3);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
loc_822B6200:
  // lwz r24,-4(r9)
  r24 = guest::LoadU32(base, uint32_t(r9) + -4);
  // rlwinm r6,r6,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // cmplw cr6,r24,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r8), so);
  // bge cr6,0x822b6238
  if (!cr6.lt) goto loc_822B6238;
  // lwz r24,4(r9)
  r24 = guest::LoadU32(base, uint32_t(r9) + 4);
  // cmplw cr6,r24,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r31), so);
  // ble cr6,0x822b6238
  if (!cr6.gt) goto loc_822B6238;
  // lwz r24,0(r9)
  r24 = guest::LoadU32(base, uint32_t(r9) + 0);
  // cmplw cr6,r24,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r29), so);
  // bge cr6,0x822b6238
  if (!cr6.lt) goto loc_822B6238;
  // lwz r24,8(r9)
  r24 = guest::LoadU32(base, uint32_t(r9) + 8);
  // cmplw cr6,r24,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r30), so);
  // ble cr6,0x822b6238
  if (!cr6.gt) goto loc_822B6238;
  // ori r6,r6,3
  r6 = r6 | 3;
loc_822B6238:
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // addi r9,r9,-16
  r9 = uint64_t(int64_t(int64_t(r9) + -16));
  // bne cr6,0x822b6200
  if (!cr6.eq) goto loc_822B6200;
  // oris r9,r6,32768
  r9 = r6 | 2147483648;
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // cmplw cr6,r10,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r4), so);
  // stw r9,8(r5)
  guest::StoreU32(base, uint32_t(r5) + 8, uint32_t(r9));
  // blt cr6,0x822b61c0
  if (cr6.lt) goto loc_822B61C0;
loc_822B6260:
  // lwz r31,0(r10)
  r31 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // beq cr6,0x822b6270
  if (cr6.eq) goto loc_822B6270;
  // bl 0x822ac470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AC470(base, LIFTED_ARGS(0x822B6270, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B6270:
  // lis r11,-16384
  r11 = uint64_t(int64_t(-1073741824));
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x822b61a8
  if (!cr6.eq) goto loc_822B61A8;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
}

// sub_822B6288 game::Device::MaskTiledDraws: lifted from generated/ by lift.py.
uint64_t f822B6288(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822B6288 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r4,0(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 0);
  // bl 0x822b6188
  // (left out: dead in C++)
  r3 = game::Device::MaskTiledDrawBlock(base, r1, r13, r3, r4, r5);
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
loc_822B62A8:
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r31,r11,4
  r31 = uint64_t(int64_t(int64_t(r11) + 4));
  // b 0x822b62c8
  goto loc_822B62C8;
loc_822B62B4:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwz r4,0(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r3,r29
  r3 = r29;
  // bl 0x822b6188
  // (left out: dead in C++)
  r3 = game::Device::MaskTiledDrawBlock(base, r1, r13, r3, r4, r5);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
loc_822B62C8:
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // blt cr6,0x822b62b4
  if (cr6.lt) goto loc_822B62B4;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lis r10,-16384
  r10 = uint64_t(int64_t(-1073741824));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x822b62a8
  if (!cr6.eq) goto loc_822B62A8;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_822B62E8: lifted from generated/ by lift.py.
lifted::Result f822B62E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822B62E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r30,r31,11244
  r30 = uint64_t(int64_t(int64_t(r31) + 11244));
  // li r29,6
  r29 = uint64_t(int64_t(6));
  // stw r11,10816(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).worker_ring_address(), uint32_t(r11));
loc_822B6308:
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r30) - 11244).worker0_thread_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822b6324
  if (cr6.eq) goto loc_822B6324;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbbd4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeSetEvent, LIFTED_ARGS(0x822B6324, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B6324:
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,56
  r30 = uint64_t(int64_t(int64_t(r30) + 56));
  // bne 0x822b6308
  if (!cr0.eq) goto loc_822B6308;
  // addi r31,r31,11260
  r31 = uint64_t(int64_t(int64_t(r31) + 11260));
  // li r30,6
  r30 = uint64_t(int64_t(6));
loc_822B6338:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Device(base, uint32_t(r31) - 11260).worker0_thread_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822b6358
  if (cr0.eq) goto loc_822B6358;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bl 0x825dbbb4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeWaitForSingleObject, LIFTED_ARGS(0x822B6358, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B6358:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,56
  r31 = uint64_t(int64_t(int64_t(r31) + 56));
  // bne 0x822b6338
  if (!cr0.eq) goto loc_822B6338;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822B6370 game::Device::WriteBinSelect: lifted from generated/ by lift.py.
lifted::Result f822B6370(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822B6370 (lifted)");
  uint64_t r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,40(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 40);
  // srawi. r9,r11,31
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x7FFFFFFF) != 0);
  r9 = uint64_t(int64_t(int32_t(r11) >> 31));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x822b6398
  if (!cr0.eq) goto loc_822B6398;
  // rlwinm. r11,r11,0,1,1
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // bne 0x822b639c
  if (!cr0.eq) goto loc_822B639C;
loc_822B6398:
  // li r11,2
  r11 = uint64_t(int64_t(2));
loc_822B639C:
  // lwz r10,44(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 44);
  // rlwinm r8,r10,1,0,30
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // slw r11,r11,r8
  r11 = uint8_t(r8) & 0x20 ? 0 : (uint32_t(r11) << (uint8_t(r8) & 0x3F));
  // bne cr6,0x822b63bc
  if (!cr6.eq) goto loc_822B63BC;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x822b63bc
  if (!cr6.eq) goto loc_822B63BC;
  // oris r11,r11,32768
  r11 = r11 | 2147483648;
loc_822B63BC:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r10,1968(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 1968);
  // lwz r31,0(r10)
  r31 = guest::LoadU32(base, uint32_t(r10) + 0);
  // lwz r10,368(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 368);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x822b648c
  if (cr6.eq) goto loc_822B648C;
  // lis r10,-16384
  r10 = uint64_t(int64_t(-1073741824));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // stw r11,368(r3)
  guest::StoreU32(base, uint32_t(r3) + 368, uint32_t(r11));
  // ori r10,r10,25088
  r10 = r10 | 25088;
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lwz r11,19980(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 19980);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822b6414
  if (cr6.eq) goto loc_822B6414;
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B6414, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B6414:
  // lwz r11,20124(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20124);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b6438
  if (cr0.eq) goto loc_822B6438;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B6438, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B6438:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // bl 0x8229dd70
  // (left out: dead in C++)
  { const lifted::Result o = f8229DD70(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,19980(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 19980);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822b6468
  if (cr6.eq) goto loc_822B6468;
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B6468, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B6468:
  // lwz r11,20124(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20124);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b648c
  if (cr0.eq) goto loc_822B648C;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B648C, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B648C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822B64A0 game::Device::WaitForWorkers: lifted from generated/ by lift.py.
lifted::Result f822B64A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822B64A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r28,6
  r28 = uint64_t(int64_t(6));
  // lwz r11,1968(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 1968);
  // lwz r29,0(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r31,r29,11224
  r31 = uint64_t(int64_t(int64_t(r29) + 11224));
loc_822B64C0:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822b6510
  if (cr6.eq) goto loc_822B6510;
  // lwz r30,-4(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + -4);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // rlwimi r30,r11,0,30,31
  r30 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x3) | (r30 & 0xFFFFFFFFFFFFFFFC);
  // bl 0x8229d6f0
  // (left out: dead in C++)
  f8229D6F0(base, r13, r3, r4, r5);
loc_822B64E8:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8229d8a0
  // (left out: dead in C++)
  { const lifted::Result o = f8229D8A0(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b6508
  if (cr0.eq) goto loc_822B6508;
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 16);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bne cr6,0x822b64e8
  if (!cr6.eq) goto loc_822B64E8;
loc_822B6508:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8229d720
  // (left out: dead in C++)
  { const lifted::Result o = f8229D720(base, r1, r13, r3, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_822B6510:
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,56
  r31 = uint64_t(int64_t(int64_t(r31) + 56));
  // bne 0x822b64c0
  if (!cr0.eq) goto loc_822B64C0;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822B6528 game::Device::WorkerMarkers: lifted from generated/ by lift.py.
lifted::Result f822B6528(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822B6528 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r31,r4
  r31 = r4;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r10,1968(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 1968);
  // lwz r28,0(r10)
  r28 = guest::LoadU32(base, uint32_t(r10) + 0);
  // rlwinm. r10,r11,0,0,0
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822b6770
  if (cr0.eq) goto loc_822B6770;
  // lis r29,-31488
  r29 = uint64_t(int64_t(-2063597568));
loc_822B6558:
  // rlwinm r10,r11,0,0,7
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFF000000;
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // bgt cr6,0x822b6670
  if (cr6.gt) goto loc_822B6670;
  // beq cr6,0x822b6634
  if (cr6.eq) goto loc_822B6634;
  // lis r9,-32768
  r9 = uint64_t(int64_t(-2147483648));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x822b6770
  if (cr6.eq) goto loc_822B6770;
  // lis r9,-32512
  r9 = uint64_t(int64_t(-2130706432));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x822b660c
  if (cr6.eq) goto loc_822B660C;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x822b675c
  if (cr6.eq) goto loc_822B675C;
  // lis r9,-32000
  r9 = uint64_t(int64_t(-2097152000));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x822b65cc
  if (cr6.eq) goto loc_822B65CC;
  // lis r9,-31744
  r9 = uint64_t(int64_t(-2080374784));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x822b66a0
  if (!cr6.eq) goto loc_822B66A0;
  // lwz r11,40(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 40);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // rlwinm r11,r11,0,2,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFBFFFFFFF;
  // stw r11,40(r31)
  guest::StoreU32(base, uint32_t(r31) + 40, uint32_t(r11));
  // rlwinm. r10,r11,0,0,0
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822b675c
  if (cr0.eq) goto loc_822B675C;
  // lwz r11,384(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 384);
  // lwz r30,380(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 380);
loc_822B65C4:
  // stw r11,84(r31)
  guest::StoreU32(base, uint32_t(r31) + 84, uint32_t(r11));
  // b 0x822b675c
  goto loc_822B675C;
loc_822B65CC:
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // mr r10,r30
  r10 = r30;
  // lwz r9,44(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 44);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // stw r11,376(r31)
  guest::StoreU32(base, uint32_t(r31) + 376, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 40);
  // stw r10,372(r31)
  guest::StoreU32(base, uint32_t(r31) + 372, uint32_t(r10));
  // oris r11,r11,16384
  r11 = r11 | 1073741824;
  // stw r11,40(r31)
  guest::StoreU32(base, uint32_t(r31) + 40, uint32_t(r11));
  // ble cr6,0x822b675c
  if (!cr6.gt) goto loc_822B675C;
  // lwz r10,120(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 120);
  // rlwinm. r10,r10,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822b675c
  if (cr0.eq) goto loc_822B675C;
  // oris r11,r11,32768
  r11 = r11 | 2147483648;
  // b 0x822b6758
  goto loc_822B6758;
loc_822B660C:
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 4);
  // clrlwi r11,r11,8
  r11 = uint32_t(r11) & 0xFFFFFF;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8229e3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::Kick(base, LIFTED_ARGS(0x822B662C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // b 0x822b675c
  goto loc_822B675C;
loc_822B6634:
  // mr r11,r30
  r11 = r30;
  // lwz r10,84(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // stw r11,380(r31)
  guest::StoreU32(base, uint32_t(r31) + 380, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 40);
  // stw r10,384(r31)
  guest::StoreU32(base, uint32_t(r31) + 384, uint32_t(r10));
  // rlwinm r10,r11,0,0,0
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwimi r11,r10,26,0,0
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 26) & 0x80000000) | (r11 & 0xFFFFFFFF7FFFFFFF);
  // stw r11,40(r31)
  guest::StoreU32(base, uint32_t(r31) + 40, uint32_t(r11));
  // rlwinm. r10,r11,0,0,0
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822b675c
  if (cr0.eq) goto loc_822B675C;
  // lwz r11,376(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 376);
  // lwz r30,372(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 372);
  // b 0x822b65c4
  goto loc_822B65C4;
loc_822B6670:
  // lis r9,-31232
  r9 = uint64_t(int64_t(-2046820352));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x822b673c
  if (cr6.eq) goto loc_822B673C;
  // lis r9,-30976
  r9 = uint64_t(int64_t(-2030043136));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x822b66d8
  if (cr6.eq) goto loc_822B66D8;
  // lis r9,-30720
  r9 = uint64_t(int64_t(-2013265920));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x822b66c8
  if (cr6.eq) goto loc_822B66C8;
  // lis r9,-30464
  r9 = uint64_t(int64_t(-1996488704));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x822b66b4
  if (cr6.eq) goto loc_822B66B4;
loc_822B66A0:
  // lis r10,-16384
  r10 = uint64_t(int64_t(-1073741824));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x822b6770
  if (cr6.eq) goto loc_822B6770;
  // addi r30,r11,4
  r30 = uint64_t(int64_t(int64_t(r11) + 4));
  // b 0x822b65c4
  goto loc_822B65C4;
loc_822B66B4:
  // mr r3,r31
  r3 = r31;
  // bl 0x822b64a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::WaitForWorkers(base, r1, r13, r3, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825dbba4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeUnlockL2, LIFTED_ARGS(0x822B66C4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822b66d0
  goto loc_822B66D0;
loc_822B66C8:
  // mr r3,r31
  r3 = r31;
  // bl 0x822b6070
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::LockL2(base, r1, r13, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822B66D0:
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // b 0x822b675c
  goto loc_822B675C;
loc_822B66D8:
  // lwz r11,44(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 44);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // lwz r9,40(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 40);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // lwz r10,124(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 124);
  // rlwinm r9,r9,0,3,1
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFDFFFFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // stw r11,44(r31)
  guest::StoreU32(base, uint32_t(r31) + 44, uint32_t(r11));
  // stw r9,40(r31)
  guest::StoreU32(base, uint32_t(r31) + 40, uint32_t(r9));
  // bge cr6,0x822b672c
  if (!cr6.lt) goto loc_822B672C;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x822b671c
  if (!cr6.eq) goto loc_822B671C;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // beq cr6,0x822b671c
  if (cr6.eq) goto loc_822B671C;
  // addi r9,r31,88
  r9 = uint64_t(int64_t(int64_t(r31) + 88));
  // addi r30,r31,92
  r30 = uint64_t(int64_t(int64_t(r31) + 92));
  // b 0x822b6724
  goto loc_822B6724;
loc_822B671C:
  // lwz r9,392(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 392);
  // lwz r30,388(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 388);
loc_822B6724:
  // stw r9,84(r31)
  guest::StoreU32(base, uint32_t(r31) + 84, uint32_t(r9));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_822B672C:
  // bne cr6,0x822b675c
  if (!cr6.eq) goto loc_822B675C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,44(r31)
  guest::StoreU32(base, uint32_t(r31) + 44, uint32_t(r11));
  // b 0x822b675c
  goto loc_822B675C;
loc_822B673C:
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // mr r10,r30
  r10 = r30;
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // stw r11,392(r31)
  guest::StoreU32(base, uint32_t(r31) + 392, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 40);
  // stw r10,388(r31)
  guest::StoreU32(base, uint32_t(r31) + 388, uint32_t(r10));
  // oris r11,r11,8192
  r11 = r11 | 536870912;
loc_822B6758:
  // stw r11,40(r31)
  guest::StoreU32(base, uint32_t(r31) + 40, uint32_t(r11));
loc_822B675C:
  // mr r3,r31
  r3 = r31;
  // bl 0x822b6370
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::WriteBinSelect(base, r1, r13, r3, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // rlwinm. r10,r11,0,0,0
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x822b6558
  if (!cr0.eq) goto loc_822B6558;
loc_822B6770:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822C3958: lifted from generated/ by lift.py.
void f822C3958(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822C3958 (lifted)");
  // stw r4,52(r3)
  guest::StoreU32(base, uint32_t(r3) + 52, uint32_t(r4));
  // blr
  return;
}

// sub_822F1218: lifted from generated/ by lift.py.
uint64_t f822F1218(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822F1218 (lifted)");
  uint64_t r3 = 0;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // blr
  return r3;
}

// sub_82304258: lifted from generated/ by lift.py.
uint64_t f82304258(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82304258 (lifted)");
  uint64_t r3 = 0;
  // mr r3,r4
  r3 = r4;
  // blr
  return r3;
}

// sub_82368D98 game::Blitter::CopyBlocks: lifted from generated/ by lift.py.
uint64_t f82368D98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82368D98 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r11,72(r10)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).box_bottom_address());
  // lwz r9,68(r10)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).box_right_address());
  // lwz r8,60(r10)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).box_left_address());
  // or r11,r11,r9
  r11 = r11 | r9;
  // lwz r7,64(r10)
  r7 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).box_top_address());
  // or r11,r11,r8
  r11 = r11 | r8;
  // or r11,r11,r7
  r11 = r11 | r7;
  // clrlwi. r11,r11,30
  r11 = uint32_t(r11) & 0x3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82368ddc
  if (cr0.eq) goto loc_82368DDC;
loc_82368DD0:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x82368f44
  goto loc_82368F44;
loc_82368DDC:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r9,72(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).box_bottom_address());
  // lwz r6,68(r11)
  r6 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).box_right_address());
  // lwz r5,64(r11)
  r5 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).box_top_address());
  // or r9,r9,r6
  r9 = r9 | r6;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).box_left_address());
  // or r9,r9,r5
  r9 = r9 | r5;
  // or r11,r9,r11
  r11 = r9 | r11;
  // clrlwi. r11,r11,30
  r11 = uint32_t(r11) & 0x3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82368dd0
  if (!cr0.eq) goto loc_82368DD0;
  // lwz r6,4(r31)
  r6 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r11,4(r6)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r6)).format_address());
  // rlwinm r11,r11,0,24,22
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFEFF;
  // addis r11,r11,-6688
  r11 = uint64_t(int64_t(int64_t(r11) + -438304768));
  // addic. r11,r11,-82
  ca = uint32_t(r11) > 81;
  r11 = uint64_t(int64_t(int64_t(r11) + -82));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82368e3c
  if (cr0.eq) goto loc_82368E3C;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x82368f4c
  if (cr6.eq) goto loc_82368F4C;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x82368f4c
  if (cr6.eq) goto loc_82368F4C;
  // cmplwi cr6,r11,40
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 40, so);
  // beq cr6,0x82368e3c
  if (cr6.eq) goto loc_82368E3C;
  // cmplwi cr6,r11,41
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 41, so);
  // bne cr6,0x82368dd0
  if (!cr6.eq) goto loc_82368DD0;
loc_82368E3C:
  // li r9,8
  r9 = uint64_t(int64_t(8));
loc_82368E40:
  // lwz r5,96(r10)
  r5 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // rlwinm r7,r7,30,2,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0x3FFFFFFF;
  // lwz r3,100(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).slice_pitch_address());
  // rlwinm r4,r8,30,2,31
  r4 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 30) & 0x3FFFFFFF;
  // lwz r30,76(r10)
  r30 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).box_front_address());
  // mullw r7,r7,r5
  r7 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r5))));
  // lwz r8,32(r10)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).data_address());
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r5,112(r6)
  r5 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r6)).depth_address());
  // mullw r10,r3,r30
  r10 = uint64_t(int64_t(int64_t(int32_t(r3)) * int64_t(int32_t(r30))));
  // lwz r3,104(r11)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // add r10,r7,r10
  r10 = r7 + r10;
  // mullw r7,r4,r9
  r7 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r9))));
  // lwz r4,60(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).box_left_address());
  // add r10,r10,r7
  r10 = r10 + r7;
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // lwz r7,100(r11)
  r7 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).slice_pitch_address());
  // add r30,r10,r8
  r30 = r10 + r8;
  // lwz r5,76(r11)
  r5 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).box_front_address());
  // lwz r10,64(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).box_top_address());
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // lwz r8,96(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // mullw r8,r10,r8
  r8 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r8))));
  // lwz r10,32(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // mullw r11,r7,r5
  r11 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r5))));
  // rlwinm r7,r4,30,2,31
  r7 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 30) & 0x3FFFFFFF;
  // add r11,r8,r11
  r11 = r8 + r11;
  // mullw r8,r7,r9
  r8 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r9))));
  // rlwinm r5,r3,30,2,31
  r5 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 30) & 0x3FFFFFFF;
  // add r11,r11,r8
  r11 = r11 + r8;
  // mullw r24,r5,r9
  r24 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r9))));
  // add r29,r11,r10
  r29 = r11 + r10;
  // ble cr6,0x82368f40
  if (!cr6.gt) goto loc_82368F40;
  // lwz r10,108(r6)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r6)).height_address());
loc_82368ECC:
  // mr r26,r29
  r26 = r29;
  // mr r27,r30
  r27 = r30;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82368f18
  if (cr6.eq) goto loc_82368F18;
loc_82368EE0:
  // mr r5,r24
  r5 = r24;
  // mr r4,r27
  r4 = r27;
  // mr r3,r26
  r3 = r26;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82368EF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // lwz r10,96(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // add r27,r10,r27
  r27 = r10 + r27;
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r11,96(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // cmplw cr6,r28,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r10), so);
  // add r26,r11,r26
  r26 = r11 + r26;
  // blt cr6,0x82368ee0
  if (cr6.lt) goto loc_82368EE0;
loc_82368F18:
  // lwz r9,0(r31)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // addi r25,r25,1
  r25 = uint64_t(int64_t(int64_t(r25) + 1));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r9,100(r9)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).slice_pitch_address());
  // add r30,r9,r30
  r30 = r9 + r30;
  // lwz r9,100(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).slice_pitch_address());
  // lwz r11,112(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // add r29,r9,r29
  r29 = r9 + r29;
  // cmplw cr6,r25,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), uint32_t(r11), so);
  // blt cr6,0x82368ecc
  if (cr6.lt) goto loc_82368ECC;
loc_82368F40:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82368F44:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
loc_82368F4C:
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // b 0x82368e40
  goto loc_82368E40;
}

// sub_82368F58: lifted from generated/ by lift.py.
uint64_t f82368F58(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_82368F58 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // mullw r11,r6,r7
  r11 = uint64_t(int64_t(int64_t(int32_t(r6)) * int64_t(int32_t(r7))));
  // add r24,r11,r4
  r24 = r11 + r4;
  // cmplw cr6,r4,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r24), so);
  // bge cr6,0x82369048
  if (!cr6.lt) goto loc_82369048;
  // rlwinm r11,r5,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r25,r7,1,0,30
  r25 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 1) & 0xFFFFFFFE;
  // add r26,r11,r4
  r26 = r11 + r4;
loc_82368F7C:
  // mr r30,r3
  r30 = r3;
  // cmplw cr6,r4,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r26), so);
  // bge cr6,0x82369034
  if (!cr6.lt) goto loc_82369034;
  // subf r10,r4,r26
  r10 = r26 - r4;
  // addi r11,r4,4
  r11 = uint64_t(int64_t(int64_t(r4) + 4));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // addi r27,r7,-4
  r27 = uint64_t(int64_t(int64_t(r7) + -4));
  // rlwinm r10,r10,29,3,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 29) & 0x1FFFFFFF;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
loc_82368FA0:
  // lwzx r9,r27,r11
  r9 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r11));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r6,-4(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + -4);
  // rlwinm r28,r9,30,2,25
  r28 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 30) & 0x3FFFFFC0;
  // lwzx r5,r11,r7
  r5 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r7));
  // rlwinm r29,r6,30,2,25
  r29 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 30) & 0x3FFFFFC0;
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 0);
  // clrlwi r6,r6,8
  r6 = uint32_t(r6) & 0xFFFFFF;
  // clrlwi r9,r9,8
  r9 = uint32_t(r9) & 0xFFFFFF;
  // rlwinm r6,r6,0,24,15
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 0) & 0xFFFFFFFFFFFF00FF;
  // rlwinm r9,r9,0,24,15
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFF00FF;
  // rlwinm r28,r28,0,18,9
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFFFFC03FFF;
  // rlwinm r29,r29,0,18,9
  r29 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0xFFFFFFFFFFC03FFF;
  // add r9,r6,r9
  r9 = r6 + r9;
  // add r29,r29,r28
  r29 = r29 + r28;
  // rlwinm r28,r5,30,2,25
  r28 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 30) & 0x3FFFFFC0;
  // clrlwi r6,r5,8
  r6 = uint32_t(r5) & 0xFFFFFF;
  // rlwinm r28,r28,0,18,9
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFFFFC03FFF;
  // rlwinm r6,r6,0,24,15
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 0) & 0xFFFFFFFFFFFF00FF;
  // add r5,r29,r28
  r5 = r29 + r28;
  // add r9,r9,r6
  r9 = r9 + r6;
  // rlwinm r29,r31,30,2,25
  r29 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 30) & 0x3FFFFFC0;
  // clrlwi r6,r31,8
  r6 = uint32_t(r31) & 0xFFFFFF;
  // rlwinm r29,r29,0,18,9
  r29 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0xFFFFFFFFFFC03FFF;
  // rlwinm r6,r6,0,24,15
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 0) & 0xFFFFFFFFFFFF00FF;
  // add r5,r5,r29
  r5 = r5 + r29;
  // add r9,r9,r6
  r9 = r9 + r6;
  // addis r6,r5,128
  r6 = uint64_t(int64_t(int64_t(r5) + 8388608));
  // addis r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 131072));
  // addi r6,r6,128
  r6 = uint64_t(int64_t(int64_t(r6) + 128));
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // rlwimi r6,r9,30,24,31
  r6 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 30) & 0xFF) | (r6 & 0xFFFFFFFFFFFFFF00);
  // rlwimi r6,r9,30,8,15
  r6 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 30) & 0xFF0000) | (r6 & 0xFFFFFFFFFF00FFFF);
  // stw r6,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r6));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x82368fa0
  if (!cr0.eq) goto loc_82368FA0;
loc_82369034:
  // add r4,r25,r4
  r4 = r25 + r4;
  // add r3,r3,r8
  r3 = r3 + r8;
  // add r26,r26,r25
  r26 = r26 + r25;
  // cmplw cr6,r4,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r24), so);
  // blt cr6,0x82368f7c
  if (cr6.lt) goto loc_82368F7C;
loc_82369048:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
}

// sub_82369050: lifted from generated/ by lift.py.
uint64_t f82369050(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_82369050 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // mullw r11,r6,r7
  r11 = uint64_t(int64_t(int64_t(int32_t(r6)) * int64_t(int32_t(r7))));
  // add r24,r11,r4
  r24 = r11 + r4;
  // cmplw cr6,r4,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r24), so);
  // bge cr6,0x8236912c
  if (!cr6.lt) goto loc_8236912C;
  // rlwinm r11,r5,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r25,r7,1,0,30
  r25 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 1) & 0xFFFFFFFE;
  // add r26,r11,r4
  r26 = r11 + r4;
loc_82369074:
  // mr r30,r3
  r30 = r3;
  // cmplw cr6,r4,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r26), so);
  // bge cr6,0x82369118
  if (!cr6.lt) goto loc_82369118;
  // subf r10,r4,r26
  r10 = r26 - r4;
  // addi r11,r4,4
  r11 = uint64_t(int64_t(int64_t(r4) + 4));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // addi r27,r7,-4
  r27 = uint64_t(int64_t(int64_t(r7) + -4));
  // rlwinm r10,r10,29,3,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 29) & 0x1FFFFFFF;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
loc_82369098:
  // lwzx r9,r27,r11
  r9 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r11));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r6,-4(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + -4);
  // clrlwi r28,r9,8
  r28 = uint32_t(r9) & 0xFFFFFF;
  // lwzx r5,r11,r7
  r5 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r7));
  // clrlwi r29,r6,8
  r29 = uint32_t(r6) & 0xFFFFFF;
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r28,r28,0,24,15
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFFFFFF00FF;
  // rlwinm r29,r29,0,24,15
  r29 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0xFFFFFFFFFFFF00FF;
  // rlwinm r6,r6,0,16,23
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 0) & 0xFF00;
  // add r29,r29,r28
  r29 = r29 + r28;
  // clrlwi r28,r5,8
  r28 = uint32_t(r5) & 0xFFFFFF;
  // rlwinm r9,r9,0,16,23
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFF00;
  // rlwinm r28,r28,0,24,15
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFFFFFF00FF;
  // add r9,r6,r9
  r9 = r6 + r9;
  // rlwinm r6,r5,0,16,23
  r6 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0xFF00;
  // add r5,r29,r28
  r5 = r29 + r28;
  // clrlwi r29,r31,8
  r29 = uint32_t(r31) & 0xFFFFFF;
  // add r9,r9,r6
  r9 = r9 + r6;
  // rlwinm r29,r29,0,24,15
  r29 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0xFFFFFFFFFFFF00FF;
  // rlwinm r6,r31,0,16,23
  r6 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 0) & 0xFF00;
  // add r5,r5,r29
  r5 = r5 + r29;
  // add r9,r9,r6
  r9 = r9 + r6;
  // addis r6,r5,2
  r6 = uint64_t(int64_t(int64_t(r5) + 131072));
  // addi r9,r9,512
  r9 = uint64_t(int64_t(int64_t(r9) + 512));
  // addi r6,r6,2
  r6 = uint64_t(int64_t(int64_t(r6) + 2));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // rlwimi r6,r9,0,14,21
  r6 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x3FC00) | (r6 & 0xFFFFFFFFFFFC03FF);
  // rlwinm r9,r6,30,8,31
  r9 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 30) & 0xFFFFFF;
  // stw r9,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r9));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x82369098
  if (!cr0.eq) goto loc_82369098;
loc_82369118:
  // add r4,r25,r4
  r4 = r25 + r4;
  // add r3,r3,r8
  r3 = r3 + r8;
  // add r26,r26,r25
  r26 = r26 + r25;
  // cmplw cr6,r4,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r24), so);
  // blt cr6,0x82369074
  if (cr6.lt) goto loc_82369074;
loc_8236912C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
}

// sub_82369138: lifted from generated/ by lift.py.
lifted::Result f82369138(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_82369138 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32201
  r10 = uint64_t(int64_t(-2110324736));
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // addi r10,r10,-28840
  r10 = uint64_t(int64_t(int64_t(r10) + -28840));
  // lis r9,-32201
  r9 = uint64_t(int64_t(-2110324736));
  // lis r31,-32156
  r31 = uint64_t(int64_t(-2107375616));
  // stw r10,-17012(r11)
  guest::StoreU32(base, uint32_t(r11) + -17012, uint32_t(r10));
  // addi r10,r9,-28592
  r10 = uint64_t(int64_t(int64_t(r9) + -28592));
  // lwz r11,-17012(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -17012);
  // stw r10,-17008(r31)
  guest::StoreU32(base, uint32_t(r31) + -17008, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82369174, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82369188: lifted from generated/ by lift.py.
lifted::Result f82369188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_82369188 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32201
  r10 = uint64_t(int64_t(-2110324736));
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // addi r10,r10,-28592
  r10 = uint64_t(int64_t(int64_t(r10) + -28592));
  // lis r9,-32201
  r9 = uint64_t(int64_t(-2110324736));
  // lis r31,-32156
  r31 = uint64_t(int64_t(-2107375616));
  // stw r10,-17008(r11)
  guest::StoreU32(base, uint32_t(r11) + -17008, uint32_t(r10));
  // addi r10,r9,-28840
  r10 = uint64_t(int64_t(int64_t(r9) + -28840));
  // lwz r11,-17008(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -17008);
  // stw r10,-17012(r31)
  guest::StoreU32(base, uint32_t(r31) + -17012, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x823691C4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_823691D8: lifted from generated/ by lift.py.
uint64_t f823691D8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_823691D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r27,32(r9)
  r27 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).data_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r9,96(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // mullw r10,r10,r9
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // add r26,r10,r4
  r26 = r10 + r4;
  // b 0x823692bc
  goto loc_823692BC;
loc_82369204:
  // lwz r8,104(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mr r9,r27
  r9 = r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // mr r11,r4
  r11 = r4;
  // rlwinm r8,r8,1,0,30
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r10,r4,r10
  r10 = r4 + r10;
  // add r28,r8,r4
  r28 = r8 + r4;
  // cmplw cr6,r4,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r28), so);
  // bge cr6,0x823692a0
  if (!cr6.lt) goto loc_823692A0;
loc_82369228:
  // lhz r7,0(r11)
  r7 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half0_address());
  // lhz r8,0(r10)
  r8 = guest::LoadU16(base, uint32_t(r10) + 0);
  // lhz r6,2(r11)
  r6 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half1_address());
  // rlwinm r30,r7,0,21,26
  r30 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0x7E0;
  // lhz r5,2(r10)
  r5 = guest::LoadU16(base, uint32_t(r10) + 2);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // andi. r31,r7,63519
  r31 = r7 & 63519;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // andi. r29,r8,63519
  r29 = r8 & 63519;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // rlwinm r8,r8,0,21,26
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x7E0;
  // add r7,r31,r29
  r7 = r31 + r29;
  // add r8,r30,r8
  r8 = r30 + r8;
  // andi. r31,r6,63519
  r31 = r6 & 63519;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // rlwinm r6,r6,0,21,26
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 0) & 0x7E0;
  // add r7,r7,r31
  r7 = r7 + r31;
  // add r8,r8,r6
  r8 = r8 + r6;
  // rlwinm r6,r5,0,21,26
  r6 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0x7E0;
  // add r8,r8,r6
  r8 = r8 + r6;
  // addi r8,r8,64
  r8 = uint64_t(int64_t(int64_t(r8) + 64));
  // rlwinm r8,r8,30,21,26
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 30) & 0x7E0;
  // andi. r31,r5,63519
  r31 = r5 & 63519;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // add r7,r7,r31
  r7 = r7 + r31;
  // addi r7,r7,4098
  r7 = uint64_t(int64_t(int64_t(r7) + 4098));
  // rlwinm r7,r7,30,16,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0xFFFF;
  // rlwinm r7,r7,0,27,20
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFFFFFFFF81F;
  // or r8,r7,r8
  r8 = r7 | r8;
  // sth r8,0(r9)
  guest::StoreU16(base, schema::ImageCodecData(base, uint32_t(r9)).half0_address(), uint16_t(r8));
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // blt cr6,0x82369228
  if (cr6.lt) goto loc_82369228;
loc_823692A0:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r10,96(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // add r27,r10,r27
  r27 = r10 + r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r4,r10,r4
  r4 = r10 + r4;
loc_823692BC:
  // cmplw cr6,r4,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r26), so);
  // blt cr6,0x82369204
  if (cr6.lt) goto loc_82369204;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_823692D0: lifted from generated/ by lift.py.
uint64_t f823692D0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_823692D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r27,32(r9)
  r27 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).data_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r9,96(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // mullw r10,r10,r9
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // add r26,r10,r4
  r26 = r10 + r4;
  // b 0x823693b4
  goto loc_823693B4;
loc_823692FC:
  // lwz r8,104(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mr r9,r27
  r9 = r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // mr r11,r4
  r11 = r4;
  // rlwinm r8,r8,1,0,30
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r10,r4,r10
  r10 = r4 + r10;
  // add r28,r8,r4
  r28 = r8 + r4;
  // cmplw cr6,r4,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r28), so);
  // bge cr6,0x82369398
  if (!cr6.lt) goto loc_82369398;
loc_82369320:
  // lhz r7,0(r11)
  r7 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half0_address());
  // lhz r8,0(r10)
  r8 = guest::LoadU16(base, uint32_t(r10) + 0);
  // lhz r6,2(r11)
  r6 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half1_address());
  // rlwinm r31,r7,0,22,26
  r31 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0x3E0;
  // rlwinm r29,r8,0,22,26
  r29 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x3E0;
  // lhz r5,2(r10)
  r5 = guest::LoadU16(base, uint32_t(r10) + 2);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // andi. r30,r7,31775
  r30 = r7 & 31775;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // add r7,r31,r29
  r7 = r31 + r29;
  // rlwinm r31,r6,0,22,26
  r31 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 0) & 0x3E0;
  // andi. r8,r8,31775
  r8 = r8 & 31775;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // andi. r6,r6,31775
  r6 = r6 & 31775;
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // add r8,r30,r8
  r8 = r30 + r8;
  // add r7,r7,r31
  r7 = r7 + r31;
  // add r8,r8,r6
  r8 = r8 + r6;
  // rlwinm r31,r5,0,22,26
  r31 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0x3E0;
  // add r7,r7,r31
  r7 = r7 + r31;
  // addi r7,r7,64
  r7 = uint64_t(int64_t(int64_t(r7) + 64));
  // rlwinm r7,r7,30,22,26
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0x3E0;
  // andi. r6,r5,31775
  r6 = r5 & 31775;
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // add r8,r8,r6
  r8 = r8 + r6;
  // addi r8,r8,2050
  r8 = uint64_t(int64_t(int64_t(r8) + 2050));
  // rlwinm r8,r8,30,17,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 30) & 0x7FFF;
  // rlwinm r8,r8,0,27,21
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFFFFFFFFFC1F;
  // or r8,r7,r8
  r8 = r7 | r8;
  // sth r8,0(r9)
  guest::StoreU16(base, schema::ImageCodecData(base, uint32_t(r9)).half0_address(), uint16_t(r8));
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // blt cr6,0x82369320
  if (cr6.lt) goto loc_82369320;
loc_82369398:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r10,96(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // add r27,r10,r27
  r27 = r10 + r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r4,r10,r4
  r4 = r10 + r4;
loc_823693B4:
  // cmplw cr6,r4,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r26), so);
  // blt cr6,0x823692fc
  if (cr6.lt) goto loc_823692FC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_823693C8: lifted from generated/ by lift.py.
uint64_t f823693C8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_823693C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r28,32(r9)
  r28 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).data_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r9,96(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // mullw r10,r10,r9
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // add r27,r10,r4
  r27 = r10 + r4;
  // b 0x823694b4
  goto loc_823694B4;
loc_823693F4:
  // lwz r8,104(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mr r9,r28
  r9 = r28;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // mr r11,r4
  r11 = r4;
  // rlwinm r8,r8,1,0,30
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r10,r4,r10
  r10 = r4 + r10;
  // add r29,r8,r4
  r29 = r8 + r4;
  // cmplw cr6,r4,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r29), so);
  // bge cr6,0x82369498
  if (!cr6.lt) goto loc_82369498;
loc_82369418:
  // lhz r8,0(r10)
  r8 = guest::LoadU16(base, uint32_t(r10) + 0);
  // lhz r7,0(r11)
  r7 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half0_address());
  // lhz r6,2(r11)
  r6 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half1_address());
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // lhz r5,2(r10)
  r5 = guest::LoadU16(base, uint32_t(r10) + 2);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // andi. r30,r8,33760
  r30 = r8 & 33760;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // andi. r31,r7,33760
  r31 = r7 & 33760;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // andi. r7,r7,31775
  r7 = r7 & 31775;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // add r31,r31,r30
  r31 = r31 + r30;
  // andi. r8,r8,31775
  r8 = r8 & 31775;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // add r8,r7,r8
  r8 = r7 + r8;
  // andi. r30,r6,33760
  r30 = r6 & 33760;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // andi. r7,r6,31775
  r7 = r6 & 31775;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // add r6,r31,r30
  r6 = r31 + r30;
  // andi. r31,r5,33760
  r31 = r5 & 33760;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // add r8,r8,r7
  r8 = r8 + r7;
  // add r6,r6,r31
  r6 = r6 + r31;
  // andi. r7,r5,31775
  r7 = r5 & 31775;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // add r8,r8,r7
  r8 = r8 + r7;
  // addis r7,r6,1
  r7 = uint64_t(int64_t(int64_t(r6) + 65536));
  // addi r8,r8,2050
  r8 = uint64_t(int64_t(int64_t(r8) + 2050));
  // addi r7,r7,64
  r7 = uint64_t(int64_t(int64_t(r7) + 64));
  // rlwinm r8,r8,30,17,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 30) & 0x7FFF;
  // rlwinm r7,r7,30,16,26
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0xFFE0;
  // rlwinm r8,r8,0,27,21
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFFFFFFFFFC1F;
  // rlwinm r7,r7,0,22,16
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFFFFFFF83FF;
  // or r8,r7,r8
  r8 = r7 | r8;
  // sth r8,0(r9)
  guest::StoreU16(base, schema::ImageCodecData(base, uint32_t(r9)).half0_address(), uint16_t(r8));
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // blt cr6,0x82369418
  if (cr6.lt) goto loc_82369418;
loc_82369498:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r10,96(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // add r28,r10,r28
  r28 = r10 + r28;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r4,r10,r4
  r4 = r10 + r4;
loc_823694B4:
  // cmplw cr6,r4,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r27), so);
  // blt cr6,0x823693f4
  if (cr6.lt) goto loc_823693F4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_823694C8: lifted from generated/ by lift.py.
uint64_t f823694C8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_823694C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r27,32(r9)
  r27 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).data_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r9,96(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // mullw r10,r10,r9
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // add r26,r10,r4
  r26 = r10 + r4;
  // b 0x823695b0
  goto loc_823695B0;
loc_823694F4:
  // lwz r8,104(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mr r9,r27
  r9 = r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // mr r11,r4
  r11 = r4;
  // rlwinm r8,r8,1,0,30
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r10,r4,r10
  r10 = r4 + r10;
  // add r28,r8,r4
  r28 = r8 + r4;
  // cmplw cr6,r4,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r28), so);
  // bge cr6,0x82369594
  if (!cr6.lt) goto loc_82369594;
loc_82369518:
  // lhz r7,0(r11)
  r7 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half0_address());
  // lhz r8,0(r10)
  r8 = guest::LoadU16(base, uint32_t(r10) + 0);
  // lhz r6,2(r11)
  r6 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half1_address());
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // lhz r5,2(r10)
  r5 = guest::LoadU16(base, uint32_t(r10) + 2);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // andi. r31,r7,61680
  r31 = r7 & 61680;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // andi. r29,r8,61680
  r29 = r8 & 61680;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // andi. r30,r7,3855
  r30 = r7 & 3855;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // add r7,r31,r29
  r7 = r31 + r29;
  // andi. r8,r8,3855
  r8 = r8 & 3855;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // add r8,r30,r8
  r8 = r30 + r8;
  // andi. r31,r6,61680
  r31 = r6 & 61680;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // andi. r6,r6,3855
  r6 = r6 & 3855;
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // add r7,r7,r31
  r7 = r7 + r31;
  // add r8,r8,r6
  r8 = r8 + r6;
  // andi. r31,r5,61680
  r31 = r5 & 61680;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // add r7,r7,r31
  r7 = r7 + r31;
  // addi r7,r7,8224
  r7 = uint64_t(int64_t(int64_t(r7) + 8224));
  // rlwinm r7,r7,30,16,27
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0xFFF0;
  // rlwinm r7,r7,0,24,19
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFFFFFFFF0FF;
  // andi. r6,r5,3855
  r6 = r5 & 3855;
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // add r8,r8,r6
  r8 = r8 + r6;
  // addi r8,r8,514
  r8 = uint64_t(int64_t(int64_t(r8) + 514));
  // rlwinm r8,r8,30,20,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 30) & 0xFFF;
  // rlwinm r8,r8,0,28,23
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFFFFFFFFFF0F;
  // or r8,r7,r8
  r8 = r7 | r8;
  // sth r8,0(r9)
  guest::StoreU16(base, schema::ImageCodecData(base, uint32_t(r9)).half0_address(), uint16_t(r8));
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // blt cr6,0x82369518
  if (cr6.lt) goto loc_82369518;
loc_82369594:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r10,96(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // add r27,r10,r27
  r27 = r10 + r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r4,r10,r4
  r4 = r10 + r4;
loc_823695B0:
  // cmplw cr6,r4,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r26), so);
  // blt cr6,0x823694f4
  if (cr6.lt) goto loc_823694F4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_823695C0: lifted from generated/ by lift.py.
uint64_t f823695C0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_823695C0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r8,4(r3)
  r8 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r30,32(r8)
  r30 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r8)).data_address());
  // lwz r9,108(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r8,96(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // lwz r10,32(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // mullw r9,r9,r8
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r8))));
  // add r29,r9,r10
  r29 = r9 + r10;
  // b 0x82369664
  goto loc_82369664;
loc_823695EC:
  // lwz r7,104(r11)
  r7 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mr r8,r30
  r8 = r30;
  // lwz r9,96(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // mr r11,r10
  r11 = r10;
  // add r7,r7,r10
  r7 = r7 + r10;
  // add r9,r10,r9
  r9 = r10 + r9;
  // cmplw cr6,r10,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r7), so);
  // bge cr6,0x82369648
  if (!cr6.lt) goto loc_82369648;
loc_8236960C:
  // lbz r31,1(r11)
  r31 = guest::LoadU8(base, schema::ImageCodecData(base, uint32_t(r11)).byte1_address());
  // lbz r6,1(r9)
  r6 = guest::LoadU8(base, uint32_t(r9) + 1);
  // lbz r4,0(r9)
  r4 = guest::LoadU8(base, uint32_t(r9) + 0);
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // add r6,r6,r31
  r6 = r6 + r31;
  // lbz r5,0(r11)
  r5 = guest::LoadU8(base, schema::ImageCodecData(base, uint32_t(r11)).byte0_address());
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // add r6,r6,r4
  r6 = r6 + r4;
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // add r6,r6,r5
  r6 = r6 + r5;
  // addi r6,r6,2
  r6 = uint64_t(int64_t(int64_t(r6) + 2));
  // rlwinm r6,r6,30,2,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 30) & 0x3FFFFFFF;
  // stb r6,0(r8)
  guest::StoreU8(base, schema::ImageCodecData(base, uint32_t(r8)).byte0_address(), uint8_t(r6));
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // blt cr6,0x8236960c
  if (cr6.lt) goto loc_8236960C;
loc_82369648:
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r9,96(r9)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).row_pitch_address());
  // add r30,r9,r30
  r30 = r9 + r30;
  // lwz r9,96(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // rlwinm r9,r9,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // add r10,r9,r10
  r10 = r9 + r10;
loc_82369664:
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // blt cr6,0x823695ec
  if (cr6.lt) goto loc_823695EC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82369678: lifted from generated/ by lift.py.
uint64_t f82369678(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82369678 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r27,32(r9)
  r27 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).data_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r9,96(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // mullw r10,r10,r9
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // add r26,r10,r4
  r26 = r10 + r4;
  // b 0x8236975c
  goto loc_8236975C;
loc_823696A4:
  // lwz r8,104(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mr r9,r27
  r9 = r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // mr r11,r4
  r11 = r4;
  // rlwinm r8,r8,1,0,30
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r10,r4,r10
  r10 = r4 + r10;
  // add r28,r8,r4
  r28 = r8 + r4;
  // cmplw cr6,r4,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r28), so);
  // bge cr6,0x82369740
  if (!cr6.lt) goto loc_82369740;
loc_823696C8:
  // lhz r7,0(r11)
  r7 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half0_address());
  // lhz r8,0(r10)
  r8 = guest::LoadU16(base, uint32_t(r10) + 0);
  // lhz r6,2(r11)
  r6 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half1_address());
  // rlwinm r31,r7,0,24,27
  r31 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xF0;
  // rlwinm r29,r8,0,24,27
  r29 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xF0;
  // lhz r5,2(r10)
  r5 = guest::LoadU16(base, uint32_t(r10) + 2);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // andi. r30,r7,3855
  r30 = r7 & 3855;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // add r7,r31,r29
  r7 = r31 + r29;
  // rlwinm r31,r6,0,24,27
  r31 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 0) & 0xF0;
  // andi. r8,r8,3855
  r8 = r8 & 3855;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // andi. r6,r6,3855
  r6 = r6 & 3855;
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // add r8,r30,r8
  r8 = r30 + r8;
  // add r7,r7,r31
  r7 = r7 + r31;
  // add r8,r8,r6
  r8 = r8 + r6;
  // rlwinm r31,r5,0,24,27
  r31 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0xF0;
  // add r7,r7,r31
  r7 = r7 + r31;
  // addi r7,r7,32
  r7 = uint64_t(int64_t(int64_t(r7) + 32));
  // rlwinm r7,r7,30,24,27
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0xF0;
  // andi. r6,r5,3855
  r6 = r5 & 3855;
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // add r8,r8,r6
  r8 = r8 + r6;
  // addi r8,r8,514
  r8 = uint64_t(int64_t(int64_t(r8) + 514));
  // rlwinm r8,r8,30,20,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 30) & 0xFFF;
  // rlwinm r8,r8,0,28,23
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFFFFFFFFFF0F;
  // or r8,r7,r8
  r8 = r7 | r8;
  // sth r8,0(r9)
  guest::StoreU16(base, schema::ImageCodecData(base, uint32_t(r9)).half0_address(), uint16_t(r8));
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // blt cr6,0x823696c8
  if (cr6.lt) goto loc_823696C8;
loc_82369740:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r10,96(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // add r27,r10,r27
  r27 = r10 + r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r4,r10,r4
  r4 = r10 + r4;
loc_8236975C:
  // cmplw cr6,r4,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r26), so);
  // blt cr6,0x823696a4
  if (cr6.lt) goto loc_823696A4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82369770: lifted from generated/ by lift.py.
uint64_t f82369770(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82369770 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r27,32(r9)
  r27 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).data_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r9,96(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // mullw r10,r10,r9
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // add r26,r10,r4
  r26 = r10 + r4;
  // b 0x82369850
  goto loc_82369850;
loc_8236979C:
  // lwz r8,104(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mr r9,r27
  r9 = r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // mr r11,r4
  r11 = r4;
  // rlwinm r8,r8,1,0,30
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r10,r4,r10
  r10 = r4 + r10;
  // add r28,r8,r4
  r28 = r8 + r4;
  // cmplw cr6,r4,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r28), so);
  // bge cr6,0x82369834
  if (!cr6.lt) goto loc_82369834;
loc_823697C0:
  // lhz r7,0(r11)
  r7 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half0_address());
  // lhz r8,0(r10)
  r8 = guest::LoadU16(base, uint32_t(r10) + 0);
  // clrlwi r31,r7,24
  r31 = uint32_t(r7) & 0xFF;
  // lhz r6,2(r11)
  r6 = guest::LoadU16(base, schema::ImageCodecData(base, uint32_t(r11)).half1_address());
  // clrlwi r29,r8,24
  r29 = uint32_t(r8) & 0xFF;
  // lhz r5,2(r10)
  r5 = guest::LoadU16(base, uint32_t(r10) + 2);
  // rlwinm r30,r7,0,0,23
  r30 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFF00;
  // rlwinm r8,r8,0,0,23
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFF00;
  // add r7,r31,r29
  r7 = r31 + r29;
  // clrlwi r31,r6,24
  r31 = uint32_t(r6) & 0xFF;
  // add r8,r30,r8
  r8 = r30 + r8;
  // rlwinm r6,r6,0,0,23
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 0) & 0xFFFFFF00;
  // add r7,r7,r31
  r7 = r7 + r31;
  // add r8,r8,r6
  r8 = r8 + r6;
  // clrlwi r31,r5,24
  r31 = uint32_t(r5) & 0xFF;
  // rlwinm r6,r5,0,0,23
  r6 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0xFFFFFF00;
  // add r7,r7,r31
  r7 = r7 + r31;
  // add r8,r8,r6
  r8 = r8 + r6;
  // addi r7,r7,2
  r7 = uint64_t(int64_t(int64_t(r7) + 2));
  // addi r8,r8,512
  r8 = uint64_t(int64_t(int64_t(r8) + 512));
  // rlwinm r7,r7,30,24,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0xFF;
  // rlwinm r8,r8,30,16,23
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 30) & 0xFF00;
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // or r8,r7,r8
  r8 = r7 | r8;
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // sth r8,0(r9)
  guest::StoreU16(base, schema::ImageCodecData(base, uint32_t(r9)).half0_address(), uint16_t(r8));
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // blt cr6,0x823697c0
  if (cr6.lt) goto loc_823697C0;
loc_82369834:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).destination_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r3)).source_address());
  // lwz r10,96(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // add r27,r10,r27
  r27 = r10 + r27;
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r4,r10,r4
  r4 = r10 + r4;
loc_82369850:
  // cmplw cr6,r4,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r26), so);
  // blt cr6,0x8236979c
  if (cr6.lt) goto loc_8236979C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82369B48 game::Blitter::FilterNone: lifted from generated/ by lift.py.
lifted::Result f82369B48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82369B48 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lbz r11,11(r30)
  r11 = guest::LoadU8(base, schema::Blitter(base, uint32_t(r30)).filter_type_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x82369b70
  if (cr6.eq) goto loc_82369B70;
loc_82369B64:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x82369dac
  goto loc_82369DAC;
loc_82369B70:
  // lwz r9,0(r30)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).source_address());
  // lwz r8,4(r30)
  r8 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // lwz r11,104(r9)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).width_address());
  // lwz r10,104(r8)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r8)).width_address());
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r27,r10
  r27 = r10;
  // bgt cr6,0x82369b90
  if (cr6.gt) goto loc_82369B90;
  // mr r27,r11
  r27 = r11;
loc_82369B90:
  // lwz r11,108(r8)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r8)).height_address());
  // lwz r10,108(r9)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).height_address());
  // mr r25,r11
  r25 = r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x82369ba8
  if (cr6.lt) goto loc_82369BA8;
  // mr r25,r10
  r25 = r10;
loc_82369BA8:
  // lwz r11,112(r8)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r8)).depth_address());
  // lwz r10,112(r9)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).depth_address());
  // mr r24,r11
  r24 = r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x82369bc0
  if (cr6.lt) goto loc_82369BC0;
  // mr r24,r10
  r24 = r10;
loc_82369BC0:
  // lis r11,4095
  r11 = uint64_t(int64_t(268369920));
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // ori r31,r11,65535
  r31 = r11 | 65535;
  // rlwinm r3,r27,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 4) & 0xFFFFFFF0;
  // cmplw cr6,r27,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r31), so);
  // ble cr6,0x82369bdc
  if (!cr6.gt) goto loc_82369BDC;
  // mr r3,r29
  r3 = r29;
loc_82369BDC:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82369BE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x82369b64
  if (cr6.eq) goto loc_82369B64;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x82369c04
  if (!cr6.gt) goto loc_82369C04;
  // mr r3,r29
  r3 = r29;
loc_82369C04:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82369C08, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // mr r3,r26
  r3 = r26;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x82369c20
  if (!cr6.eq) goto loc_82369C20;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82369C1C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82369b64
  goto loc_82369B64;
loc_82369C20:
  // rlwinm r5,r27,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 4) & 0xFFFFFFF0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82369C2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // rlwinm r5,r11,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82369C44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).srgb_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82369c74
  if (cr6.eq) goto loc_82369C74;
  // lwz r10,0(r30)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).source_address());
  // lwz r10,16(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).srgb_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82369c74
  if (cr6.eq) goto loc_82369C74;
  // stw r27,16(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).srgb_address(), uint32_t(r27));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).source_address());
  // stw r27,16(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).srgb_address(), uint32_t(r27));
loc_82369C74:
  // mr r29,r27
  r29 = r27;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x82369d2c
  if (cr6.eq) goto loc_82369D2C;
loc_82369C80:
  // mr r31,r27
  r31 = r27;
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x82369cd8
  if (cr6.eq) goto loc_82369CD8;
loc_82369C8C:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).source_address());
  // mr r6,r26
  r6 = r26;
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82369CAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // mr r6,r26
  r6 = r26;
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82369CCC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmplw cr6,r31,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r25), so);
  // blt cr6,0x82369c8c
  if (cr6.lt) goto loc_82369C8C;
loc_82369CD8:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // mr r31,r25
  r31 = r25;
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r25,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), uint32_t(r11), so);
  // bge cr6,0x82369d20
  if (!cr6.lt) goto loc_82369D20;
loc_82369CEC:
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82369D0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x82369cec
  if (cr6.lt) goto loc_82369CEC;
loc_82369D20:
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplw cr6,r29,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r24), so);
  // blt cr6,0x82369c80
  if (cr6.lt) goto loc_82369C80;
loc_82369D2C:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // mr r29,r24
  r29 = r24;
  // lwz r10,112(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // cmplw cr6,r24,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r10), so);
  // bge cr6,0x82369d98
  if (!cr6.lt) goto loc_82369D98;
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
loc_82369D44:
  // mr r31,r27
  r31 = r27;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82369d84
  if (cr6.eq) goto loc_82369D84;
loc_82369D50:
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82369D70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x82369d50
  if (cr6.lt) goto loc_82369D50;
loc_82369D84:
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r30)).destination_address());
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // lwz r10,112(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // cmplw cr6,r29,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r10), so);
  // blt cr6,0x82369d44
  if (cr6.lt) goto loc_82369D44;
loc_82369D98:
  // mr r3,r26
  r3 = r26;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82369DA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82369DA8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82369DAC:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82369DB8 game::Blitter::FilterPoint: lifted from generated/ by lift.py.
lifted::Result f82369DB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82369DB8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lbz r11,11(r31)
  r11 = guest::LoadU8(base, schema::Blitter(base, uint32_t(r31)).filter_type_address());
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x82369de0
  if (cr6.eq) goto loc_82369DE0;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x82369ff0
  goto loc_82369FF0;
loc_82369DE0:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lis r10,4095
  r10 = uint64_t(int64_t(268369920));
  // li r17,-1
  r17 = uint64_t(int64_t(-1));
  // ori r30,r10,65535
  r30 = r10 | 65535;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x82369e04
  if (!cr6.gt) goto loc_82369E04;
  // mr r3,r17
  r3 = r17;
loc_82369E04:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82369E08, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r19,r3
  r19 = r3;
  // cmplwi cr6,r19,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r19), 0, so);
  // bne cr6,0x82369e20
  if (!cr6.eq) goto loc_82369E20;
loc_82369E14:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x82369ff0
  goto loc_82369FF0;
loc_82369E20:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x82369e38
  if (!cr6.gt) goto loc_82369E38;
  // mr r3,r17
  r3 = r17;
loc_82369E38:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82369E3C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r18,r3
  r18 = r3;
  // cmplwi cr6,r18,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), 0, so);
  // bne cr6,0x82369e54
  if (!cr6.eq) goto loc_82369E54;
  // mr r3,r19
  r3 = r19;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82369E50, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82369e14
  goto loc_82369E14;
loc_82369E54:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // li r20,0
  r20 = uint64_t(int64_t(0));
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).srgb_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82369e84
  if (cr6.eq) goto loc_82369E84;
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r10,16(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).srgb_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82369e84
  if (cr6.eq) goto loc_82369E84;
  // stw r20,16(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).srgb_address(), uint32_t(r20));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // stw r20,16(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).srgb_address(), uint32_t(r20));
loc_82369E84:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r24,r20
  r24 = r20;
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // mr r26,r20
  r26 = r20;
  // mr r9,r10
  r9 = r10;
  // lwz r8,104(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // lwz r7,108(r11)
  r7 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r6,104(r10)
  r6 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // rlwinm r8,r8,16,0,15
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 16) & 0xFFFF0000;
  // lwz r11,112(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // rlwinm r7,r7,16,0,15
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 16) & 0xFFFF0000;
  // lwz r5,108(r10)
  r5 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // divwu r23,r8,r6
  r23 = uint32_t(uint32_t(r6) ? uint32_t(r8) / uint32_t(r6) : 0);
  // lwz r10,112(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // rlwinm r11,r11,16,0,15
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF0000;
  // lwz r8,112(r9)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).depth_address());
  // twllei r6,0
  if (int32_t(r6) == 0 || uint32_t(r6) < 0u) lifted::Trap(0);
  // divwu r22,r7,r5
  r22 = uint32_t(uint32_t(r5) ? uint32_t(r7) / uint32_t(r5) : 0);
  // twllei r5,0
  if (int32_t(r5) == 0 || uint32_t(r5) < 0u) lifted::Trap(0);
  // divwu r21,r11,r10
  r21 = uint32_t(uint32_t(r10) ? uint32_t(r11) / uint32_t(r10) : 0);
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // ble cr6,0x82369fdc
  if (!cr6.gt) goto loc_82369FDC;
loc_82369EE0:
  // lwz r11,108(r9)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).height_address());
  // mr r30,r20
  r30 = r20;
  // mr r27,r20
  r27 = r20;
  // mr r25,r17
  r25 = r17;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x82369fc4
  if (!cr6.gt) goto loc_82369FC4;
loc_82369EF8:
  // xor r11,r25,r30
  r11 = r25 ^ r30;
  // mr r28,r20
  r28 = r20;
  // mr r29,r20
  r29 = r20;
  // rlwinm. r11,r11,0,0,15
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFF0000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82369f30
  if (cr0.eq) goto loc_82369F30;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r6,r19
  r6 = r19;
  // rlwinm r5,r24,16,16,31
  r5 = __builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 16) & 0xFFFF;
  // rlwinm r4,r30,16,16,31
  r4 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 16) & 0xFFFF;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82369F2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r30
  r25 = r30;
loc_82369F30:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x82369f8c
  if (!cr6.gt) goto loc_82369F8C;
  // mr r9,r18
  r9 = r18;
loc_82369F44:
  // rlwinm r10,r28,20,12,27
  r10 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 20) & 0xFFFF0;
  // mr r11,r9
  r11 = r9;
  // add r10,r10,r19
  r10 = r10 + r19;
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // add r28,r28,r23
  r28 = r28 + r23;
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r8,0(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11)).red_address(), uint32_t(r8));
  // lwz r8,4(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 4);
  // stw r8,4(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11)).green_address(), uint32_t(r8));
  // lwz r8,8(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 8);
  // stw r8,8(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11)).blue_address(), uint32_t(r8));
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // stw r10,12(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11)).alpha_address(), uint32_t(r10));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x82369f44
  if (cr6.lt) goto loc_82369F44;
loc_82369F8C:
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // mr r6,r18
  r6 = r18;
  // mr r5,r26
  r5 = r26;
  // mr r4,r27
  r4 = r27;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82369FAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // add r30,r30,r22
  r30 = r30 + r22;
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // blt cr6,0x82369ef8
  if (cr6.lt) goto loc_82369EF8;
loc_82369FC4:
  // lwz r9,4(r31)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // add r24,r24,r21
  r24 = r24 + r21;
  // lwz r11,112(r9)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).depth_address());
  // cmplw cr6,r26,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r11), so);
  // blt cr6,0x82369ee0
  if (cr6.lt) goto loc_82369EE0;
loc_82369FDC:
  // mr r3,r19
  r3 = r19;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82369FE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r18
  r3 = r18;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82369FEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82369FF0:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82369FF8 game::Blitter::FilterBox2D: lifted from generated/ by lift.py.
lifted::Result f82369FF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82369FF8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-80(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -80, std::bit_cast<uint64_t>(f31));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lbz r11,11(r31)
  r11 = guest::LoadU8(base, schema::Blitter(base, uint32_t(r31)).filter_type_address());
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bne cr6,0x8236a4dc
  if (!cr6.eq) goto loc_8236A4DC;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r7,104(r11)
  r7 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // lwz r9,104(r10)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // rlwinm r8,r7,31,1,31
  r8 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 31) & 0x7FFFFFFF;
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // beq cr6,0x8236a044
  if (cr6.eq) goto loc_8236A044;
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // bne cr6,0x8236a4dc
  if (!cr6.eq) goto loc_8236A4DC;
  // cmplwi cr6,r7,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 1, so);
  // bne cr6,0x8236a4dc
  if (!cr6.eq) goto loc_8236A4DC;
loc_8236A044:
  // lwz r8,108(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r9,108(r10)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // rlwinm r6,r8,31,1,31
  r6 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 31) & 0x7FFFFFFF;
  // cmplw cr6,r9,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r6), so);
  // beq cr6,0x8236a068
  if (cr6.eq) goto loc_8236A068;
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // bne cr6,0x8236a4dc
  if (!cr6.eq) goto loc_8236A4DC;
  // cmplwi cr6,r8,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 1, so);
  // bne cr6,0x8236a4dc
  if (!cr6.eq) goto loc_8236A4DC;
loc_8236A068:
  // lwz r10,112(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8236a4dc
  if (!cr6.eq) goto loc_8236A4DC;
  // lwz r10,112(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8236a4dc
  if (!cr6.eq) goto loc_8236A4DC;
  // cmplwi cr6,r7,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 1, so);
  // ble cr6,0x8236a0b0
  if (!cr6.gt) goto loc_8236A0B0;
  // clrlwi. r10,r7,31
  r10 = uint32_t(r7) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8236a0b0
  if (cr0.eq) goto loc_8236A0B0;
  // lwz r10,104(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // rlwinm r10,r10,0,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFE;
  // stw r10,104(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address(), uint32_t(r10));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r10,120(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).bytes_per_pixel_address());
  // lwz r9,116(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_bytes_address());
  // subf r10,r10,r9
  r10 = r9 - r10;
  // stw r10,116(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).row_bytes_address(), uint32_t(r10));
loc_8236A0B0:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // ble cr6,0x8236a0cc
  if (!cr6.gt) goto loc_8236A0CC;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // rlwinm r10,r10,0,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFE;
  // stw r10,108(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address(), uint32_t(r10));
loc_8236A0CC:
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).filter_address());
  // rlwinm. r11,r11,0,12,12
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8236a254
  if (!cr0.eq) goto loc_8236A254;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r9,4(r31)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).format_address());
  // lwz r8,4(r9)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).format_address());
  // xor r8,r8,r10
  r8 = r8 ^ r10;
  // rlwinm. r8,r8,0,24,22
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFFFFFFFFFEFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x8236a254
  if (!cr0.eq) goto loc_8236A254;
  // lwz r5,104(r11)
  r5 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r5,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 2, so);
  // blt cr6,0x8236a254
  if (cr6.lt) goto loc_8236A254;
  // lwz r6,108(r11)
  r6 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplwi cr6,r6,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 2, so);
  // blt cr6,0x8236a254
  if (cr6.lt) goto loc_8236A254;
  // lwz r9,16(r9)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).srgb_address());
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x8236a254
  if (!cr6.eq) goto loc_8236A254;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).srgb_address());
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x8236a254
  if (!cr6.eq) goto loc_8236A254;
  // lis r9,10240
  r9 = uint64_t(int64_t(671088640));
  // ori r9,r9,258
  r9 = r9 | 258;
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // bgt cr6,0x8236a1dc
  if (cr6.gt) goto loc_8236A1DC;
  // beq cr6,0x8236a1d0
  if (cr6.eq) goto loc_8236A1D0;
  // lis r9,1168
  r9 = uint64_t(int64_t(76546048));
  // ori r9,r9,258
  r9 = r9 | 258;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x8236a1d0
  if (cr6.eq) goto loc_8236A1D0;
  // lis r9,2048
  r9 = uint64_t(int64_t(134217728));
  // ori r9,r9,330
  r9 = r9 | 330;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x8236a1c4
  if (cr6.eq) goto loc_8236A1C4;
  // addis r10,r10,-6184
  r10 = uint64_t(int64_t(int64_t(r10) + -405274624));
  // addic. r10,r10,-323
  ca = uint32_t(r10) > 322;
  r10 = uint64_t(int64_t(int64_t(r10) + -323));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8236a1b8
  if (cr0.eq) goto loc_8236A1B8;
  // cmplwi cr6,r10,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 12, so);
  // beq cr6,0x8236a1ac
  if (cr6.eq) goto loc_8236A1AC;
  // cmplwi cr6,r10,67
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 67, so);
  // beq cr6,0x8236a184
  if (cr6.eq) goto loc_8236A184;
  // lis r9,504
  r9 = uint64_t(int64_t(33030144));
  // ori r9,r9,67
  r9 = r9 | 67;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8236a254
  if (!cr6.eq) goto loc_8236A254;
loc_8236A184:
  // lis r9,-32156
  r9 = uint64_t(int64_t(-2107375616));
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).data_address());
  // lwz r7,96(r11)
  r7 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_pitch_address());
  // lwz r11,-17012(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + -17012);
loc_8236A194:
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r3,32(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).data_address());
  // lwz r8,96(r10)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).row_pitch_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A1A8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8236a244
  goto loc_8236A244;
loc_8236A1AC:
  // mr r3,r31
  r3 = r31;
  // bl 0x823694c8
  // (left out: dead in C++)
  r3 = f823694C8(base, r3);
  // b 0x8236a244
  goto loc_8236A244;
loc_8236A1B8:
  // mr r3,r31
  r3 = r31;
  // bl 0x823693c8
  // (left out: dead in C++)
  r3 = f823693C8(base, r3);
  // b 0x8236a244
  goto loc_8236A244;
loc_8236A1C4:
  // mr r3,r31
  r3 = r31;
  // bl 0x82369770
  // (left out: dead in C++)
  r3 = f82369770(base, r3);
  // b 0x8236a244
  goto loc_8236A244;
loc_8236A1D0:
  // mr r3,r31
  r3 = r31;
  // bl 0x823695c0
  // (left out: dead in C++)
  r3 = f823695C0(base, r3);
  // b 0x8236a244
  goto loc_8236A244;
loc_8236A1DC:
  // addis r10,r10,-10280
  r10 = uint64_t(int64_t(int64_t(r10) + -673710080));
  // addic. r10,r10,-323
  ca = uint32_t(r10) > 322;
  r10 = uint64_t(int64_t(int64_t(r10) + -323));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8236a23c
  if (cr0.eq) goto loc_8236A23C;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // beq cr6,0x8236a230
  if (cr6.eq) goto loc_8236A230;
  // cmplwi cr6,r10,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 12, so);
  // beq cr6,0x8236a224
  if (cr6.eq) goto loc_8236A224;
  // cmplwi cr6,r10,67
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 67, so);
  // beq cr6,0x8236a210
  if (cr6.eq) goto loc_8236A210;
  // lis r9,504
  r9 = uint64_t(int64_t(33030144));
  // ori r9,r9,67
  r9 = r9 | 67;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8236a254
  if (!cr6.eq) goto loc_8236A254;
loc_8236A210:
  // lis r9,-32156
  r9 = uint64_t(int64_t(-2107375616));
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 32);
  // lwz r7,96(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 96);
  // lwz r11,-17008(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + -17008);
  // b 0x8236a194
  goto loc_8236A194;
loc_8236A224:
  // mr r3,r31
  r3 = r31;
  // bl 0x82369678
  // (left out: dead in C++)
  r3 = f82369678(base, r3);
  // b 0x8236a244
  goto loc_8236A244;
loc_8236A230:
  // mr r3,r31
  r3 = r31;
  // bl 0x823691d8
  // (left out: dead in C++)
  r3 = f823691D8(base, r3);
  // b 0x8236a244
  goto loc_8236A244;
loc_8236A23C:
  // mr r3,r31
  r3 = r31;
  // bl 0x823692d0
  // (left out: dead in C++)
  r3 = f823692D0(base, r3);
loc_8236A244:
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x8236a254
  if (cr6.lt) goto loc_8236A254;
loc_8236A24C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8236a4e4
  goto loc_8236A4E4;
loc_8236A254:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lis r10,4095
  r10 = uint64_t(int64_t(268369920));
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // ori r30,r10,65535
  r30 = r10 | 65535;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236a278
  if (!cr6.gt) goto loc_8236A278;
  // mr r3,r29
  r3 = r29;
loc_8236A278:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236A27C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x8236a294
  if (!cr6.eq) goto loc_8236A294;
loc_8236A288:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x8236a4e4
  goto loc_8236A4E4;
loc_8236A294:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8236a2c8
  if (!cr6.eq) goto loc_8236A2C8;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236a2b8
  if (!cr6.gt) goto loc_8236A2B8;
  // mr r3,r29
  r3 = r29;
loc_8236A2B8:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236A2BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r29,r30
  r29 = r30;
  // b 0x8236a2f4
  goto loc_8236A2F4;
loc_8236A2C8:
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236a2dc
  if (!cr6.gt) goto loc_8236A2DC;
  // mr r3,r29
  r3 = r29;
loc_8236A2DC:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236A2E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r30,r3
  r30 = r3;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r29,r11,r30
  r29 = r11 + r30;
loc_8236A2F4:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8236a308
  if (!cr6.eq) goto loc_8236A308;
  // mr r3,r24
  r3 = r24;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236A304, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8236a288
  goto loc_8236A288;
loc_8236A308:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8236a324
  if (!cr6.eq) goto loc_8236A324;
  // mr r26,r30
  r26 = r30;
  // mr r25,r29
  r25 = r29;
  // b 0x8236a32c
  goto loc_8236A32C;
loc_8236A324:
  // addi r26,r30,16
  r26 = uint64_t(int64_t(int64_t(r30) + 16));
  // addi r25,r29,16
  r25 = uint64_t(int64_t(int64_t(r29) + 16));
loc_8236A32C:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236a4c8
  if (!cr6.gt) goto loc_8236A4C8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,10600(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10600);
  f31 = double(temp.f32);
loc_8236A348:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // rlwinm r28,r27,1,0,30
  r28 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 1) & 0xFFFFFFFE;
  // mr r6,r30
  r6 = r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r28
  r4 = r28;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A36C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r29,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r30), so);
  // beq cr6,0x8236a394
  if (cr6.eq) goto loc_8236A394;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r6,r29
  r6 = r29;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r28,1
  r4 = uint64_t(int64_t(int64_t(r28) + 1));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A394, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236A394:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236a494
  if (!cr6.gt) goto loc_8236A494;
  // mr r4,r24
  r4 = r24;
loc_8236A3AC:
  // mr r9,r5
  r9 = r5;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // rlwinm r9,r9,5,0,26
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 5) & 0xFFFFFFE0;
  // mr r10,r4
  r10 = r4;
  // add r8,r9,r26
  r8 = r9 + r26;
  // add r7,r9,r30
  r7 = r9 + r30;
  // add r6,r9,r29
  r6 = r9 + r29;
  // add r9,r9,r25
  r9 = r9 + r25;
  // addi r5,r5,1
  r5 = uint64_t(int64_t(int64_t(r5) + 1));
  // lfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // addi r4,r4,16
  r4 = uint64_t(int64_t(int64_t(r4) + 16));
  // lfs f13,0(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 0);
  f13 = double(temp.f32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // lfs f12,4(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 4);
  f12 = double(temp.f32);
  // lfs f13,4(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 4);
  f13 = double(temp.f32);
  // fadds f13,f12,f13
  f13 = double(float(f12 + f13));
  // lfs f11,8(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 8);
  f11 = double(temp.f32);
  // lfs f12,8(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8);
  f12 = double(temp.f32);
  // fadds f12,f11,f12
  f12 = double(float(f11 + f12));
  // lfs f10,12(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 12);
  f10 = double(temp.f32);
  // lfs f11,12(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 12);
  f11 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // lfs f9,0(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 0);
  f9 = double(temp.f32);
  // lfs f10,4(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 4);
  f10 = double(temp.f32);
  // fadds f0,f9,f0
  f0 = double(float(f9 + f0));
  // lfs f9,8(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 8);
  f9 = double(temp.f32);
  // fadds f13,f10,f13
  f13 = double(float(f10 + f13));
  // lfs f10,12(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 12);
  f10 = double(temp.f32);
  // fadds f12,f9,f12
  f12 = double(float(f9 + f12));
  // lfs f9,0(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 0);
  f9 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // lfs f10,4(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 4);
  f10 = double(temp.f32);
  // fadds f0,f0,f9
  f0 = double(float(f0 + f9));
  // lfs f9,8(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8);
  f9 = double(temp.f32);
  // fadds f13,f10,f13
  f13 = double(float(f10 + f13));
  // lfs f10,12(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 12);
  f10 = double(temp.f32);
  // fadds f12,f9,f12
  f12 = double(float(f9 + f12));
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // fmuls f0,f13,f31
  f0 = double(float(f13 * f31));
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // fmuls f0,f12,f31
  f0 = double(float(f12 * f31));
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // fmuls f0,f11,f31
  f0 = double(float(f11 * f31));
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r7,8(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // stw r9,0(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).red_address(), uint32_t(r9));
  // stw r8,4(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).green_address(), uint32_t(r8));
  // stw r7,8(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).blue_address(), uint32_t(r7));
  // stw r11,12(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).alpha_address(), uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r5,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r11), so);
  // blt cr6,0x8236a3ac
  if (cr6.lt) goto loc_8236A3AC;
loc_8236A494:
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // mr r6,r24
  r6 = r24;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r27
  r4 = r27;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A4B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // blt cr6,0x8236a348
  if (cr6.lt) goto loc_8236A348;
loc_8236A4C8:
  // mr r3,r30
  r3 = r30;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236A4D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r24
  r3 = r24;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236A4D8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8236a24c
  goto loc_8236A24C;
loc_8236A4DC:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
loc_8236A4E4:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-80(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -80)));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8236A4F0 game::Blitter::FilterBox3D: lifted from generated/ by lift.py.
lifted::Result f8236A4F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_8236A4F0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e58
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-144(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -144, std::bit_cast<uint64_t>(f31));
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r3
  r26 = r3;
  // lbz r11,11(r26)
  r11 = guest::LoadU8(base, schema::Blitter(base, uint32_t(r26)).filter_type_address());
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // beq cr6,0x8236a524
  if (cr6.eq) goto loc_8236A524;
loc_8236A510:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
loc_8236A518:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfd f31,-144(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -144)));
  // b 0x825a6ea8
  // (left out: dead in C++)
  return {r3, f1};
loc_8236A524:
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // lwz r10,4(r26)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).destination_address());
  // lwz r7,104(r11)
  r7 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // lwz r9,104(r10)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // rlwinm r8,r7,31,1,31
  r8 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 31) & 0x7FFFFFFF;
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // beq cr6,0x8236a550
  if (cr6.eq) goto loc_8236A550;
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // bne cr6,0x8236a510
  if (!cr6.eq) goto loc_8236A510;
  // cmplwi cr6,r7,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 1, so);
  // bne cr6,0x8236a510
  if (!cr6.eq) goto loc_8236A510;
loc_8236A550:
  // lwz r8,108(r11)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r9,108(r10)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // rlwinm r6,r8,31,1,31
  r6 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 31) & 0x7FFFFFFF;
  // cmplw cr6,r9,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r6), so);
  // beq cr6,0x8236a574
  if (cr6.eq) goto loc_8236A574;
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // bne cr6,0x8236a510
  if (!cr6.eq) goto loc_8236A510;
  // cmplwi cr6,r8,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 1, so);
  // bne cr6,0x8236a510
  if (!cr6.eq) goto loc_8236A510;
loc_8236A574:
  // lwz r9,112(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // lwz r10,112(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // rlwinm r9,r9,31,1,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 31) & 0x7FFFFFFF;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8236a510
  if (!cr6.eq) goto loc_8236A510;
  // cmplwi cr6,r7,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 1, so);
  // ble cr6,0x8236a5b8
  if (!cr6.gt) goto loc_8236A5B8;
  // clrlwi. r10,r7,31
  r10 = uint32_t(r7) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8236a5b8
  if (cr0.eq) goto loc_8236A5B8;
  // lwz r10,104(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // rlwinm r10,r10,0,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFE;
  // stw r10,104(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address(), uint32_t(r10));
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // lwz r10,120(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).bytes_per_pixel_address());
  // lwz r9,116(r11)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).row_bytes_address());
  // subf r10,r10,r9
  r10 = r9 - r10;
  // stw r10,116(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).row_bytes_address(), uint32_t(r10));
loc_8236A5B8:
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // ble cr6,0x8236a5d4
  if (!cr6.gt) goto loc_8236A5D4;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // rlwinm r10,r10,0,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFE;
  // stw r10,108(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address(), uint32_t(r10));
loc_8236A5D4:
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // lwz r10,112(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // ble cr6,0x8236a5f0
  if (!cr6.gt) goto loc_8236A5F0;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // rlwinm r10,r10,0,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFE;
  // stw r10,112(r11)
  guest::StoreU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address(), uint32_t(r10));
loc_8236A5F0:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).destination_address());
  // lis r10,4095
  r10 = uint64_t(int64_t(268369920));
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // ori r31,r10,65535
  r31 = r10 | 65535;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236a614
  if (!cr6.gt) goto loc_8236A614;
  // mr r3,r30
  r3 = r30;
loc_8236A614:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236A618, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r16,r3
  r16 = r3;
  // cmplwi cr6,r16,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r16), 0, so);
  // bne cr6,0x8236a630
  if (!cr6.eq) goto loc_8236A630;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x8236a518
  goto loc_8236A518;
loc_8236A630:
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8236a680
  if (!cr6.eq) goto loc_8236A680;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236a658
  if (!cr6.gt) goto loc_8236A658;
  // mr r3,r30
  r3 = r30;
loc_8236A658:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236A65C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // mr r30,r3
  r30 = r3;
  // addi r10,r11,104
  r10 = uint64_t(int64_t(int64_t(r11) + 104));
  // mr r25,r30
  r25 = r30;
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10) - 104).width_address());
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r27,r11,r30
  r27 = r11 + r30;
  // mr r24,r27
  r24 = r27;
  // b 0x8236a6c0
  goto loc_8236A6C0;
loc_8236A680:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236a694
  if (!cr6.gt) goto loc_8236A694;
  // mr r3,r30
  r3 = r30;
loc_8236A694:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236A698, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // mr r30,r3
  r30 = r3;
  // addi r10,r11,104
  r10 = uint64_t(int64_t(int64_t(r11) + 104));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10) - 104).width_address());
  // rlwinm r8,r11,4,0,27
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // rlwinm r9,r11,5,0,26
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // mulli r11,r11,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(48))));
  // add r27,r8,r30
  r27 = r8 + r30;
  // add r25,r9,r30
  r25 = r9 + r30;
  // add r24,r11,r30
  r24 = r11 + r30;
loc_8236A6C0:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8236a6e0
  if (!cr6.eq) goto loc_8236A6E0;
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // ori r31,r31,14
  r31 = r31 | 14;
loc_8236A6D0:
  // mr r3,r16
  r3 = r16;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236A6D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // b 0x8236a518
  goto loc_8236A518;
loc_8236A6E0:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8236a700
  if (!cr6.eq) goto loc_8236A700;
  // mr r22,r30
  r22 = r30;
  // mr r21,r27
  r21 = r27;
  // mr r20,r25
  r20 = r25;
  // mr r19,r24
  r19 = r24;
  // b 0x8236a710
  goto loc_8236A710;
loc_8236A700:
  // addi r22,r30,16
  r22 = uint64_t(int64_t(int64_t(r30) + 16));
  // addi r21,r27,16
  r21 = uint64_t(int64_t(int64_t(r27) + 16));
  // addi r20,r25,16
  r20 = uint64_t(int64_t(int64_t(r25) + 16));
  // addi r19,r24,16
  r19 = uint64_t(int64_t(int64_t(r24) + 16));
loc_8236A710:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).destination_address());
  // li r17,0
  r17 = uint64_t(int64_t(0));
  // lwz r10,112(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // ble cr6,0x8236a9b8
  if (!cr6.gt) goto loc_8236A9B8;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f31,10888(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 10888);
  f31 = double(temp.f32);
loc_8236A72C:
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // rlwinm r23,r17,1,0,30
  r23 = __builtin_rotateleft64(uint32_t(r17) | (r17 << 32), 1) & 0xFFFFFFFE;
  // li r18,0
  r18 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236a9a4
  if (!cr6.gt) goto loc_8236A9A4;
loc_8236A740:
  // lwz r3,0(r26)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // rlwinm r31,r18,1,0,30
  r31 = __builtin_rotateleft64(uint32_t(r18) | (r18 << 32), 1) & 0xFFFFFFFE;
  // mr r6,r30
  r6 = r30;
  // mr r5,r23
  r5 = r23;
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A764, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r27,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r30), so);
  // beq cr6,0x8236a78c
  if (cr6.eq) goto loc_8236A78C;
  // lwz r3,0(r26)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // mr r6,r27
  r6 = r27;
  // addi r5,r23,1
  r5 = uint64_t(int64_t(int64_t(r23) + 1));
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A78C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236A78C:
  // cmplw cr6,r25,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), uint32_t(r30), so);
  // beq cr6,0x8236a7b4
  if (cr6.eq) goto loc_8236A7B4;
  // lwz r3,0(r26)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // mr r6,r25
  r6 = r25;
  // mr r5,r23
  r5 = r23;
  // addi r4,r31,1
  r4 = uint64_t(int64_t(int64_t(r31) + 1));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A7B4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236A7B4:
  // cmplw cr6,r24,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r27), so);
  // beq cr6,0x8236a7e4
  if (cr6.eq) goto loc_8236A7E4;
  // cmplw cr6,r24,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r25), so);
  // beq cr6,0x8236a7e4
  if (cr6.eq) goto loc_8236A7E4;
  // lwz r3,0(r26)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).source_address());
  // mr r6,r24
  r6 = r24;
  // addi r5,r23,1
  r5 = uint64_t(int64_t(int64_t(r23) + 1));
  // addi r4,r31,1
  r4 = uint64_t(int64_t(int64_t(r31) + 1));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A7E4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236A7E4:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).destination_address());
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236a970
  if (!cr6.gt) goto loc_8236A970;
  // mr r28,r16
  r28 = r16;
loc_8236A7FC:
  // rlwinm r11,r29,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r8,r11,r22
  r8 = r11 + r22;
  // add r7,r11,r30
  r7 = r11 + r30;
  // add r6,r11,r27
  r6 = r11 + r27;
  // add r5,r11,r21
  r5 = r11 + r21;
  // add r4,r11,r25
  r4 = r11 + r25;
  // lfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // add r3,r11,r20
  r3 = r11 + r20;
  // lfs f13,0(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 0);
  f13 = double(temp.f32);
  // add r31,r11,r24
  r31 = r11 + r24;
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // lfs f12,4(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 4);
  f12 = double(temp.f32);
  // lfs f13,4(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 4);
  f13 = double(temp.f32);
  // add r11,r11,r19
  r11 = r11 + r19;
  // fadds f13,f12,f13
  f13 = double(float(f12 + f13));
  // lfs f11,8(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 8);
  f11 = double(temp.f32);
  // lfs f12,8(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8);
  f12 = double(temp.f32);
  // mr r9,r28
  r9 = r28;
  // fadds f12,f11,f12
  f12 = double(float(f11 + f12));
  // lfs f10,12(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 12);
  f10 = double(temp.f32);
  // lfs f11,12(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 12);
  f11 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // lfs f9,0(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 0);
  f9 = double(temp.f32);
  // lfs f10,4(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 4);
  f10 = double(temp.f32);
  // fadds f0,f9,f0
  f0 = double(float(f9 + f0));
  // lfs f9,8(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 8);
  f9 = double(temp.f32);
  // fadds f13,f10,f13
  f13 = double(float(f10 + f13));
  // lfs f10,12(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 12);
  f10 = double(temp.f32);
  // fadds f12,f9,f12
  f12 = double(float(f9 + f12));
  // lfs f9,0(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 0);
  f9 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // lfs f10,4(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 4);
  f10 = double(temp.f32);
  // fadds f0,f9,f0
  f0 = double(float(f9 + f0));
  // lfs f9,8(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 8);
  f9 = double(temp.f32);
  // fadds f13,f10,f13
  f13 = double(float(f10 + f13));
  // lfs f10,12(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 12);
  f10 = double(temp.f32);
  // fadds f12,f9,f12
  f12 = double(float(f9 + f12));
  // lfs f9,0(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 0);
  f9 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // lfs f10,4(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 4);
  f10 = double(temp.f32);
  // fadds f0,f9,f0
  f0 = double(float(f9 + f0));
  // lfs f9,8(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 8);
  f9 = double(temp.f32);
  // fadds f13,f10,f13
  f13 = double(float(f10 + f13));
  // lfs f10,12(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 12);
  f10 = double(temp.f32);
  // fadds f12,f9,f12
  f12 = double(float(f9 + f12));
  // lfs f9,0(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 0);
  f9 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // lfs f10,4(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 4);
  f10 = double(temp.f32);
  // fadds f0,f0,f9
  f0 = double(float(f0 + f9));
  // lfs f9,8(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8);
  f9 = double(temp.f32);
  // fadds f13,f10,f13
  f13 = double(float(f10 + f13));
  // lfs f10,12(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 12);
  f10 = double(temp.f32);
  // fadds f12,f9,f12
  f12 = double(float(f9 + f12));
  // lfs f9,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f9 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // lfs f10,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f10 = double(temp.f32);
  // fadds f0,f9,f0
  f0 = double(float(f9 + f0));
  // lfs f9,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f9 = double(temp.f32);
  // fadds f13,f10,f13
  f13 = double(float(f10 + f13));
  // lfs f10,12(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 12);
  f10 = double(temp.f32);
  // fadds f12,f9,f12
  f12 = double(float(f9 + f12));
  // lfs f9,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f9 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // lfs f10,4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f10 = double(temp.f32);
  // fadds f0,f9,f0
  f0 = double(float(f9 + f0));
  // lfs f9,8(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8);
  f9 = double(temp.f32);
  // fadds f13,f10,f13
  f13 = double(float(f10 + f13));
  // lfs f10,12(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 12);
  f10 = double(temp.f32);
  // fadds f12,f9,f12
  f12 = double(float(f9 + f12));
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // fmuls f0,f13,f31
  f0 = double(float(f13 * f31));
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // fmuls f0,f12,f31
  f0 = double(float(f12 * f31));
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // fmuls f0,f11,f31
  f0 = double(float(f11 * f31));
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // lwz r8,4(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 4);
  // stw r11,0(r9)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r9)).red_address(), uint32_t(r11));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // lwz r7,8(r10)
  r7 = guest::LoadU32(base, uint32_t(r10) + 8);
  // addi r28,r28,16
  r28 = uint64_t(int64_t(int64_t(r28) + 16));
  // lwz r11,12(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 12);
  // stw r8,4(r9)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r9)).green_address(), uint32_t(r8));
  // stw r7,8(r9)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r9)).blue_address(), uint32_t(r7));
  // stw r11,12(r9)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r9)).alpha_address(), uint32_t(r11));
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8236a7fc
  if (cr6.lt) goto loc_8236A7FC;
loc_8236A970:
  // lwz r3,4(r26)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).destination_address());
  // mr r6,r16
  r6 = r16;
  // mr r5,r17
  r5 = r17;
  // mr r4,r18
  r4 = r18;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236A990, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).destination_address());
  // addi r18,r18,1
  r18 = uint64_t(int64_t(int64_t(r18) + 1));
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r18,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), uint32_t(r11), so);
  // blt cr6,0x8236a740
  if (cr6.lt) goto loc_8236A740;
loc_8236A9A4:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r26)).destination_address());
  // addi r17,r17,1
  r17 = uint64_t(int64_t(int64_t(r17) + 1));
  // lwz r10,112(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // cmplw cr6,r17,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r17), uint32_t(r10), so);
  // blt cr6,0x8236a72c
  if (cr6.lt) goto loc_8236A72C;
loc_8236A9B8:
  // mr r3,r30
  r3 = r30;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236A9C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x8236a6d0
  goto loc_8236A6D0;
}

// sub_8236AB00 game::Blitter::FilterLinear2D: lifted from generated/ by lift.py.
lifted::Result f8236AB00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8236AB00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e64
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-112
  r12 = uint64_t(int64_t(int64_t(r1) + -112));
  // bl 0x825a7910
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r21,0
  r21 = uint64_t(int64_t(0));
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).filter_address());
  // clrlwi r10,r11,24
  r10 = uint32_t(r11) & 0xFF;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x8236ae8c
  if (!cr6.eq) goto loc_8236AE8C;
  // lwz r9,4(r31)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r10,112(r9)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).depth_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8236ae8c
  if (!cr6.eq) goto loc_8236AE8C;
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r8,112(r10)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // cmplwi cr6,r8,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 1, so);
  // bne cr6,0x8236ae8c
  if (!cr6.eq) goto loc_8236AE8C;
  // lwz r4,104(r9)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).width_address());
  // not r9,r11
  r9 = ~r11;
  // not r11,r11
  r11 = ~r11;
  // lwz r3,104(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // rlwinm r5,r9,16,31,31
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 16) & 0x1;
  // rlwinm r30,r11,15,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 15) & 0x1;
  // bl 0x8236a9c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::LinearWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r5,r30
  r5 = r30;
  // mr r20,r3
  r20 = r3;
  // lwz r4,108(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r3,108(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // bl 0x8236a9c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::LinearWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r19,r3
  r19 = r3;
  // cmplwi cr6,r20,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r20), 0, so);
  // beq cr6,0x8236ae5c
  if (cr6.eq) goto loc_8236AE5C;
  // cmplwi cr6,r19,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r19), 0, so);
  // beq cr6,0x8236ae5c
  if (cr6.eq) goto loc_8236AE5C;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lis r10,4095
  r10 = uint64_t(int64_t(268369920));
  // li r23,-1
  r23 = uint64_t(int64_t(-1));
  // ori r30,r10,65535
  r30 = r10 | 65535;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236abc0
  if (!cr6.gt) goto loc_8236ABC0;
  // mr r3,r23
  r3 = r23;
loc_8236ABC0:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236ABC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r21,r3
  r21 = r3;
  // cmplwi cr6,r21,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), 0, so);
  // beq cr6,0x8236ae5c
  if (cr6.eq) goto loc_8236AE5C;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236abec
  if (!cr6.gt) goto loc_8236ABEC;
  // mr r3,r23
  r3 = r23;
loc_8236ABEC:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236ABF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r22,r3
  r22 = r3;
  // cmplwi cr6,r22,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r22), 0, so);
  // beq cr6,0x8236ae5c
  if (cr6.eq) goto loc_8236AE5C;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r30,r22
  r30 = r22;
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // mr r25,r23
  r25 = r23;
  // mr r26,r23
  r26 = r23;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // lwz r10,108(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // add r29,r11,r22
  r29 = r11 + r22;
  // ble cr6,0x8236ae54
  if (!cr6.gt) goto loc_8236AE54;
  // addi r28,r19,8
  r28 = uint64_t(int64_t(int64_t(r19) + 8));
loc_8236AC30:
  // lwz r11,-8(r28)
  r11 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r28) - 8).first_index_address());
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // cmplw cr6,r11,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r25), so);
  // beq cr6,0x8236ac84
  if (cr6.eq) goto loc_8236AC84;
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // beq cr6,0x8236ac70
  if (cr6.eq) goto loc_8236AC70;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r25,r11
  r25 = r11;
  // mr r6,r30
  r6 = r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r25
  r4 = r25;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236AC6C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8236ac84
  goto loc_8236AC84;
loc_8236AC70:
  // mr r11,r30
  r11 = r30;
  // mr r25,r26
  r25 = r26;
  // mr r30,r29
  r30 = r29;
  // mr r26,r23
  r26 = r23;
  // mr r29,r11
  r29 = r11;
loc_8236AC84:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r28) - 8).second_index_address());
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // beq cr6,0x8236acb4
  if (cr6.eq) goto loc_8236ACB4;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r26,r11
  r26 = r11;
  // mr r6,r29
  r6 = r29;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r26
  r4 = r26;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236ACB4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236ACB4:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236ae1c
  if (!cr6.gt) goto loc_8236AE1C;
  // addi r11,r20,8
  r11 = uint64_t(int64_t(int64_t(r20) + 8));
  // mr r10,r21
  r10 = r21;
loc_8236ACCC:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).second_index_address());
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).second_weight_address());
  f0 = double(temp.f32);
  // lwz r8,-8(r11)
  r8 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).first_weight_address());
  f13 = double(temp.f32);
  // rlwinm r6,r9,4,0,27
  r6 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // lfs f12,4(r28)
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r28) - 8).second_weight_address());
  f12 = double(temp.f32);
  // rlwinm r5,r8,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // lfs f11,-4(r28)
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r28) - 8).first_weight_address());
  f11 = double(temp.f32);
  // add r8,r6,r29
  r8 = r6 + r29;
  // add r7,r5,r29
  r7 = r5 + r29;
  // add r6,r6,r30
  r6 = r6 + r30;
  // add r5,r5,r30
  r5 = r5 + r30;
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f10,0(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 0);
  f10 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lfs f6,0(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 0);
  f6 = double(temp.f32);
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // lfs f9,4(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 4);
  f9 = double(temp.f32);
  // fmuls f6,f13,f6
  f6 = double(float(f13 * f6));
  // lfs f8,8(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 8);
  f8 = double(temp.f32);
  // fmuls f9,f9,f0
  f9 = double(float(f9 * f0));
  // lfs f7,12(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 12);
  f7 = double(temp.f32);
  // fmuls f8,f8,f0
  f8 = double(float(f8 * f0));
  // lfs f5,4(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 4);
  f5 = double(temp.f32);
  // fmuls f7,f7,f0
  f7 = double(float(f7 * f0));
  // lfs f2,0(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 0);
  f2 = double(temp.f32);
  // fmuls f5,f5,f13
  f5 = double(float(f5 * f13));
  // lfs f1,4(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 4);
  f1 = double(temp.f32);
  // fmuls f2,f0,f2
  f2 = double(float(f0 * f2));
  // lfs f31,8(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 8);
  f31 = double(temp.f32);
  // fmuls f1,f1,f0
  f1 = double(float(f1 * f0));
  // lfs f30,12(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 12);
  f30 = double(temp.f32);
  // fmuls f31,f31,f0
  f31 = double(float(f31 * f0));
  // lfs f4,8(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8);
  f4 = double(temp.f32);
  // fmuls f0,f30,f0
  f0 = double(float(f30 * f0));
  // lfs f29,0(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 0);
  f29 = double(temp.f32);
  // fmuls f4,f4,f13
  f4 = double(float(f4 * f13));
  // lfs f3,12(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 12);
  f3 = double(temp.f32);
  // fmuls f30,f13,f29
  f30 = double(float(f13 * f29));
  // lfs f28,4(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 4);
  f28 = double(temp.f32);
  // fmuls f3,f3,f13
  f3 = double(float(f3 * f13));
  // lfs f27,8(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 8);
  f27 = double(temp.f32);
  // fmuls f29,f28,f13
  f29 = double(float(f28 * f13));
  // lfs f26,12(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 12);
  f26 = double(temp.f32);
  // fmuls f28,f27,f13
  f28 = double(float(f27 * f13));
  // fmuls f13,f26,f13
  f13 = double(float(f26 * f13));
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // fadds f10,f6,f10
  f10 = double(float(f6 + f10));
  // fadds f9,f5,f9
  f9 = double(float(f5 + f9));
  // fadds f8,f4,f8
  f8 = double(float(f4 + f8));
  // fadds f6,f30,f2
  f6 = double(float(f30 + f2));
  // fadds f7,f3,f7
  f7 = double(float(f3 + f7));
  // fadds f5,f29,f1
  f5 = double(float(f29 + f1));
  // fadds f4,f28,f31
  f4 = double(float(f28 + f31));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // fmuls f13,f10,f12
  f13 = double(float(f10 * f12));
  // fmuls f10,f9,f12
  f10 = double(float(f9 * f12));
  // fmuls f9,f8,f12
  f9 = double(float(f8 * f12));
  // fmuls f8,f6,f11
  f8 = double(float(f6 * f11));
  // fmuls f12,f7,f12
  f12 = double(float(f7 * f12));
  // fmuls f7,f5,f11
  f7 = double(float(f5 * f11));
  // fmuls f6,f4,f11
  f6 = double(float(f4 * f11));
  // fmuls f0,f0,f11
  f0 = double(float(f0 * f11));
  // fadds f13,f8,f13
  f13 = double(float(f8 + f13));
  // stfs f13,80(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // fadds f13,f7,f10
  f13 = double(float(f7 + f10));
  // stfs f13,84(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // fadds f13,f6,f9
  f13 = double(float(f6 + f9));
  // stfs f13,88(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // fadds f0,f0,f12
  f0 = double(float(f0 + f12));
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // lwz r8,0(r9)
  r8 = guest::LoadU32(base, uint32_t(r9) + 0);
  // lwz r7,4(r9)
  r7 = guest::LoadU32(base, uint32_t(r9) + 4);
  // lwz r6,8(r9)
  r6 = guest::LoadU32(base, uint32_t(r9) + 8);
  // lwz r9,12(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 12);
  // stw r8,0(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).red_address(), uint32_t(r8));
  // stw r7,4(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).green_address(), uint32_t(r7));
  // stw r6,8(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).blue_address(), uint32_t(r6));
  // stw r9,12(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).alpha_address(), uint32_t(r9));
  // lwz r9,4(r31)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r9,104(r9)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).width_address());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // cmplw cr6,r27,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r9), so);
  // blt cr6,0x8236accc
  if (cr6.lt) goto loc_8236ACCC;
loc_8236AE1C:
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // mr r6,r21
  r6 = r21;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r24
  r4 = r24;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236AE3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // addi r24,r24,1
  r24 = uint64_t(int64_t(int64_t(r24) + 1));
  // addi r28,r28,16
  r28 = uint64_t(int64_t(int64_t(r28) + 16));
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r24,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r11), so);
  // blt cr6,0x8236ac30
  if (cr6.lt) goto loc_8236AC30;
loc_8236AE54:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x8236ae64
  goto loc_8236AE64;
loc_8236AE5C:
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // ori r31,r31,14
  r31 = r31 | 14;
loc_8236AE64:
  // mr r3,r20
  r3 = r20;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236AE6C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r19
  r3 = r19;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236AE74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r21
  r3 = r21;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236AE7C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r22
  r3 = r22;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236AE84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // b 0x8236ae94
  goto loc_8236AE94;
loc_8236AE8C:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
loc_8236AE94:
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // addi r12,r1,-112
  r12 = uint64_t(int64_t(int64_t(r1) + -112));
  // bl 0x825a795c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8236AEA8 game::Blitter::FilterLinear3D: lifted from generated/ by lift.py.
lifted::Result f8236AEA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8236AEA8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f20 = 0.0, f21 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-152
  r12 = uint64_t(int64_t(int64_t(r1) + -152));
  // bl 0x825a78f8
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-352(r1)
  ea = -352 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r17,0
  r17 = uint64_t(int64_t(0));
  // li r20,0
  r20 = uint64_t(int64_t(0));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).filter_address());
  // clrlwi r10,r11,24
  r10 = uint32_t(r11) & 0xFF;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // beq cr6,0x8236aee4
  if (cr6.eq) goto loc_8236AEE4;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x8236b408
  goto loc_8236B408;
loc_8236AEE4:
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // not r9,r11
  r9 = ~r11;
  // rlwinm r5,r9,16,31,31
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 16) & 0x1;
  // lwz r3,104(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // not r10,r11
  r10 = ~r11;
  // not r11,r11
  r11 = ~r11;
  // rlwinm r30,r10,15,31,31
  r30 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 15) & 0x1;
  // rlwinm r29,r11,14,31,31
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 14) & 0x1;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r4,104(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // bl 0x8236a9c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::LinearWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // mr r5,r30
  r5 = r30;
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r16,r3
  r16 = r3;
  // lwz r4,108(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r3,108(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // bl 0x8236a9c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::LinearWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // mr r5,r29
  r5 = r29;
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r15,r3
  r15 = r3;
  // lwz r4,112(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // lwz r3,112(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // bl 0x8236a9c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::LinearWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r14,r3
  r14 = r3;
  // cmplwi cr6,r16,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r16), 0, so);
  // beq cr6,0x8236b3d4
  if (cr6.eq) goto loc_8236B3D4;
  // cmplwi cr6,r15,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r15), 0, so);
  // beq cr6,0x8236b3d4
  if (cr6.eq) goto loc_8236B3D4;
  // cmplwi cr6,r14,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r14), 0, so);
  // beq cr6,0x8236b3d4
  if (cr6.eq) goto loc_8236B3D4;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lis r10,4095
  r10 = uint64_t(int64_t(268369920));
  // li r18,-1
  r18 = uint64_t(int64_t(-1));
  // ori r30,r10,65535
  r30 = r10 | 65535;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236af88
  if (!cr6.gt) goto loc_8236AF88;
  // mr r3,r18
  r3 = r18;
loc_8236AF88:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236AF8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r17,r3
  r17 = r3;
  // cmplwi cr6,r17,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r17), 0, so);
  // beq cr6,0x8236b3d4
  if (cr6.eq) goto loc_8236B3D4;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236afb4
  if (!cr6.gt) goto loc_8236AFB4;
  // mr r3,r18
  r3 = r18;
loc_8236AFB4:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236AFB8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r20,r3
  r20 = r3;
  // cmplwi cr6,r20,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r20), 0, so);
  // beq cr6,0x8236b3d4
  if (cr6.eq) goto loc_8236B3D4;
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // mr r29,r20
  r29 = r20;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // li r19,0
  r19 = uint64_t(int64_t(0));
  // lwz r9,112(r10)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // rlwinm r8,r11,4,0,27
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // rlwinm r9,r11,5,0,26
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // mulli r11,r11,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(48))));
  // add r28,r8,r20
  r28 = r8 + r20;
  // add r27,r9,r20
  r27 = r9 + r20;
  // add r26,r11,r20
  r26 = r11 + r20;
  // ble cr6,0x8236b3cc
  if (!cr6.gt) goto loc_8236B3CC;
  // addi r30,r14,8
  r30 = uint64_t(int64_t(int64_t(r14) + 8));
loc_8236B000:
  // lwz r11,108(r10)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // li r21,0
  r21 = uint64_t(int64_t(0));
  // mr r22,r18
  r22 = r18;
  // mr r23,r18
  r23 = r18;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236b3b4
  if (!cr6.gt) goto loc_8236B3B4;
  // addi r24,r15,8
  r24 = uint64_t(int64_t(int64_t(r15) + 8));
loc_8236B01C:
  // lwz r11,-8(r24)
  r11 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r24) - 8).first_index_address());
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // cmplw cr6,r11,r22
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r22), so);
  // beq cr6,0x8236b09c
  if (cr6.eq) goto loc_8236B09C;
  // cmplw cr6,r11,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r23), so);
  // beq cr6,0x8236b07c
  if (cr6.eq) goto loc_8236B07C;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r22,r11
  r22 = r11;
  // lwz r5,-8(r30)
  r5 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r30) - 8).first_index_address());
  // mr r6,r29
  r6 = r29;
  // mr r4,r22
  r4 = r22;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236B058, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r5,0(r30)
  r5 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r30) - 8).second_index_address());
  // mr r6,r27
  r6 = r27;
  // mr r4,r22
  r4 = r22;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236B078, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8236b09c
  goto loc_8236B09C;
loc_8236B07C:
  // mr r11,r29
  r11 = r29;
  // mr r29,r28
  r29 = r28;
  // mr r28,r11
  r28 = r11;
  // mr r11,r27
  r11 = r27;
  // mr r22,r23
  r22 = r23;
  // mr r27,r26
  r27 = r26;
  // mr r23,r18
  r23 = r18;
  // mr r26,r11
  r26 = r11;
loc_8236B09C:
  // lwz r11,0(r24)
  r11 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r24) - 8).second_index_address());
  // cmplw cr6,r11,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r23), so);
  // beq cr6,0x8236b0ec
  if (cr6.eq) goto loc_8236B0EC;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // mr r23,r11
  r23 = r11;
  // lwz r5,-8(r30)
  r5 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r30) - 8).first_index_address());
  // mr r6,r28
  r6 = r28;
  // mr r4,r23
  r4 = r23;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236B0CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).source_address());
  // lwz r5,0(r30)
  r5 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r30) - 8).second_index_address());
  // mr r6,r26
  r6 = r26;
  // mr r4,r23
  r4 = r23;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236B0EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236B0EC:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236b37c
  if (!cr6.gt) goto loc_8236B37C;
  // addi r11,r16,8
  r11 = uint64_t(int64_t(int64_t(r16) + 8));
  // mr r10,r17
  r10 = r17;
loc_8236B104:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).second_index_address());
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).second_weight_address());
  f0 = double(temp.f32);
  // lwz r8,-8(r11)
  r8 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).first_weight_address());
  f13 = double(temp.f32);
  // rlwinm r4,r9,4,0,27
  r4 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // lfs f12,4(r24)
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r24) - 8).second_weight_address());
  f12 = double(temp.f32);
  // rlwinm r9,r8,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // lfs f11,-4(r24)
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r24) - 8).first_weight_address());
  f11 = double(temp.f32);
  // add r8,r4,r26
  r8 = r4 + r26;
  // lfs f10,4(r30)
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r30) - 8).second_weight_address());
  f10 = double(temp.f32);
  // add r7,r9,r26
  r7 = r9 + r26;
  // add r6,r4,r27
  r6 = r4 + r27;
  // add r5,r9,r27
  r5 = r9 + r27;
  // add r4,r4,r28
  r4 = r4 + r28;
  // lfs f9,0(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 0);
  f9 = double(temp.f32);
  // lfs f5,0(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 0);
  f5 = double(temp.f32);
  // fmuls f9,f9,f0
  f9 = double(float(f9 * f0));
  // lfs f4,4(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 4);
  f4 = double(temp.f32);
  // fmuls f5,f5,f13
  f5 = double(float(f5 * f13));
  // lfs f3,8(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8);
  f3 = double(temp.f32);
  // fmuls f4,f4,f13
  f4 = double(float(f4 * f13));
  // lfs f2,12(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 12);
  f2 = double(temp.f32);
  // fmuls f3,f3,f13
  f3 = double(float(f3 * f13));
  // lfs f8,4(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 4);
  f8 = double(temp.f32);
  // fmuls f2,f2,f13
  f2 = double(float(f2 * f13));
  // lfs f7,8(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 8);
  f7 = double(temp.f32);
  // fmuls f8,f8,f0
  f8 = double(float(f8 * f0));
  // lfs f6,12(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 12);
  f6 = double(temp.f32);
  // fmuls f7,f7,f0
  f7 = double(float(f7 * f0));
  // lfs f1,0(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 0);
  f1 = double(temp.f32);
  // fmuls f6,f6,f0
  f6 = double(float(f6 * f0));
  // lfs f31,4(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 4);
  f31 = double(temp.f32);
  // fmuls f1,f1,f0
  f1 = double(float(f1 * f0));
  // lfs f30,8(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 8);
  f30 = double(temp.f32);
  // fmuls f31,f31,f0
  f31 = double(float(f31 * f0));
  // lfs f29,12(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 12);
  f29 = double(temp.f32);
  // fmuls f30,f30,f0
  f30 = double(float(f30 * f0));
  // lfs f28,0(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 0);
  f28 = double(temp.f32);
  // fmuls f29,f29,f0
  f29 = double(float(f29 * f0));
  // lfs f27,4(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 4);
  f27 = double(temp.f32);
  // fmuls f28,f28,f13
  f28 = double(float(f28 * f13));
  // lfs f26,8(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 8);
  f26 = double(temp.f32);
  // fmuls f27,f27,f13
  f27 = double(float(f27 * f13));
  // lfs f25,12(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 12);
  f25 = double(temp.f32);
  // fmuls f26,f26,f13
  f26 = double(float(f26 * f13));
  // fmuls f25,f25,f13
  f25 = double(float(f25 * f13));
  // lwz r7,0(r11)
  r7 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r11) - 8).second_index_address());
  // fadds f9,f5,f9
  f9 = double(float(f5 + f9));
  // lfs f24,0(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 0);
  f24 = double(temp.f32);
  // fadds f8,f4,f8
  f8 = double(float(f4 + f8));
  // rlwinm r7,r7,4,0,27
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 4) & 0xFFFFFFF0;
  // fadds f7,f3,f7
  f7 = double(float(f3 + f7));
  // add r8,r9,r28
  r8 = r9 + r28;
  // fadds f6,f2,f6
  f6 = double(float(f2 + f6));
  // add r9,r9,r29
  r9 = r9 + r29;
  // add r7,r7,r29
  r7 = r7 + r29;
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // fadds f5,f28,f1
  f5 = double(float(f28 + f1));
  // fadds f4,f27,f31
  f4 = double(float(f27 + f31));
  // fadds f3,f26,f30
  f3 = double(float(f26 + f30));
  // fadds f2,f25,f29
  f2 = double(float(f25 + f29));
  // fmuls f9,f9,f12
  f9 = double(float(f9 * f12));
  // fmuls f8,f8,f12
  f8 = double(float(f8 * f12));
  // fmuls f7,f7,f12
  f7 = double(float(f7 * f12));
  // fmuls f6,f6,f12
  f6 = double(float(f6 * f12));
  // fmuls f5,f5,f11
  f5 = double(float(f5 * f11));
  // fmuls f4,f4,f11
  f4 = double(float(f4 * f11));
  // fmuls f3,f3,f11
  f3 = double(float(f3 * f11));
  // fmuls f2,f2,f11
  f2 = double(float(f2 * f11));
  // fadds f9,f5,f9
  f9 = double(float(f5 + f9));
  // fadds f8,f4,f8
  f8 = double(float(f4 + f8));
  // fadds f7,f3,f7
  f7 = double(float(f3 + f7));
  // fadds f6,f2,f6
  f6 = double(float(f2 + f6));
  // fmuls f3,f24,f0
  f3 = double(float(f24 * f0));
  // fmuls f5,f9,f10
  f5 = double(float(f9 * f10));
  // fmuls f4,f8,f10
  f4 = double(float(f8 * f10));
  // lfs f8,-4(r30)
  temp.u32 = guest::LoadU32(base, schema::BlitLinearWeights(base, uint32_t(r30) - 8).first_weight_address());
  f8 = double(temp.f32);
  // fmuls f7,f7,f10
  f7 = double(float(f7 * f10));
  // fmuls f6,f6,f10
  f6 = double(float(f6 * f10));
  // fmr f10,f0
  f10 = f0;
  // fmr f9,f13
  f9 = f13;
  // lfs f2,4(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 4);
  f2 = double(temp.f32);
  // addi r25,r25,1
  r25 = uint64_t(int64_t(int64_t(r25) + 1));
  // lfs f1,8(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 8);
  f1 = double(temp.f32);
  // fmuls f2,f2,f0
  f2 = double(float(f2 * f0));
  // lfs f31,12(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 12);
  f31 = double(temp.f32);
  // fmuls f1,f1,f0
  f1 = double(float(f1 * f0));
  // fmuls f0,f31,f0
  f0 = double(float(f31 * f0));
  // lfs f30,4(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 4);
  f30 = double(temp.f32);
  // lfs f31,0(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 0);
  f31 = double(temp.f32);
  // fmuls f30,f30,f13
  f30 = double(float(f30 * f13));
  // lfs f29,8(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 8);
  f29 = double(temp.f32);
  // fmuls f31,f31,f13
  f31 = double(float(f31 * f13));
  // lfs f28,12(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 12);
  f28 = double(temp.f32);
  // fmuls f29,f29,f13
  f29 = double(float(f29 * f13));
  // lfs f27,0(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 0);
  f27 = double(temp.f32);
  // fmuls f13,f28,f13
  f13 = double(float(f28 * f13));
  // lfs f26,4(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 4);
  f26 = double(temp.f32);
  // fmuls f28,f10,f27
  f28 = double(float(f10 * f27));
  // lfs f25,8(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8);
  f25 = double(temp.f32);
  // fmuls f27,f26,f10
  f27 = double(float(f26 * f10));
  // lfs f23,0(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 0);
  f23 = double(temp.f32);
  // fmuls f26,f25,f10
  f26 = double(float(f25 * f10));
  // lfs f24,12(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 12);
  f24 = double(temp.f32);
  // fmuls f25,f9,f23
  f25 = double(float(f9 * f23));
  // lfs f22,4(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 4);
  f22 = double(temp.f32);
  // fmuls f10,f24,f10
  f10 = double(float(f24 * f10));
  // lfs f21,8(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8);
  f21 = double(temp.f32);
  // fmuls f24,f22,f9
  f24 = double(float(f22 * f9));
  // lfs f20,12(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 12);
  f20 = double(temp.f32);
  // fmuls f23,f21,f9
  f23 = double(float(f21 * f9));
  // fmuls f9,f20,f9
  f9 = double(float(f20 * f9));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // fadds f3,f31,f3
  f3 = double(float(f31 + f3));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // fadds f2,f30,f2
  f2 = double(float(f30 + f2));
  // fadds f1,f29,f1
  f1 = double(float(f29 + f1));
  // fadds f13,f25,f28
  f13 = double(float(f25 + f28));
  // fadds f31,f24,f27
  f31 = double(float(f24 + f27));
  // fadds f30,f23,f26
  f30 = double(float(f23 + f26));
  // fadds f10,f9,f10
  f10 = double(float(f9 + f10));
  // fmuls f9,f3,f12
  f9 = double(float(f3 * f12));
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // fmuls f3,f2,f12
  f3 = double(float(f2 * f12));
  // fmuls f2,f1,f12
  f2 = double(float(f1 * f12));
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // fmuls f12,f31,f11
  f12 = double(float(f31 * f11));
  // fmuls f1,f30,f11
  f1 = double(float(f30 * f11));
  // fmuls f11,f10,f11
  f11 = double(float(f10 * f11));
  // fadds f13,f13,f9
  f13 = double(float(f13 + f9));
  // fadds f12,f12,f3
  f12 = double(float(f12 + f3));
  // fadds f10,f1,f2
  f10 = double(float(f1 + f2));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // fmuls f13,f13,f8
  f13 = double(float(f13 * f8));
  // fmuls f12,f12,f8
  f12 = double(float(f12 * f8));
  // fmuls f11,f10,f8
  f11 = double(float(f10 * f8));
  // fmuls f0,f0,f8
  f0 = double(float(f0 * f8));
  // fadds f13,f13,f5
  f13 = double(float(f13 + f5));
  // stfs f13,80(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // fadds f13,f12,f4
  f13 = double(float(f12 + f4));
  // stfs f13,84(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // fadds f13,f11,f7
  f13 = double(float(f11 + f7));
  // stfs f13,88(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // fadds f0,f0,f6
  f0 = double(float(f0 + f6));
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // lwz r9,0(r6)
  r9 = guest::LoadU32(base, uint32_t(r6) + 0);
  // lwz r8,4(r6)
  r8 = guest::LoadU32(base, uint32_t(r6) + 4);
  // lwz r7,8(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 8);
  // lwz r6,12(r6)
  r6 = guest::LoadU32(base, uint32_t(r6) + 12);
  // stw r9,0(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).red_address(), uint32_t(r9));
  // stw r8,4(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).green_address(), uint32_t(r8));
  // stw r7,8(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).blue_address(), uint32_t(r7));
  // stw r6,12(r10)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r10)).alpha_address(), uint32_t(r6));
  // lwz r9,4(r31)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // lwz r9,104(r9)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).width_address());
  // cmplw cr6,r25,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), uint32_t(r9), so);
  // blt cr6,0x8236b104
  if (cr6.lt) goto loc_8236B104;
loc_8236B37C:
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // mr r6,r17
  r6 = r17;
  // mr r5,r19
  r5 = r19;
  // mr r4,r21
  r4 = r21;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236B39C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // addi r21,r21,1
  r21 = uint64_t(int64_t(int64_t(r21) + 1));
  // addi r24,r24,16
  r24 = uint64_t(int64_t(int64_t(r24) + 16));
  // lwz r11,108(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r21,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), uint32_t(r11), so);
  // blt cr6,0x8236b01c
  if (cr6.lt) goto loc_8236B01C;
loc_8236B3B4:
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r31)).destination_address());
  // addi r19,r19,1
  r19 = uint64_t(int64_t(int64_t(r19) + 1));
  // addi r30,r30,16
  r30 = uint64_t(int64_t(int64_t(r30) + 16));
  // lwz r11,112(r10)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // cmplw cr6,r19,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r19), uint32_t(r11), so);
  // blt cr6,0x8236b000
  if (cr6.lt) goto loc_8236B000;
loc_8236B3CC:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x8236b3dc
  goto loc_8236B3DC;
loc_8236B3D4:
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // ori r31,r31,14
  r31 = r31 | 14;
loc_8236B3DC:
  // mr r3,r16
  r3 = r16;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B3E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r15
  r3 = r15;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B3EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r14
  r3 = r14;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B3F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r17
  r3 = r17;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B3FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r20
  r3 = r20;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B404, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_8236B408:
  // addi r1,r1,352
  r1 = uint64_t(int64_t(int64_t(r1) + 352));
  // addi r12,r1,-152
  r12 = uint64_t(int64_t(int64_t(r1) + -152));
  // bl 0x825a7944
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8236B718 game::Blitter::FreeWeights: lifted from generated/ by lift.py.
lifted::Result f8236B718(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8236B718 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r4
  r28 = r4;
  // mr r30,r3
  r30 = r3;
  // rlwinm. r11,r28,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8236b778
  if (cr0.eq) goto loc_8236B778;
  // addi r29,r30,-4
  r29 = uint64_t(int64_t(int64_t(r30) + -4));
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r29) + 4).frame_point_y_address());
  // mulli r10,r11,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // addic. r31,r11,-1
  ca = uint32_t(r11) > 0;
  r31 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // add r30,r10,r30
  r30 = r10 + r30;
  // blt 0x8236b760
  if (cr0.lt) goto loc_8236B760;
loc_8236B74C:
  // addi r30,r30,-12
  r30 = uint64_t(int64_t(int64_t(r30) + -12));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B758, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge 0x8236b74c
  if (!cr0.lt) goto loc_8236B74C;
loc_8236B760:
  // clrlwi. r11,r28,31
  r11 = uint32_t(r28) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8236b770
  if (cr0.eq) goto loc_8236B770;
  // mr r3,r29
  r3 = r29;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B770, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236B770:
  // mr r3,r29
  r3 = r29;
  // b 0x8236b794
  goto loc_8236B794;
loc_8236B778:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B780, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r28,31
  r11 = uint32_t(r28) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8236b790
  if (cr0.eq) goto loc_8236B790;
  // mr r3,r30
  r3 = r30;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236B790, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236B790:
  // mr r3,r30
  r3 = r30;
loc_8236B794:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8236B7A0 game::Blitter::FilterTriangle3D: lifted from generated/ by lift.py.
lifted::Result f8236B7A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8236B7A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-176(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -176, std::bit_cast<uint64_t>(f29));
  // stfd f30,-168(r1)
  guest::StoreU64(base, uint32_t(r1) + -168, std::bit_cast<uint64_t>(f30));
  // stfd f31,-160(r1)
  guest::StoreU64(base, uint32_t(r1) + -160, std::bit_cast<uint64_t>(f31));
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // mr r21,r22
  r21 = r22;
  // mr r20,r22
  r20 = r22;
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).source_address());
  // mr r23,r22
  r23 = r22;
  // lwz r11,8(r27)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).filter_address());
  // mr r15,r22
  r15 = r22;
  // stw r22,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r22));
  // not r9,r11
  r9 = ~r11;
  // lwz r3,104(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // rlwinm r5,r9,16,31,31
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 16) & 0x1;
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // not r9,r11
  r9 = ~r11;
  // not r11,r11
  r11 = ~r11;
  // rlwinm r31,r9,15,31,31
  r31 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 15) & 0x1;
  // rlwinm r30,r11,14,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 14) & 0x1;
  // lwz r4,104(r10)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // bl 0x8236b418
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::TriangleWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r19,r3
  r19 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r19), 0, so);
  // beq 0x8236bee0
  if (cr0.eq) goto loc_8236BEE0;
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // mr r5,r31
  r5 = r31;
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).source_address());
  // lwz r4,108(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r3,108(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // bl 0x8236b418
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::TriangleWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r21,r3
  r21 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r21), 0, so);
  // beq 0x8236bee0
  if (cr0.eq) goto loc_8236BEE0;
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // mr r5,r30
  r5 = r30;
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).source_address());
  // lwz r4,112(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).depth_address());
  // lwz r3,112(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // bl 0x8236b418
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::TriangleWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r20,r3
  r20 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r20), 0, so);
  // beq 0x8236bee0
  if (cr0.eq) goto loc_8236BEE0;
  // lwz r8,4(r27)
  r8 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // lis r11,5461
  r11 = uint64_t(int64_t(357892096));
  // lwz r10,0(r21)
  r10 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r21)).unknown_0_address());
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // ori r7,r11,21845
  r7 = r11 | 21845;
  // lwz r11,0(r20)
  r11 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r20)).unknown_0_address());
  // add r14,r10,r21
  r14 = r10 + r21;
  // lwz r9,0(r19)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r19)).unknown_0_address());
  // add r17,r11,r20
  r17 = r11 + r20;
  // lwz r31,112(r8)
  r31 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r8)).depth_address());
  // add r16,r9,r19
  r16 = r9 + r19;
  // cmplw cr6,r31,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r7), so);
  // stw r14,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r14));
  // mulli r11,r31,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(12))));
  // ble cr6,0x8236b88c
  if (!cr6.gt) goto loc_8236B88C;
  // mr r11,r30
  r11 = r30;
loc_8236B88C:
  // li r10,-5
  r10 = uint64_t(int64_t(-5));
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8236b8a0
  if (!cr6.gt) goto loc_8236B8A0;
  // mr r3,r30
  r3 = r30;
loc_8236B8A0:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236B8A4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8236b8e0
  if (cr0.eq) goto loc_8236B8E0;
  // addi r9,r3,4
  r9 = uint64_t(int64_t(int64_t(r3) + 4));
  // stw r31,0(r3)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r3)).red_address(), uint32_t(r31));
  // addic. r10,r31,-1
  ca = uint32_t(r31) > 0;
  r10 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // mr r11,r9
  r11 = r9;
  // blt 0x8236b8d8
  if (cr0.lt) goto loc_8236B8D8;
loc_8236B8C0:
  // stw r22,0(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 4).green_address(), uint32_t(r22));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r22,4(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 4).blue_address(), uint32_t(r22));
  // stw r22,8(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 4).alpha_address(), uint32_t(r22));
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // bge 0x8236b8c0
  if (!cr0.lt) goto loc_8236B8C0;
loc_8236B8D8:
  // mr r23,r9
  r23 = r9;
  // b 0x8236b8e4
  goto loc_8236B8E4;
loc_8236B8E0:
  // mr r23,r22
  r23 = r22;
loc_8236B8E4:
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x8236bed4
  if (cr6.eq) goto loc_8236BED4;
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).source_address());
  // lis r10,4095
  r10 = uint64_t(int64_t(268369920));
  // ori r18,r10,65535
  r18 = r10 | 65535;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r18
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r18), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236b90c
  if (!cr6.gt) goto loc_8236B90C;
  // mr r3,r30
  r3 = r30;
loc_8236B90C:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236B910, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r15,r3
  r15 = r3;
  // cmplwi cr6,r15,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r15), 0, so);
  // beq cr6,0x8236bed4
  if (cr6.eq) goto loc_8236BED4;
  // addi r7,r20,4
  r7 = uint64_t(int64_t(int64_t(r20) + 4));
  // mr r10,r7
  r10 = r7;
  // b 0x8236b95c
  goto loc_8236B95C;
loc_8236B928:
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r10) - 4).unknown_4_address());
  // addi r11,r10,4
  r11 = uint64_t(int64_t(int64_t(r10) + 4));
  // add r10,r9,r10
  r10 = r9 + r10;
  // b 0x8236b954
  goto loc_8236B954;
loc_8236B938:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r9,r9,r23
  r9 = r9 + r23;
  // lwz r8,8(r9)
  r8 = guest::LoadU32(base, uint32_t(r9) + 8);
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // stw r8,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r8));
loc_8236B954:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8236b938
  if (cr6.lt) goto loc_8236B938;
loc_8236B95C:
  // cmplw cr6,r10,r17
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r17), so);
  // blt cr6,0x8236b928
  if (cr6.lt) goto loc_8236B928;
  // mr r11,r7
  r11 = r7;
  // cmplw cr6,r11,r17
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r17), so);
  // bge cr6,0x8236becc
  if (!cr6.lt) goto loc_8236BECC;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f29,2612(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2612);
  f29 = double(temp.f32);
  // lfs f31,2608(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f31 = double(temp.f32);
  // lfs f30,7136(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 7136);
  f30 = double(temp.f32);
loc_8236B988:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 4).unknown_4_address());
  // addi r25,r11,4
  r25 = uint64_t(int64_t(int64_t(r11) + 4));
  // add r24,r10,r11
  r24 = r10 + r11;
  // mr r30,r25
  r30 = r25;
  // cmplw cr6,r30,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r24), so);
  // bge cr6,0x8236ba38
  if (!cr6.lt) goto loc_8236BA38;
  // lwz r29,80(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_8236B9A4:
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r30) - 8).first_index_address());
  // mulli r31,r11,12
  r31 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // lwzx r11,r31,r23
  r11 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r23));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8236ba2c
  if (!cr6.eq) goto loc_8236BA2C;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8236b9dc
  if (cr6.eq) goto loc_8236B9DC;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // stwx r11,r31,r23
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r23), uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
  // lwz r29,4(r29)
  r29 = guest::LoadU32(base, uint32_t(r29) + 4);
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
  // b 0x8236ba0c
  goto loc_8236BA0C;
loc_8236B9DC:
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mullw r11,r10,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // cmplw cr6,r11,r18
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r18), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236b9fc
  if (!cr6.gt) goto loc_8236B9FC;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_8236B9FC:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236BA00, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stwx r3,r31,r23
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r23), uint32_t(r3));
  // beq cr6,0x8236bed4
  if (cr6.eq) goto loc_8236BED4;
loc_8236BA0C:
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwzx r3,r31,r23
  r3 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r23));
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // mullw r11,r10,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // rlwinm r5,r11,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8236BA2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236BA2C:
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // cmplw cr6,r30,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r24), so);
  // blt cr6,0x8236b9a4
  if (cr6.lt) goto loc_8236B9A4;
loc_8236BA38:
  // addi r31,r21,4
  r31 = uint64_t(int64_t(int64_t(r21) + 4));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplw cr6,r31,r14
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r14), so);
  // bge cr6,0x8236bba8
  if (!cr6.lt) goto loc_8236BBA8;
  // addi r26,r19,4
  r26 = uint64_t(int64_t(int64_t(r19) + 4));
loc_8236BA4C:
  // lwz r3,0(r27)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).source_address());
  // mr r6,r15
  r6 = r15;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r31) - 4).unknown_4_address());
  // mr r5,r22
  r5 = r22;
  // mr r4,r29
  r4 = r29;
  // add r30,r11,r31
  r30 = r11 + r31;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236BA74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r26
  r4 = r26;
  // cmplw cr6,r4,r16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r16), so);
  // bge cr6,0x8236bb98
  if (!cr6.lt) goto loc_8236BB98;
  // addi r8,r15,8
  r8 = uint64_t(int64_t(int64_t(r15) + 8));
loc_8236BA84:
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r4) - 4).unknown_4_address());
  // mr r6,r25
  r6 = r25;
  // add r5,r11,r4
  r5 = r11 + r4;
  // cmplw cr6,r6,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r24), so);
  // bge cr6,0x8236bb88
  if (!cr6.lt) goto loc_8236BB88;
  // addi r28,r31,4
  r28 = uint64_t(int64_t(int64_t(r31) + 4));
loc_8236BA9C:
  // mr r7,r28
  r7 = r28;
  // cmplw cr6,r7,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r30), so);
  // bge cr6,0x8236bb7c
  if (!cr6.lt) goto loc_8236BB7C;
  // addi r3,r4,4
  r3 = uint64_t(int64_t(int64_t(r4) + 4));
loc_8236BAAC:
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // mr r11,r3
  r11 = r3;
  // lwz r9,0(r6)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r6) - 8).first_index_address());
  // lwz r14,0(r7)
  r14 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r7) - 8).first_index_address());
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // lwz r10,104(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // lwzx r10,r9,r23
  r10 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r23));
  // lwz r9,84(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 84);
  // mullw r9,r9,r14
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r14))));
  // rlwinm r9,r9,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // b 0x8236bb64
  goto loc_8236BB64;
loc_8236BAE0:
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_weight_address());
  f0 = double(temp.f32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,4(r7)
  temp.u32 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r7) - 8).first_weight_address());
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // lfs f13,4(r6)
  temp.u32 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r6) - 8).first_weight_address());
  f13 = double(temp.f32);
  // rlwinm r9,r9,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // lfs f12,-8(r8)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r8) - 8).red_address());
  f12 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // lfsx f13,r9,r10
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r10));
  f13 = double(temp.f32);
  // fmadds f13,f12,f0,f13
  f13 = double(float(std::fma(f12, f0, f13)));
  // stfsx f13,r9,r10
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r10), temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,-4(r8)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r8) - 8).green_address());
  f13 = double(temp.f32);
  // rlwinm r9,r9,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // add r9,r9,r10
  r9 = r9 + r10;
  // lfs f12,4(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 4);
  f12 = double(temp.f32);
  // fmadds f13,f13,f0,f12
  f13 = double(float(std::fma(f13, f0, f12)));
  // stfs f13,4(r9)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r9) + 4, temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,0(r8)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r8) - 8).blue_address());
  f13 = double(temp.f32);
  // rlwinm r9,r9,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // add r9,r9,r10
  r9 = r9 + r10;
  // lfs f12,8(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8);
  f12 = double(temp.f32);
  // fmadds f13,f13,f0,f12
  f13 = double(float(std::fma(f13, f0, f12)));
  // stfs f13,8(r9)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r9) + 8, temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,4(r8)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r8) - 8).alpha_address());
  f13 = double(temp.f32);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // rlwinm r9,r9,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // add r9,r9,r10
  r9 = r9 + r10;
  // lfs f12,12(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 12);
  f12 = double(temp.f32);
  // fmadds f0,f0,f13,f12
  f0 = double(float(std::fma(f0, f13, f12)));
  // stfs f0,12(r9)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + 12, temp.u32);
loc_8236BB64:
  // cmplw cr6,r11,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r5), so);
  // blt cr6,0x8236bae0
  if (cr6.lt) goto loc_8236BAE0;
  // addi r7,r7,8
  r7 = uint64_t(int64_t(int64_t(r7) + 8));
  // cmplw cr6,r7,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r30), so);
  // blt cr6,0x8236baac
  if (cr6.lt) goto loc_8236BAAC;
  // lwz r14,88(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 88);
loc_8236BB7C:
  // addi r6,r6,8
  r6 = uint64_t(int64_t(int64_t(r6) + 8));
  // cmplw cr6,r6,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r24), so);
  // blt cr6,0x8236ba9c
  if (cr6.lt) goto loc_8236BA9C;
loc_8236BB88:
  // addi r8,r8,16
  r8 = uint64_t(int64_t(int64_t(r8) + 16));
  // mr r4,r5
  r4 = r5;
  // cmplw cr6,r5,r16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r16), so);
  // blt cr6,0x8236ba84
  if (cr6.lt) goto loc_8236BA84;
loc_8236BB98:
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // mr r31,r30
  r31 = r30;
  // cmplw cr6,r30,r14
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r14), so);
  // blt cr6,0x8236ba4c
  if (cr6.lt) goto loc_8236BA4C;
loc_8236BBA8:
  // mr r29,r25
  r29 = r25;
  // b 0x8236beb0
  goto loc_8236BEB0;
loc_8236BBB0:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r29) - 8).first_index_address());
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r30,r11,r23
  r30 = r11 + r23;
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,8(r30)
  guest::StoreU32(base, uint32_t(r30) + 8, uint32_t(r11));
  // bne 0x8236beac
  if (!cr0.eq) goto loc_8236BEAC;
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // ble cr6,0x8236bea0
  if (!cr6.gt) goto loc_8236BEA0;
loc_8236BBE0:
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).source_address());
  // mullw r9,r11,r31
  r9 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r31))));
  // lwz r8,0(r30)
  r8 = guest::LoadU32(base, uint32_t(r30) + 0);
  // rlwinm r9,r9,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // lwz r10,8(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).kind_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // add r9,r9,r8
  r9 = r9 + r8;
  // beq cr6,0x8236bd9c
  if (cr6.eq) goto loc_8236BD9C;
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // beq cr6,0x8236bcd8
  if (cr6.eq) goto loc_8236BCD8;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x8236be5c
  if (!cr6.eq) goto loc_8236BE5C;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8236be5c
  if (cr6.eq) goto loc_8236BE5C;
  // addi r11,r9,8
  r11 = uint64_t(int64_t(int64_t(r9) + 8));
loc_8236BC24:
  // lfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -8);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236bc38
  if (!cr6.lt) goto loc_8236BC38;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236bc44
  goto loc_8236BC44;
loc_8236BC38:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236bc44
  if (cr6.lt) goto loc_8236BC44;
  // fmr f0,f31
  f0 = f31;
loc_8236BC44:
  // stfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + -8, temp.u32);
  // lfs f0,-4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -4);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236bc5c
  if (!cr6.lt) goto loc_8236BC5C;
  // fmr f13,f30
  f13 = f30;
  // b 0x8236bc70
  goto loc_8236BC70;
loc_8236BC5C:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // bge cr6,0x8236bc6c
  if (!cr6.lt) goto loc_8236BC6C;
  // fmr f13,f0
  f13 = f0;
  // b 0x8236bc70
  goto loc_8236BC70;
loc_8236BC6C:
  // fmr f13,f31
  lifted::ScalarFloatMode();
  f13 = f31;
loc_8236BC70:
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // stfs f13,-4(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r11) + -4, temp.u32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236bc88
  if (!cr6.lt) goto loc_8236BC88;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236bc94
  goto loc_8236BC94;
loc_8236BC88:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236bc94
  if (cr6.lt) goto loc_8236BC94;
  // fmr f0,f31
  f0 = f31;
loc_8236BC94:
  // stfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // lfs f0,4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236bcac
  if (!cr6.lt) goto loc_8236BCAC;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236bcb8
  goto loc_8236BCB8;
loc_8236BCAC:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236bcb8
  if (cr6.lt) goto loc_8236BCB8;
  // fmr f0,f31
  f0 = f31;
loc_8236BCB8:
  // stfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 4, temp.u32);
  // lwz r9,4(r27)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r9,104(r9)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).width_address());
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x8236bc24
  if (cr6.lt) goto loc_8236BC24;
  // b 0x8236be5c
  goto loc_8236BE5C;
loc_8236BCD8:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8236be5c
  if (cr6.eq) goto loc_8236BE5C;
  // addi r11,r9,8
  r11 = uint64_t(int64_t(int64_t(r9) + 8));
loc_8236BCE8:
  // lfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236bcfc
  if (!cr6.lt) goto loc_8236BCFC;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236bd08
  goto loc_8236BD08;
loc_8236BCFC:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236bd08
  if (cr6.lt) goto loc_8236BD08;
  // fmr f0,f31
  f0 = f31;
loc_8236BD08:
  // stfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address(), temp.u32);
  // lfs f0,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236bd20
  if (!cr6.lt) goto loc_8236BD20;
  // fmr f13,f30
  f13 = f30;
  // b 0x8236bd34
  goto loc_8236BD34;
loc_8236BD20:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // bge cr6,0x8236bd30
  if (!cr6.lt) goto loc_8236BD30;
  // fmr f13,f0
  f13 = f0;
  // b 0x8236bd34
  goto loc_8236BD34;
loc_8236BD30:
  // fmr f13,f31
  lifted::ScalarFloatMode();
  f13 = f31;
loc_8236BD34:
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f0 = double(temp.f32);
  // stfs f13,-4(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address(), temp.u32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236bd4c
  if (!cr6.lt) goto loc_8236BD4C;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236bd58
  goto loc_8236BD58;
loc_8236BD4C:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236bd58
  if (cr6.lt) goto loc_8236BD58;
  // fmr f0,f31
  f0 = f31;
loc_8236BD58:
  // stfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address(), temp.u32);
  // lfs f0,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236bd70
  if (!cr6.lt) goto loc_8236BD70;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236bd7c
  goto loc_8236BD7C;
loc_8236BD70:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236bd7c
  if (cr6.lt) goto loc_8236BD7C;
  // fmr f0,f31
  f0 = f31;
loc_8236BD7C:
  // stfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address(), temp.u32);
  // lwz r9,4(r27)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r9,104(r9)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).width_address());
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x8236bce8
  if (cr6.lt) goto loc_8236BCE8;
  // b 0x8236be5c
  goto loc_8236BE5C;
loc_8236BD9C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8236be5c
  if (cr6.eq) goto loc_8236BE5C;
  // addi r11,r9,8
  r11 = uint64_t(int64_t(int64_t(r9) + 8));
loc_8236BDAC:
  // lfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236bdc0
  if (!cr6.lt) goto loc_8236BDC0;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236bdcc
  goto loc_8236BDCC;
loc_8236BDC0:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236bdcc
  if (cr6.lt) goto loc_8236BDCC;
  // fmr f0,f31
  f0 = f31;
loc_8236BDCC:
  // stfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address(), temp.u32);
  // lfs f0,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236bde4
  if (!cr6.lt) goto loc_8236BDE4;
  // fmr f13,f29
  f13 = f29;
  // b 0x8236bdf8
  goto loc_8236BDF8;
loc_8236BDE4:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // bge cr6,0x8236bdf4
  if (!cr6.lt) goto loc_8236BDF4;
  // fmr f13,f0
  f13 = f0;
  // b 0x8236bdf8
  goto loc_8236BDF8;
loc_8236BDF4:
  // fmr f13,f31
  lifted::ScalarFloatMode();
  f13 = f31;
loc_8236BDF8:
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f0 = double(temp.f32);
  // stfs f13,-4(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address(), temp.u32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236be10
  if (!cr6.lt) goto loc_8236BE10;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236be1c
  goto loc_8236BE1C;
loc_8236BE10:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236be1c
  if (cr6.lt) goto loc_8236BE1C;
  // fmr f0,f31
  f0 = f31;
loc_8236BE1C:
  // stfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address(), temp.u32);
  // lfs f0,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236be34
  if (!cr6.lt) goto loc_8236BE34;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236be40
  goto loc_8236BE40;
loc_8236BE34:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236be40
  if (cr6.lt) goto loc_8236BE40;
  // fmr f0,f31
  f0 = f31;
loc_8236BE40:
  // stfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address(), temp.u32);
  // lwz r9,4(r27)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r9,104(r9)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).width_address());
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x8236bdac
  if (cr6.lt) goto loc_8236BDAC;
loc_8236BE5C:
  // lwz r3,4(r27)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // mr r4,r31
  r4 = r31;
  // lwz r10,0(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r5,0(r29)
  r5 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r29) - 8).first_index_address());
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // lwz r9,0(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // mullw r11,r11,r31
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r31))));
  // lwz r9,8(r9)
  r9 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r9)).encode_row_address());
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r6,r11,r10
  r6 = r11 + r10;
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236BE8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r27)).destination_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lwz r10,108(r11)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8236bbe0
  if (cr6.lt) goto loc_8236BBE0;
loc_8236BEA0:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // stw r30,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r30));
  // stw r11,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r11));
loc_8236BEAC:
  // addi r29,r29,8
  r29 = uint64_t(int64_t(int64_t(r29) + 8));
loc_8236BEB0:
  // cmplw cr6,r29,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r24), so);
  // blt cr6,0x8236bbb0
  if (cr6.lt) goto loc_8236BBB0;
  // addi r22,r22,1
  r22 = uint64_t(int64_t(int64_t(r22) + 1));
  // mr r11,r24
  r11 = r24;
  // cmplw cr6,r24,r17
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r17), so);
  // blt cr6,0x8236b988
  if (cr6.lt) goto loc_8236B988;
  // li r22,0
  r22 = uint64_t(int64_t(0));
loc_8236BECC:
  // mr r31,r22
  r31 = r22;
  // b 0x8236bee8
  goto loc_8236BEE8;
loc_8236BED4:
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // ori r31,r31,14
  r31 = r31 | 14;
  // b 0x8236bee8
  goto loc_8236BEE8;
loc_8236BEE0:
  // lis r31,-32768
  r31 = uint64_t(int64_t(-2147483648));
  // ori r31,r31,16389
  r31 = r31 | 16389;
loc_8236BEE8:
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x8236befc
  if (cr6.eq) goto loc_8236BEFC;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r23
  r3 = r23;
  // bl 0x8236b718
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::FreeWeights(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8236BEFC:
  // mr r3,r20
  r3 = r20;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236BF04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r21
  r3 = r21;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236BF0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r19
  r3 = r19;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236BF14, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r15
  r3 = r15;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236BF1C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236BF24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // lfd f29,-176(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -176)));
  // lfd f30,-168(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -168)));
  // lfd f31,-160(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -160)));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8236BF40 game::Blitter::FilterTriangle2D: lifted from generated/ by lift.py.
lifted::Result f8236BF40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8236BF40 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e58
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-160(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -160, std::bit_cast<uint64_t>(f29));
  // stfd f30,-152(r1)
  guest::StoreU64(base, uint32_t(r1) + -152, std::bit_cast<uint64_t>(f30));
  // stfd f31,-144(r1)
  guest::StoreU64(base, uint32_t(r1) + -144, std::bit_cast<uint64_t>(f31));
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r3
  r28 = r3;
  // li r18,0
  r18 = uint64_t(int64_t(0));
  // mr r20,r18
  r20 = r18;
  // mr r25,r18
  r25 = r18;
  // lwz r10,4(r28)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // mr r19,r18
  r19 = r18;
  // mr r16,r18
  r16 = r18;
  // lwz r11,112(r10)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).depth_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8236c614
  if (!cr6.eq) goto loc_8236C614;
  // lwz r9,0(r28)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).source_address());
  // lwz r11,112(r9)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).depth_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8236c614
  if (!cr6.eq) goto loc_8236C614;
  // lwz r11,8(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).filter_address());
  // lwz r4,104(r10)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // not r10,r11
  r10 = ~r11;
  // lwz r3,104(r9)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).width_address());
  // not r11,r11
  r11 = ~r11;
  // rlwinm r5,r10,16,31,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 16) & 0x1;
  // rlwinm r31,r11,15,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 15) & 0x1;
  // bl 0x8236b418
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::TriangleWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r22,r3
  r22 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r22), 0, so);
  // beq 0x8236c5d0
  if (cr0.eq) goto loc_8236C5D0;
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // mr r5,r31
  r5 = r31;
  // lwz r10,0(r28)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).source_address());
  // lwz r4,108(r11)
  r4 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).height_address());
  // lwz r3,108(r10)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).height_address());
  // bl 0x8236b418
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::TriangleWeights(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r20,r3
  r20 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r20), 0, so);
  // beq 0x8236c5d0
  if (cr0.eq) goto loc_8236C5D0;
  // lwz r9,4(r28)
  r9 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // lis r11,5461
  r11 = uint64_t(int64_t(357892096));
  // lwz r10,0(r22)
  r10 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r22)).unknown_0_address());
  // li r17,-1
  r17 = uint64_t(int64_t(-1));
  // ori r8,r11,21845
  r8 = r11 | 21845;
  // lwz r11,0(r20)
  r11 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r20)).unknown_0_address());
  // add r21,r10,r22
  r21 = r10 + r22;
  // add r23,r11,r20
  r23 = r11 + r20;
  // lwz r31,108(r9)
  r31 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r9)).height_address());
  // cmplw cr6,r31,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r8), so);
  // mulli r11,r31,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(12))));
  // ble cr6,0x8236c00c
  if (!cr6.gt) goto loc_8236C00C;
  // mr r11,r17
  r11 = r17;
loc_8236C00C:
  // li r10,-5
  r10 = uint64_t(int64_t(-5));
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8236c020
  if (!cr6.gt) goto loc_8236C020;
  // mr r3,r17
  r3 = r17;
loc_8236C020:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236C024, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8236c060
  if (cr0.eq) goto loc_8236C060;
  // addi r9,r3,4
  r9 = uint64_t(int64_t(int64_t(r3) + 4));
  // stw r31,0(r3)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r3)).red_address(), uint32_t(r31));
  // addic. r10,r31,-1
  ca = uint32_t(r31) > 0;
  r10 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // mr r11,r9
  r11 = r9;
  // blt 0x8236c058
  if (cr0.lt) goto loc_8236C058;
loc_8236C040:
  // stw r18,0(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 4).green_address(), uint32_t(r18));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r18,4(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 4).blue_address(), uint32_t(r18));
  // stw r18,8(r11)
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 4).alpha_address(), uint32_t(r18));
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // bge 0x8236c040
  if (!cr0.lt) goto loc_8236C040;
loc_8236C058:
  // mr r25,r9
  r25 = r9;
  // b 0x8236c064
  goto loc_8236C064;
loc_8236C060:
  // mr r25,r18
  r25 = r18;
loc_8236C064:
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x8236c5c4
  if (cr6.eq) goto loc_8236C5C4;
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).source_address());
  // lis r10,4095
  r10 = uint64_t(int64_t(268369920));
  // ori r24,r10,65535
  r24 = r10 | 65535;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r24), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236c08c
  if (!cr6.gt) goto loc_8236C08C;
  // mr r3,r17
  r3 = r17;
loc_8236C08C:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236C090, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r16,r3
  r16 = r3;
  // cmplwi cr6,r16,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r16), 0, so);
  // beq cr6,0x8236c5c4
  if (cr6.eq) goto loc_8236C5C4;
  // addi r7,r20,4
  r7 = uint64_t(int64_t(int64_t(r20) + 4));
  // mr r10,r7
  r10 = r7;
  // b 0x8236c0dc
  goto loc_8236C0DC;
loc_8236C0A8:
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r10) - 4).unknown_4_address());
  // addi r11,r10,4
  r11 = uint64_t(int64_t(int64_t(r10) + 4));
  // add r10,r9,r10
  r10 = r9 + r10;
  // b 0x8236c0d4
  goto loc_8236C0D4;
loc_8236C0B8:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r9,r9,r25
  r9 = r9 + r25;
  // lwz r8,8(r9)
  r8 = guest::LoadU32(base, uint32_t(r9) + 8);
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // stw r8,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r8));
loc_8236C0D4:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8236c0b8
  if (cr6.lt) goto loc_8236C0B8;
loc_8236C0DC:
  // cmplw cr6,r10,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r23), so);
  // blt cr6,0x8236c0a8
  if (cr6.lt) goto loc_8236C0A8;
  // mr r11,r7
  r11 = r7;
  // mr r26,r18
  r26 = r18;
  // cmplw cr6,r11,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r23), so);
  // bge cr6,0x8236c5bc
  if (!cr6.lt) goto loc_8236C5BC;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f29,2612(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2612);
  f29 = double(temp.f32);
  // lfs f31,2608(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f31 = double(temp.f32);
  // lfs f30,7136(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 7136);
  f30 = double(temp.f32);
loc_8236C10C:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 4).unknown_4_address());
  // addi r29,r11,4
  r29 = uint64_t(int64_t(int64_t(r11) + 4));
  // add r27,r10,r11
  r27 = r10 + r11;
  // mr r30,r29
  r30 = r29;
  // b 0x8236c194
  goto loc_8236C194;
loc_8236C120:
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r30) - 8).first_index_address());
  // mulli r31,r11,12
  r31 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // lwzx r11,r31,r25
  r11 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r25));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8236c190
  if (!cr6.eq) goto loc_8236C190;
  // cmplwi cr6,r19,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r19), 0, so);
  // beq cr6,0x8236c150
  if (cr6.eq) goto loc_8236C150;
  // lwz r11,0(r19)
  r11 = guest::LoadU32(base, uint32_t(r19) + 0);
  // stwx r11,r31,r25
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r25), uint32_t(r11));
  // stw r18,0(r19)
  guest::StoreU32(base, uint32_t(r19) + 0, uint32_t(r18));
  // lwz r19,4(r19)
  r19 = guest::LoadU32(base, uint32_t(r19) + 4);
  // b 0x8236c178
  goto loc_8236C178;
loc_8236C150:
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplw cr6,r11,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r24), so);
  // rlwinm r3,r11,4,0,27
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // ble cr6,0x8236c168
  if (!cr6.gt) goto loc_8236C168;
  // mr r3,r17
  r3 = r17;
loc_8236C168:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8236C16C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stwx r3,r31,r25
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r25), uint32_t(r3));
  // beq cr6,0x8236c5c4
  if (cr6.eq) goto loc_8236C5C4;
loc_8236C178:
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwzx r3,r31,r25
  r3 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r25));
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // rlwinm r5,r11,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8236C190, r31)); r3 = o.r3; f1 = o.f1; }
loc_8236C190:
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
loc_8236C194:
  // cmplw cr6,r30,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r27), so);
  // blt cr6,0x8236c120
  if (cr6.lt) goto loc_8236C120;
  // lwz r3,0(r28)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).source_address());
  // mr r6,r16
  r6 = r16;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r26
  r4 = r26;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236C1BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r22,4
  r11 = uint64_t(int64_t(int64_t(r22) + 4));
  // cmplw cr6,r11,r21
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r21), so);
  // bge cr6,0x8236c5a4
  if (!cr6.lt) goto loc_8236C5A4;
  // addi r9,r16,8
  r9 = uint64_t(int64_t(int64_t(r16) + 8));
loc_8236C1CC:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 4).unknown_4_address());
  // mr r7,r29
  r7 = r29;
  // add r6,r10,r11
  r6 = r10 + r11;
  // cmplw cr6,r7,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r27), so);
  // bge cr6,0x8236c288
  if (!cr6.lt) goto loc_8236C288;
  // addi r5,r11,4
  r5 = uint64_t(int64_t(int64_t(r11) + 4));
loc_8236C1E4:
  // lwz r10,0(r7)
  r10 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r7) - 8).first_index_address());
  // mr r11,r5
  r11 = r5;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // lwzx r10,r10,r25
  r10 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r25));
  // b 0x8236c274
  goto loc_8236C274;
loc_8236C1F8:
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_weight_address());
  f0 = double(temp.f32);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,4(r7)
  temp.u32 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r7) - 8).first_weight_address());
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // rlwinm r8,r8,4,0,27
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // lfs f13,-8(r9)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r9) - 8).red_address());
  f13 = double(temp.f32);
  // lfsx f12,r8,r10
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + uint32_t(r10));
  f12 = double(temp.f32);
  // fmadds f13,f13,f0,f12
  f13 = double(float(std::fma(f13, f0, f12)));
  // stfsx f13,r8,r10
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r10), temp.u32);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,-4(r9)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r9) - 8).green_address());
  f13 = double(temp.f32);
  // rlwinm r8,r8,4,0,27
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // add r8,r8,r10
  r8 = r8 + r10;
  // lfs f12,4(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 4);
  f12 = double(temp.f32);
  // fmadds f13,f13,f0,f12
  f13 = double(float(std::fma(f13, f0, f12)));
  // stfs f13,4(r8)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r8) + 4, temp.u32);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,0(r9)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r9) - 8).blue_address());
  f13 = double(temp.f32);
  // rlwinm r8,r8,4,0,27
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // add r8,r8,r10
  r8 = r8 + r10;
  // lfs f12,8(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 8);
  f12 = double(temp.f32);
  // fmadds f13,f13,f0,f12
  f13 = double(float(std::fma(f13, f0, f12)));
  // stfs f13,8(r8)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r8) + 8, temp.u32);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r11) - 8).first_index_address());
  // lfs f13,4(r9)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r9) - 8).alpha_address());
  f13 = double(temp.f32);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // rlwinm r8,r8,4,0,27
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // add r8,r8,r10
  r8 = r8 + r10;
  // lfs f12,12(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 12);
  f12 = double(temp.f32);
  // fmadds f0,f13,f0,f12
  f0 = double(float(std::fma(f13, f0, f12)));
  // stfs f0,12(r8)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r8) + 12, temp.u32);
loc_8236C274:
  // cmplw cr6,r11,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r6), so);
  // blt cr6,0x8236c1f8
  if (cr6.lt) goto loc_8236C1F8;
  // addi r7,r7,8
  r7 = uint64_t(int64_t(int64_t(r7) + 8));
  // cmplw cr6,r7,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r27), so);
  // blt cr6,0x8236c1e4
  if (cr6.lt) goto loc_8236C1E4;
loc_8236C288:
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // mr r11,r6
  r11 = r6;
  // cmplw cr6,r6,r21
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r21), so);
  // blt cr6,0x8236c1cc
  if (cr6.lt) goto loc_8236C1CC;
  // b 0x8236c5a4
  goto loc_8236C5A4;
loc_8236C29C:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r29) - 8).first_index_address());
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r31,r11,r25
  r31 = r11 + r25;
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bne 0x8236c5a0
  if (!cr0.eq) goto loc_8236C5A0;
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).source_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).kind_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x8236c498
  if (cr6.eq) goto loc_8236C498;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x8236c3b8
  if (cr6.eq) goto loc_8236C3B8;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x8236c574
  if (!cr6.eq) goto loc_8236C574;
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // mr r9,r18
  r9 = r18;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236c574
  if (!cr6.gt) goto loc_8236C574;
  // mr r11,r18
  r11 = r18;
loc_8236C2F0:
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lfsx f0,r10,r11
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236c308
  if (!cr6.lt) goto loc_8236C308;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236c314
  goto loc_8236C314;
loc_8236C308:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c314
  if (cr6.lt) goto loc_8236C314;
  // fmr f0,f31
  f0 = f31;
loc_8236C314:
  // stfsx f0,r10,r11
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,4(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 4);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236c334
  if (!cr6.lt) goto loc_8236C334;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236c340
  goto loc_8236C340;
loc_8236C334:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c340
  if (cr6.lt) goto loc_8236C340;
  // fmr f0,f31
  f0 = f31;
loc_8236C340:
  // stfs f0,4(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 4, temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,8(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 8);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236c360
  if (!cr6.lt) goto loc_8236C360;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236c36c
  goto loc_8236C36C;
loc_8236C360:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c36c
  if (cr6.lt) goto loc_8236C36C;
  // fmr f0,f31
  f0 = f31;
loc_8236C36C:
  // stfs f0,8(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 8, temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,12(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 12);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236c38c
  if (!cr6.lt) goto loc_8236C38C;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236c398
  goto loc_8236C398;
loc_8236C38C:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c398
  if (cr6.lt) goto loc_8236C398;
  // fmr f0,f31
  f0 = f31;
loc_8236C398:
  // stfs f0,12(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 12, temp.u32);
  // lwz r10,4(r28)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r10,104(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // blt cr6,0x8236c2f0
  if (cr6.lt) goto loc_8236C2F0;
  // b 0x8236c574
  goto loc_8236C574;
loc_8236C3B8:
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // mr r9,r18
  r9 = r18;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236c574
  if (!cr6.gt) goto loc_8236C574;
  // mr r11,r18
  r11 = r18;
loc_8236C3D0:
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lfsx f0,r10,r11
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236c3e8
  if (!cr6.lt) goto loc_8236C3E8;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236c3f4
  goto loc_8236C3F4;
loc_8236C3E8:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c3f4
  if (cr6.lt) goto loc_8236C3F4;
  // fmr f0,f31
  f0 = f31;
loc_8236C3F4:
  // stfsx f0,r10,r11
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,4(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 4);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236c414
  if (!cr6.lt) goto loc_8236C414;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236c420
  goto loc_8236C420;
loc_8236C414:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c420
  if (cr6.lt) goto loc_8236C420;
  // fmr f0,f31
  f0 = f31;
loc_8236C420:
  // stfs f0,4(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 4, temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,8(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 8);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // bge cr6,0x8236c440
  if (!cr6.lt) goto loc_8236C440;
  // fmr f0,f30
  f0 = f30;
  // b 0x8236c44c
  goto loc_8236C44C;
loc_8236C440:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c44c
  if (cr6.lt) goto loc_8236C44C;
  // fmr f0,f31
  f0 = f31;
loc_8236C44C:
  // stfs f0,8(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 8, temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,12(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 12);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236c46c
  if (!cr6.lt) goto loc_8236C46C;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236c478
  goto loc_8236C478;
loc_8236C46C:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c478
  if (cr6.lt) goto loc_8236C478;
  // fmr f0,f31
  f0 = f31;
loc_8236C478:
  // stfs f0,12(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 12, temp.u32);
  // lwz r10,4(r28)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r10,104(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // blt cr6,0x8236c3d0
  if (cr6.lt) goto loc_8236C3D0;
  // b 0x8236c574
  goto loc_8236C574;
loc_8236C498:
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // mr r9,r18
  r9 = r18;
  // lwz r11,104(r11)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r11)).width_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8236c574
  if (!cr6.gt) goto loc_8236C574;
  // mr r11,r18
  r11 = r18;
loc_8236C4B0:
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lfsx f0,r10,r11
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236c4c8
  if (!cr6.lt) goto loc_8236C4C8;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236c4d4
  goto loc_8236C4D4;
loc_8236C4C8:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c4d4
  if (cr6.lt) goto loc_8236C4D4;
  // fmr f0,f31
  f0 = f31;
loc_8236C4D4:
  // stfsx f0,r10,r11
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,4(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 4);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236c4f4
  if (!cr6.lt) goto loc_8236C4F4;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236c500
  goto loc_8236C500;
loc_8236C4F4:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c500
  if (cr6.lt) goto loc_8236C500;
  // fmr f0,f31
  f0 = f31;
loc_8236C500:
  // stfs f0,4(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 4, temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,8(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 8);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236c520
  if (!cr6.lt) goto loc_8236C520;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236c52c
  goto loc_8236C52C;
loc_8236C520:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c52c
  if (cr6.lt) goto loc_8236C52C;
  // fmr f0,f31
  f0 = f31;
loc_8236C52C:
  // stfs f0,8(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 8, temp.u32);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lfs f0,12(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 12);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // bge cr6,0x8236c54c
  if (!cr6.lt) goto loc_8236C54C;
  // fmr f0,f29
  f0 = f29;
  // b 0x8236c558
  goto loc_8236C558;
loc_8236C54C:
  // fcmpu cr6,f0,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8236c558
  if (cr6.lt) goto loc_8236C558;
  // fmr f0,f31
  f0 = f31;
loc_8236C558:
  // stfs f0,12(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 12, temp.u32);
  // lwz r10,4(r28)
  r10 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r10,104(r10)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r10)).width_address());
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // blt cr6,0x8236c4b0
  if (cr6.lt) goto loc_8236C4B0;
loc_8236C574:
  // lwz r3,4(r28)
  r3 = guest::LoadU32(base, schema::Blitter(base, uint32_t(r28)).destination_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,0(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r4,0(r29)
  r4 = guest::LoadU32(base, schema::BlitTriangleWeights(base, uint32_t(r29) - 8).first_index_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).vtable_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8236C594, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r19
  r11 = r19;
  // mr r19,r31
  r19 = r31;
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
loc_8236C5A0:
  // addi r29,r29,8
  r29 = uint64_t(int64_t(int64_t(r29) + 8));
loc_8236C5A4:
  // cmplw cr6,r29,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r27), so);
  // blt cr6,0x8236c29c
  if (cr6.lt) goto loc_8236C29C;
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // mr r11,r27
  r11 = r27;
  // cmplw cr6,r27,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r23), so);
  // blt cr6,0x8236c10c
  if (cr6.lt) goto loc_8236C10C;
loc_8236C5BC:
  // mr r31,r18
  r31 = r18;
  // b 0x8236c5d8
  goto loc_8236C5D8;
loc_8236C5C4:
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // ori r31,r31,14
  r31 = r31 | 14;
  // b 0x8236c5d8
  goto loc_8236C5D8;
loc_8236C5D0:
  // lis r31,-32768
  r31 = uint64_t(int64_t(-2147483648));
  // ori r31,r31,16389
  r31 = r31 | 16389;
loc_8236C5D8:
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x8236c5ec
  if (cr6.eq) goto loc_8236C5EC;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r25
  r3 = r25;
  // bl 0x8236b718
  // (left out: dead in C++)
  { const lifted::Result o = game::Blitter::FreeWeights(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8236C5EC:
  // mr r3,r20
  r3 = r20;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236C5F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r22
  r3 = r22;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236C5FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r16
  r3 = r16;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236C604, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8236C60C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // b 0x8236c61c
  goto loc_8236C61C;
loc_8236C614:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
loc_8236C61C:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfd f29,-160(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -160)));
  // lfd f30,-152(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -152)));
  // lfd f31,-144(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -144)));
  // b 0x825a6ea8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82379360: lifted from generated/ by lift.py.
uint64_t f82379360(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82379360 (lifted)");
  // lwz r3,120(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 120);
  // blr
  return r3;
}

// sub_82379368: lifted from generated/ by lift.py.
uint64_t f82379368(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82379368 (lifted)");
  // addi r3,r3,204
  r3 = uint64_t(int64_t(int64_t(r3) + 204));
  // blr
  return r3;
}

// sub_823EA0C0 game::ImageCodec::ConvertRange: lifted from generated/ by lift.py.
uint64_t f823EA0C0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_823EA0C0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).kind_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x823ea524
  if (cr6.eq) goto loc_823EA524;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x823ea2fc
  if (cr6.eq) goto loc_823EA2FC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x823ea764
  if (!cr6.eq) goto loc_823EA764;
  // lwz r11,84(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).convert_from_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x823ea26c
  if (cr6.eq) goto loc_823EA26C;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x823ea1e8
  if (cr6.eq) goto loc_823EA1E8;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x823ea764
  if (!cr6.eq) goto loc_823EA764;
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // subfic r7,r4,-8
  ca = uint32_t(r4) <= 4294967288;
  r7 = static_cast<uint64_t>(-8) - r4;
  // lfs f12,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f12 = double(temp.f32);
  // lfs f13,7136(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 7136);
  f13 = double(temp.f32);
loc_823EA120:
  // lfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x823ea134
  if (!cr6.lt) goto loc_823EA134;
  // fmr f0,f13
  f0 = f13;
  // b 0x823ea140
  goto loc_823EA140;
loc_823EA134:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x823ea140
  if (cr6.lt) goto loc_823EA140;
  // fmr f0,f12
  f0 = f12;
loc_823EA140:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r10,r7,r11
  r10 = r7 + r11;
  // stfsx f0,r9,r10
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r10), temp.u32);
  // lfs f0,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x823ea160
  if (!cr6.lt) goto loc_823EA160;
  // fmr f0,f13
  f0 = f13;
  // b 0x823ea16c
  goto loc_823EA16C;
loc_823EA160:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x823ea16c
  if (cr6.lt) goto loc_823EA16C;
  // fmr f0,f12
  f0 = f12;
loc_823EA16C:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r9,r9,r10
  r9 = r9 + r10;
  // stfs f0,4(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + 4, temp.u32);
  // lfs f0,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x823ea18c
  if (!cr6.lt) goto loc_823EA18C;
  // fmr f0,f13
  f0 = f13;
  // b 0x823ea198
  goto loc_823EA198;
loc_823EA18C:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x823ea198
  if (cr6.lt) goto loc_823EA198;
  // fmr f0,f12
  f0 = f12;
loc_823EA198:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r9,r9,r10
  r9 = r9 + r10;
  // stfs f0,8(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + 8, temp.u32);
  // lfs f0,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x823ea1b8
  if (!cr6.lt) goto loc_823EA1B8;
  // fmr f0,f13
  f0 = f13;
  // b 0x823ea1c4
  goto loc_823EA1C4;
loc_823EA1B8:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x823ea1c4
  if (cr6.lt) goto loc_823EA1C4;
  // fmr f0,f12
  f0 = f12;
loc_823EA1C4:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // add r10,r9,r10
  r10 = r9 + r10;
  // stfs f0,12(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 12, temp.u32);
  // lwz r10,104(r3)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x823ea120
  if (cr6.lt) goto loc_823EA120;
  // b 0x823ea764
  goto loc_823EA764;
loc_823EA1E8:
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // lfs f13,8680(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8680);
  f13 = double(temp.f32);
  // lfs f0,2608(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f0 = double(temp.f32);
loc_823EA210:
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f12 = double(temp.f32);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stfsx f12,r8,r10
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r10), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f12 = double(temp.f32);
  // add r8,r8,r10
  r8 = r8 + r10;
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).green_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f12 = double(temp.f32);
  // add r8,r8,r10
  r8 = r8 + r10;
  // stfs f12,8(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).blue_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f12 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // add r8,r8,r10
  r8 = r8 + r10;
  // fmsubs f12,f12,f13,f0
  f12 = double(float(std::fma(f12, f13, -f0)));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stfs f12,12(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).alpha_address(), temp.u32);
  // lwz r8,104(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x823ea210
  if (cr6.lt) goto loc_823EA210;
  // b 0x823ea764
  goto loc_823EA764;
loc_823EA26C:
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // lfs f13,8680(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8680);
  f13 = double(temp.f32);
  // lfs f0,2608(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f0 = double(temp.f32);
loc_823EA294:
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f12 = double(temp.f32);
  // fmsubs f12,f12,f13,f0
  f12 = double(float(std::fma(f12, f13, -f0)));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stfsx f12,r10,r8
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r8), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f12 = double(temp.f32);
  // add r8,r10,r8
  r8 = r10 + r8;
  // fmsubs f12,f12,f13,f0
  f12 = double(float(std::fma(f12, f13, -f0)));
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).green_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f12 = double(temp.f32);
  // add r8,r10,r8
  r8 = r10 + r8;
  // fmsubs f12,f12,f13,f0
  f12 = double(float(std::fma(f12, f13, -f0)));
  // stfs f12,8(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).blue_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f12 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // add r8,r10,r8
  r8 = r10 + r8;
  // fmsubs f12,f12,f13,f0
  f12 = double(float(std::fma(f12, f13, -f0)));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stfs f12,12(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).alpha_address(), temp.u32);
  // lwz r8,104(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x823ea294
  if (cr6.lt) goto loc_823EA294;
  // b 0x823ea764
  goto loc_823EA764;
loc_823EA2FC:
  // lwz r11,84(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).convert_from_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x823ea498
  if (cr6.eq) goto loc_823EA498;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // beq cr6,0x823ea410
  if (cr6.eq) goto loc_823EA410;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x823ea764
  if (!cr6.eq) goto loc_823EA764;
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // subfic r6,r4,-8
  ca = uint32_t(r4) <= 4294967288;
  r6 = static_cast<uint64_t>(-8) - r4;
  // lfs f11,2612(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2612);
  f11 = double(temp.f32);
  // lfs f13,2608(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f13 = double(temp.f32);
  // lfs f12,7136(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 7136);
  f12 = double(temp.f32);
loc_823EA348:
  // lfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x823ea35c
  if (!cr6.lt) goto loc_823EA35C;
  // fmr f0,f12
  f0 = f12;
  // b 0x823ea368
  goto loc_823EA368;
loc_823EA35C:
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x823ea368
  if (cr6.lt) goto loc_823EA368;
  // fmr f0,f13
  f0 = f13;
loc_823EA368:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r10,r11,r6
  r10 = r11 + r6;
  // stfsx f0,r10,r9
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r9), temp.u32);
  // lfs f0,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x823ea388
  if (!cr6.lt) goto loc_823EA388;
  // fmr f0,f12
  f0 = f12;
  // b 0x823ea394
  goto loc_823EA394;
loc_823EA388:
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x823ea394
  if (cr6.lt) goto loc_823EA394;
  // fmr f0,f13
  f0 = f13;
loc_823EA394:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r9,r10,r9
  r9 = r10 + r9;
  // stfs f0,4(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + 4, temp.u32);
  // lfs f0,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x823ea3b4
  if (!cr6.lt) goto loc_823EA3B4;
  // fmr f0,f12
  f0 = f12;
  // b 0x823ea3c0
  goto loc_823EA3C0;
loc_823EA3B4:
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x823ea3c0
  if (cr6.lt) goto loc_823EA3C0;
  // fmr f0,f13
  f0 = f13;
loc_823EA3C0:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r9,r10,r9
  r9 = r10 + r9;
  // stfs f0,8(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + 8, temp.u32);
  // lfs f0,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f11
  cr6 = lifted::CompareFloat(f0, f11);
  // bge cr6,0x823ea3e0
  if (!cr6.lt) goto loc_823EA3E0;
  // fmr f0,f11
  f0 = f11;
  // b 0x823ea3ec
  goto loc_823EA3EC;
loc_823EA3E0:
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x823ea3ec
  if (cr6.lt) goto loc_823EA3EC;
  // fmr f0,f13
  f0 = f13;
loc_823EA3EC:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // add r10,r10,r9
  r10 = r10 + r9;
  // stfs f0,12(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 12, temp.u32);
  // lwz r10,104(r3)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r7,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r10), so);
  // blt cr6,0x823ea348
  if (cr6.lt) goto loc_823EA348;
  // b 0x823ea764
  goto loc_823EA764;
loc_823EA410:
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // lfs f13,7140(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 7140);
  f13 = double(temp.f32);
  // lfs f0,2608(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f0 = double(temp.f32);
loc_823EA438:
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f12 = double(temp.f32);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stfsx f12,r10,r8
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r8), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f12 = double(temp.f32);
  // add r8,r10,r8
  r8 = r10 + r8;
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).green_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f12 = double(temp.f32);
  // add r8,r10,r8
  r8 = r10 + r8;
  // stfs f12,8(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).blue_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f12 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // add r8,r10,r8
  r8 = r10 + r8;
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,12(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).alpha_address(), temp.u32);
  // lwz r8,104(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x823ea438
  if (cr6.lt) goto loc_823EA438;
  // b 0x823ea764
  goto loc_823EA764;
loc_823EA498:
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // lfs f13,8680(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8680);
  f13 = double(temp.f32);
  // lfs f0,2608(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f0 = double(temp.f32);
loc_823EA4C0:
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f12 = double(temp.f32);
  // fmsubs f12,f12,f13,f0
  f12 = double(float(std::fma(f12, f13, -f0)));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stfsx f12,r10,r8
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r8), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f12 = double(temp.f32);
  // add r8,r10,r8
  r8 = r10 + r8;
  // fmsubs f12,f12,f13,f0
  f12 = double(float(std::fma(f12, f13, -f0)));
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).green_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f12 = double(temp.f32);
  // add r8,r10,r8
  r8 = r10 + r8;
  // fmsubs f12,f12,f13,f0
  f12 = double(float(std::fma(f12, f13, -f0)));
  // stfs f12,8(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).blue_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f12 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // add r8,r10,r8
  r8 = r10 + r8;
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stfs f12,12(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).alpha_address(), temp.u32);
  // lwz r8,104(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x823ea4c0
  if (cr6.lt) goto loc_823EA4C0;
  // b 0x823ea764
  goto loc_823EA764;
loc_823EA524:
  // lwz r11,84(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).convert_from_address());
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x823ea6d0
  if (cr6.eq) goto loc_823EA6D0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // beq cr6,0x823ea630
  if (cr6.eq) goto loc_823EA630;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x823ea764
  if (!cr6.eq) goto loc_823EA764;
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // subfic r7,r4,-8
  ca = uint32_t(r4) <= 4294967288;
  r7 = static_cast<uint64_t>(-8) - r4;
  // lfs f13,2612(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2612);
  f13 = double(temp.f32);
  // lfs f12,2608(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2608);
  f12 = double(temp.f32);
loc_823EA568:
  // lfs f0,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x823ea57c
  if (!cr6.lt) goto loc_823EA57C;
  // fmr f0,f13
  f0 = f13;
  // b 0x823ea588
  goto loc_823EA588;
loc_823EA57C:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x823ea588
  if (cr6.lt) goto loc_823EA588;
  // fmr f0,f12
  f0 = f12;
loc_823EA588:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r10,r7,r11
  r10 = r7 + r11;
  // stfsx f0,r9,r10
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r10), temp.u32);
  // lfs f0,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x823ea5a8
  if (!cr6.lt) goto loc_823EA5A8;
  // fmr f0,f13
  f0 = f13;
  // b 0x823ea5b4
  goto loc_823EA5B4;
loc_823EA5A8:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x823ea5b4
  if (cr6.lt) goto loc_823EA5B4;
  // fmr f0,f12
  f0 = f12;
loc_823EA5B4:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r9,r9,r10
  r9 = r9 + r10;
  // stfs f0,4(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + 4, temp.u32);
  // lfs f0,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x823ea5d4
  if (!cr6.lt) goto loc_823EA5D4;
  // fmr f0,f13
  f0 = f13;
  // b 0x823ea5e0
  goto loc_823EA5E0;
loc_823EA5D4:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x823ea5e0
  if (cr6.lt) goto loc_823EA5E0;
  // fmr f0,f12
  f0 = f12;
loc_823EA5E0:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // add r9,r9,r10
  r9 = r9 + r10;
  // stfs f0,8(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + 8, temp.u32);
  // lfs f0,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x823ea600
  if (!cr6.lt) goto loc_823EA600;
  // fmr f0,f13
  f0 = f13;
  // b 0x823ea60c
  goto loc_823EA60C;
loc_823EA600:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x823ea60c
  if (cr6.lt) goto loc_823EA60C;
  // fmr f0,f12
  f0 = f12;
loc_823EA60C:
  // lwz r9,88(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // add r10,r9,r10
  r10 = r9 + r10;
  // stfs f0,12(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 12, temp.u32);
  // lwz r10,104(r3)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x823ea568
  if (cr6.lt) goto loc_823EA568;
  // b 0x823ea764
  goto loc_823EA764;
loc_823EA630:
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // lfs f13,7140(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 7140);
  f13 = double(temp.f32);
  // lfs f0,2608(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f0 = double(temp.f32);
loc_823EA658:
  // lfs f12,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f12 = double(temp.f32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfsx f12,r8,r10
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r10), temp.u32);
  // lfs f12,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f12 = double(temp.f32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // add r8,r8,r10
  r8 = r8 + r10;
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).green_address(), temp.u32);
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f12 = double(temp.f32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // add r8,r8,r10
  r8 = r8 + r10;
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,8(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).blue_address(), temp.u32);
  // lfs f12,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f12 = double(temp.f32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,12(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).alpha_address(), temp.u32);
  // lwz r8,104(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x823ea658
  if (cr6.lt) goto loc_823EA658;
  // b 0x823ea764
  goto loc_823EA764;
loc_823EA6D0:
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823ea764
  if (!cr6.gt) goto loc_823EA764;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // lfs f13,7140(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 7140);
  f13 = double(temp.f32);
  // lfs f0,2608(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f0 = double(temp.f32);
loc_823EA6F8:
  // lfs f12,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f12 = double(temp.f32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfsx f12,r8,r10
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r10), temp.u32);
  // lfs f12,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f12 = double(temp.f32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // add r8,r8,r10
  r8 = r8 + r10;
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).green_address(), temp.u32);
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f12 = double(temp.f32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // add r8,r8,r10
  r8 = r8 + r10;
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,8(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).blue_address(), temp.u32);
  // lwz r8,88(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // lfs f12,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f12 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stfs f12,12(r8)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r8)).alpha_address(), temp.u32);
  // lwz r8,104(r3)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x823ea6f8
  if (cr6.lt) goto loc_823EA6F8;
loc_823EA764:
  // lwz r3,88(r3)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).row_address());
  // blr
  return r3;
}

// sub_823EADB0 game::ImageCodec::ApplyColorKey: lifted from generated/ by lift.py.
void f823EADB0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_823EADB0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r11,r11,r4
  r11 = r11 + r4;
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bgelr cr6
  if (!cr6.lt) return;
  // subf r11,r4,r11
  r11 = r11 - r4;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r10,r11,-1
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // rlwinm r10,r10,28,4,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 28) & 0xFFFFFFF;
  // lfs f0,2612(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2612);
  f0 = double(temp.f32);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
loc_823EADE0:
  // lfs f13,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).color_key_red_address());
  f13 = double(temp.f32);
  // lfs f12,-8(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // bne cr6,0x823eae30
  if (!cr6.eq) goto loc_823EAE30;
  // lfs f13,40(r3)
  temp.u32 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).color_key_green_address());
  f13 = double(temp.f32);
  // lfs f12,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // bne cr6,0x823eae30
  if (!cr6.eq) goto loc_823EAE30;
  // lfs f13,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f13 = double(temp.f32);
  // lfs f12,44(r3)
  temp.u32 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).color_key_blue_address());
  f12 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // bne cr6,0x823eae30
  if (!cr6.eq) goto loc_823EAE30;
  // lfs f13,48(r3)
  temp.u32 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).color_key_alpha_address());
  f13 = double(temp.f32);
  // lfs f12,4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address());
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // bne cr6,0x823eae30
  if (!cr6.eq) goto loc_823EAE30;
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).alpha_address(), temp.u32);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address(), temp.u32);
  // stfs f0,-4(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address(), temp.u32);
  // stfs f0,-8(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address(), temp.u32);
loc_823EAE30:
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // bne 0x823eade0
  if (!cr0.eq) goto loc_823EADE0;
  // blr
  return;
}

// sub_823EB408 game::ImageCodec::AddDiffusionError: lifted from generated/ by lift.py.
uint64_t f823EB408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_823EB408 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x823eb484
  if (!cr6.gt) goto loc_823EB484;
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // subfic r7,r4,-8
  ca = uint32_t(r4) <= 4294967288;
  r7 = static_cast<uint64_t>(-8) - r4;
loc_823EB420:
  // lwz r9,92(r3)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).error_row_address());
  // add r10,r7,r11
  r10 = r7 + r11;
  // lfs f13,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address());
  f13 = double(temp.f32);
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // add r10,r10,r9
  r10 = r10 + r9;
  // lfs f12,-4(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address());
  f12 = double(temp.f32);
  // addi r9,r11,4
  r9 = uint64_t(int64_t(int64_t(r11) + 4));
  // lfs f0,0(r11)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address());
  f0 = double(temp.f32);
  // lfs f11,16(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 16);
  f11 = double(temp.f32);
  // fadds f13,f13,f11
  f13 = double(float(f13 + f11));
  // stfs f13,-8(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).red_address(), temp.u32);
  // lfs f13,20(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 20);
  f13 = double(temp.f32);
  // fadds f13,f13,f12
  f13 = double(float(f13 + f12));
  // stfs f13,-4(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).green_address(), temp.u32);
  // lfs f13,24(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 24);
  f13 = double(temp.f32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r11) - 8).blue_address(), temp.u32);
  // lfs f0,28(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 28);
  f0 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lfs f13,0(r9)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r9) - 12).alpha_address());
  f13 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r9) - 12).alpha_address(), temp.u32);
  // lwz r10,104(r3)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x823eb420
  if (cr6.lt) goto loc_823EB420;
loc_823EB484:
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).width_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,92(r3)
  r3 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r3)).error_row_address());
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // rlwinm r5,r11,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // b 0x825a71d0
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_823F24A8 game::R32FCodec::EncodeRow: lifted from generated/ by lift.py.
uint64_t f823F24A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F24A8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).srgb_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f24d8
  if (cr6.eq) goto loc_823F24D8;
  // mr r4,r6
  r4 = r6;
  // bl 0x823eae40
  // (left out: dead in C++)
  r3 = game::ImageCodec::LinearToSrgb(base, r1, r3, r4);
  // mr r6,r3
  r6 = r3;
loc_823F24D8:
  // lwz r10,96(r31)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).row_pitch_address());
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r8,100(r31)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).slice_pitch_address());
  // mullw r10,r10,r30
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r30))));
  // lwz r9,32(r31)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).data_address());
  // mullw r8,r8,r29
  r8 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r29))));
  // add r10,r10,r8
  r10 = r10 + r8;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r9,104(r31)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).width_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // ble cr6,0x823f2528
  if (!cr6.gt) goto loc_823F2528;
  // mr r9,r6
  r9 = r6;
loc_823F2508:
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r9)).red_address());
  f0 = double(temp.f32);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 0, temp.u32);
  // lwz r8,104(r31)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).width_address());
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x823f2508
  if (cr6.lt) goto loc_823F2508;
loc_823F2528:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_823F2530 game::G32R32FCodec::EncodeRow: lifted from generated/ by lift.py.
uint64_t f823F2530(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2530 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r9,r6
  r9 = r6;
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).srgb_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2564
  if (cr6.eq) goto loc_823F2564;
  // mr r4,r9
  r4 = r9;
  // bl 0x823eae40
  // (left out: dead in C++)
  r3 = game::ImageCodec::LinearToSrgb(base, r1, r3, r4);
  // mr r9,r3
  r9 = r3;
loc_823F2564:
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).row_pitch_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r7,100(r31)
  r7 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).slice_pitch_address());
  // mullw r11,r11,r30
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r30))));
  // lwz r8,32(r31)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).data_address());
  // mullw r7,r7,r29
  r7 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r29))));
  // add r11,r11,r7
  r11 = r11 + r7;
  // add r11,r11,r8
  r11 = r11 + r8;
  // lwz r8,104(r31)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).width_address());
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // ble cr6,0x823f25b8
  if (!cr6.gt) goto loc_823F25B8;
loc_823F2590:
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r9)).red_address());
  f0 = double(temp.f32);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // lfs f0,4(r9)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r9)).green_address());
  f0 = double(temp.f32);
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 4, temp.u32);
  // lwz r8,104(r31)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).width_address());
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // blt cr6,0x823f2590
  if (cr6.lt) goto loc_823F2590;
loc_823F25B8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_823F25C0 game::A32B32G32R32FCodec::EncodeRow: lifted from generated/ by lift.py.
uint64_t f823F25C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F25C0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r4,r6
  r4 = r6;
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).srgb_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f25f0
  if (cr6.eq) goto loc_823F25F0;
  // bl 0x823eae40
  // (left out: dead in C++)
  r3 = game::ImageCodec::LinearToSrgb(base, r1, r3, r4);
  // mr r4,r3
  r4 = r3;
loc_823F25F0:
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).row_pitch_address());
  // lwz r9,100(r31)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).slice_pitch_address());
  // mullw r11,r11,r30
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r30))));
  // lwz r8,104(r31)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).width_address());
  // mullw r9,r9,r29
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r29))));
  // lwz r10,32(r31)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).data_address());
  // add r11,r11,r9
  r11 = r11 + r9;
  // rlwinm r5,r8,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // add r3,r11,r10
  r3 = r11 + r10;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x823F2618, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_823F5520 game::A32B32G32R32FCodec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F5520(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F5520 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r6
  r30 = r6;
  // mr r3,r30
  r3 = r30;
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).row_pitch_address());
  // lwz r9,100(r31)
  r9 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).slice_pitch_address());
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r8,104(r31)
  r8 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).width_address());
  // lwz r10,32(r31)
  r10 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).data_address());
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // rlwinm r5,r8,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x823F5568, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).color_key_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f5580
  if (cr6.eq) goto loc_823F5580;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F5580:
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::ImageCodec(base, uint32_t(r31)).srgb_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f5598
  if (cr6.eq) goto loc_823F5598;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F5598:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_823F59C8 game::YuvCodec::ConvertPair: lifted from generated/ by lift.py.
lifted::Result f823F59C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_823F59C8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-72(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -72, std::bit_cast<uint64_t>(f29));
  // stfd f30,-64(r1)
  guest::StoreU64(base, uint32_t(r1) + -64, std::bit_cast<uint64_t>(f30));
  // stfd f31,-56(r1)
  guest::StoreU64(base, uint32_t(r1) + -56, std::bit_cast<uint64_t>(f31));
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // lwz r11,164(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_164_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x823f5a08
  if (!cr6.eq) goto loc_823F5A08;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x823f5e18
  goto loc_823F5E18;
loc_823F5A08:
  // lwz r11,136(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_136_address());
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x823f5a38
  if (cr6.lt) goto loc_823F5A38;
  // lwz r11,144(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_144_address());
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bge cr6,0x823f5a38
  if (!cr6.lt) goto loc_823F5A38;
  // lwz r11,148(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_148_address());
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x823f5a38
  if (cr6.lt) goto loc_823F5A38;
  // lwz r11,152(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_152_address());
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x823f5e14
  if (cr6.lt) goto loc_823F5E14;
loc_823F5A38:
  // mr r3,r31
  r3 = r31;
  // bl 0x823f56a0
  // (left out: dead in C++)
  { const lifted::Result o = game::YuvCodec::Flush(base, r1, r3, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x823f5e18
  if (cr0.lt) goto loc_823F5E18;
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r30,136(r31)
  guest::StoreU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_136_address(), uint32_t(r30));
  // addi r10,r29,1
  r10 = uint64_t(int64_t(int64_t(r29) + 1));
  // stw r29,148(r31)
  guest::StoreU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_148_address(), uint32_t(r29));
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // stw r11,144(r31)
  guest::StoreU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_144_address(), uint32_t(r11));
  // stw r10,152(r31)
  guest::StoreU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_152_address(), uint32_t(r10));
  // beq cr6,0x823f5e14
  if (cr6.eq) goto loc_823F5E14;
  // lis r11,6184
  r11 = uint64_t(int64_t(405274624));
  // lwz r6,100(r31)
  r6 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).slice_pitch_address());
  // lwz r9,132(r31)
  r9 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).even_left_address());
  // ori r5,r11,75
  r5 = r11 | 75;
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).row_pitch_address());
  // mullw r6,r6,r29
  r6 = uint64_t(int64_t(int64_t(int32_t(r6)) * int64_t(int32_t(r29))));
  // lwz r8,4(r31)
  r8 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).format_address());
  // lwz r7,32(r31)
  r7 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).data_address());
  // mullw r10,r11,r30
  r10 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r30))));
  // lwz r11,128(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).buffer_address());
  // add r10,r10,r6
  r10 = r10 + r6;
  // rlwinm r6,r9,1,0,30
  r6 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // cmpw cr6,r8,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r5), so);
  // add r10,r10,r6
  r10 = r10 + r6;
  // add r10,r10,r7
  r10 = r10 + r7;
  // blt cr6,0x823f5e14
  if (cr6.lt) goto loc_823F5E14;
  // lis r7,6184
  r7 = uint64_t(int64_t(405274624));
  // ori r7,r7,76
  r7 = r7 | 76;
  // cmpw cr6,r8,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r7), so);
  // ble cr6,0x823f5d38
  if (!cr6.gt) goto loc_823F5D38;
  // addis r8,r8,-6688
  r8 = uint64_t(int64_t(int64_t(r8) + -438304768));
  // addi r8,r8,-75
  r8 = uint64_t(int64_t(int64_t(r8) + -75));
  // cmplwi cr6,r8,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 1, so);
  // bgt cr6,0x823f5e14
  if (cr6.gt) goto loc_823F5E14;
  // mr r28,r9
  r28 = r9;
  // lwz r9,140(r31)
  r9 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).even_right_address());
  // cmplw cr6,r28,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r9), so);
  // bge cr6,0x823f5e14
  if (!cr6.lt) goto loc_823F5E14;
  // lis r29,-32256
  r29 = uint64_t(int64_t(-2113929216));
  // lis r30,-32256
  r30 = uint64_t(int64_t(-2113929216));
  // lis r3,-32252
  r3 = uint64_t(int64_t(-2113667072));
  // lis r4,-32252
  r4 = uint64_t(int64_t(-2113667072));
  // lis r5,-32252
  r5 = uint64_t(int64_t(-2113667072));
  // lis r6,-32252
  r6 = uint64_t(int64_t(-2113667072));
  // lfs f9,2612(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 2612);
  f9 = double(temp.f32);
  // lis r7,-32252
  r7 = uint64_t(int64_t(-2113667072));
  // lfs f10,2608(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 2608);
  f10 = double(temp.f32);
  // lis r8,-32252
  r8 = uint64_t(int64_t(-2113667072));
  // lfs f29,-18428(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + -18428);
  f29 = double(temp.f32);
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f30,-18432(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + -18432);
  f30 = double(temp.f32);
  // lfs f31,-18436(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + -18436);
  f31 = double(temp.f32);
  // lfs f4,-18440(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + -18440);
  f4 = double(temp.f32);
  // lfs f1,-18444(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + -18444);
  f1 = double(temp.f32);
  // lfs f2,-18448(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + -18448);
  f2 = double(temp.f32);
  // lfs f3,8684(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8684);
  f3 = double(temp.f32);
loc_823F5B20:
  // lhz r9,0(r10)
  r9 = guest::LoadU16(base, uint32_t(r10) + 0);
  // lhz r8,2(r10)
  r8 = guest::LoadU16(base, uint32_t(r10) + 2);
  // rotlwi r30,r9,8
  r30 = __builtin_rotateleft32(uint32_t(r9), 8);
  // lwz r6,172(r31)
  r6 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_172_address());
  // rlwinm r29,r9,24,8,31
  r29 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFFFFFF;
  // lwz r7,168(r31)
  r7 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_168_address());
  // rotlwi r3,r9,8
  r3 = __builtin_rotateleft32(uint32_t(r9), 8);
  // stfs f10,12(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f10);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel0_alpha_address(), temp.u32);
  // rlwinm r9,r9,24,8,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFFFFFF;
  // clrlwi r30,r30,16
  r30 = uint32_t(r30) & 0xFFFF;
  // clrlwi r3,r3,16
  r3 = uint32_t(r3) & 0xFFFF;
  // or r9,r30,r9
  r9 = r30 | r9;
  // rotlwi r30,r8,8
  r30 = __builtin_rotateleft32(uint32_t(r8), 8);
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // or r3,r3,r29
  r3 = r3 | r29;
  // clrlwi r6,r6,16
  r6 = uint32_t(r6) & 0xFFFF;
  // rlwinm r29,r8,24,8,31
  r29 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 24) & 0xFFFFFF;
  // clrlwi r30,r30,16
  r30 = uint32_t(r30) & 0xFFFF;
  // clrlwi r7,r7,16
  r7 = uint32_t(r7) & 0xFFFF;
  // or r30,r30,r29
  r30 = r30 | r29;
  // clrlwi r3,r3,16
  r3 = uint32_t(r3) & 0xFFFF;
  // clrlwi r30,r30,16
  r30 = uint32_t(r30) & 0xFFFF;
  // rotlwi r27,r8,8
  r27 = __builtin_rotateleft32(uint32_t(r8), 8);
  // rlwinm r8,r8,24,8,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 24) & 0xFFFFFF;
  // clrlwi r29,r27,16
  r29 = uint32_t(r27) & 0xFFFF;
  // or r8,r29,r8
  r8 = r29 | r8;
  // clrlwi r8,r8,16
  r8 = uint32_t(r8) & 0xFFFF;
  // srw r9,r9,r6
  r9 = uint8_t(r6) & 0x20 ? 0 : (uint32_t(r9) >> (uint8_t(r6) & 0x3F));
  // clrlwi r9,r9,24
  r9 = uint32_t(r9) & 0xFF;
  // srw r5,r3,r7
  r5 = uint8_t(r7) & 0x20 ? 0 : (uint32_t(r3) >> (uint8_t(r7) & 0x3F));
  // srw r6,r30,r6
  r6 = uint8_t(r6) & 0x20 ? 0 : (uint32_t(r30) >> (uint8_t(r6) & 0x3F));
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // clrlwi r5,r5,24
  r5 = uint32_t(r5) & 0xFF;
  // clrlwi r9,r6,24
  r9 = uint32_t(r6) & 0xFF;
  // std r5,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r5);
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // srw r8,r8,r7
  r8 = uint8_t(r7) & 0x20 ? 0 : (uint32_t(r8) >> (uint8_t(r7) & 0x3F));
  // clrlwi r8,r8,24
  r8 = uint32_t(r8) & 0xFF;
  // mr r9,r8
  r9 = r8;
  // std r9,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r9);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f13,f13
  f13 = double(float(f13));
  // lfd f11,104(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // frsp f0,f0
  f0 = double(float(f0));
  // frsp f8,f12
  f8 = double(float(f12));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // fsubs f12,f13,f2
  f12 = double(float(f13 - f2));
  // fsubs f0,f0,f3
  f0 = double(float(f0 - f3));
  // fsubs f13,f8,f2
  f13 = double(float(f8 - f2));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f8,f12,f31
  f8 = double(float(f12 * f31));
  // fmuls f12,f12,f29
  f12 = double(float(f12 * f29));
  // fmuls f0,f0,f4
  f0 = double(float(f0 * f4));
  // fmuls f7,f13,f1
  f7 = double(float(f13 * f1));
  // fsubs f5,f11,f3
  f5 = double(float(f11 - f3));
  // fmuls f6,f13,f30
  f6 = double(float(f13 * f30));
  // fsubs f11,f0,f8
  f11 = double(float(f0 - f8));
  // fadds f13,f0,f7
  f13 = double(float(f0 + f7));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel0_red_address(), temp.u32);
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel0_blue_address(), temp.u32);
  // fsubs f11,f11,f6
  f11 = double(float(f11 - f6));
  // stfs f11,4(r11)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel0_green_address(), temp.u32);
  // fcmpu cr6,f13,f9
  cr6 = lifted::CompareFloat(f13, f9);
  // bge cr6,0x823f5c40
  if (!cr6.lt) goto loc_823F5C40;
  // fmr f13,f9
  f13 = f9;
  // b 0x823f5c4c
  goto loc_823F5C4C;
loc_823F5C40:
  // fcmpu cr6,f13,f10
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f13, f10);
  // ble cr6,0x823f5c4c
  if (!cr6.gt) goto loc_823F5C4C;
  // fmr f13,f10
  f13 = f10;
loc_823F5C4C:
  // stfs f13,0(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel0_red_address(), temp.u32);
  // fcmpu cr6,f11,f9
  cr6 = lifted::CompareFloat(f11, f9);
  // bge cr6,0x823f5c60
  if (!cr6.lt) goto loc_823F5C60;
  // fmr f11,f9
  f11 = f9;
  // b 0x823f5c6c
  goto loc_823F5C6C;
loc_823F5C60:
  // fcmpu cr6,f11,f10
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f11, f10);
  // ble cr6,0x823f5c6c
  if (!cr6.gt) goto loc_823F5C6C;
  // fmr f11,f10
  f11 = f10;
loc_823F5C6C:
  // stfs f11,4(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel0_green_address(), temp.u32);
  // fcmpu cr6,f0,f9
  cr6 = lifted::CompareFloat(f0, f9);
  // bge cr6,0x823f5c80
  if (!cr6.lt) goto loc_823F5C80;
  // fmr f13,f9
  f13 = f9;
  // b 0x823f5c94
  goto loc_823F5C94;
loc_823F5C80:
  // fcmpu cr6,f0,f10
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f10);
  // ble cr6,0x823f5c90
  if (!cr6.gt) goto loc_823F5C90;
  // fmr f13,f10
  f13 = f10;
  // b 0x823f5c94
  goto loc_823F5C94;
loc_823F5C90:
  // fmr f13,f0
  lifted::ScalarFloatMode();
  f13 = f0;
loc_823F5C94:
  // fmuls f0,f5,f4
  lifted::ScalarFloatMode();
  f0 = double(float(f5 * f4));
  // stfs f13,8(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel0_blue_address(), temp.u32);
  // stfs f10,28(r11)
  temp.f32 = float(f10);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel1_alpha_address(), temp.u32);
  // fsubs f11,f0,f8
  f11 = double(float(f0 - f8));
  // fadds f13,f0,f7
  f13 = double(float(f0 + f7));
  // stfs f13,16(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel1_red_address(), temp.u32);
  // fadds f12,f0,f12
  f12 = double(float(f0 + f12));
  // stfs f12,24(r11)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel1_blue_address(), temp.u32);
  // fsubs f0,f11,f6
  f0 = double(float(f11 - f6));
  // stfs f0,20(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel1_green_address(), temp.u32);
  // fcmpu cr6,f13,f9
  cr6 = lifted::CompareFloat(f13, f9);
  // bge cr6,0x823f5ccc
  if (!cr6.lt) goto loc_823F5CCC;
  // fmr f13,f9
  f13 = f9;
  // b 0x823f5cd8
  goto loc_823F5CD8;
loc_823F5CCC:
  // fcmpu cr6,f13,f10
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f13, f10);
  // ble cr6,0x823f5cd8
  if (!cr6.gt) goto loc_823F5CD8;
  // fmr f13,f10
  f13 = f10;
loc_823F5CD8:
  // stfs f13,16(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel1_red_address(), temp.u32);
  // fcmpu cr6,f0,f9
  cr6 = lifted::CompareFloat(f0, f9);
  // bge cr6,0x823f5cec
  if (!cr6.lt) goto loc_823F5CEC;
  // fmr f0,f9
  f0 = f9;
  // b 0x823f5cf8
  goto loc_823F5CF8;
loc_823F5CEC:
  // fcmpu cr6,f0,f10
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f10);
  // ble cr6,0x823f5cf8
  if (!cr6.gt) goto loc_823F5CF8;
  // fmr f0,f10
  f0 = f10;
loc_823F5CF8:
  // stfs f0,20(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel1_green_address(), temp.u32);
  // fcmpu cr6,f12,f9
  cr6 = lifted::CompareFloat(f12, f9);
  // bge cr6,0x823f5d0c
  if (!cr6.lt) goto loc_823F5D0C;
  // fmr f12,f9
  f12 = f9;
  // b 0x823f5d18
  goto loc_823F5D18;
loc_823F5D0C:
  // fcmpu cr6,f12,f10
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f12, f10);
  // ble cr6,0x823f5d18
  if (!cr6.gt) goto loc_823F5D18;
  // fmr f12,f10
  f12 = f10;
loc_823F5D18:
  // stfs f12,24(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11)).pixel1_blue_address(), temp.u32);
  // lwz r9,140(r31)
  r9 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).even_right_address());
  // addi r28,r28,2
  r28 = uint64_t(int64_t(int64_t(r28) + 2));
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r28,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r9), so);
  // blt cr6,0x823f5b20
  if (cr6.lt) goto loc_823F5B20;
  // b 0x823f5e14
  goto loc_823F5E14;
loc_823F5D38:
  // lwz r8,140(r31)
  r8 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).even_right_address());
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // bge cr6,0x823f5e14
  if (!cr6.lt) goto loc_823F5E14;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lfs f0,8704(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8704);
  f0 = double(temp.f32);
  // lfs f11,2608(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f11 = double(temp.f32);
loc_823F5D54:
  // lwz r7,168(r31)
  r7 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_168_address());
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // lwz r5,172(r31)
  r5 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_172_address());
  // lhz r8,0(r10)
  r8 = guest::LoadU16(base, uint32_t(r10) + 0);
  // clrlwi r7,r7,16
  r7 = uint32_t(r7) & 0xFFFF;
  // lhz r6,2(r10)
  r6 = guest::LoadU16(base, uint32_t(r10) + 2);
  // clrlwi r5,r5,16
  r5 = uint32_t(r5) & 0xFFFF;
  // stfs f11,12(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11) - 32).pixel2_alpha_address(), temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stfs f11,28(r11)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11) - 32).pixel3_alpha_address(), temp.u32);
  // srw r4,r8,r7
  r4 = uint8_t(r7) & 0x20 ? 0 : (uint32_t(r8) >> (uint8_t(r7) & 0x3F));
  // srw r8,r8,r5
  r8 = uint8_t(r5) & 0x20 ? 0 : (uint32_t(r8) >> (uint8_t(r5) & 0x3F));
  // srw r7,r6,r7
  r7 = uint8_t(r7) & 0x20 ? 0 : (uint32_t(r6) >> (uint8_t(r7) & 0x3F));
  // srw r6,r6,r5
  r6 = uint8_t(r5) & 0x20 ? 0 : (uint32_t(r6) >> (uint8_t(r5) & 0x3F));
  // clrlwi r5,r4,24
  r5 = uint32_t(r4) & 0xFF;
  // clrlwi r8,r8,24
  r8 = uint32_t(r8) & 0xFF;
  // clrlwi r7,r7,24
  r7 = uint32_t(r7) & 0xFF;
  // clrlwi r6,r6,24
  r6 = uint32_t(r6) & 0xFF;
  // std r5,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r5);
  // std r8,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r8);
  // std r7,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r7);
  // std r6,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r6);
  // lfd f13,104(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f10,88(r1)
  f10 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f9,80(r1)
  f9 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // fcfid f9,f9
  f9 = double(std::bit_cast<int64_t>(f9));
  // frsp f13,f13
  f13 = double(float(f13));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f8,f10
  f8 = double(float(f10));
  // frsp f7,f9
  f7 = double(float(f9));
  // fmuls f10,f13,f0
  f10 = double(float(f13 * f0));
  // stfs f10,4(r11)
  temp.f32 = float(f10);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11) - 32).pixel2_green_address(), temp.u32);
  // fmuls f13,f12,f0
  f13 = double(float(f12 * f0));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11) - 32).pixel2_red_address(), temp.u32);
  // fmuls f9,f8,f0
  f9 = double(float(f8 * f0));
  // stfs f13,16(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11) - 32).pixel3_red_address(), temp.u32);
  // fmuls f12,f7,f0
  f12 = double(float(f7 * f0));
  // stfs f9,20(r11)
  temp.f32 = float(f9);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11) - 32).pixel3_green_address(), temp.u32);
  // stfs f12,8(r11)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11) - 32).pixel2_blue_address(), temp.u32);
  // stfs f12,24(r11)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::YuvCodecPixels(base, uint32_t(r11) - 32).pixel3_blue_address(), temp.u32);
  // lwz r8,140(r31)
  r8 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).even_right_address());
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x823f5d54
  if (cr6.lt) goto loc_823F5D54;
loc_823F5E14:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_823F5E18:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // lfd f29,-72(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -72)));
  // lfd f30,-64(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // lfd f31,-56(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_823F5E30 game::YuvCodec::EncodeRow: lifted from generated/ by lift.py.
lifted::Result f823F5E30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_823F5E30 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // mr r30,r6
  r30 = r6;
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).convert_from_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x823f5e64
  if (cr6.eq) goto loc_823F5E64;
  // mr r4,r30
  r4 = r30;
  // bl 0x823ea0c0
  // (left out: dead in C++)
  r3 = game::ImageCodec::ConvertRange(base, r3, r4);
  // mr r30,r3
  r30 = r3;
loc_823F5E64:
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).srgb_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f5e80
  if (cr6.eq) goto loc_823F5E80;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x823eae40
  // (left out: dead in C++)
  r3 = game::ImageCodec::LinearToSrgb(base, r1, r3, r4);
  // mr r30,r3
  r30 = r3;
loc_823F5E80:
  // lwz r10,64(r31)
  r10 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).box_top_address());
  // mr r3,r31
  r3 = r31;
  // lwz r11,76(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).box_front_address());
  // add r4,r10,r29
  r4 = r10 + r29;
  // lwz r10,156(r31)
  r10 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).buffer_pixels_address());
  // add r5,r11,r28
  r5 = r11 + r28;
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).width_address());
  // subf r11,r10,r11
  r11 = r11 - r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r6,r11,1
  r6 = r11 ^ 1;
  // bl 0x823f59c8
  // (left out: dead in C++)
  { const lifted::Result o = game::YuvCodec::ConvertPair(base, r1, r13, r3, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x823f5ee8
  if (cr0.lt) goto loc_823F5EE8;
  // lwz r10,132(r31)
  r10 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).even_left_address());
  // mr r4,r30
  r4 = r30;
  // lwz r11,60(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).box_left_address());
  // lwz r9,104(r31)
  r9 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).width_address());
  // subf r11,r10,r11
  r11 = r11 - r10;
  // lwz r10,128(r31)
  r10 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).buffer_address());
  // rlwinm r5,r9,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r3,r11,r10
  r3 = r11 + r10;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x823F5EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,160(r31)
  guest::StoreU32(base, schema::YuvCodec(base, uint32_t(r31)).unknown_160_address(), uint32_t(r11));
loc_823F5EE8:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_823F5EF0 game::YuvCodec::DecodeRow: lifted from generated/ by lift.py.
lifted::Result f823F5EF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_823F5EF0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r6
  r30 = r6;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r10,64(r31)
  r10 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).box_top_address());
  // lwz r11,76(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).box_front_address());
  // add r4,r10,r4
  r4 = r10 + r4;
  // add r5,r11,r5
  r5 = r11 + r5;
  // bl 0x823f59c8
  // (left out: dead in C++)
  { const lifted::Result o = game::YuvCodec::ConvertPair(base, r1, r13, r3, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x823f5f84
  if (cr0.lt) goto loc_823F5F84;
  // lwz r10,132(r31)
  r10 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).even_left_address());
  // mr r3,r30
  r3 = r30;
  // lwz r11,60(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).box_left_address());
  // lwz r9,104(r31)
  r9 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).width_address());
  // subf r11,r10,r11
  r11 = r11 - r10;
  // lwz r10,128(r31)
  r10 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).buffer_address());
  // rlwinm r5,r9,4,0,27
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x823F5F54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).color_key_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f5f6c
  if (cr6.eq) goto loc_823F5F6C;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F5F6C:
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).srgb_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f5f84
  if (cr6.eq) goto loc_823F5F84;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F5F84:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_823F6C10 game::YuvCodec::Dtor: lifted from generated/ by lift.py.
lifted::Result f823F6C10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_823F6C10 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32252
  r11 = uint64_t(int64_t(-2113667072));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-17928
  r11 = uint64_t(int64_t(int64_t(r11) + -17928));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::YuvCodec(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // bl 0x823f56a0
  // (left out: dead in C++)
  { const lifted::Result o = game::YuvCodec::Flush(base, r1, r3, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,128(r31)
  r3 = guest::LoadU32(base, schema::YuvCodec(base, uint32_t(r31)).buffer_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x823f6c44
  if (cr0.eq) goto loc_823F6C44;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x823F6C44, r31)); r3 = o.r3; f1 = o.f1; }
loc_823F6C44:
  // mr r3,r31
  r3 = r31;
  // bl 0x823ea070
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x823EA070, LIFTED_ARGS(0x823F6C4C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_823F77B0: lifted from generated/ by lift.py.
lifted::Result f823F77B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_823F77B0 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x823f6c10
  // (left out: dead in C++)
  { const lifted::Result o = game::YuvCodec::Dtor(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x823f77e0
  if (cr0.eq) goto loc_823F77E0;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x823F77E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_823F77E0:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8240FBA0: lifted from generated/ by lift.py.
void f8240FBA0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8240FBA0 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // stw r11,8(r3)
  guest::StoreU32(base, uint32_t(r3) + 8, uint32_t(r11));
  // stw r11,12(r3)
  guest::StoreU32(base, uint32_t(r3) + 12, uint32_t(r11));
  // stw r11,16(r3)
  guest::StoreU32(base, uint32_t(r3) + 16, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // stw r4,16(r3)
  guest::StoreU32(base, uint32_t(r3) + 16, uint32_t(r4));
  // oris r11,r11,32768
  r11 = r11 | 2147483648;
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // stw r3,4(r4)
  guest::StoreU32(base, uint32_t(r4) + 4, uint32_t(r3));
  // blr
  return;
}

// sub_82411640: lifted from generated/ by lift.py.
uint64_t f82411640(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82411640 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // li r5,36
  r5 = uint64_t(int64_t(36));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82411668, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lis r11,64
  r11 = uint64_t(int64_t(4194304));
  // rlwinm r10,r10,0,25,17
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFC07F;
  // rlwimi r11,r30,7,18,24
  r11 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 7) & 0x3F80) | (r11 & 0xFFFFFFFFFFFFC07F);
  // mr r3,r31
  r3 = r31;
  // or r11,r11,r10
  r11 = r11 | r10;
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82420640 game::Dxt::Unpremultiply: lifted from generated/ by lift.py.
uint64_t f82420640(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82420640 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // lfs f11,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2608);
  f11 = double(temp.f32);
  // lfs f10,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f10 = double(temp.f32);
loc_82420654:
  // lfs f13,12(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r3)).alpha_address());
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f10
  cr6 = lifted::CompareFloat(f13, f10);
  // bne cr6,0x82420670
  if (!cr6.eq) goto loc_82420670;
  // stfs f10,0(r3)
  temp.f32 = float(f10);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r3)).red_address(), temp.u32);
  // stfs f10,4(r3)
  temp.f32 = float(f10);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r3)).green_address(), temp.u32);
  // stfs f10,8(r3)
  temp.f32 = float(f10);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r3)).blue_address(), temp.u32);
  // b 0x824206d0
  goto loc_824206D0;
loc_82420670:
  // fcmpu cr6,f13,f11
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f13, f11);
  // bge cr6,0x824206d0
  if (!cr6.lt) goto loc_824206D0;
  // lfs f0,0(r3)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r3)).red_address());
  f0 = double(temp.f32);
  // fdivs f12,f11,f13
  f12 = double(float(f11 / f13));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82420690
  if (!cr6.lt) goto loc_82420690;
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // b 0x82420694
  goto loc_82420694;
loc_82420690:
  // fmr f0,f11
  lifted::ScalarFloatMode();
  f0 = f11;
loc_82420694:
  // stfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r3)).red_address(), temp.u32);
  // lfs f0,4(r3)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r3)).green_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x824206ac
  if (!cr6.lt) goto loc_824206AC;
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // b 0x824206b0
  goto loc_824206B0;
loc_824206AC:
  // fmr f0,f11
  lifted::ScalarFloatMode();
  f0 = f11;
loc_824206B0:
  // stfs f0,4(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r3)).green_address(), temp.u32);
  // lfs f0,8(r3)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r3)).blue_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x824206c8
  if (!cr6.lt) goto loc_824206C8;
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // b 0x824206cc
  goto loc_824206CC;
loc_824206C8:
  // fmr f0,f11
  lifted::ScalarFloatMode();
  f0 = f11;
loc_824206CC:
  // stfs f0,8(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r3)).blue_address(), temp.u32);
loc_824206D0:
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // bne 0x82420654
  if (!cr0.eq) goto loc_82420654;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82421BF0 game::Dxt3A::DecodeBlock: lifted from generated/ by lift.py.
uint64_t f82421BF0(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82421BF0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::Dxt3ABlock(base, uint32_t(r4)).alphas_low_address());
  // addi r9,r3,12
  r9 = uint64_t(int64_t(int64_t(r3) + 12));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // rlwinm r8,r11,16,16,31
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF;
  // rlwinm r11,r11,16,0,15
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF0000;
  // or r11,r8,r11
  r11 = r8 | r11;
  // lis r8,-32255
  r8 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32672(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + -32672);
  f0 = double(temp.f32);
loc_82421C10:
  // clrldi r8,r11,60
  r8 = r11 & 0xF;
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r11,r11,28,4,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0xFFFFFFF;
  // std r8,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r8);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r9)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::Dxt3ABlock(base, uint32_t(r9) - 12).pixel0_alpha_address(), temp.u32);
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // bne 0x82421c10
  if (!cr0.eq) goto loc_82421C10;
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, schema::Dxt3ABlock(base, uint32_t(r4)).alphas_high_address());
  // addi r9,r3,140
  r9 = uint64_t(int64_t(int64_t(r3) + 140));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // rlwinm r8,r11,16,16,31
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF;
  // rlwinm r11,r11,16,0,15
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF0000;
  // or r11,r8,r11
  r11 = r8 | r11;
loc_82421C54:
  // clrldi r8,r11,60
  r8 = r11 & 0xF;
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r11,r11,28,4,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0xFFFFFFF;
  // std r8,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r8);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r9)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::Dxt3ABlock(base, uint32_t(r9) - 140).pixel8_alpha_address(), temp.u32);
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // bne 0x82421c54
  if (!cr0.eq) goto loc_82421C54;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82421C88 game::DxtAlpha::DecodeBlock: lifted from generated/ by lift.py.
uint64_t f82421C88(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82421C88 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // std r30,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  guest::StoreU64(base, uint32_t(r1) + -8, r31);
  // lhz r11,0(r4)
  r11 = guest::LoadU16(base, schema::DxtAlphaBlock(base, uint32_t(r4)).ends_address());
  // lhz r10,2(r4)
  r10 = guest::LoadU16(base, schema::DxtAlphaBlock(base, uint32_t(r4)).indices_2_address());
  // rotlwi r9,r11,8
  r9 = __builtin_rotateleft32(uint32_t(r11), 8);
  // rlwinm r8,r11,24,8,31
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xFFFFFF;
  // lhz r11,4(r4)
  r11 = guest::LoadU16(base, schema::DxtAlphaBlock(base, uint32_t(r4)).indices_4_address());
  // clrlwi r7,r9,16
  r7 = uint32_t(r9) & 0xFFFF;
  // lhz r9,6(r4)
  r9 = guest::LoadU16(base, schema::DxtAlphaBlock(base, uint32_t(r4)).indices_6_address());
  // rotlwi r5,r11,8
  r5 = __builtin_rotateleft32(uint32_t(r11), 8);
  // or r8,r7,r8
  r8 = r7 | r8;
  // rlwinm r11,r11,24,8,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xFFFFFF;
  // clrlwi r5,r5,16
  r5 = uint32_t(r5) & 0xFFFF;
  // rotlwi r4,r9,8
  r4 = __builtin_rotateleft32(uint32_t(r9), 8);
  // or r11,r5,r11
  r11 = r5 | r11;
  // sth r8,-80(r1)
  guest::StoreU16(base, uint32_t(r1) + -80, uint16_t(r8));
  // clrlwi r4,r4,16
  r4 = uint32_t(r4) & 0xFFFF;
  // lbz r8,-80(r1)
  r8 = guest::LoadU8(base, uint32_t(r1) + -80);
  // rlwinm r9,r9,24,8,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFFFFFF;
  // lbz r7,-79(r1)
  r7 = guest::LoadU8(base, uint32_t(r1) + -79);
  // rotlwi r6,r10,8
  r6 = __builtin_rotateleft32(uint32_t(r10), 8);
  // mr r31,r8
  r31 = r8;
  // mr r30,r7
  r30 = r7;
  // sth r11,-76(r1)
  guest::StoreU16(base, uint32_t(r1) + -76, uint16_t(r11));
  // or r11,r4,r9
  r11 = r4 | r9;
  // rlwinm r10,r10,24,8,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFFFFFF;
  // clrlwi r6,r6,16
  r6 = uint32_t(r6) & 0xFFFF;
  // std r31,-72(r1)
  guest::StoreU64(base, uint32_t(r1) + -72, r31);
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // std r30,-64(r1)
  guest::StoreU64(base, uint32_t(r1) + -64, r30);
  // or r10,r6,r10
  r10 = r6 | r10;
  // sth r11,-74(r1)
  guest::StoreU16(base, uint32_t(r1) + -74, uint16_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // sth r10,-78(r1)
  guest::StoreU16(base, uint32_t(r1) + -78, uint16_t(r10));
  // addi r10,r1,-40
  r10 = uint64_t(int64_t(int64_t(r1) + -40));
  // lfd f0,-72(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -72)));
  // lfd f13,-64(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f12,f0
  f12 = double(float(f0));
  // lfs f0,8704(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8704);
  f0 = double(temp.f32);
  // frsp f13,f13
  f13 = double(float(f13));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,-48(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -48, temp.u32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-44(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -44, temp.u32);
  // ble cr6,0x82421da8
  if (!cr6.gt) goto loc_82421DA8;
  // lis r9,-32252
  r9 = uint64_t(int64_t(-2113667072));
  // lfs f0,-18512(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + -18512);
  f0 = double(temp.f32);
loc_82421D50:
  // clrldi r9,r11,32
  r9 = r11 & 0xFFFFFFFF;
  // lfs f11,-44(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + -44);
  f11 = double(temp.f32);
  // subfic r8,r11,7
  ca = uint32_t(r11) <= 7;
  r8 = static_cast<uint64_t>(7) - r11;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrldi r8,r8,32
  r8 = r8 & 0xFFFFFFFF;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // std r9,-64(r1)
  guest::StoreU64(base, uint32_t(r1) + -64, r9);
  // std r8,-72(r1)
  guest::StoreU64(base, uint32_t(r1) + -72, r8);
  // lfd f13,-64(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f12,-72(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -72)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f13,f13
  f13 = double(float(f13));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // lfs f11,-48(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + -48);
  f11 = double(temp.f32);
  // fmadds f13,f12,f11,f13
  f13 = double(float(std::fma(f12, f11, f13)));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r10)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // blt cr6,0x82421d50
  if (cr6.lt) goto loc_82421D50;
  // b 0x82421e1c
  goto loc_82421E1C;
loc_82421DA8:
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f0,8712(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8712);
  f0 = double(temp.f32);
loc_82421DB0:
  // clrldi r9,r11,32
  r9 = r11 & 0xFFFFFFFF;
  // lfs f11,-44(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + -44);
  f11 = double(temp.f32);
  // subfic r8,r11,5
  ca = uint32_t(r11) <= 5;
  r8 = static_cast<uint64_t>(5) - r11;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrldi r8,r8,32
  r8 = r8 & 0xFFFFFFFF;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // std r9,-64(r1)
  guest::StoreU64(base, uint32_t(r1) + -64, r9);
  // std r8,-72(r1)
  guest::StoreU64(base, uint32_t(r1) + -72, r8);
  // lfd f13,-64(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f12,-72(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -72)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f13,f13
  f13 = double(float(f13));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // lfs f11,-48(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + -48);
  f11 = double(temp.f32);
  // fmadds f13,f12,f11,f13
  f13 = double(float(std::fma(f12, f11, f13)));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r10)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // blt cr6,0x82421db0
  if (cr6.lt) goto loc_82421DB0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-24(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -24, temp.u32);
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,-20(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -20, temp.u32);
loc_82421E1C:
  // lbz r10,-76(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + -76);
  // addi r9,r3,12
  r9 = uint64_t(int64_t(int64_t(r3) + 12));
  // lbz r8,-77(r1)
  r8 = guest::LoadU8(base, uint32_t(r1) + -77);
  // lbz r11,-78(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + -78);
  // rlwimi r8,r10,8,16,23
  r8 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFF00) | (r8 & 0xFFFFFFFFFFFF00FF);
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // clrlwi r8,r8,16
  r8 = uint32_t(r8) & 0xFFFF;
  // rlwimi r11,r8,8,0,23
  r11 = (__builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 8) & 0xFFFFFF00) | (r11 & 0xFFFFFFFF000000FF);
loc_82421E3C:
  // rlwinm r8,r11,2,27,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0x1C;
  // addi r7,r1,-48
  r7 = uint64_t(int64_t(int64_t(r1) + -48));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r11,r11,29,3,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x1FFFFFFF;
  // lfsx f0,r8,r7
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + uint32_t(r7));
  f0 = double(temp.f32);
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::DxtAlphaBlock(base, uint32_t(r9) - 12).pixel0_alpha_address(), temp.u32);
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // bne 0x82421e3c
  if (!cr0.eq) goto loc_82421E3C;
  // lbz r10,-73(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + -73);
  // addi r9,r3,140
  r9 = uint64_t(int64_t(int64_t(r3) + 140));
  // lbz r8,-74(r1)
  r8 = guest::LoadU8(base, uint32_t(r1) + -74);
  // lbz r11,-75(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + -75);
  // rlwimi r8,r10,8,16,23
  r8 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFF00) | (r8 & 0xFFFFFFFFFFFF00FF);
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // clrlwi r8,r8,16
  r8 = uint32_t(r8) & 0xFFFF;
  // rlwimi r11,r8,8,0,23
  r11 = (__builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 8) & 0xFFFFFF00) | (r11 & 0xFFFFFFFF000000FF);
loc_82421E7C:
  // rlwinm r8,r11,2,27,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0x1C;
  // addi r7,r1,-48
  r7 = uint64_t(int64_t(int64_t(r1) + -48));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r11,r11,29,3,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x1FFFFFFF;
  // lfsx f0,r8,r7
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + uint32_t(r7));
  f0 = double(temp.f32);
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::DxtAlphaBlock(base, uint32_t(r9) - 140).pixel8_alpha_address(), temp.u32);
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // bne 0x82421e7c
  if (!cr0.eq) goto loc_82421E7C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // ld r30,-16(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -16);
  // ld r31,-8(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -8);
  // blr
  return r3;
}

// sub_82422738 game::Dxt23::DecodeBlockStraight: lifted from generated/ by lift.py.
uint64_t f82422738(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82422738 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // addi r4,r30,8
  r4 = uint64_t(int64_t(int64_t(r30) + 8));
  // bl 0x82421938
  // (left out: dead in C++)
  r3 = game::DxtColor::DecodeBlock(base, r1, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8242277c
  if (cr0.lt) goto loc_8242277C;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82421bf0
  // (left out: dead in C++)
  r3 = game::Dxt3A::DecodeBlock(base, r1, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8242277c
  if (cr0.lt) goto loc_8242277C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8242277C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82422798 game::Dxt45::DecodeBlockStraight: lifted from generated/ by lift.py.
uint64_t f82422798(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82422798 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x82421938
  // (left out: dead in C++)
  r3 = game::DxtColor::DecodeBlock(base, r1, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x824227d8
  if (cr0.lt) goto loc_824227D8;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82421c88
  // (left out: dead in C++)
  r3 = game::DxtAlpha::DecodeBlock(base, r1, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x824227d8
  if (cr0.lt) goto loc_824227D8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_824227D8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82422848 game::Dxt23::DecodeBlockPremultiplied: lifted from generated/ by lift.py.
uint64_t f82422848(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82422848 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x82422738
  // (left out: dead in C++)
  r3 = game::Dxt23::DecodeBlockStraight(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8242287c
  if (cr0.lt) goto loc_8242287C;
  // mr r3,r31
  r3 = r31;
  // bl 0x82420640
  // (left out: dead in C++)
  r3 = game::Dxt::Unpremultiply(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8242287c
  if (cr0.lt) goto loc_8242287C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8242287C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82422890 game::Dxt45::DecodeBlockPremultiplied: lifted from generated/ by lift.py.
uint64_t f82422890(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82422890 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x82422798
  // (left out: dead in C++)
  r3 = game::Dxt45::DecodeBlockStraight(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x824228c4
  if (cr0.lt) goto loc_824228C4;
  // mr r3,r31
  r3 = r31;
  // bl 0x82420640
  // (left out: dead in C++)
  r3 = game::Dxt::Unpremultiply(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x824228c4
  if (cr0.lt) goto loc_824228C4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_824228C4:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_824228D8 game::Dxt23::EncodeBlockPremultiplied: lifted from generated/ by lift.py.
lifted::Result f824228D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_824228D8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-352(r1)
  ea = -352 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r9,r1,88
  r9 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r10,r4,12
  r10 = uint64_t(int64_t(int64_t(r4) + 12));
  // subf r7,r9,r4
  r7 = r4 - r9;
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // li r9,16
  r9 = uint64_t(int64_t(16));
loc_824228F8:
  // addi r8,r10,-4
  r8 = uint64_t(int64_t(int64_t(r10) + -4));
  // lfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r10) - 12).alpha_address());
  f0 = double(temp.f32);
  // lfsx f13,r7,r11
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + uint32_t(r11));
  f13 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 4, temp.u32);
  // stfs f13,-8(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r11) + -8, temp.u32);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // lfs f12,-4(r8)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r8) - 8).green_address());
  f12 = double(temp.f32);
  // lfs f11,0(r8)
  temp.u32 = guest::LoadU32(base, schema::TcFloat4(base, uint32_t(r8) - 8).blue_address());
  f11 = double(temp.f32);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f12,-4(r11)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r11) + -4, temp.u32);
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // bne 0x824228f8
  if (!cr0.eq) goto loc_824228F8;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x824227f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Dxt23::EncodeBlockStraight(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8242294c
  if (cr0.lt) goto loc_8242294C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8242294C:
  // addi r1,r1,352
  r1 = uint64_t(int64_t(int64_t(r1) + 352));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82422A20 game::Dxt23::DecodeBlock: lifted from generated/ by lift.py.
uint64_t f82422A20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82422A20 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,18016(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 18016);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82422a34
  if (cr6.eq) goto loc_82422A34;
  // b 0x82422848
  r3 = game::Dxt23::DecodeBlockPremultiplied(base, r1, r13, r3, r4);
  return r3;
loc_82422A34:
  // b 0x82422738
  r3 = game::Dxt23::DecodeBlockStraight(base, r1, r13, r3, r4);
  return r3;
}

// sub_82422A38 game::Dxt45::DecodeBlock: lifted from generated/ by lift.py.
uint64_t f82422A38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82422A38 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,18016(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 18016);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82422a4c
  if (cr6.eq) goto loc_82422A4C;
  // b 0x82422890
  r3 = game::Dxt45::DecodeBlockPremultiplied(base, r1, r13, r3, r4);
  return r3;
loc_82422A4C:
  // b 0x82422798
  r3 = game::Dxt45::DecodeBlockStraight(base, r1, r13, r3, r4);
  return r3;
}

// sub_82422A50 game::Dxt23::EncodeBlock: lifted from generated/ by lift.py.
lifted::Result f82422A50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82422A50 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,18016(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 18016);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82422a64
  if (cr6.eq) goto loc_82422A64;
  // b 0x824228d8
  { const lifted::Result o = game::Dxt23::EncodeBlockPremultiplied(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82422A64:
  // b 0x824227f0
  { const lifted::Result o = game::Dxt23::EncodeBlockStraight(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8243DC88: lifted from generated/ by lift.py.
void f8243DC88(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8243DC88 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // blr
  return;
}

// sub_8244A528: lifted from generated/ by lift.py.
uint64_t f8244A528(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8244A528 (lifted)");
  uint64_t r3 = 0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
}

// sub_82459C20 game::socket: lifted from generated/ by lift.py.
lifted::Result f82459C20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459C20 (lifted)");
  uint64_t r6 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // mr r4,r3
  r4 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825dbc44
  { const lifted::Result o = lifted::Import(base, __imp__NetDll_socket, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82459C38 game::closesocket: lifted from generated/ by lift.py.
lifted::Result f82459C38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459C38 (lifted)");
  uint64_t r4 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r4,r3
  r4 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825dbc54
  { const lifted::Result o = lifted::Import(base, __imp__NetDll_closesocket, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82459C48 game::ioctlsocket: lifted from generated/ by lift.py.
lifted::Result f82459C48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459C48 (lifted)");
  uint64_t r6 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // mr r4,r3
  r4 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825dbc64
  { const lifted::Result o = lifted::Import(base, __imp__NetDll_ioctlsocket, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82459C60 game::bind: lifted from generated/ by lift.py.
lifted::Result f82459C60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459C60 (lifted)");
  uint64_t r6 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // mr r4,r3
  r4 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825dbc74
  { const lifted::Result o = lifted::Import(base, __imp__NetDll_bind, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82459C90 game::listen: lifted from generated/ by lift.py.
lifted::Result f82459C90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459C90 (lifted)");
  uint64_t r5 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r5,r4
  r5 = r4;
  // mr r4,r3
  r4 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825dbc94
  { const lifted::Result o = lifted::Import(base, __imp__NetDll_listen, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8245A4E8 game::XAudioShutDown: lifted from generated/ by lift.py.
lifted::Result f8245A4E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8245A4E8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r31,r11,-10988
  r31 = uint64_t(int64_t(int64_t(r11) + -10988));
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x8245A50C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r3,20232(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 20232);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x8245a52c
  if (!cr6.eq) goto loc_8245A52C;
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x8245A524, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8245a54c
  goto loc_8245A54C;
loc_8245A52C:
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245A53C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x8245A548, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
loc_8245A54C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245A668: lifted from generated/ by lift.py.
lifted::Result f8245A668(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8245A668 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // mr r30,r3
  r30 = r3;
  // addi r31,r11,-10988
  r31 = uint64_t(int64_t(int64_t(r11) + -10988));
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x8245A690, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r3,20232(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 20232);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x8245a6b4
  if (!cr6.eq) goto loc_8245A6B4;
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x8245A6A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x8245a6cc
  goto loc_8245A6CC;
loc_8245A6B4:
  // mr r4,r30
  r4 = r30;
  // bl 0x8245b3a0
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioEngine::UnregisterCallback(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x8245A6C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
loc_8245A6CC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245AA48: lifted from generated/ by lift.py.
lifted::Result f8245AA48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8245AA48 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r30,r11,-11048
  r30 = uint64_t(int64_t(int64_t(r11) + -11048));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x8245AA6C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r31,r11,20148
  r31 = uint64_t(int64_t(int64_t(r11) + 20148));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245aa94
  if (cr6.eq) goto loc_8245AA94;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825dba84
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ExRegisterTitleTerminateNotification, LIFTED_ARGS(0x8245AA8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8245AA94:
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x8245AA9C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245AFA0: lifted from generated/ by lift.py.
lifted::Result f8245AFA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8245AFA0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // lwz r11,256(r13)
  r11 = guest::LoadU32(base, uint32_t(r13) + 256);
  // mr r26,r4
  r26 = r4;
  // lwz r10,300(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 300);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8245b0c8
  if (cr6.eq) goto loc_8245B0C8;
  // lbz r10,268(r13)
  r10 = guest::LoadU8(base, uint32_t(r13) + 268);
  // addi r10,r10,83
  r10 = uint64_t(int64_t(int64_t(r10) + 83));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r10,r27
  r10 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r27));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8245b0c8
  if (cr6.eq) goto loc_8245B0C8;
  // lwz r11,296(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 296);
  // cmplw cr6,r26,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r11), so);
  // bne cr6,0x8245b0c8
  if (!cr6.eq) goto loc_8245B0C8;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r28,r11,-10988
  r28 = uint64_t(int64_t(int64_t(r11) + -10988));
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
loc_8245B000:
  // mr r9,r13
  r9 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245b040
  if (cr6.eq) goto loc_8245B040;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bne cr6,0x8245b040
  if (!cr6.eq) goto loc_8245B040;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8245b040
  if (!cr6.eq) goto loc_8245B040;
  // lbz r30,12(r31)
  r30 = guest::LoadU8(base, uint32_t(r31) + 12);
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8245B038, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245B040, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245B040:
  // lwz r30,24(r28)
  r30 = guest::LoadU32(base, uint32_t(r28) + 24);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8245b078
  if (cr6.eq) goto loc_8245B078;
  // mr r29,r30
  r29 = r30;
loc_8245B050:
  // addi r3,r28,4
  r3 = uint64_t(int64_t(int64_t(r28) + 4));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x8245B058, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x8245b050
  if (!cr6.eq) goto loc_8245B050;
loc_8245B064:
  // addi r3,r28,4
  r3 = uint64_t(int64_t(int64_t(r28) + 4));
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x8245B06C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8245b064
  if (!cr6.eq) goto loc_8245B064;
loc_8245B078:
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245B07C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r29,r3
  r29 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245b09c
  if (cr6.eq) goto loc_8245B09C;
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // beq cr6,0x8245b0b4
  if (cr6.eq) goto loc_8245B0B4;
loc_8245B09C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245B0A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r30
  r10 = r30;
  // stw r10,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r10));
  // stb r29,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r29));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
loc_8245B0B4:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // lwz r9,296(r27)
  r9 = guest::LoadU32(base, uint32_t(r27) + 296);
  // cmplw cr6,r26,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r9), so);
  // beq cr6,0x8245b000
  if (cr6.eq) goto loc_8245B000;
loc_8245B0C8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8245B0D0: lifted from generated/ by lift.py.
lifted::Result f8245B0D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8245B0D0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,20148
  r11 = uint64_t(int64_t(int64_t(r11) + 20148));
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // bne cr6,0x8245b104
  if (!cr6.eq) goto loc_8245B104;
  // bl 0x8245aa48
  // (left out: dead in C++)
  { const lifted::Result o = f8245AA48(base, r1, r13, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r11,20164
  r3 = uint64_t(int64_t(int64_t(r11) + 20164));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x825dbbd4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeSetEvent, LIFTED_ARGS(0x8245B104, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245B104:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8245B118: lifted from generated/ by lift.py.
lifted::Result f8245B118(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8245B118 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-22856
  r11 = uint64_t(int64_t(int64_t(r11) + -22856));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::XAudioRootObject(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq cr6,0x8245b158
  if (cr6.eq) goto loc_8245B158;
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
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x8245B158, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245B158:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245B170: lifted from generated/ by lift.py.
lifted::Result f8245B170(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8245B170 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r30,r4
  r30 = r4;
  // addi r11,r11,-22836
  r11 = uint64_t(int64_t(int64_t(r11) + -22836));
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, schema::XAudioEngineBase(base, uint32_t(r31)).allocator_address());
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::XAudioEngineBase(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq cr6,0x8245b1bc
  if (cr6.eq) goto loc_8245B1BC;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::XAudioBumpAllocator(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::XAudioBumpAllocatorVtable(base, uint32_t(r11)).release_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245B1B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,8(r31)
  guest::StoreU32(base, schema::XAudioEngineBase(base, uint32_t(r31)).allocator_address(), uint32_t(r11));
loc_8245B1BC:
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // clrlwi r10,r30,31
  r10 = uint32_t(r30) & 0x1;
  // addi r11,r11,-22856
  r11 = uint64_t(int64_t(int64_t(r11) + -22856));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::XAudioEngineBase(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq cr6,0x8245b1e8
  if (cr6.eq) goto loc_8245B1E8;
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
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x8245B1E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245B1E8:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245B3A0 game::XAudioEngine::UnregisterCallback: lifted from generated/ by lift.py.
lifted::Result f8245B3A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8245B3A0 (lifted)");
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // addi r11,r27,168
  r11 = uint64_t(int64_t(int64_t(r27) + 168));
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // blt cr6,0x8245b3cc
  if (cr6.lt) goto loc_8245B3CC;
  // addi r11,r27,296
  r11 = uint64_t(int64_t(int64_t(r27) + 296));
  // mr r29,r4
  r29 = r4;
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // blt cr6,0x8245b3d0
  if (cr6.lt) goto loc_8245B3D0;
loc_8245B3CC:
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_8245B3D0:
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245B3D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r28,r3
  r28 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245b3fc
  if (cr6.eq) goto loc_8245B3FC;
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // beq cr6,0x8245b410
  if (cr6.eq) goto loc_8245B410;
loc_8245B3FC:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245B404, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r30));
  // stb r28,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r28));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
loc_8245B410:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r4,r29
  r4 = r29;
  // mr r3,r27
  r3 = r27;
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bl 0x8245afa0
  // (left out: dead in C++)
  { const lifted::Result o = f8245AFA0(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r10,r13
  r10 = r13;
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245b474
  if (cr6.eq) goto loc_8245B474;
  // lwz r9,8(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8245b474
  if (!cr6.eq) goto loc_8245B474;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8245b474
  if (!cr6.eq) goto loc_8245B474;
  // lbz r30,12(r31)
  r30 = guest::LoadU8(base, uint32_t(r31) + 12);
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8245B46C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245B474, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245B474:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8245B940 game::XAudioEngine::Dtor: lifted from generated/ by lift.py.
lifted::Result f8245B940(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8245B940 (lifted)");
  uint64_t r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r23,r3
  r23 = r3;
  // addi r11,r11,-22816
  r11 = uint64_t(int64_t(int64_t(r11) + -22816));
  // stw r11,0(r23)
  guest::StoreU32(base, schema::XAudioEngine(base, uint32_t(r23)).vtable_address(), uint32_t(r11));
  // bl 0x8245f5e8
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudio::UnregisterStats(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,64(r23)
  r11 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r23)).mastering_voice_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245b984
  if (cr6.eq) goto loc_8245B984;
  // lwz r3,68(r11)
  r3 = guest::LoadU32(base, schema::XAudioMasteringVoice(base, uint32_t(r11)).driver_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::XAudioMasteringDriver(base, uint32_t(r3)).vtable_address());
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, schema::XAudioMasteringDriverVtable(base, uint32_t(r11)).register_callback_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245B984, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245B984:
  // lwz r11,304(r23)
  r11 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r23)).processor_count_address());
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245b9e4
  if (cr6.eq) goto loc_8245B9E4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r11,20216
  r3 = uint64_t(int64_t(int64_t(r11) + 20216));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x825dbbd4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeSetEvent, LIFTED_ARGS(0x8245B9A8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r23,308
  r31 = uint64_t(int64_t(int64_t(r23) + 308));
  // li r30,6
  r30 = uint64_t(int64_t(6));
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
loc_8245B9B4:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r31) - 308).processor_threads_address());
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8245b9d4
  if (cr6.eq) goto loc_8245B9D4;
  // mr r4,r29
  r4 = r29;
  // bl 0x822970a0
  // (left out: dead in C++)
  { const lifted::Result o = game::WaitForSingleObject(base, r1, r13, r3, r4, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r31) - 308).processor_threads_address());
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x8245B9D0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r22,0(r31)
  guest::StoreU32(base, schema::XAudioEngine(base, uint32_t(r31) - 308).processor_threads_address(), uint32_t(r22));
loc_8245B9D4:
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8245b9b4
  if (!cr6.eq) goto loc_8245B9B4;
loc_8245B9E4:
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r30,r11,-11048
  r30 = uint64_t(int64_t(int64_t(r11) + -11048));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x8245B9F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r31,r11,20148
  r31 = uint64_t(int64_t(int64_t(r11) + 20148));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245ba1c
  if (cr6.eq) goto loc_8245BA1C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825dba84
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ExRegisterTitleTerminateNotification, LIFTED_ARGS(0x8245BA14, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r22
  r11 = r22;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8245BA1C:
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x8245BA24, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245BA28, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r27,r3
  r27 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245ba50
  if (cr6.eq) goto loc_8245BA50;
  // lwz r29,8(r31)
  r29 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // beq cr6,0x8245ba6c
  if (cr6.eq) goto loc_8245BA6C;
loc_8245BA50:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245BA58, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r30
  r29 = r30;
  // stw r29,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r29));
  // stb r27,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r27));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // b 0x8245ba70
  goto loc_8245BA70;
loc_8245BA6C:
  // lbz r27,12(r31)
  r27 = guest::LoadU8(base, uint32_t(r31) + 12);
loc_8245BA70:
  // addi r30,r11,1
  r30 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r21,r23,80
  r21 = uint64_t(int64_t(int64_t(r23) + 80));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r21
  r3 = r21;
  // stw r30,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r30));
  // bl 0x82466ea0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82466EA0, LIFTED_ARGS(0x8245BA8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x8245bb58
  if (cr6.eq) goto loc_8245BB58;
loc_8245BA98:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x8245badc
  if (cr6.eq) goto loc_8245BADC;
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x8245badc
  if (!cr6.eq) goto loc_8245BADC;
  // addi r11,r30,-1
  r11 = uint64_t(int64_t(int64_t(r30) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8245badc
  if (!cr6.eq) goto loc_8245BADC;
  // mr r11,r22
  r11 = r22;
  // mr r3,r31
  r3 = r31;
  // mr r30,r27
  r30 = r27;
  // stb r11,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8245BAD4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245BADC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245BADC:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::XAudioList(base, uint32_t(r28)).next_address());
  // mr r3,r28
  r3 = r28;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::XAudioTeardownNodeData(base, uint32_t(r11)).unknown_12_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245BAF0, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245BAF4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r27,r3
  r27 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245bb14
  if (cr6.eq) goto loc_8245BB14;
  // lwz r29,8(r31)
  r29 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // beq cr6,0x8245bb30
  if (cr6.eq) goto loc_8245BB30;
loc_8245BB14:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245BB1C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r30
  r29 = r30;
  // stw r29,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r29));
  // stb r27,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r27));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // b 0x8245bb34
  goto loc_8245BB34;
loc_8245BB30:
  // lbz r27,12(r31)
  r27 = guest::LoadU8(base, uint32_t(r31) + 12);
loc_8245BB34:
  // addi r30,r11,1
  r30 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r21
  r3 = r21;
  // stw r30,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r30));
  // bl 0x82466ea0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82466EA0, LIFTED_ARGS(0x8245BB4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x8245ba98
  if (!cr6.eq) goto loc_8245BA98;
loc_8245BB58:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x8245bb98
  if (cr6.eq) goto loc_8245BB98;
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x8245bb98
  if (!cr6.eq) goto loc_8245BB98;
  // addi r11,r30,-1
  r11 = uint64_t(int64_t(int64_t(r30) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8245bb98
  if (!cr6.eq) goto loc_8245BB98;
  // mr r11,r22
  r11 = r22;
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8245BB90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245BB98, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245BB98:
  // lbz r11,128(r23)
  r11 = guest::LoadU8(base, schema::XAudioEngine(base, uint32_t(r23)).submix_stage_count_address());
  // mr r24,r22
  r24 = r22;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245bd2c
  if (cr6.eq) goto loc_8245BD2C;
  // mr r25,r22
  r25 = r22;
loc_8245BBAC:
  // lwz r11,124(r23)
  r11 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r23)).submix_stages_address());
  // add r26,r25,r11
  r26 = r25 + r11;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245BBB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r27,r3
  r27 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245bbd8
  if (cr6.eq) goto loc_8245BBD8;
  // lwz r29,8(r31)
  r29 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // beq cr6,0x8245bbf4
  if (cr6.eq) goto loc_8245BBF4;
loc_8245BBD8:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245BBE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r30
  r29 = r30;
  // stw r29,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r29));
  // stb r27,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r27));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // b 0x8245bbf8
  goto loc_8245BBF8;
loc_8245BBF4:
  // lbz r27,12(r31)
  r27 = guest::LoadU8(base, uint32_t(r31) + 12);
loc_8245BBF8:
  // addi r30,r11,1
  r30 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // stw r30,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r30));
  // bl 0x82466ea0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82466EA0, LIFTED_ARGS(0x8245BC10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x8245bcd8
  if (cr6.eq) goto loc_8245BCD8;
loc_8245BC1C:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x8245bc5c
  if (cr6.eq) goto loc_8245BC5C;
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x8245bc5c
  if (!cr6.eq) goto loc_8245BC5C;
  // addi r11,r30,-1
  r11 = uint64_t(int64_t(int64_t(r30) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8245bc5c
  if (!cr6.eq) goto loc_8245BC5C;
  // mr r11,r22
  r11 = r22;
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8245BC54, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245BC5C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245BC5C:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::XAudioList(base, uint32_t(r28)).next_address());
  // mr r3,r28
  r3 = r28;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::XAudioTeardownNodeData(base, uint32_t(r11)).unknown_12_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245BC70, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245BC74, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r27,r3
  r27 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245bc94
  if (cr6.eq) goto loc_8245BC94;
  // lwz r29,8(r31)
  r29 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // beq cr6,0x8245bcb0
  if (cr6.eq) goto loc_8245BCB0;
loc_8245BC94:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245BC9C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r30
  r29 = r30;
  // stw r29,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r29));
  // stb r27,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r27));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // b 0x8245bcb4
  goto loc_8245BCB4;
loc_8245BCB0:
  // lbz r27,12(r31)
  r27 = guest::LoadU8(base, uint32_t(r31) + 12);
loc_8245BCB4:
  // addi r30,r11,1
  r30 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // stw r30,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r30));
  // bl 0x82466ea0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82466EA0, LIFTED_ARGS(0x8245BCCC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x8245bc1c
  if (!cr6.eq) goto loc_8245BC1C;
loc_8245BCD8:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x8245bd18
  if (cr6.eq) goto loc_8245BD18;
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x8245bd18
  if (!cr6.eq) goto loc_8245BD18;
  // addi r11,r30,-1
  r11 = uint64_t(int64_t(int64_t(r30) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8245bd18
  if (!cr6.eq) goto loc_8245BD18;
  // mr r11,r22
  r11 = r22;
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8245BD10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245BD18, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245BD18:
  // lbz r11,128(r23)
  r11 = guest::LoadU8(base, schema::XAudioEngine(base, uint32_t(r23)).submix_stage_count_address());
  // addi r24,r24,1
  r24 = uint64_t(int64_t(int64_t(r24) + 1));
  // addi r25,r25,44
  r25 = uint64_t(int64_t(int64_t(r25) + 44));
  // cmplw cr6,r24,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r11), so);
  // blt cr6,0x8245bbac
  if (cr6.lt) goto loc_8245BBAC;
loc_8245BD2C:
  // lwz r3,64(r23)
  r3 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r23)).mastering_voice_address());
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8245bd4c
  if (cr6.eq) goto loc_8245BD4C;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::XAudioMasteringVoice(base, uint32_t(r3)).vtable_address());
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::XAudioMasteringVoiceVtable(base, uint32_t(r11)).delete_object_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245BD48, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r22,64(r23)
  guest::StoreU32(base, schema::XAudioEngine(base, uint32_t(r23)).mastering_voice_address(), uint32_t(r22));
loc_8245BD4C:
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // mr r11,r22
  r11 = r22;
  // stw r11,20232(r10)
  guest::StoreU32(base, uint32_t(r10) + 20232, uint32_t(r11));
  // lwz r3,60(r23)
  r3 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r23)).factory_address());
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8245bd6c
  if (cr6.eq) goto loc_8245BD6C;
  // bl 0x8245f6d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245F6D0, LIFTED_ARGS(0x8245BD68, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r22,60(r23)
  guest::StoreU32(base, schema::XAudioEngine(base, uint32_t(r23)).factory_address(), uint32_t(r22));
loc_8245BD6C:
  // addi r31,r23,12
  r31 = uint64_t(int64_t(int64_t(r23) + 12));
  // li r29,6
  r29 = uint64_t(int64_t(6));
loc_8245BD74:
  // li r30,2
  r30 = uint64_t(int64_t(2));
loc_8245BD78:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r31) - 12).mix_buffers_address());
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8245bd8c
  if (cr6.eq) goto loc_8245BD8C;
  // bl 0x8245f6d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245F6D0, LIFTED_ARGS(0x8245BD88, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r22,0(r31)
  guest::StoreU32(base, schema::XAudioEngine(base, uint32_t(r31) - 12).mix_buffers_address(), uint32_t(r22));
loc_8245BD8C:
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8245bd78
  if (!cr6.eq) goto loc_8245BD78;
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x8245bd74
  if (!cr6.eq) goto loc_8245BD74;
  // addi r3,r21,24
  r3 = uint64_t(int64_t(int64_t(r21) + 24));
  // bl 0x8246e060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8246E060, LIFTED_ARGS(0x8245BDB0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r21,12
  r3 = uint64_t(int64_t(int64_t(r21) + 12));
  // bl 0x8246e060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8246E060, LIFTED_ARGS(0x8245BDB8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r21
  r3 = r21;
  // bl 0x8246e060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8246E060, LIFTED_ARGS(0x8245BDC0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r23,68
  r3 = uint64_t(int64_t(int64_t(r23) + 68));
  // bl 0x8246e060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8246E060, LIFTED_ARGS(0x8245BDC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lwz r3,8(r23)
  r3 = guest::LoadU32(base, schema::XAudioEngine(base, uint32_t(r23)).allocator_address());
  // addi r11,r11,-22836
  r11 = uint64_t(int64_t(int64_t(r11) + -22836));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r23)
  guest::StoreU32(base, schema::XAudioEngine(base, uint32_t(r23)).vtable_address(), uint32_t(r11));
  // beq cr6,0x8245bdf4
  if (cr6.eq) goto loc_8245BDF4;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::XAudioBumpAllocator(base, uint32_t(r3)).vtable_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::XAudioBumpAllocatorVtable(base, uint32_t(r11)).release_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245BDF0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r22,8(r23)
  guest::StoreU32(base, schema::XAudioEngine(base, uint32_t(r23)).allocator_address(), uint32_t(r22));
loc_8245BDF4:
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // addi r11,r11,-22856
  r11 = uint64_t(int64_t(int64_t(r11) + -22856));
  // stw r11,0(r23)
  guest::StoreU32(base, schema::XAudioEngine(base, uint32_t(r23)).vtable_address(), uint32_t(r11));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8245C168: lifted from generated/ by lift.py.
lifted::Result f8245C168(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8245C168 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x8245b940
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioEngine::Dtor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245D760: lifted from generated/ by lift.py.
lifted::Result f8245D760(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8245D760 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r4
  r27 = r4;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // mr r26,r25
  r26 = r25;
  // mr r11,r25
  r11 = r25;
  // lbz r10,0(r27)
  r10 = guest::LoadU8(base, schema::XAudioClassTable(base, uint32_t(r27)).count_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8245d834
  if (cr6.eq) goto loc_8245D834;
  // mr r8,r25
  r8 = r25;
loc_8245D790:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bne cr6,0x8245d7a0
  if (!cr6.eq) goto loc_8245D7A0;
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address());
loc_8245D7A0:
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_class_count_address());
  // lwz r7,20(r31)
  r7 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address());
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r10,r10,r7
  r10 = r10 + r7;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x8245d7ec
  if (!cr6.lt) goto loc_8245D7EC;
loc_8245D7B8:
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::XAudioAllocBlock(base, uint32_t(r11)).interface_vtable_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8245d7d0
  if (!cr6.eq) goto loc_8245D7D0;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::XAudioAllocBlock(base, uint32_t(r11)).vtable_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8245d7f0
  if (cr6.eq) goto loc_8245D7F0;
loc_8245D7D0:
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_class_count_address());
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // lwz r9,20(r31)
  r9 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address());
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r10,r10,r9
  r10 = r10 + r9;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8245d7b8
  if (cr6.lt) goto loc_8245D7B8;
loc_8245D7EC:
  // mr r11,r25
  r11 = r25;
loc_8245D7F0:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245d834
  if (cr6.eq) goto loc_8245D834;
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, schema::XAudioClassTable(base, uint32_t(r27)).entries_address());
  // subf r9,r7,r11
  r9 = r11 - r7;
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // add r10,r10,r8
  r10 = r10 + r8;
  // srawi r9,r9,3
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x7) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 3));
  // addi r8,r8,12
  r8 = uint64_t(int64_t(int64_t(r8) + 12));
  // addi r9,r9,128
  r9 = uint64_t(int64_t(int64_t(r9) + 128));
  // lwz r7,4(r10)
  r7 = guest::LoadU32(base, schema::XAudioClassTableEntry(base, uint32_t(r10)).first_function_address());
  // stb r9,0(r10)
  guest::StoreU8(base, schema::XAudioClassTableEntry(base, uint32_t(r10)).id_address(), uint8_t(r9));
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // lwz r10,8(r10)
  r10 = guest::LoadU32(base, schema::XAudioClassTableEntry(base, uint32_t(r10)).second_function_address());
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lbz r10,0(r27)
  r10 = guest::LoadU8(base, schema::XAudioClassTable(base, uint32_t(r27)).count_address());
  // cmplw cr6,r26,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r10), so);
  // blt cr6,0x8245d790
  if (cr6.lt) goto loc_8245D790;
loc_8245D834:
  // lbz r11,0(r27)
  r11 = guest::LoadU8(base, schema::XAudioClassTable(base, uint32_t(r27)).count_address());
  // subf r30,r26,r11
  r30 = r11 - r26;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8245d850
  if (!cr6.eq) goto loc_8245D850;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245D850:
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_class_count_address());
  // add r11,r11,r30
  r11 = r11 + r30;
  // cmplwi cr6,r11,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 128, so);
  // ble cr6,0x8245d870
  if (!cr6.gt) goto loc_8245D870;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245D870:
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r28,r10,20256
  r28 = uint64_t(int64_t(int64_t(r10) + 20256));
  // ori r5,r5,4
  r5 = r5 | 4;
  // rlwinm r4,r11,3,0,28
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // mr r3,r28
  r3 = r28;
  // bl 0x8245ef80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF80, LIFTED_ARGS(0x8245D88C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x8245d8ac
  if (!cr6.eq) goto loc_8245D8AC;
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // ori r25,r25,14
  r25 = r25 | 14;
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245D8AC:
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_class_count_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245d8e8
  if (cr6.eq) goto loc_8245D8E8;
  // rlwinm r5,r11,3,0,28
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // lwz r4,20(r31)
  r4 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address());
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8245D8C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,20(r31)
  r4 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address());
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x8245d8e8
  if (cr6.eq) goto loc_8245D8E8;
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // mr r3,r28
  r3 = r28;
  // ori r5,r5,4
  r5 = r5 | 4;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x8245D8E4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r25,20(r31)
  guest::StoreU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address(), uint32_t(r25));
loc_8245D8E8:
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_class_count_address());
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // stw r29,20(r31)
  guest::StoreU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address(), uint32_t(r29));
  // add r10,r11,r30
  r10 = r11 + r30;
  // mr r11,r25
  r11 = r25;
  // stw r10,16(r31)
  guest::StoreU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_class_count_address(), uint32_t(r10));
  // beq cr6,0x8245d960
  if (cr6.eq) goto loc_8245D960;
  // rlwinm r10,r26,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 1) & 0xFFFFFFFE;
  // add r10,r26,r10
  r10 = r26 + r10;
  // rlwinm r8,r10,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
loc_8245D910:
  // lwz r9,16(r31)
  r9 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_class_count_address());
  // lwz r7,20(r31)
  r7 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address());
  // subf r9,r30,r9
  r9 = r9 - r30;
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, schema::XAudioClassTable(base, uint32_t(r27)).entries_address());
  // add r9,r9,r11
  r9 = r9 + r11;
  // add r10,r10,r8
  r10 = r10 + r8;
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r9,r9,r29
  r9 = r9 + r29;
  // addi r8,r8,12
  r8 = uint64_t(int64_t(int64_t(r8) + 12));
  // subf r7,r7,r9
  r7 = r9 - r7;
  // lwz r6,4(r10)
  r6 = guest::LoadU32(base, uint32_t(r10) + 4);
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // srawi r7,r7,3
  ca = (int32_t(r7) < 0) & ((uint32_t(r7) & 0x7) != 0);
  r7 = uint64_t(int64_t(int32_t(r7) >> 3));
  // addi r7,r7,128
  r7 = uint64_t(int64_t(int64_t(r7) + 128));
  // stb r7,0(r10)
  guest::StoreU8(base, uint32_t(r10) + 0, uint8_t(r7));
  // stw r6,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r6));
  // lwz r10,8(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 8);
  // stw r10,4(r9)
  guest::StoreU32(base, uint32_t(r9) + 4, uint32_t(r10));
  // blt cr6,0x8245d910
  if (cr6.lt) goto loc_8245D910;
loc_8245D960:
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8245D970: lifted from generated/ by lift.py.
uint64_t f8245D970(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8245D970 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r11,0(r4)
  r11 = guest::LoadU8(base, schema::XAudioClassTable(base, uint32_t(r4)).count_address());
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r9,r8
  r9 = r8;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245d9e0
  if (cr6.eq) goto loc_8245D9E0;
loc_8245D984:
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, schema::XAudioClassTable(base, uint32_t(r4)).entries_address());
  // lwz r10,16(r3)
  r10 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r3)).high_class_count_address());
  // lbzx r11,r9,r11
  r11 = guest::LoadU8(base, uint32_t(r9) + uint32_t(r11));
  // addi r11,r11,-128
  r11 = uint64_t(int64_t(int64_t(r11) + -128));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x8245d9d0
  if (!cr6.lt) goto loc_8245D9D0;
  // lwz r10,20(r3)
  r10 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r3)).high_classes_address());
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8245d9d0
  if (cr6.eq) goto loc_8245D9D0;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8245d9d0
  if (cr6.eq) goto loc_8245D9D0;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245d9d0
  if (cr6.eq) goto loc_8245D9D0;
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
loc_8245D9D0:
  // lbz r11,0(r4)
  r11 = guest::LoadU8(base, schema::XAudioClassTable(base, uint32_t(r4)).count_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // blt cr6,0x8245d984
  if (cr6.lt) goto loc_8245D984;
loc_8245D9E0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_8245DB60 game::XAudioFactory::Dtor: lifted from generated/ by lift.py.
lifted::Result f8245DB60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8245DB60 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // addi r11,r11,-22796
  r11 = uint64_t(int64_t(int64_t(r11) + -22796));
  // lwz r4,20(r31)
  r4 = guest::LoadU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address());
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::XAudioFactory(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq cr6,0x8245dba8
  if (cr6.eq) goto loc_8245DBA8;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // ori r5,r5,4
  r5 = r5 | 4;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x8245DBA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,20(r31)
  guest::StoreU32(base, schema::XAudioFactory(base, uint32_t(r31)).high_classes_address(), uint32_t(r11));
loc_8245DBA8:
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-22856
  r11 = uint64_t(int64_t(int64_t(r11) + -22856));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::XAudioFactory(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245E308 game::XAudioVoice::GetCallbackContext: lifted from generated/ by lift.py.
uint64_t f8245E308(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8245E308 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,48(r3)
  r11 = guest::LoadU32(base, schema::XAudioVoice(base, uint32_t(r3)).callback_context_address());
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r4)
  guest::StoreU32(base, schema::XAudioWordOut(base, uint32_t(r4)).value_address(), uint32_t(r11));
  // blr
  return r3;
}

// sub_8245ED60 game::XAudioLock: lifted from generated/ by lift.py.
lifted::Result f8245ED60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8245ED60 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // ori r11,r11,1
  r11 = r11 | 1;
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // bgt cr6,0x8245eebc
  if (cr6.gt) goto loc_8245EEBC;
  // beq cr6,0x8245eea0
  if (cr6.eq) goto loc_8245EEA0;
  // addis r11,r3,-1
  r11 = uint64_t(int64_t(int64_t(r3) + -65536));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8245ee4c
  if (cr0.eq) goto loc_8245EE4C;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x8245edf4
  if (cr6.eq) goto loc_8245EDF4;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x8245eed0
  if (!cr6.eq) goto loc_8245EED0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r10,r13
  r10 = r13;
  // addi r3,r11,20240
  r3 = uint64_t(int64_t(int64_t(r11) + 20240));
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245ede8
  if (cr6.eq) goto loc_8245EDE8;
  // lwz r9,8(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8245ede8
  if (!cr6.eq) goto loc_8245EDE8;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // bne cr6,0x8245ede8
  if (!cr6.eq) goto loc_8245EDE8;
  // lbz r31,12(r3)
  r31 = guest::LoadU8(base, uint32_t(r3) + 12);
  // stb r11,12(r3)
  guest::StoreU8(base, uint32_t(r3) + 12, uint8_t(r11));
  // stw r11,8(r3)
  guest::StoreU32(base, uint32_t(r3) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8245EDE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245EDE8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245EDE8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245EDF4:
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245EDF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r29,r3
  r29 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245ee20
  if (cr6.eq) goto loc_8245EE20;
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // beq cr6,0x8245ee8c
  if (cr6.eq) goto loc_8245EE8C;
loc_8245EE20:
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbf04
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeTryToAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245EE28, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8245ee80
  if (!cr6.eq) goto loc_8245EE80;
  // mr r3,r29
  r3 = r29;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245EE3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16388
  r3 = r3 | 16388;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245EE4C:
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245EE50, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r29,r3
  r29 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8245ee78
  if (cr6.eq) goto loc_8245EE78;
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // beq cr6,0x8245ee8c
  if (cr6.eq) goto loc_8245EE8C;
loc_8245EE78:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245EE80, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245EE80:
  // stw r30,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r30));
  // stb r29,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r29));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
loc_8245EE8C:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
loc_8245EE94:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245EEA0:
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r11,r11,-10988
  r11 = uint64_t(int64_t(int64_t(r11) + -10988));
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x8245EEB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245EEBC:
  // addis r11,r3,-2
  r11 = uint64_t(int64_t(int64_t(r3) + -131072));
  // addic. r11,r11,-2
  ca = uint32_t(r11) > 1;
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8245eefc
  if (cr0.eq) goto loc_8245EEFC;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x8245eee0
  if (cr6.eq) goto loc_8245EEE0;
loc_8245EED0:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245EEE0:
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r11,r11,-10988
  r11 = uint64_t(int64_t(int64_t(r11) + -10988));
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x8245EEF0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245EEFC:
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r11,r11,-10988
  r11 = uint64_t(int64_t(int64_t(r11) + -10988));
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x825dbef4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlTryEnterCriticalSection, LIFTED_ARGS(0x8245EF0C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x8245ee94
  if (!cr6.eq) goto loc_8245EE94;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16388
  r3 = r3 | 16388;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8245EF28: lifted from generated/ by lift.py.
lifted::Result f8245EF28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8245EF28 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-22424
  r11 = uint64_t(int64_t(int64_t(r11) + -22424));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::XAudioRootObject(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq cr6,0x8245ef68
  if (cr6.eq) goto loc_8245EF68;
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
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x8245EF68, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245EF68:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245F090 game::XAudioSubmixVoice::SetFormat: lifted from generated/ by lift.py.
lifted::Result f8245F090(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8245F090 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lbz r11,1(r4)
  r11 = guest::LoadU8(base, schema::XAudioVoiceFormat(base, uint32_t(r4)).channels_address());
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r3
  r31 = r3;
  // stb r11,81(r1)
  guest::StoreU8(base, uint32_t(r1) + 81, uint8_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stb r11,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r11));
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,48000
  r11 = r11 | 48000;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x82460778
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82460778, LIFTED_ARGS(0x8245F0CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x8245f0e8
  if (cr6.lt) goto loc_8245F0E8;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // lwz r3,32(r31)
  r3 = guest::LoadU32(base, schema::XAudioSubmixVoice(base, uint32_t(r31)).buffer_address());
  // addi r4,r31,52
  r4 = uint64_t(int64_t(int64_t(r31) + 52));
  // bl 0x8245fa10
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioBuffer::ClearThunk(base, LIFTED_ARGS(0x8245F0E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245F0E8:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245F218 game::XAudioSubmixVoice::GetState: lifted from generated/ by lift.py.
uint64_t f8245F218(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8245F218 (lifted)");
  uint64_t r11 = 0;
  // lbz r11,61(r3)
  r11 = guest::LoadU8(base, schema::XAudioSubmixVoice(base, uint32_t(r3)).flags_address());
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stb r11,0(r4)
  guest::StoreU8(base, schema::XAudioByteOut(base, uint32_t(r4)).value_address(), uint8_t(r11));
  // blr
  return r3;
}

// sub_8245F4E0: lifted from generated/ by lift.py.
lifted::Result f8245F4E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8245F4E0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // cmplwi cr6,r3,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 16, so);
  // beq cr6,0x8245f568
  if (cr6.eq) goto loc_8245F568;
  // cmplwi cr6,r3,17
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 17, so);
  // beq cr6,0x8245f560
  if (cr6.eq) goto loc_8245F560;
  // cmplwi cr6,r3,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 255, so);
  // bne cr6,0x8245f57c
  if (!cr6.eq) goto loc_8245F57C;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r29,r11,20288
  r29 = uint64_t(int64_t(int64_t(r11) + 20288));
  // mr r30,r29
  r30 = r29;
loc_8245F514:
  // addi r11,r29,28
  r11 = uint64_t(int64_t(int64_t(r29) + 28));
  // lbzx r11,r31,r11
  r11 = guest::LoadU8(base, uint32_t(r31) + uint32_t(r11));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245f544
  if (cr6.eq) goto loc_8245F544;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // oris r3,r31,4
  r3 = r31 | 262144;
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f1,f0
  f1 = double(float(f0));
  // bl 0x825b9ba8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825B9BA8(base, LIFTED_ARGS(0x8245F544, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245F544:
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // addi r11,r29,28
  r11 = uint64_t(int64_t(int64_t(r29) + 28));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x8245f514
  if (cr6.lt) goto loc_8245F514;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_8245F560:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // b 0x8245f56c
  goto loc_8245F56C;
loc_8245F568:
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_8245F56C:
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,20288
  r11 = uint64_t(int64_t(int64_t(r11) + 20288));
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // stbx r10,r11,r4
  guest::StoreU8(base, uint32_t(r11) + uint32_t(r4), uint8_t(r10));
loc_8245F57C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8245F5E8 game::XAudio::UnregisterStats: lifted from generated/ by lift.py.
lifted::Result f8245F5E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8245F5E8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r11,2032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 2032);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245f62c
  if (cr6.eq) goto loc_8245F62C;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r3,47
  r3 = uint64_t(int64_t(47));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245F62C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245F62C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8245FB10 game::XAudioMasteringVoice::Dtor: lifted from generated/ by lift.py.
lifted::Result f8245FB10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8245FB10 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-22296
  r11 = uint64_t(int64_t(int64_t(r11) + -22296));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::XAudioMasteringVoice(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // bl 0x824619b0
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioVoice::StopOrTail(base, LIFTED_ARGS(0x8245FB38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,68(r31)
  r11 = guest::LoadU32(base, schema::XAudioMasteringVoice(base, uint32_t(r31)).driver_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8245fb64
  if (cr6.eq) goto loc_8245FB64;
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::XAudioMasteringDriver(base, uint32_t(r3) - 4).inner_vtable_address());
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::XAudioMasteringDriverInnerVtable(base, uint32_t(r11)).delete_object_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8245FB5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,68(r31)
  guest::StoreU32(base, schema::XAudioMasteringVoice(base, uint32_t(r31)).driver_address(), uint32_t(r11));
loc_8245FB64:
  // mr r3,r31
  r3 = r31;
  // bl 0x82461240
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82461240, LIFTED_ARGS(0x8245FB6C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8245FC70: lifted from generated/ by lift.py.
lifted::Result f8245FC70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8245FC70 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x8245fb10
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioMasteringVoice::Dtor(base, r1, r13, r3, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

lifted::Result e822B2598(uint8_t* base, const lifted::Args& a) {
  f822B2598(base, a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822B4250(uint8_t* base, const lifted::Args& a) {
  return f822B4250(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822B42D0(uint8_t* base, const lifted::Args& a) {
  return f822B42D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822B4300(uint8_t* base, const lifted::Args& a) {
  return {f822B4300(base), a.f[0]};
}

lifted::Result e822B4318(uint8_t* base, const lifted::Args& a) {
  return {f822B4318(base), a.f[0]};
}

lifted::Result e822B4330(uint8_t* base, const lifted::Args& a) {
  return f822B4330(base, a.r1, a.r13, a.r[0], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822B4360(uint8_t* base, const lifted::Args& a) {
  return {f822B4360(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822B43A8(uint8_t* base, const lifted::Args& a) {
  return {f822B43A8(base), a.f[0]};
}

lifted::Result e822B43E8(uint8_t* base, const lifted::Args& a) {
  return f822B43E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822B4680(uint8_t* base, const lifted::Args& a) {
  return f822B4680(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822B4E78(uint8_t* base, const lifted::Args& a) {
  f822B4E78(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822B4F10(uint8_t* base, const lifted::Args& a) {
  return f822B4F10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822B58C0(uint8_t* base, const lifted::Args& a) {
  return f822B58C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822B60C8(uint8_t* base, const lifted::Args& a) {
  return f822B60C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822B6188(uint8_t* base, const lifted::Args& a) {
  return {f822B6188(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e822B6288(uint8_t* base, const lifted::Args& a) {
  return {f822B6288(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822B62E8(uint8_t* base, const lifted::Args& a) {
  return f822B62E8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822B6370(uint8_t* base, const lifted::Args& a) {
  return f822B6370(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e822B64A0(uint8_t* base, const lifted::Args& a) {
  return f822B64A0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822B6528(uint8_t* base, const lifted::Args& a) {
  return f822B6528(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822C3958(uint8_t* base, const lifted::Args& a) {
  f822C3958(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822F1218(uint8_t* base, const lifted::Args& a) {
  return {f822F1218(base), a.f[0]};
}

lifted::Result e82304258(uint8_t* base, const lifted::Args& a) {
  return {f82304258(base, a.r[1]), a.f[0]};
}

lifted::Result e82368D98(uint8_t* base, const lifted::Args& a) {
  return {f82368D98(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82368F58(uint8_t* base, const lifted::Args& a) {
  return {f82368F58(base, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]), a.f[0]};
}

lifted::Result e82369050(uint8_t* base, const lifted::Args& a) {
  return {f82369050(base, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]), a.f[0]};
}

lifted::Result e82369138(uint8_t* base, const lifted::Args& a) {
  return f82369138(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e82369188(uint8_t* base, const lifted::Args& a) {
  return f82369188(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e823691D8(uint8_t* base, const lifted::Args& a) {
  return {f823691D8(base, a.r[0]), a.f[0]};
}

lifted::Result e823692D0(uint8_t* base, const lifted::Args& a) {
  return {f823692D0(base, a.r[0]), a.f[0]};
}

lifted::Result e823693C8(uint8_t* base, const lifted::Args& a) {
  return {f823693C8(base, a.r[0]), a.f[0]};
}

lifted::Result e823694C8(uint8_t* base, const lifted::Args& a) {
  return {f823694C8(base, a.r[0]), a.f[0]};
}

lifted::Result e823695C0(uint8_t* base, const lifted::Args& a) {
  return {f823695C0(base, a.r[0]), a.f[0]};
}

lifted::Result e82369678(uint8_t* base, const lifted::Args& a) {
  return {f82369678(base, a.r[0]), a.f[0]};
}

lifted::Result e82369770(uint8_t* base, const lifted::Args& a) {
  return {f82369770(base, a.r[0]), a.f[0]};
}

lifted::Result e82369B48(uint8_t* base, const lifted::Args& a) {
  return f82369B48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82369DB8(uint8_t* base, const lifted::Args& a) {
  return f82369DB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82369FF8(uint8_t* base, const lifted::Args& a) {
  return f82369FF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e8236A4F0(uint8_t* base, const lifted::Args& a) {
  return f8236A4F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e8236AB00(uint8_t* base, const lifted::Args& a) {
  return f8236AB00(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e8236AEA8(uint8_t* base, const lifted::Args& a) {
  return f8236AEA8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8236B718(uint8_t* base, const lifted::Args& a) {
  return f8236B718(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8236B7A0(uint8_t* base, const lifted::Args& a) {
  return f8236B7A0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8236BF40(uint8_t* base, const lifted::Args& a) {
  return f8236BF40(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82379360(uint8_t* base, const lifted::Args& a) {
  return {f82379360(base, a.r[0]), a.f[0]};
}

lifted::Result e82379368(uint8_t* base, const lifted::Args& a) {
  return {f82379368(base, a.r[0]), a.f[0]};
}

lifted::Result e823EA0C0(uint8_t* base, const lifted::Args& a) {
  return {f823EA0C0(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e823EADB0(uint8_t* base, const lifted::Args& a) {
  f823EADB0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e823EB408(uint8_t* base, const lifted::Args& a) {
  return {f823EB408(base, a.r1, a.r13, a.r[0], a.r[1], a.lr), a.f[0]};
}

lifted::Result e823F24A8(uint8_t* base, const lifted::Args& a) {
  return {f823F24A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2530(uint8_t* base, const lifted::Args& a) {
  return {f823F2530(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F25C0(uint8_t* base, const lifted::Args& a) {
  return {f823F25C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F5520(uint8_t* base, const lifted::Args& a) {
  return {f823F5520(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F59C8(uint8_t* base, const lifted::Args& a) {
  return f823F59C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e823F5E30(uint8_t* base, const lifted::Args& a) {
  return f823F5E30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e823F5EF0(uint8_t* base, const lifted::Args& a) {
  return f823F5EF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e823F6C10(uint8_t* base, const lifted::Args& a) {
  return f823F6C10(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e823F77B0(uint8_t* base, const lifted::Args& a) {
  return f823F77B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8240FBA0(uint8_t* base, const lifted::Args& a) {
  f8240FBA0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82411640(uint8_t* base, const lifted::Args& a) {
  return {f82411640(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82420640(uint8_t* base, const lifted::Args& a) {
  return {f82420640(base, a.r[0]), a.f[0]};
}

lifted::Result e82421BF0(uint8_t* base, const lifted::Args& a) {
  return {f82421BF0(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82421C88(uint8_t* base, const lifted::Args& a) {
  return {f82421C88(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82422738(uint8_t* base, const lifted::Args& a) {
  return {f82422738(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82422798(uint8_t* base, const lifted::Args& a) {
  return {f82422798(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82422848(uint8_t* base, const lifted::Args& a) {
  return {f82422848(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82422890(uint8_t* base, const lifted::Args& a) {
  return {f82422890(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e824228D8(uint8_t* base, const lifted::Args& a) {
  return f824228D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e82422A20(uint8_t* base, const lifted::Args& a) {
  return {f82422A20(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82422A38(uint8_t* base, const lifted::Args& a) {
  return {f82422A38(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82422A50(uint8_t* base, const lifted::Args& a) {
  return f82422A50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e8243DC88(uint8_t* base, const lifted::Args& a) {
  f8243DC88(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8244A528(uint8_t* base, const lifted::Args& a) {
  return {f8244A528(base), a.f[0]};
}

lifted::Result e82459C20(uint8_t* base, const lifted::Args& a) {
  return f82459C20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82459C38(uint8_t* base, const lifted::Args& a) {
  return f82459C38(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82459C48(uint8_t* base, const lifted::Args& a) {
  return f82459C48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82459C60(uint8_t* base, const lifted::Args& a) {
  return f82459C60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82459C90(uint8_t* base, const lifted::Args& a) {
  return f82459C90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8245A4E8(uint8_t* base, const lifted::Args& a) {
  return f8245A4E8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8245A668(uint8_t* base, const lifted::Args& a) {
  return f8245A668(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8245AA48(uint8_t* base, const lifted::Args& a) {
  return f8245AA48(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8245AFA0(uint8_t* base, const lifted::Args& a) {
  return f8245AFA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8245B0D0(uint8_t* base, const lifted::Args& a) {
  return f8245B0D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8245B118(uint8_t* base, const lifted::Args& a) {
  return f8245B118(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8245B170(uint8_t* base, const lifted::Args& a) {
  return f8245B170(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8245B3A0(uint8_t* base, const lifted::Args& a) {
  return f8245B3A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8245B940(uint8_t* base, const lifted::Args& a) {
  return f8245B940(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8245C168(uint8_t* base, const lifted::Args& a) {
  return f8245C168(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8245D760(uint8_t* base, const lifted::Args& a) {
  return f8245D760(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8245D970(uint8_t* base, const lifted::Args& a) {
  return {f8245D970(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8245DB60(uint8_t* base, const lifted::Args& a) {
  return f8245DB60(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8245E308(uint8_t* base, const lifted::Args& a) {
  return {f8245E308(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8245ED60(uint8_t* base, const lifted::Args& a) {
  return f8245ED60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8245EF28(uint8_t* base, const lifted::Args& a) {
  return f8245EF28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8245F090(uint8_t* base, const lifted::Args& a) {
  return f8245F090(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8245F218(uint8_t* base, const lifted::Args& a) {
  return {f8245F218(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8245F4E0(uint8_t* base, const lifted::Args& a) {
  return f8245F4E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8245F5E8(uint8_t* base, const lifted::Args& a) {
  return f8245F5E8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8245FB10(uint8_t* base, const lifted::Args& a) {
  return f8245FB10(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8245FC70(uint8_t* base, const lifted::Args& a) {
  return f8245FC70(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

namespace {

const Registration kEntries[] = {
    {0x822B2598, e822B2598},
    {0x822B4250, e822B4250},
    {0x822B42D0, e822B42D0},
    {0x822B4300, e822B4300},
    {0x822B4318, e822B4318},
    {0x822B4330, e822B4330},
    {0x822B4360, e822B4360},
    {0x822B43A8, e822B43A8},
    {0x822B43E8, e822B43E8},
    {0x822B4680, e822B4680},
    {0x822B4E78, e822B4E78},
    {0x822B4F10, e822B4F10},
    {0x822B58C0, e822B58C0},
    {0x822B60C8, e822B60C8},
    {0x822B6188, e822B6188},
    {0x822B6288, e822B6288},
    {0x822B62E8, e822B62E8},
    {0x822B6370, e822B6370},
    {0x822B64A0, e822B64A0},
    {0x822B6528, e822B6528},
    {0x822C3958, e822C3958},
    {0x822F1218, e822F1218},
    {0x82304258, e82304258},
    {0x82368D98, e82368D98},
    {0x82368F58, e82368F58},
    {0x82369050, e82369050},
    {0x82369138, e82369138},
    {0x82369188, e82369188},
    {0x823691D8, e823691D8},
    {0x823692D0, e823692D0},
    {0x823693C8, e823693C8},
    {0x823694C8, e823694C8},
    {0x823695C0, e823695C0},
    {0x82369678, e82369678},
    {0x82369770, e82369770},
    {0x82369B48, e82369B48},
    {0x82369DB8, e82369DB8},
    {0x82369FF8, e82369FF8},
    {0x8236A4F0, e8236A4F0},
    {0x8236AB00, e8236AB00},
    {0x8236AEA8, e8236AEA8},
    {0x8236B718, e8236B718},
    {0x8236B7A0, e8236B7A0},
    {0x8236BF40, e8236BF40},
    {0x82379360, e82379360},
    {0x82379368, e82379368},
    {0x823EA0C0, e823EA0C0},
    {0x823EADB0, e823EADB0},
    {0x823EB408, e823EB408},
    {0x823F24A8, e823F24A8},
    {0x823F2530, e823F2530},
    {0x823F25C0, e823F25C0},
    {0x823F5520, e823F5520},
    {0x823F59C8, e823F59C8},
    {0x823F5E30, e823F5E30},
    {0x823F5EF0, e823F5EF0},
    {0x823F6C10, e823F6C10},
    {0x823F77B0, e823F77B0},
    {0x8240FBA0, e8240FBA0},
    {0x82411640, e82411640},
    {0x82420640, e82420640},
    {0x82421BF0, e82421BF0},
    {0x82421C88, e82421C88},
    {0x82422738, e82422738},
    {0x82422798, e82422798},
    {0x82422848, e82422848},
    {0x82422890, e82422890},
    {0x824228D8, e824228D8},
    {0x82422A20, e82422A20},
    {0x82422A38, e82422A38},
    {0x82422A50, e82422A50},
    {0x8243DC88, e8243DC88},
    {0x8244A528, e8244A528},
    {0x82459C20, e82459C20},
    {0x82459C38, e82459C38},
    {0x82459C48, e82459C48},
    {0x82459C60, e82459C60},
    {0x82459C90, e82459C90},
    {0x8245A4E8, e8245A4E8},
    {0x8245A668, e8245A668},
    {0x8245AA48, e8245AA48},
    {0x8245AFA0, e8245AFA0},
    {0x8245B0D0, e8245B0D0},
    {0x8245B118, e8245B118},
    {0x8245B170, e8245B170},
    {0x8245B3A0, e8245B3A0},
    {0x8245B940, e8245B940},
    {0x8245C168, e8245C168},
    {0x8245D760, e8245D760},
    {0x8245D970, e8245D970},
    {0x8245DB60, e8245DB60},
    {0x8245E308, e8245E308},
    {0x8245ED60, e8245ED60},
    {0x8245EF28, e8245EF28},
    {0x8245F090, e8245F090},
    {0x8245F218, e8245F218},
    {0x8245F4E0, e8245F4E0},
    {0x8245F5E8, e8245F5E8},
    {0x8245FB10, e8245FB10},
    {0x8245FC70, e8245FC70},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_015)), true);

}  // namespace

}  // namespace lifted
