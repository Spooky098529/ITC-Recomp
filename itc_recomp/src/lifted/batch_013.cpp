// Lift sweep batch 013 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/boot_mode.h"
#include "schema/car_draw.h"
#include "schema/car_list.h"
#include "schema/files_and_saves.h"
#include "schema/menu_helpers.h"
#include "schema/network_mode.h"
#include "schema/options_screen.h"
#include "schema/save_interface.h"
#include "schema/type5_mode.h"
#include "schema/type6_type7_modes.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_013, true, "ITC",
              "Run lift sweep batch 013's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_8222E368 game::LocationScreen::Transition11: lifted from generated/ by lift.py.
lifted::Result f8222E368(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8222E368 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8222E390, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lwz r28,0(r3)
  r28 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfs f13,2608(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 2608);
  f13 = double(temp.f32);
  // lwz r7,0(r26)
  r7 = guest::LoadU32(base, uint32_t(r26) + 0);
  // lfs f0,2612(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2612);
  f0 = double(temp.f32);
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // stfs f13,108(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // ld r7,0(r11)
  r7 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // stfs f13,140(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 140, temp.u32);
  // stfs f13,156(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 156, temp.u32);
  // stfs f0,112(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 112, temp.u32);
  // stfs f0,116(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 116, temp.u32);
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 120, temp.u32);
  // std r7,0(r9)
  guest::StoreU64(base, uint32_t(r9) + 0, r7);
  // stfs f13,124(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 124, temp.u32);
  // ld r6,0(r10)
  r6 = guest::LoadU64(base, uint32_t(r10) + 0);
  // std r11,8(r9)
  guest::StoreU64(base, uint32_t(r9) + 8, r11);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // ld r10,8(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 8);
  // addi r29,r11,1920
  r29 = uint64_t(int64_t(int64_t(r11) + 1920));
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // std r6,0(r8)
  guest::StoreU64(base, uint32_t(r8) + 0, r6);
  // addi r27,r11,25880
  r27 = uint64_t(int64_t(int64_t(r11) + 25880));
  // std r10,8(r8)
  guest::StoreU64(base, uint32_t(r8) + 8, r10);
  // stfs f0,160(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 160, temp.u32);
  // stfs f0,164(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 164, temp.u32);
  // bne cr6,0x8222e510
  if (!cr6.eq) goto loc_8222E510;
  // lis r8,-32127
  r8 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r10,20056(r8)
  r10 = guest::LoadU32(base, uint32_t(r8) + 20056);
  // lwz r9,800(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 800);
  // lwz r10,36(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 36);
  // add. r10,r10,r9
  r10 = r10 + r9;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt 0x8222e458
  if (cr0.lt) goto loc_8222E458;
  // lwz r9,808(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 808);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // bgt cr6,0x8222e45c
  if (cr6.gt) goto loc_8222E45C;
  // mr r9,r10
  r9 = r10;
  // b 0x8222e45c
  goto loc_8222E45C;
loc_8222E458:
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_8222E45C:
  // stw r9,800(r11)
  guest::StoreU32(base, uint32_t(r11) + 800, uint32_t(r9));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r7,r27,4
  r7 = uint64_t(int64_t(int64_t(r27) + 4));
  // lwz r9,20056(r8)
  r9 = guest::LoadU32(base, uint32_t(r8) + 20056);
  // lfs f1,3396(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 3396);
  f1 = double(temp.f32);
  // lwz r10,800(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 800);
  // lwz r11,852(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 852);
  // lwz r9,36(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 36);
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // lwzx r5,r11,r7
  r5 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r7));
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // subf r10,r9,r10
  r10 = r10 - r9;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r29
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r29));
  // lwzx r3,r10,r29
  r3 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r29));
  // bl 0x82150040
  // (left out: dead in C++)
  game::CameraAnims::SetGlobals(base, r3, r4, r5, f1);
  // bl 0x8222c0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8222C0C8, LIFTED_ARGS(0x8222E4A4, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222b438
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8222B438, LIFTED_ARGS(0x8222E4A8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222c188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8222C188, LIFTED_ARGS(0x8222E4AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r10,808(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 808);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // ble cr6,0x8222e4e0
  if (!cr6.gt) goto loc_8222E4E0;
loc_8222E4C0:
  // addi r4,r30,84
  r4 = uint64_t(int64_t(int64_t(r30) + 84));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8222E4CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // lwz r10,808(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 808);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // blt cr6,0x8222e4c0
  if (cr6.lt) goto loc_8222E4C0;
loc_8222E4E0:
  // lwz r11,800(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 800);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r11,84
  r4 = uint64_t(int64_t(int64_t(r11) + 84));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8222E500, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8222E508, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r11));
loc_8222E510:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8222E51C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x82150b80
  // (left out: dead in C++)
  { const lifted::Result o = f82150B80(base, r1, r13, r3, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // ld r8,0(r10)
  r8 = guest::LoadU64(base, uint32_t(r10) + 0);
  // std r8,592(r11)
  guest::StoreU64(base, uint32_t(r11) + 592, r8);
  // ld r10,8(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 8);
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // std r10,600(r11)
  guest::StoreU64(base, uint32_t(r11) + 600, r10);
  // ld r10,0(r9)
  r10 = guest::LoadU64(base, uint32_t(r9) + 0);
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // std r10,608(r11)
  guest::StoreU64(base, uint32_t(r11) + 608, r10);
  // ld r10,8(r9)
  r10 = guest::LoadU64(base, uint32_t(r9) + 8);
  // std r10,616(r11)
  guest::StoreU64(base, uint32_t(r11) + 616, r10);
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lfs f0,160(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 160);
  f0 = double(temp.f32);
  // stfs f0,664(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 664, temp.u32);
  // lfs f0,164(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 164);
  f0 = double(temp.f32);
  // stfs f0,668(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 668, temp.u32);
  // beq 0x8222e5dc
  if (cr0.eq) goto loc_8222E5DC;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // addi r9,r27,4
  r9 = uint64_t(int64_t(int64_t(r27) + 4));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r10,800(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 800);
  // lwz r11,852(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 852);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // lwzx r5,r10,r29
  r5 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r29));
  // lwzx r4,r11,r9
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // bl 0x82150960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82150960, LIFTED_ARGS(0x8222E598, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // ld r8,0(r10)
  r8 = guest::LoadU64(base, uint32_t(r10) + 0);
  // std r8,592(r11)
  guest::StoreU64(base, uint32_t(r11) + 592, r8);
  // ld r10,8(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r10,600(r11)
  guest::StoreU64(base, uint32_t(r11) + 600, r10);
  // ld r10,0(r9)
  r10 = guest::LoadU64(base, uint32_t(r9) + 0);
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // std r10,608(r11)
  guest::StoreU64(base, uint32_t(r11) + 608, r10);
  // ld r10,8(r9)
  r10 = guest::LoadU64(base, uint32_t(r9) + 8);
  // std r10,616(r11)
  guest::StoreU64(base, uint32_t(r11) + 616, r10);
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lfs f0,160(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 160);
  f0 = double(temp.f32);
  // stfs f0,664(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 664, temp.u32);
  // lfs f0,164(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 164);
  f0 = double(temp.f32);
  // stfs f0,668(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 668, temp.u32);
loc_8222E5DC:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8222E5E8 game::LocationScreen::Transition13: lifted from generated/ by lift.py.
lifted::Result f8222E5E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8222E5E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8222E618, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222e66c
  if (!cr6.eq) goto loc_8222E66C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,97
  r4 = uint64_t(int64_t(97));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8222E644, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,15
  r5 = uint64_t(int64_t(15));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(base, LIFTED_ARGS(0x8222E664, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_8222E66C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8222E678, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x8222e6c0
  if (cr0.eq) goto loc_8222E6C0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r11,20056(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20056);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(base, LIFTED_ARGS(0x8222E6B4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8222E6BC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222c0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8222C0C8, LIFTED_ARGS(0x8222E6C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222E6C0:
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

// sub_8222ED98 game::LocationScreen::Update: lifted from generated/ by lift.py.
lifted::Result f8222ED98(uint8_t* base, uint64_t r1, uint64_t r13, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_8222ED98 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-432(r1)
  ea = -432 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,2312
  r6 = uint64_t(int64_t(int64_t(r11) + 2312));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8222EDBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8222EDD4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // lis r14,-32221
  r14 = uint64_t(int64_t(-2111635456));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // addi r14,r14,-12624
  r14 = uint64_t(int64_t(int64_t(r14) + -12624));
  // lis r15,-32221
  r15 = uint64_t(int64_t(-2111635456));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // lis r16,-32221
  r16 = uint64_t(int64_t(-2111635456));
  // lwz r11,20040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20040);
  // lis r17,-32221
  r17 = uint64_t(int64_t(-2111635456));
  // stw r14,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r14));
  // lis r18,-32221
  r18 = uint64_t(int64_t(-2111635456));
  // lwz r14,80(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lis r19,-32221
  r19 = uint64_t(int64_t(-2111635456));
  // stw r10,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // addi r14,r14,-21168
  r14 = uint64_t(int64_t(int64_t(r14) + -21168));
  // lis r20,-32221
  r20 = uint64_t(int64_t(-2111635456));
  // lis r21,-32221
  r21 = uint64_t(int64_t(-2111635456));
  // lis r22,-32221
  r22 = uint64_t(int64_t(-2111635456));
  // stw r10,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // stw r14,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r14));
  // lis r23,-32221
  r23 = uint64_t(int64_t(-2111635456));
  // lis r24,-32221
  r24 = uint64_t(int64_t(-2111635456));
  // lis r25,-32163
  r25 = uint64_t(int64_t(-2107834368));
  // lis r26,-32163
  r26 = uint64_t(int64_t(-2107834368));
  // stw r10,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // lis r27,-32221
  r27 = uint64_t(int64_t(-2111635456));
  // lis r28,-32221
  r28 = uint64_t(int64_t(-2111635456));
  // lis r29,-32221
  r29 = uint64_t(int64_t(-2111635456));
  // lis r30,-32221
  r30 = uint64_t(int64_t(-2111635456));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // lis r4,-32221
  r4 = uint64_t(int64_t(-2111635456));
  // lis r5,-32221
  r5 = uint64_t(int64_t(-2111635456));
  // lis r6,-32221
  r6 = uint64_t(int64_t(-2111635456));
  // lis r7,-32221
  r7 = uint64_t(int64_t(-2111635456));
  // stw r10,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // lis r8,-32221
  r8 = uint64_t(int64_t(-2111635456));
  // lis r9,-32221
  r9 = uint64_t(int64_t(-2111635456));
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // stw r10,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // stw r10,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r10));
  // lis r10,-32221
  r10 = uint64_t(int64_t(-2111635456));
  // lwz r14,112(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 112);
  // addi r14,r14,-10736
  r14 = uint64_t(int64_t(int64_t(r14) + -10736));
  // stw r14,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r14));
  // lwz r14,104(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 104);
  // addi r14,r14,-11016
  r14 = uint64_t(int64_t(int64_t(r14) + -11016));
  // stw r14,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r14));
  // lwz r14,96(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 96);
  // addi r14,r14,-10200
  r14 = uint64_t(int64_t(int64_t(r14) + -10200));
  // stw r14,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r14));
  // lwz r14,84(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 84);
  // addi r14,r14,-9712
  r14 = uint64_t(int64_t(int64_t(r14) + -9712));
  // stw r14,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r14));
  // lwz r14,88(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 88);
  // addi r14,r14,-8984
  r14 = uint64_t(int64_t(int64_t(r14) + -8984));
  // stw r14,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r14));
  // lwz r14,92(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 92);
  // addi r14,r14,-20992
  r14 = uint64_t(int64_t(int64_t(r14) + -20992));
  // stw r14,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r14));
  // lwz r14,100(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 100);
  // addi r14,r14,-9208
  r14 = uint64_t(int64_t(int64_t(r14) + -9208));
  // addi r7,r7,-21840
  r7 = uint64_t(int64_t(int64_t(r7) + -21840));
  // stw r14,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r14));
  // addi r8,r8,-11336
  r8 = uint64_t(int64_t(int64_t(r8) + -11336));
  // lwz r14,108(r1)
  r14 = guest::LoadU32(base, uint32_t(r1) + 108);
  // addi r9,r9,-24040
  r9 = uint64_t(int64_t(int64_t(r9) + -24040));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // addi r10,r10,-6680
  r10 = uint64_t(int64_t(int64_t(r10) + -6680));
  // addi r14,r14,-8480
  r14 = uint64_t(int64_t(int64_t(r14) + -8480));
  // stw r7,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r7));
  // lis r7,-32221
  r7 = uint64_t(int64_t(-2111635456));
  // stw r8,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r8));
  // lis r8,-32221
  r8 = uint64_t(int64_t(-2111635456));
  // stw r9,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r9));
  // lis r9,-32221
  r9 = uint64_t(int64_t(-2111635456));
  // addi r15,r15,-7776
  r15 = uint64_t(int64_t(int64_t(r15) + -7776));
  // stw r10,188(r1)
  guest::StoreU32(base, uint32_t(r1) + 188, uint32_t(r10));
  // addi r16,r16,-7320
  r16 = uint64_t(int64_t(int64_t(r16) + -7320));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // addi r17,r17,-18264
  r17 = uint64_t(int64_t(int64_t(r17) + -18264));
  // stw r14,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r14));
  // addi r18,r18,-6680
  r18 = uint64_t(int64_t(int64_t(r18) + -6680));
  // addi r19,r19,-6432
  r19 = uint64_t(int64_t(int64_t(r19) + -6432));
  // addi r20,r20,-5928
  r20 = uint64_t(int64_t(int64_t(r20) + -5928));
  // stw r15,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r15));
  // addi r21,r21,-5424
  r21 = uint64_t(int64_t(int64_t(r21) + -5424));
  // stw r16,172(r1)
  guest::StoreU32(base, uint32_t(r1) + 172, uint32_t(r16));
  // addi r22,r22,-16800
  r22 = uint64_t(int64_t(int64_t(r22) + -16800));
  // stw r17,176(r1)
  guest::StoreU32(base, uint32_t(r1) + 176, uint32_t(r17));
  // addi r23,r23,-16616
  r23 = uint64_t(int64_t(int64_t(r23) + -16616));
  // stw r18,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r18));
  // addi r24,r24,-16616
  r24 = uint64_t(int64_t(int64_t(r24) + -16616));
  // stw r19,184(r1)
  guest::StoreU32(base, uint32_t(r1) + 184, uint32_t(r19));
  // addi r25,r25,-31856
  r25 = uint64_t(int64_t(int64_t(r25) + -31856));
  // stw r20,192(r1)
  guest::StoreU32(base, uint32_t(r1) + 192, uint32_t(r20));
  // addi r26,r26,-31856
  r26 = uint64_t(int64_t(int64_t(r26) + -31856));
  // stw r21,196(r1)
  guest::StoreU32(base, uint32_t(r1) + 196, uint32_t(r21));
  // addi r27,r27,-23440
  r27 = uint64_t(int64_t(int64_t(r27) + -23440));
  // stw r22,200(r1)
  guest::StoreU32(base, uint32_t(r1) + 200, uint32_t(r22));
  // addi r28,r28,-11544
  r28 = uint64_t(int64_t(int64_t(r28) + -11544));
  // stw r23,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r23));
  // addi r29,r29,-11864
  r29 = uint64_t(int64_t(int64_t(r29) + -11864));
  // stw r24,208(r1)
  guest::StoreU32(base, uint32_t(r1) + 208, uint32_t(r24));
  // addi r30,r30,-11544
  r30 = uint64_t(int64_t(int64_t(r30) + -11544));
  // stw r25,212(r1)
  guest::StoreU32(base, uint32_t(r1) + 212, uint32_t(r25));
  // addi r4,r4,-21384
  r4 = uint64_t(int64_t(int64_t(r4) + -21384));
  // stw r26,216(r1)
  guest::StoreU32(base, uint32_t(r1) + 216, uint32_t(r26));
  // addi r5,r5,-11176
  r5 = uint64_t(int64_t(int64_t(r5) + -11176));
  // stw r27,220(r1)
  guest::StoreU32(base, uint32_t(r1) + 220, uint32_t(r27));
  // addi r6,r6,-12304
  r6 = uint64_t(int64_t(int64_t(r6) + -12304));
  // stw r28,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r28));
  // addi r7,r7,-22760
  r7 = uint64_t(int64_t(int64_t(r7) + -22760));
  // stw r29,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r29));
  // addi r8,r8,-12024
  r8 = uint64_t(int64_t(int64_t(r8) + -12024));
  // stw r30,232(r1)
  guest::StoreU32(base, uint32_t(r1) + 232, uint32_t(r30));
  // addi r9,r9,-16512
  r9 = uint64_t(int64_t(int64_t(r9) + -16512));
  // stw r4,236(r1)
  guest::StoreU32(base, uint32_t(r1) + 236, uint32_t(r4));
  // stw r5,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r5));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // stw r6,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r6));
  // stw r7,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r7));
  // stw r8,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r8));
  // stw r9,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r9));
  // beq cr6,0x8222f06c
  if (cr6.eq) goto loc_8222F06C;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x8222f048
  if (cr6.lt) goto loc_8222F048;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwzx r11,r10,r9
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8222F004, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20040);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8222f074
  if (cr0.eq) goto loc_8222F074;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x8222f02c
  if (!cr6.eq) goto loc_8222F02C;
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // b 0x8222f03c
  goto loc_8222F03C;
loc_8222F02C:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_8222F030:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r8,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r8));
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
loc_8222F03C:
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
loc_8222F040:
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
  // b 0x8222f074
  goto loc_8222F074;
loc_8222F048:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x8222f030
  if (!cr6.eq) goto loc_8222F030;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x8222f040
  goto loc_8222F040;
loc_8222F06C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8222F074, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222F074:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,20048(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 20048);
  // bl 0x822282e8
  // (left out: dead in C++)
  { const lifted::Result o = game::CourseView::Draw(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r31,11844(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 11844);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,5052(r31)
  guest::StoreU32(base, schema::CarScene(base, uint32_t(r31)).lighting_fixed_address(), uint32_t(r11));
  // bl 0x82227e20
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::UpdateView(base, LIFTED_ARGS(0x8222F094, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820c9608
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::Update(base, LIFTED_ARGS(0x8222F098, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x820c5de8
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::Draw(base, LIFTED_ARGS(0x8222F0A4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,432
  r1 = uint64_t(int64_t(int64_t(r1) + 432));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8222FC90 game::Mode::NoNextMode: lifted from generated/ by lift.py.
void f8222FC90(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8222FC90 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r3)
  guest::StoreU32(base, schema::GameMode(base, uint32_t(r3)).delegate_ptr_address(), uint32_t(r11));
  // stb r11,0(r3)
  guest::StoreU8(base, schema::GameMode(base, uint32_t(r3)).answer_changed_address(), uint8_t(r11));
  // blr
  return;
}

// sub_8222FCA0 game::FrontEndMode::NextMode: lifted from generated/ by lift.py.
lifted::Result f8222FCA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222FCA0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r30,r4
  r30 = r4;
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // stb r11,0(r31)
  guest::StoreU8(base, uint32_t(r31) + 0, uint8_t(r11));
  // bl 0x82228220
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::Free(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8222fcec
  if (cr0.eq) goto loc_8222FCEC;
  // lwz r11,848(r30)
  r11 = guest::LoadU32(base, schema::ModeType5(base, uint32_t(r30)).block_address());
  // addi r11,r11,28928
  r11 = uint64_t(int64_t(int64_t(r11) + 28928));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::Type5ModeBlock(base, uint32_t(r11) - 28928).answer_word0_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::Type5ModeBlock(base, uint32_t(r11) - 28928).answer_next_type_address());
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
loc_8222FCEC:
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

// sub_82230B50 game::PanelGroupBase::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f82230B50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82230B50 (lifted)");
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2420
  r11 = uint64_t(int64_t(int64_t(r11) + 2420));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::PanelGroupBase(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // bl 0x8224fb10
  // (left out: dead in C++)
  { const lifted::Result o = game::ListNode::Dtor(base, LIFTED_ARGS(0x82230B7C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82230b8c
  if (cr0.eq) goto loc_82230B8C;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82230B8C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82230B8C:
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

// sub_82231DF0 game::Panel6C::ShowArrows: lifted from generated/ by lift.py.
uint64_t f82231DF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82231DF0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
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
  // lwz r3,12(r3)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r3)).group_address());
  // mr r31,r4
  r31 = r4;
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x82231E0C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bne cr6,0x82231e24
  if (!cr6.eq) goto loc_82231E24;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82231E20, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82231e28
  goto loc_82231E28;
loc_82231E24:
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82231E28, r31)); r3 = o.r3; f1 = o.f1; }
loc_82231E28:
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

// sub_82231EF0 game::Panel6C::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f82231EF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82231EF0 (lifted)");
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2432
  r11 = uint64_t(int64_t(int64_t(r11) + 2432));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::CarPanel(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // bl 0x8224d7e0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPanelBase::Dtor(base, LIFTED_ARGS(0x82231F1C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82231f2c
  if (cr0.eq) goto loc_82231F2C;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82231F2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82231F2C:
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

// sub_82231F58 game::Type6Dialog::QueueReopen: lifted from generated/ by lift.py.
void f82231F58(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82231F58 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r11,9748(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).reopen_address(), uint32_t(r11));
  // stw r10,9756(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).type6_status_address(), uint32_t(r10));
  // blr
  return;
}

// sub_82231F70 game::Type6Dialog::IsTakingInput: lifted from generated/ by lift.py.
uint64_t f82231F70(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82231F70 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,9756(r3)
  r11 = guest::LoadU32(base, schema::Type67DialogAt9672(base, uint32_t(r3)).unknown_9756_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82231f84
  if (!cr6.eq) goto loc_82231F84;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,9756(r3)
  guest::StoreU32(base, schema::Type67DialogAt9672(base, uint32_t(r3)).unknown_9756_address(), uint32_t(r11));
loc_82231F84:
  // lwz r11,9756(r3)
  r11 = guest::LoadU32(base, schema::Type67DialogAt9672(base, uint32_t(r3)).unknown_9756_address());
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // blr
  return r3;
}

// sub_82232040 game::Type6Dialog::Wait: lifted from generated/ by lift.py.
void f82232040(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82232040 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r11,9604(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).state_address(), uint32_t(r11));
  // stw r10,9756(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).type6_status_address(), uint32_t(r10));
  // blr
  return;
}

// sub_82232058 game::Type6Dialog::OpenNotice: lifted from generated/ by lift.py.
void f82232058(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82232058 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,9744(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).movie_address(), uint32_t(r9));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r11,9644(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).next_page_address(), uint32_t(r11));
  // stw r11,9640(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address(), uint32_t(r11));
  // stw r11,9648(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).previous_page_address(), uint32_t(r11));
  // stw r10,9660(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_word_20_address(), uint32_t(r10));
  // stw r10,9652(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_address(), uint32_t(r10));
  // stw r9,9664(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_started_address(), uint32_t(r9));
  // lwz r9,9608(r3)
  r9 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).loaded_address());
  // stw r11,9752(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).is_notice_address(), uint32_t(r11));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x822320c4
  if (cr6.eq) goto loc_822320C4;
  // lwz r9,9640(r3)
  r9 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address());
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // beq cr6,0x822320a4
  if (cr6.eq) goto loc_822320A4;
  // stw r9,9648(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).previous_page_address(), uint32_t(r9));
loc_822320A4:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,9656(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).queued_step_address(), uint32_t(r10));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r11,9644(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).next_page_address(), uint32_t(r11));
  // stw r10,9640(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address(), uint32_t(r10));
  // stw r9,9652(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_address(), uint32_t(r9));
  // stw r8,9604(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).state_address(), uint32_t(r8));
  // b 0x822320c8
  goto loc_822320C8;
loc_822320C4:
  // stw r11,9604(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).state_address(), uint32_t(r11));
loc_822320C8:
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,9756(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).type6_status_address(), uint32_t(r11));
  // blr
  return;
}

// sub_822321B8 game::Type6Dialog::Cancel: lifted from generated/ by lift.py.
void f822321B8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822321B8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r9,50
  r9 = uint64_t(int64_t(50));
  // stw r11,9756(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).type6_status_address(), uint32_t(r11));
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82232208
  if (!cr6.eq) goto loc_82232208;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822321f4
  if (cr0.eq) goto loc_822321F4;
  // stw r11,9648(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).previous_page_address(), uint32_t(r11));
loc_822321F4:
  // stw r30,9652(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_address(), uint32_t(r30));
  // stw r8,9656(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).queued_step_address(), uint32_t(r8));
  // stw r6,9644(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).next_page_address(), uint32_t(r6));
  // stw r8,9640(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address(), uint32_t(r8));
  // b 0x82232214
  goto loc_82232214;
loc_82232208:
  // lwz r11,9752(r3)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).is_notice_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82232258
  if (!cr6.eq) goto loc_82232258;
loc_82232214:
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // mr r5,r6
  r5 = r6;
  // li r7,12
  r7 = uint64_t(int64_t(12));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r31,604(r11)
  r31 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r31,r31,r10
  r31 = r31 + r10;
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r29,r31,r9
  r29 = uint32_t(uint32_t(r9) ? uint32_t(r31) / uint32_t(r9) : 0);
  // mulli r29,r29,50
  r29 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(50))));
  // subf r10,r29,r31
  r10 = r31 - r29;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r5,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r5));
  // stw r6,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r6));
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
loc_82232258:
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x822322d0
  if (!cr6.eq) goto loc_822322D0;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82232274
  if (cr6.eq) goto loc_82232274;
  // stw r11,9648(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).previous_page_address(), uint32_t(r11));
loc_82232274:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r8,9656(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).queued_step_address(), uint32_t(r8));
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // stw r8,9640(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address(), uint32_t(r8));
  // stw r30,9644(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).next_page_address(), uint32_t(r30));
  // li r7,11
  r7 = uint64_t(int64_t(11));
  // mr r5,r6
  r5 = r6;
  // stw r10,9652(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_address(), uint32_t(r10));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r8,604(r11)
  r8 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r9,r8,r9
  r9 = uint32_t(uint32_t(r9) ? uint32_t(r8) / uint32_t(r9) : 0);
  // mulli r9,r9,50
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(50))));
  // subf r10,r9,r8
  r10 = r8 - r9;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // stw r6,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r6));
  // stw r5,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r5));
  // b 0x82232320
  goto loc_82232320;
loc_822322D0:
  // lwz r11,9752(r3)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).is_notice_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82232320
  if (!cr6.eq) goto loc_82232320;
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // mr r7,r6
  r7 = r6;
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r5,604(r11)
  r5 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r5,r5,r10
  r5 = r5 + r10;
  // addi r5,r5,-1
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r9,r5,r9
  r9 = uint32_t(uint32_t(r9) ? uint32_t(r5) / uint32_t(r9) : 0);
  // mulli r9,r9,50
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(50))));
  // subf r10,r9,r5
  r10 = r5 - r9;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // stw r7,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r7));
  // stw r6,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r6));
loc_82232320:
  // b 0x825a6edc
  // (left out: dead in C++)
  return;
}

// sub_822324E8 game::Type6Dialog::NoticePage: lifted from generated/ by lift.py.
lifted::Result f822324E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822324E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // mr r28,r4
  r28 = r4;
  // lwz r11,9756(r31)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r31)).type6_status_address());
  // stw r29,9752(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).is_notice_address(), uint32_t(r29));
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // stw r29,9756(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).type6_status_address(), uint32_t(r29));
  // beq cr6,0x822325d4
  if (cr6.eq) goto loc_822325D4;
  // addi r30,r31,9668
  r30 = uint64_t(int64_t(int64_t(r31) + 9668));
  // mr r3,r30
  r3 = r30;
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x82232520, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // mr r3,r30
  r3 = r30;
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8223252C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82232548
  if (!cr0.eq) goto loc_82232548;
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // mr r3,r30
  r3 = r30;
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82232540, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822325d4
  if (cr0.eq) goto loc_822325D4;
loc_82232548:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // mr r3,r30
  r3 = r30;
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82232554, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,28,30,30
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0x2;
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82232560, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9640(r31)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r31)).page_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82232570
  if (cr6.eq) goto loc_82232570;
  // stw r11,9648(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).previous_page_address(), uint32_t(r11));
loc_82232570:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,9644(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).next_page_address(), uint32_t(r29));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // addi r11,r28,204
  r11 = uint64_t(int64_t(int64_t(r28) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,9656(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).queued_step_address(), uint32_t(r10));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,9640(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).page_address(), uint32_t(r10));
  // stw r9,9652(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).step_address(), uint32_t(r9));
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
loc_822325D4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82233460 game::MemberDelegate::Invoke: lifted from generated/ by lift.py.
lifted::Result f82233460(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82233460 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,12(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 12);
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lwz r9,8(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 8);
  // add r3,r11,r10
  r3 = r11 + r10;
  // mtctr r9
  ctr = r9;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82233948 game::Type6Screen0::Suspend: lifted from generated/ by lift.py.
lifted::Result f82233948(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82233948 (lifted)");
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
  // lwz r11,9616(r31)
  r11 = guest::LoadU32(base, schema::Type6Screen0(base, uint32_t(r31)).delete_movie_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82233978
  if (cr6.eq) goto loc_82233978;
  // li r3,22
  r3 = uint64_t(int64_t(22));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(base, LIFTED_ARGS(0x82233970, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9608(r31)
  guest::StoreU32(base, schema::Type6Screen0(base, uint32_t(r31)).loaded_address(), uint32_t(r11));
loc_82233978:
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

// sub_822339B0 game::Type6Screen0::PickBack: lifted from generated/ by lift.py.
void f822339B0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822339B0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, schema::Type6Screen0(base, uint32_t(r3)).words_address());
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,72(r11)
  schema::Type67Screen0Words(base, uint32_t(r11)).set_pick_bits(uint32_t(r10));
  // blr
  return;
}

// sub_82234928 game::Type6Screen0::BackToMenu: lifted from generated/ by lift.py.
uint64_t f82234928(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82234928 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // lwz r11,9644(r3)
  r11 = guest::LoadU32(base, schema::Type6Screen0(base, uint32_t(r3)).pages_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8223493c
  if (cr0.eq) goto loc_8223493C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8223493C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // b 0x822344a8
  { const lifted::Result o = lifted::Call(base, 0x822344A8, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82235D60 game::Type6Mode::Interrupt: lifted from generated/ by lift.py.
lifted::Result f82235D60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82235D60 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // ori r10,r11,25732
  r10 = r11 | 25732;
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lwz r8,816(r31)
  r8 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r31)).iface_block_address());
  // ori r11,r11,25728
  r11 = r11 | 25728;
  // stwx r9,r8,r10
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r10), uint32_t(r9));
  // lwz r10,816(r31)
  r10 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r31)).iface_block_address());
  // lwzx r10,r10,r11
  r10 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bne cr6,0x82235e5c
  if (!cr6.eq) goto loc_82235E5C;
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // lwz r11,816(r31)
  r11 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r31)).iface_block_address());
  // ori r10,r10,25684
  r10 = r10 | 25684;
  // lwzx r10,r11,r10
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82235de8
  if (cr6.eq) goto loc_82235DE8;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31624(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -31624);
  // lwz r10,712(r10)
  r10 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r10)).save_page_closed_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82235de8
  if (cr6.eq) goto loc_82235DE8;
  // lwz r10,19428(r11)
  r10 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r11)).fault_block_type6_status_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82235e38
  if (cr6.eq) goto loc_82235E38;
  // addi r3,r11,9672
  r3 = uint64_t(int64_t(int64_t(r11) + 9672));
  // bl 0x82231f70
  // (left out: dead in C++)
  r3 = game::Type6Dialog::IsTakingInput(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82235e70
  if (cr0.eq) goto loc_82235E70;
loc_82235DE8:
  // addi r11,r31,208
  r11 = uint64_t(int64_t(int64_t(r31) + 208));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,32
  r9 = uint64_t(int64_t(32));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r11) - 208).iface_ring_count_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r11) - 208).iface_ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::Type6Mode(base, uint32_t(r11) - 208).iface_ring_count_address(), uint32_t(r10));
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // b 0x82235e70
  goto loc_82235E70;
loc_82235E38:
  // lwz r3,29108(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 29108);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82235E4C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // b 0x82235e74
  goto loc_82235E74;
loc_82235E5C:
  // lwz r10,816(r31)
  r10 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r31)).iface_block_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bgt cr6,0x82235e74
  if (cr6.gt) goto loc_82235E74;
loc_82235E70:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82235E74:
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

// sub_82235F10 game::Type6Mode::Resume: lifted from generated/ by lift.py.
void f82235F10(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82235F10 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // lwz r11,816(r3)
  r11 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r3)).iface_block_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // ori r10,r10,25732
  r10 = r10 | 25732;
  // stwx r9,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r9));
  // blr
  return;
}

// sub_82236608 game::Type6Mode::ForceExit: lifted from generated/ by lift.py.
void f82236608(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82236608 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lwz r10,816(r3)
  r10 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r3)).iface_block_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // ori r11,r11,25732
  r11 = r11 | 25732;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // stwx r9,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r9));
  // beq cr6,0x8223664c
  if (cr6.eq) goto loc_8223664C;
  // cmpwi cr6,r4,1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 1, so);
  // beq cr6,0x8223664c
  if (cr6.eq) goto loc_8223664C;
  // cmpwi cr6,r4,2
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 2, so);
  // bne cr6,0x82236640
  if (!cr6.eq) goto loc_82236640;
loc_82236638:
  // li r11,12
  r11 = uint64_t(int64_t(12));
  // b 0x82236650
  goto loc_82236650;
loc_82236640:
  // cmpwi cr6,r4,3
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 3, so);
  // bne cr6,0x8223666c
  if (!cr6.eq) goto loc_8223666C;
  // b 0x82236638
  goto loc_82236638;
loc_8223664C:
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_82236650:
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // stw r11,-12(r1)
  guest::StoreU32(base, uint32_t(r1) + -12, uint32_t(r11));
  // stb r7,-16(r1)
  guest::StoreU8(base, uint32_t(r1) + -16, uint8_t(r7));
  // lwz r11,816(r3)
  r11 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r3)).iface_block_address());
  // ori r10,r10,25720
  r10 = r10 | 25720;
  // ld r8,-16(r1)
  r8 = guest::LoadU64(base, uint32_t(r1) + -16);
  // stdx r8,r11,r10
  guest::StoreU64(base, uint32_t(r11) + uint32_t(r10), r8);
loc_8223666C:
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // lwz r11,816(r3)
  r11 = guest::LoadU32(base, schema::Type6Mode(base, uint32_t(r3)).iface_block_address());
  // ori r10,r10,25728
  r10 = r10 | 25728;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x82236698
  if (cr6.eq) goto loc_82236698;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x82236698
  if (cr6.eq) goto loc_82236698;
  // lis r11,-32221
  r11 = uint64_t(int64_t(-2111635456));
  // addi r11,r11,24832
  r11 = uint64_t(int64_t(int64_t(r11) + 24832));
  // b 0x822366a0
  goto loc_822366A0;
loc_82236698:
  // lis r11,-32221
  r11 = uint64_t(int64_t(-2111635456));
  // addi r11,r11,23576
  r11 = uint64_t(int64_t(int64_t(r11) + 23576));
loc_822366A0:
  // addi r10,r3,-32
  r10 = uint64_t(int64_t(int64_t(r3) + -32));
  // stw r11,-16(r1)
  guest::StoreU32(base, uint32_t(r1) + -16, uint32_t(r11));
  // stw r9,-12(r1)
  guest::StoreU32(base, uint32_t(r1) + -12, uint32_t(r9));
  // addi r8,r10,4
  r8 = uint64_t(int64_t(int64_t(r10) + 4));
  // addic. r11,r8,4
  ca = uint32_t(r8) > 4294967291;
  r11 = uint64_t(int64_t(int64_t(r8) + 4));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822366d4
  if (cr0.eq) goto loc_822366D4;
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // ld r6,-16(r1)
  r6 = guest::LoadU64(base, uint32_t(r1) + -16);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // addi r9,r9,4000
  r9 = uint64_t(int64_t(int64_t(r9) + 4000));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // std r6,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r6);
  // b 0x822366d8
  goto loc_822366D8;
loc_822366D4:
  // mr r11,r9
  r11 = r9;
loc_822366D8:
  // stw r11,0(r8)
  guest::StoreU32(base, schema::Type6Mode(base, uint32_t(r8) + 28).iface_delegate_ptr_address(), uint32_t(r11));
  // lis r10,-32157
  r10 = uint64_t(int64_t(-2107441152));
  // mr r11,r7
  r11 = r7;
  // stw r11,25836(r10)
  guest::StoreU32(base, uint32_t(r10) + 25836, uint32_t(r11));
  // blr
  return;
}

// sub_8223A2F8 game::RequestSlot::Dtor: lifted from generated/ by lift.py.
lifted::Result f8223A2F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8223A2F8 (lifted)");
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
  // bl 0x8224fb10
  // (left out: dead in C++)
  { const lifted::Result o = game::ListNode::Dtor(base, LIFTED_ARGS(0x8223A318, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8223a328
  if (cr0.eq) goto loc_8223A328;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8223A328, r31)); r3 = o.r3; f1 = o.f1; }
loc_8223A328:
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

// sub_8223A3F0 game::Delegate::Invoke: lifted from generated/ by lift.py.
lifted::Result f8223A3F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8223A3F0 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82241968 game::Panel7C::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f82241968(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82241968 (lifted)");
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3812
  r11 = uint64_t(int64_t(int64_t(r11) + 3812));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::CarPanel(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // bl 0x8224d7e0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPanelBase::Dtor(base, LIFTED_ARGS(0x82241994, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822419a4
  if (cr0.eq) goto loc_822419A4;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x822419A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_822419A4:
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

// sub_822419E0 game::Type7Dialog::IsTakingInput: lifted from generated/ by lift.py.
uint64_t f822419E0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822419E0 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,9760(r3)
  r11 = guest::LoadU32(base, schema::Type67DialogAt9664(base, uint32_t(r3)).unknown_9760_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822419f4
  if (!cr6.eq) goto loc_822419F4;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,9760(r3)
  guest::StoreU32(base, schema::Type67DialogAt9664(base, uint32_t(r3)).unknown_9760_address(), uint32_t(r11));
loc_822419F4:
  // lwz r11,9760(r3)
  r11 = guest::LoadU32(base, schema::Type67DialogAt9664(base, uint32_t(r3)).unknown_9760_address());
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // blr
  return r3;
}

// sub_82241AB0 game::Type7Dialog::Wait: lifted from generated/ by lift.py.
void f82241AB0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82241AB0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r11,9604(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).state_address(), uint32_t(r11));
  // stw r10,9760(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).type7_status_address(), uint32_t(r10));
  // blr
  return;
}

// sub_82241AC8 game::Type7Dialog::OpenNotice: lifted from generated/ by lift.py.
void f82241AC8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82241AC8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,9744(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).movie_address(), uint32_t(r9));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r11,9644(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).next_page_address(), uint32_t(r11));
  // stw r11,9640(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address(), uint32_t(r11));
  // stw r11,9648(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).previous_page_address(), uint32_t(r11));
  // stw r10,9660(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_word_20_address(), uint32_t(r10));
  // stw r10,9652(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_address(), uint32_t(r10));
  // stw r9,9664(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_started_address(), uint32_t(r9));
  // lwz r9,9608(r3)
  r9 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).loaded_address());
  // stw r11,9752(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).is_notice_address(), uint32_t(r11));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x82241b34
  if (cr6.eq) goto loc_82241B34;
  // lwz r9,9640(r3)
  r9 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address());
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // beq cr6,0x82241b14
  if (cr6.eq) goto loc_82241B14;
  // stw r9,9648(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).previous_page_address(), uint32_t(r9));
loc_82241B14:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,9656(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).queued_step_address(), uint32_t(r10));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r11,9644(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).next_page_address(), uint32_t(r11));
  // stw r10,9640(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address(), uint32_t(r10));
  // stw r9,9652(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_address(), uint32_t(r9));
  // stw r8,9604(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).state_address(), uint32_t(r8));
  // blr
  return;
loc_82241B34:
  // stw r11,9604(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).state_address(), uint32_t(r11));
  // blr
  return;
}

// sub_82241CE8 game::Type7Dialog::Cancel: lifted from generated/ by lift.py.
void f82241CE8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82241CE8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r9,50
  r9 = uint64_t(int64_t(50));
  // stw r11,9760(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).type7_status_address(), uint32_t(r11));
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82241d38
  if (!cr6.eq) goto loc_82241D38;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82241d24
  if (cr0.eq) goto loc_82241D24;
  // stw r11,9648(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).previous_page_address(), uint32_t(r11));
loc_82241D24:
  // stw r30,9652(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_address(), uint32_t(r30));
  // stw r8,9656(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).queued_step_address(), uint32_t(r8));
  // stw r6,9644(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).next_page_address(), uint32_t(r6));
  // stw r8,9640(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address(), uint32_t(r8));
  // b 0x82241d44
  goto loc_82241D44;
loc_82241D38:
  // lwz r11,9752(r3)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).is_notice_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82241d88
  if (!cr6.eq) goto loc_82241D88;
loc_82241D44:
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // mr r5,r6
  r5 = r6;
  // li r7,11
  r7 = uint64_t(int64_t(11));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r31,604(r11)
  r31 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r31,r31,r10
  r31 = r31 + r10;
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r29,r31,r9
  r29 = uint32_t(uint32_t(r9) ? uint32_t(r31) / uint32_t(r9) : 0);
  // mulli r29,r29,50
  r29 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(50))));
  // subf r10,r29,r31
  r10 = r31 - r29;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r5,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r5));
  // stw r6,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r6));
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
loc_82241D88:
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82241e00
  if (!cr6.eq) goto loc_82241E00;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82241da4
  if (cr6.eq) goto loc_82241DA4;
  // stw r11,9648(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).previous_page_address(), uint32_t(r11));
loc_82241DA4:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r8,9656(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).queued_step_address(), uint32_t(r8));
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // stw r8,9640(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).page_address(), uint32_t(r8));
  // stw r30,9644(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).next_page_address(), uint32_t(r30));
  // li r7,10
  r7 = uint64_t(int64_t(10));
  // mr r5,r6
  r5 = r6;
  // stw r10,9652(r3)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r3)).step_address(), uint32_t(r10));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r8,604(r11)
  r8 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r9,r8,r9
  r9 = uint32_t(uint32_t(r9) ? uint32_t(r8) / uint32_t(r9) : 0);
  // mulli r9,r9,50
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(50))));
  // subf r10,r9,r8
  r10 = r8 - r9;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // stw r6,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r6));
  // stw r5,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r5));
  // b 0x82241e50
  goto loc_82241E50;
loc_82241E00:
  // lwz r11,9752(r3)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r3)).is_notice_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82241e50
  if (!cr6.eq) goto loc_82241E50;
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r8,10
  r8 = uint64_t(int64_t(10));
  // mr r7,r6
  r7 = r6;
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r5,604(r11)
  r5 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r5,r5,r10
  r5 = r5 + r10;
  // addi r5,r5,-1
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r9,r5,r9
  r9 = uint32_t(uint32_t(r9) ? uint32_t(r5) / uint32_t(r9) : 0);
  // mulli r9,r9,50
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(50))));
  // subf r10,r9,r5
  r10 = r5 - r9;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // stw r7,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r7));
  // stw r6,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r6));
loc_82241E50:
  // b 0x825a6edc
  // (left out: dead in C++)
  return;
}

// sub_822420C0 game::Type7Dialog::NoticePage: lifted from generated/ by lift.py.
lifted::Result f822420C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822420C0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // mr r28,r4
  r28 = r4;
  // lwz r11,9760(r31)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r31)).type7_status_address());
  // stw r29,9752(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).is_notice_address(), uint32_t(r29));
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // stw r29,9760(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).type7_status_address(), uint32_t(r29));
  // beq cr6,0x822421b4
  if (cr6.eq) goto loc_822421B4;
  // addi r30,r31,9668
  r30 = uint64_t(int64_t(int64_t(r31) + 9668));
  // mr r3,r30
  r3 = r30;
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x822420F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // mr r3,r30
  r3 = r30;
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82242104, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82242120
  if (!cr0.eq) goto loc_82242120;
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // mr r3,r30
  r3 = r30;
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82242118, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822421b4
  if (cr0.eq) goto loc_822421B4;
loc_82242120:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // mr r3,r30
  r3 = r30;
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8224212C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,28,30,30
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0x2;
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82242138, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9640(r31)
  r11 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r31)).page_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82242148
  if (cr6.eq) goto loc_82242148;
  // stw r11,9648(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).previous_page_address(), uint32_t(r11));
loc_82242148:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,9644(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).next_page_address(), uint32_t(r29));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // addi r11,r28,204
  r11 = uint64_t(int64_t(int64_t(r28) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,9656(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).queued_step_address(), uint32_t(r10));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,9640(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).page_address(), uint32_t(r10));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // stw r9,9652(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).step_address(), uint32_t(r9));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // stw r4,9760(r31)
  guest::StoreU32(base, schema::Type67Dialog(base, uint32_t(r31)).type7_status_address(), uint32_t(r4));
loc_822421B4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82243EE0 game::Type7Screen0::Suspend: lifted from generated/ by lift.py.
lifted::Result f82243EE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82243EE0 (lifted)");
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
  // lwz r11,9620(r31)
  r11 = guest::LoadU32(base, schema::Type7Screen0(base, uint32_t(r31)).delete_movie_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82243f10
  if (cr6.eq) goto loc_82243F10;
  // li r3,22
  r3 = uint64_t(int64_t(22));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(base, LIFTED_ARGS(0x82243F08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9612(r31)
  guest::StoreU32(base, schema::Type7Screen0(base, uint32_t(r31)).loaded_address(), uint32_t(r11));
loc_82243F10:
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

// sub_82243F38 game::Type7Screen0::PickNo: lifted from generated/ by lift.py.
void f82243F38(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82243F38 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r11,9644(r3)
  r11 = guest::LoadU32(base, schema::Type7Screen0(base, uint32_t(r3)).words_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,72(r11)
  schema::Type67Screen0Words(base, uint32_t(r11)).set_pick_bits(uint32_t(r10));
  // blr
  return;
}

// sub_82243F48 game::Type7Screen0::PickBack: lifted from generated/ by lift.py.
void f82243F48(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82243F48 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r11,9644(r3)
  r11 = guest::LoadU32(base, schema::Type7Screen0(base, uint32_t(r3)).words_address());
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,72(r11)
  schema::Type67Screen0Words(base, uint32_t(r11)).set_pick_bits(uint32_t(r10));
  // blr
  return;
}

// sub_82246228 game::Type7Mode::Interrupt: lifted from generated/ by lift.py.
lifted::Result f82246228(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82246228 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // ori r10,r11,5752
  r10 = r11 | 5752;
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lwz r8,816(r31)
  r8 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r31)).iface_block_address());
  // ori r11,r11,5748
  r11 = r11 | 5748;
  // stwx r9,r8,r10
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r10), uint32_t(r9));
  // lwz r10,816(r31)
  r10 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r31)).iface_block_address());
  // lwzx r10,r10,r11
  r10 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bne cr6,0x82246310
  if (!cr6.eq) goto loc_82246310;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31624(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31624);
  // lwz r11,712(r11)
  r11 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).save_page_closed_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8224629c
  if (cr6.eq) goto loc_8224629C;
  // lwz r11,816(r31)
  r11 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r31)).iface_block_address());
  // lwz r10,19424(r11)
  r10 = guest::LoadU32(base, schema::Type67Dialog(base, uint32_t(r11)).fault_block_type7_status_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x822462ec
  if (cr6.eq) goto loc_822462EC;
  // addi r3,r11,9664
  r3 = uint64_t(int64_t(int64_t(r11) + 9664));
  // bl 0x822419e0
  // (left out: dead in C++)
  r3 = game::Type7Dialog::IsTakingInput(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82246324
  if (cr0.eq) goto loc_82246324;
loc_8224629C:
  // addi r11,r31,208
  r11 = uint64_t(int64_t(int64_t(r31) + 208));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,26
  r9 = uint64_t(int64_t(26));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r11) - 208).iface_ring_count_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r11) - 208).iface_ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::Type7Mode(base, uint32_t(r11) - 208).iface_ring_count_address(), uint32_t(r10));
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // b 0x82246324
  goto loc_82246324;
loc_822462EC:
  // lwz r3,29104(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 29104);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82246300, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // b 0x82246328
  goto loc_82246328;
loc_82246310:
  // lwz r10,816(r31)
  r10 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r31)).iface_block_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bgt cr6,0x82246328
  if (cr6.gt) goto loc_82246328;
loc_82246324:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82246328:
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

// sub_822463C0 game::Type7Mode::Resume: lifted from generated/ by lift.py.
void f822463C0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822463C0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // lwz r11,816(r3)
  r11 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r3)).iface_block_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // ori r10,r10,5752
  r10 = r10 | 5752;
  // stwx r9,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r9));
  // blr
  return;
}

// sub_82246A40 game::Type7Mode::ForceExit: lifted from generated/ by lift.py.
void f82246A40(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82246A40 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lwz r10,816(r3)
  r10 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r3)).iface_block_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // ori r11,r11,5752
  r11 = r11 | 5752;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // stwx r9,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r9));
  // beq cr6,0x82246a84
  if (cr6.eq) goto loc_82246A84;
  // cmpwi cr6,r4,1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 1, so);
  // beq cr6,0x82246a84
  if (cr6.eq) goto loc_82246A84;
  // cmpwi cr6,r4,2
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 2, so);
  // bne cr6,0x82246a78
  if (!cr6.eq) goto loc_82246A78;
loc_82246A70:
  // li r11,12
  r11 = uint64_t(int64_t(12));
  // b 0x82246a88
  goto loc_82246A88;
loc_82246A78:
  // cmpwi cr6,r4,3
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 3, so);
  // bne cr6,0x82246aa4
  if (!cr6.eq) goto loc_82246AA4;
  // b 0x82246a70
  goto loc_82246A70;
loc_82246A84:
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_82246A88:
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // stw r11,-12(r1)
  guest::StoreU32(base, uint32_t(r1) + -12, uint32_t(r11));
  // stb r7,-16(r1)
  guest::StoreU8(base, uint32_t(r1) + -16, uint8_t(r7));
  // lwz r11,816(r3)
  r11 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r3)).iface_block_address());
  // ori r10,r10,5740
  r10 = r10 | 5740;
  // ld r8,-16(r1)
  r8 = guest::LoadU64(base, uint32_t(r1) + -16);
  // stdx r8,r11,r10
  guest::StoreU64(base, uint32_t(r11) + uint32_t(r10), r8);
loc_82246AA4:
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // lwz r11,816(r3)
  r11 = guest::LoadU32(base, schema::Type7Mode(base, uint32_t(r3)).iface_block_address());
  // ori r10,r10,5748
  r10 = r10 | 5748;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x82246ad0
  if (cr6.eq) goto loc_82246AD0;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x82246ad0
  if (cr6.eq) goto loc_82246AD0;
  // lis r11,-32220
  r11 = uint64_t(int64_t(-2111569920));
  // addi r11,r11,26000
  r11 = uint64_t(int64_t(int64_t(r11) + 26000));
  // b 0x82246ad8
  goto loc_82246AD8;
loc_82246AD0:
  // lis r11,-32220
  r11 = uint64_t(int64_t(-2111569920));
  // addi r11,r11,24728
  r11 = uint64_t(int64_t(int64_t(r11) + 24728));
loc_82246AD8:
  // addi r10,r3,-32
  r10 = uint64_t(int64_t(int64_t(r3) + -32));
  // stw r11,-16(r1)
  guest::StoreU32(base, uint32_t(r1) + -16, uint32_t(r11));
  // stw r9,-12(r1)
  guest::StoreU32(base, uint32_t(r1) + -12, uint32_t(r9));
  // addi r8,r10,4
  r8 = uint64_t(int64_t(int64_t(r10) + 4));
  // addic. r11,r8,4
  ca = uint32_t(r8) > 4294967291;
  r11 = uint64_t(int64_t(int64_t(r8) + 4));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82246b0c
  if (cr0.eq) goto loc_82246B0C;
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // ld r6,-16(r1)
  r6 = guest::LoadU64(base, uint32_t(r1) + -16);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // addi r9,r9,4000
  r9 = uint64_t(int64_t(int64_t(r9) + 4000));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // std r6,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r6);
  // b 0x82246b10
  goto loc_82246B10;
loc_82246B0C:
  // mr r11,r9
  r11 = r9;
loc_82246B10:
  // stw r11,0(r8)
  guest::StoreU32(base, schema::Type7Mode(base, uint32_t(r8) + 28).iface_delegate_ptr_address(), uint32_t(r11));
  // lis r10,-32157
  r10 = uint64_t(int64_t(-2107441152));
  // mr r11,r7
  r11 = r7;
  // stw r11,25836(r10)
  guest::StoreU32(base, uint32_t(r10) + 25836, uint32_t(r11));
  // blr
  return;
}

// sub_82247B28 game::LoadingScreen::Free: lifted from generated/ by lift.py.
lifted::Result f82247B28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82247B28 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r31,r11,24168
  r31 = uint64_t(int64_t(int64_t(r11) + 24168));
  // b 0x82247b70
  goto loc_82247B70;
loc_82247B48:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stw r11,64(r31)
  guest::StoreU32(base, uint32_t(r31) + 64, uint32_t(r11));
  // lwzx r30,r10,r31
  r30 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r31));
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq 0x82247b74
  if (cr0.eq) goto loc_82247B74;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e160
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Dtor(base, LIFTED_ARGS(0x82247B68, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82247B70, r31)); r3 = o.r3; f1 = o.f1; }
loc_82247B70:
  // lwz r11,64(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 64);
loc_82247B74:
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82247b48
  if (!cr0.eq) goto loc_82247B48;
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

// sub_82248E60 game::PanelSetBase::ConfirmPage: lifted from generated/ by lift.py.
lifted::Result f82248E60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82248E60 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // addi r3,r31,60
  r3 = uint64_t(int64_t(int64_t(r31) + 60));
  // bl 0x8220bde0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Cancelled(base, LIFTED_ARGS(0x82248E7C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82248ed0
  if (cr0.eq) goto loc_82248ED0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82248E8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,232(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).tab_address());
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r10,r11,2
  r10 = uint64_t(int64_t(int64_t(r11) + 2));
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).page_address());
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // beq cr6,0x82248eac
  if (cr6.eq) goto loc_82248EAC;
  // stw r11,40(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).previous_page_address(), uint32_t(r11));
loc_82248EAC:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r10,36(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).next_page_address(), uint32_t(r10));
  // li r9,23
  r9 = uint64_t(int64_t(23));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r11,48(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).queued_step_address(), uint32_t(r11));
  // stw r9,44(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).step_address(), uint32_t(r9));
  // stw r11,32(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).page_address(), uint32_t(r11));
  // stw r8,12(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).done_address(), uint32_t(r8));
  // b 0x82248ed8
  goto loc_82248ED8;
loc_82248ED0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,12(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).done_address(), uint32_t(r11));
loc_82248ED8:
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

// sub_82249198 game::PanelSetBase::Step15: lifted from generated/ by lift.py.
lifted::Result f82249198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82249198 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822491d4
  if (!cr6.eq) goto loc_822491D4;
  // lwz r3,1908(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822491cc
  if (cr0.eq) goto loc_822491CC;
  // bl 0x8224df90
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPanel::PlayStats(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_822491CC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
loc_822491D4:
  // lwz r3,1908(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822491e8
  if (cr0.eq) goto loc_822491E8;
  // bl 0x8224e208
  // (left out: dead in C++)
  r3 = game::CarPanel::StatsShown(base, r1, r13, r3);
  // mr r29,r3
  r29 = r3;
loc_822491E8:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822491F8 game::PanelSetBase::Step16: lifted from generated/ by lift.py.
lifted::Result f822491F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822491F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82249234
  if (!cr6.eq) goto loc_82249234;
  // lwz r3,1908(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8224922c
  if (cr0.eq) goto loc_8224922C;
  // bl 0x8224dff0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPanel::PlayStatsOut(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_8224922C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
loc_82249234:
  // lwz r3,1908(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82249248
  if (cr0.eq) goto loc_82249248;
  // bl 0x8224e248
  // (left out: dead in C++)
  r3 = game::CarPanel::StatsHidden(base, r1, r13, r3);
  // mr r29,r3
  r29 = r3;
loc_82249248:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822493D8 game::PanelSetBase::Step21: lifted from generated/ by lift.py.
lifted::Result f822493D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_822493D8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,1912(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).confirm_panel_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82249438
  if (cr0.eq) goto loc_82249438;
  // lwz r10,0(r30)
  r10 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82249430
  if (!cr6.eq) goto loc_82249430;
  // lwz r10,232(r31)
  r10 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).tab_address());
  // mr r3,r11
  r3 = r11;
  // cntlzw r11,r10
  r11 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r4,r11,27,31,31
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x8224ed38
  // (left out: dead in C++)
  { const lifted::Result o = game::ConfirmPanel::Show(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,1912(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).confirm_panel_address());
  // bl 0x8224ecc0
  // (left out: dead in C++)
  r3 = game::ConfirmPanel::ShowNode(base, r1, r13, r3, r4);
loc_82249430:
  // lwz r3,1912(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).confirm_panel_address());
  // bl 0x8224ee28
  // (left out: dead in C++)
  r3 = game::ConfirmPanel::IsShown(base, r1, r13, r3);
loc_82249438:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
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

// sub_82249458 game::PanelSetBase::Step22: lifted from generated/ by lift.py.
lifted::Result f82249458(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82249458 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
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
  // lwz r3,1912(r3)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r3)).confirm_panel_address());
  // mr r31,r4
  r31 = r4;
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82249488
  if (cr0.eq) goto loc_82249488;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r31)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82249488
  if (!cr6.eq) goto loc_82249488;
  // bl 0x8224eda8
  // (left out: dead in C++)
  { const lifted::Result o = game::ConfirmPanel::Loop(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
loc_82249488:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r31)).first_pass_address(), uint32_t(r11));
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

// sub_822494A8 game::PanelSetBase::Step23: lifted from generated/ by lift.py.
lifted::Result f822494A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822494A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r3,1912(r30)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r30)).confirm_panel_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822494f8
  if (cr0.eq) goto loc_822494F8;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r29)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822494dc
  if (!cr6.eq) goto loc_822494DC;
  // bl 0x8224edc0
  // (left out: dead in C++)
  { const lifted::Result o = game::ConfirmPanel::Hide(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_822494DC:
  // lwz r3,1912(r30)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r30)).confirm_panel_address());
  // bl 0x8224ee68
  // (left out: dead in C++)
  r3 = game::ConfirmPanel::IsHidden(base, r1, r13, r3);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x822494f8
  if (cr0.eq) goto loc_822494F8;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,1912(r30)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r30)).confirm_panel_address());
  // bl 0x8224ecc0
  // (left out: dead in C++)
  r3 = game::ConfirmPanel::ShowNode(base, r1, r13, r3, r4);
loc_822494F8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r29)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r29)).first_pass_address(), uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82249510 game::PanelSetBase::Step24: lifted from generated/ by lift.py.
uint64_t f82249510(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82249510 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r31)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82249540
  if (!cr6.eq) goto loc_82249540;
  // addi r3,r3,60
  r3 = uint64_t(int64_t(int64_t(r3) + 60));
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(base, LIFTED_ARGS(0x82249538, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r31)).first_pass_address(), uint32_t(r11));
loc_82249540:
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_82249610 game::PanelSetBase::SetKind: lifted from generated/ by lift.py.
void f82249610(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82249610 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r4,36(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_36_address(), uint32_t(r4));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r4,32(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_32_address(), uint32_t(r4));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // stw r4,40(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_40_address(), uint32_t(r4));
  // stw r11,52(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_52_address(), uint32_t(r11));
  // stw r11,44(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_44_address(), uint32_t(r11));
  // stw r10,56(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_56_address(), uint32_t(r10));
  // bne cr6,0x82249648
  if (!cr6.eq) goto loc_82249648;
  // lwz r11,1880(r3)
  r11 = guest::LoadU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_1880_address());
  // stw r11,232(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_232_address(), uint32_t(r11));
  // stw r11,228(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_228_address(), uint32_t(r11));
  // blr
  return;
loc_82249648:
  // cmpwi cr6,r4,1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 1, so);
  // bnelr cr6
  if (!cr6.eq) return;
  // addi r11,r3,272
  r11 = uint64_t(int64_t(int64_t(r3) + 272));
  // stw r10,228(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_228_address(), uint32_t(r10));
  // stw r10,232(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_232_address(), uint32_t(r10));
  // stw r11,1820(r3)
  guest::StoreU32(base, schema::PanelSetBaseAt4_2(base, uint32_t(r3)).unknown_1820_address(), uint32_t(r11));
  // blr
  return;
}

// sub_8224A368 game::PanelSetBase::BackPage7: lifted from generated/ by lift.py.
uint64_t f8224A368(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224A368 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lwz r11,1892(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).back_object_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8224a390
  if (cr0.eq) goto loc_8224A390;
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::CarListBackObject(base, uint32_t(r11)).page7_value_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::CarListBackObject(base, uint32_t(r11)).value_address(), uint32_t(r10));
loc_8224A390:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,7148(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 7148);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,24240(r10)
  guest::StoreU32(base, uint32_t(r10) + 24240, uint32_t(r11));
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8224A3B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).page_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8224a3c8
  if (cr6.eq) goto loc_8224A3C8;
  // stw r11,40(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).previous_page_address(), uint32_t(r11));
loc_8224A3C8:
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r10,44(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).step_address(), uint32_t(r10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,48(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).queued_step_address(), uint32_t(r11));
  // stw r11,32(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).page_address(), uint32_t(r11));
  // stw r10,36(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).next_page_address(), uint32_t(r10));
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

// sub_8224A850 game::PanelSetBase::Step13: lifted from generated/ by lift.py.
lifted::Result f8224A850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8224A850 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8224a8c0
  if (!cr6.eq) goto loc_8224A8C0;
  // addi r3,r31,132
  r3 = uint64_t(int64_t(int64_t(r31) + 132));
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(base, LIFTED_ARGS(0x8224A880, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,136(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).rows_menu_scroll_address());
  // mr r10,r3
  r10 = r3;
  // lwz r3,1908(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,240(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).scroll_address(), uint32_t(r11));
  // stw r10,236(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).row_address(), uint32_t(r10));
  // stw r9,24(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).stand_model_loaded_address(), uint32_t(r9));
  // beq 0x8224a8b8
  if (cr0.eq) goto loc_8224A8B8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,1820(r31)
  r10 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).current_list_address());
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r10
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x8224ded8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8224DED8, LIFTED_ARGS(0x8224A8B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224A8B8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
loc_8224A8C0:
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_8224B138 game::PanelSetBase::Step5: lifted from generated/ by lift.py.
lifted::Result f8224B138(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224B138 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8224b198
  if (!cr6.eq) goto loc_8224B198;
  // addi r3,r31,60
  r3 = uint64_t(int64_t(int64_t(r31) + 60));
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(base, LIFTED_ARGS(0x8224B168, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,272
  r11 = uint64_t(int64_t(int64_t(r31) + 272));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r11,1820(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).current_list_address(), uint32_t(r11));
  // stw r10,232(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).tab_address(), uint32_t(r10));
  // bl 0x8224a978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8224A978, LIFTED_ARGS(0x8224B180, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,1900(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).rows_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8224b190
  if (cr0.eq) goto loc_8224B190;
  // bl 0x8224f158
  // (left out: dead in C++)
  { const lifted::Result o = game::RowPanel::ShowFirst(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_8224B190:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
loc_8224B198:
  // lwz r11,1900(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).rows_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8224b1b0
  if (cr0.eq) goto loc_8224B1B0;
  // mr r3,r11
  r3 = r11;
  // bl 0x8224f5e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8224F5E8, LIFTED_ARGS(0x8224B1B0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224B1B0:
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

// sub_8224B1C8 game::PanelSetBase::Step14: lifted from generated/ by lift.py.
lifted::Result f8224B1C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224B1C8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8224b218
  if (!cr6.eq) goto loc_8224B218;
  // lwz r3,1908(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8224b200
  if (cr0.eq) goto loc_8224B200;
  // bl 0x8224dc50
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPanel::Hide(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_8224B200:
  // mr r3,r31
  r3 = r31;
  // bl 0x8224a978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8224A978, LIFTED_ARGS(0x8224B208, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,20(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).show_stand_car_address(), uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
loc_8224B218:
  // lwz r11,1908(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8224b230
  if (cr0.eq) goto loc_8224B230;
  // mr r3,r11
  r3 = r11;
  // bl 0x8224e1a8
  // (left out: dead in C++)
  r3 = game::CarPanel::IsHidden(base, r1, r13, r3);
loc_8224B230:
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

// sub_8224B6E0 game::PanelSet::OpenKind: lifted from generated/ by lift.py.
uint64_t f8224B6E0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224B6E0 (lifted)");
  // lwz r3,1932(r3)
  r3 = guest::LoadU32(base, schema::PanelSet(base, uint32_t(r3)).open_kind_address());
  // blr
  return r3;
}

// sub_8224B700 game::PanelSet::SetKind: lifted from generated/ by lift.py.
uint64_t f8224B700(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8224B700 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // b 0x82249610
  game::PanelSetBase::SetKind(base, r3, r4);
  return r3;
}

// sub_8224B888 game::PanelSetBase::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f8224B888(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8224B888 (lifted)");
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
  // bl 0x8224ae60
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelSetBase::Dtor(base, LIFTED_ARGS(0x8224B8A8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8224b8b8
  if (cr0.eq) goto loc_8224B8B8;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8224B8B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224B8B8:
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

// sub_8224C4D8 game::PanelSetBase::Step25: lifted from generated/ by lift.py.
lifted::Result f8224C4D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_8224C4D8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8224c50c
  if (!cr6.eq) goto loc_8224C50C;
  // bl 0x8224b248
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8224B248, LIFTED_ARGS(0x8224C504, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
loc_8224C50C:
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).page_address());
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x8224c51c
  if (cr6.eq) goto loc_8224C51C;
  // stw r11,40(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).previous_page_address(), uint32_t(r11));
loc_8224C51C:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r10,18
  r10 = uint64_t(int64_t(18));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,48(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).queued_step_address(), uint32_t(r11));
  // stw r10,44(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).step_address(), uint32_t(r10));
  // stw r9,36(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).next_page_address(), uint32_t(r9));
  // stw r11,32(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).page_address(), uint32_t(r11));
  // stw r8,56(r31)
  guest::StoreU32(base, schema::PanelSetBase(base, uint32_t(r31)).first_pass_address(), uint32_t(r8));
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

// sub_8224C9D8 game::PanelSet::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f8224C9D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8224C9D8 (lifted)");
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
  // bl 0x8224b708
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelSet::Dtor(base, LIFTED_ARGS(0x8224C9F8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8224ca08
  if (cr0.eq) goto loc_8224CA08;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8224CA08, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224CA08:
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

// sub_8224CC00 game::PanelSetBase::Step10: lifted from generated/ by lift.py.
lifted::Result f8224CC00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224CC00 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8224cc48
  if (!cr6.eq) goto loc_8224CC48;
  // lwz r3,1908(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8224cc38
  if (cr0.eq) goto loc_8224CC38;
  // bl 0x8224dba0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPanel::ShowAgain(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_8224CC38:
  // mr r3,r31
  r3 = r31;
  // bl 0x8224c560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8224C560, LIFTED_ARGS(0x8224CC40, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
loc_8224CC48:
  // lwz r11,1908(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).car_panel_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8224cc60
  if (cr0.eq) goto loc_8224CC60;
  // mr r3,r11
  r3 = r11;
  // bl 0x8224e168
  // (left out: dead in C++)
  r3 = game::CarPanel::IsShownAgain(base, r1, r13, r3);
loc_8224CC60:
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

// sub_8224CC78 game::PanelSetBase::Step20: lifted from generated/ by lift.py.
lifted::Result f8224CC78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224CC78 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8224ccd0
  if (!cr6.eq) goto loc_8224CCD0;
  // lwz r11,1904(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).color_panel_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8224ccc0
  if (cr6.eq) goto loc_8224CCC0;
  // addi r3,r31,60
  r3 = uint64_t(int64_t(int64_t(r31) + 60));
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(base, LIFTED_ARGS(0x8224CCB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,1904(r31)
  r3 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).color_panel_address());
  // bl 0x8224d090
  // (left out: dead in C++)
  { const lifted::Result o = game::ColorPanel::Hide(base, r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_8224CCC0:
  // mr r3,r31
  r3 = r31;
  // bl 0x8224c560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8224C560, LIFTED_ARGS(0x8224CCC8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CarListStepWord(base, uint32_t(r30)).first_pass_address(), uint32_t(r11));
loc_8224CCD0:
  // lwz r11,1904(r31)
  r11 = guest::LoadU32(base, schema::PanelSetBase(base, uint32_t(r31)).color_panel_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8224cce8
  if (cr0.eq) goto loc_8224CCE8;
  // mr r3,r11
  r3 = r11;
  // bl 0x8224d230
  // (left out: dead in C++)
  r3 = game::ColorPanel::IsHidden(base, r1, r13, r3);
loc_8224CCE8:
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

// sub_8224D090 game::ColorPanel::Hide: lifted from generated/ by lift.py.
lifted::Result f8224D090(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224D090 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r29,r4
  r29 = r4;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224D0AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).item_anims_address());
  // mr r30,r3
  r30 = r3;
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224D0BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).item_anims_address());
  // mr r3,r30
  r3 = r30;
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x8211de30
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::UnloopNamed(base, LIFTED_ARGS(0x8224D0CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // lwz r4,20(r31)
  r4 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).hide_anim_address());
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8224D0E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,52(r31)
  r4 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).arrow_a_address());
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224d0fc
  if (cr0.lt) goto loc_8224D0FC;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224D0FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224D0FC:
  // lwz r4,56(r31)
  r4 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).arrow_b_address());
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224d110
  if (cr0.lt) goto loc_8224D110;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224D110, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224D110:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8224D230 game::ColorPanel::IsHidden: lifted from generated/ by lift.py.
uint64_t f8224D230(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224D230 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224D250, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,20(r31)
  r4 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).hide_anim_address());
  // mr r30,r3
  r30 = r3;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8224D25C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // lwz r4,20(r31)
  r4 = guest::LoadU32(base, schema::ColorPanel(base, uint32_t(r31)).hide_anim_address());
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8224D268, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_8224DBA0 game::CarPanel::ShowAgain: lifted from generated/ by lift.py.
lifted::Result f8224DBA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224DBA0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224DBC0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).action_address());
  // mr r30,r3
  r30 = r3;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x8224dbe8
  if (cr0.lt) goto loc_8224DBE8;
  // addi r11,r11,17
  r11 = uint64_t(int64_t(int64_t(r11) + 17));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r31
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r31));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224DBE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,8(r31)
  guest::StoreU32(base, schema::CarPanel(base, uint32_t(r31)).action_address(), uint32_t(r11));
loc_8224DBE8:
  // lwz r4,56(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).show_again_anim_address());
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224dc0c
  if (cr0.lt) goto loc_8224DC0C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8224DC0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224DC0C:
  // lwz r4,60(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).arrow_a_address());
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224dc20
  if (cr0.lt) goto loc_8224DC20;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224DC20, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224DC20:
  // lwz r4,64(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).arrow_b_address());
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224dc34
  if (cr0.lt) goto loc_8224DC34;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224DC34, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224DC34:
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

// sub_8224DC50 game::CarPanel::Hide: lifted from generated/ by lift.py.
lifted::Result f8224DC50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224DC50 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224DC68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).action_address());
  // mr r29,r3
  r29 = r3;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x8224dc90
  if (cr0.lt) goto loc_8224DC90;
  // addi r11,r11,17
  r11 = uint64_t(int64_t(int64_t(r11) + 17));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r31
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r31));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224DC88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,8(r31)
  guest::StoreU32(base, schema::CarPanel(base, uint32_t(r31)).action_address(), uint32_t(r11));
loc_8224DC90:
  // lwz r4,60(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).arrow_a_address());
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224dca4
  if (cr0.lt) goto loc_8224DCA4;
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224DCA4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224DCA4:
  // lwz r4,64(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).arrow_b_address());
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224dcb8
  if (cr0.lt) goto loc_8224DCB8;
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224DCB8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224DCB8:
  // addi r30,r31,48
  r30 = uint64_t(int64_t(int64_t(r31) + 48));
  // li r31,2
  r31 = uint64_t(int64_t(2));
loc_8224DCC0:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r4,0(r30)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r30) - 48).hide_anim_a_address());
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8224DCDC, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x8224dcc0
  if (!cr0.eq) goto loc_8224DCC0;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8224DF90 game::CarPanel::PlayStats: lifted from generated/ by lift.py.
lifted::Result f8224DF90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224DF90 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224DFB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,116(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).stats_in_address());
  // mr r30,r3
  r30 = r3;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224DFBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r4,116(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).stats_in_address());
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8224DFD8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8224DFF0 game::CarPanel::PlayStatsOut: lifted from generated/ by lift.py.
lifted::Result f8224DFF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224DFF0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224E010, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,120(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).stats_out_address());
  // mr r30,r3
  r30 = r3;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224E01C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r4,120(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).stats_out_address());
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8224E038, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8224E168 game::CarPanel::IsShownAgain: lifted from generated/ by lift.py.
uint64_t f8224E168(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224E168 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224E184, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,56(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).show_again_anim_address());
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8224E18C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_8224E1A8 game::CarPanel::IsHidden: lifted from generated/ by lift.py.
uint64_t f8224E1A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224E1A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224E1C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r31,r31,48
  r31 = uint64_t(int64_t(int64_t(r31) + 48));
loc_8224E1D0:
  // mr r3,r29
  r3 = r29;
  // lwz r4,0(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31) - 48).hide_anim_a_address());
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8224E1DC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8224e1f8
  if (!cr0.eq) goto loc_8224E1F8;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplwi cr6,r30,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 2, so);
  // blt cr6,0x8224e1d0
  if (cr6.lt) goto loc_8224E1D0;
  // b 0x8224e1fc
  goto loc_8224E1FC;
loc_8224E1F8:
  // li r28,0
  r28 = uint64_t(int64_t(0));
loc_8224E1FC:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8224E208 game::CarPanel::StatsShown: lifted from generated/ by lift.py.
uint64_t f8224E208(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224E208 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224E224, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,116(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).stats_in_address());
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8224E22C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_8224E248 game::CarPanel::StatsHidden: lifted from generated/ by lift.py.
uint64_t f8224E248(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224E248 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224E264, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,120(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).stats_out_address());
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8224E26C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_8224E7E8 game::CarPanel::SetPrice: lifted from generated/ by lift.py.
lifted::Result f8224E7E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8224E7E8 (lifted)");
  uint64_t r5 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r28,r4
  r28 = r4;
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224E804, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r5,28(r31)
  r5 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).price_node_address());
  // mr r30,r3
  r30 = r3;
  // lwz r4,16(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).node_set_address());
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8224E814, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,260(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).price_child_address());
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8224E820, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r28
  r5 = r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ca18
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeNumberText(base, LIFTED_ARGS(0x8224E830, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // lwz r4,268(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).price_text_b_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8224E83C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,435
  r5 = uint64_t(int64_t(435));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8224E84C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // lwz r4,264(r31)
  r4 = guest::LoadU32(base, schema::CarPanel(base, uint32_t(r31)).price_text_a_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8224E858, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r28
  r6 = r28;
  // li r5,527
  r5 = uint64_t(int64_t(527));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8224E86C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8224EB68 game::CarPanelBase::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f8224EB68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8224EB68 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,4544
  r11 = uint64_t(int64_t(int64_t(r11) + 4544));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::CarPanel(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x8224eb94
  if (cr0.eq) goto loc_8224EB94;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8224EB94, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224EB94:
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

// sub_8224EBF0 game::ConfirmPanel::Play: lifted from generated/ by lift.py.
lifted::Result f8224EBF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8224EBF0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // mr r27,r7
  r27 = r7;
  // mr r26,r8
  r26 = r8;
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x8224ec48
  if (cr6.eq) goto loc_8224EC48;
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r3)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224EC20, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r30,r3
  r30 = r3;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224EC2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r26
  r8 = r26;
  // mr r7,r27
  r7 = r27;
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8224EC48, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224EC48:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8224EC50 game::ConfirmPanel::PlayLooped: lifted from generated/ by lift.py.
lifted::Result f8224EC50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8224EC50 (lifted)");
  uint64_t r9 = 0, r10 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // mr r27,r7
  r27 = r7;
  // mr r26,r8
  r26 = r8;
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x8224ecb4
  if (cr6.eq) goto loc_8224ECB4;
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r3)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224EC80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r30,r3
  r30 = r3;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224EC8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r26
  r8 = r26;
  // mr r7,r27
  r7 = r27;
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8224ECA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8224ECB4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224ECB4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8224ECC0 game::ConfirmPanel::ShowNode: lifted from generated/ by lift.py.
uint64_t f8224ECC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8224ECC0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224ECE4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).kind_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8224ecf8
  if (!cr6.eq) goto loc_8224ECF8;
  // lwz r5,20(r31)
  r5 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).node_b_address());
  // b 0x8224ecfc
  goto loc_8224ECFC;
loc_8224ECF8:
  // lwz r5,24(r31)
  r5 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).node_c_address());
loc_8224ECFC:
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // blt cr6,0x8224ed1c
  if (cr6.lt) goto loc_8224ED1C;
  // lwz r4,16(r31)
  r4 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).node_a_address());
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x8224ed18
  if (cr6.eq) goto loc_8224ED18;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8224ED14, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8224ed1c
  goto loc_8224ED1C;
loc_8224ED18:
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8224ED1C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224ED1C:
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

// sub_8224ED38 game::ConfirmPanel::Show: lifted from generated/ by lift.py.
lifted::Result f8224ED38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_8224ED38 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r29,r4
  r29 = r4;
  // lwz r4,32(r31)
  r4 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).loop_anim_address());
  // bl 0x8224ebf0
  // (left out: dead in C++)
  { const lifted::Result o = game::ConfirmPanel::Play(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r30,32(r31)
  r30 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).loop_anim_address());
  // cmpwi cr6,r30,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r30), -1, so);
  // beq cr6,0x8224ed80
  if (cr6.eq) goto loc_8224ED80;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224ED78, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224ED80, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224ED80:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r4,28(r31)
  r4 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).show_anim_address());
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8224ebf0
  // (left out: dead in C++)
  { const lifted::Result o = game::ConfirmPanel::Play(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // stw r29,8(r31)
  guest::StoreU32(base, schema::ConfirmPanel(base, uint32_t(r31)).kind_address(), uint32_t(r29));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8224EDA8 game::ConfirmPanel::Loop: lifted from generated/ by lift.py.
lifted::Result f8224EDA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8224EDA8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0;
  // lwz r4,32(r3)
  r4 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r3)).loop_anim_address());
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // b 0x8224ec50
  { const lifted::Result o = game::ConfirmPanel::PlayLooped(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8224EDC0 game::ConfirmPanel::Hide: lifted from generated/ by lift.py.
lifted::Result f8224EDC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224EDC0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r30,32(r31)
  r30 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).loop_anim_address());
  // cmpwi cr6,r30,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r30), -1, so);
  // beq cr6,0x8224edf4
  if (cr6.eq) goto loc_8224EDF4;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224EDEC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224EDF4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224EDF4:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r4,36(r31)
  r4 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r31)).hide_anim_address());
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8224ebf0
  // (left out: dead in C++)
  { const lifted::Result o = game::ConfirmPanel::Play(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_8224EE28 game::ConfirmPanel::IsShown: lifted from generated/ by lift.py.
uint64_t f8224EE28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224EE28 (lifted)");
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
  // lwz r31,28(r3)
  r31 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r3)).show_anim_address());
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r3)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224EE44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8224EE4C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_8224EE68 game::ConfirmPanel::IsHidden: lifted from generated/ by lift.py.
uint64_t f8224EE68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224EE68 (lifted)");
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
  // lwz r31,36(r3)
  r31 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r3)).hide_anim_address());
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, schema::ConfirmPanel(base, uint32_t(r3)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224EE84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8224EE8C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_8224F158 game::RowPanel::ShowFirst: lifted from generated/ by lift.py.
lifted::Result f8224F158(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8224F158 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RowPanel(base, uint32_t(r31)).group_address());
  // bl 0x82258fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::PanelGroup::Movie(base, LIFTED_ARGS(0x8224F178, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,104(r31)
  r4 = guest::LoadU32(base, schema::RowPanel(base, uint32_t(r31)).arrow_up_address());
  // mr r30,r3
  r30 = r3;
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224f18c
  if (cr0.lt) goto loc_8224F18C;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224F18C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224F18C:
  // lwz r4,108(r31)
  r4 = guest::LoadU32(base, schema::RowPanel(base, uint32_t(r31)).arrow_down_address());
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blt 0x8224f1a0
  if (cr0.lt) goto loc_8224F1A0;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8224F1A0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8224F1A0:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r4,84(r31)
  r4 = guest::LoadU32(base, schema::RowPanel(base, uint32_t(r31)).show_first_anim_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8224F1BC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82260800: lifted from generated/ by lift.py.
lifted::Result f82260800(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82260800 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1408(r1)
  ea = -1408 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r29,r11,30752
  r29 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,88(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r28,r11,29464
  r28 = uint64_t(int64_t(int64_t(r11) + 29464));
  // bne cr6,0x82260854
  if (!cr6.eq) goto loc_82260854;
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stb r11,82(r1)
  guest::StoreU8(base, uint32_t(r1) + 82, uint8_t(r11));
  // bl 0x821489d8
  // (left out: dead in C++)
  { const lifted::Result o = f821489D8(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226088c
  if (!cr6.eq) goto loc_8226088C;
  // lwz r4,8(r28)
  r4 = guest::LoadU32(base, uint32_t(r28) + 8);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82260854
  if (cr6.eq) goto loc_82260854;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x82260854, r31)); r3 = o.r3; f1 = o.f1; }
loc_82260854:
  // li r5,60
  r5 = uint64_t(int64_t(60));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r29,8
  r3 = uint64_t(int64_t(int64_t(r29) + 8));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82260864, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r3,4108
  r3 = uint64_t(int64_t(4108));
  // stw r11,88(r29)
  guest::StoreU32(base, uint32_t(r29) + 88, uint32_t(r11));
  // stw r11,27368(r10)
  guest::StoreU32(base, uint32_t(r10) + 27368, uint32_t(r11));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,8(r28)
  guest::StoreU32(base, uint32_t(r28) + 8, uint32_t(r11));
  // addi r1,r1,1408
  r1 = uint64_t(int64_t(int64_t(r1) + 1408));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_8226088C:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,29448
  r30 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r10,8(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822608a8
  if (!cr6.gt) goto loc_822608A8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_822608A8:
  // mr r31,r11
  r31 = r11;
loc_822608AC:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822608b8
  if (!cr6.gt) goto loc_822608B8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_822608B8:
  // subf r11,r31,r10
  r11 = r10 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82260854
  if (cr0.eq) goto loc_82260854;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x822608dc
  if (cr6.lt) goto loc_822608DC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_822608DC:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x822608E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,8(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x822608f8
  if (cr6.lt) goto loc_822608F8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_822608F8:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
  // b 0x822608ac
  goto loc_822608AC;
}

// sub_8228F7D0: lifted from generated/ by lift.py.
lifted::Result f8228F7D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8228F7D0 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // rlwinm r10,r3,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,-21596
  r11 = uint64_t(int64_t(int64_t(r11) + -21596));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // lwzx r3,r10,r11
  r3 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // b 0x82295020
  { const lifted::Result o = game::XUserGetName(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8228FA20: lifted from generated/ by lift.py.
lifted::Result f8228FA20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_8228FA20 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // rlwinm r10,r3,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,-21596
  r11 = uint64_t(int64_t(int64_t(r11) + -21596));
  // lwzx r3,r10,r11
  r3 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x8228f780
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8228F780, LIFTED_ARGS(0x8228FA40, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820a00f8
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveInterface::Word992(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8228FA58: lifted from generated/ by lift.py.
lifted::Result f8228FA58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_8228FA58 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // rlwinm r10,r3,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,-21596
  r11 = uint64_t(int64_t(int64_t(r11) + -21596));
  // lwzx r3,r10,r11
  r3 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x8228f780
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8228F780, LIFTED_ARGS(0x8228FA78, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820a01b0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveInterface::Request980(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8228FD50: lifted from generated/ by lift.py.
lifted::Result f8228FD50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_8228FD50 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // rlwinm r10,r3,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,-21596
  r11 = uint64_t(int64_t(int64_t(r11) + -21596));
  // lwzx r3,r10,r11
  r3 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x8228f780
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8228F780, LIFTED_ARGS(0x8228FD70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // bl 0x820a02c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A02C8, LIFTED_ARGS(0x8228FD78, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x820a0018
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0018, LIFTED_ARGS(0x8228FD84, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // bne cr6,0x8228fd94
  if (!cr6.eq) goto loc_8228FD94;
loc_8228FD8C:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x8228fe34
  goto loc_8228FE34;
loc_8228FD94:
  // cmpwi cr6,r3,3
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 3, so);
  // bne cr6,0x8228fe30
  if (!cr6.eq) goto loc_8228FE30;
  // mr r3,r31
  r3 = r31;
  // bl 0x820a0378
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0378, LIFTED_ARGS(0x8228FDA4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x8228fe30
  if (!cr0.eq) goto loc_8228FE30;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // ble cr6,0x8228fd8c
  if (!cr6.gt) goto loc_8228FD8C;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // addi r29,r11,-32536
  r29 = uint64_t(int64_t(int64_t(r11) + -32536));
loc_8228FDC0:
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x820a0428
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0428, LIFTED_ARGS(0x8228FDCC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r29
  r10 = r29;
  // addi r11,r3,264
  r11 = uint64_t(int64_t(int64_t(r3) + 264));
loc_8228FDD4:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, schema::XContentData(base, uint32_t(r11) - 264).file_name_address());
  // lbz r8,0(r10)
  r8 = guest::LoadU8(base, uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x8228fdf8
  if (cr0.eq) goto loc_8228FDF8;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x8228fdd4
  if (cr6.eq) goto loc_8228FDD4;
loc_8228FDF8:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x8228fe10
  if (cr0.eq) goto loc_8228FE10;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpw cr6,r30,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r28), so);
  // blt cr6,0x8228fdc0
  if (cr6.lt) goto loc_8228FDC0;
  // b 0x8228fd8c
  goto loc_8228FD8C;
loc_8228FE10:
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x820a0260
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveInterface::Word84(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // addi r3,r11,7
  r3 = uint64_t(int64_t(int64_t(r11) + 7));
  // b 0x8228fe34
  goto loc_8228FE34;
loc_8228FE30:
  // li r3,4
  r3 = uint64_t(int64_t(4));
loc_8228FE34:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8228FF88 game::Replays::Load: lifted from generated/ by lift.py.
lifted::Result f8228FF88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_8228FF88 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mulli r9,r3,36
  r9 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(36))));
  // lwz r11,25072(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25072);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, schema::SaveBuffers(base, uint32_t(r11)).records_address());
  // lwz r8,20(r11)
  r8 = guest::LoadU32(base, schema::SaveBuffers(base, uint32_t(r11)).count_address());
  // add r9,r10,r9
  r9 = r10 + r9;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // lwz r9,28(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 28);
  // ble 0x8228fff8
  if (!cr0.gt) goto loc_8228FFF8;
  // lwz r7,16(r11)
  r7 = guest::LoadU32(base, schema::SaveBuffers(base, uint32_t(r11)).entries_address());
  // addi r11,r7,300
  r11 = uint64_t(int64_t(int64_t(r7) + 300));
loc_8228FFD0:
  // lwz r6,0(r11)
  r6 = guest::LoadU32(base, schema::SaveReplayEntry(base, uint32_t(r11) - 300).number_address());
  // cmpw cr6,r6,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r9), so);
  // beq cr6,0x8228fff0
  if (cr6.eq) goto loc_8228FFF0;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r11,304
  r11 = uint64_t(int64_t(int64_t(r11) + 304));
  // cmpw cr6,r10,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r8), so);
  // blt cr6,0x8228ffd0
  if (cr6.lt) goto loc_8228FFD0;
  // b 0x8228fff8
  goto loc_8228FFF8;
loc_8228FFF0:
  // mulli r11,r10,304
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(304))));
  // add r31,r11,r7
  r31 = r11 + r7;
loc_8228FFF8:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x8228f780
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8228F780, LIFTED_ARGS(0x82290004, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r8,19
  r8 = uint64_t(int64_t(1245184));
  // addi r6,r11,7880
  r6 = uint64_t(int64_t(int64_t(r11) + 7880));
  // lwz r11,25072(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25072);
  // ori r8,r8,51440
  r8 = r8 | 51440;
  // mr r5,r31
  r5 = r31;
  // addi r4,r31,42
  r4 = uint64_t(int64_t(int64_t(r31) + 42));
  // lwz r7,4(r11)
  r7 = guest::LoadU32(base, schema::SaveBuffers(base, uint32_t(r11)).replay_address());
  // bl 0x820a0510
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0510, LIFTED_ARGS(0x82290028, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82290640: lifted from generated/ by lift.py.
lifted::Result f82290640(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82290640 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // mulli r11,r11,36
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(36))));
  // lwz r3,-21596(r10)
  r3 = guest::LoadU32(base, uint32_t(r10) + -21596);
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,25072(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 25072);
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, schema::SaveBuffers(base, uint32_t(r10)).records_address());
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r31,28(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 28);
  // bl 0x8228f780
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8228F780, LIFTED_ARGS(0x82290678, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // bl 0x820a0260
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveInterface::Word84(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
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

// sub_82291408: lifted from generated/ by lift.py.
lifted::Result f82291408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82291408 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r31,r11,26128
  r31 = uint64_t(int64_t(int64_t(r11) + 26128));
  // b 0x82291450
  goto loc_82291450;
loc_82291428:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stw r11,64(r31)
  guest::StoreU32(base, uint32_t(r31) + 64, uint32_t(r11));
  // lwzx r30,r10,r31
  r30 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r31));
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq 0x82291454
  if (cr0.eq) goto loc_82291454;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e160
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Dtor(base, LIFTED_ARGS(0x82291448, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82291450, r31)); r3 = o.r3; f1 = o.f1; }
loc_82291450:
  // lwz r11,64(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 64);
loc_82291454:
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82291428
  if (!cr0.eq) goto loc_82291428;
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

// sub_82291520: lifted from generated/ by lift.py.
lifted::Result f82291520(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82291520 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x8229153C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r10,r10,30320
  r10 = uint64_t(int64_t(int64_t(r10) + 30320));
  // rlwinm r30,r11,3,0,28
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // addi r31,r10,144
  r31 = uint64_t(int64_t(int64_t(r10) + 144));
  // addi r7,r10,14
  r7 = uint64_t(int64_t(int64_t(r10) + 14));
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // add r5,r11,r7
  r5 = r11 + r7;
  // ldx r7,r30,r31
  r7 = guest::LoadU64(base, uint32_t(r30) + uint32_t(r31));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,256
  r8 = uint64_t(int64_t(256));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,28220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28220);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // std r7,168(r11)
  guest::StoreU64(base, schema::Session(base, uint32_t(r11)).xuid_address(), r7);
  // lwz r11,28220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28220);
  // addi r7,r11,248
  r7 = uint64_t(int64_t(int64_t(r11) + 248));
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = game::WideCharToMultiByte(base, LIFTED_ARGS(0x82291590, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,28220(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 28220);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r11,29320(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29320);
  // stw r11,220(r10)
  guest::StoreU32(base, schema::Session(base, uint32_t(r10)).unknown_220_address(), uint32_t(r11));
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

// sub_822916D0: lifted from generated/ by lift.py.
uint64_t f822916D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822916D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r6,r11,8560
  r6 = uint64_t(int64_t(int64_t(r11) + 8560));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,660
  r4 = uint64_t(int64_t(660));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822916FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // li r4,150
  r4 = uint64_t(int64_t(150));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // addi r6,r11,8544
  r6 = uint64_t(int64_t(int64_t(r11) + 8544));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // beq cr6,0x82291720
  if (cr6.eq) goto loc_82291720;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291720:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291724, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // addi r6,r11,8520
  r6 = uint64_t(int64_t(int64_t(r11) + 8520));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // beq cr6,0x82291748
  if (cr6.eq) goto loc_82291748;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291748:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8229174C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82291760: lifted from generated/ by lift.py.
lifted::Result f82291760(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82291760 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82291798
  if (!cr6.eq) goto loc_82291798;
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // stwx r10,r31,r11
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r11), uint32_t(r10));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82291798:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,160(r31)
  guest::StoreU32(base, uint32_t(r31) + 160, uint32_t(r11));
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

// sub_822917B8: lifted from generated/ by lift.py.
lifted::Result f822917B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_822917B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
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
  // lwz r30,-21596(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822917E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r4,0
  r4 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // ori r4,r4,32778
  r4 = r4 | 32778;
  // mr r3,r30
  r3 = r30;
  // bl 0x82294f70
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserSetContextEx(base, LIFTED_ARGS(0x822917FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r9,r31
  r9 = r31;
  // addi r10,r11,27324
  r10 = uint64_t(int64_t(int64_t(r11) + 27324));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r8,r11,26192
  r8 = uint64_t(int64_t(int64_t(r11) + 26192));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r7,r11,27328
  r7 = uint64_t(int64_t(int64_t(r11) + 27328));
  // mr r4,r30
  r4 = r30;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x82575e50
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionCreate(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x8229183c
  if (cr6.eq) goto loc_8229183C;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8229183C:
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r29,r11
  guest::StoreU32(base, uint32_t(r29) + uint32_t(r11), uint32_t(r10));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82291858: lifted from generated/ by lift.py.
lifted::Result f82291858(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82291858 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // addi r31,r27,16
  r31 = uint64_t(int64_t(int64_t(r27) + 16));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,27336(r30)
  guest::StoreU32(base, uint32_t(r30) + 27336, uint32_t(r11));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // stw r11,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r11));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82291890, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r10,r10,30320
  r10 = uint64_t(int64_t(int64_t(r10) + 30320));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // addi r8,r10,144
  r8 = uint64_t(int64_t(int64_t(r10) + 144));
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // addi r29,r9,25312
  r29 = uint64_t(int64_t(int64_t(r9) + 25312));
  // lwz r9,27336(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 27336);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // addi r28,r10,24704
  r28 = uint64_t(int64_t(int64_t(r10) + 24704));
  // ldx r11,r11,r8
  r11 = guest::LoadU64(base, uint32_t(r11) + uint32_t(r8));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // stw r10,27336(r30)
  guest::StoreU32(base, uint32_t(r30) + 27336, uint32_t(r10));
  // stdx r11,r9,r29
  guest::StoreU64(base, uint32_t(r9) + uint32_t(r29), r11);
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822918DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r7,r28
  r7 = r28;
  // mr r6,r31
  r6 = r31;
  // mr r5,r29
  r5 = r29;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,27324(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 27324);
  // bl 0x825761d8
  // (left out: dead in C++)
  { const lifted::Result o = f825761D8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x82291908
  if (cr6.eq) goto loc_82291908;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82291908:
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r27,r11
  guest::StoreU32(base, uint32_t(r27) + uint32_t(r11), uint32_t(r10));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82291920: lifted from generated/ by lift.py.
lifted::Result f82291920(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82291920 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82291950, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r5,r31
  r5 = r31;
  // lwz r3,27324(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 27324);
  // bl 0x825764a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionStart(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x82291974
  if (cr6.eq) goto loc_82291974;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82291974:
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r30,r11
  guest::StoreU32(base, uint32_t(r30) + uint32_t(r11), uint32_t(r10));
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

// sub_822919A0: lifted from generated/ by lift.py.
lifted::Result f822919A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822919A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // lwz r3,27348(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 27348);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x822919cc
  if (cr6.eq) goto loc_822919CC;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x822919C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,27348(r31)
  guest::StoreU32(base, uint32_t(r31) + 27348, uint32_t(r11));
loc_822919CC:
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lwz r11,27324(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 27324);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x822919f0
  if (!cr6.eq) goto loc_822919F0;
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r30,r11
  guest::StoreU32(base, uint32_t(r30) + uint32_t(r11), uint32_t(r10));
  // b 0x82291a34
  goto loc_82291A34;
loc_822919F0:
  // addis r30,r30,20
  r30 = uint64_t(int64_t(int64_t(r30) + 1310720));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // addi r30,r30,-13000
  r30 = uint64_t(int64_t(int64_t(r30) + -13000));
  // addi r31,r11,24704
  r31 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r10));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82291A18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // lwz r3,27324(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 27324);
  // bl 0x82576540
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionEnd(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x82291a34
  if (cr6.eq) goto loc_82291A34;
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82291A34:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82291A40: lifted from generated/ by lift.py.
lifted::Result f82291A40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82291A40 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lwz r11,27324(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 27324);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82291a70
  if (!cr6.eq) goto loc_82291A70;
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r3,r11
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r11), uint32_t(r10));
  // b 0x82291ac4
  goto loc_82291AC4;
loc_82291A70:
  // addis r30,r3,20
  r30 = uint64_t(int64_t(int64_t(r3) + 1310720));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // addi r30,r30,-13000
  r30 = uint64_t(int64_t(int64_t(r30) + -13000));
  // addi r31,r11,24704
  r31 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r10));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82291A98, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r6,r31
  r6 = r31;
  // lwz r3,27324(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 27324);
  // addi r5,r11,25312
  r5 = uint64_t(int64_t(int64_t(r11) + 25312));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r4,27336(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 27336);
  // bl 0x82576328
  // (left out: dead in C++)
  { const lifted::Result o = f82576328(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x82291ac4
  if (cr6.eq) goto loc_82291AC4;
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82291AC4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82291AD0: lifted from generated/ by lift.py.
lifted::Result f82291AD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82291AD0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lwz r11,27324(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 27324);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82291b00
  if (!cr6.eq) goto loc_82291B00;
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r3,r11
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r11), uint32_t(r10));
  // b 0x82291b44
  goto loc_82291B44;
loc_82291B00:
  // addis r30,r3,20
  r30 = uint64_t(int64_t(int64_t(r3) + 1310720));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // addi r30,r30,-13000
  r30 = uint64_t(int64_t(int64_t(r30) + -13000));
  // addi r31,r11,24704
  r31 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r10));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82291B28, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // lwz r3,27324(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 27324);
  // bl 0x82576080
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionDelete(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x82291b44
  if (cr6.eq) goto loc_82291B44;
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82291B44:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82291B50: lifted from generated/ by lift.py.
lifted::Result f82291B50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82291B50 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // beq cr6,0x82291c30
  if (cr6.eq) goto loc_82291C30;
  // lwz r11,144(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 144);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82291c14
  if (cr6.eq) goto loc_82291C14;
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x82291B88);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82291b9c
  if (!cr0.lt) goto loc_82291B9C;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,180(r31)
  guest::StoreU32(base, uint32_t(r31) + 180, uint32_t(r11));
  // b 0x82291c34
  goto loc_82291C34;
loc_82291B9C:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r8,r5
  r8 = r5;
  // stw r5,184(r31)
  guest::StoreU32(base, uint32_t(r31) + 184, uint32_t(r5));
  // lwz r11,27348(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r7,8(r10)
  r7 = guest::LoadU32(base, uint32_t(r10) + 8);
  // cmplwi r7,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq 0x82291c0c
  if (cr0.eq) goto loc_82291C0C;
  // lwz r6,148(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 148);
  // mr r11,r5
  r11 = r5;
loc_82291BC8:
  // lwz r9,12(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 12);
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq cr6,0x82291be4
  if (cr6.eq) goto loc_82291BE4;
  // add r9,r9,r11
  r9 = r9 + r11;
  // lwz r9,8(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 8);
  // cmplw cr6,r6,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r9), so);
  // b 0x82291bf0
  goto loc_82291BF0;
loc_82291BE4:
  // ld r4,152(r31)
  r4 = guest::LoadU64(base, uint32_t(r31) + 152);
  // ldx r9,r9,r11
  r9 = guest::LoadU64(base, uint32_t(r9) + uint32_t(r11));
  // cmpld cr6,r4,r9
  cr6 = lifted::Compare<uint64_t>(r4, r9, so);
loc_82291BF0:
  // beq cr6,0x82291c08
  if (cr6.eq) goto loc_82291C08;
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r11,r11,48
  r11 = uint64_t(int64_t(int64_t(r11) + 48));
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // blt cr6,0x82291bc8
  if (cr6.lt) goto loc_82291BC8;
  // b 0x82291c0c
  goto loc_82291C0C;
loc_82291C08:
  // stw r8,184(r31)
  guest::StoreU32(base, uint32_t(r31) + 184, uint32_t(r8));
loc_82291C0C:
  // stw r5,144(r31)
  guest::StoreU32(base, uint32_t(r31) + 144, uint32_t(r5));
  // b 0x82291c30
  goto loc_82291C30;
loc_82291C14:
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82291c28
  if (!cr6.eq) goto loc_82291C28;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82291C28:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,160(r31)
  guest::StoreU32(base, uint32_t(r31) + 160, uint32_t(r11));
loc_82291C30:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82291C34:
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

// sub_82291C48: lifted from generated/ by lift.py.
uint64_t f82291C48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82291C48 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r6,r11,8648
  r6 = uint64_t(int64_t(int64_t(r11) + 8648));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291C74, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // li r4,150
  r4 = uint64_t(int64_t(150));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // addi r6,r11,8636
  r6 = uint64_t(int64_t(int64_t(r11) + 8636));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // beq cr6,0x82291c98
  if (cr6.eq) goto loc_82291C98;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291C98:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291C9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // addi r6,r11,8588
  r6 = uint64_t(int64_t(int64_t(r11) + 8588));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // beq cr6,0x82291cc0
  if (cr6.eq) goto loc_82291CC0;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291CC0:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291CC4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // li r4,250
  r4 = uint64_t(int64_t(250));
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // addi r6,r11,8624
  r6 = uint64_t(int64_t(int64_t(r11) + 8624));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // beq cr6,0x82291ce8
  if (cr6.eq) goto loc_82291CE8;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291CE8:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291CEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // li r4,300
  r4 = uint64_t(int64_t(300));
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // addi r6,r11,8600
  r6 = uint64_t(int64_t(int64_t(r11) + 8600));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // beq cr6,0x82291d10
  if (cr6.eq) goto loc_82291D10;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291D10:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291D14, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82291D28: lifted from generated/ by lift.py.
uint64_t f82291D28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82291D28 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r6,r11,8912
  r6 = uint64_t(int64_t(int64_t(r11) + 8912));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291D54, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,8908
  r11 = uint64_t(int64_t(int64_t(r11) + 8908));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // addi r10,r10,8904
  r10 = uint64_t(int64_t(int64_t(r10) + 8904));
  // addi r9,r9,8888
  r9 = uint64_t(int64_t(int64_t(r9) + 8888));
  // lis r8,-32255
  r8 = uint64_t(int64_t(-2113863680));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // lis r7,-32255
  r7 = uint64_t(int64_t(-2113863680));
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // lis r6,-32255
  r6 = uint64_t(int64_t(-2113863680));
  // lis r5,-32255
  r5 = uint64_t(int64_t(-2113863680));
  // stw r10,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r10));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r9,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r9));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,-32255
  r4 = uint64_t(int64_t(-2113863680));
  // addi r7,r7,8848
  r7 = uint64_t(int64_t(int64_t(r7) + 8848));
  // addi r6,r6,8820
  r6 = uint64_t(int64_t(int64_t(r6) + 8820));
  // addi r5,r5,8808
  r5 = uint64_t(int64_t(int64_t(r5) + 8808));
  // lwz r11,29400(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29400);
  // addi r4,r4,8796
  r4 = uint64_t(int64_t(int64_t(r4) + 8796));
  // addi r8,r8,8872
  r8 = uint64_t(int64_t(int64_t(r8) + 8872));
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stw r7,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r7));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r6,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r6));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stw r5,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r5));
  // stw r4,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r4));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // stw r8,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r8));
  // li r4,150
  r4 = uint64_t(int64_t(150));
  // addi r6,r11,8776
  r6 = uint64_t(int64_t(int64_t(r11) + 8776));
  // lwzx r7,r10,r9
  r7 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // beq cr6,0x82291dec
  if (cr6.eq) goto loc_82291DEC;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291DEC:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291DF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lwz r8,160(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 160);
  // addi r11,r11,8768
  r11 = uint64_t(int64_t(int64_t(r11) + 8768));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // addi r10,r10,8756
  r10 = uint64_t(int64_t(int64_t(r10) + 8756));
  // addi r9,r9,8748
  r9 = uint64_t(int64_t(int64_t(r9) + 8748));
  // cmplwi cr6,r8,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 1, so);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // stw r9,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r9));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lwz r11,29404(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29404);
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,8728
  r6 = uint64_t(int64_t(int64_t(r11) + 8728));
  // lwzx r7,r10,r9
  r7 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // beq cr6,0x82291e4c
  if (cr6.eq) goto loc_82291E4C;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291E4C:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291E50, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lwz r9,160(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 160);
  // addi r11,r11,8724
  r11 = uint64_t(int64_t(int64_t(r11) + 8724));
  // addi r10,r10,-19188
  r10 = uint64_t(int64_t(int64_t(r10) + -19188));
  // cmplwi cr6,r9,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 2, so);
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r4,250
  r4 = uint64_t(int64_t(250));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lwz r11,29408(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29408);
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,8708
  r6 = uint64_t(int64_t(int64_t(r11) + 8708));
  // lwzx r7,r10,r9
  r7 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // beq cr6,0x82291ea0
  if (cr6.eq) goto loc_82291EA0;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291EA0:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291EA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,300
  r4 = uint64_t(int64_t(300));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lwz r11,29412(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29412);
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,8688
  r6 = uint64_t(int64_t(int64_t(r11) + 8688));
  // lwzx r7,r10,r9
  r7 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // beq cr6,0x82291edc
  if (cr6.eq) goto loc_82291EDC;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291EDC:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // li r4,350
  r4 = uint64_t(int64_t(350));
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // addi r6,r11,8676
  r6 = uint64_t(int64_t(int64_t(r11) + 8676));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // beq cr6,0x82291f04
  if (cr6.eq) goto loc_82291F04;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82291F04:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291F08, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82291F20: lifted from generated/ by lift.py.
uint64_t f82291F20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82291F20 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // lwzx r11,r3,r11
  r11 = guest::LoadU32(base, uint32_t(r3) + uint32_t(r11));
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x82291fc4
  if (!cr6.eq) goto loc_82291FC4;
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r9,3600
  r9 = uint64_t(int64_t(3600));
  // ori r10,r11,52540
  r10 = r11 | 52540;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r8,60
  r8 = uint64_t(int64_t(60));
  // addi r6,r11,9076
  r6 = uint64_t(int64_t(int64_t(r11) + 9076));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwzx r11,r3,r10
  r11 = guest::LoadU32(base, uint32_t(r3) + uint32_t(r10));
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // divwu r31,r11,r9
  r31 = uint32_t(uint32_t(r9) ? uint32_t(r11) / uint32_t(r9) : 0);
  // mulli r10,r31,3600
  r10 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(3600))));
  // subf r9,r10,r11
  r9 = r11 - r10;
  // mulli r10,r31,60
  r10 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(60))));
  // divwu r30,r9,r8
  r30 = uint32_t(uint32_t(r8) ? uint32_t(r9) / uint32_t(r8) : 0);
  // add r10,r10,r30
  r10 = r10 + r30;
  // mulli r10,r10,60
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(60))));
  // subf r29,r10,r11
  r29 = r11 - r10;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291F8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r9,r29
  r9 = r29;
  // mr r8,r30
  r8 = r30;
  // addi r6,r11,9060
  r6 = uint64_t(int64_t(int64_t(r11) + 9060));
  // mr r7,r31
  r7 = r31;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,300
  r4 = uint64_t(int64_t(300));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82291FB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,9032
  r6 = uint64_t(int64_t(int64_t(r11) + 9032));
  // li r4,150
  r4 = uint64_t(int64_t(150));
  // b 0x82292010
  goto loc_82292010;
loc_82291FC4:
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82292000
  if (cr6.lt) goto loc_82292000;
  // beq cr6,0x82291ff4
  if (cr6.eq) goto loc_82291FF4;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82291fe8
  if (cr6.lt) goto loc_82291FE8;
  // bne cr6,0x82292018
  if (!cr6.eq) goto loc_82292018;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,9012
  r6 = uint64_t(int64_t(int64_t(r11) + 9012));
  // b 0x82292008
  goto loc_82292008;
loc_82291FE8:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,8992
  r6 = uint64_t(int64_t(int64_t(r11) + 8992));
  // b 0x82292008
  goto loc_82292008;
loc_82291FF4:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,8972
  r6 = uint64_t(int64_t(int64_t(r11) + 8972));
  // b 0x82292008
  goto loc_82292008;
loc_82292000:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,8948
  r6 = uint64_t(int64_t(int64_t(r11) + 8948));
loc_82292008:
  // li r4,300
  r4 = uint64_t(int64_t(300));
  // li r5,2
  r5 = uint64_t(int64_t(2));
loc_82292010:
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292018, r31)); r3 = o.r3; f1 = o.f1; }
loc_82292018:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82292020: lifted from generated/ by lift.py.
uint64_t f82292020(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82292020 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r24,r3
  r24 = r3;
  // addi r6,r11,9204
  r6 = uint64_t(int64_t(int64_t(r11) + 9204));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,650
  r4 = uint64_t(int64_t(650));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292048, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // bne cr6,0x8229207c
  if (!cr6.eq) goto loc_8229207C;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // addi r6,r11,6044
  r6 = uint64_t(int64_t(int64_t(r11) + 6044));
  // li r4,300
  r4 = uint64_t(int64_t(300));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292074, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x82292328
  goto loc_82292328;
loc_8229207C:
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x82292080);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82292090
  if (!cr0.lt) goto loc_82292090;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // b 0x82292328
  goto loc_82292328;
loc_82292090:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,29428(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29428);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x822920b4
  if (cr6.eq) goto loc_822920B4;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x822920cc
  if (!cr6.eq) goto loc_822920CC;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,9180
  r6 = uint64_t(int64_t(int64_t(r11) + 9180));
  // b 0x822920bc
  goto loc_822920BC;
loc_822920B4:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,9156
  r6 = uint64_t(int64_t(int64_t(r11) + 9156));
loc_822920BC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822920CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_822920CC:
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
  // lwz r9,27348(r28)
  r9 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // lwz r11,4(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 4);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x82292104
  if (!cr0.eq) goto loc_82292104;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r6,r11,9132
  r6 = uint64_t(int64_t(int64_t(r11) + 9132));
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822920FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // b 0x82292328
  goto loc_82292328;
loc_82292104:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82292154
  if (cr6.eq) goto loc_82292154;
  // ld r7,168(r24)
  r7 = guest::LoadU64(base, uint32_t(r24) + 168);
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_82292118:
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r10
  r11 = r11 + r10;
  // ld r6,0(r11)
  r6 = guest::LoadU64(base, uint32_t(r11) + 0);
  // cmpld cr6,r7,r6
  cr6 = lifted::Compare<uint64_t>(r7, r6, so);
  // bne cr6,0x8229213c
  if (!cr6.eq) goto loc_8229213C;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // ld r11,16(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 16);
  // std r11,208(r24)
  guest::StoreU64(base, uint32_t(r24) + 208, r11);
  // lwz r9,27348(r28)
  r9 = guest::LoadU32(base, uint32_t(r28) + 27348);
loc_8229213C:
  // lwz r11,4(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 4);
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // lwz r6,8(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplw cr6,r8,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r6), so);
  // blt cr6,0x82292118
  if (cr6.lt) goto loc_82292118;
loc_82292154:
  // lwz r11,4(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 4);
  // li r27,-1
  r27 = uint64_t(int64_t(-1));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // ble cr6,0x82292324
  if (!cr6.gt) goto loc_82292324;
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // addi r26,r10,-24512
  r26 = uint64_t(int64_t(int64_t(r10) + -24512));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // addi r29,r10,-19400
  r29 = uint64_t(int64_t(int64_t(r10) + -19400));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // addi r25,r10,9092
  r25 = uint64_t(int64_t(int64_t(r10) + 9092));
loc_82292180:
  // cmpwi cr6,r27,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), -1, so);
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // bne cr6,0x822921a0
  if (!cr6.eq) goto loc_822921A0;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r6,r25
  r6 = r25;
  // li r4,120
  r4 = uint64_t(int64_t(120));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8229219C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8229230c
  goto loc_8229230C;
loc_822921A0:
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mulli r30,r27,48
  r30 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(48))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // ld r10,168(r24)
  r10 = guest::LoadU64(base, uint32_t(r24) + 168);
  // mr r6,r29
  r6 = r29;
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // lwz r7,8(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpld cr6,r10,r9
  cr6 = lifted::Compare<uint64_t>(r10, r9, so);
  // mulli r10,r27,20
  r10 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(20))));
  // addi r31,r10,150
  r31 = uint64_t(int64_t(int64_t(r10) + 150));
  // mr r4,r31
  r4 = r31;
  // bne cr6,0x822922b0
  if (!cr6.eq) goto loc_822922B0;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822921D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r6,r26
  r6 = r26;
  // mr r4,r31
  r4 = r31;
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r7,r11,24
  r7 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292200, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r6,r29
  r6 = r29;
  // mr r4,r31
  r4 = r31;
  // li r3,600
  r3 = uint64_t(int64_t(600));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // ld r7,16(r11)
  r7 = guest::LoadU64(base, uint32_t(r11) + 16);
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8229222C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r6,r29
  r6 = r29;
  // mr r4,r31
  r4 = r31;
  // li r3,700
  r3 = uint64_t(int64_t(700));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // lwz r7,40(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 40);
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292258, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r6,r29
  r6 = r29;
  // mr r4,r31
  r4 = r31;
  // li r3,750
  r3 = uint64_t(int64_t(750));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // lwz r7,64(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 64);
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292284, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r6,r29
  r6 = r29;
  // mr r4,r31
  r4 = r31;
  // li r3,800
  r3 = uint64_t(int64_t(800));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r7,40(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 40);
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822922AC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8229230c
  goto loc_8229230C;
loc_822922B0:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822922B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r6,r26
  r6 = r26;
  // mr r4,r31
  r4 = r31;
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r7,r11,24
  r7 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822922E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r6,r29
  r6 = r29;
  // mr r4,r31
  r4 = r31;
  // li r3,600
  r3 = uint64_t(int64_t(600));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // ld r7,16(r11)
  r7 = guest::LoadU64(base, uint32_t(r11) + 16);
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8229230C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8229230C:
  // lwz r11,27348(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27348);
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpw cr6,r27,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r10), so);
  // blt cr6,0x82292180
  if (cr6.lt) goto loc_82292180;
loc_82292324:
  // li r11,2
  r11 = uint64_t(int64_t(2));
loc_82292328:
  // stw r11,180(r24)
  guest::StoreU32(base, uint32_t(r24) + 180, uint32_t(r11));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
}

// sub_82292828: lifted from generated/ by lift.py.
uint64_t f82292828(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82292828 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r6,r11,9296
  r6 = uint64_t(int64_t(int64_t(r11) + 9296));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292854, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,9256
  r6 = uint64_t(int64_t(int64_t(r11) + 9256));
  // li r4,630
  r4 = uint64_t(int64_t(630));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8229286C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,88(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82292898
  if (cr6.eq) goto loc_82292898;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r6,r11,9244
  r6 = uint64_t(int64_t(int64_t(r11) + 9244));
  // li r4,400
  r4 = uint64_t(int64_t(400));
  // li r3,600
  r3 = uint64_t(int64_t(600));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292898, r31)); r3 = o.r3; f1 = o.f1; }
loc_82292898:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8229291c
  if (cr0.eq) goto loc_8229291C;
  // cmpwi cr6,r11,4109
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4109, so);
  // beq cr6,0x82292910
  if (cr6.eq) goto loc_82292910;
  // cmpwi cr6,r11,4110
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4110, so);
  // beq cr6,0x82292904
  if (cr6.eq) goto loc_82292904;
  // cmpwi cr6,r11,4111
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4111, so);
  // beq cr6,0x822928f8
  if (cr6.eq) goto loc_822928F8;
  // cmpwi cr6,r11,4112
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4112, so);
  // beq cr6,0x822928ec
  if (cr6.eq) goto loc_822928EC;
  // cmpwi cr6,r11,4113
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4113, so);
  // bne cr6,0x82292924
  if (!cr6.eq) goto loc_82292924;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,8588
  r6 = uint64_t(int64_t(int64_t(r11) + 8588));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822928E8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82292924
  goto loc_82292924;
loc_822928EC:
  // mr r3,r31
  r3 = r31;
  // bl 0x82292020
  // (left out: dead in C++)
  r3 = f82292020(base, r1, r13, r3);
  // b 0x82292924
  goto loc_82292924;
loc_822928F8:
  // mr r3,r31
  r3 = r31;
  // bl 0x82291f20
  // (left out: dead in C++)
  r3 = f82291F20(base, r1, r13, r3);
  // b 0x82292924
  goto loc_82292924;
loc_82292904:
  // mr r3,r31
  r3 = r31;
  // bl 0x82291d28
  // (left out: dead in C++)
  r3 = f82291D28(base, r1, r13, r3);
  // b 0x82292924
  goto loc_82292924;
loc_82292910:
  // mr r3,r31
  r3 = r31;
  // bl 0x82291c48
  // (left out: dead in C++)
  r3 = f82291C48(base, r1, r13, r3);
  // b 0x82292924
  goto loc_82292924;
loc_8229291C:
  // mr r3,r31
  r3 = r31;
  // bl 0x822916d0
  // (left out: dead in C++)
  r3 = f822916D0(base, r1, r13, r3);
loc_82292924:
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

// sub_82292938: lifted from generated/ by lift.py.
lifted::Result f82292938(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82292938 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x822929b8
  if (cr6.eq) goto loc_822929B8;
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82292988, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822929b8
  if (cr0.eq) goto loc_822929B8;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,31072(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 31072);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822929b8
  if (!cr6.eq) goto loc_822929B8;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,31076(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 31076);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x822929b8
  if (!cr6.eq) goto loc_822929B8;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,31076(r11)
  guest::StoreU32(base, uint32_t(r11) + 31076, uint32_t(r10));
loc_822929B8:
  // li r3,4113
  r3 = uint64_t(int64_t(4113));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82291520
  // (left out: dead in C++)
  { const lifted::Result o = f82291520(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,160(r31)
  guest::StoreU32(base, uint32_t(r31) + 160, uint32_t(r11));
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

lifted::Result e8222E368(uint8_t* base, const lifted::Args& a) {
  return f8222E368(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8222E5E8(uint8_t* base, const lifted::Args& a) {
  return f8222E5E8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e8222ED98(uint8_t* base, const lifted::Args& a) {
  return f8222ED98(base, a.r1, a.r13, a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e8222FC90(uint8_t* base, const lifted::Args& a) {
  f8222FC90(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8222FCA0(uint8_t* base, const lifted::Args& a) {
  return f8222FCA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82230B50(uint8_t* base, const lifted::Args& a) {
  return f82230B50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82231DF0(uint8_t* base, const lifted::Args& a) {
  return {f82231DF0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82231EF0(uint8_t* base, const lifted::Args& a) {
  return f82231EF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82231F58(uint8_t* base, const lifted::Args& a) {
  f82231F58(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82231F70(uint8_t* base, const lifted::Args& a) {
  return {f82231F70(base, a.r[0]), a.f[0]};
}

lifted::Result e82232040(uint8_t* base, const lifted::Args& a) {
  f82232040(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82232058(uint8_t* base, const lifted::Args& a) {
  f82232058(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822321B8(uint8_t* base, const lifted::Args& a) {
  f822321B8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822324E8(uint8_t* base, const lifted::Args& a) {
  return f822324E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82233460(uint8_t* base, const lifted::Args& a) {
  return f82233460(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.lr);
}

lifted::Result e82233948(uint8_t* base, const lifted::Args& a) {
  return f82233948(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822339B0(uint8_t* base, const lifted::Args& a) {
  f822339B0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82234928(uint8_t* base, const lifted::Args& a) {
  return {f82234928(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e82235D60(uint8_t* base, const lifted::Args& a) {
  return f82235D60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82235F10(uint8_t* base, const lifted::Args& a) {
  f82235F10(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82236608(uint8_t* base, const lifted::Args& a) {
  f82236608(base, a.r1, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8223A2F8(uint8_t* base, const lifted::Args& a) {
  return f8223A2F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8223A3F0(uint8_t* base, const lifted::Args& a) {
  return f8223A3F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82241968(uint8_t* base, const lifted::Args& a) {
  return f82241968(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822419E0(uint8_t* base, const lifted::Args& a) {
  return {f822419E0(base, a.r[0]), a.f[0]};
}

lifted::Result e82241AB0(uint8_t* base, const lifted::Args& a) {
  f82241AB0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82241AC8(uint8_t* base, const lifted::Args& a) {
  f82241AC8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82241CE8(uint8_t* base, const lifted::Args& a) {
  f82241CE8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822420C0(uint8_t* base, const lifted::Args& a) {
  return f822420C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82243EE0(uint8_t* base, const lifted::Args& a) {
  return f82243EE0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82243F38(uint8_t* base, const lifted::Args& a) {
  f82243F38(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82243F48(uint8_t* base, const lifted::Args& a) {
  f82243F48(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82246228(uint8_t* base, const lifted::Args& a) {
  return f82246228(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e822463C0(uint8_t* base, const lifted::Args& a) {
  f822463C0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82246A40(uint8_t* base, const lifted::Args& a) {
  f82246A40(base, a.r1, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82247B28(uint8_t* base, const lifted::Args& a) {
  return f82247B28(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82248E60(uint8_t* base, const lifted::Args& a) {
  return f82248E60(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82249198(uint8_t* base, const lifted::Args& a) {
  return f82249198(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e822491F8(uint8_t* base, const lifted::Args& a) {
  return f822491F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e822493D8(uint8_t* base, const lifted::Args& a) {
  return f822493D8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82249458(uint8_t* base, const lifted::Args& a) {
  return f82249458(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e822494A8(uint8_t* base, const lifted::Args& a) {
  return f822494A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e82249510(uint8_t* base, const lifted::Args& a) {
  return {f82249510(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82249610(uint8_t* base, const lifted::Args& a) {
  f82249610(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8224A368(uint8_t* base, const lifted::Args& a) {
  return {f8224A368(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8224A850(uint8_t* base, const lifted::Args& a) {
  return f8224A850(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8224B138(uint8_t* base, const lifted::Args& a) {
  return f8224B138(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8224B1C8(uint8_t* base, const lifted::Args& a) {
  return f8224B1C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8224B6E0(uint8_t* base, const lifted::Args& a) {
  return {f8224B6E0(base, a.r[0]), a.f[0]};
}

lifted::Result e8224B700(uint8_t* base, const lifted::Args& a) {
  return {f8224B700(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8224B888(uint8_t* base, const lifted::Args& a) {
  return f8224B888(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8224C4D8(uint8_t* base, const lifted::Args& a) {
  return f8224C4D8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e8224C9D8(uint8_t* base, const lifted::Args& a) {
  return f8224C9D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8224CC00(uint8_t* base, const lifted::Args& a) {
  return f8224CC00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8224CC78(uint8_t* base, const lifted::Args& a) {
  return f8224CC78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8224D090(uint8_t* base, const lifted::Args& a) {
  return f8224D090(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8224D230(uint8_t* base, const lifted::Args& a) {
  return {f8224D230(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8224DBA0(uint8_t* base, const lifted::Args& a) {
  return f8224DBA0(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e8224DC50(uint8_t* base, const lifted::Args& a) {
  return f8224DC50(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e8224DF90(uint8_t* base, const lifted::Args& a) {
  return f8224DF90(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e8224DFF0(uint8_t* base, const lifted::Args& a) {
  return f8224DFF0(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e8224E168(uint8_t* base, const lifted::Args& a) {
  return {f8224E168(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8224E1A8(uint8_t* base, const lifted::Args& a) {
  return {f8224E1A8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8224E208(uint8_t* base, const lifted::Args& a) {
  return {f8224E208(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8224E248(uint8_t* base, const lifted::Args& a) {
  return {f8224E248(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8224E7E8(uint8_t* base, const lifted::Args& a) {
  return f8224E7E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8224EB68(uint8_t* base, const lifted::Args& a) {
  return f8224EB68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8224EBF0(uint8_t* base, const lifted::Args& a) {
  return f8224EBF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8224EC50(uint8_t* base, const lifted::Args& a) {
  return f8224EC50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8224ECC0(uint8_t* base, const lifted::Args& a) {
  return {f8224ECC0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8224ED38(uint8_t* base, const lifted::Args& a) {
  return f8224ED38(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e8224EDA8(uint8_t* base, const lifted::Args& a) {
  return f8224EDA8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8224EDC0(uint8_t* base, const lifted::Args& a) {
  return f8224EDC0(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e8224EE28(uint8_t* base, const lifted::Args& a) {
  return {f8224EE28(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8224EE68(uint8_t* base, const lifted::Args& a) {
  return {f8224EE68(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8224F158(uint8_t* base, const lifted::Args& a) {
  return f8224F158(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e82260800(uint8_t* base, const lifted::Args& a) {
  return f82260800(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8228F7D0(uint8_t* base, const lifted::Args& a) {
  return f8228F7D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e8228FA20(uint8_t* base, const lifted::Args& a) {
  return f8228FA20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e8228FA58(uint8_t* base, const lifted::Args& a) {
  return f8228FA58(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e8228FD50(uint8_t* base, const lifted::Args& a) {
  return f8228FD50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e8228FF88(uint8_t* base, const lifted::Args& a) {
  return f8228FF88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e82290640(uint8_t* base, const lifted::Args& a) {
  return f82290640(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e82291408(uint8_t* base, const lifted::Args& a) {
  return f82291408(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82291520(uint8_t* base, const lifted::Args& a) {
  return f82291520(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e822916D0(uint8_t* base, const lifted::Args& a) {
  return {f822916D0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82291760(uint8_t* base, const lifted::Args& a) {
  return f82291760(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822917B8(uint8_t* base, const lifted::Args& a) {
  return f822917B8(base, a.r1, a.r13, a.r[0], a.r[5]);
}

lifted::Result e82291858(uint8_t* base, const lifted::Args& a) {
  return f82291858(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e82291920(uint8_t* base, const lifted::Args& a) {
  return f82291920(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822919A0(uint8_t* base, const lifted::Args& a) {
  return f822919A0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82291A40(uint8_t* base, const lifted::Args& a) {
  return f82291A40(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82291AD0(uint8_t* base, const lifted::Args& a) {
  return f82291AD0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82291B50(uint8_t* base, const lifted::Args& a) {
  return f82291B50(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82291C48(uint8_t* base, const lifted::Args& a) {
  return {f82291C48(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82291D28(uint8_t* base, const lifted::Args& a) {
  return {f82291D28(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82291F20(uint8_t* base, const lifted::Args& a) {
  return {f82291F20(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82292020(uint8_t* base, const lifted::Args& a) {
  return {f82292020(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82292828(uint8_t* base, const lifted::Args& a) {
  return {f82292828(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82292938(uint8_t* base, const lifted::Args& a) {
  return f82292938(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x8222E368, e8222E368},
    {0x8222E5E8, e8222E5E8},
    {0x8222ED98, e8222ED98},
    {0x8222FC90, e8222FC90},
    {0x8222FCA0, e8222FCA0},
    {0x82230B50, e82230B50},
    {0x82231DF0, e82231DF0},
    {0x82231EF0, e82231EF0},
    {0x82231F58, e82231F58},
    {0x82231F70, e82231F70},
    {0x82232040, e82232040},
    {0x82232058, e82232058},
    {0x822321B8, e822321B8},
    {0x822324E8, e822324E8},
    {0x82233460, e82233460},
    {0x82233948, e82233948},
    {0x822339B0, e822339B0},
    {0x82234928, e82234928},
    {0x82235D60, e82235D60},
    {0x82235F10, e82235F10},
    {0x82236608, e82236608},
    {0x8223A2F8, e8223A2F8},
    {0x8223A3F0, e8223A3F0},
    {0x82241968, e82241968},
    {0x822419E0, e822419E0},
    {0x82241AB0, e82241AB0},
    {0x82241AC8, e82241AC8},
    {0x82241CE8, e82241CE8},
    {0x822420C0, e822420C0},
    {0x82243EE0, e82243EE0},
    {0x82243F38, e82243F38},
    {0x82243F48, e82243F48},
    {0x82246228, e82246228},
    {0x822463C0, e822463C0},
    {0x82246A40, e82246A40},
    {0x82247B28, e82247B28},
    {0x82248E60, e82248E60},
    {0x82249198, e82249198},
    {0x822491F8, e822491F8},
    {0x822493D8, e822493D8},
    {0x82249458, e82249458},
    {0x822494A8, e822494A8},
    {0x82249510, e82249510},
    {0x82249610, e82249610},
    {0x8224A368, e8224A368},
    {0x8224A850, e8224A850},
    {0x8224B138, e8224B138},
    {0x8224B1C8, e8224B1C8},
    {0x8224B6E0, e8224B6E0},
    {0x8224B700, e8224B700},
    {0x8224B888, e8224B888},
    {0x8224C4D8, e8224C4D8},
    {0x8224C9D8, e8224C9D8},
    {0x8224CC00, e8224CC00},
    {0x8224CC78, e8224CC78},
    {0x8224D090, e8224D090},
    {0x8224D230, e8224D230},
    {0x8224DBA0, e8224DBA0},
    {0x8224DC50, e8224DC50},
    {0x8224DF90, e8224DF90},
    {0x8224DFF0, e8224DFF0},
    {0x8224E168, e8224E168},
    {0x8224E1A8, e8224E1A8},
    {0x8224E208, e8224E208},
    {0x8224E248, e8224E248},
    {0x8224E7E8, e8224E7E8},
    {0x8224EB68, e8224EB68},
    {0x8224EBF0, e8224EBF0},
    {0x8224EC50, e8224EC50},
    {0x8224ECC0, e8224ECC0},
    {0x8224ED38, e8224ED38},
    {0x8224EDA8, e8224EDA8},
    {0x8224EDC0, e8224EDC0},
    {0x8224EE28, e8224EE28},
    {0x8224EE68, e8224EE68},
    {0x8224F158, e8224F158},
    {0x82260800, e82260800},
    {0x8228F7D0, e8228F7D0},
    {0x8228FA20, e8228FA20},
    {0x8228FA58, e8228FA58},
    {0x8228FD50, e8228FD50},
    {0x8228FF88, e8228FF88},
    {0x82290640, e82290640},
    {0x82291408, e82291408},
    {0x82291520, e82291520},
    {0x822916D0, e822916D0},
    {0x82291760, e82291760},
    {0x822917B8, e822917B8},
    {0x82291858, e82291858},
    {0x82291920, e82291920},
    {0x822919A0, e822919A0},
    {0x82291A40, e82291A40},
    {0x82291AD0, e82291AD0},
    {0x82291B50, e82291B50},
    {0x82291C48, e82291C48},
    {0x82291D28, e82291D28},
    {0x82291F20, e82291F20},
    {0x82292020, e82292020},
    {0x82292828, e82292828},
    {0x82292938, e82292938},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_013)), true);

}  // namespace

}  // namespace lifted
