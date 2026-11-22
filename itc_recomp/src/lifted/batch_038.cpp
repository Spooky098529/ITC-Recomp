// Lift sweep batch 038 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/boot_mode.h"
#include "schema/car_makers.h"
#include "schema/menu_helpers.h"
#include "schema/menu_sounds.h"
#include "schema/mode_tick.h"
#include "schema/network_mode.h"
#include "schema/options_screen.h"
#include "schema/race_hud_machine.h"
#include "schema/stand_models.h"
#include "schema/system_services.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies_game.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_038, true, "ITC",
              "Run lift sweep batch 038's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82263840: lifted from generated/ by lift.py.
lifted::Result f82263840(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82263840 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82263870, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // lwz r11,24912(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24912);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822638a4
  if (cr6.lt) goto loc_822638A4;
  // beq cr6,0x8226389c
  if (cr6.eq) goto loc_8226389C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x822638c8
  if (!cr6.lt) goto loc_822638C8;
  // li r6,973
  r6 = uint64_t(int64_t(973));
  // b 0x822638b8
  goto loc_822638B8;
loc_8226389C:
  // li r6,972
  r6 = uint64_t(int64_t(972));
  // b 0x822638b8
  goto loc_822638B8;
loc_822638A4:
  // lwz r11,9680(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9680);
  // li r6,971
  r6 = uint64_t(int64_t(971));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822638b8
  if (cr6.eq) goto loc_822638B8;
  // li r6,976
  r6 = uint64_t(int64_t(976));
loc_822638B8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822638C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822638C8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x822638D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x822638F4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82263908, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82263920: lifted from generated/ by lift.py.
uint64_t f82263920(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82263920 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82263948, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8226395C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226396C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226397C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226398C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x822639A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x822639B4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_822639C8: lifted from generated/ by lift.py.
uint64_t f822639C8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_822639C8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822639F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263A04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263A14, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82263A24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82263A34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263A48, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263A5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263A70, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263A84, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82263A98: lifted from generated/ by lift.py.
uint64_t f82263A98(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82263A98 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82263AC0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263AD4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263AE4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82263AF4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82263B04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263B18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263B2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263B40, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263B54, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82263B68: lifted from generated/ by lift.py.
lifted::Result f82263B68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82263B68 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // lwz r11,9624(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 9624);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82263c10
  if (cr6.eq) goto loc_82263C10;
  // lwz r11,9632(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 9632);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82263c10
  if (cr6.eq) goto loc_82263C10;
  // lwz r11,9628(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 9628);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82263c10
  if (cr6.eq) goto loc_82263C10;
  // lwz r11,9636(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 9636);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82263c10
  if (cr6.eq) goto loc_82263C10;
  // lwz r11,9644(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 9644);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82263cc8
  if (!cr6.eq) goto loc_82263CC8;
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // lwz r10,24908(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 24908);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82263c18
  if (cr6.lt) goto loc_82263C18;
  // beq cr6,0x82263bd8
  if (cr6.eq) goto loc_82263BD8;
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // b 0x82263ce0
  goto loc_82263CE0;
loc_82263BD8:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82263be8
  if (cr0.eq) goto loc_82263BE8;
  // stw r11,8(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).previous_page_address(), uint32_t(r11));
loc_82263BE8:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r9,16(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).queued_step_address(), uint32_t(r9));
  // stw r11,12(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).step_address(), uint32_t(r11));
  // stw r8,4(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).next_page_address(), uint32_t(r8));
  // stw r9,0(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).page_address(), uint32_t(r9));
loc_82263C04:
  // mr r3,r27
  r3 = r27;
  // bl 0x822639c8
  // (left out: dead in C++)
  r3 = f822639C8(base, r1, r13);
loc_82263C0C:
  // stw r29,9640(r27)
  guest::StoreU32(base, uint32_t(r27) + 9640, uint32_t(r29));
loc_82263C10:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_82263C18:
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // stw r29,9648(r27)
  guest::StoreU32(base, uint32_t(r27) + 9648, uint32_t(r29));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r31,2
  r31 = uint64_t(int64_t(2));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r3,23680(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 23680);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // stw r31,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r31));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).stop_cue_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82263C4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,23680(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 23680);
  // stw r31,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r31));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).release_bank_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82263C68, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,92
  r4 = uint64_t(int64_t(int64_t(r1) + 92));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r29,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r29));
  // stw r29,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r29));
  // bl 0x82175850
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Play(base, LIFTED_ARGS(0x82263C7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4103
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4103, so);
  // beq cr6,0x82263c94
  if (cr6.eq) goto loc_82263C94;
  // cmpwi cr6,r11,4107
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4107, so);
  // bne cr6,0x82263c98
  if (!cr6.eq) goto loc_82263C98;
loc_82263C94:
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82263C98:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24908(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82263cb4
  if (cr6.eq) goto loc_82263CB4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82263CB4:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r31));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // b 0x82263c0c
  goto loc_82263C0C;
loc_82263CC8:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24908(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82263c04
  if (cr0.eq) goto loc_82263C04;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
loc_82263CE0:
  // bne cr6,0x82263c0c
  if (!cr6.eq) goto loc_82263C0C;
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82263cf4
  if (cr0.eq) goto loc_82263CF4;
  // stw r11,8(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).previous_page_address(), uint32_t(r11));
loc_82263CF4:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r31,2
  r31 = uint64_t(int64_t(2));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,12(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).step_address(), uint32_t(r11));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r29,16(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).queued_step_address(), uint32_t(r29));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).next_page_address(), uint32_t(r11));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).page_address(), uint32_t(r9));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r10,24912(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // stw r31,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r31));
  // stw r29,20(r10)
  guest::StoreU32(base, uint32_t(r10) + 20, uint32_t(r29));
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
  // stw r11,40(r10)
  guest::StoreU32(base, uint32_t(r10) + 40, uint32_t(r11));
  // stw r11,48(r10)
  guest::StoreU32(base, uint32_t(r10) + 48, uint32_t(r11));
  // stw r11,52(r10)
  guest::StoreU32(base, uint32_t(r10) + 52, uint32_t(r11));
  // stw r11,36(r10)
  guest::StoreU32(base, uint32_t(r10) + 36, uint32_t(r11));
  // stw r11,64(r10)
  guest::StoreU32(base, uint32_t(r10) + 64, uint32_t(r11));
  // stw r11,24(r10)
  guest::StoreU32(base, uint32_t(r10) + 24, uint32_t(r11));
  // stw r11,28(r10)
  guest::StoreU32(base, uint32_t(r10) + 28, uint32_t(r11));
  // stw r11,32(r10)
  guest::StoreU32(base, uint32_t(r10) + 32, uint32_t(r11));
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x82263D68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r31,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r31));
  // b 0x82263c0c
  goto loc_82263C0C;
}

// sub_82263D80: lifted from generated/ by lift.py.
uint64_t f82263D80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82263D80 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28180);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82263e08
  if (!cr6.eq) goto loc_82263E08;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,25008(r9)
  guest::StoreU32(base, uint32_t(r9) + 25008, uint32_t(r10));
  // li r9,18
  r9 = uint64_t(int64_t(18));
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
  // bl 0x82263920
  // (left out: dead in C++)
  r3 = f82263920(base, r1, r13);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9640(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9640_address(), uint32_t(r11));
loc_82263E08:
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

// sub_82263E20: lifted from generated/ by lift.py.
lifted::Result f82263E20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82263E20 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-512(r1)
  ea = -512 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r3
  r28 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r23,r4
  r23 = r4;
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82263E3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r30,43
  r30 = uint64_t(int64_t(43));
  // addi r11,r11,23756
  r11 = uint64_t(int64_t(int64_t(r11) + 23756));
  // mr r24,r3
  r24 = r3;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r11
  r3 = r11;
  // stw r30,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r30));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82263E5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r22,1
  r22 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82264258
  if (!cr6.eq) goto loc_82264258;
  // lis r25,-32148
  r25 = uint64_t(int64_t(-2106851328));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,28180(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82263e9c
  if (!cr6.eq) goto loc_82263E9C;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82263E90, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-31148(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31148);
  f0 = double(temp.f32);
  // b 0x82263ea8
  goto loc_82263EA8;
loc_82263E9C:
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82263EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8108(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8108);
  f0 = double(temp.f32);
loc_82263EA8:
  // stfs f0,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).live_scene_address(), temp.u32);
  // lwz r11,9672(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9672);
  // lwz r10,9668(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 9668);
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // bge cr6,0x82263ec4
  if (!cr6.lt) goto loc_82263EC4;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // b 0x82263edc
  goto loc_82263EDC;
loc_82263EC4:
  // ble cr6,0x82263ed8
  if (!cr6.gt) goto loc_82263ED8;
  // lwz r11,9676(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9676);
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82263edc
  if (cr6.eq) goto loc_82263EDC;
loc_82263ED8:
  // li r6,3
  r6 = uint64_t(int64_t(3));
loc_82263EDC:
  // li r5,25
  r5 = uint64_t(int64_t(25));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263EEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
loc_82263EF0:
  // lwz r11,29312(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 29312);
  // addi r10,r30,-43
  r10 = uint64_t(int64_t(int64_t(r30) + -43));
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82263fa4
  if (!cr6.gt) goto loc_82263FA4;
  // lwz r11,9668(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9668);
  // addi r27,r30,-43
  r27 = uint64_t(int64_t(int64_t(r30) + -43));
  // addi r29,r30,-3
  r29 = uint64_t(int64_t(int64_t(r30) + -3));
  // cmpw cr6,r11,r27
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r27), so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r29
  r4 = r29;
  // ble cr6,0x82263f2c
  if (!cr6.gt) goto loc_82263F2C;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263F24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // b 0x82263f34
  goto loc_82263F34;
loc_82263F2C:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82263F30, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
loc_82263F34:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263F44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9672(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9672);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // cmpw cr6,r11,r27
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r27), so);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // ble cr6,0x82263f68
  if (!cr6.gt) goto loc_82263F68;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263F60, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // b 0x82263f70
  goto loc_82263F70;
loc_82263F68:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82263F6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
loc_82263F70:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82263F80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263F90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263FA0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82263fc0
  goto loc_82263FC0;
loc_82263FA4:
  // addi r5,r30,-3
  r5 = uint64_t(int64_t(int64_t(r30) + -3));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82263FB0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82263FC0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82263FC0:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r11,r30,-43
  r11 = uint64_t(int64_t(int64_t(r30) + -43));
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // blt cr6,0x82263ef0
  if (cr6.lt) goto loc_82263EF0;
  // li r6,312
  r6 = uint64_t(int64_t(312));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82263FE4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,315
  r6 = uint64_t(int64_t(315));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82263FF8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,115
  r6 = uint64_t(int64_t(115));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226400C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28180(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 28180);
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // beq cr6,0x8226402c
  if (cr6.eq) goto loc_8226402C;
  // li r6,1
  r6 = uint64_t(int64_t(1));
loc_8226402C:
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82264030, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x82263840
  // (left out: dead in C++)
  { const lifted::Result o = f82263840(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // rlwinm r5,r24,5,0,26
  r5 = __builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 5) & 0xFFFFFFE0;
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,14
  r11 = uint64_t(int64_t(int64_t(r11) + 14));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,256
  r8 = uint64_t(int64_t(256));
  // addi r7,r1,160
  r7 = uint64_t(int64_t(int64_t(r1) + 160));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // add r5,r5,r11
  r5 = r5 + r11;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = game::WideCharToMultiByte(base, LIFTED_ARGS(0x8226406C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r1,160
  r6 = uint64_t(int64_t(int64_t(r1) + 160));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x822606b8
  // (left out: dead in C++)
  f822606B8(base, r3);
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,36
  r5 = uint64_t(int64_t(36));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // mr r3,r31
  r3 = r31;
  // std r9,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r9);
  // ld r9,8(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r9,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r9);
  // ld r9,16(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 16);
  // std r9,16(r10)
  guest::StoreU64(base, uint32_t(r10) + 16, r9);
  // ld r11,24(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 24);
  // std r11,24(r10)
  guest::StoreU64(base, uint32_t(r10) + 24, r11);
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28180(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 28180);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x82264180
  if (!cr6.eq) goto loc_82264180;
  // li r6,324
  r6 = uint64_t(int64_t(324));
  // li r5,26
  r5 = uint64_t(int64_t(26));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822640E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,317
  r6 = uint64_t(int64_t(317));
  // li r5,27
  r5 = uint64_t(int64_t(27));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822640F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,318
  r6 = uint64_t(int64_t(318));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226410C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,29
  r5 = uint64_t(int64_t(29));
  // lwz r7,9652(r28)
  r7 = guest::LoadU32(base, uint32_t(r28) + 9652);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82264124, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,152
  r10 = uint64_t(int64_t(9961472));
  // lwz r11,9656(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9656);
  // ori r10,r10,38527
  r10 = r10 | 38527;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226413c
  if (!cr6.gt) goto loc_8226413C;
  // mr r11,r10
  r11 = r10;
loc_8226413C:
  // mr r7,r11
  r7 = r11;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,30
  r5 = uint64_t(int64_t(30));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82264154, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,31
  r5 = uint64_t(int64_t(31));
  // lwz r7,9660(r28)
  r7 = guest::LoadU32(base, uint32_t(r28) + 9660);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226416C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8226417C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82264188
  goto loc_82264188;
loc_82264180:
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82264188, r31)); r3 = o.r3; f1 = o.f1; }
loc_82264188:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24912(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24912);
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822641A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // li r4,66
  r4 = uint64_t(int64_t(66));
  // mr r3,r31
  r3 = r31;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822641E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,66
  r4 = uint64_t(int64_t(66));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822641EC, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82264208, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82264214, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,65
  r4 = uint64_t(int64_t(65));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82264230, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8226424C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9688(r28)
  guest::StoreU32(base, uint32_t(r28) + 9688, uint32_t(r11));
  // stw r22,0(r23)
  guest::StoreU32(base, uint32_t(r23) + 0, uint32_t(r22));
loc_82264258:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82264264, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82264270
  if (!cr0.eq) goto loc_82264270;
  // stw r22,9688(r28)
  guest::StoreU32(base, uint32_t(r28) + 9688, uint32_t(r22));
loc_82264270:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822642a8
  if (!cr0.eq) goto loc_822642A8;
  // lwz r11,9688(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9688);
  // mr r3,r22
  r3 = r22;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822642ac
  if (!cr6.eq) goto loc_822642AC;
loc_822642A8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_822642AC:
  // addi r1,r1,512
  r1 = uint64_t(int64_t(int64_t(r1) + 512));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822642B8: lifted from generated/ by lift.py.
lifted::Result f822642B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822642B8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // mr r28,r4
  r28 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822642E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822643a4
  if (!cr6.eq) goto loc_822643A4;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226430C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82264318, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82264324, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x82263840
  // (left out: dead in C++)
  { const lifted::Result o = f82263840(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // lwz r29,0(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82264344, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // mr r3,r31
  r3 = r31;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82264388, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226439C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_822643A4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822643B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822643C0: lifted from generated/ by lift.py.
lifted::Result f822643C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822643C0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822643EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82264598
  if (!cr6.eq) goto loc_82264598;
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82264408, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82264414, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82264434
  if (!cr0.eq) goto loc_82264434;
  // lwz r9,9680(r28)
  r9 = guest::LoadU32(base, uint32_t(r28) + 9680);
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bne cr6,0x82264444
  if (!cr6.eq) goto loc_82264444;
loc_82264434:
  // addi r4,r10,7
  r4 = uint64_t(int64_t(int64_t(r10) + 7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82264440, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
loc_82264444:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822644b0
  if (!cr0.eq) goto loc_822644B0;
  // lwz r10,9680(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 9680);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x822644b0
  if (cr6.eq) goto loc_822644B0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,31
  r4 = uint64_t(int64_t(31));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82264478, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,31
  r4 = uint64_t(int64_t(31));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82264484, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822644A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822644AC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822644e0
  goto loc_822644E0;
loc_822644B0:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822644CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822644E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822644E0:
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8226452c
  if (!cr6.eq) goto loc_8226452C;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x82264524
  if (!cr6.eq) goto loc_82264524;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82264524
  if (!cr6.eq) goto loc_82264524;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82264518, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // b 0x82264564
  goto loc_82264564;
loc_82264524:
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // b 0x82264570
  goto loc_82264570;
loc_8226452C:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82264588
  if (!cr6.eq) goto loc_82264588;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226456c
  if (!cr6.eq) goto loc_8226456C;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8226456c
  if (!cr6.eq) goto loc_8226456C;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226455C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,7820(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7820);
  f0 = double(temp.f32);
loc_82264564:
  // stfs f0,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).live_scene_address(), temp.u32);
  // b 0x82264588
  goto loc_82264588;
loc_8226456C:
  // li r4,18
  r4 = uint64_t(int64_t(18));
loc_82264570:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82264588, r31)); r3 = o.r3; f1 = o.f1; }
loc_82264588:
  // mr r3,r28
  r3 = r28;
  // bl 0x82263840
  // (left out: dead in C++)
  { const lifted::Result o = f82263840(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_82264598:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822645A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822645B0: lifted from generated/ by lift.py.
lifted::Result f822645B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822645B0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // mr r30,r4
  r30 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822645DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82264694
  if (!cr6.eq) goto loc_82264694;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24912(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24912);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82264664
  if (!cr6.eq) goto loc_82264664;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226460C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,31
  r4 = uint64_t(int64_t(31));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82264628, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,31
  r4 = uint64_t(int64_t(31));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82264634, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82264650, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226465C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x82263840
  // (left out: dead in C++)
  { const lifted::Result o = f82263840(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82264664:
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82264674, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226468C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82264694:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822646A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82264A38: lifted from generated/ by lift.py.
lifted::Result f82264A38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82264A38 (lifted)");
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r29,r3
  r29 = r3;
  // lwz r31,24908(r30)
  r31 = guest::LoadU32(base, uint32_t(r30) + 24908);
  // lwz r28,0(r31)
  r28 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r31)).page_address());
  // cmpwi cr6,r28,5
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 5, so);
  // beq cr6,0x82264b88
  if (cr6.eq) goto loc_82264B88;
  // cmpwi cr6,r28,6
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 6, so);
  // beq cr6,0x82264b88
  if (cr6.eq) goto loc_82264B88;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82264A7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,12(r31)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r31)).step_address());
  // addi r4,r31,24
  r4 = uint64_t(int64_t(int64_t(r31) + 24));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82264aa4
  if (!cr6.eq) goto loc_82264AA4;
  // cmpwi cr6,r28,2
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 2, so);
  // beq cr6,0x82264b88
  if (cr6.eq) goto loc_82264B88;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82264AA0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82264b88
  goto loc_82264B88;
loc_82264AA4:
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82264b64
  if (cr6.lt) goto loc_82264B64;
  // beq cr6,0x82264b58
  if (cr6.eq) goto loc_82264B58;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82264b4c
  if (cr6.lt) goto loc_82264B4C;
  // beq cr6,0x82264b40
  if (cr6.eq) goto loc_82264B40;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // blt cr6,0x82264b34
  if (cr6.lt) goto loc_82264B34;
  // beq cr6,0x82264b04
  if (cr6.eq) goto loc_82264B04;
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r31)).queued_step_address());
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82264aec
  if (!cr6.eq) goto loc_82264AEC;
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r31)).next_page_address());
  // stw r9,12(r31)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r31)).step_address(), uint32_t(r9));
  // stw r10,0(r31)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r31)).page_address(), uint32_t(r10));
  // b 0x82264afc
  goto loc_82264AFC;
loc_82264AEC:
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r31)).queued_step_address());
  // stw r11,20(r31)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r31)).unknown_20_address(), uint32_t(r11));
  // stw r9,16(r31)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r31)).queued_step_address(), uint32_t(r9));
  // stw r10,12(r31)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r31)).step_address(), uint32_t(r10));
loc_82264AFC:
  // stw r11,24(r31)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r31)).step_started_address(), uint32_t(r11));
  // b 0x82264b88
  goto loc_82264B88;
loc_82264B04:
  // mr r3,r29
  r3 = r29;
  // bl 0x82263790
  // (left out: dead in C++)
  { const lifted::Result o = f82263790(base, r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_82264B0C:
  // lwz r10,24908(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 24908);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82264b88
  if (cr0.eq) goto loc_82264B88;
  // lwz r9,16(r10)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).queued_step_address());
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82264b70
  if (!cr6.eq) goto loc_82264B70;
  // lwz r8,4(r10)
  r8 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).next_page_address());
  // stw r8,0(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).page_address(), uint32_t(r8));
  // b 0x82264b80
  goto loc_82264B80;
loc_82264B34:
  // mr r3,r29
  r3 = r29;
  // bl 0x822645b0
  // (left out: dead in C++)
  { const lifted::Result o = f822645B0(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82264b0c
  goto loc_82264B0C;
loc_82264B40:
  // mr r3,r29
  r3 = r29;
  // bl 0x822643c0
  // (left out: dead in C++)
  { const lifted::Result o = f822643C0(base, r1, r13, r3, r4, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82264b0c
  goto loc_82264B0C;
loc_82264B4C:
  // mr r3,r29
  r3 = r29;
  // bl 0x822642b8
  // (left out: dead in C++)
  { const lifted::Result o = f822642B8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82264b0c
  goto loc_82264B0C;
loc_82264B58:
  // mr r3,r29
  r3 = r29;
  // bl 0x82263508
  // (left out: dead in C++)
  { const lifted::Result o = f82263508(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82264b0c
  goto loc_82264B0C;
loc_82264B64:
  // mr r3,r29
  r3 = r29;
  // bl 0x82263e20
  // (left out: dead in C++)
  { const lifted::Result o = f82263E20(base, r1, r13, r3, r4, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82264b0c
  goto loc_82264B0C;
loc_82264B70:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // lwz r9,16(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 16);
  // stw r8,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r8));
  // stw r11,20(r10)
  guest::StoreU32(base, uint32_t(r10) + 20, uint32_t(r11));
loc_82264B80:
  // stw r9,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,24(r10)
  guest::StoreU32(base, uint32_t(r10) + 24, uint32_t(r11));
loc_82264B88:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82264B90: lifted from generated/ by lift.py.
lifted::Result f82264B90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_82264B90 (lifted)");
  uint64_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1664(r1)
  ea = -1664 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r23,r4
  r23 = r4;
  // lwz r24,-21596(r11)
  r24 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82264BB4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // ori r28,r11,39656
  r28 = r11 | 39656;
  // lis r11,4
  r11 = uint64_t(int64_t(262144));
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // lis r21,-32148
  r21 = uint64_t(int64_t(-2106851328));
  // ori r9,r11,40928
  r9 = r11 | 40928;
  // stw r30,9664(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9664_address(), uint32_t(r30));
  // lis r11,4
  r11 = uint64_t(int64_t(262144));
  // stw r30,9676(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9676_address(), uint32_t(r30));
  // stw r30,9648(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9648_address(), uint32_t(r30));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r25,9640(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9640_address(), uint32_t(r25));
  // ori r8,r11,40932
  r8 = r11 | 40932;
  // stw r25,9644(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9644_address(), uint32_t(r25));
  // mr r26,r3
  r26 = r3;
  // stw r30,9624(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).back_address(), uint32_t(r30));
  // mr r27,r30
  r27 = r30;
  // stw r30,9628(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).race_address(), uint32_t(r30));
  // stw r30,9632(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).leave_address(), uint32_t(r30));
  // stw r30,9684(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9684_address(), uint32_t(r30));
  // stw r25,9688(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9688_address(), uint32_t(r25));
  // stw r30,9692(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9692_address(), uint32_t(r30));
  // stw r30,9652(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).save_data_a_address(), uint32_t(r30));
  // stw r30,9656(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).save_data_b_address(), uint32_t(r30));
  // stw r30,9660(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9660_address(), uint32_t(r30));
  // stw r30,9668(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9668_address(), uint32_t(r30));
  // stw r30,9672(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9672_address(), uint32_t(r30));
  // stw r25,9680(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9680_address(), uint32_t(r25));
  // lwz r11,23340(r21)
  r11 = guest::LoadU32(base, uint32_t(r21) + 23340);
  // add r29,r11,r28
  r29 = r11 + r28;
  // mr r11,r30
  r11 = r30;
  // stb r11,27404(r10)
  guest::StoreU8(base, uint32_t(r10) + 27404, uint8_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,27368(r10)
  guest::StoreU32(base, uint32_t(r10) + 27368, uint32_t(r11));
  // lwzx r11,r29,r9
  r11 = guest::LoadU32(base, uint32_t(r29) + uint32_t(r9));
  // stw r11,9664(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9664_address(), uint32_t(r11));
  // lwzx r11,r29,r8
  r11 = guest::LoadU32(base, uint32_t(r29) + uint32_t(r8));
  // lwz r10,9664(r31)
  r10 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9664_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,9676(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9676_address(), uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r22,r11,30752
  r22 = uint64_t(int64_t(int64_t(r11) + 30752));
  // ble cr6,0x82264ce4
  if (!cr6.gt) goto loc_82264CE4;
loc_82264C64:
  // mr r4,r27
  r4 = r27;
  // mr r3,r29
  r3 = r29;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(base, LIFTED_ARGS(0x82264C70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3924(r3)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r3)).finish_address());
  // lwz r10,4(r22)
  r10 = guest::LoadU32(base, uint32_t(r22) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82264c88
  if (!cr6.eq) goto loc_82264C88;
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82264c98
  if (cr6.eq) goto loc_82264C98;
loc_82264C88:
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82264ca8
  if (!cr6.eq) goto loc_82264CA8;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82264ca8
  if (!cr6.eq) goto loc_82264CA8;
loc_82264C98:
  // lwz r11,9668(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9668_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,9668(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9668_address(), uint32_t(r11));
  // b 0x82264cd4
  goto loc_82264CD4;
loc_82264CA8:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82264cb8
  if (!cr6.eq) goto loc_82264CB8;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82264cc8
  if (cr6.eq) goto loc_82264CC8;
loc_82264CB8:
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82264cd4
  if (!cr6.eq) goto loc_82264CD4;
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82264cd4
  if (!cr6.eq) goto loc_82264CD4;
loc_82264CC8:
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9672_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,9672(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9672_address(), uint32_t(r11));
loc_82264CD4:
  // lwz r11,9664(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9664_address());
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // cmpw cr6,r27,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r11), so);
  // blt cr6,0x82264c64
  if (cr6.lt) goto loc_82264C64;
loc_82264CE4:
  // li r27,2
  r27 = uint64_t(int64_t(2));
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9672_address());
  // lwz r10,9668(r31)
  r10 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9668_address());
  // lis r20,-32148
  r20 = uint64_t(int64_t(-2106851328));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stb r30,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r30));
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // addi r3,r1,84
  r3 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r27,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r27));
  // bge cr6,0x82264d44
  if (!cr6.lt) goto loc_82264D44;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r11,27208
  r4 = uint64_t(int64_t(int64_t(r11) + 27208));
  // bl 0x821063b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Music::Play(base, LIFTED_ARGS(0x82264D18, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28180(r20)
  r11 = guest::LoadU32(base, uint32_t(r20) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822650a4
  if (!cr6.eq) goto loc_822650A4;
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x82290788
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82290788, LIFTED_ARGS(0x82264D30, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x82264d50
  if (!cr6.eq) goto loc_82264D50;
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x82290a30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82290A30, LIFTED_ARGS(0x82264D40, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82264d50
  goto loc_82264D50;
loc_82264D44:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r11,27268
  r4 = uint64_t(int64_t(int64_t(r11) + 27268));
  // bl 0x821063b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Music::Play(base, LIFTED_ARGS(0x82264D50, r31)); r3 = o.r3; f1 = o.f1; }
loc_82264D50:
  // lwz r11,28180(r20)
  r11 = guest::LoadU32(base, uint32_t(r20) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822650a4
  if (!cr6.eq) goto loc_822650A4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x82264D64, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // addis r29,r11,2
  r29 = uint64_t(int64_t(int64_t(r11) + 131072));
  // addi r29,r29,-1968
  r29 = uint64_t(int64_t(int64_t(r29) + -1968));
  // bl 0x822606b8
  // (left out: dead in C++)
  f822606B8(base, r3);
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9672_address());
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r9,9668(r31)
  r9 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9668_address());
  // ld r8,16(r3)
  r8 = guest::LoadU64(base, uint32_t(r3) + 16);
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // ld r10,0(r3)
  r10 = guest::LoadU64(base, uint32_t(r3) + 0);
  // ld r9,8(r3)
  r9 = guest::LoadU64(base, uint32_t(r3) + 8);
  // ld r7,24(r3)
  r7 = guest::LoadU64(base, uint32_t(r3) + 24);
  // std r10,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r10);
  // std r9,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r9);
  // std r8,16(r11)
  guest::StoreU64(base, uint32_t(r11) + 16, r8);
  // std r7,24(r11)
  guest::StoreU64(base, uint32_t(r11) + 24, r7);
  // blt cr6,0x82264de0
  if (cr6.lt) goto loc_82264DE0;
  // lwz r11,9676(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9676_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82264de0
  if (cr6.eq) goto loc_82264DE0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r30,9652(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).save_data_a_address(), uint32_t(r30));
  // rlwinm r10,r26,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // addi r11,r11,332
  r11 = uint64_t(int64_t(int64_t(r11) + 332));
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // stw r11,9656(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).save_data_b_address(), uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24868(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24868);
  // b 0x82264e2c
  goto loc_82264E2C;
loc_82264DE0:
  // lwz r11,23340(r21)
  r11 = guest::LoadU32(base, uint32_t(r21) + 23340);
  // mr r3,r29
  r3 = r29;
  // lwz r6,120(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 120);
  // add r4,r11,r28
  r4 = r11 + r28;
  // lwz r5,4(r22)
  r5 = guest::LoadU32(base, uint32_t(r22) + 4);
  // bl 0x821059d0
  // (left out: dead in C++)
  r3 = f821059D0(base, r1, r13, r3, r4, r5, r6);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // rlwinm r11,r26,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 2) & 0xFFFFFFFC;
  // stw r3,9652(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).save_data_a_address(), uint32_t(r3));
  // addi r10,r10,30320
  r10 = uint64_t(int64_t(int64_t(r10) + 30320));
  // addi r9,r10,332
  r9 = uint64_t(int64_t(int64_t(r10) + 332));
  // addi r8,r10,348
  r8 = uint64_t(int64_t(int64_t(r10) + 348));
  // lwzx r10,r11,r9
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // add r10,r10,r3
  r10 = r10 + r3;
  // stw r10,9656(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).save_data_b_address(), uint32_t(r10));
  // lwzx r11,r11,r8
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r8));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
  // divwu r11,r10,r11
  r11 = uint32_t(uint32_t(r11) ? uint32_t(r10) / uint32_t(r11) : 0);
loc_82264E2C:
  // stw r11,9660(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9660_address(), uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // slw r10,r25,r24
  r10 = uint8_t(r24) & 0x20 ? 0 : (uint32_t(r25) << (uint8_t(r24) & 0x3F));
  // lwz r11,31068(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 31068);
  // and. r11,r10,r11
  r11 = r10 & r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82265008
  if (cr0.eq) goto loc_82265008;
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9672_address());
  // lwz r10,9668(r31)
  r10 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9668_address());
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // blt cr6,0x82264e60
  if (cr6.lt) goto loc_82264E60;
  // lwz r11,9676(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9676_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82265008
  if (!cr6.eq) goto loc_82265008;
loc_82264E60:
  // lwz r11,88(r22)
  r11 = guest::LoadU32(base, uint32_t(r22) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r29,r11,26160
  r29 = uint64_t(int64_t(int64_t(r11) + 26160));
  // bne cr6,0x82264f0c
  if (!cr6.eq) goto loc_82264F0C;
  // lwz r11,9652(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).save_data_a_address());
  // rlwinm r10,r11,8,24,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFF;
  // stb r11,278(r1)
  guest::StoreU8(base, uint32_t(r1) + 278, uint8_t(r11));
  // stb r10,275(r1)
  guest::StoreU8(base, uint32_t(r1) + 275, uint8_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // sth r10,276(r1)
  guest::StoreU16(base, uint32_t(r1) + 276, uint16_t(r10));
  // stb r11,274(r1)
  guest::StoreU8(base, uint32_t(r1) + 274, uint8_t(r11));
  // lwz r11,4(r22)
  r11 = guest::LoadU32(base, uint32_t(r22) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82264ef4
  if (!cr6.eq) goto loc_82264EF4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,29448
  r10 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82264ec0
  if (!cr6.gt) goto loc_82264EC0;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_82264EC0:
  // subf r9,r11,r10
  r9 = r10 - r11;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm. r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x82264f14
  if (cr0.eq) goto loc_82264F14;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82264ee8
  if (!cr6.gt) goto loc_82264EE8;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_82264EE8:
  // blt cr6,0x82264efc
  if (cr6.lt) goto loc_82264EFC;
  // twi 31,r0,22
  lifted::Trap(22);
  // b 0x82264efc
  goto loc_82264EFC;
loc_82264EF4:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
loc_82264EFC:
  // addi r3,r1,272
  r3 = uint64_t(int64_t(int64_t(r1) + 272));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 8);
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(base, LIFTED_ARGS(0x82264F08, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82264f14
  goto loc_82264F14;
loc_82264F0C:
  // mr r11,r30
  r11 = r30;
  // stw r11,4(r29)
  guest::StoreU32(base, uint32_t(r29) + 4, uint32_t(r11));
loc_82264F14:
  // lwz r11,9668(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9668_address());
  // mr r9,r25
  r9 = r25;
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9672_address());
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r8,r11,25284
  r8 = uint64_t(int64_t(int64_t(r11) + 25284));
  // bgt cr6,0x82264f34
  if (cr6.gt) goto loc_82264F34;
  // mr r9,r30
  r9 = r30;
loc_82264F34:
  // stw r9,0(r8)
  guest::StoreU32(base, uint32_t(r8) + 0, uint32_t(r9));
  // cntlzw r7,r9
  r7 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // lwz r10,9652(r31)
  r10 = guest::LoadU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).save_data_a_address());
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // addi r11,r23,204
  r11 = uint64_t(int64_t(int64_t(r23) + 204));
  // li r28,50
  r28 = uint64_t(int64_t(50));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r10,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r10));
  // mr r10,r30
  r10 = r30;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stb r10,27410(r9)
  guest::StoreU8(base, uint32_t(r9) + 27410, uint8_t(r10));
  // rlwinm r10,r7,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 27) & 0x1;
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,4(r8)
  guest::StoreU32(base, uint32_t(r8) + 4, uint32_t(r10));
  // mr r10,r30
  r10 = r30;
  // mr r8,r30
  r8 = r30;
  // stw r10,27372(r9)
  guest::StoreU32(base, uint32_t(r9) + 27372, uint32_t(r10));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // stw r10,27308(r9)
  guest::StoreU32(base, uint32_t(r9) + 27308, uint32_t(r10));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // mr r10,r27
  r10 = r27;
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // lwz r29,604(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r29,r29,r10
  r29 = r29 + r10;
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r10));
  // divwu r28,r29,r28
  r28 = uint32_t(uint32_t(r28) ? uint32_t(r29) / uint32_t(r28) : 0);
  // mulli r28,r28,50
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(50))));
  // subf r10,r28,r29
  r10 = r29 - r28;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x82264FD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24908(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82264ff0
  if (cr6.eq) goto loc_82264FF0;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82264FF0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // b 0x82265138
  goto loc_82265138;
loc_82265008:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4103
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4103, so);
  // beq cr6,0x82265020
  if (cr6.eq) goto loc_82265020;
  // cmpwi cr6,r11,4107
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4107, so);
  // bne cr6,0x82265024
  if (!cr6.eq) goto loc_82265024;
loc_82265020:
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82265024:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24908(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8226503c
  if (cr0.eq) goto loc_8226503C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226503C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r30,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r30));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r30,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r30));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,24912(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24912);
  // stw r30,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r30));
  // stw r27,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r27));
  // stw r25,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r25));
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // stw r30,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r30));
  // stw r30,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r30));
  // stw r30,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r30));
  // stw r30,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r30));
  // stw r30,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r30));
  // stw r30,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r30));
  // stw r30,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r30));
  // stw r30,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r30));
  // lwz r11,24912(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24912);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r27,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r27));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // b 0x82265138
  goto loc_82265138;
loc_822650A4:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4103
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4103, so);
  // beq cr6,0x822650c4
  if (cr6.eq) goto loc_822650C4;
  // cmpwi cr6,r11,4107
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4107, so);
  // beq cr6,0x822650c4
  if (cr6.eq) goto loc_822650C4;
  // stw r30,9680(r31)
  guest::StoreU32(base, schema::ModeTickNet9Machine(base, uint32_t(r31)).unknown_9680_address(), uint32_t(r30));
  // b 0x822650cc
  goto loc_822650CC;
loc_822650C4:
  // li r3,4104
  r3 = uint64_t(int64_t(4104));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_822650CC:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24908(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822650e4
  if (cr0.eq) goto loc_822650E4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822650E4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r30,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r30));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r30,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r30));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24912(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24912);
  // stw r30,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r30));
  // stw r27,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r27));
  // stw r25,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r25));
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // stw r30,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r30));
  // stw r30,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r30));
  // stw r30,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r30));
  // stw r30,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r30));
  // stw r30,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r30));
  // stw r30,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r30));
  // stw r30,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r30));
  // stw r30,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r30));
loc_82265138:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24960);
  // addi r30,r11,832
  r30 = uint64_t(int64_t(int64_t(r11) + 832));
  // lwz r11,4(r22)
  r11 = guest::LoadU32(base, uint32_t(r22) + 4);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // mr r3,r30
  r3 = r30;
  // rlwinm r10,r11,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // lwz r11,28180(r20)
  r11 = guest::LoadU32(base, uint32_t(r20) + 28180);
  // subf r9,r25,r11
  r9 = r11 - r25;
  // lwz r11,23340(r21)
  r11 = guest::LoadU32(base, uint32_t(r21) + 23340);
  // mulli r10,r10,3936
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(3936))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // addi r29,r11,48
  r29 = uint64_t(int64_t(int64_t(r11) + 48));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // rlwinm r11,r9,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // stw r11,24848(r10)
  guest::StoreU32(base, uint32_t(r10) + 24848, uint32_t(r11));
  // bl 0x82110448
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageCar::Reset(base, LIFTED_ARGS(0x82265180, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82265190, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x822639c8
  // (left out: dead in C++)
  r3 = f822639C8(base, r1, r13);
  // addi r1,r1,1664
  r1 = uint64_t(int64_t(int64_t(r1) + 1664));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822651A0: lifted from generated/ by lift.py.
lifted::Result f822651A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822651A0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1552(r1)
  ea = -1552 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r24,r11,30752
  r24 = uint64_t(int64_t(int64_t(r11) + 30752));
  // mr r21,r4
  r21 = r4;
  // lwz r11,88(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82265230
  if (cr6.eq) goto loc_82265230;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4103
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4103, so);
  // beq cr6,0x822651e0
  if (cr6.eq) goto loc_822651E0;
  // cmpwi cr6,r11,4107
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4107, so);
  // bne cr6,0x82265230
  if (!cr6.eq) goto loc_82265230;
loc_822651E0:
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9680(r26)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r26)).unknown_9680_address(), uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28180);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82265660
  if (!cr6.eq) goto loc_82265660;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24908(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8226521c
  if (cr6.eq) goto loc_8226521C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226521C:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r23,-1
  r23 = uint64_t(int64_t(-1));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r23,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r23));
  // b 0x8226565c
  goto loc_8226565C;
loc_82265230:
  // lwz r11,9692(r26)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r26)).unknown_9692_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82265318
  if (!cr6.eq) goto loc_82265318;
  // lwz r11,9680(r26)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r26)).unknown_9680_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82265318
  if (!cr6.eq) goto loc_82265318;
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lwz r10,4(r24)
  r10 = guest::LoadU32(base, uint32_t(r24) + 4);
  // lwz r11,-29856(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -29856);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82265264
  if (cr6.eq) goto loc_82265264;
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8226527c
  if (cr6.eq) goto loc_8226527C;
loc_82265264:
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lwz r11,-29860(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -29860);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82265318
  if (cr6.eq) goto loc_82265318;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82265318
  if (!cr6.eq) goto loc_82265318;
loc_8226527C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82265284, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,23
  r10 = uint64_t(int64_t(23));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // stb r10,178(r1)
  guest::StoreU8(base, uint32_t(r1) + 178, uint8_t(r10));
  // addi r11,r11,176
  r11 = uint64_t(int64_t(int64_t(r11) + 176));
  // lwz r10,4(r24)
  r10 = guest::LoadU32(base, uint32_t(r24) + 4);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // rlwinm r10,r3,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // stb r11,179(r1)
  guest::StoreU8(base, uint32_t(r1) + 179, uint8_t(r11));
  // bne cr6,0x82265304
  if (!cr6.eq) goto loc_82265304;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,29448
  r10 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822652d0
  if (!cr6.gt) goto loc_822652D0;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_822652D0:
  // subf r9,r11,r10
  r9 = r10 - r11;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm. r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x82265318
  if (cr0.eq) goto loc_82265318;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822652f8
  if (!cr6.gt) goto loc_822652F8;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_822652F8:
  // blt cr6,0x8226530c
  if (cr6.lt) goto loc_8226530C;
  // twi 31,r0,22
  lifted::Trap(22);
  // b 0x8226530c
  goto loc_8226530C;
loc_82265304:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
loc_8226530C:
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 8);
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(base, LIFTED_ARGS(0x82265318, r31)); r3 = o.r3; f1 = o.f1; }
loc_82265318:
  // lwz r11,9692(r26)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r26)).unknown_9692_address());
  // li r10,30
  r10 = uint64_t(int64_t(30));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // divwu r10,r11,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r11) / uint32_t(r10) : 0);
  // mulli r10,r10,30
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(30))));
  // subf r11,r10,r11
  r11 = r11 - r10;
  // stw r11,9692(r26)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r26)).unknown_9692_address(), uint32_t(r11));
  // lwz r3,24912(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // bl 0x821ce1d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CE1D0, LIFTED_ARGS(0x82265340, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r3,24912(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // li r23,-1
  r23 = uint64_t(int64_t(-1));
  // li r30,2
  r30 = uint64_t(int64_t(2));
  // lwz r11,28180(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822653c0
  if (!cr6.eq) goto loc_822653C0;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822653c0
  if (!cr6.eq) goto loc_822653C0;
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 4);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // bne cr6,0x822653a0
  if (!cr6.eq) goto loc_822653A0;
  // stw r30,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r30));
  // lwz r11,24912(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,24912(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // stw r23,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r23));
  // b 0x822653bc
  goto loc_822653BC;
loc_822653A0:
  // stw r28,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r28));
  // lwz r11,24912(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,24912(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // stw r28,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r28));
loc_822653BC:
  // lwz r3,24912(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24912);
loc_822653C0:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822653C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // li r22,3
  r22 = uint64_t(int64_t(3));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r25,-32127
  r25 = uint64_t(int64_t(-2105475072));
  // beq 0x8226556c
  if (cr0.eq) goto loc_8226556C;
  // lwz r11,24912(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822654c8
  if (cr6.lt) goto loc_822654C8;
  // beq cr6,0x82265480
  if (cr6.eq) goto loc_82265480;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82265624
  if (!cr6.lt) goto loc_82265624;
  // lwz r11,4(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82265414
  if (!cr6.eq) goto loc_82265414;
  // lwz r11,28180(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28180);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // beq cr6,0x82265418
  if (cr6.eq) goto loc_82265418;
loc_82265414:
  // li r11,6
  r11 = uint64_t(int64_t(6));
loc_82265418:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // mr r8,r27
  r8 = r27;
  // mr r7,r27
  r7 = r27;
  // stw r11,24996(r10)
  guest::StoreU32(base, uint32_t(r10) + 24996, uint32_t(r11));
  // addi r11,r21,204
  r11 = uint64_t(int64_t(int64_t(r21) + 204));
  // mr r3,r26
  r3 = r26;
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
  // bl 0x82263a98
  // (left out: dead in C++)
  r3 = f82263A98(base, r1, r13);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r27,9640(r26)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r26)).unknown_9640_address(), uint32_t(r27));
  // b 0x82265564
  goto loc_82265564;
loc_82265480:
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x822606b8
  // (left out: dead in C++)
  f822606B8(base, r3);
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r3,-21596(r9)
  r3 = guest::LoadU32(base, uint32_t(r9) + -21596);
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r8,8(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 8);
  // ld r7,16(r11)
  r7 = guest::LoadU64(base, uint32_t(r11) + 16);
  // ld r11,24(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 24);
  // std r9,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r9);
  // std r8,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r8);
  // std r7,16(r10)
  guest::StoreU64(base, uint32_t(r10) + 16, r7);
  // std r11,24(r10)
  guest::StoreU64(base, uint32_t(r10) + 24, r11);
  // ld r4,128(r1)
  r4 = guest::LoadU64(base, uint32_t(r1) + 128);
  // bl 0x822952d8
  // (left out: dead in C++)
  { const lifted::Result o = game::XShowPlayerReviewUI(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x822654C0); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82265564
  goto loc_82265564;
loc_822654C8:
  // lwz r11,9680(r26)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r26)).unknown_9680_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82265560
  if (!cr6.eq) goto loc_82265560;
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // stw r27,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r27));
  // stw r30,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r30));
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r3,23680(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 23680);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).stop_cue_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822654FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,23680(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 23680);
  // stw r30,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r30));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).release_bank_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82265518, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,84
  r3 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r22,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r22));
  // stw r28,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r28));
  // bl 0x82175850
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Play(base, LIFTED_ARGS(0x8226552C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r27,9648(r26)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r26)).unknown_9648_address(), uint32_t(r27));
  // lwz r11,24908(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82265544
  if (cr6.eq) goto loc_82265544;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82265544:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r23,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r23));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r30,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r30));
  // stw r23,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r23));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // b 0x82265564
  goto loc_82265564;
loc_82265560:
  // li r3,3
  r3 = uint64_t(int64_t(3));
loc_82265564:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82265568, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82265624
  goto loc_82265624;
loc_8226556C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24912(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82265578, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822655d0
  if (cr0.eq) goto loc_822655D0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82265588, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24912(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82265624
  if (cr6.eq) goto loc_82265624;
  // lwz r11,24908(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822655ac
  if (cr0.eq) goto loc_822655AC;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822655AC:
  // stw r30,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r30));
  // stw r23,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r23));
  // stw r27,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r27));
  // stw r23,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r23));
  // lwz r11,24912(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // b 0x82265624
  goto loc_82265624;
loc_822655D0:
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // lwz r3,24912(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822655DC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82265624
  if (cr0.eq) goto loc_82265624;
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x822606b8
  // (left out: dead in C++)
  f822606B8(base, r3);
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r3,-21596(r9)
  r3 = guest::LoadU32(base, uint32_t(r9) + -21596);
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r8,8(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 8);
  // ld r7,16(r11)
  r7 = guest::LoadU64(base, uint32_t(r11) + 16);
  // ld r11,24(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 24);
  // std r9,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r9);
  // std r8,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r8);
  // std r7,16(r10)
  guest::StoreU64(base, uint32_t(r10) + 16, r7);
  // std r11,24(r10)
  guest::StoreU64(base, uint32_t(r10) + 24, r11);
  // ld r4,128(r1)
  r4 = guest::LoadU64(base, uint32_t(r1) + 128);
  // bl 0x822952d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XShowGamerCardUI(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, 0x82265624); r3 = o.r3; f1 = o.f1; }
loc_82265624:
  // lwz r11,24912(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24912);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82265660
  if (cr6.eq) goto loc_82265660;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226563C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24908(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82265650
  if (cr0.eq) goto loc_82265650;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82265650:
  // stw r22,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r22));
  // stw r27,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r27));
  // stw r23,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r23));
loc_8226565C:
  // stw r23,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r23));
loc_82265660:
  // addi r1,r1,1552
  r1 = uint64_t(int64_t(int64_t(r1) + 1552));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82265668: lifted from generated/ by lift.py.
lifted::Result f82265668(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82265668 (lifted)");
  uint64_t r8 = 0, r9 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // addi r9,r11,30752
  r9 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,88(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r8,r11,26160
  r8 = uint64_t(int64_t(int64_t(r11) + 26160));
  // bne cr6,0x822656d8
  if (!cr6.eq) goto loc_822656D8;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29448
  r11 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x822656bc
  if (!cr6.gt) goto loc_822656BC;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_822656BC:
  // subf r11,r10,r11
  r11 = r11 - r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822656e4
  if (cr0.eq) goto loc_822656E4;
  // lwz r11,4(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x822656e4
  if (!cr6.eq) goto loc_822656E4;
loc_822656D8:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r8)
  guest::StoreU32(base, uint32_t(r8) + 4, uint32_t(r11));
  // b 0x822656e8
  goto loc_822656E8;
loc_822656E4:
  // lwz r11,4(r8)
  r11 = guest::LoadU32(base, uint32_t(r8) + 4);
loc_822656E8:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x822657c0
  if (cr6.eq) goto loc_822657C0;
  // bl 0x8214a728
  // (left out: dead in C++)
  { const lifted::Result o = f8214A728(base, r1, r13, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x822657c0
  if (!cr0.eq) goto loc_822657C0;
  // li r3,4106
  r3 = uint64_t(int64_t(4106));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24908(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x8226571c
  if (cr6.eq) goto loc_8226571C;
  // stw r11,8(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).previous_page_address(), uint32_t(r11));
loc_8226571C:
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // stw r6,12(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).step_address(), uint32_t(r6));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r8,0(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).page_address(), uint32_t(r8));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r6,16(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).queued_step_address(), uint32_t(r6));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r6,40
  r6 = uint64_t(int64_t(40));
  // lwz r3,604(r11)
  r3 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r3,r3,r10
  r3 = r3 + r10;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r9,604(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
loc_822657C0:
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

// sub_822657D8: lifted from generated/ by lift.py.
lifted::Result f822657D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822657D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,9684(r29)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9684_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82265848
  if (!cr6.eq) goto loc_82265848;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r30,2
  r30 = uint64_t(int64_t(2));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,23680(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 23680);
  // stw r30,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r30));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).is_bank_loading_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226581C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82265848
  if (!cr0.eq) goto loc_82265848;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r30,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r30));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x82175850
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Play(base, LIFTED_ARGS(0x8226583C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9684(r29)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9684_address(), uint32_t(r11));
  // b 0x82265918
  goto loc_82265918;
loc_82265848:
  // lwz r11,9640(r29)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9640_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82265918
  if (!cr6.eq) goto loc_82265918;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24908(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8226590c
  if (cr6.lt) goto loc_8226590C;
  // beq cr6,0x822658fc
  if (cr6.eq) goto loc_822658FC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x822658ac
  if (cr6.lt) goto loc_822658AC;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // beq cr6,0x8226589c
  if (cr6.eq) goto loc_8226589C;
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // bne cr6,0x82265918
  if (!cr6.eq) goto loc_82265918;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4107
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4107, so);
  // bne cr6,0x82265918
  if (!cr6.eq) goto loc_82265918;
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82265918
  goto loc_82265918;
loc_8226589C:
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x82265668
  // (left out: dead in C++)
  { const lifted::Result o = f82265668(base, r1, r13, r3, r4, r5, r6, r7, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82265918
  goto loc_82265918;
loc_822658AC:
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,29
  r9 = uint64_t(int64_t(29));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // b 0x82265918
  goto loc_82265918;
loc_822658FC:
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x82263d80
  // (left out: dead in C++)
  r3 = f82263D80(base, r1, r13, r3, r4);
  // b 0x82265918
  goto loc_82265918;
loc_8226590C:
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x822651a0
  // (left out: dead in C++)
  { const lifted::Result o = f822651A0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82265918:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82265930: lifted from generated/ by lift.py.
lifted::Result f82265930(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82265930 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r30,-21596(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82265958, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // stw r10,88(r11)
  guest::StoreU32(base, uint32_t(r11) + 88, uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24848(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24848);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // stw r10,28180(r9)
  guest::StoreU32(base, uint32_t(r9) + 28180, uint32_t(r10));
  // lwz r10,80(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 80);
  // bne cr6,0x8226598c
  if (!cr6.eq) goto loc_8226598C;
  // rlwinm r10,r10,0,28,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFFFEF;
  // b 0x82265990
  goto loc_82265990;
loc_8226598C:
  // ori r10,r10,16
  r10 = r10 | 16;
loc_82265990:
  // li r3,9
  r3 = uint64_t(int64_t(9));
  // stw r10,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r10));
  // stw r30,84(r11)
  guest::StoreU32(base, uint32_t(r11) + 84, uint32_t(r30));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9652(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9652_address(), uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x822659c0
  if (cr6.eq) goto loc_822659C0;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822659C0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
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

// sub_822659F0: lifted from generated/ by lift.py.
lifted::Result f822659F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822659F0 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,28180(r10)
  guest::StoreU32(base, uint32_t(r10) + 28180, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,29428(r10)
  guest::StoreU32(base, uint32_t(r10) + 29428, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r10,80(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 80);
  // ori r10,r10,16
  r10 = r10 | 16;
  // stw r10,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r10));
  // bl 0x821495e8
  // (left out: dead in C++)
  { const lifted::Result o = f821495E8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9652(r31)
  guest::StoreU32(base, uint32_t(r31) + 9652, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82265a5c
  if (cr6.eq) goto loc_82265A5C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82265A5C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
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

// sub_82265C58: lifted from generated/ by lift.py.
void f82265C58(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82265C58 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82265c9c
  if (cr6.eq) goto loc_82265C9C;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82265c7c
  if (cr0.eq) goto loc_82265C7C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82265C7C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r8,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r8));
  // blr
  return;
loc_82265C9C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_82265CA8: lifted from generated/ by lift.py.
lifted::Result f82265CA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82265CA8 (lifted)");
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
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82265CCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x82265CD4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82265CE8: lifted from generated/ by lift.py.
void f82265CE8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82265CE8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,9632(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9632);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82265d00
  if (!cr6.eq) goto loc_82265D00;
loc_82265CF8:
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // b 0x82265d70
  goto loc_82265D70;
loc_82265D00:
  // lwz r11,9648(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9648);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82265d30
  if (!cr6.eq) goto loc_82265D30;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24960);
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21696
  r11 = uint64_t(int64_t(int64_t(r11) + -21696));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // b 0x82265d70
  goto loc_82265D70;
loc_82265D30:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24920(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24920);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82265cf8
  if (cr6.lt) goto loc_82265CF8;
  // beq cr6,0x82265d58
  if (cr6.eq) goto loc_82265D58;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x82265d6c
  if (!cr6.lt) goto loc_82265D6C;
  // li r9,14
  r9 = uint64_t(int64_t(14));
  // b 0x82265d70
  goto loc_82265D70;
loc_82265D58:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24960);
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // b 0x82265d70
  goto loc_82265D70;
loc_82265D6C:
  // lwz r9,-16(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + -16);
loc_82265D70:
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // mr r8,r7
  r8 = r7;
  // li r4,9
  r4 = uint64_t(int64_t(9));
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
  // stw r4,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r4));
  // blr
  return;
}

// sub_82265E80: lifted from generated/ by lift.py.
lifted::Result f82265E80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82265E80 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82265EAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x82265EBC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82265eec
  if (!cr0.eq) goto loc_82265EEC;
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82265EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82265EEC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82265EEC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82265EFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82265F0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82265F1C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82265F2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82265F40, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82265F54, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82265F68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82265F7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,-31776(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + -31776);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetDrawWord(base, LIFTED_ARGS(0x82265F90, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31776(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + -31776);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // bl 0x8217fe60
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::Draw(base, LIFTED_ARGS(0x82265FA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31776(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + -31776);
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::Command(base, LIFTED_ARGS(0x82265FB4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x82265FC0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8226600c
  if (!cr0.eq) goto loc_8226600C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,86
  r4 = uint64_t(int64_t(86));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82265FE4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82266000, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226600C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226600C:
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

// sub_82266028: lifted from generated/ by lift.py.
lifted::Result f82266028(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82266028 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r28,r3
  r28 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r3,24920(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x82266048, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24920(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82266054, r31)); r3 = o.r3; f1 = o.f1; }
  // li r27,-1
  r27 = uint64_t(int64_t(-1));
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // beq 0x822661f8
  if (cr0.eq) goto loc_822661F8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82266070, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // mr r11,r26
  r11 = r26;
  // stw r11,24924(r10)
  guest::StoreU32(base, uint32_t(r10) + 24924, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r11,24852(r10)
  guest::StoreU32(base, uint32_t(r10) + 24852, uint32_t(r11));
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8226616c
  if (cr6.lt) goto loc_8226616C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82266140
  if (cr6.lt) goto loc_82266140;
  // beq cr6,0x82266140
  if (cr6.eq) goto loc_82266140;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x822662a4
  if (!cr6.lt) goto loc_822662A4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822603d8
  // (left out: dead in C++)
  { const lifted::Result o = f822603D8(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // mr r7,r10
  r7 = r10;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r6,r6,r9
  r6 = r6 + r9;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r5,r6,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r6) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r9,r5,r6
  r9 = r6 - r5;
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r11,r9,r11
  r11 = r9 + r11;
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // lwz r11,24916(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24916);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // beq cr6,0x82266124
  if (cr6.eq) goto loc_82266124;
  // stw r9,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_82266124:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r27,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r27));
  // stw r27,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r27));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,9640(r28)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r28)).unknown_9640_address(), uint32_t(r10));
  // stw r10,9652(r28)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r28)).unknown_9652_address(), uint32_t(r10));
  // b 0x822662a4
  goto loc_822662A4;
loc_82266140:
  // lwz r11,24916(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x82266154
  if (cr6.eq) goto loc_82266154;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82266154:
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r27,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r27));
  // stw r27,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r27));
  // b 0x822662a0
  goto loc_822662A0;
loc_8226616C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // mr r7,r10
  r7 = r10;
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r6,r6,r9
  r6 = r6 + r9;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r5,r6,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r6) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r9,r5,r6
  r9 = r6 - r5;
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r11,r9,r11
  r11 = r9 + r11;
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // lwz r11,24916(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24916);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // beq cr6,0x822661d4
  if (cr6.eq) goto loc_822661D4;
  // stw r9,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_822661D4:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r27,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r27));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r27,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r27));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,9640(r28)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r28)).unknown_9640_address(), uint32_t(r10));
  // stw r10,9652(r28)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r28)).unknown_9652_address(), uint32_t(r10));
  // bl 0x822603d8
  // (left out: dead in C++)
  { const lifted::Result o = f822603D8(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822662a4
  goto loc_822662A4;
loc_822661F8:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24920(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82266204, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822662a4
  if (cr0.eq) goto loc_822662A4;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82266214, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // mr r11,r10
  r11 = r10;
  // stw r11,24924(r9)
  guest::StoreU32(base, uint32_t(r9) + 24924, uint32_t(r11));
  // lwz r11,24916(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24916);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x82266238
  if (cr0.eq) goto loc_82266238;
  // stw r9,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_82266238:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // stw r26,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r26));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r10));
  // stw r27,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r27));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // stw r26,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r26));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24848(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_822662A0:
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
loc_822662A4:
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822662e4
  if (cr6.eq) goto loc_822662E4;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822662BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24916(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x822662d0
  if (cr6.eq) goto loc_822662D0;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822662D0:
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // stw r27,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r27));
  // stw r26,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r26));
  // stw r27,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r27));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
loc_822662E4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822662F0: lifted from generated/ by lift.py.
lifted::Result f822662F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822662F0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r30,r4
  r30 = r4;
  // lwz r29,-21596(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82266314, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // mr r7,r8
  r7 = r8;
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r9,16
  r9 = uint64_t(int64_t(16));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82266384
  if (cr6.eq) goto loc_82266384;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82266384:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r5,60
  r5 = uint64_t(int64_t(60));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // mr r11,r8
  r11 = r8;
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // stw r8,9640(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9640_address(), uint32_t(r8));
  // stw r8,9652(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9652_address(), uint32_t(r8));
  // addi r10,r10,30752
  r10 = uint64_t(int64_t(int64_t(r10) + 30752));
  // addi r3,r10,8
  r3 = uint64_t(int64_t(int64_t(r10) + 8));
  // stb r11,27404(r9)
  guest::StoreU8(base, uint32_t(r9) + 27404, uint8_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r29,84(r10)
  guest::StoreU32(base, uint32_t(r10) + 84, uint32_t(r29));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r4,4(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 4);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822663D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82148fd8
  // (left out: dead in C++)
  { const lifted::Result o = f82148FD8(base, r1, r13, r3, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822663E0: lifted from generated/ by lift.py.
lifted::Result f822663E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822663E0 (lifted)");
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
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,9652(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9652_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82266540
  if (!cr6.eq) goto loc_82266540;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // bne cr6,0x82266430
  if (!cr6.eq) goto loc_82266430;
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
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8226642C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82266540
  goto loc_82266540;
loc_82266430:
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x82266434);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x822664e0
  if (!cr0.lt) goto loc_822664E0;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r6,40
  r6 = uint64_t(int64_t(40));
  // lwz r3,604(r11)
  r3 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r3,r10
  r3 = r3 + r10;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r28,r3,r31
  r28 = uint32_t(uint32_t(r31) ? uint32_t(r3) / uint32_t(r31) : 0);
  // mulli r28,r28,50
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(50))));
  // subf r10,r28,r3
  r10 = r3 - r28;
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
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r9,604(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  // stw r29,9652(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9652_address(), uint32_t(r29));
  // b 0x82266540
  goto loc_82266540;
loc_822664E0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822601f0
  // (left out: dead in C++)
  r3 = f822601F0(base, r1, r13, r3);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24852(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24852);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // bne cr6,0x82266518
  if (!cr6.eq) goto loc_82266518;
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x82266510
  if (cr6.eq) goto loc_82266510;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82266510:
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // b 0x82266528
  goto loc_82266528;
loc_82266518:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x82266524
  if (cr6.eq) goto loc_82266524;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82266524:
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_82266528:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9652(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9652_address(), uint32_t(r11));
loc_82266540:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82266548: lifted from generated/ by lift.py.
lifted::Result f82266548(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82266548 (lifted)");
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,9652(r31)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9652_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226664c
  if (!cr6.eq) goto loc_8226664C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // bne cr6,0x82266598
  if (!cr6.eq) goto loc_82266598;
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
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82266594, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226664c
  goto loc_8226664C;
loc_82266598:
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8226659C);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x822665b4
  if (!cr0.lt) goto loc_822665B4;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,40
  r5 = uint64_t(int64_t(40));
  // b 0x822665b8
  goto loc_822665B8;
loc_822665B4:
  // li r5,39
  r5 = uint64_t(int64_t(39));
loc_822665B8:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r10,50
  r10 = uint64_t(int64_t(50));
  // li r8,37
  r8 = uint64_t(int64_t(37));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r30,604(r11)
  r30 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // add r30,r30,r9
  r30 = r30 + r9;
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r28,r30,r10
  r28 = uint32_t(uint32_t(r10) ? uint32_t(r30) / uint32_t(r10) : 0);
  // mulli r28,r28,50
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(50))));
  // subf r9,r28,r30
  r9 = r30 - r28;
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r9,r9,r11
  r9 = r9 + r11;
  // stw r8,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r8));
  // stw r7,4(r9)
  guest::StoreU32(base, uint32_t(r9) + 4, uint32_t(r7));
  // stw r6,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r6));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r8,604(r11)
  r8 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r8,r8,r9
  r8 = r8 + r9;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r10,r8,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r8) / uint32_t(r10) : 0);
  // mulli r10,r10,50
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(50))));
  // subf r10,r10,r8
  r10 = r8 - r10;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r5,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r5));
  // stw r4,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r4));
  // stw r3,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r3));
  // stw r29,9652(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9652_address(), uint32_t(r29));
loc_8226664C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82266658: lifted from generated/ by lift.py.
uint64_t f82266658(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82266658 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,9652(r31)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9652_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82266748
  if (!cr6.eq) goto loc_82266748;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // beq cr6,0x82266748
  if (cr6.eq) goto loc_82266748;
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x82266690);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r5,40
  r5 = uint64_t(int64_t(40));
  // blt 0x822666b4
  if (cr0.lt) goto loc_822666B4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822666b4
  if (cr6.eq) goto loc_822666B4;
  // li r5,39
  r5 = uint64_t(int64_t(39));
loc_822666B4:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r10,50
  r10 = uint64_t(int64_t(50));
  // li r8,37
  r8 = uint64_t(int64_t(37));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r30,604(r11)
  r30 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // add r30,r30,r9
  r30 = r30 + r9;
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r28,r30,r10
  r28 = uint32_t(uint32_t(r10) ? uint32_t(r30) / uint32_t(r10) : 0);
  // mulli r28,r28,50
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(50))));
  // subf r9,r28,r30
  r9 = r30 - r28;
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r9,r9,r11
  r9 = r9 + r11;
  // stw r8,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r8));
  // stw r7,4(r9)
  guest::StoreU32(base, uint32_t(r9) + 4, uint32_t(r7));
  // stw r6,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r6));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r8,604(r11)
  r8 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r8,r8,r9
  r8 = r8 + r9;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r10,r8,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r8) / uint32_t(r10) : 0);
  // mulli r10,r10,50
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(50))));
  // subf r10,r10,r8
  r10 = r8 - r10;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r5,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r5));
  // stw r4,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r4));
  // stw r3,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r3));
  // stw r29,9652(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9652_address(), uint32_t(r29));
loc_82266748:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82266750: lifted from generated/ by lift.py.
lifted::Result f82266750(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82266750 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r3
  r28 = r3;
  // mr r27,r4
  r27 = r4;
  // lwz r11,9652(r28)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r28)).unknown_9652_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82266900
  if (!cr6.eq) goto loc_82266900;
  // bl 0x8214d7a0
  // (left out: dead in C++)
  { const lifted::Result o = f8214D7A0(base, r1, r13, r6, r7, r9); r3 = o.r3; f1 = o.f1; }
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x82266778, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r11,29420(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29420);
  // subf r11,r11,r3
  r11 = r3 - r11;
  // cmplwi cr6,r11,10000
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10000, so);
  // ble cr6,0x82266860
  if (!cr6.gt) goto loc_82266860;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // stw r10,88(r11)
  guest::StoreU32(base, uint32_t(r11) + 88, uint32_t(r10));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r27,204
  r11 = uint64_t(int64_t(int64_t(r27) + 204));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,40
  r6 = uint64_t(int64_t(40));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r30,604(r11)
  r30 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r30,r30,r10
  r30 = r30 + r10;
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r26,r30,r31
  r26 = uint32_t(uint32_t(r31) ? uint32_t(r30) / uint32_t(r31) : 0);
  // mulli r26,r26,50
  r26 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(50))));
  // subf r10,r26,r30
  r10 = r30 - r26;
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
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r9,604(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  // stw r29,9652(r28)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r28)).unknown_9652_address(), uint32_t(r29));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24880(r10)
  guest::StoreU32(base, uint32_t(r10) + 24880, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24884(r10)
  guest::StoreU32(base, uint32_t(r10) + 24884, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24888(r10)
  guest::StoreU32(base, uint32_t(r10) + 24888, uint32_t(r11));
loc_82266860:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4096
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4096, so);
  // bne cr6,0x82266900
  if (!cr6.eq) goto loc_82266900;
  // addi r11,r27,204
  r11 = uint64_t(int64_t(int64_t(r27) + 204));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,39
  r6 = uint64_t(int64_t(39));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,604(r11)
  r3 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r3,r3,r10
  r3 = r3 + r10;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r9,604(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  // stw r29,9652(r28)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r28)).unknown_9652_address(), uint32_t(r29));
loc_82266900:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82266908: lifted from generated/ by lift.py.
lifted::Result f82266908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82266908 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r4
  r29 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82266930, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82266a84
  if (!cr6.eq) goto loc_82266A84;
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266954, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,301
  r6 = uint64_t(int64_t(301));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266968, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,115
  r6 = uint64_t(int64_t(115));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226697C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822669b8
  if (cr6.lt) goto loc_822669B8;
  // beq cr6,0x822669b0
  if (cr6.eq) goto loc_822669B0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x822669cc
  if (!cr6.lt) goto loc_822669CC;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x822669AC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822669cc
  goto loc_822669CC;
loc_822669B0:
  // li r6,938
  r6 = uint64_t(int64_t(938));
  // b 0x822669bc
  goto loc_822669BC;
loc_822669B8:
  // li r6,937
  r6 = uint64_t(int64_t(937));
loc_822669BC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822669CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_822669CC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x822669DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x822669F8, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82266A0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82266A24, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // li r4,89
  r4 = uint64_t(int64_t(89));
  // mr r3,r31
  r3 = r31;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82266A60, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x82266A7C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_82266A84:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82266A90, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
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
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82266AC0: lifted from generated/ by lift.py.
lifted::Result f82266AC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82266AC0 (lifted)");
  uint64_t r3 = 0, r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r4
  r28 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82266AE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82266c64
  if (!cr6.eq) goto loc_82266C64;
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82266B04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82266B10, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_82266B14:
  // addi r4,r30,7
  r4 = uint64_t(int64_t(int64_t(r30) + 7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82266B20, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // blt cr6,0x82266b14
  if (cr6.lt) goto loc_82266B14;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82266b5c
  if (cr6.lt) goto loc_82266B5C;
  // beq cr6,0x82266b54
  if (cr6.eq) goto loc_82266B54;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82266b70
  if (!cr6.lt) goto loc_82266B70;
  // li r6,918
  r6 = uint64_t(int64_t(918));
  // b 0x82266b60
  goto loc_82266B60;
loc_82266B54:
  // li r6,938
  r6 = uint64_t(int64_t(938));
  // b 0x82266b60
  goto loc_82266B60;
loc_82266B5C:
  // li r6,937
  r6 = uint64_t(int64_t(937));
loc_82266B60:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266B70, r31)); r3 = o.r3; f1 = o.f1; }
loc_82266B70:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82266B80, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x82266B9C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82266BB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // lwz r29,0(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82266BC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x82266BF4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82266c24
  if (!cr0.eq) goto loc_82266C24;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82266C18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82266C24, r31)); r3 = o.r3; f1 = o.f1; }
loc_82266C24:
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
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
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82266C48, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82266C5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_82266C64:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82266C70, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82266C80: lifted from generated/ by lift.py.
lifted::Result f82266C80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82266C80 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r4
  r29 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82266CA8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82266e50
  if (!cr6.eq) goto loc_82266E50;
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82266CC4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82266CD0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82266CE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82266D0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82266D20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82266d70
  if (!cr6.eq) goto loc_82266D70;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x82266d68
  if (!cr6.eq) goto loc_82266D68;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82266d68
  if (!cr6.eq) goto loc_82266D68;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82266D58, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
loc_82266D60:
  // stfs f0,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).live_scene_address(), temp.u32);
  // b 0x82266dc8
  goto loc_82266DC8;
loc_82266D68:
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // b 0x82266db0
  goto loc_82266DB0;
loc_82266D70:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82266dcc
  if (!cr6.eq) goto loc_82266DCC;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82266dac
  if (!cr6.eq) goto loc_82266DAC;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82266dac
  if (!cr6.eq) goto loc_82266DAC;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82266DA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,7820(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7820);
  f0 = double(temp.f32);
  // b 0x82266d60
  goto loc_82266D60;
loc_82266DAC:
  // li r4,18
  r4 = uint64_t(int64_t(18));
loc_82266DB0:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82266DC8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82266DC8:
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
loc_82266DCC:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82266df4
  if (cr6.lt) goto loc_82266DF4;
  // beq cr6,0x82266dec
  if (cr6.eq) goto loc_82266DEC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82266e08
  if (!cr6.lt) goto loc_82266E08;
  // li r6,918
  r6 = uint64_t(int64_t(918));
  // b 0x82266df8
  goto loc_82266DF8;
loc_82266DEC:
  // li r6,938
  r6 = uint64_t(int64_t(938));
  // b 0x82266df8
  goto loc_82266DF8;
loc_82266DF4:
  // li r6,937
  r6 = uint64_t(int64_t(937));
loc_82266DF8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266E08, r31)); r3 = o.r3; f1 = o.f1; }
loc_82266E08:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82266E18, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x82266E34, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82266E48, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_82266E50:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82266E5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82266E68: lifted from generated/ by lift.py.
lifted::Result f82266E68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82266E68 (lifted)");
  uint64_t r3 = 0, r5 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r4
  r27 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82266E90, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226708c
  if (!cr6.eq) goto loc_8226708C;
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24848(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82266edc
  if (!cr6.eq) goto loc_82266EDC;
  // addi r6,r11,300
  r6 = uint64_t(int64_t(int64_t(r11) + 300));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266EC8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x82266ED8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82266ef4
  goto loc_82266EF4;
loc_82266EDC:
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x82266EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,301
  r6 = uint64_t(int64_t(301));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266EF4, r31)); r3 = o.r3; f1 = o.f1; }
loc_82266EF4:
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x82266F04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,302
  r6 = uint64_t(int64_t(302));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266F18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,304
  r6 = uint64_t(int64_t(304));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266F2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,303
  r6 = uint64_t(int64_t(303));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266F40, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,110
  r6 = uint64_t(int64_t(110));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266F54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24848(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24848);
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x82266f78
  if (!cr6.eq) goto loc_82266F78;
  // li r6,309
  r6 = uint64_t(int64_t(309));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266F74, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82266f7c
  goto loc_82266F7C;
loc_82266F78:
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x82266F7C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82266F7C:
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r6,r11,939
  r6 = uint64_t(int64_t(int64_t(r11) + 939));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82266F9C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82266FAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x82266FC8, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82266FDC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82266FEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24848(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24848);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82266FFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82267014, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,90
  r4 = uint64_t(int64_t(90));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).live_scene_address(), temp.u32);
  // lwz r11,24848(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24848);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82267058
  if (cr6.eq) goto loc_82267058;
  // li r4,93
  r4 = uint64_t(int64_t(93));
loc_82267058:
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226705C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24924(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24924);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82267084
  if (!cr6.eq) goto loc_82267084;
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x82267084, r31)); r3 = o.r3; f1 = o.f1; }
loc_82267084:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r11));
loc_8226708C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82267098, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24924(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24924);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bne cr6,0x822670c8
  if (!cr6.eq) goto loc_822670C8;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
loc_822670C8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822670D0: lifted from generated/ by lift.py.
lifted::Result f822670D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822670D0 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r4
  r28 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822670F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822671f0
  if (!cr6.eq) goto loc_822671F0;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_8226710C:
  // addi r4,r30,46
  r4 = uint64_t(int64_t(int64_t(r30) + 46));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82267118, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 5, so);
  // blt cr6,0x8226710c
  if (cr6.lt) goto loc_8226710C;
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82267130, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,57
  r4 = uint64_t(int64_t(57));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226713C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82267150, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,301
  r6 = uint64_t(int64_t(301));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82267164, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,115
  r6 = uint64_t(int64_t(115));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82267178, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82267188, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // lwz r29,24848(r30)
  r29 = guest::LoadU32(base, uint32_t(r30) + 24848);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822671A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,92
  r4 = uint64_t(int64_t(92));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).live_scene_address(), temp.u32);
  // lwz r11,24848(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24848);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822671e4
  if (cr6.eq) goto loc_822671E4;
  // li r4,95
  r4 = uint64_t(int64_t(95));
loc_822671E4:
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822671E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_822671F0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822671FC, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82267210: lifted from generated/ by lift.py.
lifted::Result f82267210(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82267210 (lifted)");
  uint64_t r3 = 0, r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r4
  r28 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82267238, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82267434
  if (!cr6.eq) goto loc_82267434;
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24848(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82267280
  if (!cr6.eq) goto loc_82267280;
  // addi r6,r11,300
  r6 = uint64_t(int64_t(int64_t(r11) + 300));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226726C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x8226727C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82267298
  goto loc_82267298;
loc_82267280:
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x82267284, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,301
  r6 = uint64_t(int64_t(301));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82267298, r31)); r3 = o.r3; f1 = o.f1; }
loc_82267298:
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x822672A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,302
  r6 = uint64_t(int64_t(302));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822672BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,304
  r6 = uint64_t(int64_t(304));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822672D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,303
  r6 = uint64_t(int64_t(303));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822672E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,110
  r6 = uint64_t(int64_t(110));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822672F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24848(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24848);
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x8226731c
  if (!cr6.eq) goto loc_8226731C;
  // li r6,309
  r6 = uint64_t(int64_t(309));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82267318, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82267320
  goto loc_82267320;
loc_8226731C:
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x82267320, r31)); r3 = o.r3; f1 = o.f1; }
loc_82267320:
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r6,r11,939
  r6 = uint64_t(int64_t(int64_t(r11) + 939));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82267340, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82267350, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x8226736C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82267380, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82267390, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822673A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r11,r3
  r11 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // lfs f0,8112(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 8112);
  f0 = double(temp.f32);
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822673C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,91
  r4 = uint64_t(int64_t(91));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).live_scene_address(), temp.u32);
  // lwz r11,24848(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24848);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8226740c
  if (cr6.eq) goto loc_8226740C;
  // li r4,94
  r4 = uint64_t(int64_t(94));
loc_8226740C:
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82267410, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8226742C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_82267434:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82267440, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
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
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82267470: lifted from generated/ by lift.py.
lifted::Result f82267470(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82267470 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r4
  r29 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82267498, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82267528
  if (!cr6.eq) goto loc_82267528;
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x822674B4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x822674e4
  if (!cr0.eq) goto loc_822674E4;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822674D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822674E4, r31)); r3 = o.r3; f1 = o.f1; }
loc_822674E4:
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,46
  r4 = uint64_t(int64_t(int64_t(r11) + 46));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226750C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,46
  r4 = uint64_t(int64_t(int64_t(r11) + 46));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82267520, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_82267528:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82267534, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82267540: lifted from generated/ by lift.py.
lifted::Result f82267540(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82267540 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r4
  r27 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82267568, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // addi r11,r11,0
  r11 = uint64_t(int64_t(int64_t(r11) + 0));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r28,r11,4
  r28 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne cr6,0x8226771c
  if (!cr6.eq) goto loc_8226771C;
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226759C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,57
  r4 = uint64_t(int64_t(57));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822675A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r4,r11,46
  r4 = uint64_t(int64_t(int64_t(r11) + 46));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822675C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,46
  r4 = uint64_t(int64_t(int64_t(r11) + 46));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822675E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,46
  r4 = uint64_t(int64_t(int64_t(r11) + 46));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822675F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8226764c
  if (!cr6.eq) goto loc_8226764C;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r9,r28,-1
  r9 = uint64_t(int64_t(int64_t(r28) + -1));
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // bne cr6,0x82267644
  if (!cr6.eq) goto loc_82267644;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82267644
  if (!cr6.eq) goto loc_82267644;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82267634, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
loc_8226763C:
  // stfs f0,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).live_scene_address(), temp.u32);
  // b 0x822676b8
  goto loc_822676B8;
loc_82267644:
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // b 0x822676a0
  goto loc_822676A0;
loc_8226764C:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x822676bc
  if (!cr6.eq) goto loc_822676BC;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226769c
  if (!cr6.eq) goto loc_8226769C;
  // lwz r29,0(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r28,-1
  r11 = uint64_t(int64_t(int64_t(r28) + -1));
  // cmpw cr6,r11,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r29), so);
  // bne cr6,0x8226769c
  if (!cr6.eq) goto loc_8226769C;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82267680, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
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
  // b 0x8226763c
  goto loc_8226763C;
loc_8226769C:
  // li r4,57
  r4 = uint64_t(int64_t(57));
loc_822676A0:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822676B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822676B8:
  // lwz r11,24920(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24920);
loc_822676BC:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r6,r11,939
  r6 = uint64_t(int64_t(int64_t(r11) + 939));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822676D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x822676E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x82267700, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82267714, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r11));
loc_8226771C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82267728, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82267738: lifted from generated/ by lift.py.
lifted::Result f82267738(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82267738 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82267760, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82267774, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82267788, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8226779C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822677A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_822677C0: lifted from generated/ by lift.py.
lifted::Result f822677C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822677C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822677E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822677FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82267808, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_82267820: lifted from generated/ by lift.py.
lifted::Result f82267820(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82267820 (lifted)");
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
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82267850, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82267884
  if (!cr6.eq) goto loc_82267884;
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
  r3 = guest::LoadU32(base, uint32_t(r11) + 7152);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x8226787C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_82267884:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82267890, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
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

// sub_822678D0: lifted from generated/ by lift.py.
uint64_t f822678D0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_822678D0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822678F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8226790C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8226791C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226792C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226793C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82267950, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82267964, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82267978, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8226798C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_822679A0: lifted from generated/ by lift.py.
uint64_t f822679A0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_822679A0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822679C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822679DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822679EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822679FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82267A0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82267A20, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82267A34, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82267A48: lifted from generated/ by lift.py.
lifted::Result f82267A48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82267A48 (lifted)");
  uint64_t r0 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-4064(r1)
  ea = -4064 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,38
  r11 = uint64_t(int64_t(38));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82267A70, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r26,0(r3)
  r26 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r11,24916(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24916);
  // addi r4,r11,24
  r4 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).step_address());
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82267aac
  if (!cr6.eq) goto loc_82267AAC;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r11,13
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 13, so);
  // beq cr6,0x82267bb8
  if (cr6.eq) goto loc_82267BB8;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82267AA8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82267bb8
  goto loc_82267BB8;
loc_82267AAC:
  // cmplwi cr6,r10,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 10, so);
  // bgt cr6,0x82267b80
  if (cr6.gt) goto loc_82267B80;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6176
  r12 = uint64_t(int64_t(int64_t(r12) + 6176));
  // lbzx r0,r12,r10
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r10));
  // lis r12,-32218
  r12 = uint64_t(int64_t(-2111438848));
  // addi r12,r12,31452
  r12 = uint64_t(int64_t(int64_t(r12) + 31452));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r10)) {
  case 0:
  goto loc_frag_82267ADC;
  return {r3, f1};
  case 1:
  goto loc_82267B08;
  case 2:
  goto loc_82267B14;
  case 3:
  goto loc_82267B20;
  case 4:
  goto loc_82267B2C;
  case 5:
  goto loc_82267B38;
  case 6:
  goto loc_82267B44;
  case 7:
  goto loc_82267B50;
  case 8:
  goto loc_82267B5C;
  case 9:
  goto loc_82267B68;
  case 10:
  goto loc_82267B74;
  default:
  std::abort();
  }
loc_82267AE4:
  // lwz r11,24916(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24916);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82267bb8
  if (cr0.eq) goto loc_82267BB8;
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address());
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82267ba0
  if (!cr6.eq) goto loc_82267BA0;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address());
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // b 0x82267bb0
  goto loc_82267BB0;
loc_82267B08:
  // mr r3,r29
  r3 = r29;
  // bl 0x82266ac0
  // (left out: dead in C++)
  { const lifted::Result o = f82266AC0(base, r1, r13, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B14:
  // mr r3,r29
  r3 = r29;
  // bl 0x82266c80
  // (left out: dead in C++)
  { const lifted::Result o = f82266C80(base, r1, r13, r4, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B20:
  // mr r3,r29
  r3 = r29;
  // bl 0x82266e68
  // (left out: dead in C++)
  { const lifted::Result o = f82266E68(base, r1, r13, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B2C:
  // mr r3,r29
  r3 = r29;
  // bl 0x822670d0
  // (left out: dead in C++)
  { const lifted::Result o = f822670D0(base, r1, r13, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B38:
  // mr r3,r29
  r3 = r29;
  // bl 0x82267210
  // (left out: dead in C++)
  { const lifted::Result o = f82267210(base, r1, r13, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B44:
  // mr r3,r29
  r3 = r29;
  // bl 0x82267470
  // (left out: dead in C++)
  { const lifted::Result o = f82267470(base, r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B50:
  // mr r3,r29
  r3 = r29;
  // bl 0x82267540
  // (left out: dead in C++)
  { const lifted::Result o = f82267540(base, r1, r13, r4, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B5C:
  // mr r3,r29
  r3 = r29;
  // bl 0x82267738
  // (left out: dead in C++)
  { const lifted::Result o = f82267738(base, r1, r13, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B68:
  // mr r3,r29
  r3 = r29;
  // bl 0x822677c0
  // (left out: dead in C++)
  { const lifted::Result o = f822677C0(base, r1, r13, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B74:
  // mr r3,r29
  r3 = r29;
  // bl 0x82267820
  // (left out: dead in C++)
  { const lifted::Result o = f82267820(base, r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82267ae4
  goto loc_82267AE4;
loc_82267B80:
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82267ba0
  if (!cr6.eq) goto loc_82267BA0;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // b 0x82267bb4
  goto loc_82267BB4;
loc_82267BA0:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // stw r27,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r27));
loc_82267BB0:
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_82267BB4:
  // stw r27,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r27));
loc_82267BB8:
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + -31776);
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // lwz r11,9656(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9656);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82267c5c
  if (!cr6.eq) goto loc_82267C5C;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24960);
  // lwz r3,844(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 844);
  // bl 0x8215aad0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarModels::BlockOf(base, LIFTED_ARGS(0x82267BE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8217f880
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::ReloadModel(base, LIFTED_ARGS(0x82267BEC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x82267c5c
  if (!cr6.eq) goto loc_82267C5C;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x82267BFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24960);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r11,832
  r4 = uint64_t(int64_t(int64_t(r11) + 832));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82267C10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3984(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 3984);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,3984(r1)
  guest::StoreU32(base, uint32_t(r1) + 3984, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x82267C24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetDrawWord(base, LIFTED_ARGS(0x82267C30, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r4,28224(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 28224);
  // bl 0x82178de8
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetPlates(base, LIFTED_ARGS(0x82267C40, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24960);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r31
  r3 = r31;
  // lha r5,832(r11)
  r5 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r11) + 832))));
  // bl 0x8217fd58
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetCar(base, LIFTED_ARGS(0x82267C54, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r27,9656(r29)
  guest::StoreU32(base, uint32_t(r29) + 9656, uint32_t(r27));
  // b 0x82267c6c
  goto loc_82267C6C;
loc_82267C5C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8217fe60
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::Draw(base, LIFTED_ARGS(0x82267C6C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82267C6C:
  // lwz r11,9656(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9656);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82267cc8
  if (!cr6.eq) goto loc_82267CC8;
  // lwz r11,-31776(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + -31776);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // bl 0x82179be0
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::IsBusy(base, LIFTED_ARGS(0x82267C84, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82267cc8
  if (!cr0.eq) goto loc_82267CC8;
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x82267C98, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x82267cc8
  if (!cr6.eq) goto loc_82267CC8;
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82267CAC, r31)); r3 = o.r3; f1 = o.f1; }
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
  // mr r3,r26
  r3 = r26;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82267CC8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82267CC8:
  // addi r1,r1,4064
  r1 = uint64_t(int64_t(int64_t(r1) + 4064));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_frag_82267ADC:
  // mr r3,r29
  r3 = r29;
  // bl 0x82266908
  // (left out: dead in C++)
  { const lifted::Result o = f82266908(base, r1, r13, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24916(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24916);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82267bb8
  if (cr0.eq) goto loc_82267BB8_82267ADC;
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82267ba0
  if (!cr6.eq) goto loc_82267BA0_82267ADC;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x82267bb0
  goto loc_82267BB0_82267ADC;
loc_82267BA0_82267ADC:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // stw r27,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r27));
loc_82267BB0_82267ADC:
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r27,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r27));
loc_82267BB8_82267ADC:
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + -31776);
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,9656(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9656);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82267c5c
  if (!cr6.eq) goto loc_82267C5C_82267ADC;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24960);
  // lwz r3,844(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 844);
  // bl 0x8215aad0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarModels::BlockOf(base, LIFTED_ARGS(0x82267BE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8217f880
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::ReloadModel(base, LIFTED_ARGS(0x82267BEC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x82267c5c
  if (!cr6.eq) goto loc_82267C5C_82267ADC;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x82267BFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24960);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r11,832
  r4 = uint64_t(int64_t(int64_t(r11) + 832));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82267C10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3984(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 3984);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,3984(r1)
  guest::StoreU32(base, uint32_t(r1) + 3984, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x82267C24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetDrawWord(base, LIFTED_ARGS(0x82267C30, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r4,28224(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 28224);
  // bl 0x82178de8
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetPlates(base, LIFTED_ARGS(0x82267C40, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24960);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r31
  r3 = r31;
  // lha r5,832(r11)
  r5 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r11) + 832))));
  // bl 0x8217fd58
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetCar(base, LIFTED_ARGS(0x82267C54, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r27,9656(r29)
  guest::StoreU32(base, uint32_t(r29) + 9656, uint32_t(r27));
  // b 0x82267c6c
  goto loc_82267C6C_82267ADC;
loc_82267C5C_82267ADC:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8217fe60
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::Draw(base, LIFTED_ARGS(0x82267C6C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82267C6C_82267ADC:
  // lwz r11,9656(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9656);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82267cc8
  if (!cr6.eq) goto loc_82267CC8_82267ADC;
  // lwz r11,-31776(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + -31776);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x82179be0
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::IsBusy(base, LIFTED_ARGS(0x82267C84, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82267cc8
  if (!cr0.eq) goto loc_82267CC8_82267ADC;
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x82267C98, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x82267cc8
  if (!cr6.eq) goto loc_82267CC8_82267ADC;
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82267CAC, r31)); r3 = o.r3; f1 = o.f1; }
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
  // mr r3,r26
  r3 = r26;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82267CC8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82267CC8_82267ADC:
  // addi r1,r1,4064
  r1 = uint64_t(int64_t(int64_t(r1) + 4064));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82267CD0: lifted from generated/ by lift.py.
lifted::Result f82267CD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82267CD0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // mr r31,r3
  r31 = r3;
  // lwz r11,-31776(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + -31776);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::GarageStage(base, uint32_t(r11)).index_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::GarageStage(base, uint32_t(r11)).model_claimed_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82267d28
  if (!cr6.eq) goto loc_82267D28;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24960);
  // lwz r3,844(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 844);
  // bl 0x8215aad0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarModels::BlockOf(base, LIFTED_ARGS(0x82267D14, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31776(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + -31776);
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // bl 0x8217fcb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8217FCB0, LIFTED_ARGS(0x82267D28, r31)); r3 = o.r3; f1 = o.f1; }
loc_82267D28:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,9656(r31)
  guest::StoreU32(base, schema::CarMakersNetMachine(base, uint32_t(r31)).unknown_9656_address(), uint32_t(r30));
  // bl 0x82265e80
  // (left out: dead in C++)
  { const lifted::Result o = f82265E80(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r30,9640(r31)
  guest::StoreU32(base, schema::CarMakersNetMachine(base, uint32_t(r31)).unknown_9640_address(), uint32_t(r30));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,9648(r31)
  guest::StoreU32(base, schema::CarMakersNetMachine(base, uint32_t(r31)).unknown_9648_address(), uint32_t(r11));
  // stw r11,9624(r31)
  guest::StoreU32(base, schema::CarMakersNetMachine(base, uint32_t(r31)).back_address(), uint32_t(r11));
  // stw r11,9628(r31)
  guest::StoreU32(base, schema::CarMakersNetMachine(base, uint32_t(r31)).unknown_9628_address(), uint32_t(r11));
  // stw r11,9632(r31)
  guest::StoreU32(base, schema::CarMakersNetMachine(base, uint32_t(r31)).leave_address(), uint32_t(r11));
  // stw r11,9652(r31)
  guest::StoreU32(base, schema::CarMakersNetMachine(base, uint32_t(r31)).unknown_9652_address(), uint32_t(r11));
  // stw r11,9636(r31)
  guest::StoreU32(base, schema::CarMakersNetMachine(base, uint32_t(r31)).shown_address(), uint32_t(r11));
  // lwz r10,24924(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24924);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x82267e14
  if (cr6.lt) goto loc_82267E14;
  // bne cr6,0x82267e84
  if (!cr6.eq) goto loc_82267E84;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // lwz r10,24848(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24848);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r10,24920(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 24920);
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
  // stw r11,40(r10)
  guest::StoreU32(base, uint32_t(r10) + 40, uint32_t(r11));
  // stw r11,48(r10)
  guest::StoreU32(base, uint32_t(r10) + 48, uint32_t(r11));
  // stw r11,52(r10)
  guest::StoreU32(base, uint32_t(r10) + 52, uint32_t(r11));
  // stw r11,36(r10)
  guest::StoreU32(base, uint32_t(r10) + 36, uint32_t(r11));
  // stw r11,64(r10)
  guest::StoreU32(base, uint32_t(r10) + 64, uint32_t(r11));
  // stw r11,24(r10)
  guest::StoreU32(base, uint32_t(r10) + 24, uint32_t(r11));
  // stw r11,28(r10)
  guest::StoreU32(base, uint32_t(r10) + 28, uint32_t(r11));
  // stw r11,32(r10)
  guest::StoreU32(base, uint32_t(r10) + 32, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // stw r30,20(r10)
  guest::StoreU32(base, uint32_t(r10) + 20, uint32_t(r30));
  // beq cr6,0x82267dc4
  if (cr6.eq) goto loc_82267DC4;
  // li r8,3
  r8 = uint64_t(int64_t(3));
loc_82267DC4:
  // stw r8,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r8));
  // lwz r10,24852(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24852);
  // lwz r11,24920(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 24920);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82267df4
  if (cr6.eq) goto loc_82267DF4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82267DF4:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r30,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r30));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // stw r8,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r8));
  // b 0x82267e84
  goto loc_82267E84;
loc_82267E14:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24916(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24916);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x82267e2c
  if (cr0.eq) goto loc_82267E2C;
  // stw r9,8(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).previous_page_address(), uint32_t(r9));
loc_82267E2C:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r11,12(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).step_address(), uint32_t(r11));
  // stw r30,16(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).queued_step_address(), uint32_t(r30));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).next_page_address(), uint32_t(r11));
  // stw r9,0(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).page_address(), uint32_t(r9));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24920(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24920);
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // stw r8,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r8));
  // stw r30,20(r10)
  guest::StoreU32(base, uint32_t(r10) + 20, uint32_t(r30));
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
  // stw r11,40(r10)
  guest::StoreU32(base, uint32_t(r10) + 40, uint32_t(r11));
  // stw r11,48(r10)
  guest::StoreU32(base, uint32_t(r10) + 48, uint32_t(r11));
  // stw r11,52(r10)
  guest::StoreU32(base, uint32_t(r10) + 52, uint32_t(r11));
  // stw r11,36(r10)
  guest::StoreU32(base, uint32_t(r10) + 36, uint32_t(r11));
  // stw r11,64(r10)
  guest::StoreU32(base, uint32_t(r10) + 64, uint32_t(r11));
  // stw r11,24(r10)
  guest::StoreU32(base, uint32_t(r10) + 24, uint32_t(r11));
  // stw r11,28(r10)
  guest::StoreU32(base, uint32_t(r10) + 28, uint32_t(r11));
  // stw r11,32(r10)
  guest::StoreU32(base, uint32_t(r10) + 32, uint32_t(r11));
loc_82267E84:
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

// sub_82267EA0: lifted from generated/ by lift.py.
uint64_t f82267EA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82267EA0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r30,r3
  r30 = r3;
  // lwz r11,9624(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9624);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822680a4
  if (cr6.eq) goto loc_822680A4;
  // lwz r11,9632(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9632);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822680a4
  if (cr6.eq) goto loc_822680A4;
  // lwz r11,9628(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9628);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822680a4
  if (cr6.eq) goto loc_822680A4;
  // lwz r11,9636(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9636);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822680a4
  if (cr6.eq) goto loc_822680A4;
  // lwz r11,9644(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9644);
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82267fdc
  if (!cr6.eq) goto loc_82267FDC;
  // beq 0x82267fa8
  if (cr0.eq) goto loc_82267FA8;
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82267f7c
  if (cr6.eq) goto loc_82267F7C;
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x82267f54
  if (cr6.eq) goto loc_82267F54;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82267f7c
  if (cr6.eq) goto loc_82267F7C;
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x82267f3c
  if (cr6.eq) goto loc_82267F3C;
  // addi r10,r10,-10
  r10 = uint64_t(int64_t(int64_t(r10) + -10));
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // bgt cr6,0x82268094
  if (cr6.gt) goto loc_82268094;
  // b 0x82268008
  goto loc_82268008;
loc_82267F3C:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x82267f4c
  if (cr6.eq) goto loc_82267F4C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82267F4C:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // b 0x82267f68
  goto loc_82267F68;
loc_82267F54:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x82267f64
  if (cr6.eq) goto loc_82267F64;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82267F64:
  // li r9,8
  r9 = uint64_t(int64_t(8));
loc_82267F68:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r9));
  // b 0x8226802c
  goto loc_8226802C;
loc_82267F7C:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x82267f8c
  if (cr6.eq) goto loc_82267F8C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82267F8C:
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,13
  r8 = uint64_t(int64_t(13));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // b 0x8226802c
  goto loc_8226802C;
loc_82267FA8:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x82267fb8
  if (cr6.eq) goto loc_82267FB8;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82267FB8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // li r8,13
  r8 = uint64_t(int64_t(13));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r31,9648(r30)
  guest::StoreU32(base, uint32_t(r30) + 9648, uint32_t(r31));
  // b 0x82268094
  goto loc_82268094;
loc_82267FDC:
  // beq 0x8226808c
  if (cr0.eq) goto loc_8226808C;
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // ble cr6,0x82268094
  if (!cr6.gt) goto loc_82268094;
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // ble cr6,0x82268064
  if (!cr6.gt) goto loc_82268064;
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226804c
  if (cr6.eq) goto loc_8226804C;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82268034
  if (cr6.eq) goto loc_82268034;
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // bne cr6,0x82268094
  if (!cr6.eq) goto loc_82268094;
loc_82268008:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82268018
  if (cr6.eq) goto loc_82268018;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268018:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r31));
loc_8226802C:
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // b 0x82268094
  goto loc_82268094;
loc_82268034:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x82268044
  if (cr6.eq) goto loc_82268044;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268044:
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // b 0x82268078
  goto loc_82268078;
loc_8226804C:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // beq cr6,0x8226805c
  if (cr6.eq) goto loc_8226805C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226805C:
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // b 0x82268078
  goto loc_82268078;
loc_82268064:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x82268074
  if (cr6.eq) goto loc_82268074;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268074:
  // li r9,10
  r9 = uint64_t(int64_t(10));
loc_82268078:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // b 0x82268094
  goto loc_82268094;
loc_8226808C:
  // mr r3,r30
  r3 = r30;
  // bl 0x822679a0
  // (left out: dead in C++)
  r3 = f822679A0(base, r1, r13);
loc_82268094:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r31,9640(r30)
  guest::StoreU32(base, uint32_t(r30) + 9640, uint32_t(r31));
  // stw r11,9652(r30)
  guest::StoreU32(base, uint32_t(r30) + 9652, uint32_t(r11));
  // b 0x822681b4
  goto loc_822681B4;
loc_822680A4:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24916);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822681a4
  if (cr0.eq) goto loc_822681A4;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x82268164
  if (cr6.eq) goto loc_82268164;
  // addi r11,r11,-10
  r11 = uint64_t(int64_t(int64_t(r11) + -10));
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bgt cr6,0x822681b4
  if (cr6.gt) goto loc_822681B4;
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r31,9640(r30)
  guest::StoreU32(base, uint32_t(r30) + 9640, uint32_t(r31));
  // stw r11,9652(r30)
  guest::StoreU32(base, uint32_t(r30) + 9652, uint32_t(r11));
  // lwz r9,24916(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 24916);
  // lwz r10,0(r9)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r9)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x822680f0
  if (cr6.eq) goto loc_822680F0;
  // stw r10,8(r9)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r9)).previous_page_address(), uint32_t(r10));
loc_822680F0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r31,4(r9)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r9)).next_page_address(), uint32_t(r31));
  // li r8,9
  r8 = uint64_t(int64_t(9));
  // stw r10,16(r9)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r9)).queued_step_address(), uint32_t(r10));
  // stw r10,0(r9)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r9)).page_address(), uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r8,12(r9)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r9)).step_address(), uint32_t(r8));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // lwz r10,24848(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24848);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24920(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24920);
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // stw r31,20(r10)
  guest::StoreU32(base, uint32_t(r10) + 20, uint32_t(r31));
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
  // stw r11,40(r10)
  guest::StoreU32(base, uint32_t(r10) + 40, uint32_t(r11));
  // stw r11,48(r10)
  guest::StoreU32(base, uint32_t(r10) + 48, uint32_t(r11));
  // stw r11,52(r10)
  guest::StoreU32(base, uint32_t(r10) + 52, uint32_t(r11));
  // stw r11,36(r10)
  guest::StoreU32(base, uint32_t(r10) + 36, uint32_t(r11));
  // stw r11,64(r10)
  guest::StoreU32(base, uint32_t(r10) + 64, uint32_t(r11));
  // stw r11,24(r10)
  guest::StoreU32(base, uint32_t(r10) + 24, uint32_t(r11));
  // stw r11,28(r10)
  guest::StoreU32(base, uint32_t(r10) + 28, uint32_t(r11));
  // stw r11,32(r10)
  guest::StoreU32(base, uint32_t(r10) + 32, uint32_t(r11));
  // beq cr6,0x8226815c
  if (cr6.eq) goto loc_8226815C;
  // li r9,3
  r9 = uint64_t(int64_t(3));
loc_8226815C:
  // stw r9,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r9));
  // b 0x822681b4
  goto loc_822681B4;
loc_82268164:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // stw r11,9652(r30)
  guest::StoreU32(base, uint32_t(r30) + 9652, uint32_t(r11));
  // stw r31,9640(r30)
  guest::StoreU32(base, uint32_t(r30) + 9640, uint32_t(r31));
  // lwz r11,24916(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82268188
  if (cr6.eq) goto loc_82268188;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82268188:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // b 0x822681b4
  goto loc_822681B4;
loc_822681A4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,9640(r30)
  guest::StoreU32(base, uint32_t(r30) + 9640, uint32_t(r11));
  // stw r10,9652(r30)
  guest::StoreU32(base, uint32_t(r30) + 9652, uint32_t(r10));
loc_822681B4:
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

// sub_822681D0: lifted from generated/ by lift.py.
lifted::Result f822681D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822681D0 (lifted)");
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r3,24920(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x822681F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,24920(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82268240
  if (cr6.eq) goto loc_82268240;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82268208, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82268220
  if (cr0.eq) goto loc_82268220;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268220:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // b 0x82268404
  goto loc_82268404;
loc_82268240:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82268248, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82268390
  if (cr0.eq) goto loc_82268390;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82268258, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,24848(r9)
  guest::StoreU32(base, uint32_t(r9) + 24848, uint32_t(r10));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82268310
  if (cr6.lt) goto loc_82268310;
  // beq cr6,0x822682ec
  if (cr6.eq) goto loc_822682EC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82268404
  if (!cr6.lt) goto loc_82268404;
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // mr r10,r31
  r10 = r31;
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // mr r8,r31
  r8 = r31;
  // mr r7,r31
  r7 = r31;
  // mr r3,r30
  r3 = r30;
  // stw r10,24996(r9)
  guest::StoreU32(base, uint32_t(r9) + 24996, uint32_t(r10));
  // li r9,19
  r9 = uint64_t(int64_t(19));
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
  // bl 0x822678d0
  // (left out: dead in C++)
  r3 = f822678D0(base, r1, r13);
  // stw r31,9640(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9640_address(), uint32_t(r31));
  // b 0x82268404
  goto loc_82268404;
loc_822682EC:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82268304
  if (cr6.eq) goto loc_82268304;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268304:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // b 0x82268334
  goto loc_82268334;
loc_82268310:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82268328
  if (cr6.eq) goto loc_82268328;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268328:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // li r9,4
  r9 = uint64_t(int64_t(4));
loc_82268334:
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r8,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r8));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r10));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
  // stw r31,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r31));
  // b 0x82268404
  goto loc_82268404;
loc_82268390:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24920(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8226839C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82268404
  if (cr0.eq) goto loc_82268404;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822683AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82268404
  if (cr6.eq) goto loc_82268404;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822683d4
  if (cr0.eq) goto loc_822683D4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822683D4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // lwz r11,24920(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24920);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_82268404:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82268720: lifted from generated/ by lift.py.
lifted::Result f82268720(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82268720 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,9624(r3)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r3)).back_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r3)).unknown_9640_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24916(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24916);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // bne cr6,0x82268860
  if (!cr6.eq) goto loc_82268860;
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bgt cr6,0x8226878c
  if (cr6.gt) goto loc_8226878C;
  // beq cr6,0x822687fc
  if (cr6.eq) goto loc_822687FC;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82268788
  if (cr6.lt) goto loc_82268788;
  // beq cr6,0x82268784
  if (cr6.eq) goto loc_82268784;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // beq cr6,0x82268770
  if (cr6.eq) goto loc_82268770;
  // cmplwi cr6,r11,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 9, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x822662f0
  { const lifted::Result o = f822662F0(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82268770:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,24896(r9)
  guest::StoreU32(base, uint32_t(r9) + 24896, uint32_t(r10));
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // b 0x8226880c
  goto loc_8226880C;
loc_82268784:
  // b 0x82266028
  { const lifted::Result o = f82266028(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82268788:
  // b 0x822681d0
  { const lifted::Result o = f822681D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_8226878C:
  // cmpwi cr6,r11,11
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 11, so);
  // beq cr6,0x822687fc
  if (cr6.eq) goto loc_822687FC;
  // cmpwi cr6,r11,12
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 12, so);
  // beq cr6,0x822687f4
  if (cr6.eq) goto loc_822687F4;
  // cmpwi cr6,r11,13
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 13, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,26
  r9 = uint64_t(int64_t(26));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // blr
  return {r3, f1};
loc_822687F4:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // b 0x82268800
  goto loc_82268800;
loc_822687FC:
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_82268800:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // stw r10,25008(r9)
  guest::StoreU32(base, uint32_t(r9) + 25008, uint32_t(r10));
  // li r9,18
  r9 = uint64_t(int64_t(18));
loc_8226880C:
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9640(r3)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r3)).unknown_9640_address(), uint32_t(r11));
  // blr
  return {r3, f1};
loc_82268860:
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x822688a4
  if (cr6.eq) goto loc_822688A4;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x822688a0
  if (cr6.eq) goto loc_822688A0;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x8226889c
  if (cr6.eq) goto loc_8226889C;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x82268898
  if (cr6.eq) goto loc_82268898;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x82268894
  if (cr6.eq) goto loc_82268894;
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x82265930
  { const lifted::Result o = f82265930(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82268894:
  // b 0x822659f0
  { const lifted::Result o = f822659F0(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82268898:
  // b 0x82266750
  { const lifted::Result o = f82266750(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_8226889C:
  // b 0x82266658
  r3 = f82266658(base, r1, r13, r3, r4);
  return {r3, f1};
loc_822688A0:
  // b 0x82266548
  { const lifted::Result o = f82266548(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_822688A4:
  // b 0x822663e0
  { const lifted::Result o = f822663E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822688B8: lifted from generated/ by lift.py.
void f822688B8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822688B8 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9652(r3)
  guest::StoreU32(base, uint32_t(r3) + 9652, uint32_t(r11));
  // blr
  return;
}

// sub_822688C8: lifted from generated/ by lift.py.
void f822688C8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822688C8 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9652(r3)
  guest::StoreU32(base, uint32_t(r3) + 9652, uint32_t(r11));
  // blr
  return;
}

// sub_822688D8: lifted from generated/ by lift.py.
void f822688D8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822688D8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // blt cr6,0x82268918
  if (cr6.lt) goto loc_82268918;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x822688fc
  if (cr6.eq) goto loc_822688FC;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822688FC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,28
  r9 = uint64_t(int64_t(28));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
loc_82268918:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9644(r3)
  guest::StoreU32(base, uint32_t(r3) + 9644, uint32_t(r11));
  // blr
  return;
}

// sub_82268928: lifted from generated/ by lift.py.
lifted::Result f82268928(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_82268928 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r28,-21596(r11)
  r28 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82268948, r31)); r3 = o.r3; f1 = o.f1; }
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r10,r27
  r10 = r27;
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,30752
  r30 = uint64_t(int64_t(int64_t(r11) + 30752));
  // mr r11,r27
  r11 = r27;
  // stw r11,88(r30)
  guest::StoreU32(base, uint32_t(r30) + 88, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // stw r11,28180(r29)
  guest::StoreU32(base, uint32_t(r29) + 28180, uint32_t(r11));
  // lwz r11,9688(r31)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9688_address());
  // stw r11,29308(r10)
  guest::StoreU32(base, uint32_t(r10) + 29308, uint32_t(r11));
  // lwz r3,9692(r31)
  r3 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9692_address());
  // bl 0x82260170
  // (left out: dead in C++)
  r3 = f82260170(base, r3);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r3,29312(r11)
  guest::StoreU32(base, uint32_t(r11) + 29312, uint32_t(r3));
  // mr r11,r27
  r11 = r27;
  // stw r11,29316(r10)
  guest::StoreU32(base, uint32_t(r10) + 29316, uint32_t(r11));
  // lwz r11,28180(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 28180);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // lwz r11,80(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 80);
  // bne cr6,0x822689bc
  if (!cr6.eq) goto loc_822689BC;
  // rlwinm r11,r11,0,28,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFEF;
  // b 0x822689c0
  goto loc_822689C0;
loc_822689BC:
  // ori r11,r11,16
  r11 = r11 | 16;
loc_822689C0:
  // stw r11,80(r30)
  guest::StoreU32(base, uint32_t(r30) + 80, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwa r3,24868(r11)
  r3 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r11) + 24868))));
  // std r3,29376(r10)
  guest::StoreU64(base, uint32_t(r10) + 29376, r3);
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = game::__ultod(base, LIFTED_ARGS(0x822689D8, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // lfs f13,4208(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4208);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f12,f0,f13
  f12 = double(float(f0 * f13));
  // lfs f13,10708(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10708);
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // fctiwz f13,f12
  f13 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f12) ? int64_t(0x80000000U) : (f12 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f12)))));
  // stfiwx f13,0,r10
  guest::StoreU32(base, uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f13)));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwa r11,80(r1)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r1) + 80))));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfiwx f0,0,r9
  guest::StoreU32(base, uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f0)));
  // std r11,29384(r10)
  guest::StoreU64(base, uint32_t(r10) + 29384, r11);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwa r11,80(r1)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r1) + 80))));
  // std r11,29392(r10)
  guest::StoreU64(base, uint32_t(r10) + 29392, r11);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,9684(r31)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9684_address());
  // stw r28,84(r30)
  guest::StoreU32(base, uint32_t(r30) + 84, uint32_t(r28));
  // stw r11,29324(r10)
  guest::StoreU32(base, uint32_t(r10) + 29324, uint32_t(r11));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x82268a54
  if (cr6.eq) goto loc_82268A54;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268A54:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r27,9644(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9644_address(), uint32_t(r27));
  // stw r27,9648(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9648_address(), uint32_t(r27));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82268A78: lifted from generated/ by lift.py.
lifted::Result f82268A78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82268A78 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r23,-21596(r11)
  r23 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82268A98, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r24,r11,30752
  r24 = uint64_t(int64_t(int64_t(r11) + 30752));
  // mr r11,r27
  r11 = r27;
  // mr r28,r3
  r28 = r3;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,88(r24)
  guest::StoreU32(base, uint32_t(r24) + 88, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82268ad8
  if (!cr6.eq) goto loc_82268AD8;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,28180(r10)
  guest::StoreU32(base, uint32_t(r10) + 28180, uint32_t(r11));
  // lwz r11,80(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 80);
  // rlwinm r11,r11,0,28,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFEF;
  // b 0x82268ae8
  goto loc_82268AE8;
loc_82268AD8:
  // mr r11,r27
  r11 = r27;
  // stw r11,28180(r10)
  guest::StoreU32(base, uint32_t(r10) + 28180, uint32_t(r11));
  // lwz r11,80(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 80);
  // ori r11,r11,16
  r11 = r11 | 16;
loc_82268AE8:
  // stw r11,80(r24)
  guest::StoreU32(base, uint32_t(r24) + 80, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,9688(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address());
  // stw r11,29308(r10)
  guest::StoreU32(base, uint32_t(r10) + 29308, uint32_t(r11));
  // lwz r3,9692(r30)
  r3 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address());
  // bl 0x82260170
  // (left out: dead in C++)
  r3 = f82260170(base, r3);
  // lis r26,-32127
  r26 = uint64_t(int64_t(-2105475072));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // addi r31,r10,30320
  r31 = uint64_t(int64_t(int64_t(r10) + 30320));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r29,r1,80
  r29 = uint64_t(int64_t(int64_t(r1) + 80));
  // lis r25,-32148
  r25 = uint64_t(int64_t(-2106851328));
  // rlwinm r11,r28,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // addi r22,r31,332
  r22 = uint64_t(int64_t(int64_t(r31) + 332));
  // stw r3,29312(r10)
  guest::StoreU32(base, uint32_t(r10) + 29312, uint32_t(r3));
  // mr r10,r27
  r10 = r27;
  // std r27,0(r29)
  guest::StoreU64(base, uint32_t(r29) + 0, r27);
  // addi r21,r31,348
  r21 = uint64_t(int64_t(int64_t(r31) + 348));
  // std r27,8(r29)
  guest::StoreU64(base, uint32_t(r29) + 8, r27);
  // addi r20,r31,364
  r20 = uint64_t(int64_t(int64_t(r31) + 364));
  // addi r4,r31,14
  r4 = uint64_t(int64_t(int64_t(r31) + 14));
  // lwzx r29,r11,r22
  r29 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r22));
  // rlwinm r5,r28,5,0,26
  r5 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 5) & 0xFFFFFFE0;
  // stw r10,29316(r9)
  guest::StoreU32(base, uint32_t(r9) + 29316, uint32_t(r10));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lwz r10,9696(r30)
  r10 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9696_address());
  // add r5,r5,r4
  r5 = r5 + r4;
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwa r26,24868(r26)
  r26 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r26) + 24868))));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,29416(r9)
  guest::StoreU32(base, uint32_t(r9) + 29416, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // std r26,29376(r25)
  guest::StoreU64(base, uint32_t(r25) + 29376, r26);
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // std r29,29344(r26)
  guest::StoreU64(base, uint32_t(r26) + 29344, r29);
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // lwzx r29,r11,r21
  r29 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r21));
  // lwzx r11,r11,r20
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r20));
  // std r29,29328(r26)
  guest::StoreU64(base, uint32_t(r26) + 29328, r29);
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // std r11,29336(r29)
  guest::StoreU64(base, uint32_t(r29) + 29336, r11);
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = game::WideCharToMultiByte(base, LIFTED_ARGS(0x82268BA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,80(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 80);
  // lbz r10,81(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 81);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // lbz r9,88(r1)
  r9 = guest::LoadU8(base, uint32_t(r1) + 88);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // rldicr r11,r11,8,55
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // add r11,r11,r10
  r11 = r11 + r10;
  // lbz r10,89(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 89);
  // rldicr r9,r9,8,55
  r9 = __builtin_rotateleft64(r9, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r8,r10
  r8 = uint64_t(int64_t(int8_t(r10)));
  // lbz r10,82(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 82);
  // rldicr r11,r11,8,55
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // add r9,r9,r8
  r9 = r9 + r8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lbz r10,90(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 90);
  // rldicr r9,r9,8,55
  r9 = __builtin_rotateleft64(r9, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r8,r10
  r8 = uint64_t(int64_t(int8_t(r10)));
  // lbz r10,83(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 83);
  // rldicr r11,r11,8,55
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // add r9,r9,r8
  r9 = r9 + r8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lbz r10,91(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 91);
  // rldicr r9,r9,8,55
  r9 = __builtin_rotateleft64(r9, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r8,r10
  r8 = uint64_t(int64_t(int8_t(r10)));
  // lbz r10,84(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 84);
  // rldicr r11,r11,8,55
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // add r9,r9,r8
  r9 = r9 + r8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // rldicr r9,r9,8,55
  r9 = __builtin_rotateleft64(r9, 8) & 0xFFFFFFFFFFFFFF00;
  // lbz r10,92(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 92);
  // rldicr r11,r11,8,55
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFF00;
  // addi r7,r31,144
  r7 = uint64_t(int64_t(int64_t(r31) + 144));
  // extsb r8,r10
  r8 = uint64_t(int64_t(int8_t(r10)));
  // lbz r10,85(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 85);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // add r9,r9,r8
  r9 = r9 + r8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lbz r10,93(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 93);
  // rldicr r9,r9,8,55
  r9 = __builtin_rotateleft64(r9, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r8,r10
  r8 = uint64_t(int64_t(int8_t(r10)));
  // lbz r10,86(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 86);
  // rldicr r11,r11,8,55
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // add r9,r9,r8
  r9 = r9 + r8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lbz r10,94(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 94);
  // rldicr r9,r9,8,55
  r9 = __builtin_rotateleft64(r9, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r8,r10
  r8 = uint64_t(int64_t(int8_t(r10)));
  // lbz r10,87(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 87);
  // rldicr r11,r11,8,55
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFF00;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // add r8,r9,r8
  r8 = r9 + r8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lbz r10,95(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 95);
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // std r11,29352(r9)
  guest::StoreU64(base, uint32_t(r9) + 29352, r11);
  // rldicr r11,r8,8,55
  r11 = __builtin_rotateleft64(r8, 8) & 0xFFFFFFFFFFFFFF00;
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r8,r28,3,0,28
  r8 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 3) & 0xFFFFFFF8;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // std r11,29360(r9)
  guest::StoreU64(base, uint32_t(r9) + 29360, r11);
  // ldx r11,r8,r7
  r11 = guest::LoadU64(base, uint32_t(r8) + uint32_t(r7));
  // std r11,29368(r10)
  guest::StoreU64(base, uint32_t(r10) + 29368, r11);
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x82268CBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r23,84(r24)
  guest::StoreU32(base, uint32_t(r24) + 84, uint32_t(r23));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r3,29420(r11)
  guest::StoreU32(base, uint32_t(r11) + 29420, uint32_t(r3));
  // mr r11,r27
  r11 = r27;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // stb r11,27404(r10)
  guest::StoreU8(base, uint32_t(r10) + 27404, uint8_t(r11));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x82268cf4
  if (cr6.eq) goto loc_82268CF4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268CF4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r27,9644(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9644_address(), uint32_t(r27));
  // stw r27,9648(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9648_address(), uint32_t(r27));
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82268FC8: lifted from generated/ by lift.py.
void f82268FC8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82268FC8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8226900c
  if (cr6.eq) goto loc_8226900C;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82268fec
  if (cr0.eq) goto loc_82268FEC;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82268FEC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r8,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r8));
  // blr
  return;
loc_8226900C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_82269018: lifted from generated/ by lift.py.
lifted::Result f82269018(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82269018 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-64(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -4096);
  // ld r12,-8192(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -8192);
  // ld r12,-12288(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -12288);
  // ld r12,-16384(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -16384);
  // stwu r1,-19536(r1)
  ea = -19536 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x821021a8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceData::ResetGlobal(base, LIFTED_ARGS(0x8226903C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // ori r31,r11,38432
  r31 = r11 | 38432;
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x82101900
  // (left out: dead in C++)
  { const lifted::Result o = game::Stages::Clear(base, LIFTED_ARGS(0x82269054, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // ori r10,r11,15088
  r10 = r11 | 15088;
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // li r7,8
  r7 = uint64_t(int64_t(8));
  // ori r8,r11,15092
  r8 = r11 | 15092;
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // stwx r9,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r9));
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // stwx r7,r11,r8
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r8), uint32_t(r7));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27412(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27412);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x822690d4
  if (cr6.eq) goto loc_822690D4;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x822690cc
  if (cr6.eq) goto loc_822690CC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // beq cr6,0x822690c4
  if (cr6.eq) goto loc_822690C4;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // beq cr6,0x822690bc
  if (cr6.eq) goto loc_822690BC;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // beq cr6,0x822690b4
  if (cr6.eq) goto loc_822690B4;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // b 0x822690d8
  goto loc_822690D8;
loc_822690B4:
  // li r26,5
  r26 = uint64_t(int64_t(5));
  // b 0x822690d8
  goto loc_822690D8;
loc_822690BC:
  // li r26,4
  r26 = uint64_t(int64_t(4));
  // b 0x822690d8
  goto loc_822690D8;
loc_822690C4:
  // li r26,3
  r26 = uint64_t(int64_t(3));
  // b 0x822690d8
  goto loc_822690D8;
loc_822690CC:
  // li r26,2
  r26 = uint64_t(int64_t(2));
  // b 0x822690d8
  goto loc_822690D8;
loc_822690D4:
  // li r26,1
  r26 = uint64_t(int64_t(1));
loc_822690D8:
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r26
  r3 = r26;
  // bl 0x82101d38
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::SetRoute(base, LIFTED_ARGS(0x822690E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x820ff968
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820FF968(base, LIFTED_ARGS(0x822690EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r27,4(r11)
  r27 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r30,r11,63600
  r30 = r11 | 63600;
  // cmpwi r27,0
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // beq 0x82269150
  if (cr0.eq) goto loc_82269150;
  // addi r3,r1,1968
  r3 = uint64_t(int64_t(int64_t(r1) + 1968));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x82269114, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x8226911C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24960);
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // addi r4,r11,832
  r4 = uint64_t(int64_t(int64_t(r11) + 832));
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,1968
  r3 = uint64_t(int64_t(int64_t(r1) + 1968));
  // lwzx r11,r11,r30
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r30));
  // stw r11,5860(r1)
  guest::StoreU32(base, uint32_t(r1) + 5860, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8226913C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,1968
  r3 = uint64_t(int64_t(int64_t(r1) + 1968));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x82269144, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r1,1968
  r3 = uint64_t(int64_t(int64_t(r1) + 1968));
  // b 0x82269190
  goto loc_82269190;
loc_82269150:
  // addi r3,r1,13680
  r3 = uint64_t(int64_t(int64_t(r1) + 13680));
  // bl 0x82260468
  // (left out: dead in C++)
  r3 = f82260468(base, r1, r13, r3);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82269168, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,9776
  r3 = uint64_t(int64_t(int64_t(r1) + 9776));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x82269170, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,9776
  r3 = uint64_t(int64_t(int64_t(r1) + 9776));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82269180, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,9776
  r3 = uint64_t(int64_t(int64_t(r1) + 9776));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x82269188, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r3,r1,9776
  r3 = uint64_t(int64_t(int64_t(r1) + 9776));
loc_82269190:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r11,r11,2612
  r11 = uint64_t(int64_t(int64_t(r11) + 2612));
  // lfs f31,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f31 = double(temp.f32);
  // fmr f1,f31
  f1 = f31;
  // bl 0x821017c0
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::AddRacer(base, LIFTED_ARGS(0x822691A8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // beq cr6,0x822691f4
  if (cr6.eq) goto loc_822691F4;
  // addi r3,r1,13680
  r3 = uint64_t(int64_t(int64_t(r1) + 13680));
  // bl 0x82260468
  // (left out: dead in C++)
  r3 = f82260468(base, r1, r13, r3);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822691C8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,15568
  r3 = uint64_t(int64_t(int64_t(r1) + 15568));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x822691D0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,15568
  r3 = uint64_t(int64_t(int64_t(r1) + 15568));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822691E0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,15568
  r3 = uint64_t(int64_t(int64_t(r1) + 15568));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x822691E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r3,r1,15568
  r3 = uint64_t(int64_t(int64_t(r1) + 15568));
  // b 0x82269234
  goto loc_82269234;
loc_822691F4:
  // addi r3,r1,5872
  r3 = uint64_t(int64_t(int64_t(r1) + 5872));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x822691FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x82269204, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24960);
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // addi r4,r11,832
  r4 = uint64_t(int64_t(int64_t(r11) + 832));
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,5872
  r3 = uint64_t(int64_t(int64_t(r1) + 5872));
  // lwzx r11,r11,r30
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r30));
  // stw r11,9764(r1)
  guest::StoreU32(base, uint32_t(r1) + 9764, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82269224, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,5872
  r3 = uint64_t(int64_t(int64_t(r1) + 5872));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x8226922C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r1,5872
  r3 = uint64_t(int64_t(int64_t(r1) + 5872));
loc_82269234:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x821017c0
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::AddRacer(base, LIFTED_ARGS(0x82269240, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x820ff918
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820FF918(base, LIFTED_ARGS(0x82269248, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x820ffab0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820FFAB0(base, LIFTED_ARGS(0x82269258, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,29312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29312);
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // addic. r30,r11,3
  ca = uint32_t(r11) > 4294967292;
  r30 = uint64_t(int64_t(int64_t(r11) + 3));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // ble 0x822692c4
  if (!cr0.gt) goto loc_822692C4;
loc_8226926C:
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x820ffab8
  // (left out: dead in C++)
  { const lifted::Result o = game::Stages::AddRacer(base, LIFTED_ARGS(0x82269284, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x820ffab8
  // (left out: dead in C++)
  { const lifted::Result o = game::Stages::AddRacer(base, LIFTED_ARGS(0x8226929C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // mr r4,r26
  r4 = r26;
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x82101e90
  // (left out: dead in C++)
  { const lifted::Result o = game::Stages::SetRoute(base, LIFTED_ARGS(0x822692B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82101828
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::AddStage(base, LIFTED_ARGS(0x822692BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne 0x8226926c
  if (!cr0.eq) goto loc_8226926C;
loc_822692C4:
  // bl 0x82102048
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::PlaceRacers(base, LIFTED_ARGS(0x822692C8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,19536
  r1 = uint64_t(int64_t(int64_t(r1) + 19536));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822692D8: lifted from generated/ by lift.py.
lifted::Result f822692D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822692D8 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // mr r30,r3
  r30 = r3;
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // add r31,r10,r11
  r31 = r10 + r11;
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x8226930C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // li r27,-1
  r27 = uint64_t(int64_t(-1));
  // li r26,2
  r26 = uint64_t(int64_t(2));
  // li r25,6
  r25 = uint64_t(int64_t(6));
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82269418
  if (!cr6.eq) goto loc_82269418;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269358
  if (cr6.eq) goto loc_82269358;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269350, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82269384
  if (!cr0.eq) goto loc_82269384;
loc_82269358:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269418
  if (cr6.eq) goto loc_82269418;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226937C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82269418
  if (cr0.eq) goto loc_82269418;
loc_82269384:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226938C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822693e0
  if (cr6.lt) goto loc_822693E0;
  // beq cr6,0x822693c4
  if (cr6.eq) goto loc_822693C4;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x822693f8
  if (!cr6.lt) goto loc_822693F8;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9700(r30)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9700_address());
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9700(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9700_address(), uint32_t(r3));
  // b 0x822693f8
  goto loc_822693F8;
loc_822693C4:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9692(r30)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address());
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9692(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address(), uint32_t(r3));
  // b 0x822693f8
  goto loc_822693F8;
loc_822693E0:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9688(r30)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address());
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9688(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address(), uint32_t(r3));
loc_822693F8:
  // lwz r11,24928(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x8226940c
  if (cr6.eq) goto loc_8226940C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226940C:
  // li r10,8
  r10 = uint64_t(int64_t(8));
loc_82269410:
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // b 0x822696bc
  goto loc_822696BC;
loc_82269418:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82269510
  if (!cr6.eq) goto loc_82269510;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269454
  if (cr6.eq) goto loc_82269454;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226944C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82269480
  if (!cr0.eq) goto loc_82269480;
loc_82269454:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269510
  if (cr6.eq) goto loc_82269510;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269478, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82269510
  if (cr0.eq) goto loc_82269510;
loc_82269480:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82269488, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822694dc
  if (cr6.lt) goto loc_822694DC;
  // beq cr6,0x822694c0
  if (cr6.eq) goto loc_822694C0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x822694f4
  if (!cr6.lt) goto loc_822694F4;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9700(r30)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9700_address());
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9700(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9700_address(), uint32_t(r3));
  // b 0x822694f4
  goto loc_822694F4;
loc_822694C0:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9692(r30)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address());
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9692(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address(), uint32_t(r3));
  // b 0x822694f4
  goto loc_822694F4;
loc_822694DC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9688(r30)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address());
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9688(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address(), uint32_t(r3));
loc_822694F4:
  // lwz r11,24928(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82269508
  if (cr6.eq) goto loc_82269508;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82269508:
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // b 0x82269410
  goto loc_82269410;
loc_82269510:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8226951C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82269654
  if (cr0.eq) goto loc_82269654;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226952C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8226964c
  if (!cr6.eq) goto loc_8226964C;
  // lwz r11,9700(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9700_address());
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82269598
  if (!cr6.eq) goto loc_82269598;
  // lwz r11,9688(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address());
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,29308(r10)
  guest::StoreU32(base, uint32_t(r10) + 29308, uint32_t(r11));
  // lwz r3,9692(r30)
  r3 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address());
  // bl 0x82260170
  // (left out: dead in C++)
  r3 = f82260170(base, r3);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r3,29312(r11)
  guest::StoreU32(base, uint32_t(r11) + 29312, uint32_t(r3));
  // mr r11,r31
  r11 = r31;
  // stw r11,29316(r10)
  guest::StoreU32(base, uint32_t(r10) + 29316, uint32_t(r11));
  // bl 0x82148b60
  // (left out: dead in C++)
  { const lifted::Result o = f82148B60(base, r1, r13, r8); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r4,80(r3)
  r4 = guest::LoadU32(base, uint32_t(r3) + 80);
  // bl 0x8214e778
  // (left out: dead in C++)
  { const lifted::Result o = f8214E778(base, r1, r13, r3, r4, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
loc_82269598:
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r31,9668(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9668_address(), uint32_t(r31));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r8,210
  r8 = uint64_t(int64_t(210));
  // li r7,221
  r7 = uint64_t(int64_t(221));
  // stw r11,9656(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9656_address(), uint32_t(r11));
  // li r11,220
  r11 = uint64_t(int64_t(220));
  // stw r9,9660(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9660_address(), uint32_t(r9));
  // stw r10,9664(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9664_address(), uint32_t(r10));
  // stw r8,9672(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9672_address(), uint32_t(r8));
  // stw r7,9676(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9676_address(), uint32_t(r7));
  // stw r11,9680(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9680_address(), uint32_t(r11));
  // lwz r11,24928(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24928);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // beq cr6,0x822695e0
  if (cr6.eq) goto loc_822695E0;
  // stw r9,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_822695E0:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // stw r27,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r27));
  // li r8,27
  r8 = uint64_t(int64_t(27));
  // li r7,17
  r7 = uint64_t(int64_t(17));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r9));
  // stw r8,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r8));
  // stw r7,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r7));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r10));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r10,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r9));
  // b 0x822696c8
  goto loc_822696C8;
loc_8226964C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x82269690
  goto loc_82269690;
loc_82269654:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82269660, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822696c8
  if (cr0.eq) goto loc_822696C8;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82269670, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8226968c
  if (!cr6.eq) goto loc_8226968C;
  // lwz r11,9700(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9700_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822696c8
  if (cr6.eq) goto loc_822696C8;
loc_8226968C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_82269690:
  // stw r11,9700(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9700_address(), uint32_t(r11));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // stw r26,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r26));
  // lwz r11,24928(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x822696b8
  if (cr6.eq) goto loc_822696B8;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822696B8:
  // stw r25,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r25));
loc_822696BC:
  // stw r27,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r27));
  // stw r26,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r26));
  // stw r27,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r27));
loc_822696C8:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82269704
  if (cr6.eq) goto loc_82269704;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822696E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x822696f4
  if (cr6.eq) goto loc_822696F4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822696F4:
  // stw r25,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r25));
  // stw r27,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r27));
  // stw r26,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r26));
  // stw r27,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r27));
loc_82269704:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82269710: lifted from generated/ by lift.py.
lifted::Result f82269710(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82269710 (lifted)");
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
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82269734, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x8226973C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82269750: lifted from generated/ by lift.py.
void f82269750(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82269750 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,9720(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9720);
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // bne cr6,0x82269780
  if (!cr6.eq) goto loc_82269780;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,24924(r9)
  guest::StoreU32(base, uint32_t(r9) + 24924, uint32_t(r10));
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // b 0x82269794
  goto loc_82269794;
loc_82269780:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // lwz r10,24960(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24960);
  // stw r6,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r6));
loc_82269794:
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
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_82269898: lifted from generated/ by lift.py.
uint64_t f82269898(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82269898 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822698C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822698D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822698E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822698F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82269904, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82269918, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8226992C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82269940, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82269954, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,24936(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24936);
  // stw r9,0(r10)
  schema::NetScreen7Data(base, uint32_t(r10)).set_unknown_0_bits(uint32_t(r9));
  // lwz r11,24936(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24936);
  // stw r8,4(r11)
  schema::NetScreen7Data(base, uint32_t(r11)).set_unknown_4_bits(uint32_t(r8));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82269988: lifted from generated/ by lift.py.
lifted::Result f82269988(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82269988 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // mr r27,r3
  r27 = r3;
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -21596);
  // mr r30,r4
  r30 = r4;
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // add r31,r10,r11
  r31 = r10 + r11;
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x822699C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // li r28,3
  r28 = uint64_t(int64_t(3));
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82269a2c
  if (!cr6.eq) goto loc_82269A2C;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).cursor_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82269a2c
  if (!cr6.eq) goto loc_82269A2C;
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).previous_address());
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).cursor_address());
  // stw r11,4(r3)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r3)).previous_address(), uint32_t(r11));
  // bne cr6,0x82269a14
  if (!cr6.eq) goto loc_82269A14;
  // stw r10,0(r3)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r3)).cursor_address(), uint32_t(r10));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r28,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r28));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // b 0x82269a28
  goto loc_82269A28;
loc_82269A14:
  // stw r28,0(r3)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r3)).cursor_address(), uint32_t(r28));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r10,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r9));
loc_82269A28:
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269A2C:
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).moved_address());
  // li r25,-1
  r25 = uint64_t(int64_t(-1));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // li r23,2
  r23 = uint64_t(int64_t(2));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r26,-32127
  r26 = uint64_t(int64_t(-2105475072));
  // bne cr6,0x82269b4c
  if (!cr6.eq) goto loc_82269B4C;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269a78
  if (cr6.eq) goto loc_82269A78;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269A6C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82269aa4
  if (!cr0.eq) goto loc_82269AA4;
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269A78:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269b4c
  if (cr6.eq) goto loc_82269B4C;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269A9C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82269b48
  if (cr0.eq) goto loc_82269B48;
loc_82269AA4:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82269ac0
  if (cr6.eq) goto loc_82269AC0;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82269ABC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269AC0:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82269b10
  if (cr6.lt) goto loc_82269B10;
  // beq cr6,0x82269af4
  if (cr6.eq) goto loc_82269AF4;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82269b28
  if (!cr6.lt) goto loc_82269B28;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9692(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9692_address());
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r27
  r3 = r27;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9692(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9692_address(), uint32_t(r3));
  // b 0x82269b28
  goto loc_82269B28;
loc_82269AF4:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9688(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9688_address());
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r27
  r3 = r27;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9688(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9688_address(), uint32_t(r3));
  // b 0x82269b28
  goto loc_82269B28;
loc_82269B10:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9684(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9684_address());
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r27
  r3 = r27;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9684(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9684_address(), uint32_t(r3));
loc_82269B28:
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82269b3c
  if (cr0.eq) goto loc_82269B3C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82269B3C:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r24,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r24));
  // b 0x82269d80
  goto loc_82269D80;
loc_82269B48:
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269B4C:
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82269c58
  if (!cr6.eq) goto loc_82269C58;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269b88
  if (cr6.eq) goto loc_82269B88;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269B7C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82269bb4
  if (!cr0.eq) goto loc_82269BB4;
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269B88:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269c58
  if (cr6.eq) goto loc_82269C58;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269BAC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82269c54
  if (cr0.eq) goto loc_82269C54;
loc_82269BB4:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82269bd0
  if (cr6.eq) goto loc_82269BD0;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82269BCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269BD0:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82269c20
  if (cr6.lt) goto loc_82269C20;
  // beq cr6,0x82269c04
  if (cr6.eq) goto loc_82269C04;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82269c38
  if (!cr6.lt) goto loc_82269C38;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9692(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9692_address());
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r27
  r3 = r27;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9692(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9692_address(), uint32_t(r3));
  // b 0x82269c38
  goto loc_82269C38;
loc_82269C04:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9688(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9688_address());
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r27
  r3 = r27;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9688(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9688_address(), uint32_t(r3));
  // b 0x82269c38
  goto loc_82269C38;
loc_82269C20:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9684(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9684_address());
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r27
  r3 = r27;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9684(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9684_address(), uint32_t(r3));
loc_82269C38:
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82269c4c
  if (cr0.eq) goto loc_82269C4C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82269C4C:
  // stw r28,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r28));
  // b 0x82269d34
  goto loc_82269D34;
loc_82269C54:
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269C58:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82269C60, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82269d44
  if (cr0.eq) goto loc_82269D44;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82269C70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bne cr6,0x82269d10
  if (!cr6.eq) goto loc_82269D10;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // mr r10,r28
  r10 = r28;
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // mr r8,r24
  r8 = r24;
  // mr r7,r24
  r7 = r24;
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // li r9,16
  r9 = uint64_t(int64_t(16));
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
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x82269cec
  if (cr6.eq) goto loc_82269CEC;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82269CEC:
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // stw r25,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r25));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r24,9644(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9644_address(), uint32_t(r24));
  // stw r24,9648(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9648_address(), uint32_t(r24));
  // bl 0x822603d8
  // (left out: dead in C++)
  { const lifted::Result o = f822603D8(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82269d8c
  goto loc_82269D8C;
loc_82269D10:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82269d30
  if (cr0.eq) goto loc_82269D30;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82269D30:
  // stw r23,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r23));
loc_82269D34:
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r24,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r24));
  // stw r25,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r25));
  // b 0x82269d8c
  goto loc_82269D8C;
loc_82269D44:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82269D50, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82269d8c
  if (cr0.eq) goto loc_82269D8C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82269D60, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x82269d74
  if (cr6.eq) goto loc_82269D74;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82269D74:
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // li r10,28
  r10 = uint64_t(int64_t(28));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r9));
loc_82269D80:
  // stw r25,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r25));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
loc_82269D8C:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82269dc8
  if (cr6.eq) goto loc_82269DC8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82269DA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82269db8
  if (cr0.eq) goto loc_82269DB8;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82269DB8:
  // stw r23,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r23));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r24,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r24));
  // stw r25,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r25));
loc_82269DC8:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82269DD0: lifted from generated/ by lift.py.
lifted::Result f82269DD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82269DD0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // mr r27,r3
  r27 = r3;
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -21596);
  // mr r30,r4
  r30 = r4;
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // add r31,r10,r11
  r31 = r10 + r11;
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x82269E08, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // li r28,3
  r28 = uint64_t(int64_t(3));
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82269e70
  if (!cr6.eq) goto loc_82269E70;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).cursor_address());
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82269e70
  if (!cr6.eq) goto loc_82269E70;
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).moved_address());
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).cursor_address());
  // stw r11,4(r3)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r3)).previous_address(), uint32_t(r11));
  // bne cr6,0x82269e58
  if (!cr6.eq) goto loc_82269E58;
  // stw r28,0(r3)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r3)).cursor_address(), uint32_t(r28));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r24,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r24));
  // b 0x82269e68
  goto loc_82269E68;
loc_82269E58:
  // stw r24,0(r3)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r3)).cursor_address(), uint32_t(r24));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r28,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r28));
loc_82269E68:
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269E70:
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).moved_address());
  // li r25,-1
  r25 = uint64_t(int64_t(-1));
  // li r23,2
  r23 = uint64_t(int64_t(2));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r26,-32127
  r26 = uint64_t(int64_t(-2105475072));
  // bne cr6,0x82269f8c
  if (!cr6.eq) goto loc_82269F8C;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269eb8
  if (cr6.eq) goto loc_82269EB8;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269EAC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82269ee4
  if (!cr0.eq) goto loc_82269EE4;
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269EB8:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269f8c
  if (cr6.eq) goto loc_82269F8C;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269EDC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82269f88
  if (cr0.eq) goto loc_82269F88;
loc_82269EE4:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82269f00
  if (cr6.eq) goto loc_82269F00;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82269EFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269F00:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82269f50
  if (cr6.lt) goto loc_82269F50;
  // beq cr6,0x82269f34
  if (cr6.eq) goto loc_82269F34;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82269f68
  if (!cr6.lt) goto loc_82269F68;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9696(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9696_address());
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r27
  r3 = r27;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9696(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9696_address(), uint32_t(r3));
  // b 0x82269f68
  goto loc_82269F68;
loc_82269F34:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9692(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9692_address());
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r27
  r3 = r27;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9692(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9692_address(), uint32_t(r3));
  // b 0x82269f68
  goto loc_82269F68;
loc_82269F50:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9688(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9688_address());
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r27
  r3 = r27;
  // bl 0x822707a8
  // (left out: dead in C++)
  r3 = f822707A8(base, r4, r5, r6);
  // stw r3,9688(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9688_address(), uint32_t(r3));
loc_82269F68:
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82269f7c
  if (cr6.eq) goto loc_82269F7C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82269F7C:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r24,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r24));
  // b 0x8226a1c0
  goto loc_8226A1C0;
loc_82269F88:
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269F8C:
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r3)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226a098
  if (!cr6.eq) goto loc_8226A098;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82269fc8
  if (cr6.eq) goto loc_82269FC8;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269FBC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82269ff4
  if (!cr0.eq) goto loc_82269FF4;
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_82269FC8:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226a098
  if (cr6.eq) goto loc_8226A098;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82269FEC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8226a094
  if (cr0.eq) goto loc_8226A094;
loc_82269FF4:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x8226a010
  if (cr6.eq) goto loc_8226A010;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226A00C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_8226A010:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8226a060
  if (cr6.lt) goto loc_8226A060;
  // beq cr6,0x8226a044
  if (cr6.eq) goto loc_8226A044;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x8226a078
  if (!cr6.lt) goto loc_8226A078;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9696(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9696_address());
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r27
  r3 = r27;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9696(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9696_address(), uint32_t(r3));
  // b 0x8226a078
  goto loc_8226A078;
loc_8226A044:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9692(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9692_address());
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r27
  r3 = r27;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9692(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9692_address(), uint32_t(r3));
  // b 0x8226a078
  goto loc_8226A078;
loc_8226A060:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r6,9688(r27)
  r6 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9688_address());
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r27
  r3 = r27;
  // bl 0x82270790
  // (left out: dead in C++)
  r3 = f82270790(base, r4, r5, r6);
  // stw r3,9688(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9688_address(), uint32_t(r3));
loc_8226A078:
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8226a08c
  if (cr6.eq) goto loc_8226A08C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226A08C:
  // stw r28,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r28));
  // b 0x8226a174
  goto loc_8226A174;
loc_8226A094:
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
loc_8226A098:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8226A0A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226a184
  if (cr0.eq) goto loc_8226A184;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226A0B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bne cr6,0x8226a150
  if (!cr6.eq) goto loc_8226A150;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // mr r7,r8
  r7 = r8;
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r9,16
  r9 = uint64_t(int64_t(16));
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
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x8226a12c
  if (cr6.eq) goto loc_8226A12C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226A12C:
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // stw r25,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r25));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r8,9644(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9644_address(), uint32_t(r8));
  // stw r8,9648(r27)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r27)).unknown_9648_address(), uint32_t(r8));
  // bl 0x822603d8
  // (left out: dead in C++)
  { const lifted::Result o = f822603D8(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8226a1cc
  goto loc_8226A1CC;
loc_8226A150:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8226a170
  if (cr6.eq) goto loc_8226A170;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226A170:
  // stw r23,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r23));
loc_8226A174:
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r24,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r24));
  // stw r25,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r25));
  // b 0x8226a1cc
  goto loc_8226A1CC;
loc_8226A184:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24932(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8226A190, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226a1cc
  if (cr0.eq) goto loc_8226A1CC;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226A1A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x8226a1b4
  if (cr6.eq) goto loc_8226A1B4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226A1B4:
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // li r10,28
  r10 = uint64_t(int64_t(28));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r9));
loc_8226A1C0:
  // stw r25,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r25));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
loc_8226A1CC:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8226a208
  if (cr6.eq) goto loc_8226A208;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226A1E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8226a1f8
  if (cr6.eq) goto loc_8226A1F8;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226A1F8:
  // stw r23,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r23));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // stw r24,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r24));
  // stw r25,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r25));
loc_8226A208:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226A210: lifted from generated/ by lift.py.
lifted::Result f8226A210(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226A210 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,9704(r29)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9704_address());
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r31,r11,24932
  r31 = uint64_t(int64_t(int64_t(r11) + 24932));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // ble cr6,0x8226a244
  if (!cr6.gt) goto loc_8226A244;
  // bl 0x821ce1d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CE1D0, LIFTED_ARGS(0x8226A240, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226a248
  goto loc_8226A248;
loc_8226A244:
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x8226A248, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226A248:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8226A254, r31)); r3 = o.r3; f1 = o.f1; }
  // li r26,-1
  r26 = uint64_t(int64_t(-1));
  // li r27,3
  r27 = uint64_t(int64_t(3));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // beq 0x8226a31c
  if (cr0.eq) goto loc_8226A31C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226A270, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lis r8,-32127
  r8 = uint64_t(int64_t(-2105475072));
  // addi r10,r30,204
  r10 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r9,24896(r8)
  guest::StoreU32(base, uint32_t(r8) + 24896, uint32_t(r9));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // lwz r9,48(r11)
  r9 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address());
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r7,r11
  r7 = r11;
  // stw r9,9640(r29)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9640_address(), uint32_t(r9));
  // lwz r9,600(r10)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r10) - 204).ring_count_address());
  // lwz r6,604(r10)
  r6 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r10) - 204).ring_start_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r6,r6,r9
  r6 = r6 + r9;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r9,600(r10)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r10) - 204).ring_count_address(), uint32_t(r9));
  // divwu r5,r6,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r6) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r9,r5,r6
  r9 = r6 - r5;
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r10,r9,r10
  r10 = r9 + r10;
  // stw r8,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r8));
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // stw r7,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r7));
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r11,12(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).first_item_address(), uint32_t(r11));
  // stw r11,16(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).last_item_address(), uint32_t(r11));
  // stw r4,20(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).wraps_address(), uint32_t(r4));
  // stw r11,0(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).cursor_address(), uint32_t(r11));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).previous_address(), uint32_t(r11));
  // stw r11,8(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).moved_address(), uint32_t(r11));
  // stw r11,40(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).wrapped_address(), uint32_t(r11));
  // stw r11,48(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).visible_row_address(), uint32_t(r11));
  // stw r11,52(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).previous_row_address(), uint32_t(r11));
  // stw r11,36(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).side_address(), uint32_t(r11));
  // stw r11,64(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).player_address(), uint32_t(r11));
  // stw r11,24(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).up_down_locked_address(), uint32_t(r11));
  // stw r11,28(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).left_right_steps_address(), uint32_t(r11));
  // stw r11,32(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).shoulders_step_address(), uint32_t(r11));
  // stw r11,9644(r29)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9644_address(), uint32_t(r11));
  // b 0x8226a4b8
  goto loc_8226A4B8;
loc_8226A31C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8226A328, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226a3dc
  if (cr0.eq) goto loc_8226A3DC;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226A338, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,24928(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 24928);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8226a34c
  if (cr0.eq) goto loc_8226A34C;
  // stw r11,8(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).previous_page_address(), uint32_t(r11));
loc_8226A34C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r26,0(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).page_address(), uint32_t(r26));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).next_page_address(), uint32_t(r11));
  // stw r9,16(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).queued_step_address(), uint32_t(r9));
  // stw r8,12(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).step_address(), uint32_t(r8));
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r11,12(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).first_item_address(), uint32_t(r11));
  // stw r27,16(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).last_item_address(), uint32_t(r27));
  // stw r9,20(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).wraps_address(), uint32_t(r9));
  // stw r11,0(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).cursor_address(), uint32_t(r11));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).previous_address(), uint32_t(r11));
  // stw r11,8(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).moved_address(), uint32_t(r11));
  // stw r11,40(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).wrapped_address(), uint32_t(r11));
  // stw r11,48(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).visible_row_address(), uint32_t(r11));
  // stw r11,52(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).previous_row_address(), uint32_t(r11));
  // stw r11,36(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).side_address(), uint32_t(r11));
  // stw r11,64(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).player_address(), uint32_t(r11));
  // stw r11,24(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).up_down_locked_address(), uint32_t(r11));
  // stw r11,28(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).left_right_steps_address(), uint32_t(r11));
  // stw r11,32(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).shoulders_step_address(), uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24848(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24848);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226a3c8
  if (!cr6.eq) goto loc_8226A3C8;
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r10)).cursor_address());
  // stw r11,0(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).cursor_address(), uint32_t(r11));
  // stw r9,4(r10)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r10)).previous_address(), uint32_t(r9));
  // b 0x8226a4b8
  goto loc_8226A4B8;
loc_8226A3C8:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r9,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r9));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // b 0x8226a4b8
  goto loc_8226A4B8;
loc_8226A3DC:
  // li r4,157
  r4 = uint64_t(int64_t(157));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8226A3E8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226a488
  if (cr0.eq) goto loc_8226A488;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // mr r11,r27
  r11 = r27;
  // addi r10,r30,204
  r10 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // stw r11,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,600(r10)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r10) - 204).ring_count_address());
  // lwz r6,604(r10)
  r6 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r10) - 204).ring_start_address());
  // mr r7,r11
  r7 = r11;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r6,r6,r9
  r6 = r6 + r9;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r9,600(r10)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r10) - 204).ring_count_address(), uint32_t(r9));
  // divwu r5,r6,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r6) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r9,r5,r6
  r9 = r6 - r5;
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r10,r9,r10
  r10 = r9 + r10;
  // stw r8,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r8));
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // stw r7,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r7));
  // lwz r10,24928(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 24928);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // beq cr6,0x8226a45c
  if (cr6.eq) goto loc_8226A45C;
  // stw r9,8(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).previous_page_address(), uint32_t(r9));
loc_8226A45C:
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // stw r26,12(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).step_address(), uint32_t(r26));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r26,16(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).queued_step_address(), uint32_t(r26));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).page_address(), uint32_t(r9));
  // stw r11,9644(r29)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9644_address(), uint32_t(r11));
  // stw r11,9648(r29)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9648_address(), uint32_t(r11));
  // stw r8,9712(r29)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r29)).unknown_9712_address(), uint32_t(r8));
  // bl 0x822603d8
  // (left out: dead in C++)
  { const lifted::Result o = f822603D8(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8226a4b8
  goto loc_8226A4B8;
loc_8226A488:
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8226A494, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226a4b8
  if (cr0.eq) goto loc_8226A4B8;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8225fcb8
  // (left out: dead in C++)
  r3 = f8225FCB8(base, r3);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r3
  r4 = r3;
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x822952d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XShowGamerCardUI(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, 0x8226A4B8); r3 = o.r3; f1 = o.f1; }
loc_8226A4B8:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8226a4f8
  if (cr6.eq) goto loc_8226A4F8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226A4D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x8226a4e4
  if (cr6.eq) goto loc_8226A4E4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226A4E4:
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // stw r26,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r26));
  // stw r27,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r27));
  // stw r26,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r26));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
loc_8226A4F8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226A500: lifted from generated/ by lift.py.
lifted::Result f8226A500(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226A500 (lifted)");
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
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,9648(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9648_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226a658
  if (!cr6.eq) goto loc_8226A658;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // bne cr6,0x8226a550
  if (!cr6.eq) goto loc_8226A550;
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
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8226A54C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226a658
  goto loc_8226A658;
loc_8226A550:
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8226A554);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x8226a600
  if (!cr0.lt) goto loc_8226A600;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r6,40
  r6 = uint64_t(int64_t(40));
  // lwz r3,604(r11)
  r3 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r3,r10
  r3 = r3 + r10;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r28,r3,r31
  r28 = uint32_t(uint32_t(r31) ? uint32_t(r3) / uint32_t(r31) : 0);
  // mulli r28,r28,50
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(50))));
  // subf r10,r28,r3
  r10 = r3 - r28;
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
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r9,604(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  // stw r29,9648(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9648_address(), uint32_t(r29));
  // b 0x8226a658
  goto loc_8226A658;
loc_8226A600:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822601f0
  // (left out: dead in C++)
  r3 = f822601F0(base, r1, r13, r3);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24852(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24852);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // bne cr6,0x8226a638
  if (!cr6.eq) goto loc_8226A638;
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x8226a630
  if (cr6.eq) goto loc_8226A630;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226A630:
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // b 0x8226a648
  goto loc_8226A648;
loc_8226A638:
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // beq cr6,0x8226a644
  if (cr6.eq) goto loc_8226A644;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226A644:
  // li r9,11
  r9 = uint64_t(int64_t(11));
loc_8226A648:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
loc_8226A658:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226A660: lifted from generated/ by lift.py.
uint64_t f8226A660(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8226A660 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,9648(r31)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9648_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226a76c
  if (!cr6.eq) goto loc_8226A76C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r29,r11,24704
  r29 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // beq cr6,0x8226a76c
  if (cr6.eq) goto loc_8226A76C;
  // mr r3,r29
  r3 = r29;
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8226A69C);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x8226a6ac
  if (!cr0.lt) goto loc_8226A6AC;
loc_8226A6A4:
  // li r5,40
  r5 = uint64_t(int64_t(40));
  // b 0x8226a6d8
  goto loc_8226A6D8;
loc_8226A6AC:
  // mr r3,r29
  r3 = r29;
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8226A6B4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8226a76c
  if (cr0.lt) goto loc_8226A76C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8226a6a4
  if (cr0.eq) goto loc_8226A6A4;
  // stw r9,9704(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9704_address(), uint32_t(r9));
  // li r5,39
  r5 = uint64_t(int64_t(39));
loc_8226A6D8:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r10,50
  r10 = uint64_t(int64_t(50));
  // li r8,37
  r8 = uint64_t(int64_t(37));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r30,604(r11)
  r30 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // add r30,r30,r9
  r30 = r30 + r9;
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r28,r30,r10
  r28 = uint32_t(uint32_t(r10) ? uint32_t(r30) / uint32_t(r10) : 0);
  // mulli r28,r28,50
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(50))));
  // subf r9,r28,r30
  r9 = r30 - r28;
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r9,r9,r11
  r9 = r9 + r11;
  // stw r8,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r8));
  // stw r7,4(r9)
  guest::StoreU32(base, uint32_t(r9) + 4, uint32_t(r7));
  // stw r6,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r6));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r8,604(r11)
  r8 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r8,r8,r9
  r8 = r8 + r9;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r10,r8,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r8) / uint32_t(r10) : 0);
  // mulli r10,r10,50
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(50))));
  // subf r10,r10,r8
  r10 = r8 - r10;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r5,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r5));
  // stw r4,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r4));
  // stw r3,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r3));
  // stw r29,9648(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9648_address(), uint32_t(r29));
loc_8226A76C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8226A778: lifted from generated/ by lift.py.
lifted::Result f8226A778(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8226A778 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // mr r27,r4
  r27 = r4;
  // lwz r11,9648(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9648_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226a938
  if (!cr6.eq) goto loc_8226A938;
  // bl 0x8214d7a0
  // (left out: dead in C++)
  { const lifted::Result o = f8214D7A0(base, r1, r13, r6, r7, r9); r3 = o.r3; f1 = o.f1; }
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8226A7A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // lwz r11,29420(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29420);
  // subf r11,r11,r3
  r11 = r3 - r11;
  // cmplwi cr6,r11,10000
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10000, so);
  // ble cr6,0x8226a888
  if (!cr6.gt) goto loc_8226A888;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // stw r10,88(r11)
  guest::StoreU32(base, uint32_t(r11) + 88, uint32_t(r10));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r27,204
  r11 = uint64_t(int64_t(int64_t(r27) + 204));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,40
  r6 = uint64_t(int64_t(40));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r29,604(r11)
  r29 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r29,r29,r10
  r29 = r29 + r10;
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r26,r29,r31
  r26 = uint32_t(uint32_t(r31) ? uint32_t(r29) / uint32_t(r31) : 0);
  // mulli r26,r26,50
  r26 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(50))));
  // subf r10,r26,r29
  r10 = r29 - r26;
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
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r9,604(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  // stw r28,9648(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9648_address(), uint32_t(r28));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24880(r10)
  guest::StoreU32(base, uint32_t(r10) + 24880, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24884(r10)
  guest::StoreU32(base, uint32_t(r10) + 24884, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24888(r10)
  guest::StoreU32(base, uint32_t(r10) + 24888, uint32_t(r11));
loc_8226A888:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4096
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4096, so);
  // bne cr6,0x8226a938
  if (!cr6.eq) goto loc_8226A938;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
  // addi r11,r27,204
  r11 = uint64_t(int64_t(int64_t(r27) + 204));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,39
  r6 = uint64_t(int64_t(39));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,604(r11)
  r3 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r3,r3,r10
  r3 = r3 + r10;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
  // divwu r29,r3,r31
  r29 = uint32_t(uint32_t(r31) ? uint32_t(r3) / uint32_t(r31) : 0);
  // mulli r29,r29,50
  r29 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(50))));
  // subf r10,r29,r3
  r10 = r3 - r29;
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
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r9,604(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r10));
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r28,9648(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9648_address(), uint32_t(r28));
  // stw r11,9728(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9728_address(), uint32_t(r11));
loc_8226A938:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226A940: lifted from generated/ by lift.py.
uint64_t f8226A940(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8226A940 (lifted)");
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,9648(r31)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9648_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226aa3c
  if (!cr6.eq) goto loc_8226AA3C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4096
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4096, so);
  // bne cr6,0x8226a978
  if (!cr6.eq) goto loc_8226A978;
  // li r5,39
  r5 = uint64_t(int64_t(39));
  // b 0x8226a9a8
  goto loc_8226A9A8;
loc_8226A978:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,88(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8226a9a4
  if (!cr6.eq) goto loc_8226A9A4;
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8226A990, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,29420(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29420);
  // subf r11,r11,r3
  r11 = r3 - r11;
  // cmplwi cr6,r11,10000
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10000, so);
  // ble cr6,0x8226aa3c
  if (!cr6.gt) goto loc_8226AA3C;
loc_8226A9A4:
  // li r5,40
  r5 = uint64_t(int64_t(40));
loc_8226A9A8:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r10,50
  r10 = uint64_t(int64_t(50));
  // li r8,37
  r8 = uint64_t(int64_t(37));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r30,604(r11)
  r30 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // add r30,r30,r9
  r30 = r30 + r9;
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r28,r30,r10
  r28 = uint32_t(uint32_t(r10) ? uint32_t(r30) / uint32_t(r10) : 0);
  // mulli r28,r28,50
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(50))));
  // subf r9,r28,r30
  r9 = r30 - r28;
  // mulli r9,r9,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(12))));
  // add r9,r9,r11
  r9 = r9 + r11;
  // stw r8,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r8));
  // stw r7,4(r9)
  guest::StoreU32(base, uint32_t(r9) + 4, uint32_t(r7));
  // stw r6,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r6));
  // lwz r9,600(r11)
  r9 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // lwz r8,604(r11)
  r8 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r8,r8,r9
  r8 = r8 + r9;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address(), uint32_t(r9));
  // divwu r10,r8,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r8) / uint32_t(r10) : 0);
  // mulli r10,r10,50
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(50))));
  // subf r10,r10,r8
  r10 = r8 - r10;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r5,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r5));
  // stw r4,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r4));
  // stw r3,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r3));
  // stw r29,9648(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9648_address(), uint32_t(r29));
loc_8226AA3C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8226AA48: lifted from generated/ by lift.py.
lifted::Result f8226AA48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8226AA48 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r30,r4
  r30 = r4;
  // lwz r29,-21596(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x8226AA6C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // mr r7,r8
  r7 = r8;
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // li r9,16
  r9 = uint64_t(int64_t(16));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x8226aadc
  if (cr6.eq) goto loc_8226AADC;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226AADC:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // li r5,60
  r5 = uint64_t(int64_t(60));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // mr r11,r8
  r11 = r8;
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // stw r8,9644(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9644_address(), uint32_t(r8));
  // stw r8,9648(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9648_address(), uint32_t(r8));
  // addi r10,r10,30752
  r10 = uint64_t(int64_t(int64_t(r10) + 30752));
  // addi r3,r10,8
  r3 = uint64_t(int64_t(int64_t(r10) + 8));
  // stb r11,27404(r9)
  guest::StoreU8(base, uint32_t(r9) + 27404, uint8_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r29,84(r10)
  guest::StoreU32(base, uint32_t(r10) + 84, uint32_t(r29));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24896(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24896);
  // mulli r11,r11,92
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(92))));
  // add r4,r11,r9
  r4 = r11 + r9;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8226AB38, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82148fd8
  // (left out: dead in C++)
  { const lifted::Result o = f82148FD8(base, r1, r13, r3, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226AB48: lifted from generated/ by lift.py.
lifted::Result f8226AB48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8226AB48 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,27
  r9 = uint64_t(int64_t(27));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_address());
  // mr r31,r3
  r31 = r3;
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
  // bl 0x82269018
  // (left out: dead in C++)
  { const lifted::Result o = f82269018(base, r1, r13); r3 = o.r3; f1 = o.f1; }
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,9604(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).state_address(), uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r10,9628(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).race_address(), uint32_t(r10));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // addi r11,r11,26160
  r11 = uint64_t(int64_t(int64_t(r11) + 26160));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
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

// sub_8226ABE0: lifted from generated/ by lift.py.
lifted::Result f8226ABE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8226ABE0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r28,r4
  r28 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226AC0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r29,0(r3)
  r29 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226ac98
  if (!cr6.eq) goto loc_8226AC98;
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9656);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8226ac4c
  if (!cr6.gt) goto loc_8226AC4C;
loc_8226AC2C:
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // mr r3,r29
  r3 = r29;
  // add r4,r11,r30
  r4 = r11 + r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226AC3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9656);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8226ac2c
  if (cr6.lt) goto loc_8226AC2C;
loc_8226AC4C:
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r29
  r3 = r29;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226AC78, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // mr r3,r29
  r3 = r29;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r10,r11
  r4 = r10 + r11;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226AC90, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_8226AC98:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226ACA4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226ACB0: lifted from generated/ by lift.py.
lifted::Result f8226ACB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8226ACB0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r28,r4
  r28 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226ACDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226ada0
  if (!cr6.eq) goto loc_8226ADA0;
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9656);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addic. r11,r11,1
  ca = uint32_t(r11) > 4294967294;
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8226ad20
  if (cr0.eq) goto loc_8226AD20;
loc_8226ACFC:
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // mr r3,r30
  r3 = r30;
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226AD0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9656);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8226acfc
  if (cr6.lt) goto loc_8226ACFC;
loc_8226AD20:
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // mr r3,r30
  r3 = r30;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x8226ad78
  if (!cr6.eq) goto loc_8226AD78;
  // lwz r10,9700(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9700);
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226AD5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r9,9672(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // lwz r8,9700(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 9700);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r11,r9,r8
  r11 = r9 + r8;
  // add r4,r11,r10
  r4 = r11 + r10;
  // b 0x8226ad90
  goto loc_8226AD90;
loc_8226AD78:
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226AD80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r10,r11
  r4 = r10 + r11;
loc_8226AD90:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226AD98, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_8226ADA0:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226ADB0: lifted from generated/ by lift.py.
lifted::Result f8226ADB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8226ADB0 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r4
  r29 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226ADD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226ae70
  if (!cr6.eq) goto loc_8226AE70;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_8226ADEC:
  // addi r4,r30,199
  r4 = uint64_t(int64_t(int64_t(r30) + 199));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226ADF8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,7
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 7, so);
  // blt cr6,0x8226adec
  if (cr6.lt) goto loc_8226ADEC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,251
  r4 = uint64_t(int64_t(251));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226AE20, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,251
  r4 = uint64_t(int64_t(251));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226AE2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // addi r4,r11,199
  r4 = uint64_t(int64_t(int64_t(r11) + 199));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226AE54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // addi r4,r11,199
  r4 = uint64_t(int64_t(int64_t(r11) + 199));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226AE68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_8226AE70:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226AE7C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226AE88: lifted from generated/ by lift.py.
lifted::Result f8226AE88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8226AE88 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226AEB4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226af34
  if (!cr6.eq) goto loc_8226AF34;
  // mr r3,r31
  r3 = r31;
  // bl 0x82270a80
  // (left out: dead in C++)
  r3 = f82270A80(base, r1, r13);
  // mr r3,r30
  r3 = r30;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226AEDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9668);
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24932);
  // lwz r28,0(r11)
  r28 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226AEF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r28,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // li r4,260
  r4 = uint64_t(int64_t(260));
  // mr r3,r30
  r3 = r30;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).y_address(), temp.u32);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226AF2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_8226AF34:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226AF40, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226AF50: lifted from generated/ by lift.py.
lifted::Result f8226AF50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226AF50 (lifted)");
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226AF7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226afb8
  if (!cr6.eq) goto loc_8226AFB8;
  // mr r3,r29
  r3 = r29;
  // bl 0x82270dc0
  // (left out: dead in C++)
  { const lifted::Result o = f82270DC0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,262
  r4 = uint64_t(int64_t(262));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226AFB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8226AFB8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226AFC4, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226AFD8: lifted from generated/ by lift.py.
lifted::Result f8226AFD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226AFD8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r30,r4
  r30 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B004, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b0c4
  if (!cr6.eq) goto loc_8226B0C4;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a130
  // (left out: dead in C++)
  r3 = game::UiMovie::ResetNodeFields(base, r1, r13, r3, r4, r5);
  // mr r3,r29
  r3 = r29;
  // bl 0x82270dc0
  // (left out: dead in C++)
  { const lifted::Result o = f82270DC0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B04C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226B058, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,255
  r4 = uint64_t(int64_t(255));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B074, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,255
  r4 = uint64_t(int64_t(255));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226B080, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,264
  r4 = uint64_t(int64_t(264));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B09C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8226B0B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9724(r29)
  guest::StoreU32(base, uint32_t(r29) + 9724, uint32_t(r11));
  // stw r28,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r28));
loc_8226B0C4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226B0D0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8226b0dc
  if (!cr0.eq) goto loc_8226B0DC;
  // stw r28,9724(r29)
  guest::StoreU32(base, uint32_t(r29) + 9724, uint32_t(r28));
loc_8226B0DC:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8226b114
  if (!cr0.eq) goto loc_8226B114;
  // lwz r11,9724(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9724);
  // mr r3,r28
  r3 = r28;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b118
  if (!cr6.eq) goto loc_8226B118;
loc_8226B114:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8226B118:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226B120: lifted from generated/ by lift.py.
lifted::Result f8226B120(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226B120 (lifted)");
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B14C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b188
  if (!cr6.eq) goto loc_8226B188;
  // mr r3,r29
  r3 = r29;
  // bl 0x82270f40
  // (left out: dead in C++)
  { const lifted::Result o = f82270F40(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,258
  r4 = uint64_t(int64_t(258));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B180, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8226B188:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226B194, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226B1A8: lifted from generated/ by lift.py.
lifted::Result f8226B1A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226B1A8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r30,r4
  r30 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B1D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b294
  if (!cr6.eq) goto loc_8226B294;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a130
  // (left out: dead in C++)
  r3 = game::UiMovie::ResetNodeFields(base, r1, r13, r3, r4, r5);
  // mr r3,r29
  r3 = r29;
  // bl 0x82270f40
  // (left out: dead in C++)
  { const lifted::Result o = f82270F40(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B21C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226B228, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,255
  r4 = uint64_t(int64_t(255));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B244, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,255
  r4 = uint64_t(int64_t(255));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226B250, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,259
  r4 = uint64_t(int64_t(259));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B26C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8226B288, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9724(r29)
  guest::StoreU32(base, uint32_t(r29) + 9724, uint32_t(r11));
  // stw r28,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r28));
loc_8226B294:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226B2A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8226b2ac
  if (!cr0.eq) goto loc_8226B2AC;
  // stw r28,9724(r29)
  guest::StoreU32(base, uint32_t(r29) + 9724, uint32_t(r28));
loc_8226B2AC:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8226b2e4
  if (!cr0.eq) goto loc_8226B2E4;
  // lwz r11,9724(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9724);
  // mr r3,r28
  r3 = r28;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b2e8
  if (!cr6.eq) goto loc_8226B2E8;
loc_8226B2E4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8226B2E8:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226B2F0: lifted from generated/ by lift.py.
lifted::Result f8226B2F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8226B2F0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r27,r4
  r27 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B31C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b4b4
  if (!cr6.eq) goto loc_8226B4B4;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_8226B330:
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // mr r3,r30
  r3 = r30;
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226B340, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r29,4
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4, so);
  // blt cr6,0x8226b330
  if (cr6.lt) goto loc_8226B330;
  // mr r3,r30
  r3 = r30;
  // lwz r4,9676(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9676);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226B358, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // lwz r4,9680(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9680);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226B364, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226B374, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9668);
  // lwz r11,24932(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24932);
  // lwz r29,0(r11)
  r29 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B388, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).y_address(), temp.u32);
  // lwz r11,9716(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9716);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b3f0
  if (!cr6.eq) goto loc_8226B3F0;
  // lwz r11,24852(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24852);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8226b3f0
  if (!cr6.eq) goto loc_8226B3F0;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226B3D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B3D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r30
  r3 = r30;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B3F0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226B3F0:
  // lwz r11,9708(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9708);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226b40c
  if (cr6.eq) goto loc_8226B40C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8226b448
  if (!cr6.eq) goto loc_8226B448;
loc_8226B40C:
  // lwz r11,24852(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24852);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8226b448
  if (!cr6.eq) goto loc_8226B448;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226B428, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B430, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r30
  r3 = r30;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B448, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226B448:
  // lwz r11,9716(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9716);
  // lwz r10,24932(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 24932);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b470
  if (!cr6.eq) goto loc_8226B470;
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r10)).cursor_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b470
  if (!cr6.eq) goto loc_8226B470;
  // lwz r11,24852(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24852);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x8226b4ac
  if (cr6.eq) goto loc_8226B4AC;
loc_8226B470:
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r10)).cursor_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // add r4,r11,r10
  r4 = r11 + r10;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B494, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24932);
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r10,r11
  r4 = r10 + r11;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226B4AC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226B4AC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r11));
loc_8226B4B4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226B4C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226B4D0: lifted from generated/ by lift.py.
lifted::Result f8226B4D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8226B4D0 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r4
  r29 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B4F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b608
  if (!cr6.eq) goto loc_8226B608;
  // li r4,210
  r4 = uint64_t(int64_t(210));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226B514, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226B524, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B530, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B548, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B554, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B56C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B578, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B590, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B59C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B5B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B5C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B5D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B5F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226B600, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_8226B608:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226B614, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226B620: lifted from generated/ by lift.py.
lifted::Result f8226B620(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8226B620 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r4
  r29 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B648, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b768
  if (!cr6.eq) goto loc_8226B768;
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226B664, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226B674, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B680, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,67
  r7 = uint64_t(int64_t(67));
  // li r6,235
  r6 = uint64_t(int64_t(235));
  // li r5,205
  r5 = uint64_t(int64_t(205));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B698, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B6A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,67
  r7 = uint64_t(int64_t(67));
  // li r6,235
  r6 = uint64_t(int64_t(235));
  // li r5,205
  r5 = uint64_t(int64_t(205));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B6BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B6C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,67
  r7 = uint64_t(int64_t(67));
  // li r6,235
  r6 = uint64_t(int64_t(235));
  // li r5,205
  r5 = uint64_t(int64_t(205));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B6E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B6EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,67
  r7 = uint64_t(int64_t(67));
  // li r6,235
  r6 = uint64_t(int64_t(235));
  // li r5,205
  r5 = uint64_t(int64_t(205));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B704, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B710, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,67
  r7 = uint64_t(int64_t(67));
  // li r6,235
  r6 = uint64_t(int64_t(235));
  // li r5,205
  r5 = uint64_t(int64_t(205));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226B728, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B744, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,210
  r4 = uint64_t(int64_t(210));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226B760, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_8226B768:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226B774, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226B780: lifted from generated/ by lift.py.
lifted::Result f8226B780(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8226B780 (lifted)");
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
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B7B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226b7e4
  if (!cr6.eq) goto loc_8226B7E4;
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
  r3 = guest::LoadU32(base, uint32_t(r11) + 7152);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x8226B7DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8226B7E4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226B7F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
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

// sub_8226B830: lifted from generated/ by lift.py.
lifted::Result f8226B830(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226B830 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B85C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // subfic r25,r31,8
  ca = uint32_t(r31) <= 8;
  r25 = static_cast<uint64_t>(8) - r31;
  // li r24,7
  r24 = uint64_t(int64_t(7));
loc_8226B868:
  // add r28,r25,r31
  r28 = r25 + r31;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // mr r5,r28
  r5 = r28;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226B87C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9704(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 9704);
  // mr r29,r3
  r29 = r3;
  // cmpw cr6,r11,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r31), so);
  // mr r5,r28
  r5 = r28;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bgt cr6,0x8226b8b8
  if (cr6.gt) goto loc_8226B8B8;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226B89C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B8A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(base, LIFTED_ARGS(0x8226B8B4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226b928
  goto loc_8226B928;
loc_8226B8B8:
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8226B8BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B8C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8225fd18
  // (left out: dead in C++)
  r3 = f8225FD18(base, r3);
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x8226B8D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B8E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r27,108(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).texture_entry_address(), uint32_t(r27));
  // bl 0x8225fb88
  // (left out: dead in C++)
  f8225FB88(base, r1, r3, r4);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8226B90C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B918, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c8a0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeCharText(base, LIFTED_ARGS(0x8226B928, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226B928:
  // addic. r24,r24,-1
  ca = uint32_t(r24) > 0;
  r24 = uint64_t(int64_t(int64_t(r24) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r24), 0, so);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // bne 0x8226b868
  if (!cr0.eq) goto loc_8226B868;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226B940: lifted from generated/ by lift.py.
lifted::Result f8226B940(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226B940 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r22,r3
  r22 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226B96C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226B980, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x8225fb88
  // (left out: dead in C++)
  f8225FB88(base, r1, r3, r4);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8226B9A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B9AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c8a0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeCharText(base, LIFTED_ARGS(0x8226B9BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226B9CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x8225fd18
  // (left out: dead in C++)
  r3 = f8225FD18(base, r3);
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x8226B9DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226B9EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r29
  r3 = r29;
  // stw r27,108(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).texture_entry_address(), uint32_t(r27));
  // bl 0x8225fe30
  // (left out: dead in C++)
  r3 = f8225FE30(base, r3);
  // mr r28,r3
  r28 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x8225ff00
  // (left out: dead in C++)
  r3 = f8225FF00(base, r3);
  // lis r11,152
  r11 = uint64_t(int64_t(9961472));
  // mr r27,r3
  r27 = r3;
  // ori r11,r11,38527
  r11 = r11 | 38527;
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // mr r24,r11
  r24 = r11;
  // bgt cr6,0x8226ba24
  if (cr6.gt) goto loc_8226BA24;
  // mr r24,r28
  r24 = r28;
loc_8226BA24:
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8226ba3c
  if (cr6.eq) goto loc_8226BA3C;
  // divwu r25,r28,r27
  r25 = uint32_t(uint32_t(r27) ? uint32_t(r28) / uint32_t(r27) : 0);
  // twllei r27,0
  if (int32_t(r27) == 0 || uint32_t(r27) < 0u) lifted::Trap(0);
  // b 0x8226ba40
  goto loc_8226BA40;
loc_8226BA3C:
  // mr r25,r23
  r25 = r23;
loc_8226BA40:
  // mr r3,r29
  r3 = r29;
  // bl 0x8225fe98
  // (left out: dead in C++)
  r3 = f8225FE98(base, r3);
  // cmpwi cr6,r3,9999
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 9999, so);
  // ble cr6,0x8226ba58
  if (!cr6.gt) goto loc_8226BA58;
  // li r26,9999
  r26 = uint64_t(int64_t(9999));
  // b 0x8226ba64
  goto loc_8226BA64;
loc_8226BA58:
  // mr r3,r29
  r3 = r29;
  // bl 0x8225fe98
  // (left out: dead in C++)
  r3 = f8225FE98(base, r3);
  // mr r26,r3
  r26 = r3;
loc_8226BA64:
  // cmplwi cr6,r27,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 9999, so);
  // li r28,9999
  r28 = uint64_t(int64_t(9999));
  // bgt cr6,0x8226ba74
  if (cr6.gt) goto loc_8226BA74;
  // mr r28,r27
  r28 = r27;
loc_8226BA74:
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BA80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r24
  r6 = r24;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226BA94, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BAA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r25
  r6 = r25;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226BAB4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BAC0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r7,r28
  r7 = r28;
  // mr r6,r26
  r6 = r26;
  // li r5,325
  r5 = uint64_t(int64_t(325));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226BAD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226bb28
  if (!cr6.eq) goto loc_8226BB28;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BAF4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // stw r28,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r28));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BB0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // stw r28,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r28));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BB20, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,80(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).visible_address(), uint32_t(r28));
  // b 0x8226bb58
  goto loc_8226BB58;
loc_8226BB28:
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BB2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // stw r23,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r23));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BB40, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // stw r23,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r23));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BB54, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r23,80(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).visible_address(), uint32_t(r23));
loc_8226BB58:
  // mr r3,r29
  r3 = r29;
  // bl 0x8225fd78
  // (left out: dead in C++)
  r3 = f8225FD78(base, r3);
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r22
  r3 = r22;
  // bl 0x822707c8
  // (left out: dead in C++)
  r3 = f822707C8(base, r1, r4, r5);
  // mr r6,r3
  r6 = r3;
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226BB84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x8225fdd0
  // (left out: dead in C++)
  r3 = f8225FDD0(base, r3);
  // bl 0x822601b0
  // (left out: dead in C++)
  r3 = f822601B0(base, r3);
  // mr r11,r3
  r11 = r3;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // addi r7,r11,1
  r7 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226BBAC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226BBB8: lifted from generated/ by lift.py.
lifted::Result f8226BBB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8226BBB8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226BBE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r11)).value_address());
  // lwz r4,29308(r10)
  r4 = guest::LoadU32(base, uint32_t(r10) + 29308);
  // bl 0x822707c8
  // (left out: dead in C++)
  r3 = f822707C8(base, r1, r4, r5);
  // mr r6,r3
  r6 = r3;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226BC18, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,29312(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 29312);
  // bl 0x822601b0
  // (left out: dead in C++)
  r3 = f822601B0(base, r3);
  // mr r11,r3
  r11 = r3;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // addi r7,r11,1
  r7 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226BC40, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28180);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8226bc88
  if (!cr6.eq) goto loc_8226BC88;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8226BC60, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r3,r30
  r3 = r30;
  // lwz r4,29416(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 29416);
  // bl 0x82270828
  // (left out: dead in C++)
  r3 = f82270828(base, r1, r4);
  // mr r6,r3
  r6 = r3;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226BC84, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226bc9c
  goto loc_8226BC9C;
loc_8226BC88:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226BC8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x8226BC9C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226BC9C:
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

// sub_8226BCB8: lifted from generated/ by lift.py.
lifted::Result f8226BCB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226BCB8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-832(r1)
  ea = -832 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r4
  r29 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226BCE4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226BCF8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8226BD0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BD18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,317
  r5 = uint64_t(int64_t(317));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226BD28, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BD34, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,318
  r5 = uint64_t(int64_t(318));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226BD44, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BD50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,319
  r5 = uint64_t(int64_t(319));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226BD60, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r21,-32127
  r21 = uint64_t(int64_t(-2105475072));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24848(r21)
  r11 = guest::LoadU32(base, uint32_t(r21) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226bdbc
  if (!cr6.eq) goto loc_8226BDBC;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BD7C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226BD8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BD98, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226BDA8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BDB4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // b 0x8226bdfc
  goto loc_8226BDFC;
loc_8226BDBC:
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BDC0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226BDD0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BDDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226BDEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BDF8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
loc_8226BDFC:
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226BE08, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // bne cr6,0x8226c108
  if (!cr6.eq) goto loc_8226C108;
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // bne cr6,0x8226be98
  if (!cr6.eq) goto loc_8226BE98;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r11,30320
  r4 = uint64_t(int64_t(int64_t(r11) + 30320));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8226BE34, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24960);
  // lwz r23,844(r11)
  r23 = guest::LoadU32(base, uint32_t(r11) + 844);
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x8226BE48, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r11,r1,272
  r11 = uint64_t(int64_t(int64_t(r1) + 272));
  // rlwinm r10,r29,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
  // lwzx r27,r10,r11
  r27 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r3,-21596(r28)
  r3 = guest::LoadU32(base, uint32_t(r28) + -21596);
  // bl 0x82260908
  // (left out: dead in C++)
  { const lifted::Result o = game::Session::IsActive(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8226bee0
  if (!cr6.eq) goto loc_8226BEE0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r4,-21596(r28)
  r4 = guest::LoadU32(base, uint32_t(r28) + -21596);
  // lwz r3,29856(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 29856);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r3)).voice_engine_address());
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_is_headset_present_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226BE88, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8226bee0
  if (!cr6.eq) goto loc_8226BEE0;
  // mr r27,r24
  r27 = r24;
  // b 0x8226bee0
  goto loc_8226BEE0;
loc_8226BE98:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r11,29464
  r4 = uint64_t(int64_t(int64_t(r11) + 29464));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8226BEA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r22
  r29 = r22;
  // lbz r10,27404(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 27404);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // subf r10,r24,r10
  r10 = r10 - r24;
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,27416
  r11 = uint64_t(int64_t(int64_t(r11) + 27416));
  // lwz r23,12(r11)
  r23 = guest::LoadU32(base, uint32_t(r11) + 12);
  // cntlzw r11,r10
  r11 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r27,r11,27,31,31
  r27 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x82260908
  // (left out: dead in C++)
  { const lifted::Result o = game::Session::IsActive(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8226bee0
  if (!cr6.eq) goto loc_8226BEE0;
  // mr r27,r22
  r27 = r22;
loc_8226BEE0:
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226BEF0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,412
  r11 = uint64_t(int64_t(int64_t(r1) + 412));
  // rlwinm r28,r29,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // mr r26,r3
  r26 = r3;
  // lwzx r3,r28,r11
  r3 = guest::LoadU32(base, uint32_t(r28) + uint32_t(r11));
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x8226BF04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BF14, r31)); r3 = o.r3; f1 = o.f1; }
  // subf r10,r24,r27
  r10 = r27 - r24;
  // mr r11,r3
  r11 = r3;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // stw r25,108(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).texture_entry_address(), uint32_t(r25));
  // xori r6,r10,1
  r6 = r10 ^ 1;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8226BF3C, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r29,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 5) & 0xFFFFFFE0;
  // addi r5,r1,110
  r5 = uint64_t(int64_t(int64_t(r1) + 110));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,256
  r8 = uint64_t(int64_t(256));
  // addi r7,r1,480
  r7 = uint64_t(int64_t(int64_t(r1) + 480));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // add r5,r11,r5
  r5 = r11 + r5;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = game::WideCharToMultiByte(base, LIFTED_ARGS(0x8226BF68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BF74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,480
  r5 = uint64_t(int64_t(int64_t(r1) + 480));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c8a0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeCharText(base, LIFTED_ARGS(0x8226BF84, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,428
  r11 = uint64_t(int64_t(int64_t(r1) + 428));
  // addi r9,r1,444
  r9 = uint64_t(int64_t(int64_t(r1) + 444));
  // lis r10,152
  r10 = uint64_t(int64_t(9961472));
  // ori r10,r10,38527
  r10 = r10 | 38527;
  // lwzx r11,r28,r11
  r11 = guest::LoadU32(base, uint32_t(r28) + uint32_t(r11));
  // lwzx r29,r28,r9
  r29 = guest::LoadU32(base, uint32_t(r28) + uint32_t(r9));
  // mr r26,r10
  r26 = r10;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bgt cr6,0x8226bfac
  if (cr6.gt) goto loc_8226BFAC;
  // mr r26,r11
  r26 = r11;
loc_8226BFAC:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8226bfc0
  if (cr6.eq) goto loc_8226BFC0;
  // divwu r27,r11,r29
  r27 = uint32_t(uint32_t(r29) ? uint32_t(r11) / uint32_t(r29) : 0);
  // twllei r29,0
  if (int32_t(r29) == 0 || uint32_t(r29) < 0u) lifted::Trap(0);
  // b 0x8226bfc4
  goto loc_8226BFC4;
loc_8226BFC0:
  // mr r27,r22
  r27 = r22;
loc_8226BFC4:
  // addi r11,r1,460
  r11 = uint64_t(int64_t(int64_t(r1) + 460));
  // lwzx r28,r28,r11
  r28 = guest::LoadU32(base, uint32_t(r28) + uint32_t(r11));
  // cmplwi cr6,r28,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 9999, so);
  // ble cr6,0x8226bfd8
  if (!cr6.gt) goto loc_8226BFD8;
  // li r28,9999
  r28 = uint64_t(int64_t(9999));
loc_8226BFD8:
  // cmplwi cr6,r29,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 9999, so);
  // ble cr6,0x8226bfe4
  if (!cr6.gt) goto loc_8226BFE4;
  // li r29,9999
  r29 = uint64_t(int64_t(9999));
loc_8226BFE4:
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226BFF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r26
  r6 = r26;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C004, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C010, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r27
  r6 = r27;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C024, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C030, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r7,r29
  r7 = r29;
  // mr r6,r28
  r6 = r28;
  // li r5,325
  r5 = uint64_t(int64_t(325));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C048, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24848(r21)
  r11 = guest::LoadU32(base, uint32_t(r21) + 24848);
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r3,r30
  r3 = r30;
  // bne cr6,0x8226c090
  if (!cr6.eq) goto loc_8226C090;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C060, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // stw r24,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r24));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C074, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // stw r24,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r24));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C088, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r24,80(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).visible_address(), uint32_t(r24));
  // b 0x8226c0c0
  goto loc_8226C0C0;
loc_8226C090:
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C094, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // stw r22,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r22));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C0A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // stw r22,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r22));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C0BC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r22,80(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).visible_address(), uint32_t(r22));
loc_8226C0C0:
  // cmplwi cr6,r23,71
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 71, so);
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bgt cr6,0x8226c204
  if (cr6.gt) goto loc_8226C204;
  // addi r6,r23,1646
  r6 = uint64_t(int64_t(int64_t(r23) + 1646));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226C0DC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r23,1790
  r6 = uint64_t(int64_t(int64_t(r23) + 1790));
  // li r5,25
  r5 = uint64_t(int64_t(25));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226C0F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r23,1718
  r6 = uint64_t(int64_t(int64_t(r23) + 1718));
  // li r5,26
  r5 = uint64_t(int64_t(26));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226C104, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226c228
  goto loc_8226C228;
loc_8226C108:
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226C118, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x8226C124, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C134, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // stw r28,108(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).texture_entry_address(), uint32_t(r28));
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8226C150, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C15C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(base, LIFTED_ARGS(0x8226C168, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C174, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(base, LIFTED_ARGS(0x8226C180, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C18C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(base, LIFTED_ARGS(0x8226C198, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C1A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(base, LIFTED_ARGS(0x8226C1B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C1BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(base, LIFTED_ARGS(0x8226C1C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C1D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(base, LIFTED_ARGS(0x8226C1E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C1EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(base, LIFTED_ARGS(0x8226C1F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
loc_8226C204:
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x8226C208, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,25
  r5 = uint64_t(int64_t(25));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x8226C218, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,26
  r5 = uint64_t(int64_t(26));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(base, LIFTED_ARGS(0x8226C228, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226C228:
  // addi r1,r1,832
  r1 = uint64_t(int64_t(int64_t(r1) + 832));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226C230: lifted from generated/ by lift.py.
lifted::Result f8226C230(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226C230 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r26,r4
  r26 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r27,r5
  r27 = r5;
  // mr r28,r6
  r28 = r6;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226C264, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lwz r4,9660(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 9660);
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226C278, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r26,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 1, so);
  // blt cr6,0x8226c34c
  if (cr6.lt) goto loc_8226C34C;
  // beq cr6,0x8226c304
  if (cr6.eq) goto loc_8226C304;
  // cmplwi cr6,r26,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 3, so);
  // bge cr6,0x8226c3a4
  if (!cr6.lt) goto loc_8226C3A4;
  // cmpwi cr6,r28,3
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 3, so);
  // li r4,25
  r4 = uint64_t(int64_t(25));
  // bne cr6,0x8226c2b4
  if (!cr6.eq) goto loc_8226C2B4;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C2A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,416
  r5 = uint64_t(int64_t(416));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C2B0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226c2cc
  goto loc_8226C2CC;
loc_8226C2B4:
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C2B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r6,r28,1
  r6 = uint64_t(int64_t(int64_t(r28) + 1));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C2CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226C2CC:
  // cmpwi cr6,r27,3
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 3, so);
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // mr r3,r30
  r3 = r30;
  // bne cr6,0x8226c2e8
  if (!cr6.eq) goto loc_8226C2E8;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C2E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,416
  r5 = uint64_t(int64_t(416));
  // b 0x8226c398
  goto loc_8226C398;
loc_8226C2E8:
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C2EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r6,r27,1
  r6 = uint64_t(int64_t(int64_t(r27) + 1));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C300, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226c3a4
  goto loc_8226C3A4;
loc_8226C304:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x822707c8
  // (left out: dead in C++)
  r3 = f822707C8(base, r1, r4, r5);
  // mr r28,r3
  r28 = r3;
  // li r4,19
  r4 = uint64_t(int64_t(19));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C324, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r28
  r5 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C334, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r27
  r4 = r27;
  // mr r3,r29
  r3 = r29;
  // bl 0x822707c8
  // (left out: dead in C++)
  r3 = f822707C8(base, r1, r4, r5);
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // b 0x8226c388
  goto loc_8226C388;
loc_8226C34C:
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x822707c0
  // (left out: dead in C++)
  r3 = f822707C0(base, r4);
  // mr r28,r3
  r28 = r3;
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C368, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r28
  r5 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C378, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r27
  r4 = r27;
  // mr r3,r29
  r3 = r29;
  // bl 0x822707c0
  // (left out: dead in C++)
  r3 = f822707C0(base, r4);
  // li r4,16
  r4 = uint64_t(int64_t(16));
loc_8226C388:
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C394, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
loc_8226C398:
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C3A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226C3A4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226C3B0: lifted from generated/ by lift.py.
lifted::Result f8226C3B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226C3B0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r26,r4
  r26 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226C3E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lwz r4,9660(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 9660);
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226C3F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r26,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 1, so);
  // blt cr6,0x8226c494
  if (cr6.lt) goto loc_8226C494;
  // beq cr6,0x8226c450
  if (cr6.eq) goto loc_8226C450;
  // cmplwi cr6,r26,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 3, so);
  // bge cr6,0x8226c4f4
  if (!cr6.lt) goto loc_8226C4F4;
  // mr r4,r27
  r4 = r27;
  // mr r3,r29
  r3 = r29;
  // bl 0x82270828
  // (left out: dead in C++)
  r3 = f82270828(base, r1, r4);
  // mr r27,r3
  r27 = r3;
  // li r4,25
  r4 = uint64_t(int64_t(25));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C42C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r27
  r5 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C43C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x82270828
  // (left out: dead in C++)
  r3 = f82270828(base, r1, r4);
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // b 0x8226c4d8
  goto loc_8226C4D8;
loc_8226C450:
  // li r4,19
  r4 = uint64_t(int64_t(19));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C45C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r6,r27,1
  r6 = uint64_t(int64_t(int64_t(r27) + 1));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C470, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C47C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r6,r28,1
  r6 = uint64_t(int64_t(int64_t(r28) + 1));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C490, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226c4f4
  goto loc_8226C4F4;
loc_8226C494:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r27
  r4 = r27;
  // mr r3,r29
  r3 = r29;
  // bl 0x822707c8
  // (left out: dead in C++)
  r3 = f822707C8(base, r1, r4, r5);
  // mr r27,r3
  r27 = r3;
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C4B4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r27
  r5 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C4C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x822707c8
  // (left out: dead in C++)
  r3 = f822707C8(base, r1, r4, r5);
  // li r4,16
  r4 = uint64_t(int64_t(16));
loc_8226C4D8:
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226C4E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r29
  r5 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226C4F4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226C4F4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226C500: lifted from generated/ by lift.py.
lifted::Result f8226C500(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226C500 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226C52C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmplwi cr6,r31,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 1, so);
  // blt cr6,0x8226c5a8
  if (cr6.lt) goto loc_8226C5A8;
  // beq cr6,0x8226c5a0
  if (cr6.eq) goto loc_8226C5A0;
  // cmplwi cr6,r31,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 3, so);
  // blt cr6,0x8226c598
  if (cr6.lt) goto loc_8226C598;
  // beq cr6,0x8226c558
  if (cr6.eq) goto loc_8226C558;
  // cmplwi cr6,r31,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 5, so);
  // bge cr6,0x8226c5cc
  if (!cr6.lt) goto loc_8226C5CC;
  // li r6,957
  r6 = uint64_t(int64_t(957));
  // b 0x8226c5bc
  goto loc_8226C5BC;
loc_8226C558:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8226c570
  if (!cr6.eq) goto loc_8226C570;
  // li r6,981
  r6 = uint64_t(int64_t(981));
  // b 0x8226c5bc
  goto loc_8226C5BC;
loc_8226C570:
  // lwz r11,9708(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9708);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r3,r30
  r3 = r30;
  // bne cr6,0x8226c590
  if (!cr6.eq) goto loc_8226C590;
  // li r6,956
  r6 = uint64_t(int64_t(956));
  // b 0x8226c5c8
  goto loc_8226C5C8;
loc_8226C590:
  // li r6,975
  r6 = uint64_t(int64_t(975));
  // b 0x8226c5c8
  goto loc_8226C5C8;
loc_8226C598:
  // li r6,955
  r6 = uint64_t(int64_t(955));
  // b 0x8226c5bc
  goto loc_8226C5BC;
loc_8226C5A0:
  // li r6,954
  r6 = uint64_t(int64_t(954));
  // b 0x8226c5bc
  goto loc_8226C5BC;
loc_8226C5A8:
  // lwz r11,9716(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9716);
  // li r6,974
  r6 = uint64_t(int64_t(974));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8226c5bc
  if (cr6.eq) goto loc_8226C5BC;
  // li r6,953
  r6 = uint64_t(int64_t(953));
loc_8226C5BC:
  // mr r3,r30
  r3 = r30;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_8226C5C8:
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226C5CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226C5CC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x8226C5DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x8226C5F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x8226C60C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226C618: lifted from generated/ by lift.py.
lifted::Result f8226C618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226C618 (lifted)");
  uint64_t r3 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226C648, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmplwi cr6,r31,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 1, so);
  // blt cr6,0x8226c670
  if (cr6.lt) goto loc_8226C670;
  // beq cr6,0x8226c668
  if (cr6.eq) goto loc_8226C668;
  // cmplwi cr6,r31,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 3, so);
  // bge cr6,0x8226c684
  if (!cr6.lt) goto loc_8226C684;
  // li r6,957
  r6 = uint64_t(int64_t(957));
  // b 0x8226c674
  goto loc_8226C674;
loc_8226C668:
  // li r6,955
  r6 = uint64_t(int64_t(955));
  // b 0x8226c674
  goto loc_8226C674;
loc_8226C670:
  // li r6,958
  r6 = uint64_t(int64_t(958));
loc_8226C674:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226C684, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226C684:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x8226C694, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x8226C6B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x8226C6C4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8226C6E0: lifted from generated/ by lift.py.
lifted::Result f8226C6E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226C6E0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226C714, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9656);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // mr r3,r30
  r3 = r30;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // li r6,978
  r6 = uint64_t(int64_t(978));
  // beq cr6,0x8226c73c
  if (cr6.eq) goto loc_8226C73C;
  // addi r6,r31,944
  r6 = uint64_t(int64_t(int64_t(r31) + 944));
loc_8226C73C:
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226C740, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x8226C750, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x8226C76C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x8226C780, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8226C798: lifted from generated/ by lift.py.
lifted::Result f8226C798(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226C798 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226C7C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9656);
  // lwz r29,0(r3)
  r29 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // bne cr6,0x8226c814
  if (!cr6.eq) goto loc_8226C814;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x8226c7ec
  if (!cr6.eq) goto loc_8226C7EC;
  // li r6,977
  r6 = uint64_t(int64_t(977));
  // b 0x8226c83c
  goto loc_8226C83C;
loc_8226C7EC:
  // lwz r11,9700(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9700);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r3,r29
  r3 = r29;
  // bne cr6,0x8226c80c
  if (!cr6.eq) goto loc_8226C80C;
  // li r6,979
  r6 = uint64_t(int64_t(979));
  // b 0x8226c848
  goto loc_8226C848;
loc_8226C80C:
  // li r6,980
  r6 = uint64_t(int64_t(980));
  // b 0x8226c848
  goto loc_8226C848;
loc_8226C814:
  // cmplwi cr6,r31,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 1, so);
  // blt cr6,0x8226c838
  if (cr6.lt) goto loc_8226C838;
  // beq cr6,0x8226c830
  if (cr6.eq) goto loc_8226C830;
  // cmplwi cr6,r31,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 3, so);
  // bge cr6,0x8226c84c
  if (!cr6.lt) goto loc_8226C84C;
  // li r6,952
  r6 = uint64_t(int64_t(952));
  // b 0x8226c83c
  goto loc_8226C83C;
loc_8226C830:
  // li r6,950
  r6 = uint64_t(int64_t(950));
  // b 0x8226c83c
  goto loc_8226C83C;
loc_8226C838:
  // li r6,949
  r6 = uint64_t(int64_t(949));
loc_8226C83C:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r29
  r3 = r29;
loc_8226C848:
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226C84C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226C84C:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x8226C85C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r29
  r3 = r29;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x8226C878, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x8226C88C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226C898: lifted from generated/ by lift.py.
lifted::Result f8226C898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226C898 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r27,r4
  r27 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226C8C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226c9b0
  if (!cr6.eq) goto loc_8226C9B0;
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // li r28,168
  r28 = uint64_t(int64_t(168));
  // beq cr6,0x8226c8ec
  if (cr6.eq) goto loc_8226C8EC;
  // li r28,172
  r28 = uint64_t(int64_t(172));
loc_8226C8EC:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8226c90c
  if (cr6.lt) goto loc_8226C90C;
  // bne cr6,0x8226c920
  if (!cr6.eq) goto loc_8226C920;
  // lwz r6,9692(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 9692);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x8226c914
  goto loc_8226C914;
loc_8226C90C:
  // lwz r6,9688(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 9688);
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_8226C914:
  // mr r5,r6
  r5 = r6;
  // mr r3,r31
  r3 = r31;
  // bl 0x8226c3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C3B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226C920:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8226c990
  if (!cr6.eq) goto loc_8226C990;
  // li r4,179
  r4 = uint64_t(int64_t(179));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226C93C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,180
  r4 = uint64_t(int64_t(180));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226C948, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9700(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9700);
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
  // addi r4,r11,179
  r4 = uint64_t(int64_t(int64_t(r11) + 179));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226C968, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9700(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9700);
  // mr r3,r30
  r3 = r30;
  // addi r4,r11,179
  r4 = uint64_t(int64_t(int64_t(r11) + 179));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226C978, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8226c798
  // (left out: dead in C++)
  { const lifted::Result o = f8226C798(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8226c9a8
  goto loc_8226C9A8;
loc_8226C990:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // add r4,r11,r28
  r4 = r11 + r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226C9A8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226C9A8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r11));
loc_8226C9B0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226C9BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bne cr6,0x8226c9d8
  if (!cr6.eq) goto loc_8226C9D8;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_8226C9D8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226C9E0: lifted from generated/ by lift.py.
lifted::Result f8226C9E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8226C9E0 (lifted)");
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r30,r4
  r30 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226CA10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r27,0(r3)
  r27 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226cb24
  if (!cr6.eq) goto loc_8226CB24;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r10,24852(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24852);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x8226ca98
  if (!cr6.eq) goto loc_8226CA98;
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8226ca6c
  if (cr6.lt) goto loc_8226CA6C;
  // beq cr6,0x8226ca60
  if (cr6.eq) goto loc_8226CA60;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x8226ca80
  if (!cr6.lt) goto loc_8226CA80;
  // lwz r6,9692(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 9692);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x8226ca74
  goto loc_8226CA74;
loc_8226CA60:
  // lwz r6,9688(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 9688);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x8226ca74
  goto loc_8226CA74;
loc_8226CA6C:
  // lwz r6,9684(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 9684);
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_8226CA74:
  // mr r5,r6
  r5 = r6;
  // mr r3,r31
  r3 = r31;
  // bl 0x8226c230
  // (left out: dead in C++)
  { const lifted::Result o = f8226C230(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226CA80:
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // bne cr6,0x8226ca90
  if (!cr6.eq) goto loc_8226CA90;
  // li r11,50
  r11 = uint64_t(int64_t(50));
  // b 0x8226caf8
  goto loc_8226CAF8;
loc_8226CA90:
  // li r11,54
  r11 = uint64_t(int64_t(54));
  // b 0x8226caf8
  goto loc_8226CAF8;
loc_8226CA98:
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x8226caf8
  if (!cr6.eq) goto loc_8226CAF8;
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8226cad4
  if (cr6.lt) goto loc_8226CAD4;
  // beq cr6,0x8226cac8
  if (cr6.eq) goto loc_8226CAC8;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x8226cae8
  if (!cr6.lt) goto loc_8226CAE8;
  // lwz r6,9696(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 9696);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x8226cadc
  goto loc_8226CADC;
loc_8226CAC8:
  // lwz r6,9692(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 9692);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x8226cadc
  goto loc_8226CADC;
loc_8226CAD4:
  // lwz r6,9688(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 9688);
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_8226CADC:
  // mr r5,r6
  r5 = r6;
  // mr r3,r31
  r3 = r31;
  // bl 0x8226c3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C3B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226CAE8:
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // li r11,109
  r11 = uint64_t(int64_t(109));
  // beq cr6,0x8226caf8
  if (cr6.eq) goto loc_8226CAF8;
  // li r11,113
  r11 = uint64_t(int64_t(113));
loc_8226CAF8:
  // lwz r10,24932(r29)
  r10 = guest::LoadU32(base, uint32_t(r29) + 24932);
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
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r10)).cursor_address());
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226CB1C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_8226CB24:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226CB30, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226CB40: lifted from generated/ by lift.py.
lifted::Result f8226CB40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226CB40 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r3
  r30 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // stw r31,9640(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9640_address(), uint32_t(r31));
  // stw r29,9644(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9644_address(), uint32_t(r29));
  // stw r31,9648(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9648_address(), uint32_t(r31));
  // stw r29,9652(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9652_address(), uint32_t(r29));
  // stw r31,9700(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9700_address(), uint32_t(r31));
  // stw r31,9704(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9704_address(), uint32_t(r31));
  // stw r31,9712(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9712_address(), uint32_t(r31));
  // stw r31,9716(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9716_address(), uint32_t(r31));
  // stw r29,9724(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9724_address(), uint32_t(r29));
  // stw r31,9624(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).back_address(), uint32_t(r31));
  // stw r31,9628(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).race_address(), uint32_t(r31));
  // stw r31,9636(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).interrupted_address(), uint32_t(r31));
  // stw r31,9728(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9728_address(), uint32_t(r31));
  // stw r31,9656(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9656_address(), uint32_t(r31));
  // stw r31,9660(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9660_address(), uint32_t(r31));
  // stw r31,9664(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9664_address(), uint32_t(r31));
  // stw r31,9668(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9668_address(), uint32_t(r31));
  // stw r31,9672(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9672_address(), uint32_t(r31));
  // stw r31,9676(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9676_address(), uint32_t(r31));
  // stw r31,9680(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9680_address(), uint32_t(r31));
  // stw r31,9684(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9684_address(), uint32_t(r31));
  // stw r31,9688(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address(), uint32_t(r31));
  // stw r31,9692(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address(), uint32_t(r31));
  // stw r31,9696(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9696_address(), uint32_t(r31));
  // stw r31,9708(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9708_address(), uint32_t(r31));
  // bl 0x82269898
  // (left out: dead in C++)
  r3 = f82269898(base, r1, r13);
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // lwz r11,24860(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 24860);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226ccc0
  if (!cr6.eq) goto loc_8226CCC0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24960);
  // addi r28,r11,832
  r28 = uint64_t(int64_t(int64_t(r11) + 832));
  // mr r3,r28
  r3 = r28;
  // bl 0x82110448
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageCar::Reset(base, LIFTED_ARGS(0x8226CBE4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x821104d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::SetModelAndClass(base, LIFTED_ARGS(0x8226CBF0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stw r29,9664(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9664_address(), uint32_t(r29));
  // li r9,210
  r9 = uint64_t(int64_t(210));
  // stw r31,9668(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9668_address(), uint32_t(r31));
  // li r8,221
  r8 = uint64_t(int64_t(221));
  // stw r11,9656(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9656_address(), uint32_t(r11));
  // li r11,220
  r11 = uint64_t(int64_t(220));
  // stw r10,9660(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9660_address(), uint32_t(r10));
  // stw r9,9672(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9672_address(), uint32_t(r9));
  // stw r8,9676(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9676_address(), uint32_t(r8));
  // stw r11,9680(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9680_address(), uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x8226cc38
  if (cr6.eq) goto loc_8226CC38;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226CC38:
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r10));
  // stw r29,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r29));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
  // li r3,6
  r3 = uint64_t(int64_t(6));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
  // mr r11,r31
  r11 = r31;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24860(r27)
  guest::StoreU32(base, uint32_t(r27) + 24860, uint32_t(r11));
  // stw r11,24852(r10)
  guest::StoreU32(base, uint32_t(r10) + 24852, uint32_t(r11));
  // b 0x8226d090
  goto loc_8226D090;
loc_8226CCC0:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // addi r11,r11,-4096
  r11 = uint64_t(int64_t(int64_t(r11) + -4096));
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // bgt cr6,0x8226ceec
  if (cr6.gt) goto loc_8226CEEC;
  // lwz r11,9708(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9708_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r28,r11,30752
  r28 = uint64_t(int64_t(int64_t(r11) + 30752));
  // beq cr6,0x8226ccf4
  if (cr6.eq) goto loc_8226CCF4;
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226cd08
  if (!cr6.eq) goto loc_8226CD08;
loc_8226CCF4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
  // li r3,6
  r3 = uint64_t(int64_t(6));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(base, r3);
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 4);
loc_8226CD08:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226cddc
  if (!cr6.eq) goto loc_8226CDDC;
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r29,9664(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9664_address(), uint32_t(r29));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stw r31,9668(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9668_address(), uint32_t(r31));
  // li r9,210
  r9 = uint64_t(int64_t(210));
  // li r8,221
  r8 = uint64_t(int64_t(221));
  // stw r11,9656(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9656_address(), uint32_t(r11));
  // li r11,220
  r11 = uint64_t(int64_t(220));
  // stw r10,9660(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9660_address(), uint32_t(r10));
  // stw r9,9672(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9672_address(), uint32_t(r9));
  // stw r8,9676(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9676_address(), uint32_t(r8));
  // stw r11,9680(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9680_address(), uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226cd58
  if (cr6.eq) goto loc_8226CD58;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226CD58:
  // li r9,14
  r9 = uint64_t(int64_t(14));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r10));
  // stw r8,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // lwz r11,24932(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r10));
  // stw r29,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r29));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // lwz r11,9720(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9720_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lwz r11,24932(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // bne cr6,0x8226cee4
  if (!cr6.eq) goto loc_8226CEE4;
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r9));
  // b 0x8226d090
  goto loc_8226D090;
loc_8226CDDC:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r3,23680(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 23680);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).stop_cue_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226CE04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,3
  r28 = uint64_t(int64_t(3));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r29,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r29));
  // addi r3,r1,92
  r3 = uint64_t(int64_t(int64_t(r1) + 92));
  // stw r28,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r28));
  // bl 0x82175850
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Play(base, LIFTED_ARGS(0x8226CE1C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // stw r28,9656(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9656_address(), uint32_t(r28));
  // li r10,210
  r10 = uint64_t(int64_t(210));
  // stw r29,9664(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9664_address(), uint32_t(r29));
  // li r9,221
  r9 = uint64_t(int64_t(221));
  // stw r31,9668(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9668_address(), uint32_t(r31));
  // li r8,220
  r8 = uint64_t(int64_t(220));
  // stw r11,9660(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9660_address(), uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // stw r10,9672(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9672_address(), uint32_t(r10));
  // stw r9,9676(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9676_address(), uint32_t(r9));
  // stw r8,9680(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9680_address(), uint32_t(r8));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x8226ce60
  if (cr6.eq) goto loc_8226CE60;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226CE60:
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // lwz r11,24932(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r6,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r6));
  // stw r29,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r29));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // lwz r11,9720(r30)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9720_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lwz r11,24932(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // bne cr6,0x8226cee4
  if (!cr6.eq) goto loc_8226CEE4;
  // stw r29,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r29));
  // b 0x8226d090
  goto loc_8226D090;
loc_8226CEE4:
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // b 0x8226d090
  goto loc_8226D090;
loc_8226CEEC:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24852(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24852);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226cfbc
  if (cr6.eq) goto loc_8226CFBC;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8226d090
  if (!cr6.eq) goto loc_8226D090;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r31,9664(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9664_address(), uint32_t(r31));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r31,9668(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9668_address(), uint32_t(r31));
  // li r9,118
  r9 = uint64_t(int64_t(118));
  // li r8,77
  r8 = uint64_t(int64_t(77));
  // stw r11,9656(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9656_address(), uint32_t(r11));
  // li r11,76
  r11 = uint64_t(int64_t(76));
  // stw r10,9660(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9660_address(), uint32_t(r10));
  // stw r9,9672(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9672_address(), uint32_t(r9));
  // stw r8,9676(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9676_address(), uint32_t(r8));
  // stw r11,9680(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9680_address(), uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8226cf4c
  if (cr6.eq) goto loc_8226CF4C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226CF4C:
  // li r10,25
  r10 = uint64_t(int64_t(25));
  // stw r29,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r29));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r10));
  // stw r29,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r29));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // stw r9,9688(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address(), uint32_t(r9));
  // stw r29,9692(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address(), uint32_t(r29));
  // stw r31,9696(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9696_address(), uint32_t(r31));
  // b 0x8226d090
  goto loc_8226D090;
loc_8226CFBC:
  // li r11,59
  r11 = uint64_t(int64_t(59));
  // stw r31,9664(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9664_address(), uint32_t(r31));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r31,9668(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9668_address(), uint32_t(r31));
  // li r9,18
  r9 = uint64_t(int64_t(18));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r11,9672(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9672_address(), uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // stw r10,9656(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9656_address(), uint32_t(r10));
  // stw r10,9660(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9660_address(), uint32_t(r10));
  // stw r9,9676(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9676_address(), uint32_t(r9));
  // stw r8,9680(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9680_address(), uint32_t(r8));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8226d000
  if (cr0.eq) goto loc_8226D000;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226D000:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r31,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r31));
  // stw r29,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r29));
  // li r28,3
  r28 = uint64_t(int64_t(3));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r31));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,24932(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r28,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r28));
  // stw r29,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r29));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,24932(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // bne cr6,0x8226d07c
  if (!cr6.eq) goto loc_8226D07C;
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // b 0x8226d080
  goto loc_8226D080;
loc_8226D07C:
  // stw r29,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r29));
loc_8226D080:
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r31,9684(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9684_address(), uint32_t(r31));
  // stw r28,9692(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9692_address(), uint32_t(r28));
  // stw r11,9688(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9688_address(), uint32_t(r11));
loc_8226D090:
  // stw r31,9720(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).unknown_9720_address(), uint32_t(r31));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226D0A0: lifted from generated/ by lift.py.
lifted::Result f8226D0A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8226D0A0 (lifted)");
  uint64_t r0 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lwz r11,9624(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9624);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226d784
  if (cr6.eq) goto loc_8226D784;
  // lwz r11,9632(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9632);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226d784
  if (cr6.eq) goto loc_8226D784;
  // lwz r11,9628(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9628);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226d784
  if (cr6.eq) goto loc_8226D784;
  // lwz r11,9636(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9636);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226d784
  if (cr6.eq) goto loc_8226D784;
  // lwz r11,9652(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9652);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226d60c
  if (!cr6.eq) goto loc_8226D60C;
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // addi r10,r10,-3
  r10 = uint64_t(int64_t(int64_t(r10) + -3));
  // cmplwi cr6,r10,14
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 14, so);
  // bgt cr6,0x8226d778
  if (cr6.gt) goto loc_8226D778;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6216
  r12 = uint64_t(int64_t(int64_t(r12) + 6216));
  // rlwinm r0,r10,1,0,30
  r0 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // lhzx r0,r12,r0
  r0 = guest::LoadU16(base, uint32_t(r12) + uint32_t(r0));
  // lis r12,-32217
  r12 = uint64_t(int64_t(-2111373312));
  // addi r12,r12,-11980
  r12 = uint64_t(int64_t(int64_t(r12) + -11980));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r10)) {
  case 0:
  goto loc_8226D300;
  case 1:
  goto loc_8226D3B8;
  case 2:
  goto loc_8226D3B8;
  case 3:
  goto loc_8226D778;
  case 4:
  goto loc_8226D778;
  case 5:
  goto loc_frag_8226D134;
  return {r3, f1};
  case 6:
  goto loc_8226D218;
  case 7:
  goto loc_8226D318;
  case 8:
  goto loc_8226D778;
  case 9:
  goto loc_8226D778;
  case 10:
  goto loc_8226D778;
  case 11:
  goto loc_8226D400;
  case 12:
  goto loc_8226D42C;
  case 13:
  goto loc_8226D524;
  case 14:
  goto loc_8226D544;
  default:
  std::abort();
  }
loc_8226D210:
  // stw r31,9712(r30)
  guest::StoreU32(base, uint32_t(r30) + 9712, uint32_t(r31));
  // b 0x8226d778
  goto loc_8226D778;
loc_8226D218:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r27,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r27));
  // stw r27,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r27));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r3,23680(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 23680);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).stop_cue_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226D240, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r27,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r27));
  // addi r3,r1,92
  r3 = uint64_t(int64_t(int64_t(r1) + 92));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // bl 0x82175850
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Play(base, LIFTED_ARGS(0x8226D258, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r31,9708(r30)
  guest::StoreU32(base, uint32_t(r30) + 9708, uint32_t(r31));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stw r27,9664(r30)
  guest::StoreU32(base, uint32_t(r30) + 9664, uint32_t(r27));
  // li r9,210
  r9 = uint64_t(int64_t(210));
  // stw r31,9668(r30)
  guest::StoreU32(base, uint32_t(r30) + 9668, uint32_t(r31));
  // li r8,221
  r8 = uint64_t(int64_t(221));
  // stw r11,9656(r30)
  guest::StoreU32(base, uint32_t(r30) + 9656, uint32_t(r11));
  // li r11,220
  r11 = uint64_t(int64_t(220));
  // stw r10,9660(r30)
  guest::StoreU32(base, uint32_t(r30) + 9660, uint32_t(r10));
  // stw r9,9672(r30)
  guest::StoreU32(base, uint32_t(r30) + 9672, uint32_t(r9));
  // stw r8,9676(r30)
  guest::StoreU32(base, uint32_t(r30) + 9676, uint32_t(r8));
  // stw r11,9680(r30)
  guest::StoreU32(base, uint32_t(r30) + 9680, uint32_t(r11));
  // lwz r11,24928(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226d2a0
  if (cr6.eq) goto loc_8226D2A0;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226D2A0:
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r8));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r10));
loc_8226D2B8:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r27,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r27));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // b 0x8226d774
  goto loc_8226D774;
loc_8226D300:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x8226d310
  if (cr6.eq) goto loc_8226D310;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r10));
loc_8226D310:
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // b 0x8226d768
  goto loc_8226D768;
loc_8226D318:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r27,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r27));
  // stw r27,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r27));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,100
  r4 = uint64_t(int64_t(int64_t(r1) + 100));
  // lwz r3,23680(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 23680);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).stop_cue_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226D340, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,3
  r28 = uint64_t(int64_t(3));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // stw r27,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r27));
  // addi r3,r1,108
  r3 = uint64_t(int64_t(int64_t(r1) + 108));
  // stw r28,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r28));
  // bl 0x82175850
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Play(base, LIFTED_ARGS(0x8226D358, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // stw r28,9656(r30)
  guest::StoreU32(base, uint32_t(r30) + 9656, uint32_t(r28));
  // li r10,210
  r10 = uint64_t(int64_t(210));
  // stw r27,9664(r30)
  guest::StoreU32(base, uint32_t(r30) + 9664, uint32_t(r27));
  // li r9,221
  r9 = uint64_t(int64_t(221));
  // stw r31,9668(r30)
  guest::StoreU32(base, uint32_t(r30) + 9668, uint32_t(r31));
  // li r8,220
  r8 = uint64_t(int64_t(220));
  // stw r11,9660(r30)
  guest::StoreU32(base, uint32_t(r30) + 9660, uint32_t(r11));
  // stw r10,9672(r30)
  guest::StoreU32(base, uint32_t(r30) + 9672, uint32_t(r10));
  // stw r9,9676(r30)
  guest::StoreU32(base, uint32_t(r30) + 9676, uint32_t(r9));
  // stw r8,9680(r30)
  guest::StoreU32(base, uint32_t(r30) + 9680, uint32_t(r8));
  // lwz r11,24928(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x8226d398
  if (cr6.eq) goto loc_8226D398;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226D398:
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // b 0x8226d2b8
  goto loc_8226D2B8;
loc_8226D3B8:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r27,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r27));
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,116
  r4 = uint64_t(int64_t(int64_t(r1) + 116));
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23680(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 23680);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AudioManager(base, uint32_t(r3)).vtable_address());
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::AudioManagerVtable(base, uint32_t(r11)).stop_cue_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226D3E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,120
  r4 = uint64_t(int64_t(int64_t(r1) + 120));
  // addi r3,r1,124
  r3 = uint64_t(int64_t(int64_t(r1) + 124));
  // stw r27,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r27));
  // stw r27,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r27));
  // bl 0x82175850
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Play(base, LIFTED_ARGS(0x8226D3F8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24928);
loc_8226D400:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x8226d410
  if (cr6.eq) goto loc_8226D410;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226D410:
  // li r9,28
  r9 = uint64_t(int64_t(28));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // b 0x8226d774
  goto loc_8226D774;
loc_8226D42C:
  // lwz r10,9712(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 9712);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226d45c
  if (!cr6.eq) goto loc_8226D45C;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8226d448
  if (cr0.eq) goto loc_8226D448;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226D448:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r31));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // b 0x8226d210
  goto loc_8226D210;
loc_8226D45C:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r31,9664(r30)
  guest::StoreU32(base, uint32_t(r30) + 9664, uint32_t(r31));
  // li r11,59
  r11 = uint64_t(int64_t(59));
  // stw r31,9668(r30)
  guest::StoreU32(base, uint32_t(r30) + 9668, uint32_t(r31));
  // li r9,18
  r9 = uint64_t(int64_t(18));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r10,9656(r30)
  guest::StoreU32(base, uint32_t(r30) + 9656, uint32_t(r10));
  // stw r10,9660(r30)
  guest::StoreU32(base, uint32_t(r30) + 9660, uint32_t(r10));
  // stw r11,9672(r30)
  guest::StoreU32(base, uint32_t(r30) + 9672, uint32_t(r11));
  // stw r9,9676(r30)
  guest::StoreU32(base, uint32_t(r30) + 9676, uint32_t(r9));
  // stw r8,9680(r30)
  guest::StoreU32(base, uint32_t(r30) + 9680, uint32_t(r8));
  // lwz r11,24928(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8226d49c
  if (cr0.eq) goto loc_8226D49C;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226D49C:
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // stw r27,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r27));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r31));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // lwz r11,24932(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r8,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r8));
  // stw r27,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r27));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,24932(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // bne cr6,0x8226d51c
  if (!cr6.eq) goto loc_8226D51C;
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // b 0x8226d210
  goto loc_8226D210;
loc_8226D51C:
  // stw r27,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r27));
  // b 0x8226d210
  goto loc_8226D210;
loc_8226D524:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8226d534
  if (cr6.eq) goto loc_8226D534;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r10));
loc_8226D534:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r27,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r27));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r10));
  // b 0x8226d774
  goto loc_8226D774;
loc_8226D544:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r31,9664(r30)
  guest::StoreU32(base, uint32_t(r30) + 9664, uint32_t(r31));
  // li r11,59
  r11 = uint64_t(int64_t(59));
  // stw r31,9668(r30)
  guest::StoreU32(base, uint32_t(r30) + 9668, uint32_t(r31));
  // li r9,18
  r9 = uint64_t(int64_t(18));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r10,9656(r30)
  guest::StoreU32(base, uint32_t(r30) + 9656, uint32_t(r10));
  // stw r10,9660(r30)
  guest::StoreU32(base, uint32_t(r30) + 9660, uint32_t(r10));
  // stw r11,9672(r30)
  guest::StoreU32(base, uint32_t(r30) + 9672, uint32_t(r11));
  // stw r9,9676(r30)
  guest::StoreU32(base, uint32_t(r30) + 9676, uint32_t(r9));
  // stw r8,9680(r30)
  guest::StoreU32(base, uint32_t(r30) + 9680, uint32_t(r8));
  // lwz r11,24928(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8226d584
  if (cr0.eq) goto loc_8226D584;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_8226D584:
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // stw r27,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r27));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r31));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // lwz r11,24932(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r8,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r8));
  // stw r27,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r27));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,24932(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r10));
  // bne cr6,0x8226d604
  if (!cr6.eq) goto loc_8226D604;
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // b 0x8226d778
  goto loc_8226D778;
loc_8226D604:
  // stw r27,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r27));
  // b 0x8226d778
  goto loc_8226D778;
loc_8226D60C:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // addi r10,r10,-3
  r10 = uint64_t(int64_t(int64_t(r10) + -3));
  // cmplwi cr6,r10,11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 11, so);
  // bgt cr6,0x8226d778
  if (cr6.gt) goto loc_8226D778;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6200
  r12 = uint64_t(int64_t(int64_t(r12) + 6200));
  // lbzx r0,r12,r10
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r10));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32217
  r12 = uint64_t(int64_t(-2111373312));
  // addi r12,r12,-10676
  r12 = uint64_t(int64_t(int64_t(r12) + -10676));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r10)) {
  case 0:
  goto loc_8226D6B8;
  case 1:
  goto loc_frag_8226D64C;
  return {r3, f1};
  case 2:
  goto loc_8226D664;
  case 3:
  goto loc_8226D778;
  case 4:
  goto loc_8226D688;
  case 5:
  goto loc_8226D670;
  case 6:
  goto loc_8226D688;
  case 7:
  goto loc_8226D6A0;
  case 8:
  goto loc_8226D778;
  case 9:
  goto loc_8226D778;
  case 10:
  goto loc_8226D778;
  case 11:
  goto loc_8226D754;
  default:
  std::abort();
  }
loc_8226D664:
  // mr r3,r30
  r3 = r30;
  // bl 0x82270bf8
  // (left out: dead in C++)
  r3 = f82270BF8(base, r1, r13);
  // b 0x8226d778
  goto loc_8226D778;
loc_8226D670:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,15
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 15, so);
  // beq cr6,0x8226d680
  if (cr6.eq) goto loc_8226D680;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8226D680:
  // li r9,15
  r9 = uint64_t(int64_t(15));
  // b 0x8226d768
  goto loc_8226D768;
loc_8226D688:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,16
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 16, so);
  // beq cr6,0x8226d698
  if (cr6.eq) goto loc_8226D698;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8226D698:
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // b 0x8226d768
  goto loc_8226D768;
loc_8226D6A0:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x8226d6b0
  if (cr6.eq) goto loc_8226D6B0;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8226D6B0:
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // b 0x8226d768
  goto loc_8226D768;
loc_8226D6B8:
  // lwz r11,9704(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9704);
  // addi r9,r11,-1
  r9 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r10,r11,24932
  r10 = uint64_t(int64_t(int64_t(r11) + 24932));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r31,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r31));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r9));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // ble cr6,0x8226d710
  if (!cr6.gt) goto loc_8226D710;
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // stw r31,44(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).list_wrapped_address(), uint32_t(r31));
  // stw r31,56(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_row_address(), uint32_t(r31));
  // stw r8,60(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).rows_shown_address(), uint32_t(r8));
  // b 0x8226d720
  goto loc_8226D720;
loc_8226D710:
  // stw r31,40(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
loc_8226D720:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r9,24896(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 24896);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // stw r9,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address(), uint32_t(r8));
  // lwz r9,9640(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 9640);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r9,48(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address(), uint32_t(r9));
  // lwz r9,9640(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 9640);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r9,52(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address(), uint32_t(r9));
  // b 0x8226d778
  goto loc_8226D778;
loc_8226D754:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x8226d764
  if (cr6.eq) goto loc_8226D764;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address(), uint32_t(r10));
loc_8226D764:
  // li r9,19
  r9 = uint64_t(int64_t(19));
loc_8226D768:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address(), uint32_t(r9));
loc_8226D774:
  // stw r10,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r10));
loc_8226D778:
  // stw r27,9644(r30)
  guest::StoreU32(base, uint32_t(r30) + 9644, uint32_t(r27));
  // stw r31,9648(r30)
  guest::StoreU32(base, uint32_t(r30) + 9648, uint32_t(r31));
  // b 0x8226d7a8
  goto loc_8226D7A8;
loc_8226D784:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24928);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x8226d7a8
  if (!cr6.eq) goto loc_8226D7A8;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,9644(r30)
  guest::StoreU32(base, uint32_t(r30) + 9644, uint32_t(r11));
  // stw r10,9648(r30)
  guest::StoreU32(base, uint32_t(r30) + 9648, uint32_t(r10));
loc_8226D7A8:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_frag_8226D64C:
  // lwz r11,9708(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9708);
  // mr r3,r30
  r3 = r30;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226d668
  if (cr6.eq) goto loc_8226D668_8226D64C;
  // bl 0x82270a80
  // (left out: dead in C++)
  r3 = f82270A80(base, r1, r13);
  // b 0x8226d778
  goto loc_8226D778_8226D64C;
loc_8226D668_8226D64C:
  // bl 0x82270bf8
  // (left out: dead in C++)
  r3 = f82270BF8(base, r1, r13);
  // b 0x8226d778
  goto loc_8226D778_8226D64C;
loc_8226D778_8226D64C:
  // stw r27,9644(r30)
  guest::StoreU32(base, uint32_t(r30) + 9644, uint32_t(r27));
  // stw r31,9648(r30)
  guest::StoreU32(base, uint32_t(r30) + 9648, uint32_t(r31));
  // b 0x8226d7a8
  goto loc_8226D7A8_8226D64C;
loc_8226D7A8_8226D64C:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_frag_8226D134:
  // lwz r10,9712(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 9712);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bne cr6,0x8226d174
  if (!cr6.eq) goto loc_8226D174_8226D134;
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x8226d150
  if (cr6.eq) goto loc_8226D150_8226D134;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8226D150_8226D134:
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // b 0x8226d1ac
  goto loc_8226D1AC_8226D134;
loc_8226D174_8226D134:
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x8226d180
  if (cr6.eq) goto loc_8226D180_8226D134;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8226D180_8226D134:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // bl 0x8226b830
  // (left out: dead in C++)
  { const lifted::Result o = f8226B830(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8226b940
  // (left out: dead in C++)
  { const lifted::Result o = f8226B940(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226D1AC_8226D134:
  // lwz r11,9704(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9704);
  // addi r10,r11,-1
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24932);
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // stw r31,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r31));
  // stw r31,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r31));
  // ble cr6,0x8226d200
  if (!cr6.gt) goto loc_8226D200_8226D134;
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // stw r31,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r31));
  // stw r31,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r31));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // b 0x8226d210
  goto loc_8226D210_8226D134;
loc_8226D200_8226D134:
  // stw r31,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r31));
loc_8226D210_8226D134:
  // stw r31,9712(r30)
  guest::StoreU32(base, uint32_t(r30) + 9712, uint32_t(r31));
  // b 0x8226d778
  goto loc_8226D778_8226D134;
loc_8226D778_8226D134:
  // stw r27,9644(r30)
  guest::StoreU32(base, uint32_t(r30) + 9644, uint32_t(r27));
  // stw r31,9648(r30)
  guest::StoreU32(base, uint32_t(r30) + 9648, uint32_t(r31));
  // b 0x8226d7a8
  goto loc_8226D7A8_8226D134;
loc_8226D7A8_8226D134:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226D7B0: lifted from generated/ by lift.py.
lifted::Result f8226D7B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8226D7B0 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r27,r4
  r27 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226D7E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226da94
  if (!cr6.eq) goto loc_8226DA94;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a130
  // (left out: dead in C++)
  r3 = game::UiMovie::ResetNodeFields(base, r1, r13, r3, r4, r5);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226d88c
  if (!cr6.eq) goto loc_8226D88C;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D824, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // mr r11,r3
  r11 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lfs f31,6316(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 6316);
  f31 = double(temp.f32);
  // stfs f31,16(r11)
  temp.f32 = float(f31);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D844, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // stfs f31,16(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D85C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // mr r11,r3
  r11 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lfs f0,6312(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 6312);
  f0 = double(temp.f32);
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D87C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226D884, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,80(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).visible_address(), uint32_t(r28));
  // b 0x8226d8f8
  goto loc_8226D8F8;
loc_8226D88C:
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D890, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // mr r11,r3
  r11 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lfs f31,6308(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 6308);
  f31 = double(temp.f32);
  // stfs f31,16(r11)
  temp.f32 = float(f31);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D8B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // stfs f31,16(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D8C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // mr r11,r3
  r11 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lfs f0,6304(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 6304);
  f0 = double(temp.f32);
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).live_scene_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D8E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226D8F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,80(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).visible_address(), uint32_t(r11));
loc_8226D8F8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D908, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226D914, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,318
  r5 = uint64_t(int64_t(318));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226D924, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226D930, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,320
  r5 = uint64_t(int64_t(320));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226D940, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226D94C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,321
  r5 = uint64_t(int64_t(321));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226D95C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,314
  r6 = uint64_t(int64_t(314));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226D970, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8226c6e0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C6E0(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r6,9684(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 9684);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // mr r5,r6
  r5 = r6;
  // bl 0x8226c230
  // (left out: dead in C++)
  { const lifted::Result o = f8226C230(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r6,9688(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 9688);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // mr r5,r6
  r5 = r6;
  // bl 0x8226c230
  // (left out: dead in C++)
  { const lifted::Result o = f8226C230(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r6,9692(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 9692);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // mr r5,r6
  r5 = r6;
  // bl 0x8226c230
  // (left out: dead in C++)
  { const lifted::Result o = f8226C230(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82270a20
  // (left out: dead in C++)
  r3 = f82270A20(base, r1, r13);
  // mr r3,r31
  r3 = r31;
  // lwz r5,9664(r30)
  r5 = guest::LoadU32(base, uint32_t(r30) + 9664);
  // lwz r4,9660(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226D9D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r4,9668(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 9668);
  // lwz r29,0(r11)
  r29 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226D9E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).y_address(), temp.u32);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226DA24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226DA30, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,255
  r4 = uint64_t(int64_t(255));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226DA4C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,255
  r4 = uint64_t(int64_t(255));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226DA58, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,256
  r4 = uint64_t(int64_t(256));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226DA74, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8226DA90, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r28));
loc_8226DA94:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226DAA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
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
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-56(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226DAD8: lifted from generated/ by lift.py.
lifted::Result f8226DAD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8226DAD8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r27,r4
  r27 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226DB04, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lwz r29,0(r3)
  r29 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226dbfc
  if (!cr6.eq) goto loc_8226DBFC;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x82270848
  // (left out: dead in C++)
  { const lifted::Result o = f82270848(base, r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r29
  r3 = r29;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226DB4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // mr r3,r29
  r3 = r29;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226DB64, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r10,9656(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9656);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8226dbbc
  if (!cr6.eq) goto loc_8226DBBC;
  // mr r3,r29
  r3 = r29;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226DB8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9668);
  // lwz r28,0(r11)
  r28 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DB9C, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r28,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(40))));
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
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).y_address(), temp.u32);
  // b 0x8226dbc8
  goto loc_8226DBC8;
loc_8226DBBC:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x822708c0
  // (left out: dead in C++)
  r3 = f822708C0(base, r1, r13, r3, r4);
loc_8226DBC8:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24852(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24852);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bne cr6,0x8226dbf0
  if (!cr6.eq) goto loc_8226DBF0;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x8226c798
  // (left out: dead in C++)
  { const lifted::Result o = f8226C798(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8226dbf4
  goto loc_8226DBF4;
loc_8226DBF0:
  // bl 0x8226c6e0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C6E0(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226DBF4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r11));
loc_8226DBFC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226DC08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226DC18: lifted from generated/ by lift.py.
lifted::Result f8226DC18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8226DC18 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r30,r4
  r30 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226DC44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226dd84
  if (!cr6.eq) goto loc_8226DD84;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x82270848
  // (left out: dead in C++)
  { const lifted::Result o = f82270848(base, r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9656);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addic. r11,r11,1
  ca = uint32_t(r11) > 4294967294;
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8226dc94
  if (cr0.eq) goto loc_8226DC94;
loc_8226DC70:
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // mr r3,r30
  r3 = r30;
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226DC80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9656);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8226dc70
  if (cr6.lt) goto loc_8226DC70;
loc_8226DC94:
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // mr r3,r30
  r3 = r30;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x8226dcec
  if (!cr6.eq) goto loc_8226DCEC;
  // lwz r10,9700(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9700);
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226DCD0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r9,9672(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // lwz r8,9700(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 9700);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r11,r9,r8
  r11 = r9 + r8;
  // add r4,r11,r10
  r4 = r11 + r10;
  // b 0x8226dd04
  goto loc_8226DD04;
loc_8226DCEC:
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226DCF4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r10,r11
  r4 = r10 + r11;
loc_8226DD04:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226DD0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r10,9656(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9656);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8226dd64
  if (!cr6.eq) goto loc_8226DD64;
  // mr r3,r30
  r3 = r30;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226DD34, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 9668);
  // lwz r28,0(r11)
  r28 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DD44, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r28,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(40))));
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
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).y_address(), temp.u32);
  // b 0x8226dd70
  goto loc_8226DD70;
loc_8226DD64:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822708c0
  // (left out: dead in C++)
  r3 = f822708C0(base, r1, r13, r3, r4);
loc_8226DD70:
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8226c798
  // (left out: dead in C++)
  { const lifted::Result o = f8226C798(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226DD84:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226DD90, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226DDA0: lifted from generated/ by lift.py.
lifted::Result f8226DDA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226DDA0 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r28,r4
  r28 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226DDCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226dfe0
  if (!cr6.eq) goto loc_8226DFE0;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226DDEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DDF8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,317
  r5 = uint64_t(int64_t(317));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226DE08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DE14, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,318
  r5 = uint64_t(int64_t(318));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226DE24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DE30, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,319
  r5 = uint64_t(int64_t(319));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226DE40, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226de9c
  if (!cr6.eq) goto loc_8226DE9C;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DE5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226DE6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DE78, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226DE88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DE94, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // b 0x8226dedc
  goto loc_8226DEDC;
loc_8226DE9C:
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DEA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226DEB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DEBC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226DECC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226DED8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
loc_8226DEDC:
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(base, LIFTED_ARGS(0x8226DEE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x82270cc8
  // (left out: dead in C++)
  r3 = f82270CC8(base, r1, r13);
  // li r6,948
  r6 = uint64_t(int64_t(948));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226DF04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x8226DF14, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x8226DF30, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x8226DF44, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r29
  r3 = r29;
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8226b830
  // (left out: dead in C++)
  { const lifted::Result o = f8226B830(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r29
  r3 = r29;
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8226b940
  // (left out: dead in C++)
  { const lifted::Result o = f8226B940(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9704(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9704);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // mr r3,r31
  r3 = r31;
  // ble cr6,0x8226df88
  if (!cr6.gt) goto loc_8226DF88;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8226DF84, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226df8c
  goto loc_8226DF8C;
loc_8226DF88:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226DF8C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226DF8C:
  // li r4,251
  r4 = uint64_t(int64_t(251));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8226DF98, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8226dfac
  if (!cr6.eq) goto loc_8226DFAC;
  // li r4,251
  r4 = uint64_t(int64_t(251));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226DFAC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226DFAC:
  // lwz r11,9672(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9672);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,3
  r4 = uint64_t(int64_t(int64_t(r11) + 3));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226DFBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,257
  r4 = uint64_t(int64_t(257));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226DFD8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_8226DFE0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226DFEC, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226E000: lifted from generated/ by lift.py.
lifted::Result f8226E000(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226E000 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r28,r4
  r28 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226E02C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e1ac
  if (!cr6.eq) goto loc_8226E1AC;
  // lwz r11,9704(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9704);
  // li r4,207
  r4 = uint64_t(int64_t(207));
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // ble cr6,0x8226e128
  if (!cr6.gt) goto loc_8226E128;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E054, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,206
  r4 = uint64_t(int64_t(206));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E060, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address());
  // addi r4,r11,199
  r4 = uint64_t(int64_t(int64_t(r11) + 199));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E078, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r29
  r3 = r29;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address());
  // subf r4,r11,r10
  r4 = r10 - r11;
  // bl 0x8226b830
  // (left out: dead in C++)
  { const lifted::Result o = f8226B830(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r29
  r3 = r29;
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8226b940
  // (left out: dead in C++)
  { const lifted::Result o = f8226B940(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
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
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address());
  // addi r4,r11,199
  r4 = uint64_t(int64_t(int64_t(r11) + 199));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E0C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address());
  // addi r4,r11,199
  r4 = uint64_t(int64_t(int64_t(r11) + 199));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226E0D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r10,52(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_row_address());
  // lwz r9,48(r11)
  r9 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).visible_row_address());
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // bne cr6,0x8226e1a4
  if (!cr6.eq) goto loc_8226E1A4;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x8226e100
  if (!cr6.eq) goto loc_8226E100;
  // li r4,207
  r4 = uint64_t(int64_t(207));
  // b 0x8226e10c
  goto loc_8226E10C;
loc_8226E100:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226e1a4
  if (!cr6.eq) goto loc_8226E1A4;
  // li r4,206
  r4 = uint64_t(int64_t(206));
loc_8226E10C:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E124, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226e1a4
  goto loc_8226E1A4;
loc_8226E128:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E12C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,206
  r4 = uint64_t(int64_t(206));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E138, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address());
  // addi r4,r11,199
  r4 = uint64_t(int64_t(int64_t(r11) + 199));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E150, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8226b830
  // (left out: dead in C++)
  { const lifted::Result o = f8226B830(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r29
  r3 = r29;
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8226b940
  // (left out: dead in C++)
  { const lifted::Result o = f8226B940(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
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
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // addi r4,r11,199
  r4 = uint64_t(int64_t(int64_t(r11) + 199));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E190, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // addi r4,r11,199
  r4 = uint64_t(int64_t(int64_t(r11) + 199));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226E1A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226E1A4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_8226E1AC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226E1B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226E1C8: lifted from generated/ by lift.py.
lifted::Result f8226E1C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8226E1C8 (lifted)");
  uint64_t r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r26,r4
  r26 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226E1F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // lwz r28,0(r3)
  r28 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e3a4
  if (!cr6.eq) goto loc_8226E3A4;
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24852(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 24852);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8226e260
  if (!cr6.eq) goto loc_8226E260;
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).previous_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bne cr6,0x8226e248
  if (!cr6.eq) goto loc_8226E248;
  // lwz r11,9708(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9708);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8226e25c
  if (cr6.eq) goto loc_8226E25C;
  // lwz r11,28180(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8226e25c
  if (cr6.eq) goto loc_8226E25C;
loc_8226E248:
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226e260
  if (!cr6.eq) goto loc_8226E260;
  // lwz r11,9716(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9716);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e260
  if (!cr6.eq) goto loc_8226E260;
loc_8226E25C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_8226E260:
  // mr r3,r31
  r3 = r31;
  // bl 0x82270848
  // (left out: dead in C++)
  { const lifted::Result o = f82270848(base, r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9708(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9708);
  // lwz r9,24852(r27)
  r9 = guest::LoadU32(base, uint32_t(r27) + 24852);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // beq cr6,0x8226e288
  if (cr6.eq) goto loc_8226E288;
  // lwz r10,28180(r29)
  r10 = guest::LoadU32(base, uint32_t(r29) + 28180);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8226e29c
  if (!cr6.eq) goto loc_8226E29C;
loc_8226E288:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bne cr6,0x8226e29c
  if (!cr6.eq) goto loc_8226E29C;
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // beq cr6,0x8226e2bc
  if (cr6.eq) goto loc_8226E2BC;
loc_8226E29C:
  // lwz r10,9716(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9716);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226e310
  if (!cr6.eq) goto loc_8226E310;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226e310
  if (!cr6.eq) goto loc_8226E310;
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // bne cr6,0x8226e310
  if (!cr6.eq) goto loc_8226E310;
loc_8226E2BC:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,234
  r4 = uint64_t(int64_t(234));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E2D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,234
  r4 = uint64_t(int64_t(234));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E2E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,235
  r4 = uint64_t(int64_t(235));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E300, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,235
  r4 = uint64_t(int64_t(235));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E30C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226e36c
  goto loc_8226E36C;
loc_8226E310:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r3,r28
  r3 = r28;
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8226E324, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8226e36c
  if (!cr0.eq) goto loc_8226E36C;
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E354, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9672);
  // mr r3,r28
  r3 = r28;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226E36C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226E36C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822708c0
  // (left out: dead in C++)
  r3 = f822708C0(base, r1, r13, r3, r4);
  // lwz r11,24852(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 24852);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bne cr6,0x8226e398
  if (!cr6.eq) goto loc_8226E398;
  // bl 0x8226c500
  // (left out: dead in C++)
  { const lifted::Result o = f8226C500(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8226e39c
  goto loc_8226E39C;
loc_8226E398:
  // bl 0x8226c618
  // (left out: dead in C++)
  { const lifted::Result o = f8226C618(base, r1, r13, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226E39C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r11));
loc_8226E3A4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226E3B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226E3C0: lifted from generated/ by lift.py.
lifted::Result f8226E3C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226E3C0 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r28,r4
  r28 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226E3EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e4c0
  if (!cr6.eq) goto loc_8226E4C0;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e41c
  if (!cr6.eq) goto loc_8226E41C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8226c500
  // (left out: dead in C++)
  { const lifted::Result o = f8226C500(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226E41C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226E42C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226E434, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,67
  r7 = uint64_t(int64_t(67));
  // li r6,235
  r6 = uint64_t(int64_t(235));
  // li r5,205
  r5 = uint64_t(int64_t(205));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226E44C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e490
  if (!cr6.eq) goto loc_8226E490;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r4,9672(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 9672);
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E478, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24932);
  // lwz r10,9672(r29)
  r10 = guest::LoadU32(base, uint32_t(r29) + 9672);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // add r4,r10,r11
  r4 = r10 + r11;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226E490, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226E490:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E4AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226E4B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_8226E4C0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226E4CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226E4D8: lifted from generated/ by lift.py.
lifted::Result f8226E4D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8226E4D8 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226E504, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e600
  if (!cr6.eq) goto loc_8226E600;
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // lwz r11,24932(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24932);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8226e538
  if (!cr6.eq) goto loc_8226E538;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8226c500
  // (left out: dead in C++)
  { const lifted::Result o = f8226C500(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 24932);
loc_8226E538:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e5a0
  if (!cr6.eq) goto loc_8226E5A0;
  // mr r3,r31
  r3 = r31;
  // lwz r4,9672(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 9672);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E550, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,234
  r4 = uint64_t(int64_t(234));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E56C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,234
  r4 = uint64_t(int64_t(234));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E578, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,235
  r4 = uint64_t(int64_t(235));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E594, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,235
  r4 = uint64_t(int64_t(235));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E5A0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226E5A0:
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226E5AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226E5BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226E5C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,179
  r7 = uint64_t(int64_t(179));
  // li r6,179
  r6 = uint64_t(int64_t(179));
  // li r5,179
  r5 = uint64_t(int64_t(179));
  // mr r3,r31
  r3 = r31;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x8226E5DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E5F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_8226E600:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226E60C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226E620: lifted from generated/ by lift.py.
lifted::Result f8226E620(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226E620 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // mr r26,r4
  r26 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226E64C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226e878
  if (!cr6.eq) goto loc_8226E878;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a130
  // (left out: dead in C++)
  r3 = game::UiMovie::ResetNodeFields(base, r1, r13, r3, r4, r5);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24848);
  // addi r11,r11,0
  r11 = uint64_t(int64_t(int64_t(r11) + 0));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r29,r11,1
  r29 = r11 ^ 1;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226E694, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226E6A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r28
  r3 = r28;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226E6B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r27
  r3 = r27;
  // stw r29,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r29));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226E6CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // stw r29,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226E6E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226E6F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,320
  r5 = uint64_t(int64_t(320));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226E700, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226E70C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,321
  r5 = uint64_t(int64_t(321));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226E71C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226E728, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,323
  r5 = uint64_t(int64_t(323));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8226E738, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8226E74C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8226c798
  // (left out: dead in C++)
  { const lifted::Result o = f8226C798(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r6,9688(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 9688);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // mr r5,r6
  r5 = r6;
  // bl 0x8226c3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C3B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r6,9692(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 9692);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // mr r5,r6
  r5 = r6;
  // bl 0x8226c3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C3B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r6,9696(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 9696);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // mr r5,r6
  r5 = r6;
  // bl 0x8226c3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C3B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82270a20
  // (left out: dead in C++)
  r3 = f82270A20(base, r1, r13);
  // mr r3,r31
  r3 = r31;
  // lwz r5,9664(r30)
  r5 = guest::LoadU32(base, uint32_t(r30) + 9664);
  // lwz r4,9660(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226E7B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24932);
  // lwz r4,9668(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 9668);
  // lwz r29,0(r11)
  r29 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8226E7C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).y_address(), temp.u32);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E804, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226E810, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,255
  r4 = uint64_t(int64_t(255));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E82C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,255
  r4 = uint64_t(int64_t(255));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226E838, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,261
  r4 = uint64_t(int64_t(261));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226E854, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8226E870, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r11));
loc_8226E878:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8226E884, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20700);
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
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

lifted::Result e82263840(uint8_t* base, const lifted::Args& a) {
  return f82263840(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82263920(uint8_t* base, const lifted::Args& a) {
  return {f82263920(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e822639C8(uint8_t* base, const lifted::Args& a) {
  return {f822639C8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82263A98(uint8_t* base, const lifted::Args& a) {
  return {f82263A98(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82263B68(uint8_t* base, const lifted::Args& a) {
  return f82263B68(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82263D80(uint8_t* base, const lifted::Args& a) {
  return {f82263D80(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82263E20(uint8_t* base, const lifted::Args& a) {
  return f82263E20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822642B8(uint8_t* base, const lifted::Args& a) {
  return f822642B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822643C0(uint8_t* base, const lifted::Args& a) {
  return f822643C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822645B0(uint8_t* base, const lifted::Args& a) {
  return f822645B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82264A38(uint8_t* base, const lifted::Args& a) {
  return f82264A38(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82264B90(uint8_t* base, const lifted::Args& a) {
  return f82264B90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4]);
}

lifted::Result e822651A0(uint8_t* base, const lifted::Args& a) {
  return f822651A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82265668(uint8_t* base, const lifted::Args& a) {
  return f82265668(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[7], a.f[0]);
}

lifted::Result e822657D8(uint8_t* base, const lifted::Args& a) {
  return f822657D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82265930(uint8_t* base, const lifted::Args& a) {
  return f82265930(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822659F0(uint8_t* base, const lifted::Args& a) {
  return f822659F0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82265C58(uint8_t* base, const lifted::Args& a) {
  f82265C58(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82265CA8(uint8_t* base, const lifted::Args& a) {
  return f82265CA8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82265CE8(uint8_t* base, const lifted::Args& a) {
  f82265CE8(base, a.r1, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82265E80(uint8_t* base, const lifted::Args& a) {
  return f82265E80(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82266028(uint8_t* base, const lifted::Args& a) {
  return f82266028(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822662F0(uint8_t* base, const lifted::Args& a) {
  return f822662F0(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e822663E0(uint8_t* base, const lifted::Args& a) {
  return f822663E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82266548(uint8_t* base, const lifted::Args& a) {
  return f82266548(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82266658(uint8_t* base, const lifted::Args& a) {
  return {f82266658(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82266750(uint8_t* base, const lifted::Args& a) {
  return f82266750(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82266908(uint8_t* base, const lifted::Args& a) {
  return f82266908(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82266AC0(uint8_t* base, const lifted::Args& a) {
  return f82266AC0(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82266C80(uint8_t* base, const lifted::Args& a) {
  return f82266C80(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82266E68(uint8_t* base, const lifted::Args& a) {
  return f82266E68(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822670D0(uint8_t* base, const lifted::Args& a) {
  return f822670D0(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82267210(uint8_t* base, const lifted::Args& a) {
  return f82267210(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82267470(uint8_t* base, const lifted::Args& a) {
  return f82267470(base, a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e82267540(uint8_t* base, const lifted::Args& a) {
  return f82267540(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82267738(uint8_t* base, const lifted::Args& a) {
  return f82267738(base, a.r1, a.r13, a.r[5], a.f[0]);
}

lifted::Result e822677C0(uint8_t* base, const lifted::Args& a) {
  return f822677C0(base, a.r1, a.r13, a.r[5], a.f[0]);
}

lifted::Result e82267820(uint8_t* base, const lifted::Args& a) {
  return f82267820(base, a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e822678D0(uint8_t* base, const lifted::Args& a) {
  return {f822678D0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e822679A0(uint8_t* base, const lifted::Args& a) {
  return {f822679A0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82267A48(uint8_t* base, const lifted::Args& a) {
  return f82267A48(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82267CD0(uint8_t* base, const lifted::Args& a) {
  return f82267CD0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82267EA0(uint8_t* base, const lifted::Args& a) {
  return {f82267EA0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822681D0(uint8_t* base, const lifted::Args& a) {
  return f822681D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82268720(uint8_t* base, const lifted::Args& a) {
  return f82268720(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822688B8(uint8_t* base, const lifted::Args& a) {
  f822688B8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822688C8(uint8_t* base, const lifted::Args& a) {
  f822688C8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822688D8(uint8_t* base, const lifted::Args& a) {
  f822688D8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82268928(uint8_t* base, const lifted::Args& a) {
  return f82268928(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e82268A78(uint8_t* base, const lifted::Args& a) {
  return f82268A78(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82268FC8(uint8_t* base, const lifted::Args& a) {
  f82268FC8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82269018(uint8_t* base, const lifted::Args& a) {
  return f82269018(base, a.r1, a.r13);
}

lifted::Result e822692D8(uint8_t* base, const lifted::Args& a) {
  return f822692D8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82269710(uint8_t* base, const lifted::Args& a) {
  return f82269710(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82269750(uint8_t* base, const lifted::Args& a) {
  f82269750(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82269898(uint8_t* base, const lifted::Args& a) {
  return {f82269898(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82269988(uint8_t* base, const lifted::Args& a) {
  return f82269988(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82269DD0(uint8_t* base, const lifted::Args& a) {
  return f82269DD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226A210(uint8_t* base, const lifted::Args& a) {
  return f8226A210(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226A500(uint8_t* base, const lifted::Args& a) {
  return f8226A500(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226A660(uint8_t* base, const lifted::Args& a) {
  return {f8226A660(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8226A778(uint8_t* base, const lifted::Args& a) {
  return f8226A778(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226A940(uint8_t* base, const lifted::Args& a) {
  return {f8226A940(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8226AA48(uint8_t* base, const lifted::Args& a) {
  return f8226AA48(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e8226AB48(uint8_t* base, const lifted::Args& a) {
  return f8226AB48(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e8226ABE0(uint8_t* base, const lifted::Args& a) {
  return f8226ABE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8226ACB0(uint8_t* base, const lifted::Args& a) {
  return f8226ACB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8226ADB0(uint8_t* base, const lifted::Args& a) {
  return f8226ADB0(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8226AE88(uint8_t* base, const lifted::Args& a) {
  return f8226AE88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.f[0]);
}

lifted::Result e8226AF50(uint8_t* base, const lifted::Args& a) {
  return f8226AF50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226AFD8(uint8_t* base, const lifted::Args& a) {
  return f8226AFD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226B120(uint8_t* base, const lifted::Args& a) {
  return f8226B120(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226B1A8(uint8_t* base, const lifted::Args& a) {
  return f8226B1A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226B2F0(uint8_t* base, const lifted::Args& a) {
  return f8226B2F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8226B4D0(uint8_t* base, const lifted::Args& a) {
  return f8226B4D0(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8226B620(uint8_t* base, const lifted::Args& a) {
  return f8226B620(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8226B780(uint8_t* base, const lifted::Args& a) {
  return f8226B780(base, a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e8226B830(uint8_t* base, const lifted::Args& a) {
  return f8226B830(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226B940(uint8_t* base, const lifted::Args& a) {
  return f8226B940(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226BBB8(uint8_t* base, const lifted::Args& a) {
  return f8226BBB8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226BCB8(uint8_t* base, const lifted::Args& a) {
  return f8226BCB8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226C230(uint8_t* base, const lifted::Args& a) {
  return f8226C230(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226C3B0(uint8_t* base, const lifted::Args& a) {
  return f8226C3B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226C500(uint8_t* base, const lifted::Args& a) {
  return f8226C500(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226C618(uint8_t* base, const lifted::Args& a) {
  return f8226C618(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226C6E0(uint8_t* base, const lifted::Args& a) {
  return f8226C6E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226C798(uint8_t* base, const lifted::Args& a) {
  return f8226C798(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226C898(uint8_t* base, const lifted::Args& a) {
  return f8226C898(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226C9E0(uint8_t* base, const lifted::Args& a) {
  return f8226C9E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226CB40(uint8_t* base, const lifted::Args& a) {
  return f8226CB40(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226D0A0(uint8_t* base, const lifted::Args& a) {
  return f8226D0A0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226D7B0(uint8_t* base, const lifted::Args& a) {
  return f8226D7B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226DAD8(uint8_t* base, const lifted::Args& a) {
  return f8226DAD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226DC18(uint8_t* base, const lifted::Args& a) {
  return f8226DC18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226DDA0(uint8_t* base, const lifted::Args& a) {
  return f8226DDA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226E000(uint8_t* base, const lifted::Args& a) {
  return f8226E000(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226E1C8(uint8_t* base, const lifted::Args& a) {
  return f8226E1C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8226E3C0(uint8_t* base, const lifted::Args& a) {
  return f8226E3C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226E4D8(uint8_t* base, const lifted::Args& a) {
  return f8226E4D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8226E620(uint8_t* base, const lifted::Args& a) {
  return f8226E620(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x82263840, e82263840},
    {0x82263920, e82263920},
    {0x822639C8, e822639C8},
    {0x82263A98, e82263A98},
    {0x82263B68, e82263B68},
    {0x82263D80, e82263D80},
    {0x82263E20, e82263E20},
    {0x822642B8, e822642B8},
    {0x822643C0, e822643C0},
    {0x822645B0, e822645B0},
    {0x82264A38, e82264A38},
    {0x82264B90, e82264B90},
    {0x822651A0, e822651A0},
    {0x82265668, e82265668},
    {0x822657D8, e822657D8},
    {0x82265930, e82265930},
    {0x822659F0, e822659F0},
    {0x82265C58, e82265C58},
    {0x82265CA8, e82265CA8},
    {0x82265CE8, e82265CE8},
    {0x82265E80, e82265E80},
    {0x82266028, e82266028},
    {0x822662F0, e822662F0},
    {0x822663E0, e822663E0},
    {0x82266548, e82266548},
    {0x82266658, e82266658},
    {0x82266750, e82266750},
    {0x82266908, e82266908},
    {0x82266AC0, e82266AC0},
    {0x82266C80, e82266C80},
    {0x82266E68, e82266E68},
    {0x822670D0, e822670D0},
    {0x82267210, e82267210},
    {0x82267470, e82267470},
    {0x82267540, e82267540},
    {0x82267738, e82267738},
    {0x822677C0, e822677C0},
    {0x82267820, e82267820},
    {0x822678D0, e822678D0},
    {0x822679A0, e822679A0},
    {0x82267A48, e82267A48},
    {0x82267CD0, e82267CD0},
    {0x82267EA0, e82267EA0},
    {0x822681D0, e822681D0},
    {0x82268720, e82268720},
    {0x822688B8, e822688B8},
    {0x822688C8, e822688C8},
    {0x822688D8, e822688D8},
    {0x82268928, e82268928},
    {0x82268A78, e82268A78},
    {0x82268FC8, e82268FC8},
    {0x82269018, e82269018},
    {0x822692D8, e822692D8},
    {0x82269710, e82269710},
    {0x82269750, e82269750},
    {0x82269898, e82269898},
    {0x82269988, e82269988},
    {0x82269DD0, e82269DD0},
    {0x8226A210, e8226A210},
    {0x8226A500, e8226A500},
    {0x8226A660, e8226A660},
    {0x8226A778, e8226A778},
    {0x8226A940, e8226A940},
    {0x8226AA48, e8226AA48},
    {0x8226AB48, e8226AB48},
    {0x8226ABE0, e8226ABE0},
    {0x8226ACB0, e8226ACB0},
    {0x8226ADB0, e8226ADB0},
    {0x8226AE88, e8226AE88},
    {0x8226AF50, e8226AF50},
    {0x8226AFD8, e8226AFD8},
    {0x8226B120, e8226B120},
    {0x8226B1A8, e8226B1A8},
    {0x8226B2F0, e8226B2F0},
    {0x8226B4D0, e8226B4D0},
    {0x8226B620, e8226B620},
    {0x8226B780, e8226B780},
    {0x8226B830, e8226B830},
    {0x8226B940, e8226B940},
    {0x8226BBB8, e8226BBB8},
    {0x8226BCB8, e8226BCB8},
    {0x8226C230, e8226C230},
    {0x8226C3B0, e8226C3B0},
    {0x8226C500, e8226C500},
    {0x8226C618, e8226C618},
    {0x8226C6E0, e8226C6E0},
    {0x8226C798, e8226C798},
    {0x8226C898, e8226C898},
    {0x8226C9E0, e8226C9E0},
    {0x8226CB40, e8226CB40},
    {0x8226D0A0, e8226D0A0},
    {0x8226D7B0, e8226D7B0},
    {0x8226DAD8, e8226DAD8},
    {0x8226DC18, e8226DC18},
    {0x8226DDA0, e8226DDA0},
    {0x8226E000, e8226E000},
    {0x8226E1C8, e8226E1C8},
    {0x8226E3C0, e8226E3C0},
    {0x8226E4D8, e8226E4D8},
    {0x8226E620, e8226E620},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_038)), true);

}  // namespace

}  // namespace lifted
