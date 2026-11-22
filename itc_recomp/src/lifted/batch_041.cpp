// Lift sweep batch 041 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/c_runtime.h"
#include "schema/c_runtime_core.h"
#include "schema/car_sounds.h"
#include "schema/command_buffer.h"
#include "schema/device_state.h"
#include "schema/graphics_resources.h"
#include "schema/heap.h"
#include "schema/menu_helpers.h"
#include "schema/model_animation.h"
#include "schema/movies.h"
#include "schema/network_mode.h"
#include "schema/options_screen.h"
#include "schema/screen_objects.h"
#include "schema/second_set.h"
#include "schema/std_string.h"
#include "schema/ui_movies_game.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_041, true, "ITC",
              "Run lift sweep batch 041's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82286640: lifted from generated/ by lift.py.
uint64_t f82286640(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82286640 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r10));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x822866b0
  if (cr6.eq) goto loc_822866B0;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822866B0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // b 0x82282f10
  r3 = f82282F10(base, r1, r13);
  return r3;
}

// sub_822866C8: lifted from generated/ by lift.py.
lifted::Result f822866C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822866C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822866F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822867a8
  if (!cr6.eq) goto loc_822867A8;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x82286710, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,134
  r4 = uint64_t(int64_t(134));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228672C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286748, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286750, r31)); r3 = o.r3; f1 = o.f1; }
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
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228676C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286774, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // lwz r3,92(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 92);
  // bl 0x82285348
  // (left out: dead in C++)
  r3 = f82285348(base, r1, r13, r3);
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
  r3 = guest::LoadU32(base, uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x822867A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_822867A8:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822867D0, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82286894
  if (cr0.eq) goto loc_82286894;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x82286894
  if (cr6.eq) goto loc_82286894;
  // bl 0x822843b8
  // (left out: dead in C++)
  { const lifted::Result o = f822843B8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286804, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8228680C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,137
  r4 = uint64_t(int64_t(137));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286828, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286830, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228684C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286854, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,96
  r4 = uint64_t(int64_t(96));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286870, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286878, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82284668
  // (left out: dead in C++)
  { const lifted::Result o = f82284668(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8228688C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82286898
  goto loc_82286898;
loc_82286894:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82286898:
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

// sub_822868B0: lifted from generated/ by lift.py.
lifted::Result f822868B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822868B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822868E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82286980
  if (!cr6.eq) goto loc_82286980;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x822868F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,134
  r4 = uint64_t(int64_t(134));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286914, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286930, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286938, r31)); r3 = o.r3; f1 = o.f1; }
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
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286954, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8228695C, r31)); r3 = o.r3; f1 = o.f1; }
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
  r3 = guest::LoadU32(base, uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x82286978, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82286980:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822869A8, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82286a6c
  if (cr0.eq) goto loc_82286A6C;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x82286a6c
  if (cr6.eq) goto loc_82286A6C;
  // bl 0x822843b8
  // (left out: dead in C++)
  { const lifted::Result o = f822843B8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822869DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x822869E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,137
  r4 = uint64_t(int64_t(137));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286A00, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286A08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286A24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286A2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,96
  r4 = uint64_t(int64_t(96));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286A48, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286A50, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82284668
  // (left out: dead in C++)
  { const lifted::Result o = f82284668(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82286A64, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82286a70
  goto loc_82286A70;
loc_82286A6C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82286A70:
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

// sub_82286A88: lifted from generated/ by lift.py.
lifted::Result f82286A88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82286A88 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r31,25040(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82286AB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82286ce4
  if (!cr6.eq) goto loc_82286CE4;
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82286AD4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,70
  r4 = uint64_t(int64_t(70));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82286AE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,82
  r4 = uint64_t(int64_t(82));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82286AEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,69
  r4 = uint64_t(int64_t(69));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82286AF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 96);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // stw r11,100(r31)
  guest::StoreU32(base, uint32_t(r31) + 100, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25032);
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x82286b44
  if (!cr0.eq) goto loc_82286B44;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // bne cr6,0x82286b44
  if (!cr6.eq) goto loc_82286B44;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r3,200
  r3 = uint64_t(int64_t(200));
  // stw r11,92(r31)
  guest::StoreU32(base, uint32_t(r31) + 92, uint32_t(r11));
  // stw r10,96(r31)
  guest::StoreU32(base, uint32_t(r31) + 96, uint32_t(r10));
  // stw r29,100(r31)
  guest::StoreU32(base, uint32_t(r31) + 100, uint32_t(r29));
  // b 0x82286b6c
  goto loc_82286B6C;
loc_82286B44:
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // bne cr6,0x82286b74
  if (!cr6.eq) goto loc_82286B74;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82286b74
  if (!cr6.eq) goto loc_82286B74;
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r29,92(r31)
  guest::StoreU32(base, uint32_t(r31) + 92, uint32_t(r29));
  // stw r29,96(r31)
  guest::StoreU32(base, uint32_t(r31) + 96, uint32_t(r29));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,100(r31)
  guest::StoreU32(base, uint32_t(r31) + 100, uint32_t(r11));
loc_82286B6C:
  // bl 0x822852d8
  // (left out: dead in C++)
  r3 = f822852D8(base, r1, r13, r3);
  // b 0x82286c68
  goto loc_82286C68;
loc_82286B74:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82286be4
  if (!cr6.eq) goto loc_82286BE4;
  // lwz r10,92(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 92);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x82286bac
  if (!cr6.eq) goto loc_82286BAC;
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // bne cr6,0x82286bac
  if (!cr6.eq) goto loc_82286BAC;
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r8,1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 1, so);
  // bne cr6,0x82286bac
  if (!cr6.eq) goto loc_82286BAC;
  // li r4,81
  r4 = uint64_t(int64_t(81));
loc_82286BA4:
  // stw r27,92(r31)
  guest::StoreU32(base, uint32_t(r31) + 92, uint32_t(r27));
  // b 0x82286c50
  goto loc_82286C50;
loc_82286BAC:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82286bd4
  if (!cr6.eq) goto loc_82286BD4;
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bne cr6,0x82286bd4
  if (!cr6.eq) goto loc_82286BD4;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82286bd4
  if (!cr6.eq) goto loc_82286BD4;
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // stw r29,92(r31)
  guest::StoreU32(base, uint32_t(r31) + 92, uint32_t(r29));
  // b 0x82286c50
  goto loc_82286C50;
loc_82286BD4:
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 96);
  // li r4,69
  r4 = uint64_t(int64_t(69));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x82286c4c
  goto loc_82286C4C;
loc_82286BE4:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82286c68
  if (!cr6.eq) goto loc_82286C68;
  // lwz r10,92(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 92);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82286c14
  if (!cr0.eq) goto loc_82286C14;
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r8,6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 6, so);
  // bne cr6,0x82286c14
  if (!cr6.eq) goto loc_82286C14;
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // bne cr6,0x82286c14
  if (!cr6.eq) goto loc_82286C14;
  // li r4,82
  r4 = uint64_t(int64_t(82));
  // b 0x82286ba4
  goto loc_82286BA4;
loc_82286C14:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82286c40
  if (!cr6.eq) goto loc_82286C40;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bne cr6,0x82286c40
  if (!cr6.eq) goto loc_82286C40;
  // cmpwi cr6,r9,6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 6, so);
  // bne cr6,0x82286c40
  if (!cr6.eq) goto loc_82286C40;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r4,82
  r4 = uint64_t(int64_t(82));
  // stw r11,92(r31)
  guest::StoreU32(base, uint32_t(r31) + 92, uint32_t(r11));
  // b 0x82286c50
  goto loc_82286C50;
loc_82286C40:
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 96);
  // li r4,70
  r4 = uint64_t(int64_t(70));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
loc_82286C4C:
  // stw r11,96(r31)
  guest::StoreU32(base, uint32_t(r31) + 96, uint32_t(r11));
loc_82286C50:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286C68, r31)); r3 = o.r3; f1 = o.f1; }
loc_82286C68:
  // bl 0x822843b8
  // (left out: dead in C++)
  { const lifted::Result o = f822843B8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // addi r28,r11,31768
  r28 = uint64_t(int64_t(int64_t(r11) + 31768));
loc_82286C74:
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // addi r5,r29,5
  r5 = uint64_t(int64_t(int64_t(r29) + 5));
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // add r11,r11,r29
  r11 = r11 + r29;
  // mr r3,r30
  r3 = r30;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r6,r11,r28
  r6 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r28));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82286C94, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 6, so);
  // blt cr6,0x82286c74
  if (cr6.lt) goto loc_82286C74;
  // lwz r11,100(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 100);
  // mr r3,r30
  r3 = r30;
  // addi r4,r11,59
  r4 = uint64_t(int64_t(int64_t(r11) + 59));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82286CB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 96);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r11,59
  r4 = uint64_t(int64_t(int64_t(r11) + 59));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286CD0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286CD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,92(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 92);
  // bl 0x82285348
  // (left out: dead in C++)
  r3 = f82285348(base, r1, r13, r3);
  // stw r27,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r27));
loc_82286CE4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82286CF0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82286D00: lifted from generated/ by lift.py.
lifted::Result f82286D00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82286D00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82286D30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82286db0
  if (!cr6.eq) goto loc_82286DB0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x822853e8
  // (left out: dead in C++)
  r3 = f822853E8(base, r1, r13, r3);
  // bl 0x82283468
  // (left out: dead in C++)
  { const lifted::Result o = f82283468(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a1e0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ResetNode(base, LIFTED_ARGS(0x82286D5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,135
  r4 = uint64_t(int64_t(135));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286D78, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,132
  r4 = uint64_t(int64_t(132));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82286D84, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,132
  r4 = uint64_t(int64_t(132));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286DA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286DA8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82286DB0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82286DBC, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x82286de8
  if (cr0.eq) goto loc_82286DE8;
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82286DD8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,34
  r5 = uint64_t(int64_t(34));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82286DE8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82286DE8:
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

// sub_82286E08: lifted from generated/ by lift.py.
lifted::Result f82286E08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82286E08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82286E30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82286ea4
  if (!cr6.eq) goto loc_82286EA4;
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82286E54, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,34
  r5 = uint64_t(int64_t(34));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82286E64, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // lwz r3,92(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 92);
  // bl 0x82285348
  // (left out: dead in C++)
  r3 = f82285348(base, r1, r13, r3);
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // lwz r11,96(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 96);
  // mulli r3,r11,40
  r3 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(40))));
  // bl 0x822852d8
  // (left out: dead in C++)
  r3 = f822852D8(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,136
  r4 = uint64_t(int64_t(136));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286E9C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82286EA4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82286EB0, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x82286fe4
  if (cr0.eq) goto loc_82286FE4;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x82286EC4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286EE8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286F04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286F0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,96
  r4 = uint64_t(int64_t(96));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286F28, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286F30, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286F4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286F54, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,137
  r4 = uint64_t(int64_t(137));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286F70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286F78, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,965
  r6 = uint64_t(int64_t(965));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82286F8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82286FA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82286FB0, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x822843b8
  // (left out: dead in C++)
  { const lifted::Result o = f822843B8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82284668
  // (left out: dead in C++)
  { const lifted::Result o = f82284668(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,96(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 96);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,59
  r4 = uint64_t(int64_t(int64_t(r11) + 59));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82286FDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82286FE4, r31)); r3 = o.r3; f1 = o.f1; }
loc_82286FE4:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82286FF0: lifted from generated/ by lift.py.
lifted::Result f82286FF0(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82286FF0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,6808
  r6 = uint64_t(int64_t(int64_t(r11) + 6808));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82287014, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // addi r29,r11,31800
  r29 = uint64_t(int64_t(int64_t(r11) + 31800));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r29
  r31 = r29;
  // addi r28,r11,-24512
  r28 = uint64_t(int64_t(int64_t(r11) + -24512));
loc_8228702C:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r7,0(r31)
  r7 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r6,r28
  r6 = r28;
  // mr r4,r30
  r4 = r30;
  // li r3,150
  r3 = uint64_t(int64_t(150));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82287044, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r11,r29,32
  r11 = uint64_t(int64_t(int64_t(r29) + 32));
  // addi r30,r30,15
  r30 = uint64_t(int64_t(int64_t(r30) + 15));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x8228702c
  if (cr6.lt) goto loc_8228702C;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r3,135
  r3 = uint64_t(int64_t(135));
  // lwz r11,25032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25032);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,-24508
  r6 = uint64_t(int64_t(int64_t(r11) + -24508));
  // mulli r11,r10,15
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(15))));
  // addi r4,r11,50
  r4 = uint64_t(int64_t(int64_t(r11) + 50));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82287080, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82287098, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // lis r10,-32216
  r10 = uint64_t(int64_t(-2111307776));
  // lis r21,-32216
  r21 = uint64_t(int64_t(-2111307776));
  // addi r10,r10,16984
  r10 = uint64_t(int64_t(int64_t(r10) + 16984));
  // lis r22,-32216
  r22 = uint64_t(int64_t(-2111307776));
  // lwz r11,25028(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25028);
  // lis r23,-32216
  r23 = uint64_t(int64_t(-2111307776));
  // lis r24,-32216
  r24 = uint64_t(int64_t(-2111307776));
  // lis r25,-32163
  r25 = uint64_t(int64_t(-2107834368));
  // lis r26,-32216
  r26 = uint64_t(int64_t(-2111307776));
  // stw r10,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r10));
  // lis r27,-32216
  r27 = uint64_t(int64_t(-2111307776));
  // lis r28,-32216
  r28 = uint64_t(int64_t(-2111307776));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).step_address());
  // lis r29,-32216
  r29 = uint64_t(int64_t(-2111307776));
  // lis r30,-32216
  r30 = uint64_t(int64_t(-2111307776));
  // lis r4,-32216
  r4 = uint64_t(int64_t(-2111307776));
  // lis r5,-32216
  r5 = uint64_t(int64_t(-2111307776));
  // lis r6,-32216
  r6 = uint64_t(int64_t(-2111307776));
  // lis r7,-32216
  r7 = uint64_t(int64_t(-2111307776));
  // lis r8,-32216
  r8 = uint64_t(int64_t(-2111307776));
  // lis r9,-32216
  r9 = uint64_t(int64_t(-2111307776));
  // addi r21,r21,26312
  r21 = uint64_t(int64_t(int64_t(r21) + 26312));
  // addi r22,r22,16824
  r22 = uint64_t(int64_t(int64_t(r22) + 16824));
  // addi r23,r23,27272
  r23 = uint64_t(int64_t(int64_t(r23) + 27272));
  // addi r24,r24,16648
  r24 = uint64_t(int64_t(int64_t(r24) + 16648));
  // addi r25,r25,-31856
  r25 = uint64_t(int64_t(int64_t(r25) + -31856));
  // addi r26,r26,27904
  r26 = uint64_t(int64_t(int64_t(r26) + 27904));
  // stw r21,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r21));
  // addi r27,r27,28168
  r27 = uint64_t(int64_t(int64_t(r27) + 28168));
  // stw r22,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r22));
  // addi r28,r28,18320
  r28 = uint64_t(int64_t(int64_t(r28) + 18320));
  // stw r23,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r23));
  // addi r29,r29,19640
  r29 = uint64_t(int64_t(int64_t(r29) + 19640));
  // stw r24,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r24));
  // addi r30,r30,19984
  r30 = uint64_t(int64_t(int64_t(r30) + 19984));
  // stw r25,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r25));
  // addi r4,r4,20248
  r4 = uint64_t(int64_t(int64_t(r4) + 20248));
  // stw r26,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r26));
  // addi r5,r5,20656
  r5 = uint64_t(int64_t(int64_t(r5) + 20656));
  // stw r27,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r27));
  // addi r6,r6,20904
  r6 = uint64_t(int64_t(int64_t(r6) + 20904));
  // stw r28,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r28));
  // addi r7,r7,18176
  r7 = uint64_t(int64_t(int64_t(r7) + 18176));
  // stw r29,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r29));
  // addi r8,r8,18848
  r8 = uint64_t(int64_t(int64_t(r8) + 18848));
  // stw r30,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r30));
  // addi r9,r9,26800
  r9 = uint64_t(int64_t(int64_t(r9) + 26800));
  // stw r4,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r4));
  // stw r5,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r5));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // stw r6,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r6));
  // stw r7,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r7));
  // stw r8,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r8));
  // stw r9,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r9));
  // beq cr6,0x82287204
  if (cr6.eq) goto loc_82287204;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x822871e0
  if (cr6.lt) goto loc_822871E0;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwzx r11,r10,r9
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8228719C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25028(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25028);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82287218
  if (cr0.eq) goto loc_82287218;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x822871c4
  if (!cr6.eq) goto loc_822871C4;
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address());
  // stw r8,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r8));
  // b 0x822871d4
  goto loc_822871D4;
loc_822871C4:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_822871C8:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address());
  // stw r8,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r8));
  // stw r10,20(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).unknown_20_address(), uint32_t(r10));
loc_822871D4:
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
loc_822871D8:
  // stw r10,24(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_started_address(), uint32_t(r10));
  // b 0x82287218
  goto loc_82287218;
loc_822871E0:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x822871c8
  if (!cr6.eq) goto loc_822871C8;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address());
  // stw r8,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // b 0x822871d8
  goto loc_822871D8;
loc_82287204:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x82287218
  if (cr6.eq) goto loc_82287218;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82287218, r31)); r3 = o.r3; f1 = o.f1; }
loc_82287218:
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82287220: lifted from generated/ by lift.py.
lifted::Result f82287220(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82287220 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // mr r29,r4
  r29 = r4;
  // lwz r11,28220(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28220);
  // lwz r3,23376(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 23376);
  // addi r4,r11,1088
  r4 = uint64_t(int64_t(int64_t(r11) + 1088));
  // bl 0x821a66a8
  // (left out: dead in C++)
  { const lifted::Result o = game::Replay::CopyFrom(base, LIFTED_ARGS(0x82287248, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23376(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 23376);
  // lwz r5,23372(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 23372);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r4,23340(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 23340);
  // bl 0x82105228
  // (left out: dead in C++)
  { const lifted::Result o = game::Replay::SetUpRace(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,9604(r29)
  guest::StoreU32(base, uint32_t(r29) + 9604, uint32_t(r11));
  // lwz r11,28220(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28220);
  // stw r10,244(r11)
  guest::StoreU32(base, uint32_t(r11) + 244, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // stw r11,28228(r10)
  guest::StoreU32(base, uint32_t(r10) + 28228, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,25032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25032);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r11,28232(r10)
  guest::StoreU32(base, uint32_t(r10) + 28232, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822872A8: lifted from generated/ by lift.py.
lifted::Result f822872A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_822872A8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r24,r4
  r24 = r4;
  // lis r25,-32216
  r25 = uint64_t(int64_t(-2111307776));
  // lis r26,-32240
  r26 = uint64_t(int64_t(-2112880640));
  // lis r27,-32216
  r27 = uint64_t(int64_t(-2111307776));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lis r28,-32216
  r28 = uint64_t(int64_t(-2111307776));
  // lis r29,-32216
  r29 = uint64_t(int64_t(-2111307776));
  // lis r30,-32240
  r30 = uint64_t(int64_t(-2112880640));
  // lis r31,-32216
  r31 = uint64_t(int64_t(-2111307776));
  // lis r4,-32216
  r4 = uint64_t(int64_t(-2111307776));
  // lis r5,-32216
  r5 = uint64_t(int64_t(-2111307776));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // lis r6,-32216
  r6 = uint64_t(int64_t(-2111307776));
  // lis r7,-32216
  r7 = uint64_t(int64_t(-2111307776));
  // lis r8,-32216
  r8 = uint64_t(int64_t(-2111307776));
  // lis r9,-32216
  r9 = uint64_t(int64_t(-2111307776));
  // lis r10,-32216
  r10 = uint64_t(int64_t(-2111307776));
  // addi r25,r25,23016
  r25 = uint64_t(int64_t(int64_t(r25) + 23016));
  // addi r26,r26,25920
  r26 = uint64_t(int64_t(int64_t(r26) + 25920));
  // addi r27,r27,8504
  r27 = uint64_t(int64_t(int64_t(r27) + 8504));
  // addi r28,r28,11128
  r28 = uint64_t(int64_t(int64_t(r28) + 11128));
  // addi r29,r29,24792
  r29 = uint64_t(int64_t(int64_t(r29) + 24792));
  // addi r30,r30,25920
  r30 = uint64_t(int64_t(int64_t(r30) + 25920));
  // stw r25,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r25));
  // addi r31,r31,10488
  r31 = uint64_t(int64_t(int64_t(r31) + 10488));
  // stw r26,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r26));
  // addi r4,r4,11512
  r4 = uint64_t(int64_t(int64_t(r4) + 11512));
  // stw r27,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r27));
  // addi r5,r5,23696
  r5 = uint64_t(int64_t(int64_t(r5) + 23696));
  // stw r28,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r28));
  // addi r6,r6,24008
  r6 = uint64_t(int64_t(int64_t(r6) + 24008));
  // stw r29,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r29));
  // addi r7,r7,24408
  r7 = uint64_t(int64_t(int64_t(r7) + 24408));
  // stw r30,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r30));
  // addi r8,r8,29216
  r8 = uint64_t(int64_t(int64_t(r8) + 29216));
  // stw r31,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r31));
  // addi r9,r9,26176
  r9 = uint64_t(int64_t(int64_t(r9) + 26176));
  // stw r4,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r4));
  // addi r10,r10,23448
  r10 = uint64_t(int64_t(int64_t(r10) + 23448));
  // stw r5,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r5));
  // stw r6,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r6));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // stw r7,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r7));
  // stw r8,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r8));
  // stw r9,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r9));
  // stw r10,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r10));
  // beq cr6,0x82287390
  if (cr6.eq) goto loc_82287390;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r3
  r4 = r3;
  // mr r3,r24
  r3 = r24;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82287390, r31)); r3 = o.r3; f1 = o.f1; }
loc_82287390:
  // bl 0x82293460
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82293460, LIFTED_ARGS(0x82287394, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82287CE0: lifted from generated/ by lift.py.
lifted::Result f82287CE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82287CE0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,9640(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9640);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82287d48
  if (!cr6.eq) goto loc_82287D48;
  // lwz r11,9644(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9644);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // blt cr6,0x82287d48
  if (cr6.lt) goto loc_82287D48;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82287D14, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25044(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25044);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82287d2c
  if (cr6.eq) goto loc_82287D2C;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82287D2C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_82287D48:
  // lwz r11,9644(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9644);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // bge cr6,0x82287d5c
  if (!cr6.lt) goto loc_82287D5C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,9644(r31)
  guest::StoreU32(base, uint32_t(r31) + 9644, uint32_t(r11));
loc_82287D5C:
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

// sub_82287D70: lifted from generated/ by lift.py.
lifted::Result f82287D70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82287D70 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9640);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82287dc4
  if (cr6.eq) goto loc_82287DC4;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82287D90, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25044(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25044);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82287da8
  if (cr6.eq) goto loc_82287DA8;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82287DA8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_82287DC4:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82287F98: lifted from generated/ by lift.py.
void f82287F98(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82287F98 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82287fdc
  if (cr6.eq) goto loc_82287FDC;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25044(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25044);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82287fbc
  if (cr0.eq) goto loc_82287FBC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82287FBC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r8));
  // blr
  return;
loc_82287FDC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_82287FE8: lifted from generated/ by lift.py.
lifted::Result f82287FE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82287FE8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,40
  r11 = uint64_t(int64_t(40));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8228800C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x82288014, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822880E8: lifted from generated/ by lift.py.
uint64_t f822880E8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_822880E8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,40
  r11 = uint64_t(int64_t(40));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8228810C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8228811C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82288138: lifted from generated/ by lift.py.
uint64_t f82288138(uint8_t* base, uint64_t r1, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82288138 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  // std r30,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  guest::StoreU64(base, uint32_t(r1) + -8, r31);
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r9,30
  r9 = uint64_t(int64_t(30));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // li r6,38
  r6 = uint64_t(int64_t(38));
  // lwz r3,604(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 604);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r3,r10
  r3 = r3 + r10;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r10));
  // divwu r30,r3,r31
  r30 = uint32_t(uint32_t(r31) ? uint32_t(r3) / uint32_t(r31) : 0);
  // mulli r30,r30,50
  r30 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(50))));
  // subf r10,r30,r3
  r10 = r3 - r30;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r9,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r8));
  // stw r7,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r7));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r10));
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r6));
  // stw r5,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r5));
  // stw r4,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r4));
  // ld r30,-16(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -16);
  // ld r31,-8(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -8);
  // blr
  return r3;
}

// sub_822881E0: lifted from generated/ by lift.py.
lifted::Result f822881E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822881E0 (lifted)");
  uint64_t r0 = 0, r3 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r4
  r30 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,40
  r11 = uint64_t(int64_t(40));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82288210, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822882b0
  if (!cr6.eq) goto loc_822882B0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25052(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25052);
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bgt cr6,0x8228828c
  if (cr6.gt) goto loc_8228828C;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6840
  r12 = uint64_t(int64_t(int64_t(r12) + 6840));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // lis r12,-32215
  r12 = uint64_t(int64_t(-2111242240));
  // addi r12,r12,-32168
  r12 = uint64_t(int64_t(int64_t(r12) + -32168));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_82288258;
  return {r3, f1};
  case 1:
  goto loc_82288260;
  case 2:
  goto loc_82288268;
  case 3:
  goto loc_frag_82288258;
  return {r3, f1};
  case 4:
  goto loc_82288270;
  case 5:
  goto loc_82288260;
  case 6:
  goto loc_82288268;
  case 7:
  goto loc_82288268;
  case 8:
  goto loc_82288278;
  default:
  std::abort();
  }
loc_82288260:
  // li r6,37
  r6 = uint64_t(int64_t(37));
  // b 0x8228827c
  goto loc_8228827C;
loc_82288268:
  // li r6,44
  r6 = uint64_t(int64_t(44));
  // b 0x8228827c
  goto loc_8228827C;
loc_82288270:
  // li r6,39
  r6 = uint64_t(int64_t(39));
  // b 0x8228827c
  goto loc_8228827C;
loc_82288278:
  // li r6,47
  r6 = uint64_t(int64_t(47));
loc_8228827C:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8228828C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8228828C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822882A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_822882B0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822882BC, r31)); r3 = o.r3; f1 = o.f1; }
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
  return {r3, f1};
loc_frag_82288258:
  // li r6,34
  r6 = uint64_t(int64_t(34));
  // b 0x8228827c
  goto loc_8228827C_82288258;
loc_8228827C_82288258:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8228828C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822882A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822882BC, r31)); r3 = o.r3; f1 = o.f1; }
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
  return {r3, f1};
}

// sub_822882E0: lifted from generated/ by lift.py.
lifted::Result f822882E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822882E0 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r4
  r31 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,40
  r11 = uint64_t(int64_t(40));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82288310, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82288350
  if (!cr6.eq) goto loc_82288350;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228833C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82288348, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_82288350:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8228835C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82288378: lifted from generated/ by lift.py.
lifted::Result f82288378(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82288378 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r4
  r31 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,40
  r11 = uint64_t(int64_t(40));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822883A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822883dc
  if (!cr6.eq) goto loc_822883DC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822883D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_822883DC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822883E8, r31)); r3 = o.r3; f1 = o.f1; }
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
  return {r3, f1};
}

// sub_82288408: lifted from generated/ by lift.py.
lifted::Result f82288408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82288408 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25044(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25044);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82288430
  if (cr6.lt) goto loc_82288430;
  // beq cr6,0x8228842c
  if (cr6.eq) goto loc_8228842C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bgelr cr6
  if (!cr6.lt) return {r3, f1};
  // b 0x82287d70
  { const lifted::Result o = f82287D70(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_8228842C:
  // b 0x82288138
  r3 = f82288138(base, r1, r4);
  return {r3, f1};
loc_82288430:
  // b 0x82287ce0
  { const lifted::Result o = f82287CE0(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82288438: lifted from generated/ by lift.py.
lifted::Result f82288438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82288438 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,40
  r11 = uint64_t(int64_t(40));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82288468, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,25044(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25044);
  // addi r4,r11,24
  r4 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8228849c
  if (!cr6.eq) goto loc_8228849C;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82288534
  if (cr6.eq) goto loc_82288534;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82288498, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82288534
  goto loc_82288534;
loc_8228849C:
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x82288510
  if (cr6.lt) goto loc_82288510;
  // beq cr6,0x82288504
  if (cr6.eq) goto loc_82288504;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x822884d4
  if (cr6.lt) goto loc_822884D4;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82288520
  if (!cr6.eq) goto loc_82288520;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x82288530
  goto loc_82288530;
loc_822884D4:
  // mr r3,r30
  r3 = r30;
  // bl 0x82288378
  // (left out: dead in C++)
  { const lifted::Result o = f82288378(base, r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_822884DC:
  // lwz r11,25044(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25044);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82288534
  if (cr0.eq) goto loc_82288534;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x8228851c
  if (!cr6.eq) goto loc_8228851C;
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // b 0x8228852c
  goto loc_8228852C;
loc_82288504:
  // mr r3,r30
  r3 = r30;
  // bl 0x822882e0
  // (left out: dead in C++)
  { const lifted::Result o = f822882E0(base, r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822884dc
  goto loc_822884DC;
loc_82288510:
  // mr r3,r30
  r3 = r30;
  // bl 0x822881e0
  // (left out: dead in C++)
  { const lifted::Result o = f822881E0(base, r1, r13, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822884dc
  goto loc_822884DC;
loc_8228851C:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_82288520:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
  // stw r8,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r8));
loc_8228852C:
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
loc_82288530:
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
loc_82288534:
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

// sub_82288550: lifted from generated/ by lift.py.
lifted::Result f82288550(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82288550 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x822880e8
  // (left out: dead in C++)
  r3 = f822880E8(base, r1, r13);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // stw r30,9644(r31)
  guest::StoreU32(base, uint32_t(r31) + 9644, uint32_t(r30));
  // stw r30,9640(r31)
  guest::StoreU32(base, uint32_t(r31) + 9640, uint32_t(r30));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82288580, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25052(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25052);
  // addi r11,r11,-7
  r11 = uint64_t(int64_t(int64_t(r11) + -7));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25044(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25044);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bgt cr6,0x822885c0
  if (cr6.gt) goto loc_822885C0;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x822885ac
  if (cr6.eq) goto loc_822885AC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822885AC:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // b 0x822885d8
  goto loc_822885D8;
loc_822885C0:
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822885cc
  if (cr0.eq) goto loc_822885CC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822885CC:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
loc_822885D8:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r30,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r30));
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

// sub_82290AB8: lifted from generated/ by lift.py.
uint64_t f82290AB8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82290AB8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lwz r11,-32472(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -32472);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,26112(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 26112);
  // bne cr6,0x82290b10
  if (!cr6.eq) goto loc_82290B10;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82290ae0
  if (!cr6.eq) goto loc_82290AE0;
loc_82290AD8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_82290AE0:
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82290b24
  if (!cr6.eq) goto loc_82290B24;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,26116(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 26116);
  // cmpwi cr6,r11,30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 30, so);
  // blt cr6,0x82290b24
  if (cr6.lt) goto loc_82290B24;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,26120(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 26120);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82290b24
  if (!cr6.eq) goto loc_82290B24;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
loc_82290B10:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82290ad8
  if (cr6.eq) goto loc_82290AD8;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beqlr cr6
  if (cr6.eq) return r3;
loc_82290B24:
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
}

// sub_82290B98: lifted from generated/ by lift.py.
lifted::Result f82290B98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82290B98 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r29,r11,26128
  r29 = uint64_t(int64_t(int64_t(r11) + 26128));
  // lwz r30,0(r29)
  r30 = guest::LoadU32(base, uint32_t(r29) + 0);
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290BBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290BC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r31,r11,26100
  r31 = uint64_t(int64_t(int64_t(r11) + 26100));
  // b 0x82290c24
  goto loc_82290C24;
loc_82290BD4:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
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
  // bge cr6,0x82290c08
  if (!cr6.lt) goto loc_82290C08;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290C00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x82290c1c
  goto loc_82290C1C;
loc_82290C08:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290C18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
loc_82290C1C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290C24, r31)); r3 = o.r3; f1 = o.f1; }
loc_82290C24:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82290bd4
  if (!cr6.eq) goto loc_82290BD4;
  // lwz r30,4(r29)
  r30 = guest::LoadU32(base, uint32_t(r29) + 4);
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290C40, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290C4C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x82290c9c
  goto loc_82290C9C;
loc_82290C54:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
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
  // bge cr6,0x82290c88
  if (!cr6.lt) goto loc_82290C88;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290C80, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // b 0x82290c9c
  goto loc_82290C9C;
loc_82290C88:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290C98, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
loc_82290C9C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290CA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82290c54
  if (!cr6.eq) goto loc_82290C54;
  // lwz r30,8(r29)
  r30 = guest::LoadU32(base, uint32_t(r29) + 8);
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290CC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290CCC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290CD8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x82290d28
  goto loc_82290D28;
loc_82290CE0:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
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
  // bge cr6,0x82290d14
  if (!cr6.lt) goto loc_82290D14;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290D0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // b 0x82290d28
  goto loc_82290D28;
loc_82290D14:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290D24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
loc_82290D28:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290D30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82290ce0
  if (!cr6.eq) goto loc_82290CE0;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82290D48: lifted from generated/ by lift.py.
lifted::Result f82290D48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82290D48 (lifted)");
  uint64_t r0 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // bl 0x82290b98
  // (left out: dead in C++)
  { const lifted::Result o = f82290B98(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r31,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 8, so);
  // bgt cr6,0x82290e74
  if (cr6.gt) goto loc_82290E74;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,8480
  r12 = uint64_t(int64_t(int64_t(r12) + 8480));
  // lbzx r0,r12,r31
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r31));
  // lis r12,-32215
  r12 = uint64_t(int64_t(-2111242240));
  // addi r12,r12,3472
  r12 = uint64_t(int64_t(int64_t(r12) + 3472));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r31)) {
  case 0:
  goto loc_frag_82290D90;
  return {r3, f1};
  case 1:
  goto loc_82290D98;
  case 2:
  goto loc_82290DA0;
  case 3:
  goto loc_82290E08;
  case 4:
  goto loc_82290E10;
  case 5:
  goto loc_82290E18;
  case 6:
  goto loc_82290E20;
  case 7:
  goto loc_82290E28;
  case 8:
  goto loc_82290E30;
  default:
  std::abort();
  }
loc_82290D98:
  // li r6,7523
  r6 = uint64_t(int64_t(7523));
  // b 0x82290e34
  goto loc_82290E34;
loc_82290DA0:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r6,37
  r6 = uint64_t(int64_t(37));
  // addi r31,r11,26128
  r31 = uint64_t(int64_t(int64_t(r11) + 26128));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82290DBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r11,r11,26100
  r11 = uint64_t(int64_t(int64_t(r11) + 26100));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82290DFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r11));
  // b 0x82290e74
  goto loc_82290E74;
loc_82290E08:
  // li r6,38
  r6 = uint64_t(int64_t(38));
  // b 0x82290e34
  goto loc_82290E34;
loc_82290E10:
  // li r6,106
  r6 = uint64_t(int64_t(106));
  // b 0x82290e34
  goto loc_82290E34;
loc_82290E18:
  // li r6,7524
  r6 = uint64_t(int64_t(7524));
  // b 0x82290e34
  goto loc_82290E34;
loc_82290E20:
  // li r6,7525
  r6 = uint64_t(int64_t(7525));
  // b 0x82290e34
  goto loc_82290E34;
loc_82290E28:
  // li r6,7526
  r6 = uint64_t(int64_t(7526));
  // b 0x82290e34
  goto loc_82290E34;
loc_82290E30:
  // li r6,108
  r6 = uint64_t(int64_t(108));
loc_82290E34:
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,26128(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 26128);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82290E48, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,26128(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 26128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r10,26100(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 26100);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,26100(r11)
  guest::StoreU32(base, uint32_t(r11) + 26100, uint32_t(r10));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290E74, r31)); r3 = o.r3; f1 = o.f1; }
loc_82290E74:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,26116(r10)
  guest::StoreU32(base, uint32_t(r10) + 26116, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,26120(r10)
  guest::StoreU32(base, uint32_t(r10) + 26120, uint32_t(r11));
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
loc_frag_82290D90:
  // li r6,7522
  r6 = uint64_t(int64_t(7522));
  // b 0x82290e34
  goto loc_82290E34_82290D90;
loc_82290E34_82290D90:
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,26128(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 26128);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82290E48, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,26128(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 26128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r10,26100(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 26100);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,26100(r11)
  guest::StoreU32(base, uint32_t(r11) + 26100, uint32_t(r10));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290E74, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,26116(r10)
  guest::StoreU32(base, uint32_t(r10) + 26116, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,26120(r10)
  guest::StoreU32(base, uint32_t(r10) + 26120, uint32_t(r11));
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

// sub_82290EA0: lifted from generated/ by lift.py.
lifted::Result f82290EA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82290EA0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // mr r31,r3
  r31 = r3;
  // stw r11,26112(r10)
  guest::StoreU32(base, uint32_t(r10) + 26112, uint32_t(r11));
  // bl 0x82290b98
  // (left out: dead in C++)
  { const lifted::Result o = f82290B98(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r31,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 2, so);
  // blt cr6,0x82290edc
  if (cr6.lt) goto loc_82290EDC;
  // beq cr6,0x82290f2c
  if (cr6.eq) goto loc_82290F2C;
  // cmplwi cr6,r31,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 8, so);
  // blt cr6,0x82290edc
  if (cr6.lt) goto loc_82290EDC;
  // bne cr6,0x82290f0c
  if (!cr6.eq) goto loc_82290F0C;
loc_82290EDC:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // lwz r3,26128(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 26128);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290EFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,26100(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 26100);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,26100(r11)
  guest::StoreU32(base, uint32_t(r11) + 26100, uint32_t(r10));
loc_82290F0C:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,26120(r10)
  guest::StoreU32(base, uint32_t(r10) + 26120, uint32_t(r11));
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
loc_82290F2C:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r31,r11,26128
  r31 = uint64_t(int64_t(int64_t(r11) + 26128));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290F50, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82290F5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,26100
  r11 = uint64_t(int64_t(int64_t(r11) + 26100));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // b 0x82290f0c
  goto loc_82290F0C;
}

// sub_82290F78: lifted from generated/ by lift.py.
uint64_t f82290F78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82290F78 (lifted)");
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
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lwz r11,-32472(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -32472);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82290fc8
  if (!cr6.eq) goto loc_82290FC8;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r31,r11,26128
  r31 = uint64_t(int64_t(int64_t(r11) + 26128));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82290FBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82290FC8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82290FC8:
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

// sub_82290FE0: lifted from generated/ by lift.py.
uint64_t f82290FE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82290FE0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // rlwinm r10,r3,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,26064
  r11 = uint64_t(int64_t(int64_t(r11) + 26064));
  // lwzx r9,r10,r11
  r9 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x82291050
  if (!cr6.eq) goto loc_82291050;
  // lis r9,-32156
  r9 = uint64_t(int64_t(-2107375616));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lwz r9,-32472(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + -32472);
  // stwx r8,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r8));
  // cmplwi cr6,r9,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 2, so);
  // blt cr6,0x8229102c
  if (cr6.lt) goto loc_8229102C;
  // beq cr6,0x82291038
  if (cr6.eq) goto loc_82291038;
  // cmplwi cr6,r9,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 8, so);
  // blt cr6,0x8229102c
  if (cr6.lt) goto loc_8229102C;
  // bne cr6,0x82291048
  if (!cr6.eq) goto loc_82291048;
loc_8229102C:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,26128(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 26128);
  // b 0x8229104c
  goto loc_8229104C;
loc_82291038:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,26128
  r11 = uint64_t(int64_t(int64_t(r11) + 26128));
  // lwz r3,4(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 4);
  // b 0x8229104c
  goto loc_8229104C;
loc_82291048:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_8229104C:
  // bl 0x82290b30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82290B30, LIFTED_ARGS(0x82291050, r31)); r3 = o.r3; f1 = o.f1; }
loc_82291050:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82291060: lifted from generated/ by lift.py.
lifted::Result f82291060(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82291060 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r31,r3
  r31 = r3;
  // stw r11,26112(r10)
  guest::StoreU32(base, uint32_t(r10) + 26112, uint32_t(r11));
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // stw r31,-32472(r11)
  guest::StoreU32(base, uint32_t(r11) + -32472, uint32_t(r31));
  // bl 0x82290d48
  // (left out: dead in C++)
  { const lifted::Result o = f82290D48(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,675
  r5 = uint64_t(int64_t(675));
  // li r4,673
  r4 = uint64_t(int64_t(673));
  // mr r3,r31
  r3 = r31;
  // bl 0x82290fe0
  // (left out: dead in C++)
  r3 = f82290FE0(base, r1, r13, r3, r4, r5);
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

// sub_822910B0: lifted from generated/ by lift.py.
lifted::Result f822910B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822910B0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x822910c0
  if (cr6.eq) goto loc_822910C0;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // stw r3,-32468(r11)
  guest::StoreU32(base, uint32_t(r11) + -32468, uint32_t(r3));
loc_822910C0:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,26116(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 26116);
  // cmpwi cr6,r11,30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 30, so);
  // blt cr6,0x822910dc
  if (cr6.lt) goto loc_822910DC;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lwz r3,-32472(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -32472);
  // b 0x82290ea0
  { const lifted::Result o = f82290EA0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_822910DC:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,26120(r10)
  guest::StoreU32(base, uint32_t(r10) + 26120, uint32_t(r11));
  // blr
  return {r3, f1};
}

// sub_822910F0: lifted from generated/ by lift.py.
lifted::Result f822910F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822910F0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,26112(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 26112);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82291228
  if (cr6.eq) goto loc_82291228;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x822911ac
  if (cr6.eq) goto loc_822911AC;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x82291288
  if (!cr6.eq) goto loc_82291288;
  // lis r31,-32156
  r31 = uint64_t(int64_t(-2107375616));
  // lwz r11,-32472(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -32472);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // blt cr6,0x82291144
  if (cr6.lt) goto loc_82291144;
  // beq cr6,0x82291150
  if (cr6.eq) goto loc_82291150;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // blt cr6,0x82291144
  if (cr6.lt) goto loc_82291144;
  // bne cr6,0x82291160
  if (!cr6.eq) goto loc_82291160;
loc_82291144:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,26128(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 26128);
  // b 0x82291164
  goto loc_82291164;
loc_82291150:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,26128
  r11 = uint64_t(int64_t(int64_t(r11) + 26128));
  // lwz r3,4(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 4);
  // b 0x82291164
  goto loc_82291164;
loc_82291160:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_82291164:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8229116C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82291288
  if (!cr0.eq) goto loc_82291288;
  // lis r10,-32156
  r10 = uint64_t(int64_t(-2107375616));
  // lwz r3,-32468(r10)
  r3 = guest::LoadU32(base, uint32_t(r10) + -32468);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x822911a0
  if (cr6.eq) goto loc_822911A0;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r3,-32472(r31)
  guest::StoreU32(base, uint32_t(r31) + -32472, uint32_t(r3));
  // stw r11,-32468(r10)
  guest::StoreU32(base, uint32_t(r10) + -32468, uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,26112(r30)
  guest::StoreU32(base, uint32_t(r30) + 26112, uint32_t(r11));
  // bl 0x82290d48
  // (left out: dead in C++)
  { const lifted::Result o = f82290D48(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82291288
  goto loc_82291288;
loc_822911A0:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,26112(r30)
  guest::StoreU32(base, uint32_t(r30) + 26112, uint32_t(r11));
  // b 0x82291288
  goto loc_82291288;
loc_822911AC:
  // lis r31,-32156
  r31 = uint64_t(int64_t(-2107375616));
  // lwz r11,-32472(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -32472);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // blt cr6,0x822911cc
  if (cr6.lt) goto loc_822911CC;
  // beq cr6,0x822911d8
  if (cr6.eq) goto loc_822911D8;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // blt cr6,0x822911cc
  if (cr6.lt) goto loc_822911CC;
  // bne cr6,0x822911e8
  if (!cr6.eq) goto loc_822911E8;
loc_822911CC:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,26128(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 26128);
  // b 0x822911ec
  goto loc_822911EC;
loc_822911D8:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,26128
  r11 = uint64_t(int64_t(int64_t(r11) + 26128));
  // lwz r3,4(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 4);
  // b 0x822911ec
  goto loc_822911EC;
loc_822911E8:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_822911EC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822911F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,26116(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 26116);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r11,30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 30, so);
  // stw r11,26116(r10)
  guest::StoreU32(base, uint32_t(r10) + 26116, uint32_t(r11));
  // blt cr6,0x82291288
  if (cr6.lt) goto loc_82291288;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,26120(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 26120);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82291288
  if (!cr6.eq) goto loc_82291288;
  // lwz r3,-32472(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -32472);
  // bl 0x82290ea0
  // (left out: dead in C++)
  { const lifted::Result o = f82290EA0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82291288
  goto loc_82291288;
loc_82291228:
  // lis r31,-32156
  r31 = uint64_t(int64_t(-2107375616));
  // lwz r11,-32472(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -32472);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // blt cr6,0x82291248
  if (cr6.lt) goto loc_82291248;
  // beq cr6,0x82291254
  if (cr6.eq) goto loc_82291254;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // blt cr6,0x82291248
  if (cr6.lt) goto loc_82291248;
  // bne cr6,0x82291264
  if (!cr6.eq) goto loc_82291264;
loc_82291248:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,26128(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 26128);
  // b 0x82291268
  goto loc_82291268;
loc_82291254:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,26128
  r11 = uint64_t(int64_t(int64_t(r11) + 26128));
  // lwz r3,4(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 4);
  // b 0x82291268
  goto loc_82291268;
loc_82291264:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_82291268:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82291270, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82291288
  if (!cr0.eq) goto loc_82291288;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // lwz r3,-32472(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -32472);
  // stw r11,26112(r30)
  guest::StoreU32(base, uint32_t(r30) + 26112, uint32_t(r11));
  // bl 0x82290f78
  // (left out: dead in C++)
  r3 = f82290F78(base, r1, r13, r3);
loc_82291288:
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

// sub_82291478: lifted from generated/ by lift.py.
uint64_t f82291478(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82291478 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r11,28220(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 28220);
  // lwz r10,220(r11)
  r10 = guest::LoadU32(base, schema::Session(base, uint32_t(r11)).unknown_220_address());
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8229149c
  if (!cr6.eq) goto loc_8229149C;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,29320(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 29320);
  // stw r10,220(r11)
  guest::StoreU32(base, schema::Session(base, uint32_t(r11)).unknown_220_address(), uint32_t(r10));
  // lwz r11,28220(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 28220);
loc_8229149C:
  // lwz r3,220(r11)
  r3 = guest::LoadU32(base, schema::Session(base, uint32_t(r11)).unknown_220_address());
  // blr
  return r3;
}

// sub_82292338: lifted from generated/ by lift.py.
uint64_t f82292338(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82292338 (lifted)");
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
  // bne cr6,0x8229237c
  if (!cr6.eq) goto loc_8229237C;
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
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82292378, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822923a0
  goto loc_822923A0;
loc_8229237C:
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x82292380);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x822923a0
  if (cr0.lt) goto loc_822923A0;
  // lis r10,19
  r10 = uint64_t(int64_t(1245184));
  // lwz r11,556(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 556);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // ori r10,r10,51440
  r10 = r10 | 51440;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x822923a4
  if (cr6.eq) goto loc_822923A4;
loc_822923A0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_822923A4:
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

// sub_822923B8: lifted from generated/ by lift.py.
uint64_t f822923B8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822923B8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,24704(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24704);
  // addi r11,r11,-997
  r11 = uint64_t(int64_t(int64_t(r11) + -997));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // blr
  return r3;
}

// sub_822923D8: lifted from generated/ by lift.py.
lifted::Result f822923D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822923D8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r27,r11,24704
  r27 = uint64_t(int64_t(int64_t(r11) + 24704));
  // mr r31,r4
  r31 = r4;
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // bne cr6,0x82292408
  if (!cr6.eq) goto loc_82292408;
loc_82292400:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8229250c
  goto loc_8229250C;
loc_82292408:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addis r29,r28,20
  r29 = uint64_t(int64_t(int64_t(r28) + 1310720));
  // addi r29,r29,-13004
  r29 = uint64_t(int64_t(int64_t(r29) + -13004));
  // lwz r11,29428(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29428);
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8229242c
  if (cr0.eq) goto loc_8229242C;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x8229242C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8229242C:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r30,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r30));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x82576fe8
  // (left out: dead in C++)
  { const lifted::Result o = f82576FE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82292400
  if (!cr0.eq) goto loc_82292400;
  // lis r11,32767
  r11 = uint64_t(int64_t(2147418112));
  // ori r10,r11,65535
  r10 = r11 | 65535;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // rlwinm r3,r11,1,0,30
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // ble cr6,0x82292478
  if (!cr6.gt) goto loc_82292478;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_82292478:
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x8229247C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r3
  r8 = r3;
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r8,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r8));
  // mr r3,r31
  r3 = r31;
  // bl 0x82576fe8
  // (left out: dead in C++)
  { const lifted::Result o = f82576FE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // addi r31,r28,556
  r31 = uint64_t(int64_t(int64_t(r28) + 556));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // stw r30,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r30));
  // stw r30,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r30));
  // stw r30,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r30));
  // stw r30,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r30));
  // stw r30,16(r31)
  guest::StoreU32(base, uint32_t(r31) + 16, uint32_t(r30));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822924C8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r28,504
  r30 = uint64_t(int64_t(int64_t(r28) + 504));
  // li r5,52
  r5 = uint64_t(int64_t(52));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x822924DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r8,r31
  r8 = r31;
  // lwz r4,0(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 0);
  // mr r9,r27
  r9 = r27;
  // li r7,20
  r7 = uint64_t(int64_t(20));
  // mr r6,r30
  r6 = r30;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x82576900
  // (left out: dead in C++)
  { const lifted::Result o = f82576900(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,-997
  r11 = uint64_t(int64_t(int64_t(r3) + -997));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
loc_8229250C:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82292518: lifted from generated/ by lift.py.
lifted::Result f82292518(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82292518 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r27,r11,24704
  r27 = uint64_t(int64_t(int64_t(r11) + 24704));
  // mr r31,r4
  r31 = r4;
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // bne cr6,0x82292548
  if (!cr6.eq) goto loc_82292548;
loc_82292540:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8229263c
  goto loc_8229263C;
loc_82292548:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addis r29,r28,20
  r29 = uint64_t(int64_t(int64_t(r28) + 1310720));
  // addi r29,r29,-13004
  r29 = uint64_t(int64_t(int64_t(r29) + -13004));
  // lwz r11,29428(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29428);
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8229256c
  if (cr0.eq) goto loc_8229256C;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x8229256C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8229256C:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r30,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r30));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x82576fe8
  // (left out: dead in C++)
  { const lifted::Result o = f82576FE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82292540
  if (!cr0.eq) goto loc_82292540;
  // lis r11,32767
  r11 = uint64_t(int64_t(2147418112));
  // ori r10,r11,65535
  r10 = r11 | 65535;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // rlwinm r3,r11,1,0,30
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // ble cr6,0x822925b8
  if (!cr6.gt) goto loc_822925B8;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_822925B8:
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x822925BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r3
  r8 = r3;
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r8,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r8));
  // mr r3,r31
  r3 = r31;
  // bl 0x82576fe8
  // (left out: dead in C++)
  { const lifted::Result o = f82576FE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // addi r31,r28,556
  r31 = uint64_t(int64_t(int64_t(r28) + 556));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // stw r30,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r30));
  // stw r30,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r30));
  // stw r30,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r30));
  // stw r30,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r30));
  // stw r30,16(r31)
  guest::StoreU32(base, uint32_t(r31) + 16, uint32_t(r30));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82292608, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r5,19
  r5 = uint64_t(int64_t(1245184));
  // lwz r4,0(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 0);
  // mr r8,r31
  r8 = r31;
  // mr r9,r27
  r9 = r27;
  // li r7,20
  r7 = uint64_t(int64_t(20));
  // addi r6,r28,1088
  r6 = uint64_t(int64_t(int64_t(r28) + 1088));
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // ori r5,r5,51440
  r5 = r5 | 51440;
  // bl 0x82576900
  // (left out: dead in C++)
  { const lifted::Result o = f82576900(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,-997
  r11 = uint64_t(int64_t(int64_t(r3) + -997));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
loc_8229263C:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82292768: lifted from generated/ by lift.py.
lifted::Result f82292768(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82292768 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,-21596(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // slw r9,r10,r11
  r9 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r10) << (uint8_t(r11) & 0x3F));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,31068(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 31068);
  // and. r10,r9,r10
  r10 = r9 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beqlr
  if (cr0.eq) return {r3, f1};
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mulli r9,r3,48
  r9 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(48))));
  // lwz r10,27348(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 27348);
  // mr r3,r11
  r3 = r11;
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r11,12(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 12);
  // ldx r4,r11,r9
  r4 = guest::LoadU64(base, uint32_t(r11) + uint32_t(r9));
  // b 0x822952d0
  { const lifted::Result o = game::XShowGamerCardUI(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822952C8 game::XShowFriendsUI: lifted from generated/ by lift.py.
lifted::Result f822952C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822952C8 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825db554
  { const lifted::Result o = lifted::Import(base, __imp__XamShowFriendsUI, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822952D0 game::XShowGamerCardUI: lifted from generated/ by lift.py.
lifted::Result f822952D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822952D0 (lifted)");
  uint64_t r5 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // b 0x825db564
  { const lifted::Result o = lifted::Import(base, __imp__XamShowGamerCardUIForXUID, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822952D8 game::XShowPlayerReviewUI: lifted from generated/ by lift.py.
lifted::Result f822952D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822952D8 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825db574
  { const lifted::Result o = lifted::Import(base, __imp__XamShowPlayerReviewUI, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822AD690: lifted from generated/ by lift.py.
uint64_t f822AD690(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_822AD690 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // mr r11,r3
  r11 = r3;
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r9,13160(r11)
  r9 = guest::LoadU32(base, schema::Device(base, uint32_t(r11)).recorder_address());
  // beq 0x822ad718
  if (cr0.eq) goto loc_822AD718;
  // lwz r10,0(r5)
  r10 = guest::LoadU32(base, schema::CommandSpaceSize(base, uint32_t(r5)).size_address());
  // rlwinm r8,r10,30,2,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // lwz r10,13516(r11)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r11)).data_cursor_address());
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x822ad6c4
  if (!cr0.eq) goto loc_822AD6C4;
  // lwz r10,156(r9)
  r10 = guest::LoadU32(base, schema::CommandRecorder(base, uint32_t(r9)).buffer_size_address());
  // lwz r9,152(r9)
  r9 = guest::LoadU32(base, schema::CommandRecorder(base, uint32_t(r9)).buffer_address());
  // rlwinm r10,r10,0,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFC;
  // add r10,r10,r9
  r10 = r10 + r9;
loc_822AD6C4:
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwz r9,40(r11)
  r9 = guest::LoadU32(base, schema::Device(base, uint32_t(r11)).write_position_address());
  // addi r7,r6,-1
  r7 = uint64_t(int64_t(int64_t(r6) + -1));
  // subf r8,r8,r10
  r8 = r10 - r8;
  // addi r9,r9,164
  r9 = uint64_t(int64_t(int64_t(r9) + 164));
  // andc r3,r8,r7
  r3 = r8 & ~r7;
  // cmplw cr6,r3,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r9), so);
  // bge cr6,0x822ad6ec
  if (!cr6.lt) goto loc_822AD6EC;
loc_822AD6E4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_822AD6EC:
  // subf r10,r3,r10
  r10 = r10 - r3;
  // lwz r9,44(r11)
  r9 = guest::LoadU32(base, schema::Device(base, uint32_t(r11)).segment_end_address());
  // lwz r8,48(r11)
  r8 = guest::LoadU32(base, schema::Device(base, uint32_t(r11)).soft_end_address());
  // srawi r10,r10,2
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 2));
  // stw r3,13516(r11)
  guest::StoreU32(base, schema::Device(base, uint32_t(r11)).data_cursor_address(), uint32_t(r3));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // subf r9,r10,r9
  r9 = r9 - r10;
  // subf r10,r10,r8
  r10 = r8 - r10;
  // stw r9,44(r11)
  guest::StoreU32(base, schema::Device(base, uint32_t(r11)).segment_end_address(), uint32_t(r9));
  // stw r10,48(r11)
  guest::StoreU32(base, schema::Device(base, uint32_t(r11)).soft_end_address(), uint32_t(r10));
  // blr
  return r3;
loc_822AD718:
  // lwz r10,48(r11)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r11)).soft_end_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x822ad6e4
  if (!cr6.eq) goto loc_822AD6E4;
  // lwz r10,152(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 152);
  // stw r10,13528(r11)
  guest::StoreU32(base, schema::Device(base, uint32_t(r11)).segment_start_address(), uint32_t(r10));
  // lwz r11,156(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 156);
  // stw r11,0(r5)
  guest::StoreU32(base, schema::CommandSpaceSize(base, uint32_t(r5)).size_address(), uint32_t(r11));
  // lwz r3,152(r9)
  r3 = guest::LoadU32(base, uint32_t(r9) + 152);
  // blr
  return r3;
}

// sub_822AD740: lifted from generated/ by lift.py.
uint64_t f822AD740(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822AD740 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,152(r3)
  r11 = guest::LoadU32(base, schema::ResourceHeader(base, uint32_t(r3)).type9_data_address());
  // lis r9,16384
  r9 = uint64_t(int64_t(1073741824));
  // lwz r10,156(r3)
  r10 = guest::LoadU32(base, schema::ResourceHeader(base, uint32_t(r3)).unknown_156_address());
  // mr r7,r9
  r7 = r9;
  // add r10,r10,r11
  r10 = r10 + r11;
  // mr r6,r9
  r6 = r9;
  // rlwinm r8,r10,12,20,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFF;
  // rlwinm r9,r11,12,20,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 12) & 0xFFF;
  // addi r8,r8,512
  r8 = uint64_t(int64_t(int64_t(r8) + 512));
  // addi r5,r9,512
  r5 = uint64_t(int64_t(int64_t(r9) + 512));
  // rlwinm r9,r8,0,19,19
  r9 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x1000;
  // clrlwi r8,r10,3
  r8 = uint32_t(r10) & 0x1FFFFFFF;
  // rlwinm r10,r5,0,19,19
  r10 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0x1000;
  // clrlwi r11,r11,3
  r11 = uint32_t(r11) & 0x1FFFFFFF;
  // add r9,r9,r8
  r9 = r9 + r8;
  // add r11,r10,r11
  r11 = r10 + r11;
  // subf r4,r7,r9
  r4 = r9 - r7;
  // subf r3,r6,r11
  r3 = r11 - r6;
  // b 0x822ac470
  { const lifted::Result o = lifted::entry::e822AC470(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_822AD8F8: lifted from generated/ by lift.py.
lifted::Result f822AD8F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_822AD8F8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // mr r30,r5
  r30 = r5;
  // mr r28,r4
  r28 = r4;
  // mr r27,r6
  r27 = r6;
  // lwz r10,1968(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 1968);
  // lwz r11,168(r31)
  r11 = guest::LoadU32(base, schema::CommandRecorder(base, uint32_t(r31)).max_size_address());
  // lwz r9,0(r30)
  r9 = guest::LoadU32(base, schema::CommandSpaceSize(base, uint32_t(r30)).size_address());
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // bge cr6,0x822ad934
  if (!cr6.lt) goto loc_822AD934;
  // stw r11,0(r30)
  guest::StoreU32(base, schema::CommandSpaceSize(base, uint32_t(r30)).size_address(), uint32_t(r11));
loc_822AD934:
  // addi r29,r10,13576
  r29 = uint64_t(int64_t(int64_t(r10) + 13576));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x822AD940, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,164(r31)
  r3 = guest::LoadU32(base, schema::CommandRecorder(base, uint32_t(r31)).callback_context_address());
  // lwz r11,172(r31)
  r11 = guest::LoadU32(base, schema::CommandRecorder(base, uint32_t(r31)).callback_address());
  // mr r6,r27
  r6 = r27;
  // mr r5,r30
  r5 = r30;
  // mr r4,r28
  r4 = r28;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AD95C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x822AD968, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822AD978: lifted from generated/ by lift.py.
lifted::Result f822AD978(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822AD978 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // lwz r11,1968(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 1968);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r30,r11,13576
  r30 = uint64_t(int64_t(int64_t(r11) + 13576));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x822AD9A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,164(r31)
  r3 = guest::LoadU32(base, schema::ResourceHeader(base, uint32_t(r31)).unknown_164_address());
  // lwz r11,176(r31)
  r11 = guest::LoadU32(base, schema::ResourceHeader(base, uint32_t(r31)).unknown_176_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AD9B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x822AD9C0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822AD9D8: lifted from generated/ by lift.py.
lifted::Result f822AD9D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822AD9D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,152(r31)
  r11 = guest::LoadU32(base, schema::ResourceHeader(base, uint32_t(r31)).type9_data_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822ada00
  if (!cr6.eq) goto loc_822ADA00;
  // bl 0x822ad978
  // (left out: dead in C++)
  { const lifted::Result o = f822AD978(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x822ada10
  goto loc_822ADA10;
loc_822ADA00:
  // bl 0x822ad740
  // (left out: dead in C++)
  r3 = f822AD740(base, r1, r13, r3, 0x822ADA04);
  // lis r4,-20096
  r4 = uint64_t(int64_t(-1317011456));
  // lwz r3,152(r31)
  r3 = guest::LoadU32(base, schema::ResourceHeader(base, uint32_t(r31)).type9_data_address());
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x822ADA10, r31)); r3 = o.r3; f1 = o.f1; }
loc_822ADA10:
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

// sub_82459CA0 game::accept: lifted from generated/ by lift.py.
lifted::Result f82459CA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459CA0 (lifted)");
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
  // b 0x825dbca4
  { const lifted::Result o = lifted::Import(base, __imp__NetDll_accept, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82459CB8 game::select: lifted from generated/ by lift.py.
lifted::Result f82459CB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459CB8 (lifted)");
  uint64_t r8 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r8,r7
  r8 = r7;
  // mr r7,r6
  r7 = r6;
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // mr r4,r3
  r4 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825dbcb4
  { const lifted::Result o = lifted::Import(base, __imp__NetDll_select, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82459CD8 game::recvfrom: lifted from generated/ by lift.py.
lifted::Result f82459CD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459CD8 (lifted)");
  uint64_t r9 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r9,r8
  r9 = r8;
  // mr r8,r7
  r8 = r7;
  // mr r7,r6
  r7 = r6;
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // mr r4,r3
  r4 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825dbcc4
  { const lifted::Result o = lifted::Import(base, __imp__NetDll_recvfrom, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82459D20 game::__WSAFDIsSet: lifted from generated/ by lift.py.
lifted::Result f82459D20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82459D20 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825dbcf4
  { const lifted::Result o = lifted::Import(base, __imp__NetDll___WSAFDIsSet, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82480780 game::X3DAudio::EmitterAngle: lifted from generated/ by lift.py.
lifted::Result f82480780(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82480780 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stfd f31,-32(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r6
  r31 = r6;
  // mr r10,r4
  r10 = r4;
  // mr r30,r5
  r30 = r5;
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,36(r31)
  temp.u32 = guest::LoadU32(base, schema::X3DAudioDspSettings(base, uint32_t(r31)).distance_address());
  f13 = double(temp.f32);
  // lfs f0,26896(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26896);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x824807e4
  if (!cr6.gt) goto loc_824807E4;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r30,28
  r4 = uint64_t(int64_t(int64_t(r30) + 28));
  // addi r3,r10,24
  r3 = uint64_t(int64_t(int64_t(r10) + 24));
  // bl 0x824819e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x824819E0, LIFTED_ARGS(0x824807D0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r30,4
  r4 = uint64_t(int64_t(int64_t(r30) + 4));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x824819b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x824819B8, LIFTED_ARGS(0x824807DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::X3DAudioDspSettings(base, uint32_t(r31)).distance_address());
  f0 = double(temp.f32);
  // fdivs f31,f1,f0
  f31 = double(float(f1 / f0));
loc_824807E4:
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x825a8170
  // (left out: dead in C++)
  { const lifted::Result o = game::acos(base, LIFTED_ARGS(0x824807EC, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f1,f31
  f1 = f31;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,32(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::X3DAudioDspSettings(base, uint32_t(r31)).emitter_angle_address(), temp.u32);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-32(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82481708 game::X3DAudio::Delay: lifted from generated/ by lift.py.
lifted::Result f82481708(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_82481708 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-48(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -48, std::bit_cast<uint64_t>(f30));
  // stfd f31,-40(r1)
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r29,r5
  r29 = r5;
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::X3DAudioVector(base, uint32_t(r31)).x_address());
  // mr r30,r6
  r30 = r6;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::X3DAudioVector(base, uint32_t(r31)).y_address());
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::X3DAudioVector(base, uint32_t(r31)).z_address());
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // bl 0x82481a18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82481A18, LIFTED_ARGS(0x82481750, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82481a18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82481A18, LIFTED_ARGS(0x82481760, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r29,28
  r11 = uint64_t(int64_t(int64_t(r29) + 28));
  // addi r5,r1,128
  r5 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r4,r31,24
  r4 = uint64_t(int64_t(int64_t(r31) + 24));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::X3DAudioEmitter(base, uint32_t(r11) - 28).position_x_address());
  // stw r10,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::X3DAudioEmitter(base, uint32_t(r11) - 28).position_y_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::X3DAudioEmitter(base, uint32_t(r11) - 28).position_z_address());
  // stw r10,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r10));
  // stw r11,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r11));
  // bl 0x824819e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x824819E0, LIFTED_ARGS(0x8248178C, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f13,132(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 132);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,100(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 100);
  f12 = double(temp.f32);
  // fmuls f10,f13,f12
  f10 = double(float(f13 * f12));
  // lfs f7,84(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f7 = double(temp.f32);
  // fmuls f13,f7,f13
  f13 = double(float(f7 * f13));
  // lfs f0,136(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 136);
  f0 = double(temp.f32);
  // lfs f11,104(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 104);
  f11 = double(temp.f32);
  // lfs f8,88(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f8 = double(temp.f32);
  // lfs f12,128(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 128);
  f12 = double(temp.f32);
  // lfs f9,96(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 96);
  f9 = double(temp.f32);
  // lfs f7,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f7 = double(temp.f32);
  // fmadds f11,f0,f11,f10
  f11 = double(float(std::fma(f0, f11, f10)));
  // fmadds f0,f8,f0,f13
  f0 = double(float(std::fma(f8, f0, f13)));
  // fmadds f31,f12,f9,f11
  f31 = double(float(std::fma(f12, f9, f11)));
  // fmadds f2,f7,f12,f0
  f2 = double(float(std::fma(f7, f12, f0)));
  // lfs f0,26896(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26896);
  f0 = double(temp.f32);
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // bgt cr6,0x8248180c
  if (cr6.gt) goto loc_8248180C;
  // fcmpu cr6,f2,f0
  cr6 = lifted::CompareFloat(f2, f0);
  // bgt cr6,0x8248180c
  if (cr6.gt) goto loc_8248180C;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfs f0,-11528(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -11528);
  f0 = double(temp.f32);
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // blt cr6,0x8248180c
  if (cr6.lt) goto loc_8248180C;
  // fcmpu cr6,f2,f0
  cr6 = lifted::CompareFloat(f2, f0);
  // blt cr6,0x8248180c
  if (cr6.lt) goto loc_8248180C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::X3DAudioDspSettings(base, uint32_t(r30)).delays_address());
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::X3DAudioDelays(base, uint32_t(r11)).delay_0_address(), temp.u32);
  // b 0x82481864
  goto loc_82481864;
loc_8248180C:
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x825a8770
  // (left out: dead in C++)
  { const lifted::Result o = game::atan2(base, LIFTED_ARGS(0x82481814, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f1,f1
  lifted::ScalarFloatMode();
  f1 = double(float(f1));
  // lfs f30,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f30 = double(temp.f32);
  // fcmpu cr6,f31,f30
  cr6 = lifted::CompareFloat(f31, f30);
  // ble cr6,0x82481850
  if (!cr6.gt) goto loc_82481850;
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = game::sin(base, LIFTED_ARGS(0x8248182C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::X3DAudioDspSettings(base, uint32_t(r30)).delays_address());
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::X3DAudioDelays(base, uint32_t(r11)).delay_0_address(), temp.u32);
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::X3DAudioDspSettings(base, uint32_t(r30)).delays_address());
  // stfs f30,4(r11)
  temp.f32 = float(f30);
  guest::StoreU32(base, schema::X3DAudioDelays(base, uint32_t(r11)).delay_1_address(), temp.u32);
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f30,-48(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_82481850:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::X3DAudioDspSettings(base, uint32_t(r30)).delays_address());
  // fneg f1,f1
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f1) ^ 0x8000000000000000));
  // stfs f30,0(r11)
  temp.f32 = float(f30);
  guest::StoreU32(base, schema::X3DAudioDelays(base, uint32_t(r11)).delay_0_address(), temp.u32);
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = game::sin(base, LIFTED_ARGS(0x82481860, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
loc_82481864:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::X3DAudioDspSettings(base, uint32_t(r30)).delays_address());
  // stfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::X3DAudioDelays(base, uint32_t(r11)).delay_1_address(), temp.u32);
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f30,-48(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82481A70 game::MoviePlayer::RenderNextFrame: lifted from generated/ by lift.py.
lifted::Result f82481A70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82481A70 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, schema::MoviePlayerVtable(base, uint32_t(r11)).render_next_frame_address());
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82481A80 game::MoviePlayer::SetRect: lifted from generated/ by lift.py.
lifted::Result f82481A80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82481A80 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0;
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
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // mr r30,r4
  r30 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,116(r11)
  r11 = guest::LoadU32(base, schema::MoviePlayerVtable(base, uint32_t(r11)).set_rect_address());
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481AB0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x82481b14
  if (cr6.lt) goto loc_82481B14;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481aec
  if (cr6.eq) goto loc_82481AEC;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,64(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481AE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_82481AEC:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x82481b14
  if (cr6.lt) goto loc_82481B14;
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82481b14
  if (cr6.eq) goto loc_82481B14;
  // mr r4,r30
  r4 = r30;
  // mr r3,r11
  r3 = r11;
  // bl 0x82482da8
  // (left out: dead in C++)
  { const lifted::Result o = f82482DA8(base, r1, r13, r3, r4, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_82481B14:
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481b30
  if (cr6.eq) goto loc_82481B30;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481B2C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
loc_82481B30:
  // lwz r3,84(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 84);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481b4c
  if (cr6.eq) goto loc_82481B4C;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481B4C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82481B4C:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82482CA8: lifted from generated/ by lift.py.
lifted::Result f82482CA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82482CA8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r3,56(r31)
  r3 = guest::LoadU32(base, schema::MovieLib82482CA8R3(base, uint32_t(r31)).unknown_56_address());
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82482d50
  if (cr6.eq) goto loc_82482D50;
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822a3880
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::GetStreamSource(base, LIFTED_ARGS(0x82482CDC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,4096
  r8 = uint64_t(int64_t(4096));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r30,r3
  r30 = r3;
  // lwz r3,56(r31)
  r3 = guest::LoadU32(base, schema::MovieLib82482CA8R3(base, uint32_t(r31)).unknown_56_address());
  // bl 0x822a3758
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::SetStreamSource(base, LIFTED_ARGS(0x82482CFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,120
  r5 = uint64_t(int64_t(120));
  // lwz r3,64(r31)
  r3 = guest::LoadU32(base, schema::MovieLib82482CA8R3(base, uint32_t(r31)).unknown_64_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822a68f8
  // (left out: dead in C++)
  { const lifted::Result o = game::VertexBuffer::Lock(base, LIFTED_ARGS(0x82482D10, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r31,144
  r4 = uint64_t(int64_t(int64_t(r31) + 144));
  // li r5,120
  r5 = uint64_t(int64_t(120));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82482D1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,64(r31)
  r3 = guest::LoadU32(base, schema::MovieLib82482CA8R3(base, uint32_t(r31)).unknown_64_address());
  // bl 0x822a6958
  // (left out: dead in C++)
  { const lifted::Result o = game::VertexBuffer::Unlock(base, LIFTED_ARGS(0x82482D24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,4096
  r8 = uint64_t(int64_t(4096));
  // mr r5,r30
  r5 = r30;
  // lwz r7,80(r1)
  r7 = guest::LoadU32(base, uint32_t(r1) + 80);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,56(r31)
  r3 = guest::LoadU32(base, schema::MovieLib82482CA8R3(base, uint32_t(r31)).unknown_56_address());
  // lwz r6,84(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x822a3758
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::SetStreamSource(base, LIFTED_ARGS(0x82482D40, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82482d50
  if (cr6.eq) goto loc_82482D50;
  // mr r3,r30
  r3 = r30;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Resource::Release(base, LIFTED_ARGS(0x82482D50, r31)); r3 = o.r3; f1 = o.f1; }
loc_82482D50:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_82482DA8: lifted from generated/ by lift.py.
lifted::Result f82482DA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82482DA8 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x82482dcc
  if (!cr6.eq) goto loc_82482DCC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r4,r3,96
  r4 = uint64_t(int64_t(int64_t(r3) + 96));
  // stw r11,112(r3)
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_112_address(), uint32_t(r11));
  // b 0x82482df4
  goto loc_82482DF4;
loc_82482DCC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,112(r3)
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_112_address(), uint32_t(r11));
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_96_address());
  // stw r11,80(r3)
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_80_address(), uint32_t(r11));
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_100_address());
  // stw r11,84(r3)
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_84_address(), uint32_t(r11));
  // lwz r11,8(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_104_address());
  // stw r11,88(r3)
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_88_address(), uint32_t(r11));
  // lwz r11,12(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_108_address());
  // stw r11,92(r3)
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_92_address(), uint32_t(r11));
loc_82482DF4:
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_96_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,144(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_144_address(), temp.u32);
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_100_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,148(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_148_address(), temp.u32);
  // lwz r11,8(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_104_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,164(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_164_address(), temp.u32);
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_100_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,168(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_168_address(), temp.u32);
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_96_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,184(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_184_address(), temp.u32);
  // lwz r11,12(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_108_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,188(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_188_address(), temp.u32);
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_96_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,204(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_204_address(), temp.u32);
  // lwz r11,12(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_108_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,208(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_208_address(), temp.u32);
  // lwz r11,8(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_104_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,224(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_224_address(), temp.u32);
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_100_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,228(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_228_address(), temp.u32);
  // lwz r11,8(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_104_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,244(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_244_address(), temp.u32);
  // lwz r11,12(r4)
  r11 = guest::LoadU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r4) - 96).unknown_108_address());
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,248(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MovieLib82482DA8R3(base, uint32_t(r3)).unknown_248_address(), temp.u32);
  // bl 0x82482ca8
  // (left out: dead in C++)
  { const lifted::Result o = f82482CA8(base, r1, r13, r3, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
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

// sub_82575FE0 game::XSessionModify: lifted from generated/ by lift.py.
lifted::Result f82575FE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82575FE0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // mr r30,r7
  r30 = r7;
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x82576004, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x82576074
  if (!cr0.eq) goto loc_82576074;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,16
  r7 = uint64_t(int64_t(16));
  // stw r29,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r29));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r28,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r28));
  // mr r5,r30
  r5 = r30;
  // stw r27,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r27));
  // ori r4,r4,24
  r4 = r4 | 24;
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x8257603C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x8257604c
  if (!cr0.lt) goto loc_8257604C;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x8257606c
  goto loc_8257606C;
loc_8257604C:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x82576068
  if (!cr6.eq) goto loc_82576068;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82576058, r31)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x8257606c
  goto loc_8257606C;
loc_82576068:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_8257606C:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x82576074, r31)); r3 = o.r3; f1 = o.f1; }
loc_82576074:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82576130: lifted from generated/ by lift.py.
lifted::Result f82576130(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82576130 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // mr r30,r7
  r30 = r7;
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x82576154, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x825761cc
  if (!cr0.eq) goto loc_825761CC;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,20
  r7 = uint64_t(int64_t(20));
  // stw r29,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r29));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r28,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r28));
  // mr r5,r30
  r5 = r30;
  // stw r27,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r27));
  // ori r4,r4,18
  r4 = r4 | 18;
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82576194, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x825761a4
  if (!cr0.lt) goto loc_825761A4;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x825761c4
  goto loc_825761C4;
loc_825761A4:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825761c0
  if (!cr6.eq) goto loc_825761C0;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x825761B0, r31)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x825761c4
  goto loc_825761C4;
loc_825761C0:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_825761C4:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x825761CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825761CC:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82576280: lifted from generated/ by lift.py.
lifted::Result f82576280(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82576280 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // mr r30,r6
  r30 = r6;
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x825762A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x82576318
  if (!cr0.eq) goto loc_82576318;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,20
  r7 = uint64_t(int64_t(20));
  // stw r29,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r29));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r28,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r28));
  // mr r5,r30
  r5 = r30;
  // ori r4,r4,19
  r4 = r4 | 19;
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x825762E0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x825762f0
  if (!cr0.lt) goto loc_825762F0;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x82576310
  goto loc_82576310;
loc_825762F0:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8257630c
  if (!cr6.eq) goto loc_8257630C;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x825762FC, r31)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x82576310
  goto loc_82576310;
loc_8257630C:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_82576310:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x82576318, r31)); r3 = o.r3; f1 = o.f1; }
loc_82576318:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825767D0: lifted from generated/ by lift.py.
lifted::Result f825767D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825767D0 (lifted)");
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
  // stwu r1,-720(r1)
  ea = -720 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stw r3,740(r1)
  guest::StoreU32(base, uint32_t(r1) + 740, uint32_t(r3));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,136
  r4 = uint64_t(int64_t(int64_t(r1) + 136));
  // li r3,1414
  r3 = uint64_t(int64_t(1414));
  // stw r30,672(r1)
  guest::StoreU32(base, uint32_t(r1) + 672, uint32_t(r30));
  // stw r30,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r30));
  // stw r30,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r30));
  // bl 0x82575b68
  // (left out: dead in C++)
  { const lifted::Result o = f82575B68(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x825768cc
  if (cr0.lt) goto loc_825768CC;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,136(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 136);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r7,r1,132
  r7 = uint64_t(int64_t(int64_t(r1) + 132));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82575bf0
  // (left out: dead in C++)
  r3 = f82575BF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x825768cc
  if (cr0.lt) goto loc_825768CC;
  // addi r5,r1,128
  r5 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r4,132(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 132);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825dbd64
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamAlloc, LIFTED_ARGS(0x82576840, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x825768cc
  if (cr0.lt) goto loc_825768CC;
  // addi r6,r1,144
  r6 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r8,132(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 132);
  // addi r7,r1,148
  r7 = uint64_t(int64_t(int64_t(r1) + 148));
  // lwz r4,136(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 136);
  // addi r11,r1,140
  r11 = uint64_t(int64_t(int64_t(r1) + 140));
  // stw r30,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r30));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r30,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r30));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r6,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r6));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r7,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r7));
  // li r3,1414
  r3 = uint64_t(int64_t(1414));
  // lwz r7,128(r1)
  r7 = guest::LoadU32(base, uint32_t(r1) + 128);
  // stw r11,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r11));
  // bl 0x82575c90
  // (left out: dead in C++)
  { const lifted::Result o = f82575C90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x825768cc
  if (cr0.lt) goto loc_825768CC;
  // addi r4,r1,740
  r4 = uint64_t(int64_t(int64_t(r1) + 740));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // lis r4,5
  r4 = uint64_t(int64_t(327680));
  // addi r6,r1,160
  r6 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r5,140(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 140);
  // ori r4,r4,32803
  r4 = r4 | 32803;
  // li r3,252
  r3 = uint64_t(int64_t(252));
  // bl 0x825dbd44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgInProcessCall, LIFTED_ARGS(0x825768C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge 0x825768ec
  if (!cr0.lt) goto loc_825768EC;
loc_825768CC:
  // lwz r3,128(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 128);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825768dc
  if (cr6.eq) goto loc_825768DC;
  // bl 0x825dbd54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamFree, LIFTED_ARGS(0x825768DC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825768DC:
  // mr r3,r31
  r3 = r31;
  // bl 0x82296fd0
  // (left out: dead in C++)
  { const lifted::Result o = game::SetLastError_82296FD0(base, LIFTED_ARGS(0x825768E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,1627
  r30 = uint64_t(int64_t(1627));
  // b 0x825768f4
  goto loc_825768F4;
loc_825768EC:
  // lwz r3,128(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 128);
  // bl 0x825dbd54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamFree, LIFTED_ARGS(0x825768F4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825768F4:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,720
  r1 = uint64_t(int64_t(int64_t(r1) + 720));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82576900: lifted from generated/ by lift.py.
lifted::Result f82576900(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82576900 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-800(r1)
  ea = -800 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r3,820(r1)
  guest::StoreU32(base, uint32_t(r1) + 820, uint32_t(r3));
  // mr r28,r4
  r28 = r4;
  // stw r5,836(r1)
  guest::StoreU32(base, uint32_t(r1) + 836, uint32_t(r5));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r3,3329
  r3 = uint64_t(int64_t(3329));
  // mr r27,r6
  r27 = r6;
  // mr r26,r7
  r26 = r7;
  // stw r31,720(r1)
  guest::StoreU32(base, uint32_t(r1) + 720, uint32_t(r31));
  // mr r25,r8
  r25 = r8;
  // stw r31,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r31));
  // mr r29,r9
  r29 = r9;
  // stw r31,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r31));
  // bl 0x82575b68
  // (left out: dead in C++)
  { const lifted::Result o = f82575B68(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,128(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r7,r1,132
  r7 = uint64_t(int64_t(int64_t(r1) + 132));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82575bf0
  // (left out: dead in C++)
  r3 = f82575BF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // lwz r11,132(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 132);
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r3,3073
  r3 = uint64_t(int64_t(3073));
  // addi r30,r11,448
  r30 = uint64_t(int64_t(int64_t(r11) + 448));
  // bl 0x82575b68
  // (left out: dead in C++)
  { const lifted::Result o = f82575B68(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,128(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r7,r1,132
  r7 = uint64_t(int64_t(int64_t(r1) + 132));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82575bf0
  // (left out: dead in C++)
  r3 = f82575BF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // lwz r11,132(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 132);
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r3,3585
  r3 = uint64_t(int64_t(3585));
  // add r30,r30,r11
  r30 = r30 + r11;
  // bl 0x82575b68
  // (left out: dead in C++)
  { const lifted::Result o = f82575B68(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,128(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r7,r1,132
  r7 = uint64_t(int64_t(int64_t(r1) + 132));
  // li r6,256
  r6 = uint64_t(int64_t(256));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82575bf0
  // (left out: dead in C++)
  r3 = f82575BF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // lwz r11,132(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 132);
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r3,3601
  r3 = uint64_t(int64_t(3601));
  // add r30,r30,r11
  r30 = r30 + r11;
  // bl 0x82575b68
  // (left out: dead in C++)
  { const lifted::Result o = f82575B68(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,128(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r7,r1,132
  r7 = uint64_t(int64_t(int64_t(r1) + 132));
  // mr r6,r30
  r6 = r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x82575bf0
  // (left out: dead in C++)
  r3 = f82575BF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // addi r5,r1,136
  r5 = uint64_t(int64_t(int64_t(r1) + 136));
  // lwz r4,132(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 132);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825dbd64
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamAlloc, LIFTED_ARGS(0x82576A50, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // mr r3,r28
  r3 = r28;
  // bl 0x825a9958
  // (left out: dead in C++)
  r3 = game::wcslen(base, r3);
  // mr r11,r3
  r11 = r3;
  // addi r6,r1,144
  r6 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r7,136(r1)
  r7 = guest::LoadU32(base, uint32_t(r1) + 136);
  // addi r5,r1,152
  r5 = uint64_t(int64_t(int64_t(r1) + 152));
  // lwz r4,128(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 128);
  // addi r8,r1,140
  r8 = uint64_t(int64_t(int64_t(r1) + 140));
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r31,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r31));
  // mr r10,r26
  r10 = r26;
  // stw r27,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r27));
  // stw r6,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r6));
  // mr r9,r25
  r9 = r25;
  // stw r5,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r5));
  // mr r6,r30
  r6 = r30;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // stw r8,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r8));
  // li r3,3601
  r3 = uint64_t(int64_t(3601));
  // lwz r8,132(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 132);
  // stw r11,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r11));
  // bl 0x82575c90
  // (left out: dead in C++)
  { const lifted::Result o = f82575C90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // addi r4,r1,820
  r4 = uint64_t(int64_t(int64_t(r1) + 820));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // mr r4,r28
  r4 = r28;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // addi r4,r1,836
  r4 = uint64_t(int64_t(int64_t(r1) + 836));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // addi r4,r1,156
  r4 = uint64_t(int64_t(int64_t(r1) + 156));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // addi r4,r1,148
  r4 = uint64_t(int64_t(int64_t(r1) + 148));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // lwz r9,152(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 152);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r5,128(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 128);
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82575d78
  // (left out: dead in C++)
  { const lifted::Result o = f82575D78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576b88
  if (cr0.lt) goto loc_82576B88;
  // lwz r10,144(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 144);
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r9,140(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 140);
  // li r7,40
  r7 = uint64_t(int64_t(40));
  // addi r6,r1,160
  r6 = uint64_t(int64_t(int64_t(r1) + 160));
  // mr r5,r29
  r5 = r29;
  // li r3,252
  r3 = uint64_t(int64_t(252));
  // stw r10,28(r9)
  guest::StoreU32(base, uint32_t(r9) + 28, uint32_t(r10));
  // std r31,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r31);
  // std r31,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r31);
  // lwz r10,128(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 128);
  // std r31,16(r11)
  guest::StoreU64(base, uint32_t(r11) + 16, r31);
  // std r31,24(r11)
  guest::StoreU64(base, uint32_t(r11) + 24, r31);
  // clrlwi r4,r10,16
  r4 = uint32_t(r10) & 0xFFFF;
  // std r31,32(r11)
  guest::StoreU64(base, uint32_t(r11) + 32, r31);
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // lwz r11,140(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 140);
  // rlwimi r4,r10,16,0,15
  r4 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 16) & 0xFFFF0000) | (r4 & 0xFFFFFFFF0000FFFF);
  // stw r11,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82576B80, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82576bb4
  if (!cr0.lt) goto loc_82576BB4;
loc_82576B88:
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x82576b9c
  if (cr6.eq) goto loc_82576B9C;
  // stw r3,24(r29)
  guest::StoreU32(base, uint32_t(r29) + 24, uint32_t(r3));
  // b 0x82576ba0
  goto loc_82576BA0;
loc_82576B9C:
  // bl 0x82296e70
  // (left out: dead in C++)
  { const lifted::Result o = game::SetLastError_82296E70(base, LIFTED_ARGS(0x82576BA0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82576BA0:
  // lwz r3,136(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 136);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82576bd4
  if (cr6.eq) goto loc_82576BD4;
  // bl 0x825dbd54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamFree, LIFTED_ARGS(0x82576BB0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82576bd4
  goto loc_82576BD4;
loc_82576BB4:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x82576bc4
  if (cr6.eq) goto loc_82576BC4;
  // li r31,997
  r31 = uint64_t(int64_t(997));
  // b 0x82576bd4
  goto loc_82576BD4;
loc_82576BC4:
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82576BC8, r31)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
loc_82576BD4:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,800
  r1 = uint64_t(int64_t(int64_t(r1) + 800));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82576EE0: lifted from generated/ by lift.py.
lifted::Result f82576EE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82576EE0 (lifted)");
  uint64_t r6 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // mr r10,r4
  r10 = r4;
  // lis r4,5
  r4 = uint64_t(int64_t(327680));
  // mr r6,r5
  r6 = r5;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // ori r4,r4,32782
  r4 = r4 | 32782;
  // std r10,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r10);
  // li r3,252
  r3 = uint64_t(int64_t(252));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x825dbd44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgInProcessCall, LIFTED_ARGS(0x82576F1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,96(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 96);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82576FE8: lifted from generated/ by lift.py.
lifted::Result f82576FE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82576FE8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r10,r4
  r10 = r4;
  // stw r7,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r7));
  // mr r11,r3
  r11 = r3;
  // stw r8,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r8));
  // mr r31,r5
  r31 = r5;
  // stw r9,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r9));
  // mr r30,r6
  r30 = r6;
  // li r29,254
  r29 = uint64_t(int64_t(254));
  // lis r4,5
  r4 = uint64_t(int64_t(327680));
  // stw r10,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r10));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // std r11,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r11);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r31,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r31));
  // ori r4,r4,32821
  r4 = r4 | 32821;
  // stw r30,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r30));
  // li r3,252
  r3 = uint64_t(int64_t(252));
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x825dbd44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgInProcessCall, LIFTED_ARGS(0x82577040, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577050
  if (cr0.lt) goto loc_82577050;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82577080
  goto loc_82577080;
loc_82577050:
  // lis r11,-32761
  r11 = uint64_t(int64_t(-2147024896));
  // ori r11,r11,87
  r11 = r11 | 87;
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // bne cr6,0x82577068
  if (!cr6.eq) goto loc_82577068;
  // li r3,87
  r3 = uint64_t(int64_t(87));
  // b 0x82577080
  goto loc_82577080;
loc_82577068:
  // lis r11,-32761
  r11 = uint64_t(int64_t(-2147024896));
  // ori r11,r11,122
  r11 = r11 | 122;
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // li r3,122
  r3 = uint64_t(int64_t(122));
  // beq cr6,0x82577080
  if (cr6.eq) goto loc_82577080;
  // li r3,1627
  r3 = uint64_t(int64_t(1627));
loc_82577080:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258B7E0: lifted from generated/ by lift.py.
uint64_t f8258B7E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258B7E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Vec v0{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{};
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // lwz r11,116(r27)
  r11 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).vertices_address());
  // lwz r4,0(r27)
  r4 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).base_stream_address());
  // rlwinm r22,r11,5,0,26
  r22 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // lwz r3,8(r27)
  r3 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).out1_address());
  // rlwinm r23,r11,4,0,27
  r23 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // mr r5,r23
  r5 = r23;
  // stw r22,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r22));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8258B810, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r22
  r5 = r22;
  // lwz r4,4(r27)
  r4 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).base_stream2_address());
  // lwz r3,12(r27)
  r3 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).out2_address());
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8258B820, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,112(r27)
  r11 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).count_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258b924
  if (cr6.eq) goto loc_8258B924;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r26,116(r27)
  r26 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).vertices_address());
  // lwz r29,8(r27)
  r29 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).out1_address());
  // addi r30,r27,80
  r30 = uint64_t(int64_t(int64_t(r27) + 80));
  // lwz r25,4(r27)
  r25 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).base_stream2_address());
  // mr r24,r11
  r24 = r11;
  // lwz r28,12(r27)
  r28 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).out2_address());
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
loc_8258B850:
  // lwz r7,-32(r30)
  r7 = guest::LoadU32(base, schema::Morph(base, uint32_t(r30) - 80).target_second0_address());
  // mr r11,r28
  r11 = r28;
  // lfs f0,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Morph(base, uint32_t(r30) - 80).weight0_address());
  f0 = double(temp.f32);
  // mr r10,r29
  r10 = r29;
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // addi r6,r25,16
  r6 = uint64_t(int64_t(int64_t(r25) + 16));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // addi r9,r11,16
  r9 = uint64_t(int64_t(int64_t(r11) + 16));
  // addi r8,r7,16
  r8 = uint64_t(int64_t(int64_t(r7) + 16));
  // beq cr6,0x8258b914
  if (cr6.eq) goto loc_8258B914;
  // addi r21,r1,96
  r21 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r4,0(r27)
  r4 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).base_stream_address());
  // lwz r3,-64(r30)
  r3 = guest::LoadU32(base, schema::Morph(base, uint32_t(r30) - 80).target0_address());
  // subf r31,r28,r7
  r31 = r7 - r28;
  // subf r5,r28,r25
  r5 = r25 - r28;
  // subf r4,r29,r4
  r4 = r4 - r29;
  // subf r3,r29,r3
  r3 = r3 - r29;
  // lvx128 v0,r0,r21
  ea = (uint32_t(r21)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // mr r7,r26
  r7 = r26;
loc_8258B8A4:
  // lvx128 v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // lvx128 v13,r0,r6
  ea = (uint32_t(r6)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r6,r6,32
  r6 = uint64_t(int64_t(int64_t(r6) + 32));
  // vsubfp v13,v12,v13
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_sub_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v13.f32)));
  // lvx128 v11,r4,r10
  ea = (uint32_t(r4) + uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v12,r3,r10
  ea = (uint32_t(r3) + uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // vsubfp v12,v12,v11
  simde_mm_store_ps(v12.f32, simde_mm_sub_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v11.f32)));
  // lvx128 v10,r5,r11
  ea = (uint32_t(r5) + uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v11,r31,r11
  ea = (uint32_t(r31) + uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vsubfp v11,v11,v10
  simde_mm_store_ps(v11.f32, simde_mm_sub_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v10.f32)));
  // lvx128 v10,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v9,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v8,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v8.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f0,12(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::MorphStream(base, uint32_t(r8) - 16).unknown_28_address());
  f0 = double(temp.f32);
  // addi r8,r8,32
  r8 = uint64_t(int64_t(int64_t(r8) + 32));
  // vmaddfp v13,v13,v0,v10
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v0.f32)), simde_mm_load_ps(v10.f32)));
  // vmaddfp v12,v12,v0,v9
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)), simde_mm_load_ps(v9.f32)));
  // vmaddfp v11,v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v0.f32)), simde_mm_load_ps(v8.f32)));
  // stvx v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stfs f0,12(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::MorphStream(base, uint32_t(r9) - 16).unknown_28_address(), temp.u32);
  // addi r9,r9,32
  r9 = uint64_t(int64_t(int64_t(r9) + 32));
  // stvx v12,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne cr6,0x8258b8a4
  if (!cr6.eq) goto loc_8258B8A4;
loc_8258B914:
  // addi r24,r24,-1
  r24 = uint64_t(int64_t(int64_t(r24) + -1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x8258b850
  if (!cr6.eq) goto loc_8258B850;
loc_8258B924:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // beq cr6,0x8258b94c
  if (cr6.eq) goto loc_8258B94C;
  // lwz r10,8(r27)
  r10 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).out1_address());
loc_8258B938:
  // addi r11,r11,128
  r11 = uint64_t(int64_t(int64_t(r11) + 128));
  // cmplw cr6,r11,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r23), so);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // blt cr6,0x8258b938
  if (cr6.lt) goto loc_8258B938;
loc_8258B94C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmplwi cr6,r22,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r22), 0, so);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // beq cr6,0x8258b974
  if (cr6.eq) goto loc_8258B974;
  // lwz r10,12(r27)
  r10 = guest::LoadU32(base, schema::Morph(base, uint32_t(r27)).out2_address());
loc_8258B960:
  // addi r11,r11,128
  r11 = uint64_t(int64_t(int64_t(r11) + 128));
  // cmplw cr6,r11,r22
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r22), so);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // blt cr6,0x8258b960
  if (cr6.lt) goto loc_8258B960;
loc_8258B974:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return r3;
}

// sub_82590D18: lifted from generated/ by lift.py.
lifted::Result f82590D18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82590D18 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
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
  // subf r11,r3,r4
  r11 = r4 - r3;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // subf r31,r6,r5
  r31 = r5 - r6;
  // ble cr6,0x82590d50
  if (!cr6.gt) goto loc_82590D50;
  // mr r5,r3
  r5 = r3;
  // mr r4,r6
  r4 = r6;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82590D50, r31)); r3 = o.r3; f1 = o.f1; }
loc_82590D50:
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

// sub_82592A20: lifted from generated/ by lift.py.
lifted::Result f82592A20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82592A20 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r3
  r26 = r3;
  // mr r31,r4
  r31 = r4;
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // mr r24,r7
  r24 = r7;
  // lwz r11,20(r26)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).length_address());
  // mr r30,r8
  r30 = r8;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // blt cr6,0x82592a5c
  if (cr6.lt) goto loc_82592A5C;
  // lwz r11,20(r27)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r27)).length_address());
  // cmplw cr6,r11,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r24), so);
  // bge cr6,0x82592a60
  if (!cr6.lt) goto loc_82592A60;
loc_82592A5C:
  // bl 0x825a6b18
  // (left out: dead in C++)
  { const lifted::Result o = f825A6B18(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82592A60:
  // lwz r10,20(r26)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).length_address());
  // subf r11,r31,r10
  r11 = r10 - r31;
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // bge cr6,0x82592a74
  if (!cr6.lt) goto loc_82592A74;
  // mr r28,r11
  r28 = r11;
loc_82592A74:
  // lwz r11,20(r27)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r27)).length_address());
  // subf r11,r24,r11
  r11 = r11 - r24;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bge cr6,0x82592a88
  if (!cr6.lt) goto loc_82592A88;
  // mr r30,r11
  r30 = r11;
loc_82592A88:
  // subf r11,r28,r10
  r11 = r10 - r28;
  // subfic r10,r30,-1
  ca = uint32_t(r30) <= 4294967295;
  r10 = static_cast<uint64_t>(-1) - r30;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bgt cr6,0x82592a9c
  if (cr6.gt) goto loc_82592A9C;
  // bl 0x825a6a20
  // (left out: dead in C++)
  { const lifted::Result o = f825A6A20(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82592A9C:
  // lwz r11,20(r26)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).length_address());
  // subf r10,r28,r11
  r10 = r11 - r28;
  // subf r9,r31,r11
  r9 = r11 - r31;
  // add r23,r10,r30
  r23 = r10 + r30;
  // subf r25,r28,r9
  r25 = r9 - r28;
  // cmplw cr6,r11,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r23), so);
  // bge cr6,0x82592ac8
  if (!cr6.lt) goto loc_82592AC8;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r23
  r4 = r23;
  // mr r3,r26
  r3 = r26;
  // bl 0x82091540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091540, LIFTED_ARGS(0x82592AC8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82592AC8:
  // lwz r9,24(r26)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).capacity_address());
  // cmplw cr6,r26,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r27), so);
  // addi r29,r26,4
  r29 = uint64_t(int64_t(int64_t(r26) + 4));
  // beq cr6,0x82592b68
  if (cr6.eq) goto loc_82592B68;
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592ae8
  if (cr6.lt) goto loc_82592AE8;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592aec
  goto loc_82592AEC;
loc_82592AE8:
  // mr r10,r29
  r10 = r29;
loc_82592AEC:
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592afc
  if (cr6.lt) goto loc_82592AFC;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592b00
  goto loc_82592B00;
loc_82592AFC:
  // mr r11,r29
  r11 = r29;
loc_82592B00:
  // add r10,r10,r31
  r10 = r10 + r31;
  // subf r9,r31,r9
  r9 = r9 - r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // mr r6,r25
  r6 = r25;
  // add r5,r10,r28
  r5 = r10 + r28;
  // subf r4,r30,r9
  r4 = r9 - r30;
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592B20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r27)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r27)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592b34
  if (cr6.lt) goto loc_82592B34;
  // lwz r9,4(r27)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r27)).block_address());
  // b 0x82592b38
  goto loc_82592B38;
loc_82592B34:
  // addi r9,r27,4
  r9 = uint64_t(int64_t(int64_t(r27) + 4));
loc_82592B38:
  // lwz r10,24(r26)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).capacity_address());
  // cmplwi cr6,r10,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 16, so);
  // blt cr6,0x82592b4c
  if (cr6.lt) goto loc_82592B4C;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592b50
  goto loc_82592B50;
loc_82592B4C:
  // mr r11,r29
  r11 = r29;
loc_82592B50:
  // mr r6,r30
  r6 = r30;
  // add r5,r9,r24
  r5 = r9 + r24;
  // subf r4,r31,r10
  r4 = r10 - r31;
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x82592B64, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82592df0
  goto loc_82592DF0;
loc_82592B68:
  // cmplw cr6,r30,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r28), so);
  // bgt cr6,0x82592bf8
  if (cr6.gt) goto loc_82592BF8;
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592b80
  if (cr6.lt) goto loc_82592B80;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592b84
  goto loc_82592B84;
loc_82592B80:
  // mr r10,r29
  r10 = r29;
loc_82592B84:
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592b94
  if (cr6.lt) goto loc_82592B94;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592b98
  goto loc_82592B98;
loc_82592B94:
  // mr r11,r29
  r11 = r29;
loc_82592B98:
  // mr r6,r30
  r6 = r30;
  // add r5,r10,r24
  r5 = r10 + r24;
  // subf r4,r31,r9
  r4 = r9 - r31;
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592BAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r26)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592bc0
  if (cr6.lt) goto loc_82592BC0;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592bc4
  goto loc_82592BC4;
loc_82592BC0:
  // mr r10,r29
  r10 = r29;
loc_82592BC4:
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592bd4
  if (cr6.lt) goto loc_82592BD4;
  // lwz r9,0(r29)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592bd8
  goto loc_82592BD8;
loc_82592BD4:
  // mr r9,r29
  r9 = r29;
loc_82592BD8:
  // subf r8,r31,r11
  r8 = r11 - r31;
  // add r10,r10,r31
  r10 = r10 + r31;
  // add r11,r9,r31
  r11 = r9 + r31;
  // mr r6,r25
  r6 = r25;
  // add r5,r10,r28
  r5 = r10 + r28;
  // subf r4,r30,r8
  r4 = r8 - r30;
  // add r3,r11,r30
  r3 = r11 + r30;
  // b 0x82592dec
  goto loc_82592DEC;
loc_82592BF8:
  // cmplw cr6,r24,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r31), so);
  // bgt cr6,0x82592c88
  if (cr6.gt) goto loc_82592C88;
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592c10
  if (cr6.lt) goto loc_82592C10;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592c14
  goto loc_82592C14;
loc_82592C10:
  // mr r10,r29
  r10 = r29;
loc_82592C14:
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592c24
  if (cr6.lt) goto loc_82592C24;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592c28
  goto loc_82592C28;
loc_82592C24:
  // mr r11,r29
  r11 = r29;
loc_82592C28:
  // add r10,r10,r31
  r10 = r10 + r31;
  // subf r9,r31,r9
  r9 = r9 - r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // mr r6,r25
  r6 = r25;
  // add r5,r10,r28
  r5 = r10 + r28;
  // subf r4,r30,r9
  r4 = r9 - r30;
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592C48, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r26)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592c5c
  if (cr6.lt) goto loc_82592C5C;
  // lwz r9,0(r29)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592c60
  goto loc_82592C60;
loc_82592C5C:
  // mr r9,r29
  r9 = r29;
loc_82592C60:
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592c70
  if (cr6.lt) goto loc_82592C70;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592c74
  goto loc_82592C74;
loc_82592C70:
  // mr r10,r29
  r10 = r29;
loc_82592C74:
  // mr r6,r30
  r6 = r30;
  // add r5,r9,r24
  r5 = r9 + r24;
  // subf r4,r31,r11
  r4 = r11 - r31;
  // add r3,r10,r31
  r3 = r10 + r31;
  // b 0x82592dec
  goto loc_82592DEC;
loc_82592C88:
  // add r11,r31,r28
  r11 = r31 + r28;
  // cmplw cr6,r11,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r24), so);
  // bgt cr6,0x82592d1c
  if (cr6.gt) goto loc_82592D1C;
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592ca4
  if (cr6.lt) goto loc_82592CA4;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592ca8
  goto loc_82592CA8;
loc_82592CA4:
  // mr r10,r29
  r10 = r29;
loc_82592CA8:
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592cb8
  if (cr6.lt) goto loc_82592CB8;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592cbc
  goto loc_82592CBC;
loc_82592CB8:
  // mr r11,r29
  r11 = r29;
loc_82592CBC:
  // add r10,r10,r31
  r10 = r10 + r31;
  // subf r9,r31,r9
  r9 = r9 - r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // mr r6,r25
  r6 = r25;
  // add r5,r10,r28
  r5 = r10 + r28;
  // subf r4,r30,r9
  r4 = r9 - r30;
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592CDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r26)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592cf0
  if (cr6.lt) goto loc_82592CF0;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592cf4
  goto loc_82592CF4;
loc_82592CF0:
  // mr r10,r29
  r10 = r29;
loc_82592CF4:
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592d04
  if (cr6.lt) goto loc_82592D04;
  // lwz r9,0(r29)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592d08
  goto loc_82592D08;
loc_82592D04:
  // mr r9,r29
  r9 = r29;
loc_82592D08:
  // subf r10,r28,r10
  r10 = r10 - r28;
  // mr r6,r30
  r6 = r30;
  // subf r4,r31,r11
  r4 = r11 - r31;
  // add r3,r9,r31
  r3 = r9 + r31;
  // b 0x82592de4
  goto loc_82592DE4;
loc_82592D1C:
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592d2c
  if (cr6.lt) goto loc_82592D2C;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592d30
  goto loc_82592D30;
loc_82592D2C:
  // mr r10,r29
  r10 = r29;
loc_82592D30:
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592d40
  if (cr6.lt) goto loc_82592D40;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592d44
  goto loc_82592D44;
loc_82592D40:
  // mr r11,r29
  r11 = r29;
loc_82592D44:
  // mr r6,r28
  r6 = r28;
  // add r5,r10,r24
  r5 = r10 + r24;
  // subf r4,r31,r9
  r4 = r9 - r31;
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592D58, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r26)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592d6c
  if (cr6.lt) goto loc_82592D6C;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592d70
  goto loc_82592D70;
loc_82592D6C:
  // mr r10,r29
  r10 = r29;
loc_82592D70:
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592d80
  if (cr6.lt) goto loc_82592D80;
  // lwz r9,0(r29)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592d84
  goto loc_82592D84;
loc_82592D80:
  // mr r9,r29
  r9 = r29;
loc_82592D84:
  // subf r8,r31,r11
  r8 = r11 - r31;
  // add r10,r10,r31
  r10 = r10 + r31;
  // add r11,r9,r31
  r11 = r9 + r31;
  // mr r6,r25
  r6 = r25;
  // add r5,r10,r28
  r5 = r10 + r28;
  // subf r4,r30,r8
  r4 = r8 - r30;
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592DA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r26)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592db8
  if (cr6.lt) goto loc_82592DB8;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592dbc
  goto loc_82592DBC;
loc_82592DB8:
  // mr r10,r29
  r10 = r29;
loc_82592DBC:
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592dcc
  if (cr6.lt) goto loc_82592DCC;
  // lwz r9,0(r29)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x82592dd0
  goto loc_82592DD0;
loc_82592DCC:
  // mr r9,r29
  r9 = r29;
loc_82592DD0:
  // subf r8,r31,r11
  r8 = r11 - r31;
  // add r11,r9,r31
  r11 = r9 + r31;
  // subf r6,r28,r30
  r6 = r30 - r28;
  // subf r4,r28,r8
  r4 = r8 - r28;
  // add r3,r11,r28
  r3 = r11 + r28;
loc_82592DE4:
  // add r10,r10,r24
  r10 = r10 + r24;
  // add r5,r10,r30
  r5 = r10 + r30;
loc_82592DEC:
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592DF0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82592DF0:
  // lwz r11,24(r26)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r26)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82592e00
  if (cr6.lt) goto loc_82592E00;
  // lwz r29,0(r29)
  r29 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
loc_82592E00:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r23,20(r26)
  guest::StoreU32(base, schema::StdString(base, uint32_t(r26)).length_address(), uint32_t(r23));
  // mr r3,r26
  r3 = r26;
  // stbx r11,r29,r23
  guest::StoreU8(base, uint32_t(r29) + uint32_t(r23), uint8_t(r11));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82592E18: lifted from generated/ by lift.py.
lifted::Result f82592E18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82592E18 (lifted)");
  uint64_t r9 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // mr r24,r6
  r24 = r6;
  // mr r28,r7
  r28 = r7;
  // lwz r9,24(r29)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).capacity_address());
  // addi r25,r29,4
  r25 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592e50
  if (cr6.lt) goto loc_82592E50;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82592e54
  goto loc_82592E54;
loc_82592E50:
  // mr r11,r25
  r11 = r25;
loc_82592E54:
  // cmplw cr6,r24,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r11), so);
  // blt cr6,0x82592e84
  if (cr6.lt) goto loc_82592E84;
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592e6c
  if (cr6.lt) goto loc_82592E6C;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82592e70
  goto loc_82592E70;
loc_82592E6C:
  // mr r11,r25
  r11 = r25;
loc_82592E70:
  // lwz r10,20(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).length_address());
  // add r11,r10,r11
  r11 = r10 + r11;
  // cmplw cr6,r11,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r24), so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // bgt cr6,0x82592e88
  if (cr6.gt) goto loc_82592E88;
loc_82592E84:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82592E88:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82592ec4
  if (cr0.eq) goto loc_82592EC4;
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592ea0
  if (cr6.lt) goto loc_82592EA0;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82592ea4
  goto loc_82592EA4;
loc_82592EA0:
  // mr r11,r25
  r11 = r25;
loc_82592EA4:
  // mr r8,r28
  r8 = r28;
  // subf r7,r11,r24
  r7 = r24 - r11;
  // mr r6,r29
  r6 = r29;
  // mr r5,r30
  r5 = r30;
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x82592a20
  // (left out: dead in C++)
  { const lifted::Result o = f82592A20(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82593038
  goto loc_82593038;
loc_82592EC4:
  // lwz r11,20(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).length_address());
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // bge cr6,0x82592ed4
  if (!cr6.lt) goto loc_82592ED4;
  // bl 0x825a6b18
  // (left out: dead in C++)
  { const lifted::Result o = f825A6B18(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82592ED4:
  // lwz r10,20(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).length_address());
  // subf r11,r31,r10
  r11 = r10 - r31;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bge cr6,0x82592ee8
  if (!cr6.lt) goto loc_82592EE8;
  // mr r30,r11
  r30 = r11;
loc_82592EE8:
  // subf r11,r30,r10
  r11 = r10 - r30;
  // subfic r10,r28,-1
  ca = uint32_t(r28) <= 4294967295;
  r10 = static_cast<uint64_t>(-1) - r28;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bgt cr6,0x82592efc
  if (cr6.gt) goto loc_82592EFC;
  // bl 0x825a6a20
  // (left out: dead in C++)
  { const lifted::Result o = f825A6A20(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82592EFC:
  // lwz r11,20(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).length_address());
  // cmplw cr6,r28,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r30), so);
  // subf r11,r31,r11
  r11 = r11 - r31;
  // subf r27,r30,r11
  r27 = r11 - r30;
  // bge cr6,0x82592f5c
  if (!cr6.lt) goto loc_82592F5C;
  // lwz r9,24(r29)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).capacity_address());
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592f24
  if (cr6.lt) goto loc_82592F24;
  // lwz r10,0(r25)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82592f28
  goto loc_82592F28;
loc_82592F24:
  // mr r10,r25
  r10 = r25;
loc_82592F28:
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592f38
  if (cr6.lt) goto loc_82592F38;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82592f3c
  goto loc_82592F3C;
loc_82592F38:
  // mr r11,r25
  r11 = r25;
loc_82592F3C:
  // add r10,r10,r31
  r10 = r10 + r31;
  // subf r9,r31,r9
  r9 = r9 - r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // mr r6,r27
  r6 = r27;
  // add r5,r10,r30
  r5 = r10 + r30;
  // subf r4,r28,r9
  r4 = r9 - r28;
  // add r3,r11,r28
  r3 = r11 + r28;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592F5C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82592F5C:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x82592f6c
  if (!cr6.eq) goto loc_82592F6C;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82593034
  if (cr6.eq) goto loc_82593034;
loc_82592F6C:
  // lwz r10,20(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).length_address());
  // subf r11,r30,r28
  r11 = r28 - r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // add r26,r11,r10
  r26 = r11 + r10;
  // mr r3,r29
  r3 = r29;
  // mr r4,r26
  r4 = r26;
  // bl 0x82091540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091540, LIFTED_ARGS(0x82592F88, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82593034
  if (cr0.eq) goto loc_82593034;
  // cmplw cr6,r30,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r28), so);
  // bge cr6,0x82592fe4
  if (!cr6.lt) goto loc_82592FE4;
  // lwz r9,24(r29)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).capacity_address());
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592fac
  if (cr6.lt) goto loc_82592FAC;
  // lwz r10,0(r25)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82592fb0
  goto loc_82592FB0;
loc_82592FAC:
  // mr r10,r25
  r10 = r25;
loc_82592FB0:
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x82592fc0
  if (cr6.lt) goto loc_82592FC0;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82592fc4
  goto loc_82592FC4;
loc_82592FC0:
  // mr r11,r25
  r11 = r25;
loc_82592FC4:
  // add r10,r10,r31
  r10 = r10 + r31;
  // subf r9,r31,r9
  r9 = r9 - r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // mr r6,r27
  r6 = r27;
  // add r5,r10,r30
  r5 = r10 + r30;
  // subf r4,r28,r9
  r4 = r9 - r28;
  // add r3,r11,r28
  r3 = r11 + r28;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82592FE4, r31)); r3 = o.r3; f1 = o.f1; }
loc_82592FE4:
  // lwz r10,24(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).capacity_address());
  // cmplwi cr6,r10,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 16, so);
  // blt cr6,0x82592ff8
  if (cr6.lt) goto loc_82592FF8;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82592ffc
  goto loc_82592FFC;
loc_82592FF8:
  // mr r11,r25
  r11 = r25;
loc_82592FFC:
  // mr r6,r28
  r6 = r28;
  // mr r5,r24
  r5 = r24;
  // subf r4,r31,r10
  r4 = r10 - r31;
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x82593010, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82593024
  if (cr6.lt) goto loc_82593024;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r25) - 4).block_address());
  // b 0x82593028
  goto loc_82593028;
loc_82593024:
  // mr r11,r25
  r11 = r25;
loc_82593028:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r26,20(r29)
  guest::StoreU32(base, schema::StdString(base, uint32_t(r29)).length_address(), uint32_t(r26));
  // stbx r10,r11,r26
  guest::StoreU8(base, uint32_t(r11) + uint32_t(r26), uint8_t(r10));
loc_82593034:
  // mr r3,r29
  r3 = r29;
loc_82593038:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825981C8: lifted from generated/ by lift.py.
lifted::Result f825981C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825981C8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // addi r4,r11,3416
  r4 = uint64_t(int64_t(int64_t(r11) + 3416));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // stb r11,84(r1)
  guest::StoreU8(base, uint32_t(r1) + 84, uint8_t(r11));
  // bl 0x82091808
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82091808(base, LIFTED_ARGS(0x82598200, r31)); r3 = o.r3; f1 = o.f1; }
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
  r10 = guest::LoadU32(base, uint32_t(r11) + 4104);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8259822c
  if (cr6.eq) goto loc_8259822C;
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259822C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8259822C:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::StdThrownObjectVtable(base, uint32_t(r11)).slot2_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82598240, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a69d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Std::_Throw(base, LIFTED_ARGS(0x82598248, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x820915f8
  // (left out: dead in C++)
  { const lifted::Result o = f820915F8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 104);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82598270
  if (cr6.lt) goto loc_82598270;
  // lwz r3,84(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82598270, r31)); r3 = o.r3; f1 = o.f1; }
loc_82598270:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825A1148: lifted from generated/ by lift.py.
void f825A1148(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825A1148 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::TrackSet(base, uint32_t(r3)).section_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // lwz r10,16(r3)
  r10 = guest::LoadU32(base, schema::TrackSet(base, uint32_t(r3)).values_address());
  // rlwinm r11,r5,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 5) & 0xFFFFFFE0;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,20(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 20);
  // clrlwi r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825a1178
  if (cr6.eq) goto loc_825A1178;
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r4) + 0, temp.u32);
loc_825A1178:
  // lwz r10,20(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 20);
  // rlwinm r10,r10,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x2;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825a1190
  if (cr6.eq) goto loc_825A1190;
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f0 = double(temp.f32);
  // stfs f0,4(r4)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r4) + 4, temp.u32);
loc_825A1190:
  // lwz r10,20(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 20);
  // rlwinm r10,r10,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825a11a8
  if (cr6.eq) goto loc_825A11A8;
  // lfs f0,8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8);
  f0 = double(temp.f32);
  // stfs f0,8(r4)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r4) + 8, temp.u32);
loc_825A11A8:
  // lwz r10,20(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 20);
  // rlwinm r10,r10,0,28,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x8;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // lfs f13,12(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 12);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,12(r4)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r4) + 12, temp.u32);
  // blr
  return;
}

// sub_825A1938: lifted from generated/ by lift.py.
uint64_t f825A1938(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A1938 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::TrackSetAt384(base, uint32_t(r3)).unknown_0_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825a1a20
  if (cr6.eq) goto loc_825A1A20;
  // lwz r10,16(r3)
  r10 = guest::LoadU32(base, schema::TrackSetAt384(base, uint32_t(r3)).unknown_16_address());
  // mulli r11,r4,208
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(208))));
  // add r31,r11,r10
  r31 = r11 + r10;
  // bl 0x825a6648
  // (left out: dead in C++)
  r3 = f825A6648(base, r3, r4);
  // addi r11,r3,64
  r11 = uint64_t(int64_t(int64_t(r3) + 64));
  // addi r10,r3,80
  r10 = uint64_t(int64_t(int64_t(r3) + 80));
  // addi r9,r3,96
  r9 = uint64_t(int64_t(int64_t(r3) + 96));
  // addi r8,r3,112
  r8 = uint64_t(int64_t(int64_t(r3) + 112));
  // addi r7,r3,128
  r7 = uint64_t(int64_t(int64_t(r3) + 128));
  // addi r6,r3,144
  r6 = uint64_t(int64_t(int64_t(r3) + 144));
  // ld r4,0(r11)
  r4 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r11) - 64).unknown_64_address());
  // addi r5,r3,160
  r5 = uint64_t(int64_t(int64_t(r3) + 160));
  // std r4,64(r31)
  guest::StoreU64(base, uint32_t(r31) + 64, r4);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r11) - 64).unknown_72_address());
  // std r11,72(r31)
  guest::StoreU64(base, uint32_t(r31) + 72, r11);
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r10) - 80).unknown_80_address());
  // std r11,80(r31)
  guest::StoreU64(base, uint32_t(r31) + 80, r11);
  // ld r11,8(r10)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r10) - 80).unknown_88_address());
  // std r11,88(r31)
  guest::StoreU64(base, uint32_t(r31) + 88, r11);
  // ld r11,0(r9)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r9) - 96).unknown_96_address());
  // std r11,96(r31)
  guest::StoreU64(base, uint32_t(r31) + 96, r11);
  // ld r11,8(r9)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r9) - 96).unknown_104_address());
  // std r11,104(r31)
  guest::StoreU64(base, uint32_t(r31) + 104, r11);
  // ld r11,0(r8)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r8) - 112).unknown_112_address());
  // std r11,112(r31)
  guest::StoreU64(base, uint32_t(r31) + 112, r11);
  // ld r11,8(r8)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r8) - 112).unknown_120_address());
  // std r11,120(r31)
  guest::StoreU64(base, uint32_t(r31) + 120, r11);
  // ld r11,0(r7)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r7) - 128).unknown_128_address());
  // std r11,128(r31)
  guest::StoreU64(base, uint32_t(r31) + 128, r11);
  // ld r11,8(r7)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r7) - 128).unknown_136_address());
  // std r11,136(r31)
  guest::StoreU64(base, uint32_t(r31) + 136, r11);
  // ld r11,0(r6)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r6) - 144).unknown_144_address());
  // std r11,144(r31)
  guest::StoreU64(base, uint32_t(r31) + 144, r11);
  // ld r11,8(r6)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r6) - 144).unknown_152_address());
  // std r11,152(r31)
  guest::StoreU64(base, uint32_t(r31) + 152, r11);
  // ld r11,0(r5)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r5) - 160).unknown_160_address());
  // std r11,160(r31)
  guest::StoreU64(base, uint32_t(r31) + 160, r11);
  // ld r11,8(r5)
  r11 = guest::LoadU64(base, schema::AnimationTrack208(base, uint32_t(r5) - 160).unknown_168_address());
  // std r11,168(r31)
  guest::StoreU64(base, uint32_t(r31) + 168, r11);
  // lfs f0,176(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AnimationTrack208(base, uint32_t(r3)).unknown_176_address());
  f0 = double(temp.f32);
  // stfs f0,176(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 176, temp.u32);
  // lfs f0,180(r3)
  temp.u32 = guest::LoadU32(base, schema::AnimationTrack208(base, uint32_t(r3)).unknown_180_address());
  f0 = double(temp.f32);
  // stfs f0,180(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 180, temp.u32);
  // lfs f0,184(r3)
  temp.u32 = guest::LoadU32(base, schema::AnimationTrack208(base, uint32_t(r3)).unknown_184_address());
  f0 = double(temp.f32);
  // stfs f0,184(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 184, temp.u32);
  // lfs f0,188(r3)
  temp.u32 = guest::LoadU32(base, schema::AnimationTrack208(base, uint32_t(r3)).unknown_188_address());
  f0 = double(temp.f32);
  // stfs f0,188(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 188, temp.u32);
  // lfs f0,212(r3)
  temp.u32 = guest::LoadU32(base, schema::AnimationTrack208(base, uint32_t(r3)).unknown_212_address());
  f0 = double(temp.f32);
  // stfs f0,192(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 192, temp.u32);
  // lwz r11,216(r3)
  r11 = guest::LoadU32(base, schema::AnimationTrack208(base, uint32_t(r3)).unknown_216_address());
  // stw r11,200(r31)
  guest::StoreU32(base, uint32_t(r31) + 200, uint32_t(r11));
loc_825A1A20:
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

// sub_825A6608: lifted from generated/ by lift.py.
double f825A6608(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A6608 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::TrackSetAt384(base, uint32_t(r3)).unknown_0_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825a6620
  if (!cr6.eq) goto loc_825A6620;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // blr
  return f1;
loc_825A6620:
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // bne cr6,0x825a6630
  if (!cr6.eq) goto loc_825A6630;
  // lfs f1,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f1 = double(temp.f32);
  // blr
  return f1;
loc_825A6630:
  // lwz r10,8(r3)
  r10 = guest::LoadU32(base, schema::TrackSetAt384(base, uint32_t(r3)).unknown_8_address());
  // mulli r11,r4,352
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(352))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // lfs f1,204(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 204);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_825A6648: lifted from generated/ by lift.py.
uint64_t f825A6648(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A6648 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::TrackSetAt384(base, uint32_t(r3)).unknown_0_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825a665c
  if (!cr6.eq) goto loc_825A665C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_825A665C:
  // lwz r10,8(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 8);
  // mulli r11,r4,352
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(352))));
  // add r3,r11,r10
  r3 = r11 + r10;
  // blr
  return r3;
}

// sub_825A67A8: lifted from generated/ by lift.py.
lifted::Result f825A67A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825A67A8 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r3
  r30 = r3;
  // addi r11,r11,3264
  r11 = uint64_t(int64_t(int64_t(r11) + 3264));
  // mr r29,r4
  r29 = r4;
  // stw r11,0(r30)
  guest::StoreU32(base, schema::StdException(base, uint32_t(r30)).vtable_address(), uint32_t(r11));
  // lwz r11,8(r29)
  r11 = guest::LoadU32(base, schema::StdException(base, uint32_t(r29)).owns_what_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,8(r30)
  guest::StoreU32(base, schema::StdException(base, uint32_t(r30)).owns_what_address(), uint32_t(r11));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::StdException(base, uint32_t(r29)).what_address());
  // beq 0x825a6830
  if (cr0.eq) goto loc_825A6830;
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x825a682c
  if (cr0.eq) goto loc_825A682C;
  // mr r10,r11
  r10 = r11;
loc_825A67E8:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, schema::ExceptionMessage(base, uint32_t(r11)).first_char_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x825a67e8
  if (!cr6.eq) goto loc_825A67E8;
  // subf r11,r10,r11
  r11 = r11 - r10;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // addi r31,r11,1
  r31 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r3,r31
  r3 = r31;
  // bl 0x825a9a90
  // (left out: dead in C++)
  { const lifted::Result o = game::malloc(base, LIFTED_ARGS(0x825A6810, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,4(r30)
  guest::StoreU32(base, schema::StdException(base, uint32_t(r30)).what_address(), uint32_t(r3));
  // beq 0x825a6834
  if (cr0.eq) goto loc_825A6834;
  // mr r4,r31
  r4 = r31;
  // lwz r5,4(r29)
  r5 = guest::LoadU32(base, schema::StdException(base, uint32_t(r29)).what_address());
  // bl 0x825a9dd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A9DD8, LIFTED_ARGS(0x825A6828, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825a6834
  goto loc_825A6834;
loc_825A682C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825A6830:
  // stw r11,4(r30)
  guest::StoreU32(base, schema::StdException(base, uint32_t(r30)).what_address(), uint32_t(r11));
loc_825A6834:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A6A20: lifted from generated/ by lift.py.
lifted::Result f825A6A20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A6A20 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // addi r31,r1,-176
  r31 = uint64_t(int64_t(int64_t(r1) + -176));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r31,80
  r3 = uint64_t(int64_t(int64_t(r31) + 80));
  // addi r4,r11,8904
  r4 = uint64_t(int64_t(int64_t(r11) + 8904));
  // bl 0x825931d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825931D0(base, LIFTED_ARGS(0x825A6A44, r31)); r3 = o.r3; f1 = o.f1; }
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
  // addi r4,r11,29488
  r4 = uint64_t(int64_t(int64_t(r11) + 29488));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
  // stw r11,112(r31)
  guest::StoreU32(base, uint32_t(r31) + 112, uint32_t(r11));
  // bl 0x825ad198
  // (left out: dead in C++)
  { const lifted::Result o = game::_CxxThrowException(base, LIFTED_ARGS(0x825A6A6C, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825A9EC0: lifted from generated/ by lift.py.
lifted::Result f825A9EC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A9EC0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a9ee0
  if (cr6.eq) goto loc_825A9EE0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x825a9f10
  if (!cr6.eq) goto loc_825A9F10;
loc_825A9EE0:
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825A9EE4, r31)); r3 = o.r3; f1 = o.f1; }
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
  guest::StoreU32(base, schema::ErrnoSlot(base, uint32_t(r11)).value_address(), uint32_t(r10));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::_invalid_parameter(base, LIFTED_ARGS(0x825A9F08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,22
  r3 = uint64_t(int64_t(22));
  // b 0x825a9f90
  goto loc_825A9F90;
loc_825A9F10:
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x825a9f4c
  if (!cr6.eq) goto loc_825A9F4C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // sth r11,0(r3)
  guest::StoreU16(base, uint32_t(r3) + 0, uint16_t(r11));
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825A9F24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,22
  r31 = uint64_t(int64_t(22));
loc_825A9F28:
  // stw r31,0(r3)
  guest::StoreU32(base, schema::ErrnoSlot(base, uint32_t(r3)).value_address(), uint32_t(r31));
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
  { const lifted::Result o = game::_invalid_parameter(base, LIFTED_ARGS(0x825A9F44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // b 0x825a9f90
  goto loc_825A9F90;
loc_825A9F4C:
  // mr r11,r3
  r11 = r3;
loc_825A9F50:
  // lhz r10,0(r5)
  r10 = guest::LoadU16(base, uint32_t(r5) + 0);
  // addi r5,r5,2
  r5 = uint64_t(int64_t(int64_t(r5) + 2));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // sth r10,0(r11)
  guest::StoreU16(base, uint32_t(r11) + 0, uint16_t(r10));
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // beq 0x825a9f70
  if (cr0.eq) goto loc_825A9F70;
  // addic. r4,r4,-1
  ca = uint32_t(r4) > 0;
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne 0x825a9f50
  if (!cr0.eq) goto loc_825A9F50;
loc_825A9F70:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x825a9f8c
  if (!cr6.eq) goto loc_825A9F8C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // sth r11,0(r3)
  guest::StoreU16(base, uint32_t(r3) + 0, uint16_t(r11));
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825A9F84, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,34
  r31 = uint64_t(int64_t(34));
  // b 0x825a9f28
  goto loc_825A9F28;
loc_825A9F8C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825A9F90:
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

// sub_825A9FA8: lifted from generated/ by lift.py.
lifted::Result f825A9FA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825A9FA8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r10,r5
  r10 = r5;
  // mr r4,r6
  r4 = r6;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825aa000
  if (!cr6.eq) goto loc_825AA000;
loc_825A9FD0:
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825A9FD4, r31)); r3 = o.r3; f1 = o.f1; }
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
  guest::StoreU32(base, schema::ErrnoSlot(base, uint32_t(r11)).value_address(), uint32_t(r10));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::_invalid_parameter(base, LIFTED_ARGS(0x825A9FF8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // b 0x825aa044
  goto loc_825AA044;
loc_825AA000:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825a9fd0
  if (cr6.eq) goto loc_825A9FD0;
  // lis r9,32767
  r9 = uint64_t(int64_t(2147418112));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // li r6,73
  r6 = uint64_t(int64_t(73));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // ori r9,r9,65535
  r9 = r9 | 65535;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // stw r6,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r6));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // bgt cr6,0x825aa030
  if (cr6.gt) goto loc_825AA030;
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
loc_825AA030:
  // mr r6,r8
  r6 = r8;
  // mr r5,r7
  r5 = r7;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mtctr r31
  ctr = r31;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825AA044, r31)); r3 = o.r3; f1 = o.f1; }
loc_825AA044:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825AA058: lifted from generated/ by lift.py.
lifted::Result f825AA058(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825AA058 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r6,40(r1)
  guest::StoreU64(base, uint32_t(r1) + 40, r6);
  // std r7,48(r1)
  guest::StoreU64(base, uint32_t(r1) + 48, r7);
  // std r8,56(r1)
  guest::StoreU64(base, uint32_t(r1) + 56, r8);
  // std r9,64(r1)
  guest::StoreU64(base, uint32_t(r1) + 64, r9);
  // std r10,72(r1)
  guest::StoreU64(base, uint32_t(r1) + 72, r10);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r1,136
  r9 = uint64_t(int64_t(int64_t(r1) + 136));
  // mr r6,r5
  r6 = r5;
  // lis r11,-32165
  r11 = uint64_t(int64_t(-2107965440));
  // mr r5,r4
  r5 = r4;
  // mr r4,r3
  r4 = r3;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r9));
  // addi r3,r11,8120
  r3 = uint64_t(int64_t(int64_t(r11) + 8120));
  // lwz r8,80(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x825a9fa8
  // (left out: dead in C++)
  { const lifted::Result o = f825A9FA8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825AB060: lifted from generated/ by lift.py.
uint64_t f825AB060(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825AB060 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // rlwinm r10,r3,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0xFFFFFFFE;
  // addi r11,r11,-17592
  r11 = uint64_t(int64_t(int64_t(r11) + -17592));
  // lwz r11,200(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 200);
  // lhzx r11,r10,r11
  r11 = guest::LoadU16(base, uint32_t(r10) + uint32_t(r11));
  // rlwinm r3,r11,0,24,24
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80;
  // blr
  return r3;
}

// sub_825AB080: lifted from generated/ by lift.py.
uint64_t f825AB080(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825AB080 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // rlwinm r10,r3,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0xFFFFFFFE;
  // addi r11,r11,-17592
  r11 = uint64_t(int64_t(int64_t(r11) + -17592));
  // lwz r11,200(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 200);
  // lhzx r11,r10,r11
  r11 = guest::LoadU16(base, uint32_t(r10) + uint32_t(r11));
  // rlwinm r3,r11,0,28,28
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // blr
  return r3;
}

// sub_825AC108: lifted from generated/ by lift.py.
uint64_t f825AC108(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825AC108 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lwz r11,-16664(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -16664);
  // ori r10,r11,1
  r10 = r11 | 1;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lwz r11,4460(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4460);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // blr
  return r3;
}

// sub_825ACA90: lifted from generated/ by lift.py.
lifted::Result f825ACA90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825ACA90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r27,r3
  r27 = r3;
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 12);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825acbb4
  if (!cr0.eq) goto loc_825ACBB4;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825ACAB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32104
  r11 = uint64_t(int64_t(-2103967744));
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // addi r29,r11,-32224
  r29 = uint64_t(int64_t(int64_t(r11) + -32224));
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // addi r28,r11,-16456
  r28 = uint64_t(int64_t(int64_t(r11) + -16456));
  // beq cr6,0x825acb10
  if (cr6.eq) goto loc_825ACB10;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825ACAD8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -2, so);
  // beq cr6,0x825acb10
  if (cr6.eq) goto loc_825ACB10;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825ACAE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825ACAF4, r31)); r3 = o.r3; f1 = o.f1; }
  // srawi r11,r30,5
  ca = (int32_t(r30) < 0) & ((uint32_t(r30) & 0x1F) != 0);
  r11 = uint64_t(int64_t(int32_t(r30) >> 5));
  // clrlwi r10,r3,27
  r10 = uint32_t(r3) & 0x1F;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mulli r10,r10,44
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(44))));
  // lwzx r11,r11,r29
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r29));
  // add r11,r11,r10
  r11 = r11 + r10;
  // b 0x825acb14
  goto loc_825ACB14;
loc_825ACB10:
  // mr r11,r28
  r11 = r28;
loc_825ACB14:
  // lbz r11,40(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 40);
  // rlwinm. r11,r11,0,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFE;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825acb80
  if (!cr0.eq) goto loc_825ACB80;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825ACB28, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x825acb70
  if (cr6.eq) goto loc_825ACB70;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825ACB38, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -2, so);
  // beq cr6,0x825acb70
  if (cr6.eq) goto loc_825ACB70;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825ACB48, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825ACB54, r31)); r3 = o.r3; f1 = o.f1; }
  // srawi r11,r30,5
  ca = (int32_t(r30) < 0) & ((uint32_t(r30) & 0x1F) != 0);
  r11 = uint64_t(int64_t(int32_t(r30) >> 5));
  // clrlwi r10,r3,27
  r10 = uint32_t(r3) & 0x1F;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mulli r10,r10,44
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(44))));
  // lwzx r11,r11,r29
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r29));
  // add r11,r11,r10
  r11 = r11 + r10;
  // b 0x825acb74
  goto loc_825ACB74;
loc_825ACB70:
  // mr r11,r28
  r11 = r28;
loc_825ACB74:
  // lbz r11,40(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 40);
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825acbb4
  if (cr0.eq) goto loc_825ACBB4;
loc_825ACB80:
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825ACB84, r31)); r3 = o.r3; f1 = o.f1; }
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::_invalid_parameter(base, LIFTED_ARGS(0x825ACBA8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825ACBA8:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_825ACBAC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825ACBB4:
  // cmpwi cr6,r27,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), -1, so);
  // beq cr6,0x825acba8
  if (cr6.eq) goto loc_825ACBA8;
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 12);
  // clrlwi. r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x825acbd8
  if (!cr0.eq) goto loc_825ACBD8;
  // rlwinm. r10,r11,0,24,24
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825acba8
  if (cr0.eq) goto loc_825ACBA8;
  // rlwinm. r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825acba8
  if (!cr0.eq) goto loc_825ACBA8;
loc_825ACBD8:
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825acbec
  if (!cr6.eq) goto loc_825ACBEC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b4790
  // (left out: dead in C++)
  { const lifted::Result o = game::_getbuf(base, LIFTED_ARGS(0x825ACBEC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825ACBEC:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825acc10
  if (!cr6.eq) goto loc_825ACC10;
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x825acba8
  if (!cr6.eq) goto loc_825ACBA8;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_825ACC10:
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 12);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x825acc48
  if (cr0.eq) goto loc_825ACC48;
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // extsb r10,r27
  r10 = uint64_t(int64_t(int8_t(r27)));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // beq cr6,0x825acc4c
  if (cr6.eq) goto loc_825ACC4C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // b 0x825acba8
  goto loc_825ACBA8;
loc_825ACC48:
  // stb r27,0(r11)
  guest::StoreU8(base, uint32_t(r11) + 0, uint8_t(r27));
loc_825ACC4C:
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 12);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // clrlwi r3,r27,24
  r3 = uint32_t(r27) & 0xFF;
  // rlwimi r11,r10,0,31,31
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1) | (r11 & 0xFFFFFFFFFFFFFFFE);
  // rlwimi r11,r10,0,27,27
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x10) | (r11 & 0xFFFFFFFFFFFFFFEF);
  // stw r11,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // b 0x825acbac
  goto loc_825ACBAC;
}

// sub_825AD150: lifted from generated/ by lift.py.
lifted::Result f825AD150(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825AD150 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lwz r11,4464(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4464);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x825ad180
  if (cr0.eq) goto loc_825AD180;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825AD174, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bne 0x825ad184
  if (!cr0.eq) goto loc_825AD184;
loc_825AD180:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825AD184:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825AECC0: lifted from generated/ by lift.py.
lifted::Result f825AECC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825AECC0 (lifted)");
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
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825aed18
  if (cr6.eq) goto loc_825AED18;
  // li r11,-4096
  r11 = uint64_t(int64_t(-4096));
  // twllei r4,0
  if (int32_t(r4) == 0 || uint32_t(r4) < 0u) lifted::Trap(0);
  // divwu r11,r11,r4
  r11 = uint32_t(uint32_t(r4) ? uint32_t(r11) / uint32_t(r4) : 0);
  // cmplw cr6,r11,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r5), so);
  // bge cr6,0x825aed18
  if (!cr6.lt) goto loc_825AED18;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825AECEC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,12
  r10 = uint64_t(int64_t(12));
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::_invalid_parameter(base, LIFTED_ARGS(0x825AED10, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825aed20
  goto loc_825AED20;
loc_825AED18:
  // mullw r4,r4,r5
  r4 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r5))));
  // bl 0x825aebc8
  // (left out: dead in C++)
  { const lifted::Result o = game::realloc(base, LIFTED_ARGS(0x825AED20, r31)); r3 = o.r3; f1 = o.f1; }
loc_825AED20:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825AFEC0: lifted from generated/ by lift.py.
double f825AFEC0(uint8_t* base, uint64_t r1, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_825AFEC0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // stfd f1,-8(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -8, std::bit_cast<uint64_t>(f1));
  // stfd f1,16(r1)
  guest::StoreU64(base, uint32_t(r1) + 16, std::bit_cast<uint64_t>(f1));
  // lhz r10,-8(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + -8);
  // lhz r11,16(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 16);
  // stfd f1,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, std::bit_cast<uint64_t>(f1));
  // rlwinm r11,r11,28,21,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0x7FF;
  // add r11,r11,r4
  r11 = r11 + r4;
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // andi. r10,r10,32783
  r10 = r10 & 32783;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // or r11,r11,r10
  r11 = r11 | r10;
  // sth r11,-16(r1)
  guest::StoreU16(base, uint32_t(r1) + -16, uint16_t(r11));
  // lfd f1,-16(r1)
  f1 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // blr
  return f1;
}

// sub_825AFEF8: lifted from generated/ by lift.py.
uint64_t f825AFEF8(uint8_t* base, uint64_t r1, double f1) {
  ITC_NATIVE_RUNNING("sub_825AFEF8 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // stfd f1,16(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + 16, std::bit_cast<uint64_t>(f1));
  // lis r11,32752
  r11 = uint64_t(int64_t(2146435072));
  // lwz r10,16(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 16);
  // lwz r9,20(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 20);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x825aff20
  if (!cr6.eq) goto loc_825AFF20;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x825aff3c
  if (!cr6.eq) goto loc_825AFF3C;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
loc_825AFF20:
  // lis r11,-16
  r11 = uint64_t(int64_t(-1048576));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x825aff3c
  if (!cr6.eq) goto loc_825AFF3C;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x825aff3c
  if (!cr6.eq) goto loc_825AFF3C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
loc_825AFF3C:
  // lhz r11,16(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 16);
  // rlwinm r11,r11,0,17,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x7FF8;
  // cmplwi cr6,r11,32760
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32760, so);
  // bne cr6,0x825aff54
  if (!cr6.eq) goto loc_825AFF54;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // blr
  return r3;
loc_825AFF54:
  // cmplwi cr6,r11,32752
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32752, so);
  // bne cr6,0x825aff74
  if (!cr6.eq) goto loc_825AFF74;
  // clrlwi. r11,r10,13
  r11 = uint32_t(r10) & 0x7FFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825aff6c
  if (!cr0.eq) goto loc_825AFF6C;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x825aff74
  if (cr6.eq) goto loc_825AFF74;
loc_825AFF6C:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // blr
  return r3;
loc_825AFF74:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825B0068: lifted from generated/ by lift.py.
lifted::Result f825B0068(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825B0068 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r3,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r3));
  // rlwinm. r10,r5,0,27,27
  r10 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // mr r31,r4
  r31 = r4;
  // mr r28,r6
  r28 = r6;
  // mr r29,r7
  r29 = r7;
  // stw r11,4(r3)
  guest::StoreU32(base, schema::FpIeeeRecord(base, uint32_t(r3)).cause_address(), uint32_t(r11));
  // mr r26,r8
  r26 = r8;
  // lwz r10,180(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 180);
  // mr r25,r9
  r25 = r9;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
  // lwz r10,180(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 180);
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // beq 0x825b00c8
  if (cr0.eq) goto loc_825B00C8;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lis r27,-16384
  r27 = uint64_t(int64_t(-1073741824));
  // ori r27,r27,143
  r27 = r27 | 143;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // oris r10,r10,32768
  r10 = r10 | 2147483648;
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // b 0x825b00cc
  goto loc_825B00CC;
loc_825B00C8:
  // lwz r27,80(r1)
  r27 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_825B00CC:
  // rlwinm. r11,r5,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b00ec
  if (cr0.eq) goto loc_825B00EC;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lis r27,-16384
  r27 = uint64_t(int64_t(-1073741824));
  // ori r27,r27,147
  r27 = r27 | 147;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // oris r10,r10,16384
  r10 = r10 | 1073741824;
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_825B00EC:
  // clrlwi. r11,r5,31
  r11 = uint32_t(r5) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b010c
  if (cr0.eq) goto loc_825B010C;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lis r27,-16384
  r27 = uint64_t(int64_t(-1073741824));
  // ori r27,r27,145
  r27 = r27 | 145;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // oris r10,r10,8192
  r10 = r10 | 536870912;
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_825B010C:
  // rlwinm. r11,r5,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b012c
  if (cr0.eq) goto loc_825B012C;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lis r27,-16384
  r27 = uint64_t(int64_t(-1073741824));
  // ori r27,r27,142
  r27 = r27 | 142;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // oris r10,r10,4096
  r10 = r10 | 268435456;
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_825B012C:
  // rlwinm. r11,r5,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b014c
  if (cr0.eq) goto loc_825B014C;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lis r27,-16384
  r27 = uint64_t(int64_t(-1073741824));
  // ori r27,r27,144
  r27 = r27 | 144;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // oris r10,r10,2048
  r10 = r10 | 134217728;
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_825B014C:
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // not r10,r10
  r10 = ~r10;
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // rlwimi r9,r10,20,4,4
  r9 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 20) & 0x8000000) | (r9 & 0xFFFFFFFFF7FFFFFF);
  // stw r9,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r9));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // not r10,r10
  r10 = ~r10;
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // rlwimi r9,r10,24,3,3
  r9 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0x10000000) | (r9 & 0xFFFFFFFFEFFFFFFF);
  // stw r9,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r9));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // not r10,r10
  r10 = ~r10;
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // rlwimi r9,r10,23,2,2
  r9 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 23) & 0x20000000) | (r9 & 0xFFFFFFFFDFFFFFFF);
  // stw r9,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r9));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // not r10,r10
  r10 = ~r10;
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // rlwimi r9,r10,25,1,1
  r9 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 25) & 0x40000000) | (r9 & 0xFFFFFFFFBFFFFFFF);
  // stw r9,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r9));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // not r10,r10
  r10 = ~r10;
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // rlwimi r9,r10,28,0,0
  r9 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 28) & 0x80000000) | (r9 & 0xFFFFFFFF7FFFFFFF);
  // stw r9,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r9));
  // bl 0x825b0bb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B0BB0, LIFTED_ARGS(0x825B01C8, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm. r11,r3,0,2,2
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x20000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b01e0
  if (cr0.eq) goto loc_825B01E0;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // oris r10,r10,2048
  r10 = r10 | 134217728;
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_825B01E0:
  // rlwinm. r11,r3,0,5,5
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x4000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b01f8
  if (cr0.eq) goto loc_825B01F8;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // oris r10,r10,4096
  r10 = r10 | 268435456;
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_825B01F8:
  // rlwinm. r11,r3,0,3,3
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x10000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0210
  if (cr0.eq) goto loc_825B0210;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // oris r10,r10,8192
  r10 = r10 | 536870912;
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_825B0210:
  // rlwinm. r11,r3,0,4,4
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x8000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0228
  if (cr0.eq) goto loc_825B0228;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // oris r10,r10,16384
  r10 = r10 | 1073741824;
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_825B0228:
  // rlwinm. r11,r3,0,6,6
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x2000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0240
  if (cr0.eq) goto loc_825B0240;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // oris r10,r10,32768
  r10 = r10 | 2147483648;
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_825B0240:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // clrlwi r11,r11,30
  r11 = uint32_t(r11) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825b0294
  if (cr6.lt) goto loc_825B0294;
  // beq cr6,0x825b0284
  if (cr6.eq) goto loc_825B0284;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x825b0274
  if (cr6.lt) goto loc_825B0274;
  // bne cr6,0x825b02a4
  if (!cr6.eq) goto loc_825B02A4;
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwimi r10,r30,30,0,1
  r10 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 30) & 0xC0000000) | (r10 & 0xFFFFFFFF3FFFFFFF);
  // b 0x825b02a0
  goto loc_825B02A0;
loc_825B0274:
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwimi r10,r30,31,0,1
  r10 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 31) & 0xC0000000) | (r10 & 0xFFFFFFFF3FFFFFFF);
  // b 0x825b02a0
  goto loc_825B02A0;
loc_825B0284:
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // oris r10,r10,49152
  r10 = r10 | 3221225472;
  // b 0x825b02a0
  goto loc_825B02A0;
loc_825B0294:
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // clrlwi r10,r10,2
  r10 = uint32_t(r10) & 0x3FFFFFFF;
loc_825B02A0:
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_825B02A4:
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwimi r10,r30,27,2,4
  r10 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 27) & 0x38000000) | (r10 & 0xFFFFFFFFC7FFFFFF);
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwimi r10,r28,15,5,16
  r10 = (__builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 15) & 0x7FF8000) | (r10 & 0xFFFFFFFFF8007FFF);
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,32(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 32);
  // oris r10,r10,32768
  r10 = r10 | 2147483648;
  // stw r10,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r10));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,32(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 32);
  // beq cr6,0x825b0328
  if (cr6.eq) goto loc_825B0328;
  // rlwinm r10,r10,0,5,0
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFF87FFFFFF;
  // stw r10,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r10));
  // lfs f0,0(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::FpOperand(base, uint32_t(r29)).as_float_address());
  f0 = double(temp.f32);
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 16, temp.u32);
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 96);
  // oris r10,r10,32768
  r10 = r10 | 2147483648;
  // stw r10,96(r11)
  guest::StoreU32(base, uint32_t(r11) + 96, uint32_t(r10));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 96);
  // rlwinm r10,r10,0,5,0
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFF87FFFFFF;
  // stw r10,96(r11)
  guest::StoreU32(base, uint32_t(r11) + 96, uint32_t(r10));
  // lfs f0,0(r26)
  temp.u32 = guest::LoadU32(base, schema::FpOperand(base, uint32_t(r26)).as_float_address());
  f0 = double(temp.f32);
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // stfs f0,80(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 80, temp.u32);
  // b 0x825b0368
  goto loc_825B0368;
loc_825B0328:
  // rlwimi r10,r30,27,1,4
  r10 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 27) & 0x78000000) | (r10 & 0xFFFFFFFF87FFFFFF);
  // stw r10,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r10));
  // lfd f0,0(r29)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, schema::FpOperand(base, uint32_t(r29)).as_double_address())));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // stfd f0,16(r11)
  guest::StoreU64(base, uint32_t(r11) + 16, std::bit_cast<uint64_t>(f0));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 96);
  // oris r10,r10,32768
  r10 = r10 | 2147483648;
  // stw r10,96(r11)
  guest::StoreU32(base, uint32_t(r11) + 96, uint32_t(r10));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r10,96(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 96);
  // rlwimi r10,r30,27,1,4
  r10 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 27) & 0x78000000) | (r10 & 0xFFFFFFFF87FFFFFF);
  // stw r10,96(r11)
  guest::StoreU32(base, uint32_t(r11) + 96, uint32_t(r10));
  // lfd f0,0(r26)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, schema::FpOperand(base, uint32_t(r26)).as_double_address())));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // stfd f0,80(r11)
  guest::StoreU64(base, uint32_t(r11) + 80, std::bit_cast<uint64_t>(f0));
loc_825B0368:
  // bl 0x825ab568
  // (left out: dead in C++)
  { const lifted::Result o = game::_clearfp(base, LIFTED_ARGS(0x825B036C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r1,180
  r6 = uint64_t(int64_t(int64_t(r1) + 180));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // bl 0x825ba420
  // (left out: dead in C++)
  { const lifted::Result o = game::RaiseException(base, LIFTED_ARGS(0x825B0380, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,180(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // rlwinm. r11,r11,0,4,4
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b039c
  if (cr0.eq) goto loc_825B039C;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // rlwinm r11,r11,0,25,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFF7F;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address(), uint32_t(r11));
loc_825B039C:
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // rlwinm. r11,r11,0,3,3
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b03b4
  if (cr0.eq) goto loc_825B03B4;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // rlwinm r11,r11,0,28,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFEF;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address(), uint32_t(r11));
loc_825B03B4:
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // rlwinm. r11,r11,0,2,2
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b03cc
  if (cr0.eq) goto loc_825B03CC;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // rlwinm r11,r11,0,26,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFBF;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address(), uint32_t(r11));
loc_825B03CC:
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // rlwinm. r11,r11,0,1,1
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b03e4
  if (cr0.eq) goto loc_825B03E4;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // rlwinm r11,r11,0,27,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFDF;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address(), uint32_t(r11));
loc_825B03E4:
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // rlwinm. r11,r11,0,0,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b03fc
  if (cr0.eq) goto loc_825B03FC;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // rlwinm r11,r11,0,29,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFF7;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address(), uint32_t(r11));
loc_825B03FC:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // rlwinm r11,r11,2,30,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825b0440
  if (cr6.lt) goto loc_825B0440;
  // beq cr6,0x825b0434
  if (cr6.eq) goto loc_825B0434;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x825b0428
  if (cr6.lt) goto loc_825B0428;
  // bne cr6,0x825b044c
  if (!cr6.eq) goto loc_825B044C;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // rlwimi r11,r30,0,30,31
  r11 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x3) | (r11 & 0xFFFFFFFFFFFFFFFC);
  // b 0x825b0448
  goto loc_825B0448;
loc_825B0428:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // rlwimi r11,r30,1,30,31
  r11 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0x3) | (r11 & 0xFFFFFFFFFFFFFFFC);
  // b 0x825b0448
  goto loc_825B0448;
loc_825B0434:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // ori r11,r11,3
  r11 = r11 | 3;
  // b 0x825b0448
  goto loc_825B0448;
loc_825B0440:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address());
  // rlwinm r11,r11,0,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFC;
loc_825B0448:
  // stw r11,0(r31)
  guest::StoreU32(base, schema::FpControlWord(base, uint32_t(r31)).value_address(), uint32_t(r11));
loc_825B044C:
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // beq cr6,0x825b0460
  if (cr6.eq) goto loc_825B0460;
  // lfs f0,80(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 80);
  f0 = double(temp.f32);
  // stfs f0,0(r26)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::FpOperand(base, uint32_t(r26)).as_float_address(), temp.u32);
  // b 0x825b0468
  goto loc_825B0468;
loc_825B0460:
  // lfd f0,80(r10)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r10) + 80)));
  // stfd f0,0(r26)
  guest::StoreU64(base, schema::FpOperand(base, uint32_t(r26)).as_double_address(), std::bit_cast<uint64_t>(f0));
loc_825B0468:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825B0470: lifted from generated/ by lift.py.
lifted::Result f825B0470(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_825B0470 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-56(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -56, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r27,r5
  r27 = r5;
  // clrlwi r28,r29,27
  r28 = uint32_t(r29) & 0x1F;
  // rlwinm. r11,r29,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b04b0
  if (cr0.eq) goto loc_825B04B0;
  // rlwinm. r11,r27,0,24,24
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 0) & 0x80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b04b0
  if (cr0.eq) goto loc_825B04B0;
  // lis r3,8192
  r3 = uint64_t(int64_t(536870912));
  // bl 0x825b0be4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B0BE4, LIFTED_ARGS(0x825B04A8, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r28,r28,0,29,27
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFFFFFFFFF7;
  // b 0x825b0694
  goto loc_825B0694;
loc_825B04B0:
  // rlwinm. r11,r29,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b04d0
  if (cr0.eq) goto loc_825B04D0;
  // rlwinm. r11,r27,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b04d0
  if (cr0.eq) goto loc_825B04D0;
  // lis r3,1024
  r3 = uint64_t(int64_t(67108864));
  // bl 0x825b0be4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B0BE4, LIFTED_ARGS(0x825B04C8, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r28,r28,0,30,28
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFFFFFFFFFB;
  // b 0x825b0694
  goto loc_825B0694;
loc_825B04D0:
  // clrlwi. r11,r29,31
  r11 = uint32_t(r29) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b059c
  if (cr0.eq) goto loc_825B059C;
  // rlwinm. r11,r27,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b059c
  if (cr0.eq) goto loc_825B059C;
  // lis r3,4096
  r3 = uint64_t(int64_t(268435456));
  // bl 0x825b0be4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B0BE4, LIFTED_ARGS(0x825B04E8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r27,30
  r11 = uint32_t(r27) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825b0564
  if (cr6.lt) goto loc_825B0564;
  // beq cr6,0x825b0544
  if (cr6.eq) goto loc_825B0544;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x825b0524
  if (cr6.lt) goto loc_825B0524;
  // bne cr6,0x825b0594
  if (!cr6.eq) goto loc_825B0594;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f13,0(r30)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, schema::DoubleValue(base, uint32_t(r30)).value_address())));
  // lfd f0,8792(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8792)));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x825b0584
  if (!cr6.gt) goto loc_825B0584;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lfd f0,-17344(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -17344)));
  // b 0x825b0590
  goto loc_825B0590;
loc_825B0524:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f13,0(r30)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, schema::DoubleValue(base, uint32_t(r30)).value_address())));
  // lfd f0,8792(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8792)));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x825b0578
  if (cr6.gt) goto loc_825B0578;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lfd f0,-17344(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -17344)));
  // b 0x825b058c
  goto loc_825B058C;
loc_825B0544:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f13,0(r30)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, schema::DoubleValue(base, uint32_t(r30)).value_address())));
  // lfd f0,8792(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8792)));
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // lfd f0,-17344(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -17344)));
  // bgt cr6,0x825b0590
  if (cr6.gt) goto loc_825B0590;
  // b 0x825b058c
  goto loc_825B058C;
loc_825B0564:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f13,0(r30)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, schema::DoubleValue(base, uint32_t(r30)).value_address())));
  // lfd f0,8792(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8792)));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x825b0584
  if (!cr6.gt) goto loc_825B0584;
loc_825B0578:
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lfd f0,-17360(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -17360)));
  // b 0x825b0590
  goto loc_825B0590;
loc_825B0584:
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lfd f0,-17360(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -17360)));
loc_825B058C:
  // fneg f0,f0
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
loc_825B0590:
  // stfd f0,0(r30)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, schema::DoubleValue(base, uint32_t(r30)).value_address(), std::bit_cast<uint64_t>(f0));
loc_825B0594:
  // rlwinm r28,r28,0,0,30
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFE;
  // b 0x825b0694
  goto loc_825B0694;
loc_825B059C:
  // rlwinm. r11,r29,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0694
  if (cr0.eq) goto loc_825B0694;
  // rlwinm. r11,r27,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0694
  if (cr0.eq) goto loc_825B0694;
  // rlwinm. r11,r29,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // beq 0x825b05bc
  if (cr0.eq) goto loc_825B05BC;
  // li r31,1
  r31 = uint64_t(int64_t(1));
loc_825B05BC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f1,0(r30)
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, schema::DoubleValue(base, uint32_t(r30)).value_address())));
  // lfd f31,8792(r11)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8792)));
  // fcmpu cr6,f1,f31
  cr6 = lifted::CompareFloat(f1, f31);
  // beq cr6,0x825b067c
  if (cr6.eq) goto loc_825B067C;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825aff80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825AFF80, LIFTED_ARGS(0x825B05D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // stfd f1,80(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + 80, std::bit_cast<uint64_t>(f1));
  // addi r11,r11,-1536
  r11 = uint64_t(int64_t(int64_t(r11) + -1536));
  // cmpwi cr6,r11,-1074
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1074, so);
  // bge cr6,0x825b05f8
  if (!cr6.lt) goto loc_825B05F8;
  // fmul f0,f1,f31
  f0 = f1 * f31;
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // b 0x825b0674
  goto loc_825B0674;
loc_825B05F8:
  // fcmpu cr6,f1,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f31);
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // blt cr6,0x825b0608
  if (cr6.lt) goto loc_825B0608;
  // li r8,0
  r8 = uint64_t(int64_t(0));
loc_825B0608:
  // lhz r10,80(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 80);
  // cmpwi cr6,r11,-1021
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1021, so);
  // clrlwi r10,r10,28
  r10 = uint32_t(r10) & 0xF;
  // ori r10,r10,16
  r10 = r10 | 16;
  // sth r10,80(r1)
  guest::StoreU16(base, uint32_t(r1) + 80, uint16_t(r10));
  // bge cr6,0x825b0664
  if (!cr6.lt) goto loc_825B0664;
  // subfic r9,r11,-1021
  ca = uint32_t(r11) <= 4294966275;
  r9 = static_cast<uint64_t>(-1021) - r11;
  // lwz r10,80(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
loc_825B062C:
  // clrlwi. r7,r11,31
  r7 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beq 0x825b0640
  if (cr0.eq) goto loc_825B0640;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne cr6,0x825b0640
  if (!cr6.eq) goto loc_825B0640;
  // li r31,1
  r31 = uint64_t(int64_t(1));
loc_825B0640:
  // clrlwi. r7,r10,31
  r7 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // rlwinm r11,r11,31,1,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x7FFFFFFF;
  // beq 0x825b0650
  if (cr0.eq) goto loc_825B0650;
  // oris r11,r11,32768
  r11 = r11 | 2147483648;
loc_825B0650:
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // rlwinm r10,r10,31,1,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 31) & 0x7FFFFFFF;
  // bne 0x825b062c
  if (!cr0.eq) goto loc_825B062C;
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_825B0664:
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x825b0674
  if (cr6.eq) goto loc_825B0674;
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
loc_825B0674:
  // stfd f0,0(r30)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, schema::DoubleValue(base, uint32_t(r30)).value_address(), std::bit_cast<uint64_t>(f0));
  // b 0x825b0680
  goto loc_825B0680;
loc_825B067C:
  // li r31,1
  r31 = uint64_t(int64_t(1));
loc_825B0680:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq cr6,0x825b0690
  if (cr6.eq) goto loc_825B0690;
  // lis r3,2048
  r3 = uint64_t(int64_t(134217728));
  // bl 0x825b0be4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B0BE4, LIFTED_ARGS(0x825B0690, r31)); r3 = o.r3; f1 = o.f1; }
loc_825B0690:
  // rlwinm r28,r28,0,31,29
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFFFFFFFFFD;
loc_825B0694:
  // rlwinm. r11,r29,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b06b0
  if (cr0.eq) goto loc_825B06B0;
  // rlwinm. r11,r27,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b06b0
  if (cr0.eq) goto loc_825B06B0;
  // lis r3,512
  r3 = uint64_t(int64_t(33554432));
  // bl 0x825b0be4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B0BE4, LIFTED_ARGS(0x825B06AC, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r28,r28,0,28,26
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0xFFFFFFFFFFFFFFEF;
loc_825B06B0:
  // cntlzw r11,r28
  r11 = uint32_t(r28) == 0 ? 32 : __builtin_clz(uint32_t(r28));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-56(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825B06C8: lifted from generated/ by lift.py.
lifted::Result f825B06C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825B06C8 (lifted)");
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
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // beq cr6,0x825b06f4
  if (cr6.eq) goto loc_825B06F4;
  // ble cr6,0x825b0700
  if (!cr6.gt) goto loc_825B0700;
  // cmpwi cr6,r3,3
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 3, so);
  // bgt cr6,0x825b0700
  if (cr6.gt) goto loc_825B0700;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B06EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // b 0x825b06fc
  goto loc_825B06FC;
loc_825B06F4:
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B06F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,33
  r11 = uint64_t(int64_t(33));
loc_825B06FC:
  // stw r11,0(r3)
  guest::StoreU32(base, schema::ErrnoSlot(base, uint32_t(r3)).value_address(), uint32_t(r11));
loc_825B0700:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825B0710: lifted from generated/ by lift.py.
uint64_t f825B0710(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825B0710 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // rlwinm. r11,r3,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0720
  if (cr0.eq) goto loc_825B0720;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // blr
  return r3;
loc_825B0720:
  // rlwinm. r11,r3,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0730
  if (cr0.eq) goto loc_825B0730;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
loc_825B0730:
  // rlwinm. r11,r3,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0740
  if (cr0.eq) goto loc_825B0740;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
loc_825B0740:
  // clrlwi. r11,r3,31
  r11 = uint32_t(r3) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b0750
  if (cr0.eq) goto loc_825B0750;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // blr
  return r3;
loc_825B0750:
  // rlwinm r3,r3,1,29,29
  r3 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0x4;
  // blr
  return r3;
}

// sub_825B0758: lifted from generated/ by lift.py.
lifted::Result f825B0758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_825B0758 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // fmr f31,f3
  f31 = f3;
  // mr r31,r3
  r31 = r3;
  // stfd f1,176(r1)
  guest::StoreU64(base, uint32_t(r1) + 176, std::bit_cast<uint64_t>(f1));
  // addi r11,r11,-17320
  r11 = uint64_t(int64_t(int64_t(r11) + -17320));
  // stfd f2,184(r1)
  guest::StoreU64(base, uint32_t(r1) + 184, std::bit_cast<uint64_t>(f2));
  // stfd f31,192(r1)
  guest::StoreU64(base, uint32_t(r1) + 192, std::bit_cast<uint64_t>(f31));
  // mr r3,r8
  r3 = r8;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // mr r10,r11
  r10 = r11;
loc_825B0794:
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplw cr6,r8,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r4), so);
  // beq cr6,0x825b0824
  if (cr6.eq) goto loc_825B0824;
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // addi r8,r11,232
  r8 = uint64_t(int64_t(int64_t(r11) + 232));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpw cr6,r10,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r8), so);
  // blt cr6,0x825b0794
  if (cr6.lt) goto loc_825B0794;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825B07B8:
  // lis r4,-16377
  r4 = uint64_t(int64_t(-1073283072));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ori r4,r4,65279
  r4 = r4 | 65279;
  // beq cr6,0x825b0834
  if (cr6.eq) goto loc_825B0834;
  // lwz r11,176(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 176);
  // stw r31,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r31));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // lwz r11,184(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 184);
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // lwz r11,188(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 188);
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // lwz r11,192(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 192);
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // lwz r11,196(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 196);
  // stw r11,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r11));
  // bl 0x825b0b80
  // (left out: dead in C++)
  { const lifted::Result o = game::_ctrlfp(base, LIFTED_ARGS(0x825B0804, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825c8388
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C8388(base, LIFTED_ARGS(0x825B080C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b081c
  if (!cr0.eq) goto loc_825B081C;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b06c8
  // (left out: dead in C++)
  { const lifted::Result o = f825B06C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B081C:
  // lfd f1,104(r1)
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // b 0x825b0844
  goto loc_825B0844;
loc_825B0824:
  // rlwinm r10,r9,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // b 0x825b07b8
  goto loc_825B07B8;
loc_825B0834:
  // bl 0x825b0b80
  // (left out: dead in C++)
  { const lifted::Result o = game::_ctrlfp(base, LIFTED_ARGS(0x825B0838, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825b06c8
  // (left out: dead in C++)
  { const lifted::Result o = f825B06C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
loc_825B0844:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825B0860: lifted from generated/ by lift.py.
lifted::Result f825B0860(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825B0860 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // fmr f31,f1
  f31 = f1;
  // mr r4,r3
  r4 = r3;
  // mr r31,r5
  r31 = r5;
  // lwz r11,-15036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -15036);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b08ac
  if (!cr6.eq) goto loc_825B08AC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f3,f31
  f3 = f31;
  // mr r8,r31
  r8 = r31;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lfd f2,8792(r11)
  f2 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8792)));
  // bl 0x825b0758
  // (left out: dead in C++)
  { const lifted::Result o = f825B0758(base, r1, r13, r3, r4, r5, r6, r7, r8, f1, f2, f3); r3 = o.r3; f1 = o.f1; }
  // b 0x825b08d0
  goto loc_825B08D0;
loc_825B08AC:
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B08B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,33
  r10 = uint64_t(int64_t(33));
  // lis r4,-16377
  r4 = uint64_t(int64_t(-1073283072));
  // mr r3,r31
  r3 = r31;
  // ori r4,r4,65279
  r4 = r4 | 65279;
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ErrnoSlot(base, uint32_t(r11)).value_address(), uint32_t(r10));
  // bl 0x825b0b80
  // (left out: dead in C++)
  { const lifted::Result o = game::_ctrlfp(base, LIFTED_ARGS(0x825B08CC, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
loc_825B08D0:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825B08E8: lifted from generated/ by lift.py.
lifted::Result f825B08E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_825B08E8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // fadd f31,f1,f2
  f31 = f1 + f2;
  // mr r4,r3
  r4 = r3;
  // mr r31,r6
  r31 = r6;
  // lwz r11,-15036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -15036);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b092c
  if (!cr6.eq) goto loc_825B092C;
  // mr r8,r31
  r8 = r31;
  // fmr f3,f31
  f3 = f31;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x825b0758
  // (left out: dead in C++)
  { const lifted::Result o = f825B0758(base, r1, r13, r3, r4, r5, r6, r7, r8, f1, f2, f3); r3 = o.r3; f1 = o.f1; }
  // b 0x825b0950
  goto loc_825B0950;
loc_825B092C:
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B0930, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,33
  r10 = uint64_t(int64_t(33));
  // lis r4,-16377
  r4 = uint64_t(int64_t(-1073283072));
  // mr r3,r31
  r3 = r31;
  // ori r4,r4,65279
  r4 = r4 | 65279;
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ErrnoSlot(base, uint32_t(r11)).value_address(), uint32_t(r10));
  // bl 0x825b0b80
  // (left out: dead in C++)
  { const lifted::Result o = game::_ctrlfp(base, LIFTED_ARGS(0x825B094C, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
loc_825B0950:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825B0968: lifted from generated/ by lift.py.
lifted::Result f825B0968(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_825B0968 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-40(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r7
  r30 = r7;
  // fmr f31,f1
  f31 = f1;
  // mr r29,r4
  r29 = r4;
  // stfd f31,272(r1)
  guest::StoreU64(base, uint32_t(r1) + 272, std::bit_cast<uint64_t>(f31));
  // mr r5,r30
  r5 = r30;
  // stfd f2,280(r1)
  guest::StoreU64(base, uint32_t(r1) + 280, std::bit_cast<uint64_t>(f2));
  // addi r4,r1,280
  r4 = uint64_t(int64_t(int64_t(r1) + 280));
  // mr r31,r3
  r31 = r3;
  // stw r30,292(r1)
  guest::StoreU32(base, uint32_t(r1) + 292, uint32_t(r30));
  // bl 0x825b0470
  // (left out: dead in C++)
  { const lifted::Result o = f825B0470(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b09d8
  if (!cr0.eq) goto loc_825B09D8;
  // lwz r11,144(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 144);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r8,r1,280
  r8 = uint64_t(int64_t(int64_t(r1) + 280));
  // clrlwi r11,r11,1
  r11 = uint32_t(r11) & 0x7FFFFFFF;
  // addi r7,r1,272
  r7 = uint64_t(int64_t(int64_t(r1) + 272));
  // mr r6,r29
  r6 = r29;
  // mr r5,r31
  r5 = r31;
  // addi r4,r1,292
  r4 = uint64_t(int64_t(int64_t(r1) + 292));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r11));
  // bl 0x825b0068
  // (left out: dead in C++)
  { const lifted::Result o = f825B0068(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lwz r30,292(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 292);
loc_825B09D8:
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0710
  // (left out: dead in C++)
  r3 = f825B0710(base, r3);
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lwz r11,-15036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -15036);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b0a18
  if (!cr6.eq) goto loc_825B0A18;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825b0a18
  if (cr6.eq) goto loc_825B0A18;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f3,280(r1)
  lifted::ScalarFloatMode();
  f3 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 280)));
  // mr r8,r30
  r8 = r30;
  // fmr f1,f31
  f1 = f31;
  // mr r4,r29
  r4 = r29;
  // lfd f2,8792(r11)
  f2 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8792)));
  // bl 0x825b0758
  // (left out: dead in C++)
  { const lifted::Result o = f825B0758(base, r1, r13, r3, r4, r5, r6, r7, r8, f1, f2, f3); r3 = o.r3; f1 = o.f1; }
  // b 0x825b0a30
  goto loc_825B0A30;
loc_825B0A18:
  // bl 0x825b06c8
  // (left out: dead in C++)
  { const lifted::Result o = f825B06C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r4,-16377
  r4 = uint64_t(int64_t(-1073283072));
  // mr r3,r30
  r3 = r30;
  // ori r4,r4,65279
  r4 = r4 | 65279;
  // bl 0x825b0b80
  // (left out: dead in C++)
  { const lifted::Result o = game::_ctrlfp(base, LIFTED_ARGS(0x825B0A2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lfd f1,280(r1)
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 280)));
loc_825B0A30:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825B0A40: lifted from generated/ by lift.py.
lifted::Result f825B0A40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_825B0A40 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-48(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -48, std::bit_cast<uint64_t>(f30));
  // stfd f31,-40(r1)
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r8
  r30 = r8;
  // fmr f30,f1
  f30 = f1;
  // mr r29,r4
  r29 = r4;
  // stfd f30,272(r1)
  guest::StoreU64(base, uint32_t(r1) + 272, std::bit_cast<uint64_t>(f30));
  // mr r5,r30
  r5 = r30;
  // stfd f3,288(r1)
  guest::StoreU64(base, uint32_t(r1) + 288, std::bit_cast<uint64_t>(f3));
  // addi r4,r1,288
  r4 = uint64_t(int64_t(int64_t(r1) + 288));
  // fmr f31,f2
  f31 = f2;
  // mr r31,r3
  r31 = r3;
  // stw r30,300(r1)
  guest::StoreU32(base, uint32_t(r1) + 300, uint32_t(r30));
  // bl 0x825b0470
  // (left out: dead in C++)
  { const lifted::Result o = f825B0470(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b0ac0
  if (!cr0.eq) goto loc_825B0AC0;
  // lwz r11,144(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 144);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r8,r1,288
  r8 = uint64_t(int64_t(int64_t(r1) + 288));
  // stfd f31,128(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + 128, std::bit_cast<uint64_t>(f31));
  // clrlwi r11,r11,5
  r11 = uint32_t(r11) & 0x7FFFFFF;
  // addi r7,r1,272
  r7 = uint64_t(int64_t(int64_t(r1) + 272));
  // oris r11,r11,34816
  r11 = r11 | 2281701376;
  // mr r6,r29
  r6 = r29;
  // mr r5,r31
  r5 = r31;
  // addi r4,r1,300
  r4 = uint64_t(int64_t(int64_t(r1) + 300));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r11));
  // bl 0x825b0068
  // (left out: dead in C++)
  { const lifted::Result o = f825B0068(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lwz r30,300(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 300);
loc_825B0AC0:
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0710
  // (left out: dead in C++)
  r3 = f825B0710(base, r3);
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lwz r11,-15036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -15036);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b0afc
  if (!cr6.eq) goto loc_825B0AFC;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825b0afc
  if (cr6.eq) goto loc_825B0AFC;
  // mr r8,r30
  r8 = r30;
  // lfd f3,288(r1)
  lifted::ScalarFloatMode();
  f3 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 288)));
  // mr r4,r29
  r4 = r29;
  // fmr f2,f31
  f2 = f31;
  // fmr f1,f30
  f1 = f30;
  // bl 0x825b0758
  // (left out: dead in C++)
  { const lifted::Result o = f825B0758(base, r1, r13, r3, r4, r5, r6, r7, r8, f1, f2, f3); r3 = o.r3; f1 = o.f1; }
  // b 0x825b0b14
  goto loc_825B0B14;
loc_825B0AFC:
  // bl 0x825b06c8
  // (left out: dead in C++)
  { const lifted::Result o = f825B06C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r4,-16377
  r4 = uint64_t(int64_t(-1073283072));
  // mr r3,r30
  r3 = r30;
  // ori r4,r4,65279
  r4 = r4 | 65279;
  // bl 0x825b0b80
  // (left out: dead in C++)
  { const lifted::Result o = game::_ctrlfp(base, LIFTED_ARGS(0x825B0B10, r31)); r3 = o.r3; f1 = o.f1; }
  // lfd f1,288(r1)
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 288)));
loc_825B0B14:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfd f30,-48(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825B1F18: lifted from generated/ by lift.py.
lifted::Result f825B1F18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825B1F18 (lifted)");
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
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // mr r29,r6
  r29 = r6;
  // mr r28,r7
  r28 = r7;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // bne cr6,0x825b1fac
  if (!cr6.eq) goto loc_825B1FAC;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // cmplw cr6,r3,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r29), so);
  // bne cr6,0x825b1f88
  if (!cr6.eq) goto loc_825B1F88;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r11
  r3 = r11;
  // bl 0x825ac320
  // (left out: dead in C++)
  { const lifted::Result o = game::calloc(base, LIFTED_ARGS(0x825B1F58, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r3));
  // bne 0x825b1f6c
  if (!cr0.eq) goto loc_825B1F6C;
loc_825B1F64:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825b1fb0
  goto loc_825B1FB0;
loc_825B1F6C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r4,r29
  r4 = r29;
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
  // lwz r5,0(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x825B1F84, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825b1fa0
  goto loc_825B1FA0;
loc_825B1F88:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r4,r11
  r4 = r11;
  // bl 0x825aecc0
  // (left out: dead in C++)
  { const lifted::Result o = f825AECC0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825b1f64
  if (cr0.eq) goto loc_825B1F64;
  // stw r3,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r3));
loc_825B1FA0:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_825B1FAC:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_825B1FB0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825B1FB8: lifted from generated/ by lift.py.
lifted::Result f825B1FB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825B1FB8 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-688(r1)
  ea = -688 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r11,r1,176
  r11 = uint64_t(int64_t(int64_t(r1) + 176));
  // stw r6,732(r1)
  guest::StoreU32(base, uint32_t(r1) + 732, uint32_t(r6));
  // mr r28,r4
  r28 = r4;
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // mr r26,r3
  r26 = r3;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // li r11,350
  r11 = uint64_t(int64_t(350));
  // stw r28,716(r1)
  guest::StoreU32(base, uint32_t(r1) + 716, uint32_t(r28));
  // stw r23,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r23));
  // sth r23,96(r1)
  guest::StoreU16(base, uint32_t(r1) + 96, uint16_t(r23));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // bne cr6,0x825b2028
  if (!cr6.eq) goto loc_825B2028;
loc_825B1FF8:
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B1FFC, r31)); r3 = o.r3; f1 = o.f1; }
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::_invalid_parameter(base, LIFTED_ARGS(0x825B2020, r31)); r3 = o.r3; f1 = o.f1; }
loc_825B2020:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // b 0x825b31d0
  goto loc_825B31D0;
loc_825B2028:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x825b1ff8
  if (cr6.eq) goto loc_825B1FF8;
  // lwz r11,12(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 12);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b210c
  if (!cr0.eq) goto loc_825B210C;
  // mr r3,r26
  r3 = r26;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B2044, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32104
  r11 = uint64_t(int64_t(-2103967744));
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // addi r30,r11,-32224
  r30 = uint64_t(int64_t(int64_t(r11) + -32224));
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // addi r29,r11,-16456
  r29 = uint64_t(int64_t(int64_t(r11) + -16456));
  // beq cr6,0x825b209c
  if (cr6.eq) goto loc_825B209C;
  // mr r3,r26
  r3 = r26;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B2064, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -2, so);
  // beq cr6,0x825b209c
  if (cr6.eq) goto loc_825B209C;
  // mr r3,r26
  r3 = r26;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B2074, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // mr r3,r26
  r3 = r26;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B2080, r31)); r3 = o.r3; f1 = o.f1; }
  // srawi r11,r31,5
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0x1F) != 0);
  r11 = uint64_t(int64_t(int32_t(r31) >> 5));
  // clrlwi r10,r3,27
  r10 = uint32_t(r3) & 0x1F;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mulli r10,r10,44
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(44))));
  // lwzx r11,r11,r30
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r30));
  // add r11,r11,r10
  r11 = r11 + r10;
  // b 0x825b20a0
  goto loc_825B20A0;
loc_825B209C:
  // mr r11,r29
  r11 = r29;
loc_825B20A0:
  // lbz r11,40(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 40);
  // rlwinm. r11,r11,0,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFE;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b1ff8
  if (!cr0.eq) goto loc_825B1FF8;
  // mr r3,r26
  r3 = r26;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B20B4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x825b20fc
  if (cr6.eq) goto loc_825B20FC;
  // mr r3,r26
  r3 = r26;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B20C4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -2, so);
  // beq cr6,0x825b20fc
  if (cr6.eq) goto loc_825B20FC;
  // mr r3,r26
  r3 = r26;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B20D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // mr r3,r26
  r3 = r26;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B20E0, r31)); r3 = o.r3; f1 = o.f1; }
  // srawi r11,r31,5
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0x1F) != 0);
  r11 = uint64_t(int64_t(int32_t(r31) >> 5));
  // clrlwi r10,r3,27
  r10 = uint32_t(r3) & 0x1F;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mulli r10,r10,44
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(44))));
  // lwzx r11,r11,r30
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r30));
  // add r11,r11,r10
  r11 = r11 + r10;
  // b 0x825b2100
  goto loc_825B2100;
loc_825B20FC:
  // mr r11,r29
  r11 = r29;
loc_825B2100:
  // lbz r11,40(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 40);
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b1ff8
  if (!cr0.eq) goto loc_825B1FF8;
loc_825B210C:
  // lbz r3,0(r28)
  r3 = guest::LoadU8(base, uint32_t(r28) + 0);
  // mr r14,r23
  r14 = r23;
  // stb r23,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r23));
  // stw r23,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r23));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825b31cc
  if (cr0.eq) goto loc_825B31CC;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lwz r19,716(r1)
  r19 = guest::LoadU32(base, uint32_t(r1) + 716);
  // addi r11,r11,-17368
  r11 = uint64_t(int64_t(int64_t(r11) + -17368));
  // stw r11,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r11));
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // addi r11,r11,-17828
  r11 = uint64_t(int64_t(int64_t(r11) + -17828));
  // stw r11,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r11));
loc_825B2140:
  // bl 0x825ab080
  // (left out: dead in C++)
  r3 = f825AB080(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b21c8
  if (cr0.eq) goto loc_825B21C8;
  // addi r14,r14,-1
  r14 = uint64_t(int64_t(int64_t(r14) + -1));
loc_825B2150:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2178
  if (cr0.lt) goto loc_825B2178;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r31,0(r11)
  r31 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2184
  goto loc_825B2184;
loc_825B2178:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
loc_825B2184:
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x825b21b0
  if (cr6.eq) goto loc_825B21B0;
  // clrlwi r3,r31,24
  r3 = uint32_t(r31) & 0xFF;
  // bl 0x825ab080
  // (left out: dead in C++)
  r3 = f825AB080(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b2150
  if (!cr0.eq) goto loc_825B2150;
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x825b21b0
  if (cr6.eq) goto loc_825B21B0;
  // mr r4,r26
  r4 = r26;
  // mr r3,r31
  r3 = r31;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B21B0:
  // addi r19,r19,1
  r19 = uint64_t(int64_t(int64_t(r19) + 1));
  // lbz r3,0(r19)
  r3 = guest::LoadU8(base, uint32_t(r19) + 0);
  // bl 0x825ab080
  // (left out: dead in C++)
  r3 = f825AB080(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b21b0
  if (!cr0.eq) goto loc_825B21B0;
  // b 0x825b3160
  goto loc_825B3160;
loc_825B21C8:
  // lbz r11,0(r19)
  r11 = guest::LoadU8(base, uint32_t(r19) + 0);
  // cmplwi cr6,r11,37
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 37, so);
  // bne cr6,0x825b3098
  if (!cr6.eq) goto loc_825B3098;
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // li r18,0
  r18 = uint64_t(int64_t(0));
  // li r17,0
  r17 = uint64_t(int64_t(0));
  // li r16,0
  r16 = uint64_t(int64_t(0));
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // li r20,0
  r20 = uint64_t(int64_t(0));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r15,1
  r15 = uint64_t(int64_t(1));
  // li r21,0
  r21 = uint64_t(int64_t(0));
loc_825B2208:
  // addi r19,r19,1
  r19 = uint64_t(int64_t(int64_t(r19) + 1));
  // lbz r31,0(r19)
  r31 = guest::LoadU8(base, uint32_t(r19) + 0);
  // mr r3,r31
  r3 = r31;
  // bl 0x825ab040
  // (left out: dead in C++)
  { const lifted::Result o = game::isdigit(base, LIFTED_ARGS(0x825B2218, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b2234
  if (cr0.eq) goto loc_825B2234;
  // mulli r11,r16,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r16 * static_cast<uint64_t>(10))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // addi r17,r17,1
  r17 = uint64_t(int64_t(int64_t(r17) + 1));
  // addi r16,r11,-48
  r16 = uint64_t(int64_t(int64_t(r11) + -48));
  // b 0x825b2348
  goto loc_825B2348;
loc_825B2234:
  // cmpwi cr6,r31,78
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 78, so);
  // bgt cr6,0x825b22f4
  if (cr6.gt) goto loc_825B22F4;
  // beq cr6,0x825b2348
  if (cr6.eq) goto loc_825B2348;
  // cmpwi cr6,r31,42
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 42, so);
  // beq cr6,0x825b22e8
  if (cr6.eq) goto loc_825B22E8;
  // cmpwi cr6,r31,70
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 70, so);
  // beq cr6,0x825b2348
  if (cr6.eq) goto loc_825B2348;
  // cmpwi cr6,r31,73
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 73, so);
  // beq cr6,0x825b226c
  if (cr6.eq) goto loc_825B226C;
  // cmpwi cr6,r31,76
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 76, so);
  // bne cr6,0x825b230c
  if (!cr6.eq) goto loc_825B230C;
  // addi r11,r15,1
  r11 = uint64_t(int64_t(int64_t(r15) + 1));
  // extsb r15,r11
  r15 = uint64_t(int64_t(int8_t(r11)));
  // b 0x825b2348
  goto loc_825B2348;
loc_825B226C:
  // lbz r10,1(r19)
  r10 = guest::LoadU8(base, uint32_t(r19) + 1);
  // cmplwi cr6,r10,54
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 54, so);
  // bne cr6,0x825b229c
  if (!cr6.eq) goto loc_825B229C;
  // addi r11,r19,2
  r11 = uint64_t(int64_t(int64_t(r19) + 2));
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // cmplwi cr6,r9,52
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 52, so);
  // bne cr6,0x825b229c
  if (!cr6.eq) goto loc_825B229C;
loc_825B2288:
  // mr r19,r11
  r19 = r11;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r21,r21,1
  r21 = uint64_t(int64_t(int64_t(r21) + 1));
  // std r11,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r11);
  // b 0x825b2348
  goto loc_825B2348;
loc_825B229C:
  // cmplwi cr6,r10,51
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 51, so);
  // bne cr6,0x825b22bc
  if (!cr6.eq) goto loc_825B22BC;
  // addi r11,r19,2
  r11 = uint64_t(int64_t(int64_t(r19) + 2));
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // cmplwi cr6,r9,50
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 50, so);
  // bne cr6,0x825b22bc
  if (!cr6.eq) goto loc_825B22BC;
  // mr r19,r11
  r19 = r11;
  // b 0x825b2348
  goto loc_825B2348;
loc_825B22BC:
  // cmplwi cr6,r10,100
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 100, so);
  // beq cr6,0x825b2348
  if (cr6.eq) goto loc_825B2348;
  // cmplwi cr6,r10,105
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 105, so);
  // beq cr6,0x825b2348
  if (cr6.eq) goto loc_825B2348;
  // cmplwi cr6,r10,111
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 111, so);
  // beq cr6,0x825b2348
  if (cr6.eq) goto loc_825B2348;
  // cmplwi cr6,r10,120
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 120, so);
  // beq cr6,0x825b2348
  if (cr6.eq) goto loc_825B2348;
  // cmplwi cr6,r10,88
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 88, so);
  // bne cr6,0x825b230c
  if (!cr6.eq) goto loc_825B230C;
  // b 0x825b2348
  goto loc_825B2348;
loc_825B22E8:
  // addi r11,r29,1
  r11 = uint64_t(int64_t(int64_t(r29) + 1));
  // extsb r29,r11
  r29 = uint64_t(int64_t(int8_t(r11)));
  // b 0x825b2348
  goto loc_825B2348;
loc_825B22F4:
  // cmpwi cr6,r31,104
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 104, so);
  // beq cr6,0x825b2338
  if (cr6.eq) goto loc_825B2338;
  // cmpwi cr6,r31,108
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 108, so);
  // beq cr6,0x825b2318
  if (cr6.eq) goto loc_825B2318;
  // cmpwi cr6,r31,119
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 119, so);
  // beq cr6,0x825b2330
  if (cr6.eq) goto loc_825B2330;
loc_825B230C:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // extsb r30,r11
  r30 = uint64_t(int64_t(int8_t(r11)));
  // b 0x825b2348
  goto loc_825B2348;
loc_825B2318:
  // addi r11,r19,1
  r11 = uint64_t(int64_t(int64_t(r19) + 1));
  // lbz r10,0(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 0);
  // cmplwi cr6,r10,108
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 108, so);
  // beq cr6,0x825b2288
  if (cr6.eq) goto loc_825B2288;
  // addi r11,r15,1
  r11 = uint64_t(int64_t(int64_t(r15) + 1));
  // extsb r15,r11
  r15 = uint64_t(int64_t(int8_t(r11)));
loc_825B2330:
  // addi r11,r28,1
  r11 = uint64_t(int64_t(int64_t(r28) + 1));
  // b 0x825b2344
  goto loc_825B2344;
loc_825B2338:
  // addi r10,r15,-1
  r10 = uint64_t(int64_t(int64_t(r15) + -1));
  // addi r11,r28,-1
  r11 = uint64_t(int64_t(int64_t(r28) + -1));
  // extsb r15,r10
  r15 = uint64_t(int64_t(int8_t(r10)));
loc_825B2344:
  // extsb r28,r11
  r28 = uint64_t(int64_t(int8_t(r11)));
loc_825B2348:
  // extsb. r11,r30
  r11 = uint64_t(int64_t(int8_t(r30)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b2208
  if (cr0.eq) goto loc_825B2208;
  // extsb. r11,r29
  r11 = uint64_t(int64_t(int8_t(r29)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r19,716(r1)
  guest::StoreU32(base, uint32_t(r1) + 716, uint32_t(r19));
  // stw r11,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r11));
  // bne 0x825b2380
  if (!cr0.eq) goto loc_825B2380;
  // lwz r11,732(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 732);
  // addi r10,r11,7
  r10 = uint64_t(int64_t(int64_t(r11) + 7));
  // stw r11,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r11));
  // rlwinm r11,r10,0,0,28
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFF8;
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // stw r11,732(r1)
  guest::StoreU32(base, uint32_t(r1) + 732, uint32_t(r11));
  // lwz r11,-4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -4);
  // b 0x825b2384
  goto loc_825B2384;
loc_825B2380:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825B2384:
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // extsb. r11,r28
  r11 = uint64_t(int64_t(int8_t(r28)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b23b0
  if (!cr0.eq) goto loc_825B23B0;
  // lbz r11,0(r19)
  r11 = guest::LoadU8(base, uint32_t(r19) + 0);
  // cmplwi cr6,r11,83
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 83, so);
  // beq cr6,0x825b23ac
  if (cr6.eq) goto loc_825B23AC;
  // cmplwi cr6,r11,67
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 67, so);
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // bne cr6,0x825b23b0
  if (!cr6.eq) goto loc_825B23B0;
loc_825B23AC:
  // li r28,1
  r28 = uint64_t(int64_t(1));
loc_825B23B0:
  // lwz r10,716(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 716);
  // lbz r11,0(r10)
  r11 = guest::LoadU8(base, uint32_t(r10) + 0);
  // ori r19,r11,32
  r19 = r11 | 32;
  // cmpwi cr6,r19,110
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 110, so);
  // beq cr6,0x825b2468
  if (cr6.eq) goto loc_825B2468;
  // cmpwi cr6,r19,99
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 99, so);
  // beq cr6,0x825b2428
  if (cr6.eq) goto loc_825B2428;
  // cmpwi cr6,r19,123
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 123, so);
  // beq cr6,0x825b2428
  if (cr6.eq) goto loc_825B2428;
loc_825B23D4:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b23fc
  if (cr0.lt) goto loc_825B23FC;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r31,0(r11)
  r31 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2408
  goto loc_825B2408;
loc_825B23FC:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
loc_825B2408:
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x825b2420
  if (cr6.eq) goto loc_825B2420;
  // clrlwi r3,r31,24
  r3 = uint32_t(r31) & 0xFF;
  // bl 0x825ab080
  // (left out: dead in C++)
  r3 = f825AB080(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b23d4
  if (!cr0.eq) goto loc_825B23D4;
loc_825B2420:
  // mr r23,r31
  r23 = r31;
  // b 0x825b245c
  goto loc_825B245C;
loc_825B2428:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2450
  if (cr0.lt) goto loc_825B2450;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b245c
  goto loc_825B245C;
loc_825B2450:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B245C:
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // beq cr6,0x825b3194
  if (cr6.eq) goto loc_825B3194;
  // lwz r10,716(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 716);
loc_825B2468:
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // beq cr6,0x825b2478
  if (cr6.eq) goto loc_825B2478;
  // cmpwi cr6,r16,0
  cr6 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // beq cr6,0x825b3180
  if (cr6.eq) goto loc_825B3180;
loc_825B2478:
  // addi r11,r19,-99
  r11 = uint64_t(int64_t(int64_t(r19) + -99));
  // cmplwi cr6,r11,24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 24, so);
  // bgt cr6,0x825b304c
  if (cr6.gt) goto loc_825B304C;
  // lis r12,-32249
  r12 = uint64_t(int64_t(-2113470464));
  // addi r12,r12,14208
  r12 = uint64_t(int64_t(int64_t(r12) + 14208));
  // rlwinm r0,r11,1,0,30
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // lhzx r0,r12,r0
  r0 = guest::LoadU16(base, uint32_t(r12) + uint32_t(r0));
  // lis r12,-32165
  r12 = uint64_t(int64_t(-2107965440));
  // addi r12,r12,9388
  r12 = uint64_t(int64_t(int64_t(r12) + 9388));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825B24AC;
  case 1:
  goto loc_825B2948;
  case 2:
  goto loc_825B2C74;
  case 3:
  goto loc_825B2C74;
  case 4:
  goto loc_825B2C74;
  case 5:
  goto loc_825B304C;
  case 6:
  goto loc_825B27E8;
  case 7:
  goto loc_825B304C;
  case 8:
  goto loc_825B304C;
  case 9:
  goto loc_825B304C;
  case 10:
  goto loc_825B304C;
  case 11:
  goto loc_825B2C20;
  case 12:
  goto loc_825B2948;
  case 13:
  goto loc_825B2944;
  case 14:
  goto loc_825B304C;
  case 15:
  goto loc_825B304C;
  case 16:
  goto loc_825B24BC;
  case 17:
  goto loc_825B304C;
  case 18:
  goto loc_825B2948;
  case 19:
  goto loc_825B304C;
  case 20:
  goto loc_825B304C;
  case 21:
  goto loc_825B27EC;
  case 22:
  goto loc_825B304C;
  case 23:
  goto loc_825B304C;
  case 24:
  goto loc_825B24CC;
  default:
  std::abort();
  }
loc_825B24AC:
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // bne cr6,0x825b24bc
  if (!cr6.eq) goto loc_825B24BC;
  // li r17,1
  r17 = uint64_t(int64_t(1));
  // addi r16,r16,1
  r16 = uint64_t(int64_t(int64_t(r16) + 1));
loc_825B24BC:
  // extsb. r11,r28
  r11 = uint64_t(int64_t(int8_t(r28)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x825b260c
  if (!cr0.gt) goto loc_825B260C;
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // b 0x825b260c
  goto loc_825B260C;
loc_825B24CC:
  // extsb. r11,r28
  r11 = uint64_t(int64_t(int8_t(r28)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x825b24d8
  if (!cr0.gt) goto loc_825B24D8;
  // li r24,1
  r24 = uint64_t(int64_t(1));
loc_825B24D8:
  // addi r6,r10,1
  r6 = uint64_t(int64_t(int64_t(r10) + 1));
  // lbz r11,0(r6)
  r11 = guest::LoadU8(base, uint32_t(r6) + 0);
  // stw r6,716(r1)
  guest::StoreU32(base, uint32_t(r1) + 716, uint32_t(r6));
  // cmplwi cr6,r11,94
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 94, so);
  // bne cr6,0x825b24f4
  if (!cr6.eq) goto loc_825B24F4;
  // addi r6,r6,1
  r6 = uint64_t(int64_t(int64_t(r6) + 1));
  // li r25,-1
  r25 = uint64_t(int64_t(-1));
loc_825B24F4:
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // cmpwi cr6,r19,123
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 123, so);
  // std r5,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r5);
  // std r5,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r5);
  // std r5,16(r11)
  guest::StoreU64(base, uint32_t(r11) + 16, r5);
  // std r5,24(r11)
  guest::StoreU64(base, uint32_t(r11) + 24, r5);
  // bne cr6,0x825b2530
  if (!cr6.eq) goto loc_825B2530;
  // lbz r11,0(r6)
  r11 = guest::LoadU8(base, uint32_t(r6) + 0);
  // cmplwi cr6,r11,93
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 93, so);
  // bne cr6,0x825b2530
  if (!cr6.eq) goto loc_825B2530;
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // li r27,93
  r27 = uint64_t(int64_t(93));
  // addi r6,r6,1
  r6 = uint64_t(int64_t(int64_t(r6) + 1));
  // stb r11,155(r1)
  guest::StoreU8(base, uint32_t(r1) + 155, uint8_t(r11));
loc_825B2530:
  // lbz r11,0(r6)
  r11 = guest::LoadU8(base, uint32_t(r6) + 0);
  // cmplwi cr6,r11,93
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 93, so);
  // beq cr6,0x825b25f4
  if (cr6.eq) goto loc_825B25F4;
  // li r4,1
  r4 = uint64_t(int64_t(1));
loc_825B2540:
  // mr r8,r11
  r8 = r11;
  // addi r6,r6,1
  r6 = uint64_t(int64_t(int64_t(r6) + 1));
  // clrlwi r9,r8,24
  r9 = uint32_t(r8) & 0xFF;
  // cmplwi cr6,r9,45
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 45, so);
  // bne cr6,0x825b25c8
  if (!cr6.eq) goto loc_825B25C8;
  // clrlwi. r10,r27,24
  r10 = uint32_t(r27) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825b25c8
  if (cr0.eq) goto loc_825B25C8;
  // lbz r11,0(r6)
  r11 = guest::LoadU8(base, uint32_t(r6) + 0);
  // cmplwi cr6,r11,93
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 93, so);
  // beq cr6,0x825b25c8
  if (cr6.eq) goto loc_825B25C8;
  // clrlwi r9,r11,24
  r9 = uint32_t(r11) & 0xFF;
  // addi r6,r6,1
  r6 = uint64_t(int64_t(int64_t(r6) + 1));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bge cr6,0x825b2580
  if (!cr6.lt) goto loc_825B2580;
  // mr r10,r11
  r10 = r11;
  // b 0x825b2588
  goto loc_825B2588;
loc_825B2580:
  // mr r10,r27
  r10 = r27;
  // mr r27,r11
  r27 = r11;
loc_825B2588:
  // clrlwi r7,r10,24
  r7 = uint32_t(r10) & 0xFF;
  // clrlwi r11,r27,24
  r11 = uint32_t(r27) & 0xFF;
  // b 0x825b25b8
  goto loc_825B25B8;
loc_825B2594:
  // rlwinm r10,r11,29,3,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x1FFFFFFF;
  // addi r8,r11,1
  r8 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r11,r11,29
  r11 = uint32_t(r11) & 0x7;
  // addi r9,r1,144
  r9 = uint64_t(int64_t(int64_t(r1) + 144));
  // slw r3,r4,r11
  r3 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r4) << (uint8_t(r11) & 0x3F));
  // clrlwi r11,r8,24
  r11 = uint32_t(r8) & 0xFF;
  // lbzx r8,r10,r9
  r8 = guest::LoadU8(base, uint32_t(r10) + uint32_t(r9));
  // or r8,r3,r8
  r8 = r3 | r8;
  // stbx r8,r10,r9
  guest::StoreU8(base, uint32_t(r10) + uint32_t(r9), uint8_t(r8));
loc_825B25B8:
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // ble cr6,0x825b2594
  if (!cr6.gt) goto loc_825B2594;
  // mr r27,r5
  r27 = r5;
  // b 0x825b25e8
  goto loc_825B25E8;
loc_825B25C8:
  // rlwinm r11,r9,29,3,31
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 29) & 0x1FFFFFFF;
  // clrlwi r9,r9,29
  r9 = uint32_t(r9) & 0x7;
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r27,r8
  r27 = r8;
  // lbzx r8,r11,r10
  r8 = guest::LoadU8(base, uint32_t(r11) + uint32_t(r10));
  // slw r9,r4,r9
  r9 = uint8_t(r9) & 0x20 ? 0 : (uint32_t(r4) << (uint8_t(r9) & 0x3F));
  // or r9,r9,r8
  r9 = r9 | r8;
  // stbx r9,r11,r10
  guest::StoreU8(base, uint32_t(r11) + uint32_t(r10), uint8_t(r9));
loc_825B25E8:
  // lbz r11,0(r6)
  r11 = guest::LoadU8(base, uint32_t(r6) + 0);
  // cmplwi cr6,r11,93
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 93, so);
  // bne cr6,0x825b2540
  if (!cr6.eq) goto loc_825B2540;
loc_825B25F4:
  // lbz r11,0(r6)
  r11 = guest::LoadU8(base, uint32_t(r6) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x825b3194
  if (cr0.eq) goto loc_825B3194;
  // cmpwi cr6,r19,123
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 123, so);
  // bne cr6,0x825b260c
  if (!cr6.eq) goto loc_825B260C;
  // stw r6,716(r1)
  guest::StoreU32(base, uint32_t(r1) + 716, uint32_t(r6));
loc_825B260C:
  // lwz r30,88(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 88);
  // addi r14,r14,-1
  r14 = uint64_t(int64_t(int64_t(r14) + -1));
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // mr r31,r30
  r31 = r30;
  // beq cr6,0x825b262c
  if (cr6.eq) goto loc_825B262C;
  // mr r4,r26
  r4 = r26;
  // mr r3,r23
  r3 = r23;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B262C:
  // lwz r29,116(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 116);
loc_825B2630:
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // beq cr6,0x825b2644
  if (cr6.eq) goto loc_825B2644;
  // cmpwi cr6,r16,0
  cr6 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // addi r16,r16,-1
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  // beq cr6,0x825b279c
  if (cr6.eq) goto loc_825B279C;
loc_825B2644:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b266c
  if (cr0.lt) goto loc_825B266C;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2678
  goto loc_825B2678;
loc_825B266C:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2678:
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // beq cr6,0x825b2784
  if (cr6.eq) goto loc_825B2784;
  // cmpwi cr6,r19,99
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 99, so);
  // beq cr6,0x825b26dc
  if (cr6.eq) goto loc_825B26DC;
  // cmpwi cr6,r19,115
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 115, so);
  // bne cr6,0x825b26a8
  if (!cr6.eq) goto loc_825B26A8;
  // cmpwi cr6,r23,9
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 9, so);
  // blt cr6,0x825b26a0
  if (cr6.lt) goto loc_825B26A0;
  // cmpwi cr6,r23,13
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 13, so);
  // ble cr6,0x825b2784
  if (!cr6.gt) goto loc_825B2784;
loc_825B26A0:
  // cmpwi cr6,r23,32
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 32, so);
  // bne cr6,0x825b26dc
  if (!cr6.eq) goto loc_825B26DC;
loc_825B26A8:
  // cmpwi cr6,r19,123
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 123, so);
  // bne cr6,0x825b2784
  if (!cr6.eq) goto loc_825B2784;
  // srawi r9,r23,3
  ca = (int32_t(r23) < 0) & ((uint32_t(r23) & 0x7) != 0);
  r9 = uint64_t(int64_t(int32_t(r23) >> 3));
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // clrlwi r11,r23,29
  r11 = uint32_t(r23) & 0x7;
  // extsb r8,r25
  r8 = uint64_t(int64_t(int8_t(r25)));
  // lbzx r10,r9,r10
  r10 = guest::LoadU8(base, uint32_t(r9) + uint32_t(r10));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // xor r10,r10,r8
  r10 = r10 ^ r8;
  // slw r11,r9,r11
  r11 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r9) << (uint8_t(r11) & 0x3F));
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b2784
  if (cr0.eq) goto loc_825B2784;
loc_825B26DC:
  // lwz r11,108(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 108);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b277c
  if (!cr6.eq) goto loc_825B277C;
  // extsb. r11,r24
  r11 = uint64_t(int64_t(int8_t(r24)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b276c
  if (cr0.eq) goto loc_825B276C;
  // extsb r11,r23
  r11 = uint64_t(int64_t(int8_t(r23)));
  // clrlwi r3,r11,24
  r3 = uint32_t(r11) & 0xFF;
  // stb r11,112(r1)
  guest::StoreU8(base, uint32_t(r1) + 112, uint8_t(r11));
  // bl 0x825b6898
  // (left out: dead in C++)
  { const lifted::Result o = game::isleadbyte(base, LIFTED_ARGS(0x825B2700, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b273c
  if (cr0.eq) goto loc_825B273C;
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2730
  if (cr0.lt) goto loc_825B2730;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r3,0(r11)
  r3 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2738
  goto loc_825B2738;
loc_825B2730:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B2738:
  // stb r3,113(r1)
  guest::StoreU8(base, uint32_t(r1) + 113, uint8_t(r3));
loc_825B273C:
  // li r11,63
  r11 = uint64_t(int64_t(63));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r6,r29
  r6 = r29;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // sth r11,96(r1)
  guest::StoreU16(base, uint32_t(r1) + 96, uint16_t(r11));
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r5,172(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 172);
  // bl 0x825b7ec8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B7EC8, LIFTED_ARGS(0x825B275C, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,96(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 96);
  // sth r11,0(r30)
  guest::StoreU16(base, uint32_t(r30) + 0, uint16_t(r11));
  // addi r30,r30,2
  r30 = uint64_t(int64_t(int64_t(r30) + 2));
  // b 0x825b2774
  goto loc_825B2774;
loc_825B276C:
  // stb r23,0(r30)
  guest::StoreU8(base, uint32_t(r30) + 0, uint8_t(r23));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
loc_825B2774:
  // stw r30,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r30));
  // b 0x825b2630
  goto loc_825B2630;
loc_825B277C:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // b 0x825b2630
  goto loc_825B2630;
loc_825B2784:
  // addi r14,r14,-1
  r14 = uint64_t(int64_t(int64_t(r14) + -1));
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // beq cr6,0x825b279c
  if (cr6.eq) goto loc_825B279C;
  // mr r4,r26
  r4 = r26;
  // mr r3,r23
  r3 = r23;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B279C:
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825b3194
  if (cr6.eq) goto loc_825B3194;
  // lwz r11,108(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 108);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b3078
  if (!cr6.eq) goto loc_825B3078;
  // lwz r11,92(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 92);
  // cmpwi cr6,r19,99
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 99, so);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // beq cr6,0x825b3078
  if (cr6.eq) goto loc_825B3078;
  // lwz r10,88(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 88);
  // extsb. r11,r24
  r11 = uint64_t(int64_t(int8_t(r24)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b27dc
  if (cr0.eq) goto loc_825B27DC;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // sth r11,0(r10)
  guest::StoreU16(base, uint32_t(r10) + 0, uint16_t(r11));
  // b 0x825b3078
  goto loc_825B3078;
loc_825B27DC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stb r11,0(r10)
  guest::StoreU8(base, uint32_t(r10) + 0, uint8_t(r11));
  // b 0x825b3078
  goto loc_825B3078;
loc_825B27E8:
  // li r19,100
  r19 = uint64_t(int64_t(100));
loc_825B27EC:
  // cmpwi cr6,r23,45
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 45, so);
  // bne cr6,0x825b27fc
  if (!cr6.eq) goto loc_825B27FC;
  // li r20,1
  r20 = uint64_t(int64_t(1));
  // b 0x825b2804
  goto loc_825B2804;
loc_825B27FC:
  // cmpwi cr6,r23,43
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 43, so);
  // bne cr6,0x825b2850
  if (!cr6.eq) goto loc_825B2850;
loc_825B2804:
  // addic. r16,r16,-1
  ca = uint32_t(r16) > 0;
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // bne 0x825b281c
  if (!cr0.eq) goto loc_825B281C;
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // beq cr6,0x825b281c
  if (cr6.eq) goto loc_825B281C;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825b2850
  goto loc_825B2850;
loc_825B281C:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2844
  if (cr0.lt) goto loc_825B2844;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2850
  goto loc_825B2850;
loc_825B2844:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2850:
  // cmpwi cr6,r23,48
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 48, so);
  // bne cr6,0x825b29ac
  if (!cr6.eq) goto loc_825B29AC;
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2880
  if (cr0.lt) goto loc_825B2880;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b288c
  goto loc_825B288C;
loc_825B2880:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B288C:
  // extsb r11,r23
  r11 = uint64_t(int64_t(int8_t(r23)));
  // cmpwi cr6,r11,120
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 120, so);
  // beq cr6,0x825b28ec
  if (cr6.eq) goto loc_825B28EC;
  // cmpwi cr6,r11,88
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 88, so);
  // beq cr6,0x825b28ec
  if (cr6.eq) goto loc_825B28EC;
  // li r18,1
  r18 = uint64_t(int64_t(1));
  // cmpwi cr6,r19,120
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 120, so);
  // beq cr6,0x825b28cc
  if (cr6.eq) goto loc_825B28CC;
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // beq cr6,0x825b28c4
  if (cr6.eq) goto loc_825B28C4;
  // addic. r16,r16,-1
  ca = uint32_t(r16) > 0;
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // bne 0x825b28c4
  if (!cr0.eq) goto loc_825B28C4;
  // addi r11,r29,1
  r11 = uint64_t(int64_t(int64_t(r29) + 1));
  // extsb r29,r11
  r29 = uint64_t(int64_t(int8_t(r11)));
loc_825B28C4:
  // li r19,111
  r19 = uint64_t(int64_t(111));
  // b 0x825b29ac
  goto loc_825B29AC;
loc_825B28CC:
  // addi r14,r14,-1
  r14 = uint64_t(int64_t(int64_t(r14) + -1));
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // beq cr6,0x825b28e4
  if (cr6.eq) goto loc_825B28E4;
  // mr r4,r26
  r4 = r26;
  // mr r3,r23
  r3 = r23;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B28E4:
  // li r23,48
  r23 = uint64_t(int64_t(48));
  // b 0x825b29ac
  goto loc_825B29AC;
loc_825B28EC:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2914
  if (cr0.lt) goto loc_825B2914;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2920
  goto loc_825B2920;
loc_825B2914:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2920:
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // beq cr6,0x825b293c
  if (cr6.eq) goto loc_825B293C;
  // addi r16,r16,-2
  r16 = uint64_t(int64_t(int64_t(r16) + -2));
  // cmpwi cr6,r16,1
  cr6 = lifted::Compare<int32_t>(int32_t(r16), 1, so);
  // bge cr6,0x825b293c
  if (!cr6.lt) goto loc_825B293C;
  // addi r11,r29,1
  r11 = uint64_t(int64_t(int64_t(r29) + 1));
  // extsb r29,r11
  r29 = uint64_t(int64_t(int8_t(r11)));
loc_825B293C:
  // li r19,120
  r19 = uint64_t(int64_t(120));
  // b 0x825b29ac
  goto loc_825B29AC;
loc_825B2944:
  // li r15,1
  r15 = uint64_t(int64_t(1));
loc_825B2948:
  // cmpwi cr6,r23,45
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 45, so);
  // bne cr6,0x825b2958
  if (!cr6.eq) goto loc_825B2958;
  // li r20,1
  r20 = uint64_t(int64_t(1));
  // b 0x825b2960
  goto loc_825B2960;
loc_825B2958:
  // cmpwi cr6,r23,43
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 43, so);
  // bne cr6,0x825b29ac
  if (!cr6.eq) goto loc_825B29AC;
loc_825B2960:
  // addic. r16,r16,-1
  ca = uint32_t(r16) > 0;
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // bne 0x825b2978
  if (!cr0.eq) goto loc_825B2978;
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // beq cr6,0x825b2978
  if (cr6.eq) goto loc_825B2978;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825b29ac
  goto loc_825B29AC;
loc_825B2978:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b29a0
  if (cr0.lt) goto loc_825B29A0;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b29ac
  goto loc_825B29AC;
loc_825B29A0:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B29AC:
  // cmpwi cr6,r21,0
  cr6 = lifted::Compare<int32_t>(int32_t(r21), 0, so);
  // extsb. r30,r29
  r30 = uint64_t(int64_t(int8_t(r29)));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x825b2ae0
  if (cr6.eq) goto loc_825B2AE0;
  // bne 0x825b2ac8
  if (!cr0.eq) goto loc_825B2AC8;
  // ld r28,120(r1)
  r28 = guest::LoadU64(base, uint32_t(r1) + 120);
loc_825B29C0:
  // cmpwi cr6,r19,120
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 120, so);
  // beq cr6,0x825b2a00
  if (cr6.eq) goto loc_825B2A00;
  // cmpwi cr6,r19,112
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 112, so);
  // beq cr6,0x825b2a00
  if (cr6.eq) goto loc_825B2A00;
  // clrlwi r3,r23,24
  r3 = uint32_t(r23) & 0xFF;
  // bl 0x825ab040
  // (left out: dead in C++)
  { const lifted::Result o = game::isdigit(base, LIFTED_ARGS(0x825B29D8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b2a34
  if (cr0.eq) goto loc_825B2A34;
  // cmpwi cr6,r19,111
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 111, so);
  // bne cr6,0x825b29f8
  if (!cr6.eq) goto loc_825B29F8;
  // cmpwi cr6,r23,56
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 56, so);
  // bge cr6,0x825b2a34
  if (!cr6.lt) goto loc_825B2A34;
  // rldicr r28,r28,3,60
  r28 = __builtin_rotateleft64(r28, 3) & 0xFFFFFFFFFFFFFFF8;
  // b 0x825b2a3c
  goto loc_825B2A3C;
loc_825B29F8:
  // mulli r28,r28,10
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(10))));
  // b 0x825b2a3c
  goto loc_825B2A3C;
loc_825B2A00:
  // clrlwi r31,r23,24
  r31 = uint32_t(r23) & 0xFF;
  // mr r3,r31
  r3 = r31;
  // bl 0x825ab060
  // (left out: dead in C++)
  r3 = f825AB060(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b2a34
  if (cr0.eq) goto loc_825B2A34;
  // mr r3,r31
  r3 = r31;
  // rldicr r28,r28,4,59
  r28 = __builtin_rotateleft64(r28, 4) & 0xFFFFFFFFFFFFFFF0;
  // bl 0x825ab040
  // (left out: dead in C++)
  { const lifted::Result o = game::isdigit(base, LIFTED_ARGS(0x825B2A20, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b2a3c
  if (!cr0.eq) goto loc_825B2A3C;
  // rlwinm r11,r23,0,27,25
  r11 = __builtin_rotateleft64(uint32_t(r23) | (r23 << 32), 0) & 0xFFFFFFFFFFFFFFDF;
  // addi r23,r11,-7
  r23 = uint64_t(int64_t(int64_t(r11) + -7));
  // b 0x825b2a3c
  goto loc_825B2A3C;
loc_825B2A34:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // extsb r29,r11
  r29 = uint64_t(int64_t(int8_t(r11)));
loc_825B2A3C:
  // extsb. r11,r29
  r11 = uint64_t(int64_t(int8_t(r29)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b2aa4
  if (!cr0.eq) goto loc_825B2AA4;
  // addi r11,r23,-48
  r11 = uint64_t(int64_t(int64_t(r23) + -48));
  // addi r18,r18,1
  r18 = uint64_t(int64_t(int64_t(r18) + 1));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // add r28,r11,r28
  r28 = r11 + r28;
  // beq cr6,0x825b2a6c
  if (cr6.eq) goto loc_825B2A6C;
  // addic. r16,r16,-1
  ca = uint32_t(r16) > 0;
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // bne 0x825b2a6c
  if (!cr0.eq) goto loc_825B2A6C;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825b2abc
  goto loc_825B2ABC;
loc_825B2A6C:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2a94
  if (cr0.lt) goto loc_825B2A94;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2abc
  goto loc_825B2ABC;
loc_825B2A94:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // b 0x825b2abc
  goto loc_825B2ABC;
loc_825B2AA4:
  // addi r14,r14,-1
  r14 = uint64_t(int64_t(int64_t(r14) + -1));
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // beq cr6,0x825b2abc
  if (cr6.eq) goto loc_825B2ABC;
  // mr r4,r26
  r4 = r26;
  // mr r3,r23
  r3 = r23;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B2ABC:
  // extsb. r30,r29
  r30 = uint64_t(int64_t(int8_t(r29)));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x825b29c0
  if (cr0.eq) goto loc_825B29C0;
  // std r28,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r28);
loc_825B2AC8:
  // extsb. r11,r20
  r11 = uint64_t(int64_t(int8_t(r20)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b2bf0
  if (cr0.eq) goto loc_825B2BF0;
  // ld r11,120(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 120);
  // neg r11,r11
  r11 = uint64_t(int64_t(static_cast<int64_t>(-r11)));
  // std r11,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r11);
  // b 0x825b2bf0
  goto loc_825B2BF0;
loc_825B2AE0:
  // bne 0x825b2be4
  if (!cr0.eq) goto loc_825B2BE4;
loc_825B2AE4:
  // cmpwi cr6,r19,120
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 120, so);
  // beq cr6,0x825b2b24
  if (cr6.eq) goto loc_825B2B24;
  // cmpwi cr6,r19,112
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 112, so);
  // beq cr6,0x825b2b24
  if (cr6.eq) goto loc_825B2B24;
  // clrlwi r3,r23,24
  r3 = uint32_t(r23) & 0xFF;
  // bl 0x825ab040
  // (left out: dead in C++)
  { const lifted::Result o = game::isdigit(base, LIFTED_ARGS(0x825B2AFC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b2b58
  if (cr0.eq) goto loc_825B2B58;
  // cmpwi cr6,r19,111
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 111, so);
  // bne cr6,0x825b2b1c
  if (!cr6.eq) goto loc_825B2B1C;
  // cmpwi cr6,r23,56
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 56, so);
  // bge cr6,0x825b2b58
  if (!cr6.lt) goto loc_825B2B58;
  // rlwinm r22,r22,3,0,28
  r22 = __builtin_rotateleft64(uint32_t(r22) | (r22 << 32), 3) & 0xFFFFFFF8;
  // b 0x825b2b60
  goto loc_825B2B60;
loc_825B2B1C:
  // mulli r22,r22,10
  r22 = uint64_t(int64_t(static_cast<int64_t>(r22 * static_cast<uint64_t>(10))));
  // b 0x825b2b60
  goto loc_825B2B60;
loc_825B2B24:
  // clrlwi r31,r23,24
  r31 = uint32_t(r23) & 0xFF;
  // mr r3,r31
  r3 = r31;
  // bl 0x825ab060
  // (left out: dead in C++)
  r3 = f825AB060(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b2b58
  if (cr0.eq) goto loc_825B2B58;
  // mr r3,r31
  r3 = r31;
  // rlwinm r22,r22,4,0,27
  r22 = __builtin_rotateleft64(uint32_t(r22) | (r22 << 32), 4) & 0xFFFFFFF0;
  // bl 0x825ab040
  // (left out: dead in C++)
  { const lifted::Result o = game::isdigit(base, LIFTED_ARGS(0x825B2B44, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b2b60
  if (!cr0.eq) goto loc_825B2B60;
  // rlwinm r11,r23,0,27,25
  r11 = __builtin_rotateleft64(uint32_t(r23) | (r23 << 32), 0) & 0xFFFFFFFFFFFFFFDF;
  // addi r23,r11,-7
  r23 = uint64_t(int64_t(int64_t(r11) + -7));
  // b 0x825b2b60
  goto loc_825B2B60;
loc_825B2B58:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // extsb r29,r11
  r29 = uint64_t(int64_t(int8_t(r11)));
loc_825B2B60:
  // extsb. r11,r29
  r11 = uint64_t(int64_t(int8_t(r29)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b2bc4
  if (!cr0.eq) goto loc_825B2BC4;
  // add r11,r22,r23
  r11 = r22 + r23;
  // addi r18,r18,1
  r18 = uint64_t(int64_t(int64_t(r18) + 1));
  // addi r22,r11,-48
  r22 = uint64_t(int64_t(int64_t(r11) + -48));
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // beq cr6,0x825b2b8c
  if (cr6.eq) goto loc_825B2B8C;
  // addic. r16,r16,-1
  ca = uint32_t(r16) > 0;
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // bne 0x825b2b8c
  if (!cr0.eq) goto loc_825B2B8C;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825b2bdc
  goto loc_825B2BDC;
loc_825B2B8C:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2bb4
  if (cr0.lt) goto loc_825B2BB4;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2bdc
  goto loc_825B2BDC;
loc_825B2BB4:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // b 0x825b2bdc
  goto loc_825B2BDC;
loc_825B2BC4:
  // addi r14,r14,-1
  r14 = uint64_t(int64_t(int64_t(r14) + -1));
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // beq cr6,0x825b2bdc
  if (cr6.eq) goto loc_825B2BDC;
  // mr r4,r26
  r4 = r26;
  // mr r3,r23
  r3 = r23;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B2BDC:
  // extsb. r30,r29
  r30 = uint64_t(int64_t(int8_t(r29)));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x825b2ae4
  if (cr0.eq) goto loc_825B2AE4;
loc_825B2BE4:
  // extsb. r11,r20
  r11 = uint64_t(int64_t(int8_t(r20)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b2bf0
  if (cr0.eq) goto loc_825B2BF0;
  // neg r22,r22
  r22 = uint64_t(int64_t(static_cast<int64_t>(-r22)));
loc_825B2BF0:
  // cmpwi cr6,r19,70
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 70, so);
  // bne cr6,0x825b2bfc
  if (!cr6.eq) goto loc_825B2BFC;
  // li r18,0
  r18 = uint64_t(int64_t(0));
loc_825B2BFC:
  // cmpwi cr6,r18,0
  cr6 = lifted::Compare<int32_t>(int32_t(r18), 0, so);
  // beq cr6,0x825b3194
  if (cr6.eq) goto loc_825B3194;
  // lwz r11,108(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 108);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b3078
  if (!cr6.eq) goto loc_825B3078;
  // lwz r11,92(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 92);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // b 0x825b2c3c
  goto loc_825B2C3C;
loc_825B2C20:
  // lwz r11,108(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 108);
  // mr r22,r14
  r22 = r14;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b3078
  if (!cr6.eq) goto loc_825B3078;
  // bl 0x825ac108
  // (left out: dead in C++)
  r3 = f825AC108(base);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b3078
  if (cr0.eq) goto loc_825B3078;
loc_825B2C3C:
  // cmpwi cr6,r21,0
  cr6 = lifted::Compare<int32_t>(int32_t(r21), 0, so);
  // beq cr6,0x825b2c54
  if (cr6.eq) goto loc_825B2C54;
  // ld r11,120(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 120);
  // lwz r10,88(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 88);
  // std r11,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r11);
  // b 0x825b3078
  goto loc_825B3078;
loc_825B2C54:
  // extsb. r11,r15
  r11 = uint64_t(int64_t(int8_t(r15)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b2c68
  if (cr0.eq) goto loc_825B2C68;
  // lwz r11,88(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 88);
  // stw r22,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r22));
  // b 0x825b3078
  goto loc_825B3078;
loc_825B2C68:
  // lwz r10,88(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 88);
  // sth r22,0(r10)
  guest::StoreU16(base, uint32_t(r10) + 0, uint16_t(r22));
  // b 0x825b3078
  goto loc_825B3078;
loc_825B2C74:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmpwi cr6,r23,45
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 45, so);
  // bne cr6,0x825b2c94
  if (!cr6.eq) goto loc_825B2C94;
  // lwz r10,84(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 84);
  // li r11,45
  r11 = uint64_t(int64_t(45));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // stb r11,0(r10)
  guest::StoreU8(base, uint32_t(r10) + 0, uint8_t(r11));
  // b 0x825b2c9c
  goto loc_825B2C9C;
loc_825B2C94:
  // cmpwi cr6,r23,43
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 43, so);
  // bne cr6,0x825b2cd4
  if (!cr6.eq) goto loc_825B2CD4;
loc_825B2C9C:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r16,r16,-1
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2cc8
  if (cr0.lt) goto loc_825B2CC8;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2cd4
  goto loc_825B2CD4;
loc_825B2CC8:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2CD4:
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // bne cr6,0x825b2d54
  if (!cr6.eq) goto loc_825B2D54;
  // li r16,-1
  r16 = uint64_t(int64_t(-1));
  // b 0x825b2d54
  goto loc_825B2D54;
loc_825B2CE4:
  // cmpwi cr6,r16,0
  cr6 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // addi r16,r16,-1
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  // beq cr6,0x825b2d68
  if (cr6.eq) goto loc_825B2D68;
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // addi r7,r1,100
  r7 = uint64_t(int64_t(int64_t(r1) + 100));
  // addi r6,r1,176
  r6 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // addi r18,r18,1
  r18 = uint64_t(int64_t(int64_t(r18) + 1));
  // stbx r31,r30,r11
  guest::StoreU8(base, uint32_t(r30) + uint32_t(r11), uint8_t(r31));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r30
  r3 = r30;
  // bl 0x825b1f18
  // (left out: dead in C++)
  { const lifted::Result o = f825B1F18(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b3194
  if (cr0.eq) goto loc_825B3194;
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2d48
  if (cr0.lt) goto loc_825B2D48;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2d54
  goto loc_825B2D54;
loc_825B2D48:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2D54:
  // clrlwi r31,r23,24
  r31 = uint32_t(r23) & 0xFF;
  // mr r3,r31
  r3 = r31;
  // bl 0x825ab040
  // (left out: dead in C++)
  { const lifted::Result o = game::isdigit(base, LIFTED_ARGS(0x825B2D60, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b2ce4
  if (!cr0.eq) goto loc_825B2CE4;
loc_825B2D68:
  // lwz r11,116(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 116);
  // extsb r10,r23
  r10 = uint64_t(int64_t(int8_t(r23)));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,188(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 188);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lbz r31,0(r11)
  r31 = guest::LoadU8(base, uint32_t(r11) + 0);
  // extsb r11,r31
  r11 = uint64_t(int64_t(int8_t(r31)));
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // bne cr6,0x825b2e80
  if (!cr6.eq) goto loc_825B2E80;
  // cmpwi cr6,r16,0
  cr6 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // addi r16,r16,-1
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  // beq cr6,0x825b2e80
  if (cr6.eq) goto loc_825B2E80;
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2dc0
  if (cr0.lt) goto loc_825B2DC0;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2dcc
  goto loc_825B2DCC;
loc_825B2DC0:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2DCC:
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // addi r7,r1,100
  r7 = uint64_t(int64_t(int64_t(r1) + 100));
  // addi r6,r1,176
  r6 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // stbx r31,r30,r11
  guest::StoreU8(base, uint32_t(r30) + uint32_t(r11), uint8_t(r31));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r30
  r3 = r30;
  // bl 0x825b1f18
  // (left out: dead in C++)
  { const lifted::Result o = f825B1F18(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b3194
  if (cr0.eq) goto loc_825B3194;
  // b 0x825b2e6c
  goto loc_825B2E6C;
loc_825B2DFC:
  // cmpwi cr6,r16,0
  cr6 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // addi r16,r16,-1
  r16 = uint64_t(int64_t(int64_t(r16) + -1));
  // beq cr6,0x825b2e80
  if (cr6.eq) goto loc_825B2E80;
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // addi r7,r1,100
  r7 = uint64_t(int64_t(int64_t(r1) + 100));
  // addi r6,r1,176
  r6 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // addi r18,r18,1
  r18 = uint64_t(int64_t(int64_t(r18) + 1));
  // stbx r31,r30,r11
  guest::StoreU8(base, uint32_t(r30) + uint32_t(r11), uint8_t(r31));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r30
  r3 = r30;
  // bl 0x825b1f18
  // (left out: dead in C++)
  { const lifted::Result o = f825B1F18(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b3194
  if (cr0.eq) goto loc_825B3194;
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2e60
  if (cr0.lt) goto loc_825B2E60;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2e6c
  goto loc_825B2E6C;
loc_825B2E60:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2E6C:
  // clrlwi r31,r23,24
  r31 = uint32_t(r23) & 0xFF;
  // mr r3,r31
  r3 = r31;
  // bl 0x825ab040
  // (left out: dead in C++)
  { const lifted::Result o = game::isdigit(base, LIFTED_ARGS(0x825B2E78, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b2dfc
  if (!cr0.eq) goto loc_825B2DFC;
loc_825B2E80:
  // cmpwi cr6,r18,0
  cr6 = lifted::Compare<int32_t>(int32_t(r18), 0, so);
  // beq cr6,0x825b2fe4
  if (cr6.eq) goto loc_825B2FE4;
  // cmpwi cr6,r23,101
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 101, so);
  // beq cr6,0x825b2e98
  if (cr6.eq) goto loc_825B2E98;
  // cmpwi cr6,r23,69
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 69, so);
  // bne cr6,0x825b2fe4
  if (!cr6.eq) goto loc_825B2FE4;
loc_825B2E98:
  // cmpwi cr6,r16,0
  cr6 = lifted::Compare<int32_t>(int32_t(r16), 0, so);
  // addi r29,r16,-1
  r29 = uint64_t(int64_t(int64_t(r16) + -1));
  // beq cr6,0x825b2fe4
  if (cr6.eq) goto loc_825B2FE4;
  // lwz r10,84(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 84);
  // li r11,101
  r11 = uint64_t(int64_t(101));
  // addi r7,r1,100
  r7 = uint64_t(int64_t(int64_t(r1) + 100));
  // addi r6,r1,176
  r6 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // stbx r11,r30,r10
  guest::StoreU8(base, uint32_t(r30) + uint32_t(r10), uint8_t(r11));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r30
  r3 = r30;
  // bl 0x825b1f18
  // (left out: dead in C++)
  { const lifted::Result o = f825B1F18(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b3194
  if (cr0.eq) goto loc_825B3194;
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2efc
  if (cr0.lt) goto loc_825B2EFC;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2f08
  goto loc_825B2F08;
loc_825B2EFC:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2F08:
  // cmpwi cr6,r23,45
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 45, so);
  // bne cr6,0x825b2f44
  if (!cr6.eq) goto loc_825B2F44;
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // li r10,45
  r10 = uint64_t(int64_t(45));
  // addi r7,r1,100
  r7 = uint64_t(int64_t(int64_t(r1) + 100));
  // addi r6,r1,176
  r6 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // stbx r10,r30,r11
  guest::StoreU8(base, uint32_t(r30) + uint32_t(r11), uint8_t(r10));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r30
  r3 = r30;
  // bl 0x825b1f18
  // (left out: dead in C++)
  { const lifted::Result o = f825B1F18(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b3194
  if (cr0.eq) goto loc_825B3194;
  // b 0x825b2f4c
  goto loc_825B2F4C;
loc_825B2F44:
  // cmpwi cr6,r23,43
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 43, so);
  // bne cr6,0x825b2fd0
  if (!cr6.eq) goto loc_825B2FD0;
loc_825B2F4C:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // bne cr6,0x825b2f9c
  if (!cr6.eq) goto loc_825B2F9C;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // b 0x825b2fd0
  goto loc_825B2FD0;
loc_825B2F60:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // beq cr6,0x825b2fe4
  if (cr6.eq) goto loc_825B2FE4;
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // addi r7,r1,100
  r7 = uint64_t(int64_t(int64_t(r1) + 100));
  // addi r6,r1,176
  r6 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // addi r18,r18,1
  r18 = uint64_t(int64_t(int64_t(r18) + 1));
  // stbx r31,r30,r11
  guest::StoreU8(base, uint32_t(r30) + uint32_t(r11), uint8_t(r31));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r30
  r3 = r30;
  // bl 0x825b1f18
  // (left out: dead in C++)
  { const lifted::Result o = f825B1F18(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b3194
  if (cr0.eq) goto loc_825B3194;
loc_825B2F9C:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b2fc4
  if (cr0.lt) goto loc_825B2FC4;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b2fd0
  goto loc_825B2FD0;
loc_825B2FC4:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B2FD0:
  // clrlwi r31,r23,24
  r31 = uint32_t(r23) & 0xFF;
  // mr r3,r31
  r3 = r31;
  // bl 0x825ab040
  // (left out: dead in C++)
  { const lifted::Result o = game::isdigit(base, LIFTED_ARGS(0x825B2FDC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b2f60
  if (!cr0.eq) goto loc_825B2F60;
loc_825B2FE4:
  // addi r14,r14,-1
  r14 = uint64_t(int64_t(int64_t(r14) + -1));
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // beq cr6,0x825b2ffc
  if (cr6.eq) goto loc_825B2FFC;
  // mr r4,r26
  r4 = r26;
  // mr r3,r23
  r3 = r23;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B2FFC:
  // cmpwi cr6,r18,0
  cr6 = lifted::Compare<int32_t>(int32_t(r18), 0, so);
  // beq cr6,0x825b3194
  if (cr6.eq) goto loc_825B3194;
  // lwz r11,108(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 108);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825b3078
  if (!cr6.eq) goto loc_825B3078;
  // lwz r10,92(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 92);
  // extsb r11,r15
  r11 = uint64_t(int64_t(int8_t(r15)));
  // lwz r5,84(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 84);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // lwz r6,116(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 116);
  // lwz r4,88(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 88);
  // addi r3,r11,-1
  r3 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stbx r10,r30,r5
  guest::StoreU8(base, uint32_t(r30) + uint32_t(r5), uint8_t(r10));
  // lwz r10,128(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 128);
  // lwz r10,28(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 28);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825B3048, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825b3078
  goto loc_825B3078;
loc_825B304C:
  // lbz r11,0(r10)
  r11 = guest::LoadU8(base, uint32_t(r10) + 0);
  // cmpw cr6,r11,r23
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r23), so);
  // bne cr6,0x825b3180
  if (!cr6.eq) goto loc_825B3180;
  // lbz r11,80(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 80);
  // lwz r10,108(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 108);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stb r11,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r11));
  // bne cr6,0x825b3078
  if (!cr6.eq) goto loc_825B3078;
  // lwz r11,132(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 132);
  // stw r11,732(r1)
  guest::StoreU32(base, uint32_t(r1) + 732, uint32_t(r11));
loc_825B3078:
  // lbz r11,80(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 80);
  // lwz r10,716(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 716);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // rotlwi r19,r10,0
  r19 = __builtin_rotateleft32(uint32_t(r10), 0);
  // stb r11,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r11));
  // stw r10,716(r1)
  guest::StoreU32(base, uint32_t(r1) + 716, uint32_t(r10));
  // b 0x825b3138
  goto loc_825B3138;
loc_825B3098:
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r14,r14,1
  r14 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b30c0
  if (cr0.lt) goto loc_825B30C0;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r23,0(r11)
  r23 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b30cc
  goto loc_825B30CC;
loc_825B30C0:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_825B30CC:
  // lbz r11,0(r19)
  r11 = guest::LoadU8(base, uint32_t(r19) + 0);
  // addi r19,r19,1
  r19 = uint64_t(int64_t(int64_t(r19) + 1));
  // cmpw cr6,r11,r23
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r23), so);
  // stw r19,716(r1)
  guest::StoreU32(base, uint32_t(r1) + 716, uint32_t(r19));
  // bne cr6,0x825b3180
  if (!cr6.eq) goto loc_825B3180;
  // clrlwi r3,r23,24
  r3 = uint32_t(r23) & 0xFF;
  // bl 0x825b6898
  // (left out: dead in C++)
  { const lifted::Result o = game::isleadbyte(base, LIFTED_ARGS(0x825B30E8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b3138
  if (cr0.eq) goto loc_825B3138;
  // lwz r11,4(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 4);
  // addi r31,r14,1
  r31 = uint64_t(int64_t(int64_t(r14) + 1));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // blt 0x825b3118
  if (cr0.lt) goto loc_825B3118;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r3,0(r11)
  r3 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r10));
  // b 0x825b3120
  goto loc_825B3120;
loc_825B3118:
  // mr r3,r26
  r3 = r26;
  // bl 0x825b4490
  // (left out: dead in C++)
  { const lifted::Result o = f825B4490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B3120:
  // lbz r11,0(r19)
  r11 = guest::LoadU8(base, uint32_t(r19) + 0);
  // addi r19,r19,1
  r19 = uint64_t(int64_t(int64_t(r19) + 1));
  // cmpw cr6,r11,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r3), so);
  // stw r19,716(r1)
  guest::StoreU32(base, uint32_t(r1) + 716, uint32_t(r19));
  // bne cr6,0x825b3170
  if (!cr6.eq) goto loc_825B3170;
  // addi r14,r31,-1
  r14 = uint64_t(int64_t(int64_t(r31) + -1));
loc_825B3138:
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // bne cr6,0x825b3160
  if (!cr6.eq) goto loc_825B3160;
  // lbz r11,0(r19)
  r11 = guest::LoadU8(base, uint32_t(r19) + 0);
  // cmplwi cr6,r11,37
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 37, so);
  // bne cr6,0x825b3194
  if (!cr6.eq) goto loc_825B3194;
  // lwz r11,716(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 716);
  // lbz r11,1(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 1);
  // cmplwi cr6,r11,110
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 110, so);
  // bne cr6,0x825b3194
  if (!cr6.eq) goto loc_825B3194;
  // lwz r19,716(r1)
  r19 = guest::LoadU32(base, uint32_t(r1) + 716);
loc_825B3160:
  // lbz r3,0(r19)
  r3 = guest::LoadU8(base, uint32_t(r19) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x825b2140
  if (!cr0.eq) goto loc_825B2140;
  // b 0x825b3194
  goto loc_825B3194;
loc_825B3170:
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x825b3180
  if (cr6.eq) goto loc_825B3180;
  // mr r4,r26
  r4 = r26;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B3180:
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // beq cr6,0x825b3194
  if (cr6.eq) goto loc_825B3194;
  // mr r4,r26
  r4 = r26;
  // mr r3,r23
  r3 = r23;
  // bl 0x825aca90
  // (left out: dead in C++)
  { const lifted::Result o = f825ACA90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825B3194:
  // lwz r11,100(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 100);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x825b31a8
  if (!cr6.eq) goto loc_825B31A8;
  // lwz r3,84(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x825a9b58
  // (left out: dead in C++)
  { const lifted::Result o = game::free(base, LIFTED_ARGS(0x825B31A8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825B31A8:
  // cmpwi cr6,r23,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r23), -1, so);
  // bne cr6,0x825b31cc
  if (!cr6.eq) goto loc_825B31CC;
  // lwz r3,92(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 92);
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825b31d0
  if (!cr6.eq) goto loc_825B31D0;
  // lbz r11,80(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 80);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x825b31d0
  if (!cr0.eq) goto loc_825B31D0;
  // b 0x825b2020
  goto loc_825B2020;
loc_825B31CC:
  // lwz r3,92(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 92);
loc_825B31D0:
  // addi r1,r1,688
  r1 = uint64_t(int64_t(int64_t(r1) + 688));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825B4490: lifted from generated/ by lift.py.
lifted::Result f825B4490(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825B4490 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
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
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x825b44dc
  if (!cr6.eq) goto loc_825B44DC;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B44B4, r31)); r3 = o.r3; f1 = o.f1; }
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::_invalid_parameter(base, LIFTED_ARGS(0x825B44D8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825b464c
  goto loc_825B464C;
loc_825B44DC:
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 12);
  // andi. r10,r11,131
  r10 = r11 & 131;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825b464c
  if (cr0.eq) goto loc_825B464C;
  // rlwinm. r10,r11,0,25,25
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x825b464c
  if (!cr0.eq) goto loc_825B464C;
  // rlwinm. r10,r11,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825b4504
  if (cr0.eq) goto loc_825B4504;
  // ori r11,r11,32
  r11 = r11 | 32;
  // b 0x825b4648
  goto loc_825B4648;
loc_825B4504:
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r11));
  // andi. r10,r11,268
  r10 = r11 & 268;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x825b4524
  if (!cr0.eq) goto loc_825B4524;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b4790
  // (left out: dead in C++)
  { const lifted::Result o = game::_getbuf(base, LIFTED_ARGS(0x825B4520, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825b452c
  goto loc_825B452C;
loc_825B4524:
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_825B452C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B4534, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lwz r5,24(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 24);
  // bl 0x825b8778
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B8778, LIFTED_ARGS(0x825B4540, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // stw r3,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r3));
  // beq 0x825b4628
  if (cr0.eq) goto loc_825B4628;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x825b4628
  if (cr6.eq) goto loc_825B4628;
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 12);
  // andi. r11,r11,130
  r11 = r11 & 130;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b45e0
  if (!cr0.eq) goto loc_825B45E0;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B456C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x825b45bc
  if (cr6.eq) goto loc_825B45BC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B457C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -2, so);
  // beq cr6,0x825b45bc
  if (cr6.eq) goto loc_825B45BC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B458C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x825b0e88
  // (left out: dead in C++)
  { const lifted::Result o = game::_fileno(base, LIFTED_ARGS(0x825B4598, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32104
  r11 = uint64_t(int64_t(-2103967744));
  // srawi r10,r30,5
  ca = (int32_t(r30) < 0) & ((uint32_t(r30) & 0x1F) != 0);
  r10 = uint64_t(int64_t(int32_t(r30) >> 5));
  // addi r11,r11,-32224
  r11 = uint64_t(int64_t(int64_t(r11) + -32224));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // clrlwi r10,r3,27
  r10 = uint32_t(r3) & 0x1F;
  // mulli r10,r10,44
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(44))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // b 0x825b45c4
  goto loc_825B45C4;
loc_825B45BC:
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // addi r11,r11,-16456
  r11 = uint64_t(int64_t(int64_t(r11) + -16456));
loc_825B45C4:
  // lbz r11,4(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 4);
  // andi. r11,r11,130
  r11 = r11 & 130;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // cmplwi cr6,r11,130
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 130, so);
  // bne cr6,0x825b45e0
  if (!cr6.eq) goto loc_825B45E0;
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 12);
  // ori r11,r11,8192
  r11 = r11 | 8192;
  // stw r11,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r11));
loc_825B45E0:
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24);
  // cmpwi cr6,r11,512
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 512, so);
  // bne cr6,0x825b4608
  if (!cr6.eq) goto loc_825B4608;
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 12);
  // rlwinm. r10,r11,0,28,28
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825b4608
  if (cr0.eq) goto loc_825B4608;
  // rlwinm. r11,r11,0,21,21
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x400;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b4608
  if (!cr0.eq) goto loc_825B4608;
  // li r11,4096
  r11 = uint64_t(int64_t(4096));
  // stw r11,24(r31)
  guest::StoreU32(base, uint32_t(r31) + 24, uint32_t(r11));
loc_825B4608:
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // lbz r3,0(r11)
  r3 = guest::LoadU8(base, uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // b 0x825b4650
  goto loc_825B4650;
loc_825B4628:
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // lwz r10,12(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 12);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stw r9,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r9));
  // or r11,r11,r10
  r11 = r11 | r10;
loc_825B4648:
  // stw r11,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r11));
loc_825B464C:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_825B4650:
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

// sub_825B81A0 game::_read: lifted from generated/ by lift.py.
lifted::Result f825B81A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825B81A0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e60
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r21,r3
  r21 = r3;
  // mr r31,r5
  r31 = r5;
  // mr r18,r4
  r18 = r4;
  // li r20,-2
  r20 = uint64_t(int64_t(-2));
  // mr r19,r31
  r19 = r31;
  // cmpwi cr6,r21,-2
  cr6 = lifted::Compare<int32_t>(int32_t(r21), -2, so);
  // bne cr6,0x825b81ec
  if (!cr6.eq) goto loc_825B81EC;
  // bl 0x825a8218
  // (left out: dead in C++)
  { const lifted::Result o = game::__doserrno(base, LIFTED_ARGS(0x825B81CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B81D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,9
  r10 = uint64_t(int64_t(9));
loc_825B81DC:
  // mr r11,r3
  r11 = r3;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // b 0x825b8768
  goto loc_825B8768;
loc_825B81EC:
  // cmpwi cr6,r21,0
  cr6 = lifted::Compare<int32_t>(int32_t(r21), 0, so);
  // blt cr6,0x825b8204
  if (cr6.lt) goto loc_825B8204;
  // lis r11,-32104
  r11 = uint64_t(int64_t(-2103967744));
  // lwz r11,-32240(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -32240);
  // cmplw cr6,r21,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), uint32_t(r11), so);
  // blt cr6,0x825b8240
  if (cr6.lt) goto loc_825B8240;
loc_825B8204:
  // bl 0x825a8218
  // (left out: dead in C++)
  { const lifted::Result o = game::__doserrno(base, LIFTED_ARGS(0x825B8208, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B8214, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,9
  r10 = uint64_t(int64_t(9));
loc_825B8218:
  // mr r11,r3
  r11 = r3;
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
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::_invalid_parameter(base, LIFTED_ARGS(0x825B8238, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // b 0x825b8768
  goto loc_825B8768;
loc_825B8240:
  // lis r11,-32104
  r11 = uint64_t(int64_t(-2103967744));
  // addi r29,r11,-32224
  r29 = uint64_t(int64_t(int64_t(r11) + -32224));
  // srawi r11,r21,5
  ca = (int32_t(r21) < 0) & ((uint32_t(r21) & 0x1F) != 0);
  r11 = uint64_t(int64_t(int32_t(r21) >> 5));
  // rlwinm r27,r11,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // clrlwi r11,r21,27
  r11 = uint32_t(r21) & 0x1F;
  // mulli r28,r11,44
  r28 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(44))));
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // add r11,r28,r11
  r11 = r28 + r11;
  // lbz r10,4(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 4);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // clrlwi. r9,r10,31
  r9 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x825b8204
  if (cr0.eq) goto loc_825B8204;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // mr r30,r26
  r30 = r26;
  // beq cr6,0x825b8764
  if (cr6.eq) goto loc_825B8764;
  // rlwinm. r10,r10,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x825b8764
  if (!cr0.eq) goto loc_825B8764;
  // cmplwi cr6,r18,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), 0, so);
  // bne cr6,0x825b82a4
  if (!cr6.eq) goto loc_825B82A4;
loc_825B8290:
  // bl 0x825a8218
  // (left out: dead in C++)
  { const lifted::Result o = game::__doserrno(base, LIFTED_ARGS(0x825B8294, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r26,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r26));
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B829C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,22
  r10 = uint64_t(int64_t(22));
  // b 0x825b8218
  goto loc_825B8218;
loc_825B82A4:
  // lbz r11,40(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 40);
  // rotlwi r11,r11,24
  r11 = __builtin_rotateleft32(uint32_t(r11), 24);
  // srawi r11,r11,25
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x1FFFFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 25));
  // extsb r22,r11
  r22 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r22,1
  cr6 = lifted::Compare<int32_t>(int32_t(r22), 1, so);
  // beq cr6,0x825b8414
  if (cr6.eq) goto loc_825B8414;
  // cmpwi cr6,r22,2
  cr6 = lifted::Compare<int32_t>(int32_t(r22), 2, so);
  // bne cr6,0x825b82d4
  if (!cr6.eq) goto loc_825B82D4;
  // not r11,r31
  r11 = ~r31;
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b8290
  if (cr0.eq) goto loc_825B8290;
  // rlwinm r31,r31,0,0,30
  r31 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 0) & 0xFFFFFFFE;
loc_825B82D4:
  // mr r23,r18
  r23 = r18;
loc_825B82D8:
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // mr r4,r23
  r4 = r23;
  // li r24,10
  r24 = uint64_t(int64_t(10));
  // add r11,r28,r11
  r11 = r28 + r11;
  // lbz r10,4(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 4);
  // andi. r10,r10,72
  r10 = r10 & 72;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825b83a8
  if (cr0.eq) goto loc_825B83A8;
  // lbz r11,5(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 5);
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x825b83a8
  if (cr6.eq) goto loc_825B83A8;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825b83a8
  if (cr6.eq) goto loc_825B83A8;
  // stb r11,0(r23)
  guest::StoreU8(base, uint32_t(r23) + 0, uint8_t(r11));
  // addi r4,r23,1
  r4 = uint64_t(int64_t(int64_t(r23) + 1));
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // add r11,r28,r11
  r11 = r28 + r11;
  // cmpwi cr6,r22,0
  cr6 = lifted::Compare<int32_t>(int32_t(r22), 0, so);
  // stb r24,5(r11)
  guest::StoreU8(base, uint32_t(r11) + 5, uint8_t(r24));
  // beq cr6,0x825b83a8
  if (cr6.eq) goto loc_825B83A8;
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // add r11,r28,r11
  r11 = r28 + r11;
  // lbz r11,41(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 41);
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x825b83a8
  if (cr6.eq) goto loc_825B83A8;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825b83a8
  if (cr6.eq) goto loc_825B83A8;
  // stb r11,0(r4)
  guest::StoreU8(base, uint32_t(r4) + 0, uint8_t(r11));
  // li r30,2
  r30 = uint64_t(int64_t(2));
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // cmpwi cr6,r22,1
  cr6 = lifted::Compare<int32_t>(int32_t(r22), 1, so);
  // add r11,r28,r11
  r11 = r28 + r11;
  // addi r4,r4,1
  r4 = uint64_t(int64_t(int64_t(r4) + 1));
  // stb r24,41(r11)
  guest::StoreU8(base, uint32_t(r11) + 41, uint8_t(r24));
  // bne cr6,0x825b83a8
  if (!cr6.eq) goto loc_825B83A8;
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // add r11,r28,r11
  r11 = r28 + r11;
  // lbz r11,42(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 42);
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x825b83a8
  if (cr6.eq) goto loc_825B83A8;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825b83a8
  if (cr6.eq) goto loc_825B83A8;
  // stb r11,0(r4)
  guest::StoreU8(base, uint32_t(r4) + 0, uint8_t(r11));
  // li r30,3
  r30 = uint64_t(int64_t(3));
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // addi r4,r4,1
  r4 = uint64_t(int64_t(int64_t(r4) + 1));
  // add r11,r28,r11
  r11 = r28 + r11;
  // stb r24,42(r11)
  guest::StoreU8(base, uint32_t(r11) + 42, uint8_t(r24));
loc_825B83A8:
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r5,r31
  r5 = r31;
  // lwzx r3,r28,r11
  r3 = guest::LoadU32(base, uint32_t(r28) + uint32_t(r11));
  // bl 0x82295fc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295FC8, LIFTED_ARGS(0x825B83C0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825b8730
  if (cr0.eq) goto loc_825B8730;
  // lwz r10,84(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 84);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x825b8730
  if (cr6.lt) goto loc_825B8730;
  // cmplw cr6,r10,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r31), so);
  // bgt cr6,0x825b8730
  if (cr6.gt) goto loc_825B8730;
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // add r30,r10,r30
  r30 = r10 + r30;
  // add r11,r28,r11
  r11 = r28 + r11;
  // lbz r9,4(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 4);
  // rlwinm. r9,r9,0,0,24
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x825b8704
  if (cr0.eq) goto loc_825B8704;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825b8458
  if (cr6.eq) goto loc_825B8458;
  // lbz r10,0(r23)
  r10 = guest::LoadU8(base, uint32_t(r23) + 0);
  // cmplwi cr6,r10,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 10, so);
  // bne cr6,0x825b8458
  if (!cr6.eq) goto loc_825B8458;
  // lbz r10,4(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 4);
  // ori r10,r10,4
  r10 = r10 | 4;
  // b 0x825b8464
  goto loc_825B8464;
loc_825B8414:
  // not r11,r31
  r11 = ~r31;
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b8290
  if (cr0.eq) goto loc_825B8290;
  // rlwinm r31,r31,31,1,31
  r31 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 31) & 0x7FFFFFFF;
  // cmplwi cr6,r31,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 4, so);
  // bge cr6,0x825b8430
  if (!cr6.lt) goto loc_825B8430;
  // li r31,4
  r31 = uint64_t(int64_t(4));
loc_825B8430:
  // mr r3,r31
  r3 = r31;
  // bl 0x825a9a90
  // (left out: dead in C++)
  { const lifted::Result o = game::malloc(base, LIFTED_ARGS(0x825B8438, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r23,r3
  r23 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // bne 0x825b82d8
  if (!cr0.eq) goto loc_825B82D8;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B8444, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,12
  r11 = uint64_t(int64_t(12));
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // bl 0x825a8218
  // (left out: dead in C++)
  { const lifted::Result o = game::__doserrno(base, LIFTED_ARGS(0x825B8450, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // b 0x825b81dc
  goto loc_825B81DC;
loc_825B8458:
  // lbz r10,4(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 4);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // rlwinm r10,r10,0,30,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFFFFB;
loc_825B8464:
  // add r25,r23,r30
  r25 = r23 + r30;
  // stb r10,4(r11)
  guest::StoreU8(base, uint32_t(r11) + 4, uint8_t(r10));
  // mr r31,r23
  r31 = r23;
  // mr r30,r23
  r30 = r23;
  // cmplw cr6,r23,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), uint32_t(r25), so);
  // bge cr6,0x825b85c0
  if (!cr6.lt) goto loc_825B85C0;
  // li r26,13
  r26 = uint64_t(int64_t(13));
loc_825B8480:
  // lbz r10,0(r30)
  r10 = guest::LoadU8(base, uint32_t(r30) + 0);
  // extsb r11,r10
  r11 = uint64_t(int64_t(int8_t(r10)));
  // cmpwi cr6,r11,26
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 26, so);
  // beq cr6,0x825b8590
  if (cr6.eq) goto loc_825B8590;
  // cmpwi cr6,r11,13
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 13, so);
  // beq cr6,0x825b84a4
  if (cr6.eq) goto loc_825B84A4;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
loc_825B849C:
  // stb r10,0(r31)
  guest::StoreU8(base, uint32_t(r31) + 0, uint8_t(r10));
  // b 0x825b8580
  goto loc_825B8580;
loc_825B84A4:
  // addi r11,r25,-1
  r11 = uint64_t(int64_t(int64_t(r25) + -1));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bge cr6,0x825b84d4
  if (!cr6.lt) goto loc_825B84D4;
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // cmplwi cr6,r9,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 10, so);
  // bne cr6,0x825b84cc
  if (!cr6.eq) goto loc_825B84CC;
  // addi r30,r30,2
  r30 = uint64_t(int64_t(int64_t(r30) + 2));
loc_825B84C4:
  // stb r24,0(r31)
  guest::StoreU8(base, uint32_t(r31) + 0, uint8_t(r24));
  // b 0x825b8580
  goto loc_825B8580;
loc_825B84CC:
  // mr r30,r11
  r30 = r11;
  // b 0x825b849c
  goto loc_825B849C;
loc_825B84D4:
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // lwzx r3,r28,r11
  r3 = guest::LoadU32(base, uint32_t(r28) + uint32_t(r11));
  // bl 0x82295fc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295FC8, LIFTED_ARGS(0x825B84F4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x825b8508
  if (!cr0.eq) goto loc_825B8508;
  // bl 0x82295f18
  // (left out: dead in C++)
  { const lifted::Result o = game::GetLastError(base, LIFTED_ARGS(0x825B8500, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x825b857c
  if (!cr0.eq) goto loc_825B857C;
loc_825B8508:
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825b857c
  if (cr6.eq) goto loc_825B857C;
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // add r11,r28,r11
  r11 = r28 + r11;
  // lbz r11,4(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 4);
  // andi. r11,r11,72
  r11 = r11 & 72;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b854c
  if (cr0.eq) goto loc_825B854C;
  // lbz r10,80(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 80);
  // cmplwi cr6,r10,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 10, so);
  // beq cr6,0x825b84c4
  if (cr6.eq) goto loc_825B84C4;
  // stb r26,0(r31)
  guest::StoreU8(base, uint32_t(r31) + 0, uint8_t(r26));
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // add r11,r28,r11
  r11 = r28 + r11;
  // stb r10,5(r11)
  guest::StoreU8(base, uint32_t(r11) + 5, uint8_t(r10));
  // b 0x825b8580
  goto loc_825B8580;
loc_825B854C:
  // cmplw cr6,r31,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r23), so);
  // bne cr6,0x825b8560
  if (!cr6.eq) goto loc_825B8560;
  // lbz r11,80(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 80);
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x825b84c4
  if (cr6.eq) goto loc_825B84C4;
loc_825B8560:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r21
  r3 = r21;
  // bl 0x825b6500
  // (left out: dead in C++)
  { const lifted::Result o = game::_lseeki64_nolock(base, LIFTED_ARGS(0x825B8570, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,80(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 80);
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x825b8584
  if (cr6.eq) goto loc_825B8584;
loc_825B857C:
  // stb r26,0(r31)
  guest::StoreU8(base, uint32_t(r31) + 0, uint8_t(r26));
loc_825B8580:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
loc_825B8584:
  // cmplw cr6,r30,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r25), so);
  // blt cr6,0x825b8480
  if (cr6.lt) goto loc_825B8480;
  // b 0x825b85c0
  goto loc_825B85C0;
loc_825B8590:
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // add r11,r28,r11
  r11 = r28 + r11;
  // lbz r10,4(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 4);
  // rlwinm. r10,r10,0,25,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x825b85b4
  if (!cr0.eq) goto loc_825B85B4;
  // lbz r10,4(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 4);
  // ori r10,r10,2
  r10 = r10 | 2;
  // stb r10,4(r11)
  guest::StoreU8(base, uint32_t(r11) + 4, uint8_t(r10));
  // b 0x825b85c0
  goto loc_825B85C0;
loc_825B85B4:
  // lbz r11,0(r30)
  r11 = guest::LoadU8(base, uint32_t(r30) + 0);
  // stb r11,0(r31)
  guest::StoreU8(base, uint32_t(r31) + 0, uint8_t(r11));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
loc_825B85C0:
  // subf r30,r23,r31
  r30 = r31 - r23;
  // cmpwi cr6,r22,1
  cr6 = lifted::Compare<int32_t>(int32_t(r22), 1, so);
  // bne cr6,0x825b8704
  if (!cr6.eq) goto loc_825B8704;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x825b8704
  if (cr6.eq) goto loc_825B8704;
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // lbz r10,0(r31)
  r10 = guest::LoadU8(base, uint32_t(r31) + 0);
  // rlwinm. r11,r10,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b85ec
  if (!cr0.eq) goto loc_825B85EC;
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // b 0x825b86d0
  goto loc_825B86D0;
loc_825B85EC:
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // addi r11,r11,-14328
  r11 = uint64_t(int64_t(int64_t(r11) + -14328));
  // b 0x825b8618
  goto loc_825B8618;
loc_825B85FC:
  // cmpwi cr6,r8,4
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 4, so);
  // bgt cr6,0x825b8624
  if (cr6.gt) goto loc_825B8624;
  // cmplw cr6,r31,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r23), so);
  // blt cr6,0x825b8624
  if (cr6.lt) goto loc_825B8624;
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // lbz r10,0(r31)
  r10 = guest::LoadU8(base, uint32_t(r31) + 0);
loc_825B8618:
  // lbzx r10,r10,r11
  r10 = guest::LoadU8(base, uint32_t(r10) + uint32_t(r11));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825b85fc
  if (cr6.eq) goto loc_825B85FC;
loc_825B8624:
  // lbz r9,0(r31)
  r9 = guest::LoadU8(base, uint32_t(r31) + 0);
  // mr r10,r9
  r10 = r9;
  // lbzx r11,r10,r11
  r11 = guest::LoadU8(base, uint32_t(r10) + uint32_t(r11));
  // extsb. r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825b8648
  if (!cr0.eq) goto loc_825B8648;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B863C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,42
  r11 = uint64_t(int64_t(42));
loc_825B8640:
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // b 0x825b8700
  goto loc_825B8700;
loc_825B8648:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpw cr6,r11,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r8), so);
  // bne cr6,0x825b865c
  if (!cr6.eq) goto loc_825B865C;
  // add r31,r8,r31
  r31 = r8 + r31;
  // b 0x825b86d0
  goto loc_825B86D0;
loc_825B865C:
  // lwzx r11,r27,r29
  r11 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // add r10,r28,r11
  r10 = r28 + r11;
  // lbz r11,4(r10)
  r11 = guest::LoadU8(base, uint32_t(r10) + 4);
  // andi. r11,r11,72
  r11 = r11 & 72;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825b86bc
  if (cr0.eq) goto loc_825B86BC;
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // stb r9,5(r10)
  guest::StoreU8(base, uint32_t(r10) + 5, uint8_t(r9));
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // blt cr6,0x825b8698
  if (cr6.lt) goto loc_825B8698;
  // lwzx r10,r27,r29
  r10 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r10,r28,r10
  r10 = r28 + r10;
  // stb r9,41(r10)
  guest::StoreU8(base, uint32_t(r10) + 41, uint8_t(r9));
loc_825B8698:
  // cmpwi cr6,r8,3
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 3, so);
  // bne cr6,0x825b86b4
  if (!cr6.eq) goto loc_825B86B4;
  // lwzx r10,r27,r29
  r10 = guest::LoadU32(base, uint32_t(r27) + uint32_t(r29));
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r10,r28,r10
  r10 = r28 + r10;
  // stb r9,42(r10)
  guest::StoreU8(base, uint32_t(r10) + 42, uint8_t(r9));
loc_825B86B4:
  // subf r31,r8,r11
  r31 = r11 - r8;
  // b 0x825b86d0
  goto loc_825B86D0;
loc_825B86BC:
  // neg r11,r8
  r11 = uint64_t(int64_t(static_cast<int64_t>(-r8)));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // extsw r4,r11
  r4 = uint64_t(int64_t(int32_t(r11)));
  // mr r3,r21
  r3 = r21;
  // bl 0x825b6500
  // (left out: dead in C++)
  { const lifted::Result o = game::_lseeki64_nolock(base, LIFTED_ARGS(0x825B86D0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825B86D0:
  // lis r3,0
  r3 = uint64_t(int64_t(0));
  // rlwinm r8,r19,31,1,31
  r8 = __builtin_rotateleft64(uint32_t(r19) | (r19 << 32), 31) & 0x7FFFFFFF;
  // mr r7,r18
  r7 = r18;
  // subf r6,r23,r31
  r6 = r31 - r23;
  // mr r5,r23
  r5 = r23;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r3,r3,65001
  r3 = r3 | 65001;
  // bl 0x82296648
  // (left out: dead in C++)
  { const lifted::Result o = game::MultiByteToWideChar(base, LIFTED_ARGS(0x825B86F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne 0x825b8728
  if (!cr0.eq) goto loc_825B8728;
  // bl 0x82295f18
  // (left out: dead in C++)
  { const lifted::Result o = game::GetLastError(base, LIFTED_ARGS(0x825B86FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825B86FC:
  // bl 0x825a8250
  // (left out: dead in C++)
  { const lifted::Result o = game::_dosmaperr(base, LIFTED_ARGS(0x825B8700, r31)); r3 = o.r3; f1 = o.f1; }
loc_825B8700:
  // li r20,-1
  r20 = uint64_t(int64_t(-1));
loc_825B8704:
  // cmplw cr6,r23,r18
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), uint32_t(r18), so);
  // beq cr6,0x825b8714
  if (cr6.eq) goto loc_825B8714;
  // mr r3,r23
  r3 = r23;
  // bl 0x825a9b58
  // (left out: dead in C++)
  { const lifted::Result o = game::free(base, LIFTED_ARGS(0x825B8714, r31)); r3 = o.r3; f1 = o.f1; }
loc_825B8714:
  // cmpwi cr6,r20,-2
  cr6 = lifted::Compare<int32_t>(int32_t(r20), -2, so);
  // mr r3,r30
  r3 = r30;
  // beq cr6,0x825b8768
  if (cr6.eq) goto loc_825B8768;
  // mr r3,r20
  r3 = r20;
  // b 0x825b8768
  goto loc_825B8768;
loc_825B8728:
  // rlwinm r30,r30,1,0,30
  r30 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // b 0x825b8704
  goto loc_825B8704;
loc_825B8730:
  // bl 0x82295f18
  // (left out: dead in C++)
  { const lifted::Result o = game::GetLastError(base, LIFTED_ARGS(0x825B8734, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 5, so);
  // bne cr6,0x825b8754
  if (!cr6.eq) goto loc_825B8754;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825B8740, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // bl 0x825a8218
  // (left out: dead in C++)
  { const lifted::Result o = game::__doserrno(base, LIFTED_ARGS(0x825B874C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // b 0x825b8640
  goto loc_825B8640;
loc_825B8754:
  // cmplwi cr6,r3,109
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 109, so);
  // bne cr6,0x825b86fc
  if (!cr6.eq) goto loc_825B86FC;
  // mr r20,r26
  r20 = r26;
  // b 0x825b8704
  goto loc_825B8704;
loc_825B8764:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825B8768:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6eb0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825BFB38: lifted from generated/ by lift.py.
lifted::Result f825BFB38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825BFB38 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stfd f31,-32(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r5
  r31 = r5;
  // fmr f31,f1
  f31 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r4
  r30 = r4;
  // lbz r10,0(r31)
  r10 = guest::LoadU8(base, uint32_t(r31) + 0);
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // rlwinm r11,r10,30,30,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3;
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825bfbac
  if (cr6.lt) goto loc_825BFBAC;
  // beq cr6,0x825bfb9c
  if (cr6.eq) goto loc_825BFB9C;
  // lwz r11,5(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 5);
  // lwz r10,1(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 1);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // stw r10,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r10));
  // lfs f2,84(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f2 = double(temp.f32);
  // lfs f1,88(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f1 = double(temp.f32);
  // bl 0x825bf720
  // (left out: dead in C++)
  { const lifted::Result o = f825BF720(base, r1, r13, r3, f1, f2); r3 = o.r3; f1 = o.f1; }
  // b 0x825bfbc8
  goto loc_825BFBC8;
loc_825BFB9C:
  // lwz r11,1(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 1);
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // lfs f1,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f1 = double(temp.f32);
  // b 0x825bfbc8
  goto loc_825BFBC8;
loc_825BFBAC:
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lhz r4,1(r31)
  r4 = guest::LoadU16(base, uint32_t(r31) + 1);
  // lwz r11,64(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825BFBC4, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f1 = double(temp.f32);
loc_825BFBC8:
  // lbz r11,0(r31)
  r11 = guest::LoadU8(base, uint32_t(r31) + 0);
  // clrlwi r11,r11,30
  r11 = uint32_t(r11) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825bfc30
  if (cr6.lt) goto loc_825BFC30;
  // beq cr6,0x825bfc20
  if (cr6.eq) goto loc_825BFC20;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x825bfc10
  if (cr6.lt) goto loc_825BFC10;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 0);
  f13 = double(temp.f32);
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fdivs f0,f0,f31
  f0 = double(float(f0 / f31));
  // fmuls f12,f0,f1
  f12 = double(float(f0 * f1));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 0, temp.u32);
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 0, temp.u32);
  // b 0x825bfc34
  goto loc_825BFC34;
loc_825BFC10:
  // lfs f0,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f0,f1
  f0 = double(float(f0 - f1));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 0, temp.u32);
  // b 0x825bfc34
  goto loc_825BFC34;
loc_825BFC20:
  // lfs f0,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 0);
  f0 = double(temp.f32);
  // fadds f0,f0,f1
  f0 = double(float(f0 + f1));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 0, temp.u32);
  // b 0x825bfc34
  goto loc_825BFC34;
loc_825BFC30:
  // stfs f1,0(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r30) + 0, temp.u32);
loc_825BFC34:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-32(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825C07C8: lifted from generated/ by lift.py.
lifted::Result f825C07C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C07C8 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,124(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 124);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c07e4
  if (cr6.eq) goto loc_825C07E4;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825C07E4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return {r3, f1};
}

lifted::Result e82286640(uint8_t* base, const lifted::Args& a) {
  return {f82286640(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822866C8(uint8_t* base, const lifted::Args& a) {
  return f822866C8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822868B0(uint8_t* base, const lifted::Args& a) {
  return f822868B0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82286A88(uint8_t* base, const lifted::Args& a) {
  return f82286A88(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82286D00(uint8_t* base, const lifted::Args& a) {
  return f82286D00(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82286E08(uint8_t* base, const lifted::Args& a) {
  return f82286E08(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82286FF0(uint8_t* base, const lifted::Args& a) {
  return f82286FF0(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e82287220(uint8_t* base, const lifted::Args& a) {
  return f82287220(base, a.r1, a.r13, a.r[1], a.f[0]);
}

lifted::Result e822872A8(uint8_t* base, const lifted::Args& a) {
  return f822872A8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82287CE0(uint8_t* base, const lifted::Args& a) {
  return f82287CE0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82287D70(uint8_t* base, const lifted::Args& a) {
  return f82287D70(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82287F98(uint8_t* base, const lifted::Args& a) {
  f82287F98(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82287FE8(uint8_t* base, const lifted::Args& a) {
  return f82287FE8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822880E8(uint8_t* base, const lifted::Args& a) {
  return {f822880E8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82288138(uint8_t* base, const lifted::Args& a) {
  return {f82288138(base, a.r1, a.r[1]), a.f[0]};
}

lifted::Result e822881E0(uint8_t* base, const lifted::Args& a) {
  return f822881E0(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822882E0(uint8_t* base, const lifted::Args& a) {
  return f822882E0(base, a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e82288378(uint8_t* base, const lifted::Args& a) {
  return f82288378(base, a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e82288408(uint8_t* base, const lifted::Args& a) {
  return f82288408(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82288438(uint8_t* base, const lifted::Args& a) {
  return f82288438(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82288550(uint8_t* base, const lifted::Args& a) {
  return f82288550(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82290AB8(uint8_t* base, const lifted::Args& a) {
  return {f82290AB8(base), a.f[0]};
}

lifted::Result e82290B98(uint8_t* base, const lifted::Args& a) {
  return f82290B98(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e82290D48(uint8_t* base, const lifted::Args& a) {
  return f82290D48(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82290EA0(uint8_t* base, const lifted::Args& a) {
  return f82290EA0(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e82290F78(uint8_t* base, const lifted::Args& a) {
  return {f82290F78(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82290FE0(uint8_t* base, const lifted::Args& a) {
  return {f82290FE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82291060(uint8_t* base, const lifted::Args& a) {
  return f82291060(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822910B0(uint8_t* base, const lifted::Args& a) {
  return f822910B0(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e822910F0(uint8_t* base, const lifted::Args& a) {
  return f822910F0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82291478(uint8_t* base, const lifted::Args& a) {
  return {f82291478(base), a.f[0]};
}

lifted::Result e82292338(uint8_t* base, const lifted::Args& a) {
  return {f82292338(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822923B8(uint8_t* base, const lifted::Args& a) {
  return {f822923B8(base), a.f[0]};
}

lifted::Result e822923D8(uint8_t* base, const lifted::Args& a) {
  return f822923D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82292518(uint8_t* base, const lifted::Args& a) {
  return f82292518(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82292768(uint8_t* base, const lifted::Args& a) {
  return f82292768(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0], a.lr);
}

lifted::Result e822952C8(uint8_t* base, const lifted::Args& a) {
  return f822952C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e822952D0(uint8_t* base, const lifted::Args& a) {
  return f822952D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e822952D8(uint8_t* base, const lifted::Args& a) {
  return f822952D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e822AD690(uint8_t* base, const lifted::Args& a) {
  return {f822AD690(base, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e822AD740(uint8_t* base, const lifted::Args& a) {
  return {f822AD740(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e822AD8F8(uint8_t* base, const lifted::Args& a) {
  return f822AD8F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e822AD978(uint8_t* base, const lifted::Args& a) {
  return f822AD978(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822AD9D8(uint8_t* base, const lifted::Args& a) {
  return f822AD9D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82459CA0(uint8_t* base, const lifted::Args& a) {
  return f82459CA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82459CB8(uint8_t* base, const lifted::Args& a) {
  return f82459CB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[6], a.r[7], a.lr);
}

lifted::Result e82459CD8(uint8_t* base, const lifted::Args& a) {
  return f82459CD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[7], a.lr);
}

lifted::Result e82459D20(uint8_t* base, const lifted::Args& a) {
  return f82459D20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82480780(uint8_t* base, const lifted::Args& a) {
  return f82480780(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e82481708(uint8_t* base, const lifted::Args& a) {
  return f82481708(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e82481A70(uint8_t* base, const lifted::Args& a) {
  return f82481A70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82481A80(uint8_t* base, const lifted::Args& a) {
  return f82481A80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82482CA8(uint8_t* base, const lifted::Args& a) {
  return f82482CA8(base, a.r1, a.r13, a.r[0], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82482DA8(uint8_t* base, const lifted::Args& a) {
  return f82482DA8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82575FE0(uint8_t* base, const lifted::Args& a) {
  return f82575FE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82576130(uint8_t* base, const lifted::Args& a) {
  return f82576130(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82576280(uint8_t* base, const lifted::Args& a) {
  return f82576280(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825767D0(uint8_t* base, const lifted::Args& a) {
  return f825767D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82576900(uint8_t* base, const lifted::Args& a) {
  return f82576900(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82576EE0(uint8_t* base, const lifted::Args& a) {
  return f82576EE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e82576FE8(uint8_t* base, const lifted::Args& a) {
  return f82576FE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e8258B7E0(uint8_t* base, const lifted::Args& a) {
  return {f8258B7E0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82590D18(uint8_t* base, const lifted::Args& a) {
  return f82590D18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82592A20(uint8_t* base, const lifted::Args& a) {
  return f82592A20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82592E18(uint8_t* base, const lifted::Args& a) {
  return f82592E18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[7], a.f[0]);
}

lifted::Result e825981C8(uint8_t* base, const lifted::Args& a) {
  return f825981C8(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A1148(uint8_t* base, const lifted::Args& a) {
  f825A1148(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825A1938(uint8_t* base, const lifted::Args& a) {
  return {f825A1938(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825A6608(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825A6608(base, a.r[0], a.r[1])};
}

lifted::Result e825A6648(uint8_t* base, const lifted::Args& a) {
  return {f825A6648(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825A67A8(uint8_t* base, const lifted::Args& a) {
  return f825A67A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825A6A20(uint8_t* base, const lifted::Args& a) {
  return f825A6A20(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A9EC0(uint8_t* base, const lifted::Args& a) {
  return f825A9EC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A9FA8(uint8_t* base, const lifted::Args& a) {
  return f825A9FA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825AA058(uint8_t* base, const lifted::Args& a) {
  return f825AA058(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825AB060(uint8_t* base, const lifted::Args& a) {
  return {f825AB060(base, a.r[0]), a.f[0]};
}

lifted::Result e825AB080(uint8_t* base, const lifted::Args& a) {
  return {f825AB080(base, a.r[0]), a.f[0]};
}

lifted::Result e825AC108(uint8_t* base, const lifted::Args& a) {
  return {f825AC108(base), a.f[0]};
}

lifted::Result e825ACA90(uint8_t* base, const lifted::Args& a) {
  return f825ACA90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825AD150(uint8_t* base, const lifted::Args& a) {
  return f825AD150(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825AECC0(uint8_t* base, const lifted::Args& a) {
  return f825AECC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825AFEC0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825AFEC0(base, a.r1, a.r[1], a.f[0])};
}

lifted::Result e825AFEF8(uint8_t* base, const lifted::Args& a) {
  return {f825AFEF8(base, a.r1, a.f[0]), a.f[0]};
}

lifted::Result e825B0068(uint8_t* base, const lifted::Args& a) {
  return f825B0068(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825B0470(uint8_t* base, const lifted::Args& a) {
  return f825B0470(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e825B06C8(uint8_t* base, const lifted::Args& a) {
  return f825B06C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825B0710(uint8_t* base, const lifted::Args& a) {
  return {f825B0710(base, a.r[0]), a.f[0]};
}

lifted::Result e825B0758(uint8_t* base, const lifted::Args& a) {
  return f825B0758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2]);
}

lifted::Result e825B0860(uint8_t* base, const lifted::Args& a) {
  return f825B0860(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825B08E8(uint8_t* base, const lifted::Args& a) {
  return f825B08E8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1]);
}

lifted::Result e825B0968(uint8_t* base, const lifted::Args& a) {
  return f825B0968(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1]);
}

lifted::Result e825B0A40(uint8_t* base, const lifted::Args& a) {
  return f825B0A40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2]);
}

lifted::Result e825B1F18(uint8_t* base, const lifted::Args& a) {
  return f825B1F18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825B1FB8(uint8_t* base, const lifted::Args& a) {
  return f825B1FB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825B4490(uint8_t* base, const lifted::Args& a) {
  return f825B4490(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825B81A0(uint8_t* base, const lifted::Args& a) {
  return f825B81A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825BFB38(uint8_t* base, const lifted::Args& a) {
  return f825BFB38(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C07C8(uint8_t* base, const lifted::Args& a) {
  return f825C07C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

namespace {

const Registration kEntries[] = {
    {0x82286640, e82286640},
    {0x822866C8, e822866C8},
    {0x822868B0, e822868B0},
    {0x82286A88, e82286A88},
    {0x82286D00, e82286D00},
    {0x82286E08, e82286E08},
    {0x82286FF0, e82286FF0},
    {0x82287220, e82287220},
    {0x822872A8, e822872A8},
    {0x82287CE0, e82287CE0},
    {0x82287D70, e82287D70},
    {0x82287F98, e82287F98},
    {0x82287FE8, e82287FE8},
    {0x822880E8, e822880E8},
    {0x82288138, e82288138},
    {0x822881E0, e822881E0},
    {0x822882E0, e822882E0},
    {0x82288378, e82288378},
    {0x82288408, e82288408},
    {0x82288438, e82288438},
    {0x82288550, e82288550},
    {0x82290AB8, e82290AB8},
    {0x82290B98, e82290B98},
    {0x82290D48, e82290D48},
    {0x82290EA0, e82290EA0},
    {0x82290F78, e82290F78},
    {0x82290FE0, e82290FE0},
    {0x82291060, e82291060},
    {0x822910B0, e822910B0},
    {0x822910F0, e822910F0},
    {0x82291478, e82291478},
    {0x82292338, e82292338},
    {0x822923B8, e822923B8},
    {0x822923D8, e822923D8},
    {0x82292518, e82292518},
    {0x82292768, e82292768},
    {0x822952C8, e822952C8},
    {0x822952D0, e822952D0},
    {0x822952D8, e822952D8},
    {0x822AD690, e822AD690},
    {0x822AD740, e822AD740},
    {0x822AD8F8, e822AD8F8},
    {0x822AD978, e822AD978},
    {0x822AD9D8, e822AD9D8},
    {0x82459CA0, e82459CA0},
    {0x82459CB8, e82459CB8},
    {0x82459CD8, e82459CD8},
    {0x82459D20, e82459D20},
    {0x82480780, e82480780},
    {0x82481708, e82481708},
    {0x82481A70, e82481A70},
    {0x82481A80, e82481A80},
    {0x82482CA8, e82482CA8},
    {0x82482DA8, e82482DA8},
    {0x82575FE0, e82575FE0},
    {0x82576130, e82576130},
    {0x82576280, e82576280},
    {0x825767D0, e825767D0},
    {0x82576900, e82576900},
    {0x82576EE0, e82576EE0},
    {0x82576FE8, e82576FE8},
    {0x8258B7E0, e8258B7E0},
    {0x82590D18, e82590D18},
    {0x82592A20, e82592A20},
    {0x82592E18, e82592E18},
    {0x825981C8, e825981C8},
    {0x825A1148, e825A1148},
    {0x825A1938, e825A1938},
    {0x825A6608, e825A6608},
    {0x825A6648, e825A6648},
    {0x825A67A8, e825A67A8},
    {0x825A6A20, e825A6A20},
    {0x825A9EC0, e825A9EC0},
    {0x825A9FA8, e825A9FA8},
    {0x825AA058, e825AA058},
    {0x825AB060, e825AB060},
    {0x825AB080, e825AB080},
    {0x825AC108, e825AC108},
    {0x825ACA90, e825ACA90},
    {0x825AD150, e825AD150},
    {0x825AECC0, e825AECC0},
    {0x825AFEC0, e825AFEC0},
    {0x825AFEF8, e825AFEF8},
    {0x825B0068, e825B0068},
    {0x825B0470, e825B0470},
    {0x825B06C8, e825B06C8},
    {0x825B0710, e825B0710},
    {0x825B0758, e825B0758},
    {0x825B0860, e825B0860},
    {0x825B08E8, e825B08E8},
    {0x825B0968, e825B0968},
    {0x825B0A40, e825B0A40},
    {0x825B1F18, e825B1F18},
    {0x825B1FB8, e825B1FB8},
    {0x825B4490, e825B4490},
    {0x825B81A0, e825B81A0},
    {0x825BFB38, e825BFB38},
    {0x825C07C8, e825C07C8},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_041)), true);

}  // namespace

}  // namespace lifted
