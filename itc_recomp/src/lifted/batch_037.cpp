// Lift sweep batch 037 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/car_physics.h"
#include "schema/menu_helpers.h"
#include "schema/minimap.h"
#include "schema/mode_tick.h"
#include "schema/network_mode.h"
#include "schema/options_screen.h"
#include "schema/race_hud_machine.h"
#include "schema/race_hud_object.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/stand_models.h"
#include "schema/system_services.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies.h"
#include "schema/ui_movies_game.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_037, true, "ITC",
              "Run lift sweep batch 037's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_8216C798 game::RaceHudMachine::OpenPauseByStage: lifted from generated/ by lift.py.
lifted::Result f8216C798(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_8216C798 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-27104
  r3 = uint64_t(int64_t(int64_t(r3) + -27104));
  // bl 0x820ffbe8
  // (left out: dead in C++)
  { const lifted::Result o = game::Stages::Type(base, LIFTED_ARGS(0x8216C7CC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 1, so);
  // blt cr6,0x8216c7ec
  if (cr6.lt) goto loc_8216C7EC;
  // beq cr6,0x8216c7e4
  if (cr6.eq) goto loc_8216C7E4;
  // cmplwi cr6,r3,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 3, so);
  // blt cr6,0x8216c7ec
  if (cr6.lt) goto loc_8216C7EC;
  // bne cr6,0x8216c7f0
  if (!cr6.eq) goto loc_8216C7F0;
loc_8216C7E4:
  // li r31,6
  r31 = uint64_t(int64_t(6));
  // b 0x8216c7f0
  goto loc_8216C7F0;
loc_8216C7EC:
  // li r31,7
  r31 = uint64_t(int64_t(7));
loc_8216C7F0:
  // lwz r11,9608(r30)
  r11 = guest::LoadU32(base, schema::RaceHudMachine(base, uint32_t(r30)).holder_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r31
  r4 = r31;
  // lwz r3,1012(r11)
  r3 = schema::RaceScreens(base, uint32_t(r11)).pause_menu_bits();
  // bl 0x8220fa88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220FA88, LIFTED_ARGS(0x8216C804, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8216D328 game::RaceHudMachine::ShowSplitResults: lifted from generated/ by lift.py.
lifted::Result f8216D328(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8216D328 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-25880
  r3 = uint64_t(int64_t(int64_t(r3) + -25880));
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(base, LIFTED_ARGS(0x8216D350, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = guest::LoadU32(base, schema::RaceHudMachine(base, uint32_t(r31)).holder_address());
  // mr r29,r3
  r29 = r3;
  // lwz r10,8(r11)
  r10 = schema::RaceScreens(base, uint32_t(r11)).flags_bits();
  // clrlwi. r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x8216d3c8
  if (!cr0.eq) goto loc_8216D3C8;
  // addi r3,r11,656
  r3 = uint64_t(int64_t(int64_t(r11) + 656));
  // bl 0x82216830
  // (left out: dead in C++)
  r3 = f82216830(base, r1, r13, r3);
  // lwz r11,3924(r29)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r29)).finish_address());
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8216d3a0
  if (cr6.eq) goto loc_8216D3A0;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x8216d394
  if (cr6.eq) goto loc_8216D394;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x8216d3b4
  if (!cr6.eq) goto loc_8216D3B4;
  // li r30,2
  r30 = uint64_t(int64_t(2));
  // b 0x8216d3a4
  goto loc_8216D3A4;
loc_8216D394:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x8216d3a8
  goto loc_8216D3A8;
loc_8216D3A0:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_8216D3A4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_8216D3A8:
  // lwz r11,9608(r31)
  r11 = guest::LoadU32(base, schema::RaceHudMachine(base, uint32_t(r31)).holder_address());
  // addi r3,r11,1004
  r3 = uint64_t(int64_t(int64_t(r11) + 1004));
  // bl 0x82217c70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82217C70, LIFTED_ARGS(0x8216D3B4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8216D3B4:
  // lwz r11,9608(r31)
  r11 = guest::LoadU32(base, schema::RaceHudMachine(base, uint32_t(r31)).holder_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // lwz r3,1032(r11)
  r3 = schema::RaceScreens(base, uint32_t(r11)).split_results_screen_bits();
  // bl 0x82220050
  // (left out: dead in C++)
  { const lifted::Result o = game::SplitResults::Open(base, LIFTED_ARGS(0x8216D3C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8216D3C8:
  // lwz r11,3920(r29)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r29)).outcome_address());
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // lwz r10,9608(r31)
  r10 = guest::LoadU32(base, schema::RaceHudMachine(base, uint32_t(r31)).holder_address());
  // addi r11,r11,-3
  r11 = uint64_t(int64_t(int64_t(r11) + -3));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r5,r11,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // lwz r3,1020(r10)
  r3 = schema::RaceScreens(base, uint32_t(r10)).after_race_menu_bits();
  // bl 0x8221ba68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8221BA68, LIFTED_ARGS(0x8216D3E8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8216D7B8 game::RaceHudMachine::ShowKind6Results: lifted from generated/ by lift.py.
lifted::Result f8216D7B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8216D7B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r11,23340(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-25880
  r3 = uint64_t(int64_t(int64_t(r3) + -25880));
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(base, LIFTED_ARGS(0x8216D7E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r8,9608(r28)
  r8 = guest::LoadU32(base, schema::RaceHudMachine(base, uint32_t(r28)).holder_address());
  // mr r30,r3
  r30 = r3;
  // lwz r11,8(r8)
  r11 = schema::RaceScreens(base, uint32_t(r8)).flags_bits();
  // lwz r29,3916(r30)
  r29 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r30)).kind6_result_address());
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8216d890
  if (!cr0.eq) goto loc_8216D890;
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x8216d81c
  if (cr6.lt) goto loc_8216D81C;
  // beq cr6,0x8216d810
  if (cr6.eq) goto loc_8216D810;
  // cmplwi cr6,r29,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 3, so);
  // blt cr6,0x8216d81c
  if (cr6.lt) goto loc_8216D81C;
  // bne cr6,0x8216d890
  if (!cr6.eq) goto loc_8216D890;
loc_8216D810:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x8216d888
  goto loc_8216D888;
loc_8216D81C:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,3924(r30)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r30)).finish_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lwz r10,-32012(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -32012);
  // lwz r10,2308(r10)
  r10 = guest::LoadU32(base, schema::RaceManager(base, uint32_t(r10)).racer0_address());
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, schema::MinimapRacerEntry(base, uint32_t(r10)).record_address());
  // mulli r9,r10,3936
  r9 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(3936))));
  // lwz r10,23340(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 23340);
  // add r10,r9,r10
  r10 = r9 + r10;
  // lwz r10,3936(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 3936);
  // not r10,r10
  r10 = ~r10;
  // rlwinm r10,r10,22,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 22) & 0x1;
  // beq cr6,0x8216d87c
  if (cr6.eq) goto loc_8216D87C;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x8216d86c
  if (cr6.eq) goto loc_8216D86C;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x8216d884
  if (!cr6.eq) goto loc_8216D884;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x8216d884
  goto loc_8216D884;
loc_8216D86C:
  // cntlzw r11,r10
  r11 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r4,r11,1
  r4 = r11 ^ 1;
  // b 0x8216d884
  goto loc_8216D884;
loc_8216D87C:
  // cntlzw r11,r10
  r11 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r4,r11,27,31,31
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
loc_8216D884:
  // li r5,1
  r5 = uint64_t(int64_t(1));
loc_8216D888:
  // lwz r3,1032(r8)
  r3 = schema::RaceScreens(base, uint32_t(r8)).split_results_screen_bits();
  // bl 0x82220050
  // (left out: dead in C++)
  { const lifted::Result o = game::SplitResults::Open(base, LIFTED_ARGS(0x8216D890, r31)); r3 = o.r3; f1 = o.f1; }
loc_8216D890:
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // beq cr6,0x8216d8a0
  if (cr6.eq) goto loc_8216D8A0;
  // cmpwi cr6,r29,3
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 3, so);
  // bne cr6,0x8216d8c0
  if (!cr6.eq) goto loc_8216D8C0;
loc_8216D8A0:
  // lwz r11,9608(r28)
  r11 = guest::LoadU32(base, schema::RaceHudMachine(base, uint32_t(r28)).holder_address());
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,1036(r11)
  r3 = schema::RaceScreens(base, uint32_t(r11)).kind6_menu_bits();
  // lwz r11,20(r30)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r30)).e0_time_address());
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r30)).e0_record_index_address());
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // bl 0x8221ec48
  // (left out: dead in C++)
  { const lifted::Result o = game::Kind6Menu::Open(base, LIFTED_ARGS(0x8216D8C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8216D8C0:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8217A758: lifted from generated/ by lift.py.
lifted::Result f8217A758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8217A758 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-400(r1)
  ea = -400 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r29,r3
  r29 = r3;
  // addi r31,r11,-31792
  r31 = uint64_t(int64_t(int64_t(r11) + -31792));
  // mr r19,r5
  r19 = r5;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8258c9d0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::OpenChunk(base, LIFTED_ARGS(0x8217A77C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217A788, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // li r28,5
  r28 = uint64_t(int64_t(5));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // sth r28,2(r23)
  guest::StoreU16(base, uint32_t(r23) + 2, uint16_t(r28));
  // stw r30,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217A7A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r22,1
  r22 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r22,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r22));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217A7CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r10,96
  r10 = uint64_t(int64_t(96));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217A7E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r10,60
  r10 = uint64_t(int64_t(60));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217A804, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r22,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r22));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217A824, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r22,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r22));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217A844, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // stw r28,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r28));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258cb08
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateVertices(base, LIFTED_ARGS(0x8217A864, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // addi r10,r1,204
  r10 = uint64_t(int64_t(int64_t(r1) + 204));
  // lfs f0,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f0 = double(temp.f32);
loc_8217A874:
  // stfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 0, temp.u32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // bge 0x8217a874
  if (!cr0.lt) goto loc_8217A874;
  // lwz r11,80(r29)
  r11 = guest::LoadU32(base, schema::ColorRect(base, uint32_t(r29)).x_address());
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // lwz r10,84(r29)
  r10 = guest::LoadU32(base, schema::ColorRect(base, uint32_t(r29)).y_address());
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // lwz r20,92(r29)
  r20 = guest::LoadU32(base, schema::ColorRect(base, uint32_t(r29)).height_address());
  // extsw r18,r11
  r18 = uint64_t(int64_t(int32_t(r11)));
  // extsw r17,r10
  r17 = uint64_t(int64_t(int32_t(r10)));
  // lwz r21,88(r29)
  r21 = guest::LoadU32(base, schema::ColorRect(base, uint32_t(r29)).width_address());
  // add r10,r20,r10
  r10 = r20 + r10;
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // add r11,r21,r11
  r11 = r21 + r11;
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 120, temp.u32);
  // std r18,168(r1)
  guest::StoreU64(base, uint32_t(r1) + 168, r18);
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r17,160(r1)
  guest::StoreU64(base, uint32_t(r1) + 160, r17);
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 124, temp.u32);
  // stfs f0,152(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 152, temp.u32);
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfs f0,156(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 156, temp.u32);
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // std r10,176(r1)
  guest::StoreU64(base, uint32_t(r1) + 176, r10);
  // stfs f0,136(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 136, temp.u32);
  // std r11,184(r1)
  guest::StoreU64(base, uint32_t(r1) + 184, r11);
  // stfs f0,140(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 140, temp.u32);
  // addi r7,r1,192
  r7 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r6,r1,112
  r6 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r5,r1,208
  r5 = uint64_t(int64_t(int64_t(r1) + 208));
  // addi r4,r1,144
  r4 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r27,r1,224
  r27 = uint64_t(int64_t(int64_t(r1) + 224));
  // addi r26,r1,128
  r26 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r25,r1,240
  r25 = uint64_t(int64_t(int64_t(r1) + 240));
  // addi r24,r1,256
  r24 = uint64_t(int64_t(int64_t(r1) + 256));
  // lfd f0,168(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 168)));
  // lfd f13,160(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 160)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f12,176(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 176)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,184(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 184)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // frsp f13,f13
  f13 = double(float(f13));
  // stfs f13,84(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // ld r11,0(r9)
  r11 = guest::LoadU64(base, uint32_t(r9) + 0);
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // ld r10,8(r9)
  r10 = guest::LoadU64(base, uint32_t(r9) + 8);
  // stfs f13,148(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 148, temp.u32);
  // frsp f12,f12
  f12 = double(float(f12));
  // stfs f12,100(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // ld r9,0(r8)
  r9 = guest::LoadU64(base, uint32_t(r8) + 0);
  // frsp f11,f11
  f11 = double(float(f11));
  // ld r8,8(r8)
  r8 = guest::LoadU64(base, uint32_t(r8) + 8);
  // stfs f11,112(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 112, temp.u32);
  // std r11,0(r7)
  guest::StoreU64(base, uint32_t(r7) + 0, r11);
  // stfs f12,116(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 116, temp.u32);
  // std r10,8(r7)
  guest::StoreU64(base, uint32_t(r7) + 8, r10);
  // ld r11,0(r6)
  r11 = guest::LoadU64(base, uint32_t(r6) + 0);
  // stfs f11,144(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 144, temp.u32);
  // ld r10,8(r6)
  r10 = guest::LoadU64(base, uint32_t(r6) + 8);
  // stfs f0,128(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 128, temp.u32);
  // std r9,0(r5)
  guest::StoreU64(base, uint32_t(r5) + 0, r9);
  // stfs f13,132(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 132, temp.u32);
  // std r8,8(r5)
  guest::StoreU64(base, uint32_t(r5) + 8, r8);
  // ld r9,0(r4)
  r9 = guest::LoadU64(base, uint32_t(r4) + 0);
  // ld r8,8(r4)
  r8 = guest::LoadU64(base, uint32_t(r4) + 8);
  // std r11,0(r27)
  guest::StoreU64(base, uint32_t(r27) + 0, r11);
  // std r10,8(r27)
  guest::StoreU64(base, uint32_t(r27) + 8, r10);
  // ld r11,0(r26)
  r11 = guest::LoadU64(base, uint32_t(r26) + 0);
  // ld r10,8(r26)
  r10 = guest::LoadU64(base, uint32_t(r26) + 8);
  // std r9,0(r25)
  guest::StoreU64(base, uint32_t(r25) + 0, r9);
  // addi r9,r29,96
  r9 = uint64_t(int64_t(int64_t(r29) + 96));
  // std r8,8(r25)
  guest::StoreU64(base, uint32_t(r25) + 8, r8);
  // std r11,0(r24)
  guest::StoreU64(base, uint32_t(r24) + 0, r11);
  // std r10,8(r24)
  guest::StoreU64(base, uint32_t(r24) + 8, r10);
  // addi r11,r3,8
  r11 = uint64_t(int64_t(int64_t(r3) + 8));
  // addi r10,r1,200
  r10 = uint64_t(int64_t(int64_t(r1) + 200));
loc_8217A9C0:
  // lfs f0,-8(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -8);
  f0 = double(temp.f32);
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // lfs f13,-4(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -4);
  f13 = double(temp.f32);
  // lfs f12,0(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 0);
  f12 = double(temp.f32);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stfs f0,-8(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::CommandListVertices(base, uint32_t(r11) - 8).v0_x_address(), temp.u32);
  // stfs f13,-4(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::CommandListVertices(base, uint32_t(r11) - 8).v0_y_address(), temp.u32);
  // stfs f12,0(r11)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::CommandListVertices(base, uint32_t(r11) - 8).v0_z_address(), temp.u32);
  // lwz r8,12(r9)
  r8 = guest::LoadU32(base, schema::ColorRect(base, uint32_t(r9) - 96).alpha_address());
  // lbz r7,3(r9)
  r7 = guest::LoadU8(base, schema::ColorRect(base, uint32_t(r9) - 96).red_byte_address());
  // rlwinm r8,r8,8,0,23
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 8) & 0xFFFFFF00;
  // lbz r6,7(r9)
  r6 = guest::LoadU8(base, schema::ColorRect(base, uint32_t(r9) - 96).green_byte_address());
  // lbz r5,11(r9)
  r5 = guest::LoadU8(base, schema::ColorRect(base, uint32_t(r9) - 96).blue_byte_address());
  // or r8,r7,r8
  r8 = r7 | r8;
  // rlwinm r8,r8,8,0,23
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 8) & 0xFFFFFF00;
  // or r8,r8,r6
  r8 = r8 | r6;
  // rlwinm r8,r8,8,0,23
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 8) & 0xFFFFFF00;
  // or r8,r8,r5
  r8 = r8 | r5;
  // stw r8,4(r11)
  guest::StoreU32(base, schema::CommandListVertices(base, uint32_t(r11) - 8).v0_color_address(), uint32_t(r8));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // bne 0x8217a9c0
  if (!cr0.eq) goto loc_8217A9C0;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217AA20, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r22,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r22));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217AA40, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8217AA4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,36
  r10 = uint64_t(int64_t(36));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258ca50
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::CloseChunk(base, LIFTED_ARGS(0x8217AA68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r5,r19
  r5 = r19;
  // mr r4,r23
  r4 = r23;
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // lwz r11,16(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8217AA8C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,400
  r1 = uint64_t(int64_t(int64_t(r1) + 400));
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B8290: lifted from generated/ by lift.py.
lifted::Result f821B8290(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B8290 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31640(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -31640);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x821B82AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31640(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -31640);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x821B82B8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b82d4
  if (!cr0.eq) goto loc_821B82D4;
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31640(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -31640);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x821B82CC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b830c
  if (cr0.eq) goto loc_821B830C;
loc_821B82D4:
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x821B82DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31644(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31644);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b82f4
  if (cr6.eq) goto loc_821B82F4;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_821B82F4:
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_821B830C:
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

// sub_821B8528: lifted from generated/ by lift.py.
void f821B8528(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821B8528 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r6,9
  r6 = uint64_t(int64_t(9));
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,36
  r9 = uint64_t(int64_t(36));
  // lwz r10,-31636(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -31636);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r6,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r6));
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
  return;
}

// sub_821B88A0: lifted from generated/ by lift.py.
lifted::Result f821B88A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B88A0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r9,-32228
  r9 = uint64_t(int64_t(-2112094208));
  // lis r10,-32228
  r10 = uint64_t(int64_t(-2112094208));
  // addi r9,r9,-32112
  r9 = uint64_t(int64_t(int64_t(r9) + -32112));
  // addi r10,r10,-31448
  r10 = uint64_t(int64_t(int64_t(r10) + -31448));
  // lwz r11,-31644(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31644);
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821b88f0
  if (cr6.eq) goto loc_821B88F0;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r4
  r3 = r4;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821B88F0, r31)); r3 = o.r3; f1 = o.f1; }
loc_821B88F0:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82212100 game::HudBar::Snap: lifted from generated/ by lift.py.
void f82212100(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82212100 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::RaceHudBar(base, uint32_t(r3)).value_address());
  f0 = double(temp.f32);
  // stfs f0,8(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::RaceHudBar(base, uint32_t(r3)).trail_address(), temp.u32);
  // stw r11,36(r3)
  guest::StoreU32(base, schema::RaceHudBar(base, uint32_t(r3)).touched_address(), uint32_t(r11));
  // blr
  return;
}

// sub_822157E8: lifted from generated/ by lift.py.
void f822157E8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_822157E8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // extsw r10,r5
  r10 = uint64_t(int64_t(int32_t(r5)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // std r10,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r10);
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // mulli r11,r4,124
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(124))));
  // add r11,r11,r3
  r11 = r11 + r3;
  // stfs f0,120(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 120, temp.u32);
  // lfd f13,-16(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f0,f13
  f0 = double(float(f13));
  // stfs f0,116(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 116, temp.u32);
  // stfs f0,112(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 112, temp.u32);
  // blr
  return;
}

// sub_82215870: lifted from generated/ by lift.py.
uint64_t f82215870(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82215870 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8221589C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822158B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r30,80(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 16, uint32_t(r30));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822158CC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r30));
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

// sub_822158E8: lifted from generated/ by lift.py.
lifted::Result f822158E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_822158E8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-496(r1)
  ea = -496 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r5
  r31 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82215918, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r28,12(r31)
  r28 = guest::LoadU32(base, schema::RaceDataAt48(base, uint32_t(r31)).unknown_12_address());
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // lwz r27,0(r11)
  r27 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r11)).value_address());
  // bl 0x82216b08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82216B08, LIFTED_ARGS(0x82215930, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r26,r3
  r26 = r3;
  // addi r31,r11,-1888
  r31 = uint64_t(int64_t(int64_t(r11) + -1888));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r31,-336
  r4 = uint64_t(int64_t(int64_t(r31) + -336));
  // li r5,112
  r5 = uint64_t(int64_t(112));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8221594C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r31,-224
  r4 = uint64_t(int64_t(int64_t(r31) + -224));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // li r5,112
  r5 = uint64_t(int64_t(112));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8221595C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r31,-112
  r4 = uint64_t(int64_t(int64_t(r31) + -112));
  // addi r3,r1,320
  r3 = uint64_t(int64_t(int64_t(r1) + 320));
  // li r5,112
  r5 = uint64_t(int64_t(112));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8221596C, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,124
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(124))));
  // rlwinm r9,r28,4,0,27
  r9 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 4) & 0xFFFFFFF0;
  // add r30,r11,r30
  r30 = r11 + r30;
  // rlwinm r11,r28,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r7,r31,4
  r7 = uint64_t(int64_t(int64_t(r31) + 4));
  // add r11,r11,r26
  r11 = r11 + r26;
  // lwzx r9,r9,r31
  r9 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r31));
  // li r5,112
  r5 = uint64_t(int64_t(112));
  // rlwinm r8,r11,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mulli r11,r9,112
  r11 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(112))));
  // lwzx r31,r8,r7
  r31 = guest::LoadU32(base, uint32_t(r8) + uint32_t(r7));
  // add r4,r11,r10
  r4 = r11 + r10;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822159B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r3,r30
  r3 = r30;
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // bl 0x82212118
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82212118, LIFTED_ARGS(0x822159C8, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r27
  r3 = r27;
  // lwzx r4,r11,r10
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822159E0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r31,108(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 44, uint32_t(r31));
  // addi r1,r1,496
  r1 = uint64_t(int64_t(int64_t(r1) + 496));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82215C68: lifted from generated/ by lift.py.
uint64_t f82215C68(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82215C68 (lifted)");
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
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82215C8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82215C9C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82215CB8: lifted from generated/ by lift.py.
uint64_t f82215CB8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82215CB8 (lifted)");
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
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82215CDC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82215CEC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82215E58: lifted from generated/ by lift.py.
lifted::Result f82215E58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82215E58 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r4
  r30 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r5
  r31 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82215E88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // lwz r25,0(r3)
  r25 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmplwi cr6,r31,999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 999, so);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bgt cr6,0x82215f54
  if (cr6.gt) goto loc_82215F54;
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r3,r25
  r3 = r25;
  // lwzx r26,r11,r10
  r26 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // mr r4,r26
  r4 = r26;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82215EC0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // lwz r11,104(r28)
  r11 = guest::LoadU32(base, schema::UiMovie(base, uint32_t(r28)).draw_context_address() + 40);
  // addic. r30,r11,-1
  ca = uint32_t(r11) > 0;
  r30 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x82215f54
  if (cr0.lt) goto loc_82215F54;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r24,10
  r24 = uint64_t(int64_t(10));
  // lfd f31,-2264(r11)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -2264)));
loc_82215EDC:
  // lwz r11,100(r28)
  r11 = guest::LoadU32(base, schema::UiMovie(base, uint32_t(r28)).draw_context_address() + 36);
  // divw r10,r31,r24
  r10 = uint32_t((int32_t(r24) && !(int32_t(r31) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r31) / int32_t(r24) : 0);
  // mr r4,r26
  r4 = r26;
  // add r5,r11,r30
  r5 = r11 + r30;
  // mulli r11,r10,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(10))));
  // mr r3,r25
  r3 = r25;
  // subf r29,r11,r31
  r29 = r31 - r11;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82215EFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x82215f40
  if (!cr6.eq) goto loc_82215F40;
  // lwz r11,104(r28)
  r11 = guest::LoadU32(base, schema::UiMovie(base, uint32_t(r28)).draw_context_address() + 40);
  // subf r11,r30,r11
  r11 = r11 - r30;
  // addic. r4,r11,-1
  ca = uint32_t(r11) > 0;
  r4 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // ble 0x82215f40
  if (!cr0.gt) goto loc_82215F40;
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x82213c40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82213C40, LIFTED_ARGS(0x82215F20, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // fctiwz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x80000000U) : (f1 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  guest::StoreU32(base, uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // bge cr6,0x82215f40
  if (!cr6.lt) goto loc_82215F40;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x82215f48
  goto loc_82215F48;
loc_82215F40:
  // addi r11,r29,1
  r11 = uint64_t(int64_t(int64_t(r29) + 1));
  // divw r31,r31,r24
  r31 = uint32_t((int32_t(r24) && !(int32_t(r31) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r31) / int32_t(r24) : 0);
loc_82215F48:
  // stw r11,108(r27)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r27)).draw_context_address() + 44, uint32_t(r11));
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x82215edc
  if (!cr0.lt) goto loc_82215EDC;
loc_82215F54:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-80(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -80)));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82215F60: lifted from generated/ by lift.py.
uint64_t f82215F60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82215F60 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r4
  r31 = r4;
  // mulli r11,r31,124
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(124))));
  // add r30,r11,r3
  r30 = r11 + r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x82212118
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82212118, LIFTED_ARGS(0x82215F88, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82215FA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // lwzx r4,r11,r10
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82215FC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,100(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 100);
  f13 = double(temp.f32);
  // lfs f0,-2272(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -2272);
  f0 = double(temp.f32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,36(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).texture_count_address(), temp.u32);
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

// sub_82215FF8: lifted from generated/ by lift.py.
uint64_t f82215FF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82215FF8 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r5
  r31 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8221602C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // lwzx r4,r11,r10
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82216054, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // ble cr6,0x82216074
  if (!cr6.gt) goto loc_82216074;
  // cmpwi cr6,r31,6
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 6, so);
  // bne cr6,0x8221606c
  if (!cr6.eq) goto loc_8221606C;
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // b 0x82216080
  goto loc_82216080;
loc_8221606C:
  // addi r11,r31,-1
  r11 = uint64_t(int64_t(int64_t(r31) + -1));
  // b 0x82216080
  goto loc_82216080;
loc_82216074:
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // beq cr6,0x82216080
  if (cr6.eq) goto loc_82216080;
  // li r11,6
  r11 = uint64_t(int64_t(6));
loc_82216080:
  // stw r11,108(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 44, uint32_t(r11));
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

// sub_822160A0: lifted from generated/ by lift.py.
uint64_t f822160A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_822160A0 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r4
  r31 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r5
  r30 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822160D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // lwzx r4,r11,r10
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822160FC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r30));
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

// sub_82216118: lifted from generated/ by lift.py.
uint64_t f82216118(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82216118 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmr f31,f1
  f31 = f1;
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82216154, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,124
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(124))));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // rlwinm r10,r31,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // add r31,r11,r30
  r31 = r11 + r30;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwzx r4,r10,r9
  r4 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82216184, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,120(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 120);
  f0 = double(temp.f32);
  // stfs f31,112(r31)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r31) + 112, temp.u32);
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // bgt cr6,0x822161a0
  if (cr6.gt) goto loc_822161A0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r11));
  // b 0x822161b0
  goto loc_822161B0;
loc_822161A0:
  // lfs f13,116(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 116);
  f13 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // fdivs f0,f31,f0
  f0 = double(float(f31 / f0));
  // stfs f0,28(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).textures_address(), temp.u32);
loc_822161B0:
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
  return r3;
}

// sub_822161D0: lifted from generated/ by lift.py.
lifted::Result f822161D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822161D0 (lifted)");
  uint64_t r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r26,r5
  r26 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82216200, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // add r11,r11,r27
  r11 = r11 + r27;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // addi r31,r11,248
  r31 = uint64_t(int64_t(int64_t(r11) + 248));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r28,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r28));
  // stw r24,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r24));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // stw r28,32(r31)
  guest::StoreU32(base, uint32_t(r31) + 32, uint32_t(r28));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 36);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8221625c
  if (!cr6.eq) goto loc_8221625C;
  // mr r4,r29
  r4 = r29;
  // mr r3,r27
  r3 = r27;
  // bl 0x82215820
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82215820, LIFTED_ARGS(0x8221624C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8221625c
  if (cr0.eq) goto loc_8221625C;
  // mr r3,r31
  r3 = r31;
  // bl 0x82212100
  // (left out: dead in C++)
  game::HudBar::Snap(base, r3);
loc_8221625C:
  // mr r4,r26
  r4 = r26;
  // mr r3,r31
  r3 = r31;
  // bl 0x82211fb8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82211FB8, LIFTED_ARGS(0x82216268, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // rlwinm r25,r29,2,0,29
  r25 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // lwzx r5,r25,r11
  r5 = guest::LoadU32(base, uint32_t(r25) + uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82216280, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // lwz r11,100(r29)
  r11 = guest::LoadU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 36);
  // addi r5,r11,2
  r5 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82216298, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,100(r29)
  r11 = guest::LoadU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 36);
  // mr r27,r3
  r27 = r3;
  // addi r5,r11,4
  r5 = uint64_t(int64_t(int64_t(r11) + 4));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822162B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,100(r29)
  r11 = guest::LoadU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 36);
  // mr r26,r3
  r26 = r3;
  // addi r5,r11,3
  r5 = uint64_t(int64_t(int64_t(r11) + 3));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822162C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,100(r29)
  r11 = guest::LoadU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 36);
  // mr r23,r3
  r23 = r3;
  // addi r5,r11,5
  r5 = uint64_t(int64_t(int64_t(r11) + 5));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822162E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f13,24(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f13 = double(temp.f32);
  // lfs f0,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f0 = double(temp.f32);
  // mr r29,r3
  r29 = r3;
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x82216320
  if (cr6.gt) goto loc_82216320;
  // stw r28,80(r27)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r27)).draw_context_address() + 16, uint32_t(r28));
  // stw r28,80(r26)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r26)).draw_context_address() + 16, uint32_t(r28));
  // stw r28,80(r29)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 16, uint32_t(r28));
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 32);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8221636c
  if (cr6.eq) goto loc_8221636C;
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r3,r30
  r3 = r30;
  // lwzx r4,r25,r11
  r4 = guest::LoadU32(base, uint32_t(r25) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8221631C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8221636c
  goto loc_8221636C;
loc_82216320:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f12,20(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f12 = double(temp.f32);
  // lfd f0,-2248(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -2248)));
  // fmul f0,f12,f0
  f0 = f12 * f0;
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x8221634c
  if (cr6.gt) goto loc_8221634C;
  // stw r28,80(r27)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r27)).draw_context_address() + 16, uint32_t(r28));
  // stw r28,80(r26)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r26)).draw_context_address() + 16, uint32_t(r28));
  // stw r24,80(r29)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 16, uint32_t(r24));
  // stw r24,32(r31)
  guest::StoreU32(base, uint32_t(r31) + 32, uint32_t(r24));
  // b 0x8221636c
  goto loc_8221636C;
loc_8221634C:
  // lwz r11,28(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82216360
  if (cr6.eq) goto loc_82216360;
  // stw r28,80(r27)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r27)).draw_context_address() + 16, uint32_t(r28));
  // b 0x82216364
  goto loc_82216364;
loc_82216360:
  // stw r24,80(r27)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r27)).draw_context_address() + 16, uint32_t(r24));
loc_82216364:
  // stw r24,80(r26)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r26)).draw_context_address() + 16, uint32_t(r24));
  // stw r28,80(r29)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 16, uint32_t(r28));
loc_8221636C:
  // lwz r11,36(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 36);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82216380
  if (!cr6.eq) goto loc_82216380;
  // stw r24,80(r23)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r23)).draw_context_address() + 16, uint32_t(r24));
  // b 0x82216384
  goto loc_82216384;
loc_82216380:
  // stw r28,80(r23)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r23)).draw_context_address() + 16, uint32_t(r28));
loc_82216384:
  // lfs f11,20(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f11 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f13 = double(temp.f32);
  // fsubs f13,f11,f13
  f13 = double(float(f11 - f13));
  // lfs f12,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // lfs f0,-2256(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -2256);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // stfs f13,28(r26)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r26)).textures_address(), temp.u32);
  // lfs f12,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f12 = double(temp.f32);
  // lfs f13,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f13 = double(temp.f32);
  // fsubs f13,f13,f12
  f13 = double(float(f13 - f12));
  // lfs f12,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // stfs f13,28(r27)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r27)).textures_address(), temp.u32);
  // lfs f12,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // lfs f13,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // fsubs f13,f13,f12
  f13 = double(float(f13 - f12));
  // lfs f11,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f11 = double(temp.f32);
  // lfs f12,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f12 = double(temp.f32);
  // fsubs f12,f11,f12
  f12 = double(float(f11 - f12));
  // fdivs f13,f13,f12
  f13 = double(float(f13 / f12));
  // stfs f13,28(r23)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r23)).textures_address(), temp.u32);
  // lfs f12,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f12 = double(temp.f32);
  // lfs f13,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f13 = double(temp.f32);
  // fsubs f13,f13,f12
  f13 = double(float(f13 - f12));
  // lfs f12,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // stfs f13,28(r29)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r29)).textures_address(), temp.u32);
  // lfs f12,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f12 = double(temp.f32);
  // lfs f13,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f13 = double(temp.f32);
  // fsubs f13,f13,f12
  f13 = double(float(f13 - f12));
  // lfs f12,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f12 = double(temp.f32);
  // lfs f11,12(r27)
  temp.u32 = guest::LoadU32(base, schema::UiMovie(base, uint32_t(r27)).texture_set_address());
  f11 = double(temp.f32);
  // fdivs f12,f12,f13
  f12 = double(float(f12 / f13));
  // lfs f13,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // fsubs f13,f13,f12
  f13 = double(float(f13 - f12));
  // fmadds f0,f13,f0,f11
  f0 = double(float(std::fma(f13, f0, f11)));
  // stfs f0,12(r23)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r23)).texture_set_address(), temp.u32);
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82216428: lifted from generated/ by lift.py.
uint64_t f82216428(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82216428 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r4
  r30 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r5
  r31 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82216454, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,32
  r9 = uint64_t(int64_t(32));
  // lwz r26,0(r31)
  r26 = guest::LoadU32(base, schema::SplitHudTime(base, uint32_t(r31)).packed_address());
  // lis r11,59
  r11 = uint64_t(int64_t(3866624));
  // lwz r27,0(r3)
  r27 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // rlwinm r10,r26,0,1,15
  r10 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 0) & 0x7FFF0000;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // li r9,49
  r9 = uint64_t(int64_t(49));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // ble cr6,0x82216488
  if (!cr6.gt) goto loc_82216488;
  // lis r11,59
  r11 = uint64_t(int64_t(3866624));
  // ori r11,r11,61415
  r11 = r11 | 61415;
  // rlwimi r26,r11,0,1,31
  r26 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x7FFFFFFF) | (r26 & 0xFFFFFFFF80000000);
loc_82216488:
  // rlwinm r31,r26,16,17,31
  r31 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 16) & 0x7FFF;
  // cmpwi cr6,r31,59
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 59, so);
  // ble cr6,0x82216498
  if (!cr6.gt) goto loc_82216498;
  // li r31,59
  r31 = uint64_t(int64_t(59));
loc_82216498:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r30,10
  r30 = uint64_t(int64_t(10));
  // lwzx r28,r11,r10
  r28 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
loc_822164AC:
  // divw r11,r31,r30
  r11 = uint32_t((int32_t(r30) && !(int32_t(r31) == INT32_MIN && int32_t(r30) == -1)) ? int32_t(r31) / int32_t(r30) : 0);
  // add r5,r28,r29
  r5 = r28 + r29;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r27
  r3 = r27;
  // addi r25,r11,1
  r25 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822164CC, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // divw r31,r31,r30
  r31 = uint32_t((int32_t(r30) && !(int32_t(r31) == INT32_MIN && int32_t(r30) == -1)) ? int32_t(r31) / int32_t(r30) : 0);
  // stw r25,108(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 44, uint32_t(r25));
  // bge 0x822164ac
  if (!cr0.lt) goto loc_822164AC;
  // rlwinm r31,r26,22,26,31
  r31 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 22) & 0x3F;
  // cmpwi cr6,r31,59
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 59, so);
  // ble cr6,0x822164ec
  if (!cr6.gt) goto loc_822164EC;
  // li r31,59
  r31 = uint64_t(int64_t(59));
loc_822164EC:
  // li r29,3
  r29 = uint64_t(int64_t(3));
loc_822164F0:
  // divw r11,r31,r30
  r11 = uint32_t((int32_t(r30) && !(int32_t(r31) == INT32_MIN && int32_t(r30) == -1)) ? int32_t(r31) / int32_t(r30) : 0);
  // add r5,r28,r29
  r5 = r28 + r29;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r27
  r3 = r27;
  // addi r25,r11,1
  r25 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82216510, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // divw r31,r31,r30
  r31 = uint32_t((int32_t(r30) && !(int32_t(r31) == INT32_MIN && int32_t(r30) == -1)) ? int32_t(r31) / int32_t(r30) : 0);
  // stw r25,108(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 44, uint32_t(r25));
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // bge cr6,0x822164f0
  if (!cr6.lt) goto loc_822164F0;
  // clrlwi r31,r26,22
  r31 = uint32_t(r26) & 0x3FF;
  // cmpwi cr6,r31,999
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 999, so);
  // ble cr6,0x82216534
  if (!cr6.gt) goto loc_82216534;
  // li r31,999
  r31 = uint64_t(int64_t(999));
loc_82216534:
  // li r29,6
  r29 = uint64_t(int64_t(6));
loc_82216538:
  // divw r11,r31,r30
  r11 = uint32_t((int32_t(r30) && !(int32_t(r31) == INT32_MIN && int32_t(r30) == -1)) ? int32_t(r31) / int32_t(r30) : 0);
  // add r5,r28,r29
  r5 = r28 + r29;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r27
  r3 = r27;
  // addi r26,r11,1
  r26 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82216558, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // divw r31,r31,r30
  r31 = uint32_t((int32_t(r30) && !(int32_t(r31) == INT32_MIN && int32_t(r30) == -1)) ? int32_t(r31) / int32_t(r30) : 0);
  // stw r26,108(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 44, uint32_t(r26));
  // cmpwi cr6,r29,4
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4, so);
  // bge cr6,0x82216538
  if (!cr6.lt) goto loc_82216538;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return r3;
}

// sub_82216578: lifted from generated/ by lift.py.
lifted::Result f82216578(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82216578 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r4
  r31 = r4;
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x822165a0
  if (!cr6.lt) goto loc_822165A0;
  // fmr f1,f0
  f1 = f0;
loc_822165A0:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-2240(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -2240);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x822165b4
  if (!cr6.gt) goto loc_822165B4;
  // fmr f1,f0
  f1 = f0;
loc_822165B4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fctiwz f12,f1
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x80000000U) : (f1 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f1)))));
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // li r25,10
  r25 = uint64_t(int64_t(10));
  // lfs f0,8152(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8152);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f1,f0
  f13 = double(float(f1 * f0));
  // stfiwx f12,0,r10
  guest::StoreU32(base, uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f12)));
  // lfs f0,10764(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10764);
  f0 = double(temp.f32);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmuls f0,f1,f0
  f0 = double(float(f1 * f0));
  // fctiwz f13,f13
  f13 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f13) ? int64_t(0x80000000U) : (f13 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f13)))));
  // stfiwx f13,0,r11
  guest::StoreU32(base, uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f13)));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // divw r9,r11,r25
  r9 = uint32_t((int32_t(r25) && !(int32_t(r11) == INT32_MIN && int32_t(r25) == -1)) ? int32_t(r11) / int32_t(r25) : 0);
  // mulli r9,r9,10
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(10))));
  // stfiwx f0,0,r10
  guest::StoreU32(base, uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // subf r24,r9,r11
  r24 = r11 - r9;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // divw r10,r11,r25
  r10 = uint32_t((int32_t(r25) && !(int32_t(r11) == INT32_MIN && int32_t(r25) == -1)) ? int32_t(r11) / int32_t(r25) : 0);
  // mulli r10,r10,10
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(10))));
  // subf. r11,r10,r11
  r11 = r11 - r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x82216634
  if (!cr0.gt) goto loc_82216634;
  // addi r24,r24,1
  r24 = uint64_t(int64_t(int64_t(r24) + 1));
  // cmpwi cr6,r24,10
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 10, so);
  // bne cr6,0x82216634
  if (!cr6.eq) goto loc_82216634;
  // lwz r11,88(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 88);
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // addi r28,r11,1
  r28 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x82216638
  goto loc_82216638;
loc_82216634:
  // lwz r28,88(r1)
  r28 = guest::LoadU32(base, uint32_t(r1) + 88);
loc_82216638:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82216650, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,27
  r9 = uint64_t(int64_t(27));
  // lwz r26,0(r3)
  r26 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // li r31,2
  r31 = uint64_t(int64_t(2));
  // stw r9,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r9));
  // li r9,44
  r9 = uint64_t(int64_t(44));
  // stw r9,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r9));
  // lwzx r27,r11,r10
  r27 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f31,-2264(r11)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -2264)));
loc_8221667C:
  // add r5,r27,r31
  r5 = r27 + r31;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8221668C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // bne cr6,0x822166a0
  if (!cr6.eq) goto loc_822166A0;
  // addi r11,r24,1
  r11 = uint64_t(int64_t(int64_t(r24) + 1));
  // b 0x822166ec
  goto loc_822166EC;
loc_822166A0:
  // divw r11,r28,r25
  r11 = uint32_t((int32_t(r25) && !(int32_t(r28) == INT32_MIN && int32_t(r25) == -1)) ? int32_t(r28) / int32_t(r25) : 0);
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf. r30,r11,r28
  r30 = r28 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne 0x822166e4
  if (!cr0.eq) goto loc_822166E4;
  // subfic r4,r31,1
  ca = uint32_t(r31) <= 1;
  r4 = static_cast<uint64_t>(1) - r31;
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // ble 0x822166e4
  if (!cr0.gt) goto loc_822166E4;
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x82213c40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82213C40, LIFTED_ARGS(0x822166C4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // fctiwz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x80000000U) : (f1 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  guest::StoreU32(base, uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpw cr6,r28,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r11), so);
  // bge cr6,0x822166e4
  if (!cr6.lt) goto loc_822166E4;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x822166ec
  goto loc_822166EC;
loc_822166E4:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // divw r28,r28,r25
  r28 = uint32_t((int32_t(r25) && !(int32_t(r28) == INT32_MIN && int32_t(r25) == -1)) ? int32_t(r28) / int32_t(r25) : 0);
loc_822166EC:
  // stw r11,108(r29)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 44, uint32_t(r11));
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge 0x8221667c
  if (!cr0.lt) goto loc_8221667C;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-80(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -80)));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82216708: lifted from generated/ by lift.py.
lifted::Result f82216708(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82216708 (lifted)");
  uint64_t r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r5
  r30 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82216738, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,14
  r11 = uint64_t(int64_t(14));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // blt cr6,0x822167f0
  if (cr6.lt) goto loc_822167F0;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // rlwinm r28,r29,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lwzx r5,r28,r11
  r5 = guest::LoadU32(base, uint32_t(r28) + uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8221677C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r29,82
  r11 = uint64_t(int64_t(int64_t(r29) + 82));
  // rlwinm r26,r11,2,0,29
  r26 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r11));
  // lwzx r11,r26,r27
  r11 = guest::LoadU32(base, uint32_t(r26) + uint32_t(r27));
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // beq cr6,0x82216808
  if (cr6.eq) goto loc_82216808;
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwzx r28,r28,r11
  r28 = guest::LoadU32(base, uint32_t(r28) + uint32_t(r11));
loc_822167A4:
  // add r4,r28,r29
  r4 = r28 + r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822167B0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // blt cr6,0x822167a4
  if (cr6.lt) goto loc_822167A4;
  // add r29,r28,r30
  r29 = r28 + r30;
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822167DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822167E8, r31)); r3 = o.r3; f1 = o.f1; }
  // stwx r30,r26,r27
  guest::StoreU32(base, uint32_t(r26) + uint32_t(r27), uint32_t(r30));
  // b 0x82216808
  goto loc_82216808;
loc_822167F0:
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r5,r11,r10
  r5 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82216800, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r11));
loc_82216808:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82216830: lifted from generated/ by lift.py.
uint64_t f82216830(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82216830 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,340(r3)
  r11 = schema::RaceHudAt656(base, uint32_t(r3)).unknown_340_bits();
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82216844
  if (cr6.lt) goto loc_82216844;
  // bnelr cr6
  if (!cr6.eq) return r3;
  // b 0x82215cb8
  r3 = f82215CB8(base, r1, r13);
  return r3;
loc_82216844:
  // b 0x82215c68
  r3 = f82215C68(base, r1, r13);
  return r3;
}

// sub_82216850: lifted from generated/ by lift.py.
lifted::Result f82216850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82216850 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
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
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r29,r3
  r29 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lfd f31,-23280(r11)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -23280)));
loc_82216870:
  // mr r3,r31
  r3 = r31;
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::Get(base, LIFTED_ARGS(0x82216878, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // bl 0x820c0128
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::GetSpeedKmh(base, LIFTED_ARGS(0x82216880, r31)); r3 = o.r3; f1 = o.f1; }
  // fmul f0,f1,f31
  lifted::ScalarFloatMode();
  f0 = f1 * f31;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // frsp f0,f0
  f0 = double(float(f0));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  guest::StoreU32(base, uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r5,80(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x82215e58
  // (left out: dead in C++)
  { const lifted::Result o = f82215E58(base, r1, r13, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // lwz r5,2252(r30)
  r5 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r30)).gear_address());
  // mr r3,r29
  r3 = r29;
  // bl 0x82215ff8
  // (left out: dead in C++)
  r3 = f82215FF8(base, r1, r13, r4, r5);
  // mr r3,r30
  r3 = r30;
  // bl 0x820c0140
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820C0140(base, LIFTED_ARGS(0x822168BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x82215f60
  // (left out: dead in C++)
  r3 = f82215F60(base, r1, r13, r3, r4, f1);
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // lfs f1,2420(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r30)).nitrous_value_address());
  f1 = double(temp.f32);
  // bl 0x82216118
  // (left out: dead in C++)
  r3 = f82216118(base, r1, r13, r3, r4, f1);
  // lwz r11,5712(r30)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r30)).car_address());
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // lwz r5,3892(r11)
  r5 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r11)).route_address());
  // bl 0x822160a0
  // (left out: dead in C++)
  r3 = f822160A0(base, r1, r13, r4, r5);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x82216870
  if (cr6.lt) goto loc_82216870;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82216908: lifted from generated/ by lift.py.
lifted::Result f82216908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82216908 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82216920:
  // lwz r11,344(r29)
  r11 = schema::SplitHud(base, uint32_t(r29)).results_bits();
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r5,788(r11)
  r5 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r11)).e0_sp_address());
  // bl 0x822161d0
  // (left out: dead in C++)
  { const lifted::Result o = f822161D0(base, r1, r13, r3, r4, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,972
  r31 = uint64_t(int64_t(int64_t(r31) + 972));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r31,1944
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 1944, so);
  // blt cr6,0x82216920
  if (cr6.lt) goto loc_82216920;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82216950: lifted from generated/ by lift.py.
lifted::Result f82216950(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_82216950 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU64(base, uint32_t(r1) + -56, std::bit_cast<uint64_t>(f29));
  // stfd f30,-48(r1)
  guest::StoreU64(base, uint32_t(r1) + -48, std::bit_cast<uint64_t>(f30));
  // stfd f31,-40(r1)
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lwz r11,344(r30)
  r11 = schema::SplitHud(base, uint32_t(r30)).results_bits();
  // lwz r3,3932(r11)
  r3 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r11)).ticks_address());
  // bl 0x82101760
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceTime::FromTicks(base, LIFTED_ARGS(0x82216978, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
loc_82216980:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x82216428
  // (left out: dead in C++)
  r3 = f82216428(base, r1, r13, r4, r5);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x82216980
  if (cr6.lt) goto loc_82216980;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,-32024(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -32024);
  // lfd f30,-23280(r10)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r10) + -23280)));
  // lfs f29,7156(r11)
  temp.u32 = guest::LoadU32(base, schema::RaceProgressPart(base, uint32_t(r11)).unknown_7156_address());
  f29 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,10712(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10712);
  f31 = double(temp.f32);
loc_822169C0:
  // lwz r11,344(r30)
  r11 = schema::SplitHud(base, uint32_t(r30)).results_bits();
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // add r11,r31,r11
  r11 = r31 + r11;
  // lfs f0,828(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r11)).e0_route_distance_address());
  f0 = double(temp.f32);
  // fsubs f0,f29,f0
  f0 = double(float(f29 - f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fmul f0,f0,f30
  f0 = f0 * f30;
  // frsp f1,f0
  f1 = double(float(f0));
  // bl 0x82216578
  // (left out: dead in C++)
  { const lifted::Result o = f82216578(base, r1, r13, r4, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,972
  r31 = uint64_t(int64_t(int64_t(r31) + 972));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r31,1944
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 1944, so);
  // blt cr6,0x822169c0
  if (cr6.lt) goto loc_822169C0;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82216A00:
  // lwz r11,344(r30)
  r11 = schema::SplitHud(base, uint32_t(r30)).results_bits();
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // add r11,r31,r11
  r11 = r31 + r11;
  // lwz r5,12(r11)
  r5 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r11)).e0_place_address());
  // bl 0x82216708
  // (left out: dead in C++)
  { const lifted::Result o = f82216708(base, r1, r13, r3, r4, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,972
  r31 = uint64_t(int64_t(int64_t(r31) + 972));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r31,1944
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 1944, so);
  // blt cr6,0x82216a00
  if (cr6.lt) goto loc_82216A00;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f29,-56(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // lfd f30,-48(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82216A40: lifted from generated/ by lift.py.
lifted::Result f82216A40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82216A40 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,340(r3)
  r11 = schema::SplitHud(base, uint32_t(r3)).mode_bits();
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82216a54
  if (cr6.lt) goto loc_82216A54;
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x82216950
  { const lifted::Result o = f82216950(base, r1, r13, r3, r7); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82216A54:
  // b 0x82216908
  { const lifted::Result o = f82216908(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8225DF98: lifted from generated/ by lift.py.
void f8225DF98(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8225DF98 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // lwz r10,24808(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24808);
  // stw r9,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r9));
  // lwz r11,24808(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24808);
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // blr
  return;
}

// sub_8225DFB8: lifted from generated/ by lift.py.
void f8225DFB8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8225DFB8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // lwz r10,24808(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24808);
  // stw r9,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r9));
  // lwz r11,24808(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24808);
  // stw r8,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r8));
  // blr
  return;
}

// sub_8225E268: lifted from generated/ by lift.py.
void f8225E268(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225E268 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8225e2e8
  if (cr6.eq) goto loc_8225E2E8;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // lwz r11,24808(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24808);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24800(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24800);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8225e2bc
  if (cr6.eq) goto loc_8225E2BC;
  // beq 0x8225e2a4
  if (cr0.eq) goto loc_8225E2A4;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8225E2A4:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // b 0x8225e2d8
  goto loc_8225E2D8;
loc_8225E2BC:
  // beq 0x8225e2c4
  if (cr0.eq) goto loc_8225E2C4;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8225E2C4:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r8));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_8225E2D8:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r9));
  // blr
  return;
loc_8225E2E8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_8225E2F8: lifted from generated/ by lift.py.
void f8225E2F8(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8225E2F8 (lifted)");
  uint64_t r11 = 0;
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // stw r11,9604(r4)
  guest::StoreU32(base, uint32_t(r4) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_8225E308: lifted from generated/ by lift.py.
uint64_t f8225E308(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8225E308 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r8,-32127
  r8 = uint64_t(int64_t(-2105475072));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24808(r8)
  r10 = guest::LoadU32(base, uint32_t(r8) + 24808);
  // lwz r11,24800(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24800);
  // lwz r9,8(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 8);
  // lwz r7,0(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r6,12(r10)
  r6 = guest::LoadU32(base, uint32_t(r10) + 12);
  // cmpw cr6,r9,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r7), so);
  // beq cr6,0x8225e33c
  if (cr6.eq) goto loc_8225E33C;
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
loc_8225E33C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // cmpwi cr6,r6,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), -1, so);
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // bne cr6,0x8225e358
  if (!cr6.eq) goto loc_8225E358;
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // b 0x8225e364
  goto loc_8225E364;
loc_8225E358:
  // stw r6,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r6));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_8225E364:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // lwz r11,24808(r8)
  r11 = guest::LoadU32(base, uint32_t(r8) + 24808);
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,24808(r8)
  r11 = guest::LoadU32(base, uint32_t(r8) + 24808);
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225E38C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8225E39C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8225E3B0: lifted from generated/ by lift.py.
lifted::Result f8225E3B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8225E3B0 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225E3DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x8225E3E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9616);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8225e400
  if (cr6.eq) goto loc_8225E400;
  // li r3,32
  r3 = uint64_t(int64_t(32));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(base, LIFTED_ARGS(0x8225E3F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9608(r31)
  guest::StoreU32(base, uint32_t(r31) + 9608, uint32_t(r11));
loc_8225E400:
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

// sub_8225E418: lifted from generated/ by lift.py.
lifted::Result f8225E418(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8225E418 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,24804(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24804);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8225e498
  if (cr6.lt) goto loc_8225E498;
  // beq cr6,0x8225e48c
  if (cr6.eq) goto loc_8225E48C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x8225e474
  if (cr6.lt) goto loc_8225E474;
  // bne cr6,0x8225e4fc
  if (!cr6.eq) goto loc_8225E4FC;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // stw r10,160(r11)
  guest::StoreU32(base, schema::Session(base, uint32_t(r11)).unknown_160_address(), uint32_t(r10));
  // bl 0x82292710
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82292710, LIFTED_ARGS(0x8225E46C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // b 0x8225e4ac
  goto loc_8225E4AC;
loc_8225E474:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // lwz r10,28220(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 28220);
  // stw r6,244(r10)
  guest::StoreU32(base, schema::Session(base, uint32_t(r10)).ranking_after_race_address(), uint32_t(r6));
  // b 0x8225e4ac
  goto loc_8225E4AC;
loc_8225E48C:
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // b 0x8225e4a0
  goto loc_8225E4A0;
loc_8225E498:
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // li r6,0
  r6 = uint64_t(int64_t(0));
loc_8225E4A0:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,28220(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 28220);
  // stw r6,160(r10)
  guest::StoreU32(base, schema::Session(base, uint32_t(r10)).unknown_160_address(), uint32_t(r6));
loc_8225E4AC:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // lwz r10,600(r11)
  r10 = schema::NetModeDispatcher(base, uint32_t(r11) - 204).ring_count_bits();
  // lwz r6,604(r11)
  r6 = schema::NetModeDispatcher(base, uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::NetModeDispatcher(base, uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  // stw r4,9604(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).state_address(), uint32_t(r4));
loc_8225E4FC:
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

// sub_8225E5C8: lifted from generated/ by lift.py.
lifted::Result f8225E5C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8225E5C8 (lifted)");
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
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -4096);
  // stwu r1,-6032(r1)
  ea = -6032 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225E5F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,109
  r11 = uint64_t(int64_t(109));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r28,r1,96
  r28 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r30,r29
  r30 = r29;
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // li r11,110
  r11 = uint64_t(int64_t(110));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // li r11,309
  r11 = uint64_t(int64_t(309));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // li r11,115
  r11 = uint64_t(int64_t(115));
  // stw r11,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r11));
loc_8225E620:
  // addi r5,r30,5
  r5 = uint64_t(int64_t(int64_t(r30) + 5));
  // lwz r6,0(r28)
  r6 = guest::LoadU32(base, uint32_t(r28) + 0);
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8225E634, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // cmplwi cr6,r30,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 4, so);
  // blt cr6,0x8225e620
  if (cr6.lt) goto loc_8225E620;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r11,26248
  r4 = uint64_t(int64_t(int64_t(r11) + 26248));
  // li r5,1968
  r5 = uint64_t(int64_t(1968));
  // lwz r10,-31776(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -31776);
  // lwz r30,0(r10)
  r30 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r10)).stage_block_0_address());
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8225E664, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,2080
  r3 = uint64_t(int64_t(int64_t(r1) + 2080));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x8225E66C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,2080
  r3 = uint64_t(int64_t(int64_t(r1) + 2080));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8225E67C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,5968(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 5968);
  // addi r3,r1,2080
  r3 = uint64_t(int64_t(int64_t(r1) + 2080));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,5968(r1)
  guest::StoreU32(base, uint32_t(r1) + 5968, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x8225E690, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r3,r30
  r3 = r30;
  // lwz r4,28224(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 28224);
  // bl 0x82178de8
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetPlates(base, LIFTED_ARGS(0x8225E6A0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,2080
  r4 = uint64_t(int64_t(int64_t(r1) + 2080));
  // mr r3,r30
  r3 = r30;
  // lha r5,112(r1)
  r5 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r1) + 112))));
  // bl 0x8217fd58
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetCar(base, LIFTED_ARGS(0x8225E6B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetDrawWord(base, LIFTED_ARGS(0x8225E6BC, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8225E6D0, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8225E6E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8225E6F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8225E704, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8225E714, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24804(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24804);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,6016
  r11 = uint64_t(int64_t(int64_t(r11) + 6016));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8225E740, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x8225E754, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x8225E764, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lwz r10,24808(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24808);
  // stw r9,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,24808(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24808);
  // stw r29,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r29));
  // lwz r11,24808(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24808);
  // stw r29,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r29));
  // addi r1,r1,6032
  r1 = uint64_t(int64_t(int64_t(r1) + 6032));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8225E790: lifted from generated/ by lift.py.
lifted::Result f8225E790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8225E790 (lifted)");
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
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225E7C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8225e83c
  if (!cr6.eq) goto loc_8225E83C;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x8225E7D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,50
  r4 = uint64_t(int64_t(50));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225E7F4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225E810, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8225E818, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8225E834, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_8225E83C:
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
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8225E864, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8225e8b8
  if (cr0.eq) goto loc_8225E8B8;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x8225e8b8
  if (cr6.eq) goto loc_8225E8B8;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,45
  r4 = uint64_t(int64_t(45));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225E894, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8225E89C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,-31776(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetDrawWord(base, LIFTED_ARGS(0x8225E8B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x8225e8bc
  goto loc_8225E8BC;
loc_8225E8B8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8225E8BC:
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

// sub_8225E8D8: lifted from generated/ by lift.py.
lifted::Result f8225E8D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8225E8D8 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225E908, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8225e93c
  if (!cr6.eq) goto loc_8225E93C;
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
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x8225E934, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8225E93C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8225E948, r31)); r3 = o.r3; f1 = o.f1; }
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
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x8225e97c
  if (cr0.eq) goto loc_8225E97C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,-31776(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetDrawWord(base, LIFTED_ARGS(0x8225E97C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8225E97C:
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

// sub_8225E998: lifted from generated/ by lift.py.
lifted::Result f8225E998(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8225E998 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225E9C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8225ea1c
  if (!cr6.eq) goto loc_8225EA1C;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225E9E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225EA0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8225EA14, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_8225EA1C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8225EA28, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8225EA38: lifted from generated/ by lift.py.
uint64_t f8225EA38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225EA38 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225EA68, r31)); r3 = o.r3; f1 = o.f1; }
  // clrldi r11,r31,32
  r11 = r31 & 0xFFFFFFFF;
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8225EA8C, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r3) + 16, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8225EAA8: lifted from generated/ by lift.py.
lifted::Result f8225EAA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8225EAA8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225EAD0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r4,47
  r4 = uint64_t(int64_t(47));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EAE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EAEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,47
  r4 = uint64_t(int64_t(47));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225EB08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,46
  r4 = uint64_t(int64_t(46));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225EB24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8225EB2C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8225EB40: lifted from generated/ by lift.py.
lifted::Result f8225EB40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8225EB40 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225EB68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r4,47
  r4 = uint64_t(int64_t(47));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EB78, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EB84, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225EBA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,46
  r4 = uint64_t(int64_t(46));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EBAC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8225EBC0: lifted from generated/ by lift.py.
lifted::Result f8225EBC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8225EBC0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31776);
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::GarageStage(base, uint32_t(r31)).index_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::GarageStage(base, uint32_t(r11)).model_claimed_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8225ec0c
  if (!cr6.eq) goto loc_8225EC0C;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,26248
  r11 = uint64_t(int64_t(int64_t(r11) + 26248));
  // lwz r3,12(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 12);
  // bl 0x8215aad0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarModels::BlockOf(base, LIFTED_ARGS(0x8225EBFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8217fcb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8217FCB0, LIFTED_ARGS(0x8225EC0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8225EC0C:
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::Command(base, LIFTED_ARGS(0x8225EC18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x82178de8
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetPlates(base, LIFTED_ARGS(0x8225EC24, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8225e5c8
  // (left out: dead in C++)
  { const lifted::Result o = f8225E5C8(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24800(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24800);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8225ec40
  if (cr0.eq) goto loc_8225EC40;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8225EC40:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
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

// sub_8225EC70: lifted from generated/ by lift.py.
lifted::Result f8225EC70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8225EC70 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r31,r3
  r31 = r3;
  // lwz r3,24804(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x8225EC8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24804(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8225EC98, r31)); r3 = o.r3; f1 = o.f1; }
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // li r26,2
  r26 = uint64_t(int64_t(2));
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // beq 0x8225edd0
  if (cr0.eq) goto loc_8225EDD0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8225ECB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x8225ed98
  if (!cr6.eq) goto loc_8225ED98;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // mr r11,r25
  r11 = r25;
  // stw r11,24996(r10)
  guest::StoreU32(base, uint32_t(r10) + 24996, uint32_t(r11));
  // lwz r10,24800(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 24800);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x8225ece8
  if (cr6.eq) goto loc_8225ECE8;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_8225ECE8:
  // stw r26,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r26));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // stw r28,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r28));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // stw r28,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r28));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // li r10,32
  r10 = uint64_t(int64_t(32));
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 604);
  // mr r8,r25
  r8 = r25;
  // mr r7,r25
  r7 = r25;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
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
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225ED58, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x8225ED6C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8225ED80, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x8225ED94, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8225eed4
  goto loc_8225EED4;
loc_8225ED98:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24960);
  // stw r25,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r25));
  // lwz r11,24800(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 24800);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x8225edb8
  if (cr6.eq) goto loc_8225EDB8;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8225EDB8:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r28,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r28));
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // b 0x8225eed4
  goto loc_8225EED4;
loc_8225EDD0:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24804(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8225EDDC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8225eed4
  if (cr0.eq) goto loc_8225EED4;
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // lwz r29,16(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 16);
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8225EDF4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpw cr6,r31,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r29), so);
  // beq cr6,0x8225eed4
  if (cr6.eq) goto loc_8225EED4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225EE1C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r31,7
  r4 = uint64_t(int64_t(int64_t(r31) + 7));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EE2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r29,7
  r4 = uint64_t(int64_t(int64_t(r29) + 7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225EE48, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8225EE50, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EE5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EE68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r29,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mulli r3,r11,40
  r3 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(40))));
  // bl 0x8225ea38
  // (left out: dead in C++)
  r3 = f8225EA38(base, r1, r13, r3);
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,6016
  r11 = uint64_t(int64_t(int64_t(r11) + 6016));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8225EEB0, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x8225EEC4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x8225EED4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8225EED4:
  // lwz r11,24804(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24804);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8225ef10
  if (cr6.eq) goto loc_8225EF10;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8225EEEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24800(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 24800);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8225ef00
  if (cr0.eq) goto loc_8225EF00;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8225EF00:
  // stw r26,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r26));
  // stw r28,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r28));
  // stw r25,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r25));
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
loc_8225EF10:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8225EF18: lifted from generated/ by lift.py.
lifted::Result f8225EF18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8225EF18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225EF40, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8225f060
  if (!cr6.eq) goto loc_8225F060;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24804(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24804);
  // lwz r30,4(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r30,7
  r4 = uint64_t(int64_t(int64_t(r30) + 7));
  // lwz r28,8(r11)
  r28 = guest::LoadU32(base, uint32_t(r11) + 8);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EF70, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r29,7
  r4 = uint64_t(int64_t(int64_t(r29) + 7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225EF8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8225EF94, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EFA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8225EFAC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x8225efc4
  if (!cr6.eq) goto loc_8225EFC4;
  // cmpwi cr6,r29,3
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 3, so);
  // bne cr6,0x8225efe0
  if (!cr6.eq) goto loc_8225EFE0;
  // li r3,120
  r3 = uint64_t(int64_t(120));
  // b 0x8225efd8
  goto loc_8225EFD8;
loc_8225EFC4:
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // bne cr6,0x8225efe0
  if (!cr6.eq) goto loc_8225EFE0;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x8225efe0
  if (!cr6.eq) goto loc_8225EFE0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8225EFD8:
  // bl 0x8225ea38
  // (left out: dead in C++)
  r3 = f8225EA38(base, r1, r13, r3);
  // b 0x8225f014
  goto loc_8225F014;
loc_8225EFE0:
  // cmpwi cr6,r28,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), -1, so);
  // bne cr6,0x8225eff0
  if (!cr6.eq) goto loc_8225EFF0;
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // b 0x8225effc
  goto loc_8225EFFC;
loc_8225EFF0:
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // bne cr6,0x8225f014
  if (!cr6.eq) goto loc_8225F014;
  // li r4,18
  r4 = uint64_t(int64_t(18));
loc_8225EFFC:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8225F014, r31)); r3 = o.r3; f1 = o.f1; }
loc_8225F014:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // rlwinm r10,r29,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,6016
  r11 = uint64_t(int64_t(int64_t(r11) + 6016));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8225F034, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x8225F048, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x8225F058, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r11));
loc_8225F060:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8225F06C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8225F078: lifted from generated/ by lift.py.
lifted::Result f8225F078(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_8225F078 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lis r7,-32218
  r7 = uint64_t(int64_t(-2111438848));
  // lis r8,-32240
  r8 = uint64_t(int64_t(-2112880640));
  // lis r9,-32240
  r9 = uint64_t(int64_t(-2112880640));
  // lis r10,-32218
  r10 = uint64_t(int64_t(-2111438848));
  // lwz r11,24800(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24800);
  // addi r7,r7,-5008
  r7 = uint64_t(int64_t(int64_t(r7) + -5008));
  // addi r8,r8,25920
  r8 = uint64_t(int64_t(int64_t(r8) + 25920));
  // addi r9,r9,25920
  r9 = uint64_t(int64_t(int64_t(r9) + 25920));
  // addi r10,r10,-7432
  r10 = uint64_t(int64_t(int64_t(r10) + -7432));
  // mr r6,r3
  r6 = r3;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r3,r4
  r3 = r4;
  // stw r7,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r7));
  // stw r8,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r8));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // stw r9,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r9));
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // beq cr6,0x8225f0e8
  if (cr6.eq) goto loc_8225F0E8;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r6
  r4 = r6;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8225F0E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8225F0E8:
  // bl 0x82293460
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82293460, LIFTED_ARGS(0x8225F0EC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8225F100: lifted from generated/ by lift.py.
lifted::Result f8225F100(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_8225F100 (lifted)");
  uint64_t r3 = 0, r4 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -4096);
  // stwu r1,-6032(r1)
  ea = -6032 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,32
  r11 = uint64_t(int64_t(32));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8225F128, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // lis r10,-32163
  r10 = uint64_t(int64_t(-2107834368));
  // lis r6,-32218
  r6 = uint64_t(int64_t(-2111438848));
  // addi r10,r10,-31856
  r10 = uint64_t(int64_t(int64_t(r10) + -31856));
  // lis r7,-32218
  r7 = uint64_t(int64_t(-2111438848));
  // lwz r11,24800(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24800);
  // lis r8,-32218
  r8 = uint64_t(int64_t(-2111438848));
  // lis r9,-32218
  r9 = uint64_t(int64_t(-2111438848));
  // addi r6,r6,-6256
  r6 = uint64_t(int64_t(int64_t(r6) + -6256));
  // addi r7,r7,-5736
  r7 = uint64_t(int64_t(int64_t(r7) + -5736));
  // stw r10,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r10));
  // addi r8,r8,-4328
  r8 = uint64_t(int64_t(int64_t(r8) + -4328));
  // addi r9,r9,-5928
  r9 = uint64_t(int64_t(int64_t(r9) + -5928));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // stw r6,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r6));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // stw r7,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r7));
  // stw r8,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r8));
  // stw r9,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r9));
  // beq cr6,0x8225f200
  if (cr6.eq) goto loc_8225F200;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x8225f1e0
  if (cr6.lt) goto loc_8225F1E0;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8225F1A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24800(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24800);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8225f214
  if (cr0.eq) goto loc_8225F214;
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8225f1c4
  if (!cr6.eq) goto loc_8225F1C4;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x8225f1d4
  goto loc_8225F1D4;
loc_8225F1C4:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
loc_8225F1C8:
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // stw r29,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r29));
loc_8225F1D4:
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_8225F1D8:
  // stw r29,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r29));
  // b 0x8225f214
  goto loc_8225F214;
loc_8225F1E0:
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8225f1c8
  if (!cr6.eq) goto loc_8225F1C8;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // b 0x8225f1d8
  goto loc_8225F1D8;
loc_8225F200:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x8225f214
  if (cr6.eq) goto loc_8225F214;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8225F214, r31)); r3 = o.r3; f1 = o.f1; }
loc_8225F214:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31776);
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, schema::StandHolder(base, uint32_t(r11)).stage_block_0_address());
  // mr r3,r30
  r3 = r30;
  // bl 0x82179be0
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::IsBusy(base, LIFTED_ARGS(0x8225F228, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // addi r31,r11,24808
  r31 = uint64_t(int64_t(int64_t(r11) + 24808));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20);
  // beq 0x8225f25c
  if (cr0.eq) goto loc_8225F25C;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8225f270
  if (!cr6.eq) goto loc_8225F270;
  // bl 0x8225eaa8
  // (left out: dead in C++)
  { const lifted::Result o = f8225EAA8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
  // b 0x8225f270
  goto loc_8225F270;
loc_8225F25C:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8225f270
  if (!cr6.eq) goto loc_8225F270;
  // bl 0x8225eb40
  // (left out: dead in C++)
  { const lifted::Result o = f8225EB40(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r29,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r29));
loc_8225F270:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8225f308
  if (!cr6.eq) goto loc_8225F308;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r4,r11,26248
  r4 = uint64_t(int64_t(int64_t(r11) + 26248));
  // li r5,1968
  r5 = uint64_t(int64_t(1968));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8225F294, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // lwz r4,140(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 140);
  // bl 0x8217f880
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::ReloadModel(base, LIFTED_ARGS(0x8225F2A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r3,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r3));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8225f324
  if (cr6.eq) goto loc_8225F324;
  // addi r3,r1,2096
  r3 = uint64_t(int64_t(int64_t(r1) + 2096));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x8225F2C0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,2096
  r3 = uint64_t(int64_t(int64_t(r1) + 2096));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8225F2D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,5984(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 5984);
  // addi r3,r1,2096
  r3 = uint64_t(int64_t(int64_t(r1) + 2096));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,5984(r1)
  guest::StoreU32(base, uint32_t(r1) + 5984, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x8225F2E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r3,r30
  r3 = r30;
  // lwz r4,28224(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 28224);
  // bl 0x82178de8
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetPlates(base, LIFTED_ARGS(0x8225F2F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,2096
  r4 = uint64_t(int64_t(int64_t(r1) + 2096));
  // mr r3,r30
  r3 = r30;
  // lha r5,2096(r1)
  r5 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r1) + 2096))));
  // bl 0x8217fd58
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetCar(base, LIFTED_ARGS(0x8225F304, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8225f324
  goto loc_8225F324;
loc_8225F308:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetDrawWord(base, LIFTED_ARGS(0x8225F314, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8217fe60
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::Draw(base, LIFTED_ARGS(0x8225F324, r31)); r3 = o.r3; f1 = o.f1; }
loc_8225F324:
  // addi r1,r1,6032
  r1 = uint64_t(int64_t(int64_t(r1) + 6032));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8225FB88: lifted from generated/ by lift.py.
void f8225FB88(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8225FB88 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // std r30,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  guest::StoreU64(base, uint32_t(r1) + -8, r31);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r4,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r8,76(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmplwi r8,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq 0x8225fbdc
  if (cr0.eq) goto loc_8225FBDC;
  // lwz r9,84(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FBB8:
  // lis r6,8192
  r6 = uint64_t(int64_t(536870912));
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, uint32_t(r9) + 0);
  // ori r6,r6,16
  r6 = r6 | 16;
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8225fbdc
  if (cr6.eq) goto loc_8225FBDC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,24
  r9 = uint64_t(int64_t(int64_t(r9) + 24));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x8225fbb8
  if (cr6.lt) goto loc_8225FBB8;
loc_8225FBDC:
  // lwz r9,84(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r9,r11,r9
  r9 = r11 + r9;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // ld r11,16(r9)
  r11 = guest::LoadU64(base, uint32_t(r9) + 16);
  // lbz r9,16(r9)
  r9 = guest::LoadU8(base, uint32_t(r9) + 16);
  // stb r11,7(r3)
  guest::StoreU8(base, uint32_t(r3) + 7, uint8_t(r11));
  // stb r9,0(r3)
  guest::StoreU8(base, uint32_t(r3) + 0, uint8_t(r9));
  // sradi r9,r11,48
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFFFFFFFFFF) != 0);
  r9 = uint64_t(int64_t(int64_t(r11) >> 48));
  // sradi r6,r11,40
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFFFFFFFF) != 0);
  r6 = uint64_t(int64_t(int64_t(r11) >> 40));
  // sradi r5,r11,32
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFFFFFF) != 0);
  r5 = uint64_t(int64_t(int64_t(r11) >> 32));
  // sradi r4,r11,24
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFFFF) != 0);
  r4 = uint64_t(int64_t(int64_t(r11) >> 24));
  // sradi r31,r11,16
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFF) != 0);
  r31 = uint64_t(int64_t(int64_t(r11) >> 16));
  // sradi r30,r11,8
  ca = (int64_t(r11) < 0) & ((r11 & 0xFF) != 0);
  r30 = uint64_t(int64_t(int64_t(r11) >> 8));
  // stb r9,1(r3)
  guest::StoreU8(base, uint32_t(r3) + 1, uint8_t(r9));
  // stb r6,2(r3)
  guest::StoreU8(base, uint32_t(r3) + 2, uint8_t(r6));
  // stb r5,3(r3)
  guest::StoreU8(base, uint32_t(r3) + 3, uint8_t(r5));
  // stb r4,4(r3)
  guest::StoreU8(base, uint32_t(r3) + 4, uint8_t(r4));
  // stb r31,5(r3)
  guest::StoreU8(base, uint32_t(r3) + 5, uint8_t(r31));
  // stb r30,6(r3)
  guest::StoreU8(base, uint32_t(r3) + 6, uint8_t(r30));
  // beq cr6,0x8225fc5c
  if (cr6.eq) goto loc_8225FC5C;
  // lwz r11,84(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FC38:
  // lis r6,8192
  r6 = uint64_t(int64_t(536870912));
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // ori r6,r6,17
  r6 = r6 | 17;
  // cmplw cr6,r9,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r6), so);
  // beq cr6,0x8225fc5c
  if (cr6.eq) goto loc_8225FC5C;
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // cmplw cr6,r7,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r8), so);
  // blt cr6,0x8225fc38
  if (cr6.lt) goto loc_8225FC38;
loc_8225FC5C:
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r7,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(24))));
  // add r10,r11,r10
  r10 = r11 + r10;
  // ld r11,16(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 16);
  // lbz r10,16(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 16);
  // stb r11,15(r3)
  guest::StoreU8(base, uint32_t(r3) + 15, uint8_t(r11));
  // stb r10,8(r3)
  guest::StoreU8(base, uint32_t(r3) + 8, uint8_t(r10));
  // sradi r10,r11,48
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFFFFFFFFFF) != 0);
  r10 = uint64_t(int64_t(int64_t(r11) >> 48));
  // sradi r9,r11,40
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFFFFFFFF) != 0);
  r9 = uint64_t(int64_t(int64_t(r11) >> 40));
  // sradi r8,r11,32
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFFFFFF) != 0);
  r8 = uint64_t(int64_t(int64_t(r11) >> 32));
  // sradi r7,r11,24
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFFFF) != 0);
  r7 = uint64_t(int64_t(int64_t(r11) >> 24));
  // sradi r6,r11,16
  ca = (int64_t(r11) < 0) & ((r11 & 0xFFFF) != 0);
  r6 = uint64_t(int64_t(int64_t(r11) >> 16));
  // sradi r5,r11,8
  ca = (int64_t(r11) < 0) & ((r11 & 0xFF) != 0);
  r5 = uint64_t(int64_t(int64_t(r11) >> 8));
  // stb r10,9(r3)
  guest::StoreU8(base, uint32_t(r3) + 9, uint8_t(r10));
  // stb r9,10(r3)
  guest::StoreU8(base, uint32_t(r3) + 10, uint8_t(r9));
  // stb r8,11(r3)
  guest::StoreU8(base, uint32_t(r3) + 11, uint8_t(r8));
  // stb r7,12(r3)
  guest::StoreU8(base, uint32_t(r3) + 12, uint8_t(r7));
  // stb r6,13(r3)
  guest::StoreU8(base, uint32_t(r3) + 13, uint8_t(r6));
  // stb r5,14(r3)
  guest::StoreU8(base, uint32_t(r3) + 14, uint8_t(r5));
  // ld r30,-16(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -16);
  // ld r31,-8(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -8);
  // blr
  return;
}

// sub_8225FCB8: lifted from generated/ by lift.py.
uint64_t f8225FCB8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225FCB8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r3,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,76(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8225fd04
  if (cr0.eq) goto loc_8225FD04;
  // lwz r8,84(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FCE0:
  // lis r6,8192
  r6 = uint64_t(int64_t(536870912));
  // lwz r7,0(r8)
  r7 = guest::LoadU32(base, uint32_t(r8) + 0);
  // ori r6,r6,18
  r6 = r6 | 18;
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8225fd04
  if (cr6.eq) goto loc_8225FD04;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,24
  r8 = uint64_t(int64_t(int64_t(r8) + 24));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8225fce0
  if (cr6.lt) goto loc_8225FCE0;
loc_8225FD04:
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // ld r3,16(r11)
  r3 = guest::LoadU64(base, uint32_t(r11) + 16);
  // blr
  return r3;
}

// sub_8225FD18: lifted from generated/ by lift.py.
uint64_t f8225FD18(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225FD18 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r3,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,76(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8225fd64
  if (cr0.eq) goto loc_8225FD64;
  // lwz r8,84(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FD40:
  // lis r6,4096
  r6 = uint64_t(int64_t(268435456));
  // lwz r7,0(r8)
  r7 = guest::LoadU32(base, uint32_t(r8) + 0);
  // ori r6,r6,2
  r6 = r6 | 2;
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8225fd64
  if (cr6.eq) goto loc_8225FD64;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,24
  r8 = uint64_t(int64_t(int64_t(r8) + 24));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8225fd40
  if (cr6.lt) goto loc_8225FD40;
loc_8225FD64:
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r3,16(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 16);
  // blr
  return r3;
}

// sub_8225FD78: lifted from generated/ by lift.py.
uint64_t f8225FD78(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225FD78 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r3,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,80(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 80);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8225fdbc
  if (cr0.eq) goto loc_8225FDBC;
  // lwz r8,88(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 88);
loc_8225FDA0:
  // lwz r7,0(r8)
  r7 = guest::LoadU32(base, uint32_t(r8) + 0);
  // cmplwi cr6,r7,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 2, so);
  // beq cr6,0x8225fdbc
  if (cr6.eq) goto loc_8225FDBC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,8
  r8 = uint64_t(int64_t(int64_t(r8) + 8));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8225fda0
  if (cr6.lt) goto loc_8225FDA0;
loc_8225FDBC:
  // lwz r10,88(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 88);
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r3,4(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 4);
  // blr
  return r3;
}

// sub_8225FDD0: lifted from generated/ by lift.py.
uint64_t f8225FDD0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225FDD0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r3,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,76(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8225fe1c
  if (cr0.eq) goto loc_8225FE1C;
  // lwz r8,84(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FDF8:
  // lis r6,4096
  r6 = uint64_t(int64_t(268435456));
  // lwz r7,0(r8)
  r7 = guest::LoadU32(base, uint32_t(r8) + 0);
  // ori r6,r6,10
  r6 = r6 | 10;
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8225fe1c
  if (cr6.eq) goto loc_8225FE1C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,24
  r8 = uint64_t(int64_t(int64_t(r8) + 24));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8225fdf8
  if (cr6.lt) goto loc_8225FDF8;
loc_8225FE1C:
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r3,16(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 16);
  // blr
  return r3;
}

// sub_8225FE30: lifted from generated/ by lift.py.
uint64_t f8225FE30(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225FE30 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r3,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,76(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8225fe7c
  if (cr0.eq) goto loc_8225FE7C;
  // lwz r8,84(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FE58:
  // lis r6,8192
  r6 = uint64_t(int64_t(536870912));
  // lwz r7,0(r8)
  r7 = guest::LoadU32(base, uint32_t(r8) + 0);
  // ori r6,r6,8
  r6 = r6 | 8;
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8225fe7c
  if (cr6.eq) goto loc_8225FE7C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,24
  r8 = uint64_t(int64_t(int64_t(r8) + 24));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8225fe58
  if (cr6.lt) goto loc_8225FE58;
loc_8225FE7C:
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // ld r11,16(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 16);
  // extsw r3,r11
  r3 = uint64_t(int64_t(int32_t(r11)));
  // blr
  return r3;
}

// sub_8225FE98: lifted from generated/ by lift.py.
uint64_t f8225FE98(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225FE98 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r3,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,76(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8225fee4
  if (cr0.eq) goto loc_8225FEE4;
  // lwz r8,84(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FEC0:
  // lis r6,8192
  r6 = uint64_t(int64_t(536870912));
  // lwz r7,0(r8)
  r7 = guest::LoadU32(base, uint32_t(r8) + 0);
  // ori r6,r6,4
  r6 = r6 | 4;
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8225fee4
  if (cr6.eq) goto loc_8225FEE4;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,24
  r8 = uint64_t(int64_t(int64_t(r8) + 24));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8225fec0
  if (cr6.lt) goto loc_8225FEC0;
loc_8225FEE4:
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // ld r11,16(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 16);
  // extsw r3,r11
  r3 = uint64_t(int64_t(int32_t(r11)));
  // blr
  return r3;
}

// sub_8225FF00: lifted from generated/ by lift.py.
uint64_t f8225FF00(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225FF00 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r3,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,76(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8225ff4c
  if (cr0.eq) goto loc_8225FF4C;
  // lwz r8,84(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FF28:
  // lis r6,8192
  r6 = uint64_t(int64_t(536870912));
  // lwz r7,0(r8)
  r7 = guest::LoadU32(base, uint32_t(r8) + 0);
  // ori r6,r6,5
  r6 = r6 | 5;
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8225ff4c
  if (cr6.eq) goto loc_8225FF4C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,24
  r8 = uint64_t(int64_t(int64_t(r8) + 24));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8225ff28
  if (cr6.lt) goto loc_8225FF28;
loc_8225FF4C:
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // ld r11,16(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 16);
  // extsw r3,r11
  r3 = uint64_t(int64_t(int32_t(r11)));
  // blr
  return r3;
}

// sub_8225FF68: lifted from generated/ by lift.py.
uint64_t f8225FF68(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8225FF68 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r10,r3,92
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(92))));
  // lwz r11,27312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27312);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // add r10,r11,r10
  r10 = r11 + r10;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,76(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8225ffb4
  if (cr0.eq) goto loc_8225FFB4;
  // lwz r8,84(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 84);
loc_8225FF90:
  // lis r6,4096
  r6 = uint64_t(int64_t(268435456));
  // lwz r7,0(r8)
  r7 = guest::LoadU32(base, uint32_t(r8) + 0);
  // ori r6,r6,7
  r6 = r6 | 7;
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8225ffb4
  if (cr6.eq) goto loc_8225FFB4;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,24
  r8 = uint64_t(int64_t(int64_t(r8) + 24));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8225ff90
  if (cr6.lt) goto loc_8225FF90;
loc_8225FFB4:
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 84);
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r3,16(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 16);
  // blr
  return r3;
}

// sub_82260170: lifted from generated/ by lift.py.
uint64_t f82260170(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260170 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822601a4
  if (cr6.lt) goto loc_822601A4;
  // beq cr6,0x8226019c
  if (cr6.eq) goto loc_8226019C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82260194
  if (cr6.lt) goto loc_82260194;
  // bnelr cr6
  if (!cr6.eq) return r3;
  // blr
  return r3;
loc_82260194:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // blr
  return r3;
loc_8226019C:
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
loc_822601A4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_822601B0: lifted from generated/ by lift.py.
uint64_t f822601B0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822601B0 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822601e8
  if (cr6.lt) goto loc_822601E8;
  // beq cr6,0x822601e0
  if (cr6.eq) goto loc_822601E0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x822601d8
  if (cr6.lt) goto loc_822601D8;
  // bnelr cr6
  if (!cr6.eq) return r3;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
loc_822601D8:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
loc_822601E0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_822601E8:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // blr
  return r3;
}

// sub_822601F0: lifted from generated/ by lift.py.
uint64_t f822601F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822601F0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82260210, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmpwi cr6,r31,1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 1, so);
  // bne cr6,0x82260260
  if (!cr6.eq) goto loc_82260260;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r10,r30
  r10 = r30;
  // lwz r11,29432(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29432);
  // mr r3,r11
  r3 = r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82260268
  if (cr6.eq) goto loc_82260268;
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lwz r9,31060(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 31060);
loc_8226023C:
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // slw r8,r8,r10
  r8 = uint8_t(r10) & 0x20 ? 0 : (uint32_t(r8) << (uint8_t(r10) & 0x3F));
  // and. r8,r8,r9
  r8 = r8 & r9;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x82260250
  if (!cr0.eq) goto loc_82260250;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
loc_82260250:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x8226023c
  if (cr6.lt) goto loc_8226023C;
  // b 0x82260268
  goto loc_82260268;
loc_82260260:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82260268, r31)); r3 = o.r3; f1 = o.f1; }
loc_82260268:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,27348(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // lwz r9,4(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r5,8(r9)
  r5 = guest::LoadU32(base, uint32_t(r9) + 8);
  // cmplwi r5,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // beq 0x82260358
  if (cr0.eq) goto loc_82260358;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r7,12(r9)
  r7 = guest::LoadU32(base, uint32_t(r9) + 12);
  // rlwinm r6,r3,3,0,28
  r6 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 3) & 0xFFFFFFF8;
  // addi r8,r11,30320
  r8 = uint64_t(int64_t(int64_t(r11) + 30320));
  // mr r11,r30
  r11 = r30;
  // addi r4,r8,144
  r4 = uint64_t(int64_t(int64_t(r8) + 144));
  // mr r9,r30
  r9 = r30;
  // ldx r6,r6,r4
  r6 = guest::LoadU64(base, uint32_t(r6) + uint32_t(r4));
loc_822602A0:
  // ld r4,0(r7)
  r4 = guest::LoadU64(base, uint32_t(r7) + 0);
  // cmpld cr6,r6,r4
  cr6 = lifted::Compare<uint64_t>(r6, r4, so);
  // bne cr6,0x822602b0
  if (!cr6.eq) goto loc_822602B0;
  // mr r11,r9
  r11 = r9;
loc_822602B0:
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r7,r7,48
  r7 = uint64_t(int64_t(int64_t(r7) + 48));
  // cmplw cr6,r9,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r5), so);
  // blt cr6,0x822602a0
  if (cr6.lt) goto loc_822602A0;
  // lwz r7,4(r10)
  r7 = guest::LoadU32(base, uint32_t(r10) + 4);
  // mulli r11,r11,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(48))));
  // addi r6,r8,332
  r6 = uint64_t(int64_t(int64_t(r8) + 332));
  // lwz r5,12(r7)
  r5 = guest::LoadU32(base, uint32_t(r7) + 12);
  // addi r7,r8,348
  r7 = uint64_t(int64_t(int64_t(r8) + 348));
  // addi r4,r8,364
  r4 = uint64_t(int64_t(int64_t(r8) + 364));
  // add r8,r5,r11
  r8 = r5 + r11;
  // lis r5,-32127
  r5 = uint64_t(int64_t(-2105475072));
  // rlwinm r9,r3,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // add r7,r9,r7
  r7 = r9 + r7;
  // lwz r8,8(r8)
  r8 = guest::LoadU32(base, uint32_t(r8) + 8);
  // stw r8,24864(r5)
  guest::StoreU32(base, uint32_t(r5) + 24864, uint32_t(r8));
  // add r8,r9,r6
  r8 = r9 + r6;
  // lwz r5,4(r10)
  r5 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r6,12(r5)
  r6 = guest::LoadU32(base, uint32_t(r5) + 12);
  // lis r5,-32127
  r5 = uint64_t(int64_t(-2105475072));
  // add r6,r6,r11
  r6 = r6 + r11;
  // lwz r6,44(r6)
  r6 = guest::LoadU32(base, uint32_t(r6) + 44);
  // ld r6,64(r6)
  r6 = guest::LoadU64(base, uint32_t(r6) + 64);
  // stw r6,24872(r5)
  guest::StoreU32(base, uint32_t(r5) + 24872, uint32_t(r6));
  // lwz r6,4(r10)
  r6 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r6,12(r6)
  r6 = guest::LoadU32(base, uint32_t(r6) + 12);
  // add r6,r6,r11
  r6 = r6 + r11;
  // ld r6,16(r6)
  r6 = guest::LoadU64(base, uint32_t(r6) + 16);
  // stw r6,0(r8)
  guest::StoreU32(base, uint32_t(r8) + 0, uint32_t(r6));
  // lwz r6,4(r10)
  r6 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r6,12(r6)
  r6 = guest::LoadU32(base, uint32_t(r6) + 12);
  // add r6,r6,r11
  r6 = r6 + r11;
  // lwz r6,44(r6)
  r6 = guest::LoadU32(base, uint32_t(r6) + 44);
  // ld r6,16(r6)
  r6 = guest::LoadU64(base, uint32_t(r6) + 16);
  // stwx r6,r9,r4
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r4), uint32_t(r6));
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // ld r11,40(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 40);
  // stw r11,0(r7)
  guest::StoreU32(base, uint32_t(r7) + 0, uint32_t(r11));
  // b 0x82260398
  goto loc_82260398;
loc_82260358:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r7,-32127
  r7 = uint64_t(int64_t(-2105475072));
  // addi r10,r10,30320
  r10 = uint64_t(int64_t(int64_t(r10) + 30320));
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r10,332
  r8 = uint64_t(int64_t(int64_t(r10) + 332));
  // addi r9,r10,348
  r9 = uint64_t(int64_t(int64_t(r10) + 348));
  // addi r6,r10,364
  r6 = uint64_t(int64_t(int64_t(r10) + 364));
  // mr r10,r30
  r10 = r30;
  // add r8,r11,r8
  r8 = r11 + r8;
  // stw r10,24864(r7)
  guest::StoreU32(base, uint32_t(r7) + 24864, uint32_t(r10));
  // lis r7,-32127
  r7 = uint64_t(int64_t(-2105475072));
  // stw r30,0(r8)
  guest::StoreU32(base, uint32_t(r8) + 0, uint32_t(r30));
  // stwx r30,r11,r6
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r6), uint32_t(r30));
  // stw r10,24872(r7)
  guest::StoreU32(base, uint32_t(r7) + 24872, uint32_t(r10));
  // add r7,r11,r9
  r7 = r11 + r9;
  // stw r30,0(r7)
  guest::StoreU32(base, uint32_t(r7) + 0, uint32_t(r30));
loc_82260398:
  // lwz r11,0(r7)
  r11 = guest::LoadU32(base, uint32_t(r7) + 0);
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822603b8
  if (cr0.eq) goto loc_822603B8;
  // lwz r9,0(r8)
  r9 = guest::LoadU32(base, uint32_t(r8) + 0);
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
  // divwu r11,r9,r11
  r11 = uint32_t(uint32_t(r11) ? uint32_t(r9) / uint32_t(r11) : 0);
  // b 0x822603bc
  goto loc_822603BC;
loc_822603B8:
  // mr r11,r30
  r11 = r30;
loc_822603BC:
  // stw r11,24868(r10)
  guest::StoreU32(base, uint32_t(r10) + 24868, uint32_t(r11));
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

// sub_822603D8: lifted from generated/ by lift.py.
lifted::Result f822603D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822603D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // bne cr6,0x82260430
  if (!cr6.eq) goto loc_82260430;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,29432(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 29432);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r10
  r3 = r10;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82260438
  if (cr6.eq) goto loc_82260438;
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lwz r9,31060(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 31060);
loc_8226040C:
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // slw r8,r8,r11
  r8 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r8) << (uint8_t(r11) & 0x3F));
  // and. r8,r8,r9
  r8 = r8 & r9;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x82260420
  if (!cr0.eq) goto loc_82260420;
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
loc_82260420:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8226040c
  if (cr6.lt) goto loc_8226040C;
  // b 0x82260438
  goto loc_82260438;
loc_82260430:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82260438, r31)); r3 = o.r3; f1 = o.f1; }
loc_82260438:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // rlwinm r10,r3,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 3) & 0xFFFFFFF8;
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r11,r11,144
  r11 = uint64_t(int64_t(int64_t(r11) + 144));
  // ldx r4,r10,r11
  r4 = guest::LoadU64(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x821495e8
  // (left out: dead in C++)
  { const lifted::Result o = f821495E8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82260468: lifted from generated/ by lift.py.
uint64_t f82260468(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260468 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-2016(r1)
  ea = -2016 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r4,r11,27416
  r4 = uint64_t(int64_t(int64_t(r11) + 27416));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,764
  r5 = uint64_t(int64_t(764));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8226048C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,1488
  r3 = uint64_t(int64_t(int64_t(r1) + 1488));
  // addi r4,r11,28184
  r4 = uint64_t(int64_t(int64_t(r11) + 28184));
  // li r5,480
  r5 = uint64_t(int64_t(480));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822604A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,848
  r3 = uint64_t(int64_t(int64_t(r1) + 848));
  // addi r4,r11,28664
  r4 = uint64_t(int64_t(int64_t(r11) + 28664));
  // li r5,640
  r5 = uint64_t(int64_t(640));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822604B4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820cd7f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Clear(base, LIFTED_ARGS(0x822604BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r8,86(r1)
  r8 = guest::LoadU16(base, uint32_t(r1) + 86);
  // lhz r7,88(r1)
  r7 = guest::LoadU16(base, uint32_t(r1) + 88);
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // rotlwi r8,r8,16
  r8 = __builtin_rotateleft32(uint32_t(r8), 16);
  // lhz r11,80(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 80);
  // lhz r10,82(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 82);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // or r8,r7,r8
  r8 = r7 | r8;
  // lhz r9,84(r1)
  r9 = guest::LoadU16(base, uint32_t(r1) + 84);
  // lwz r7,92(r1)
  r7 = guest::LoadU32(base, uint32_t(r1) + 92);
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // sth r11,0(r31)
  guest::StoreU16(base, schema::SysLiveBlock1592(base, uint32_t(r31)).unknown_0_address(), uint16_t(r11));
  // sth r10,2(r31)
  guest::StoreU16(base, schema::SysLiveBlock1592(base, uint32_t(r31)).unknown_2_address(), uint16_t(r10));
  // sth r9,4(r31)
  guest::StoreU16(base, schema::SysLiveBlock1592(base, uint32_t(r31)).unknown_4_address(), uint16_t(r9));
  // stw r8,8(r31)
  guest::StoreU32(base, schema::SysLiveBlock1592(base, uint32_t(r31)).unknown_8_address(), uint32_t(r8));
  // stw r7,12(r31)
  guest::StoreU32(base, schema::SysLiveBlock1592(base, uint32_t(r31)).unknown_12_address(), uint32_t(r7));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82260500, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,96
  r3 = uint64_t(int64_t(int64_t(r31) + 96));
  // addi r4,r1,176
  r4 = uint64_t(int64_t(int64_t(r1) + 176));
  // li r5,192
  r5 = uint64_t(int64_t(192));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82260510, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r9,r31,1568
  r9 = uint64_t(int64_t(int64_t(r31) + 1568));
  // addi r10,r1,368
  r10 = uint64_t(int64_t(int64_t(r1) + 368));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // mtctr r11
  ctr = r11;
loc_82260520:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r11,0(r9)
  guest::StoreU32(base, schema::SysLiveBlock1592(base, uint32_t(r9) - 1568).unknown_1568_address(), uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x82260520
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82260520;
  // addi r10,r31,1588
  r10 = uint64_t(int64_t(int64_t(r31) + 1588));
  // addi r11,r1,388
  r11 = uint64_t(int64_t(int64_t(r1) + 388));
  // li r9,15
  r9 = uint64_t(int64_t(15));
  // mtctr r9
  ctr = r9;
loc_82260544:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r9,0(r10)
  guest::StoreU32(base, schema::SysLiveBlock1592(base, uint32_t(r10) - 1588).unknown_1588_address(), uint32_t(r9));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x82260544
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82260544;
  // addi r3,r31,1648
  r3 = uint64_t(int64_t(int64_t(r31) + 1648));
  // addi r4,r1,448
  r4 = uint64_t(int64_t(int64_t(r1) + 448));
  // li r5,236
  r5 = uint64_t(int64_t(236));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82260568, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r31,288
  r10 = uint64_t(int64_t(int64_t(r31) + 288));
  // addi r11,r1,684
  r11 = uint64_t(int64_t(int64_t(r1) + 684));
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // mtctr r9
  ctr = r9;
loc_82260578:
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  guest::StoreU64(base, schema::SysLiveBlock1592(base, uint32_t(r10) - 288).unknown_288_address(), r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x82260578
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82260578;
  // addi r30,r1,1648
  r30 = uint64_t(int64_t(int64_t(r1) + 1648));
  // addi r29,r31,608
  r29 = uint64_t(int64_t(int64_t(r31) + 608));
  // li r28,5
  r28 = uint64_t(int64_t(5));
loc_82260598:
  // addi r4,r30,-160
  r4 = uint64_t(int64_t(int64_t(r30) + -160));
  // addi r3,r29,-160
  r3 = uint64_t(int64_t(int64_t(r29) + -160));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822605A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x822605B8, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r30,r30,32
  r30 = uint64_t(int64_t(int64_t(r30) + 32));
  // addi r29,r29,32
  r29 = uint64_t(int64_t(int64_t(r29) + 32));
  // bne 0x82260598
  if (!cr0.eq) goto loc_82260598;
  // addi r10,r31,768
  r10 = uint64_t(int64_t(int64_t(r31) + 768));
  // addi r11,r1,1808
  r11 = uint64_t(int64_t(int64_t(r1) + 1808));
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // mtctr r9
  ctr = r9;
loc_822605D8:
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  guest::StoreU64(base, schema::SysLiveBlock1592(base, uint32_t(r10) - 768).unknown_768_address(), r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x822605d8
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_822605D8;
  // addi r10,r31,928
  r10 = uint64_t(int64_t(int64_t(r31) + 928));
  // addi r11,r1,848
  r11 = uint64_t(int64_t(int64_t(r1) + 848));
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // mtctr r9
  ctr = r9;
loc_822605FC:
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  guest::StoreU64(base, schema::SysLiveBlock1592(base, uint32_t(r10) - 928).unknown_928_address(), r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x822605fc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_822605FC;
  // addi r10,r31,1088
  r10 = uint64_t(int64_t(int64_t(r31) + 1088));
  // addi r11,r1,1008
  r11 = uint64_t(int64_t(int64_t(r1) + 1008));
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // mtctr r9
  ctr = r9;
loc_82260620:
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  guest::StoreU64(base, schema::SysLiveBlock1592(base, uint32_t(r10) - 1088).unknown_1088_address(), r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x82260620
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82260620;
  // addi r30,r1,1328
  r30 = uint64_t(int64_t(int64_t(r1) + 1328));
  // addi r29,r31,1408
  r29 = uint64_t(int64_t(int64_t(r31) + 1408));
  // li r28,5
  r28 = uint64_t(int64_t(5));
loc_82260640:
  // addi r4,r30,-160
  r4 = uint64_t(int64_t(int64_t(r30) + -160));
  // addi r3,r29,-160
  r3 = uint64_t(int64_t(int64_t(r29) + -160));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82260650, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82260660, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r30,r30,32
  r30 = uint64_t(int64_t(int64_t(r30) + 32));
  // addi r29,r29,32
  r29 = uint64_t(int64_t(int64_t(r29) + 32));
  // bne 0x82260640
  if (!cr0.eq) goto loc_82260640;
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,2016
  r1 = uint64_t(int64_t(int64_t(r1) + 2016));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82260680: lifted from generated/ by lift.py.
void f82260680(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82260680 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,24816
  r11 = uint64_t(int64_t(int64_t(r11) + 24816));
  // subf r8,r11,r3
  r8 = r3 - r11;
loc_82260690:
  // add r9,r10,r11
  r9 = r10 + r11;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,16
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 16, so);
  // lbzx r7,r8,r9
  r7 = guest::LoadU8(base, uint32_t(r8) + uint32_t(r9));
  // stb r7,0(r9)
  guest::StoreU8(base, uint32_t(r9) + 0, uint8_t(r7));
  // blt cr6,0x82260690
  if (cr6.lt) goto loc_82260690;
  // std r4,16(r11)
  guest::StoreU64(base, uint32_t(r11) + 16, r4);
  // stw r5,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r5));
  // blr
  return;
}

// sub_822606B8: lifted from generated/ by lift.py.
void f822606B8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822606B8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,24816
  r11 = uint64_t(int64_t(int64_t(r11) + 24816));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r9,8(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 8);
  // ld r8,16(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 16);
  // ld r11,24(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 24);
  // std r10,0(r3)
  guest::StoreU64(base, schema::SysLiveBlock32(base, uint32_t(r3)).unknown_0_address(), r10);
  // std r9,8(r3)
  guest::StoreU64(base, schema::SysLiveBlock32(base, uint32_t(r3)).unknown_8_address(), r9);
  // std r8,16(r3)
  guest::StoreU64(base, schema::SysLiveBlock32(base, uint32_t(r3)).unknown_16_address(), r8);
  // std r11,24(r3)
  guest::StoreU64(base, schema::SysLiveBlock32(base, uint32_t(r3)).unknown_24_address(), r11);
  // blr
  return;
}

// sub_822606E8: lifted from generated/ by lift.py.
void f822606E8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822606E8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24876(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24876);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8226074c
  if (cr6.eq) goto loc_8226074C;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x8226074c
  if (cr6.eq) goto loc_8226074C;
  // cmpwi cr6,r3,5
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 5, so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // bne cr6,0x82260718
  if (!cr6.eq) goto loc_82260718;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24888(r10)
  guest::StoreU32(base, uint32_t(r10) + 24888, uint32_t(r11));
  // blr
  return;
loc_82260718:
  // cmpwi cr6,r3,4
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 4, so);
  // bne cr6,0x8226072c
  if (!cr6.eq) goto loc_8226072C;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24884(r10)
  guest::StoreU32(base, uint32_t(r10) + 24884, uint32_t(r11));
  // blr
  return;
loc_8226072C:
  // cmpwi cr6,r3,6
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 6, so);
  // bne cr6,0x82260740
  if (!cr6.eq) goto loc_82260740;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24892(r10)
  guest::StoreU32(base, uint32_t(r10) + 24892, uint32_t(r11));
  // blr
  return;
loc_82260740:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24880(r10)
  guest::StoreU32(base, uint32_t(r10) + 24880, uint32_t(r11));
  // blr
  return;
loc_8226074C:
  // stw r3,24876(r11)
  guest::StoreU32(base, uint32_t(r11) + 24876, uint32_t(r3));
  // blr
  return;
}

// sub_82260758: lifted from generated/ by lift.py.
lifted::Result f82260758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82260758 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // bl 0x8214d7a0
  // (left out: dead in C++)
  { const lifted::Result o = f8214D7A0(base, r1, r13, r6, r7, r9); r3 = o.r3; f1 = o.f1; }
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8226077C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,29420(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29420);
  // subf r11,r11,r3
  r11 = r3 - r11;
  // cmplwi cr6,r11,10000
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10000, so);
  // ble cr6,0x822607c4
  if (!cr6.gt) goto loc_822607C4;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::NetTimeoutFlag(base, uint32_t(r30)).flag_address(), uint32_t(r11));
  // stw r11,24876(r10)
  guest::StoreU32(base, uint32_t(r10) + 24876, uint32_t(r11));
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
loc_822607C4:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4096
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4096, so);
  // bne cr6,0x822607e0
  if (!cr6.eq) goto loc_822607E0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::NetTimeoutFlag(base, uint32_t(r30)).flag_address(), uint32_t(r11));
loc_822607E0:
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

// sub_82260908 game::Session::IsActive: lifted from generated/ by lift.py.
lifted::Result f82260908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82260908 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // addi r11,r11,-4096
  r11 = uint64_t(int64_t(int64_t(r11) + -4096));
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // bgt cr6,0x822609ac
  if (cr6.gt) goto loc_822609AC;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82260998
  if (!cr6.eq) goto loc_82260998;
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
  // ble cr6,0x82260964
  if (!cr6.gt) goto loc_82260964;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_82260964:
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
  // beq 0x822609ac
  if (cr0.eq) goto loc_822609AC;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226098c
  if (!cr6.gt) goto loc_8226098C;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226098C:
  // blt cr6,0x822609a0
  if (cr6.lt) goto loc_822609A0;
  // twi 31,r0,22
  lifted::Trap(22);
  // b 0x822609a0
  goto loc_822609A0;
loc_82260998:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
loc_822609A0:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r4,144(r11)
  r4 = guest::LoadU64(base, uint32_t(r11) + 144);
  // bl 0x82576ee0
  // (left out: dead in C++)
  { const lifted::Result o = f82576EE0(base, r1, r13, r3, r4, r5, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
loc_822609AC:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822609F0: lifted from generated/ by lift.py.
void f822609F0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822609F0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82260a08
  if (cr6.eq) goto loc_82260A08;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82260A08:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r7,9664(r3)
  guest::StoreU32(base, uint32_t(r3) + 9664, uint32_t(r7));
  // blr
  return;
}

// sub_82260A30: lifted from generated/ by lift.py.
lifted::Result f82260A30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82260A30 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r30
  r11 = r30;
  // mr r31,r3
  r31 = r3;
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
  // lwz r11,9652(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r31)).unknown_9652_address());
  // addic. r11,r11,-50
  ca = uint32_t(r11) > 49;
  r11 = uint64_t(int64_t(int64_t(r11) + -50));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,9648(r31)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r31)).unknown_9648_address(), uint32_t(r11));
  // bge 0x82260a8c
  if (!cr0.lt) goto loc_82260A8C;
  // stw r30,9648(r31)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r31)).unknown_9648_address(), uint32_t(r30));
loc_82260A8C:
  // lwz r11,9648(r31)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r31)).unknown_9648_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r11,1
  r3 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x821495e8
  // (left out: dead in C++)
  { const lifted::Result o = f821495E8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // stw r30,9656(r31)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r31)).unknown_9656_address(), uint32_t(r30));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82260ab8
  if (cr6.eq) goto loc_82260AB8;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82260AB8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
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

// sub_82260AE8: lifted from generated/ by lift.py.
uint64_t f82260AE8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260AE8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r3)).unknown_9640_address());
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bgt cr6,0x82260afc
  if (cr6.gt) goto loc_82260AFC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_82260AFC:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,24904(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24904);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address());
  // lwz r10,48(r10)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).visible_row_address());
  // subf r10,r10,r9
  r10 = r9 - r10;
  // addic. r10,r10,-7
  ca = uint32_t(r10) > 6;
  r10 = uint64_t(int64_t(int64_t(r10) + -7));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt 0x82260b38
  if (cr0.lt) goto loc_82260B38;
  // addi r10,r9,-7
  r10 = uint64_t(int64_t(int64_t(r9) + -7));
  // stw r10,9652(r3)
  guest::StoreU32(base, uint32_t(r3) + 9652, uint32_t(r10));
  // lwz r11,24904(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24904);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_address(), uint32_t(r9));
  // b 0x82260b68
  goto loc_82260B68;
loc_82260B38:
  // lwz r9,9648(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 9648);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x82260ba0
  if (!cr0.eq) goto loc_82260BA0;
  // stw r8,9652(r3)
  guest::StoreU32(base, uint32_t(r3) + 9652, uint32_t(r8));
  // lwz r10,24904(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).fault_24904_address());
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r10)).cursor_address());
  // stw r8,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r10)).cursor_address(), uint32_t(r8));
  // stw r9,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r10)).previous_address(), uint32_t(r9));
  // lwz r10,24904(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).fault_24904_address());
  // stw r8,48(r10)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r10)).visible_row_address(), uint32_t(r8));
  // lwz r11,24904(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).fault_24904_address());
  // stw r8,52(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).previous_row_address(), uint32_t(r8));
loc_82260B68:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82260b80
  if (cr0.eq) goto loc_82260B80;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82260B80:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // blr
  return r3;
loc_82260BA0:
  // add. r11,r9,r10
  r11 = r9 + r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,9652(r3)
  guest::StoreU32(base, uint32_t(r3) + 9652, uint32_t(r11));
  // bge 0x82260bb4
  if (!cr0.lt) goto loc_82260BB4;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9652(r3)
  guest::StoreU32(base, uint32_t(r3) + 9652, uint32_t(r11));
loc_82260BB4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_82260BC0: lifted from generated/ by lift.py.
uint64_t f82260BC0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260BC0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mr r10,r3
  r10 = r3;
  // lwz r11,9640(r10)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r10)).unknown_9640_address());
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bgt cr6,0x82260bd8
  if (cr6.gt) goto loc_82260BD8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_82260BD8:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r8,9648(r10)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r10)).unknown_9648_address());
  // lwz r9,24904(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 24904);
  // lwz r6,0(r9)
  r6 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).cursor_address());
  // lwz r9,48(r9)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).visible_row_address());
  // subf r9,r9,r6
  r9 = r6 - r9;
  // addi r9,r9,13
  r9 = uint64_t(int64_t(int64_t(r9) + 13));
  // cmpwi cr6,r9,100
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 100, so);
  // add r9,r8,r9
  r9 = r8 + r9;
  // bge cr6,0x82260c94
  if (!cr6.lt) goto loc_82260C94;
  // lwz r7,9644(r10)
  r7 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r10)).unknown_9644_address());
  // cmpw cr6,r9,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r7), so);
  // blt cr6,0x82260c40
  if (cr6.lt) goto loc_82260C40;
  // subf r9,r8,r7
  r9 = r7 - r8;
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r9,9652(r10)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r10)).unknown_9652_address(), uint32_t(r9));
  // lwz r10,24904(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24904);
  // lwz r7,0(r10)
  r7 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address());
  // stw r9,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address(), uint32_t(r9));
  // stw r7,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_address(), uint32_t(r7));
  // lwz r10,24904(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24904);
  // stw r8,48(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).visible_row_address(), uint32_t(r8));
  // lwz r11,24904(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24904);
  // stw r8,52(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_row_address(), uint32_t(r8));
  // b 0x82260c58
  goto loc_82260C58;
loc_82260C40:
  // addi r9,r6,7
  r9 = uint64_t(int64_t(int64_t(r6) + 7));
  // stw r9,9652(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).fault_9652_address(), uint32_t(r9));
  // lwz r11,24904(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).fault_24904_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).cursor_address());
  // stw r9,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).cursor_address(), uint32_t(r9));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).previous_address(), uint32_t(r10));
loc_82260C58:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82260c70
  if (cr0.eq) goto loc_82260C70;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82260C70:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // blr
  return r3;
loc_82260C94:
  // lwz r11,9644(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 9644);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // addi r11,r11,-7
  r11 = uint64_t(int64_t(int64_t(r11) + -7));
  // ble cr6,0x82260cac
  if (!cr6.gt) goto loc_82260CAC;
  // addi r11,r9,-6
  r11 = uint64_t(int64_t(int64_t(r9) + -6));
loc_82260CAC:
  // stw r11,9652(r10)
  guest::StoreU32(base, uint32_t(r10) + 9652, uint32_t(r11));
  // blr
  return r3;
}

// sub_82260CB8: lifted from generated/ by lift.py.
uint64_t f82260CB8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260CB8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mr r10,r3
  r10 = r3;
  // lwz r11,9648(r10)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r10)).unknown_9648_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9652(r10)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r10)).unknown_9652_address(), uint32_t(r11));
  // bne cr6,0x82260d2c
  if (!cr6.eq) goto loc_82260D2C;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r10,24904(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address());
  // stw r11,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address(), uint32_t(r11));
  // stw r8,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_address(), uint32_t(r8));
  // lwz r10,24904(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // stw r11,48(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).visible_row_address(), uint32_t(r11));
  // lwz r10,24904(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // stw r11,52(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_row_address(), uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24900(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24900);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).page_address());
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x82260d0c
  if (cr0.eq) goto loc_82260D0C;
  // stw r9,8(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).previous_page_address(), uint32_t(r9));
loc_82260D0C:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).next_page_address(), uint32_t(r11));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r9,16(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).queued_step_address(), uint32_t(r9));
  // stw r8,12(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).step_address(), uint32_t(r8));
  // stw r9,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).page_address(), uint32_t(r9));
  // blr
  return r3;
loc_82260D2C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_82260D38: lifted from generated/ by lift.py.
uint64_t f82260D38(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260D38 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r10,9648(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9648_address());
  // addi r9,r10,100
  r9 = uint64_t(int64_t(int64_t(r10) + 100));
  // lwz r10,9644(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9644_address());
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // bgt cr6,0x82260de0
  if (cr6.gt) goto loc_82260DE0;
  // lwz r10,9640(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9640_address());
  // addi r8,r10,-1
  r8 = uint64_t(int64_t(int64_t(r10) + -1));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r8,9652(r11)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9652_address(), uint32_t(r8));
  // lwz r9,24904(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 24904);
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).cursor_address());
  // stw r8,0(r9)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).cursor_address(), uint32_t(r8));
  // stw r7,4(r9)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).previous_address(), uint32_t(r7));
  // lwz r9,9644(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9644_address());
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // bge cr6,0x82260d90
  if (!cr6.lt) goto loc_82260D90;
  // lwz r9,24904(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 24904);
  // lwz r8,9652(r11)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9652_address());
  // stw r8,48(r9)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).visible_row_address(), uint32_t(r8));
  // lwz r9,9652(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9652_address());
  // b 0x82260d9c
  goto loc_82260D9C;
loc_82260D90:
  // lwz r11,24904(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24904);
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // stw r9,48(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address(), uint32_t(r9));
loc_82260D9C:
  // lwz r11,24904(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24904);
  // stw r9,52(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_row_address(), uint32_t(r9));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82260dbc
  if (cr0.eq) goto loc_82260DBC;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82260DBC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // blr
  return r3;
loc_82260DE0:
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,9652(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).fault_9652_address(), uint32_t(r10));
  // blr
  return r3;
}

// sub_82260DF0: lifted from generated/ by lift.py.
uint64_t f82260DF0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260DF0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // lwz r10,9648(r3)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r3)).unknown_9648_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82260e60
  if (!cr0.eq) goto loc_82260E60;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // stw r11,9652(r3)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r3)).unknown_9652_address(), uint32_t(r11));
  // lwz r10,24904(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address());
  // stw r11,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address(), uint32_t(r11));
  // stw r8,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_address(), uint32_t(r8));
  // lwz r10,24904(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // stw r11,48(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).visible_row_address(), uint32_t(r11));
  // lwz r10,24904(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // stw r11,52(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_row_address(), uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24900(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24900);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).page_address());
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x82260e40
  if (cr0.eq) goto loc_82260E40;
  // stw r9,8(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).previous_page_address(), uint32_t(r9));
loc_82260E40:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).next_page_address(), uint32_t(r11));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r9,16(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).queued_step_address(), uint32_t(r9));
  // stw r8,12(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).step_address(), uint32_t(r8));
  // stw r9,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).page_address(), uint32_t(r9));
  // blr
  return r3;
loc_82260E60:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24904(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24904);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // add r11,r10,r11
  r11 = r10 + r11;
  // addic. r11,r11,-100
  ca = uint32_t(r11) > 99;
  r11 = uint64_t(int64_t(int64_t(r11) + -100));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,9652(r3)
  guest::StoreU32(base, uint32_t(r3) + 9652, uint32_t(r11));
  // bge 0x82260e84
  if (!cr0.lt) goto loc_82260E84;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9652(r3)
  guest::StoreU32(base, uint32_t(r3) + 9652, uint32_t(r11));
loc_82260E84:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_82260E90: lifted from generated/ by lift.py.
uint64_t f82260E90(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260E90 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r10,9648(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9648_address());
  // lwz r9,9640(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9640_address());
  // addi r8,r10,100
  r8 = uint64_t(int64_t(int64_t(r10) + 100));
  // add r7,r9,r10
  r7 = r9 + r10;
  // cmpw cr6,r7,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r8), so);
  // bge cr6,0x82260f38
  if (!cr6.lt) goto loc_82260F38;
  // addi r8,r9,-1
  r8 = uint64_t(int64_t(int64_t(r9) + -1));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r8,9652(r11)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9652_address(), uint32_t(r8));
  // lwz r9,24904(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 24904);
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).cursor_address());
  // stw r8,0(r9)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).cursor_address(), uint32_t(r8));
  // stw r7,4(r9)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).previous_address(), uint32_t(r7));
  // lwz r9,9644(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9644_address());
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // bge cr6,0x82260ee8
  if (!cr6.lt) goto loc_82260EE8;
  // lwz r9,24904(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 24904);
  // lwz r8,9652(r11)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9652_address());
  // stw r8,48(r9)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).visible_row_address(), uint32_t(r8));
  // lwz r9,9652(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r11)).unknown_9652_address());
  // b 0x82260ef4
  goto loc_82260EF4;
loc_82260EE8:
  // lwz r11,24904(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24904);
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // stw r9,48(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address(), uint32_t(r9));
loc_82260EF4:
  // lwz r11,24904(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24904);
  // stw r9,52(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_row_address(), uint32_t(r9));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82260f14
  if (cr0.eq) goto loc_82260F14;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82260F14:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // blr
  return r3;
loc_82260F38:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r9,24904(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // lwz r9,0(r9)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r9)).cursor_address());
  // add r10,r10,r9
  r10 = r10 + r9;
  // addi r10,r10,100
  r10 = uint64_t(int64_t(int64_t(r10) + 100));
  // stw r10,9652(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).fault_9652_address(), uint32_t(r10));
  // blr
  return r3;
}

// sub_82260F58: lifted from generated/ by lift.py.
uint64_t f82260F58(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82260F58 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,24864(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24864);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x8226101c
  if (!cr6.gt) goto loc_8226101C;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r11,9648(r3)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r3)).unknown_9648_address());
  // addi r6,r10,-1
  r6 = uint64_t(int64_t(int64_t(r10) + -1));
  // lwz r8,24904(r9)
  r8 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // lwz r8,0(r8)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r8)).cursor_address());
  // add r8,r11,r8
  r8 = r11 + r8;
  // cmpw cr6,r6,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r8), so);
  // beq cr6,0x8226101c
  if (cr6.eq) goto loc_8226101C;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // stw r10,9652(r3)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r3)).unknown_9652_address(), uint32_t(r10));
  // bgt cr6,0x82261024
  if (cr6.gt) goto loc_82261024;
  // addi r8,r11,100
  r8 = uint64_t(int64_t(int64_t(r11) + 100));
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // ble cr6,0x82261024
  if (!cr6.gt) goto loc_82261024;
  // lwz r8,9640(r3)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r3)).unknown_9640_address());
  // addi r6,r10,7
  r6 = uint64_t(int64_t(int64_t(r10) + 7));
  // add r5,r8,r11
  r5 = r8 + r11;
  // cmpw cr6,r5,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r5), int32_t(r6), so);
  // bgt cr6,0x82260fc8
  if (cr6.gt) goto loc_82260FC8;
  // subf r8,r8,r10
  r8 = r10 - r8;
  // subf r11,r11,r8
  r11 = r8 - r11;
  // addi r7,r11,7
  r7 = uint64_t(int64_t(int64_t(r11) + 7));
loc_82260FC8:
  // lwz r11,24904(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address(), uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_address(), uint32_t(r8));
  // lwz r11,24904(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // stw r7,48(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address(), uint32_t(r7));
  // lwz r11,24904(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 24904);
  // stw r7,52(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_row_address(), uint32_t(r7));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82261000
  if (cr0.eq) goto loc_82261000;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82261000:
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r9));
loc_8226101C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_82261024:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_82261250: lifted from generated/ by lift.py.
void f82261250(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82261250 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82261294
  if (cr6.eq) goto loc_82261294;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82261274
  if (cr0.eq) goto loc_82261274;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82261274:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r8,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r8));
  // blr
  return;
loc_82261294:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_822612A0: lifted from generated/ by lift.py.
lifted::Result f822612A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822612A0 (lifted)");
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
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822612C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x822612CC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822612E0: lifted from generated/ by lift.py.
void f822612E0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822612E0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,9632(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9632);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x822612f4
  if (!cr6.eq) goto loc_822612F4;
  // li r6,13
  r6 = uint64_t(int64_t(13));
  // b 0x82261304
  goto loc_82261304;
loc_822612F4:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r6,12
  r6 = uint64_t(int64_t(12));
  // stw r11,24924(r10)
  guest::StoreU32(base, uint32_t(r10) + 24924, uint32_t(r11));
loc_82261304:
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // lwz r7,604(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r7,r7,r10
  r7 = r7 + r10;
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r10));
  // divwu r5,r7,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r7) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r10,r5,r7
  r10 = r7 - r5;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r6,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r6));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r8));
  // stw r4,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r4));
  // blr
  return;
}

// sub_82261418: lifted from generated/ by lift.py.
uint64_t f82261418(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82261418 (lifted)");
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
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82261440, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82261454, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82261464, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82261474, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82261484, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82261498, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x822614AC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822614C0: lifted from generated/ by lift.py.
lifted::Result f822614C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822614C0 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // bne cr6,0x82261508
  if (!cr6.eq) goto loc_82261508;
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
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82261504, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822615e4
  goto loc_822615E4;
loc_82261508:
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8226150C);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x822615b0
  if (!cr0.lt) goto loc_822615B0;
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
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // li r6,40
  r6 = uint64_t(int64_t(40));
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
  // b 0x822615e4
  goto loc_822615E4;
loc_822615B0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822601f0
  // (left out: dead in C++)
  r3 = f822601F0(base, r1, r13, r3);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x822615d0
  if (cr6.eq) goto loc_822615D0;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822615D0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
loc_822615E4:
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

// sub_82261600: lifted from generated/ by lift.py.
lifted::Result f82261600(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82261600 (lifted)");
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
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9656_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82261704
  if (!cr6.eq) goto loc_82261704;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // bne cr6,0x82261650
  if (!cr6.eq) goto loc_82261650;
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
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8226164C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82261704
  goto loc_82261704;
loc_82261650:
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x82261654);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x8226166c
  if (!cr0.lt) goto loc_8226166C;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,40
  r5 = uint64_t(int64_t(40));
  // b 0x82261670
  goto loc_82261670;
loc_8226166C:
  // li r5,39
  r5 = uint64_t(int64_t(39));
loc_82261670:
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
  r9 = guest::LoadU32(base, uint32_t(r11) + 600);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r30,604(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 604);
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
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r9));
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
  r9 = guest::LoadU32(base, uint32_t(r11) + 600);
  // lwz r8,604(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 604);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // add r8,r8,r9
  r8 = r8 + r9;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r9,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r9));
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
  // stw r29,9656(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).unknown_9656_address(), uint32_t(r29));
loc_82261704:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82261710: lifted from generated/ by lift.py.
lifted::Result f82261710(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82261710 (lifted)");
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
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82261740, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82261774
  if (!cr6.eq) goto loc_82261774;
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
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x8226176C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_82261774:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82261780, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822617C0: lifted from generated/ by lift.py.
lifted::Result f822617C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822617C0 (lifted)");
  uint64_t r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r20,r3
  r20 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // mr r18,r4
  r18 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r17,r5
  r17 = r5;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822617F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,152
  r11 = uint64_t(int64_t(9961472));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lis r21,-32148
  r21 = uint64_t(int64_t(-2106851328));
  // ori r19,r11,38527
  r19 = r11 | 38527;
loc_82261804:
  // addi r27,r29,1
  r27 = uint64_t(int64_t(int64_t(r29) + 1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // mr r5,r27
  r5 = r27;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82261818, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // add r24,r29,r18
  r24 = r29 + r18;
  // cmpwi cr6,r29,7
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 7, so);
  // bne cr6,0x82261834
  if (!cr6.eq) goto loc_82261834;
  // cmpwi cr6,r17,0
  cr6 = lifted::Compare<int32_t>(int32_t(r17), 0, so);
  // bne cr6,0x82261834
  if (!cr6.eq) goto loc_82261834;
  // addi r24,r18,-1
  r24 = uint64_t(int64_t(int64_t(r18) + -1));
loc_82261834:
  // lwz r11,9640(r20)
  r11 = guest::LoadU32(base, uint32_t(r20) + 9640);
  // cmpw cr6,r11,r24
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r24), so);
  // ble cr6,0x822619e0
  if (!cr6.gt) goto loc_822619E0;
  // lwz r10,27348(r21)
  r10 = guest::LoadU32(base, uint32_t(r21) + 27348);
  // mulli r26,r24,48
  r26 = uint64_t(int64_t(static_cast<int64_t>(r24 * static_cast<uint64_t>(48))));
  // mr r22,r19
  r22 = r19;
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r26
  r11 = r11 + r26;
  // lwz r8,44(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 44);
  // ld r9,16(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 16);
  // mr r11,r9
  r11 = r9;
  // ld r28,40(r8)
  r28 = guest::LoadU64(base, uint32_t(r8) + 40);
  // cmplw cr6,r11,r19
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r19), so);
  // bgt cr6,0x82261874
  if (cr6.gt) goto loc_82261874;
  // mr r22,r11
  r22 = r11;
loc_82261874:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x82261888
  if (cr6.eq) goto loc_82261888;
  // divwu r23,r11,r28
  r23 = uint32_t(uint32_t(r28) ? uint32_t(r11) / uint32_t(r28) : 0);
  // twllei r28,0
  if (int32_t(r28) == 0 || uint32_t(r28) < 0u) lifted::Trap(0);
  // b 0x8226188c
  goto loc_8226188C;
loc_82261888:
  // li r23,0
  r23 = uint64_t(int64_t(0));
loc_8226188C:
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r26
  r11 = r11 + r26;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // ld r10,16(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 16);
  // cmpdi cr6,r10,9999
  cr6 = lifted::Compare<int64_t>(int64_t(r10), 9999, so);
  // ble cr6,0x822618ac
  if (!cr6.gt) goto loc_822618AC;
  // li r10,9999
  r10 = uint64_t(int64_t(9999));
loc_822618AC:
  // extsw r25,r10
  r25 = uint64_t(int64_t(int32_t(r10)));
  // cmplwi cr6,r28,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 9999, so);
  // ble cr6,0x822618bc
  if (!cr6.gt) goto loc_822618BC;
  // li r28,9999
  r28 = uint64_t(int64_t(9999));
loc_822618BC:
  // mr r5,r27
  r5 = r27;
  // lwz r29,64(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 64);
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822618D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822618DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9648(r20)
  r11 = guest::LoadU32(base, uint32_t(r20) + 9648);
  // mr r4,r3
  r4 = r3;
  // add r11,r11,r24
  r11 = r11 + r24;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // addi r6,r11,1
  r6 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822618F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82261904, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r21)
  r11 = guest::LoadU32(base, uint32_t(r21) + 27348);
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r26
  r11 = r11 + r26;
  // addi r5,r11,24
  r5 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x8211c8a0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeCharText(base, LIFTED_ARGS(0x82261924, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82261930, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r22
  r6 = r22;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82261944, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82261950, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r23
  r6 = r23;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82261964, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82261970, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r25
  r6 = r25;
  // li r5,326
  r5 = uint64_t(int64_t(326));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82261984, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82261990, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r28
  r6 = r28;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822619A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r27
  r5 = r27;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822619B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822619BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x822619C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822619D8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,108(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).texture_entry_address(), uint32_t(r29));
  // b 0x822619f0
  goto loc_822619F0;
loc_822619E0:
  // mr r5,r27
  r5 = r27;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822619F0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822619F0:
  // mr r29,r27
  r29 = r27;
  // cmpwi cr6,r29,8
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 8, so);
  // blt cr6,0x82261804
  if (cr6.lt) goto loc_82261804;
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82261A08: lifted from generated/ by lift.py.
lifted::Result f82261A08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82261A08 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-416(r1)
  ea = -416 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82261A2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r11)).value_address());
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x82261A3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r25,r3
  r25 = r3;
  // lwz r7,24864(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 24864);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // addi r29,r11,30320
  r29 = uint64_t(int64_t(int64_t(r11) + 30320));
  // ble cr6,0x82261b28
  if (!cr6.gt) goto loc_82261B28;
  // lis r11,152
  r11 = uint64_t(int64_t(9961472));
  // rlwinm r9,r25,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 2) & 0xFFFFFFFC;
  // ori r10,r11,38527
  r10 = r11 | 38527;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r8,r29,332
  r8 = uint64_t(int64_t(int64_t(r29) + 332));
  // mr r27,r10
  r27 = r10;
  // lwz r26,24868(r11)
  r26 = guest::LoadU32(base, uint32_t(r11) + 24868);
  // lwzx r11,r9,r8
  r11 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r8));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bgt cr6,0x82261a84
  if (cr6.gt) goto loc_82261A84;
  // mr r27,r11
  r27 = r11;
loc_82261A84:
  // addi r11,r29,364
  r11 = uint64_t(int64_t(int64_t(r29) + 364));
  // li r28,9999
  r28 = uint64_t(int64_t(9999));
  // lwzx r11,r9,r11
  r11 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r11));
  // cmplwi cr6,r11,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 9999, so);
  // bgt cr6,0x82261a9c
  if (cr6.gt) goto loc_82261A9C;
  // mr r28,r11
  r28 = r11;
loc_82261A9C:
  // addi r11,r29,348
  r11 = uint64_t(int64_t(int64_t(r29) + 348));
  // lwzx r30,r9,r11
  r30 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r11));
  // cmplwi cr6,r30,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 9999, so);
  // ble cr6,0x82261ab0
  if (!cr6.gt) goto loc_82261AB0;
  // li r30,9999
  r30 = uint64_t(int64_t(9999));
loc_82261AB0:
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82261AC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r7,r27
  r7 = r27;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82261ADC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r7,r26
  r7 = r26;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82261AF4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r7,r28
  r7 = r28;
  // li r6,326
  r6 = uint64_t(int64_t(326));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82261B0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r7,r30
  r7 = r30;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82261B24, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82261ba0
  goto loc_82261BA0;
loc_82261B28:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,6096
  r6 = uint64_t(int64_t(int64_t(r11) + 6096));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // addi r6,r11,6088
  r6 = uint64_t(int64_t(int64_t(r11) + 6088));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // addi r30,r11,6080
  r30 = uint64_t(int64_t(int64_t(r11) + 6080));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r6,r30
  r6 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // addi r6,r11,6072
  r6 = uint64_t(int64_t(int64_t(r11) + 6072));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // mr r6,r30
  r6 = r30;
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_82261BA0:
  // rlwinm r11,r25,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 5) & 0xFFFFFFE0;
  // addi r5,r29,14
  r5 = uint64_t(int64_t(int64_t(r29) + 14));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,256
  r8 = uint64_t(int64_t(256));
  // addi r7,r1,96
  r7 = uint64_t(int64_t(int64_t(r1) + 96));
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
  { const lifted::Result o = game::WideCharToMultiByte(base, LIFTED_ARGS(0x82261BCC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82261BF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // lwz r3,29320(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 29320);
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x82261C00, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82261C10, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,108(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).texture_entry_address(), uint32_t(r30));
  // addi r1,r1,416
  r1 = uint64_t(int64_t(int64_t(r1) + 416));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82261C20: lifted from generated/ by lift.py.
lifted::Result f82261C20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82261C20 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r31,r4
  r31 = r4;
  // bl 0x822603d8
  // (left out: dead in C++)
  { const lifted::Result o = f822603D8(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // mr r8,r7
  r8 = r7;
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // li r9,16
  r9 = uint64_t(int64_t(16));
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
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82261cb4
  if (cr6.eq) goto loc_82261CB4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82261CB4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r7,9664(r30)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9664_address(), uint32_t(r7));
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

// sub_82261CE8: lifted from generated/ by lift.py.
uint64_t f82261CE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82261CE8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82261418
  // (left out: dead in C++)
  r3 = f82261418(base, r1, r13);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,9640(r31)
  guest::StoreU32(base, uint32_t(r31) + 9640, uint32_t(r11));
  // stw r11,9644(r31)
  guest::StoreU32(base, uint32_t(r31) + 9644, uint32_t(r11));
  // stw r11,9624(r31)
  guest::StoreU32(base, uint32_t(r31) + 9624, uint32_t(r11));
  // stw r11,9628(r31)
  guest::StoreU32(base, uint32_t(r31) + 9628, uint32_t(r11));
  // stw r11,9632(r31)
  guest::StoreU32(base, uint32_t(r31) + 9632, uint32_t(r11));
  // stw r9,9660(r31)
  guest::StoreU32(base, uint32_t(r31) + 9660, uint32_t(r9));
  // stw r9,9664(r31)
  guest::StoreU32(base, uint32_t(r31) + 9664, uint32_t(r9));
  // stw r11,9656(r31)
  guest::StoreU32(base, uint32_t(r31) + 9656, uint32_t(r11));
  // stw r11,9648(r31)
  guest::StoreU32(base, uint32_t(r31) + 9648, uint32_t(r11));
  // stw r11,9652(r31)
  guest::StoreU32(base, uint32_t(r31) + 9652, uint32_t(r11));
  // lwz r8,27348(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 27348);
  // lwz r8,4(r8)
  r8 = guest::LoadU32(base, uint32_t(r8) + 4);
  // lwz r8,8(r8)
  r8 = guest::LoadU32(base, uint32_t(r8) + 8);
  // stw r8,9640(r31)
  guest::StoreU32(base, uint32_t(r31) + 9640, uint32_t(r8));
  // lwz r10,27348(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 27348);
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // stw r10,9644(r31)
  guest::StoreU32(base, uint32_t(r31) + 9644, uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24900(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24900);
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).page_address());
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x82261d6c
  if (cr0.eq) goto loc_82261D6C;
  // stw r8,8(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).previous_page_address(), uint32_t(r8));
loc_82261D6C:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // stw r11,12(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).step_address(), uint32_t(r11));
  // stw r9,16(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).queued_step_address(), uint32_t(r9));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).next_page_address(), uint32_t(r11));
  // stw r8,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r10)).page_address(), uint32_t(r8));
  // lwz r10,9640(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 9640);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82261db0
  if (!cr0.eq) goto loc_82261DB0;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // addi r9,r10,24904
  r9 = uint64_t(int64_t(int64_t(r10) + 24904));
  // lwz r10,0(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 0);
  // stw r11,16(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).last_item_address(), uint32_t(r11));
loc_82261D9C:
  // stw r11,40(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).wrapped_address(), uint32_t(r11));
  // stw r11,48(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).visible_row_address(), uint32_t(r11));
  // stw r11,52(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_row_address(), uint32_t(r11));
  // stw r11,32(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).shoulders_step_address(), uint32_t(r11));
  // b 0x82261ddc
  goto loc_82261DDC;
loc_82261DB0:
  // addi r8,r10,-1
  r8 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // addi r9,r10,24904
  r9 = uint64_t(int64_t(int64_t(r10) + 24904));
  // lwz r10,0(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 0);
  // stw r8,16(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).last_item_address(), uint32_t(r8));
  // ble cr6,0x82261d9c
  if (!cr6.gt) goto loc_82261D9C;
  // li r7,7
  r7 = uint64_t(int64_t(7));
  // stw r11,44(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).list_wrapped_address(), uint32_t(r11));
  // stw r11,56(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).first_row_address(), uint32_t(r11));
  // stw r7,60(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).rows_shown_address(), uint32_t(r7));
loc_82261DDC:
  // stw r11,28(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).left_right_steps_address(), uint32_t(r11));
  // stw r11,24(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).up_down_locked_address(), uint32_t(r11));
  // stw r11,64(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).player_address(), uint32_t(r11));
  // stw r11,36(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).side_address(), uint32_t(r11));
  // stw r11,8(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).moved_address(), uint32_t(r11));
  // stw r11,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_address(), uint32_t(r11));
  // stw r11,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address(), uint32_t(r11));
  // stw r11,20(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).wraps_address(), uint32_t(r11));
  // stw r11,12(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).first_item_address(), uint32_t(r11));
  // lwz r10,0(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 0);
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address());
  // stw r11,0(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).cursor_address(), uint32_t(r11));
  // stw r8,4(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_address(), uint32_t(r8));
  // lwz r10,0(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 0);
  // stw r11,48(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).visible_row_address(), uint32_t(r11));
  // lwz r10,0(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 0);
  // stw r11,52(r10)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10)).previous_row_address(), uint32_t(r11));
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

// sub_82261E38: lifted from generated/ by lift.py.
lifted::Result f82261E38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82261E38 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,9660(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9660);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // bne cr6,0x82261ffc
  if (!cr6.eq) goto loc_82261FFC;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82261ea4
  if (cr6.eq) goto loc_82261EA4;
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // bne cr6,0x8226202c
  if (!cr6.eq) goto loc_8226202C;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82261e88
  if (cr6.eq) goto loc_82261E88;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82261E88:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // b 0x82262028
  goto loc_82262028;
loc_82261EA4:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82261eb4
  if (cr0.eq) goto loc_82261EB4;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82261EB4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r29));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,27348(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 8);
  // stw r10,9640(r30)
  guest::StoreU32(base, uint32_t(r30) + 9640, uint32_t(r10));
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,27348(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r11,9644(r30)
  guest::StoreU32(base, uint32_t(r30) + 9644, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // bgt cr6,0x82261f1c
  if (cr6.gt) goto loc_82261F1C;
  // addi r10,r11,24904
  r10 = uint64_t(int64_t(int64_t(r11) + 24904));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r31,40(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).wrapped_address(), uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address(), uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_row_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
  // b 0x82261f38
  goto loc_82261F38;
loc_82261F1C:
  // addi r9,r10,-1
  r9 = uint64_t(int64_t(int64_t(r10) + -1));
  // addi r10,r11,24904
  r10 = uint64_t(int64_t(int64_t(r11) + 24904));
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10) - 24904).fault_24904_address());
  // stw r31,44(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).list_wrapped_address(), uint32_t(r31));
  // stw r31,56(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).first_row_address(), uint32_t(r31));
  // stw r8,60(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).rows_shown_address(), uint32_t(r8));
loc_82261F38:
  // stw r31,28(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).last_item_address(), uint32_t(r9));
  // mr r9,r31
  r9 = r31;
  // stw r31,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).moved_address(), uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).previous_address(), uint32_t(r31));
  // stw r31,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).cursor_address(), uint32_t(r31));
  // stw r31,20(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).wraps_address(), uint32_t(r31));
  // stw r31,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // lwz r8,9648(r30)
  r8 = guest::LoadU32(base, uint32_t(r30) + 9648);
  // lwz r6,9640(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 9640);
  // lwz r7,9652(r30)
  r7 = guest::LoadU32(base, uint32_t(r30) + 9652);
  // add r11,r8,r6
  r11 = r8 + r6;
  // cmpw cr6,r11,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r7), so);
  // bgt cr6,0x82261f84
  if (cr6.gt) goto loc_82261F84;
  // addi r7,r11,-1
  r7 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r7,9652(r30)
  guest::StoreU32(base, uint32_t(r30) + 9652, uint32_t(r7));
loc_82261F84:
  // lwz r7,9652(r30)
  r7 = guest::LoadU32(base, uint32_t(r30) + 9652);
  // subf r8,r8,r7
  r8 = r7 - r8;
  // addi r7,r7,7
  r7 = uint64_t(int64_t(int64_t(r7) + 7));
  // cmpw cr6,r11,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r7), so);
  // bgt cr6,0x82261fa0
  if (cr6.gt) goto loc_82261FA0;
  // subf r11,r6,r8
  r11 = r8 - r6;
  // addi r9,r11,7
  r9 = uint64_t(int64_t(int64_t(r11) + 7));
loc_82261FA0:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10) - 24904).fault_24904_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // lwz r7,0(r11)
  r7 = guest::LoadU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).cursor_address());
  // stw r8,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).cursor_address(), uint32_t(r8));
  // stw r7,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).previous_address(), uint32_t(r7));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10) - 24904).fault_24904_address());
  // stw r9,48(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).visible_row_address(), uint32_t(r9));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10) - 24904).fault_24904_address());
  // stw r9,52(r11)
  guest::StoreU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).previous_row_address(), uint32_t(r9));
  // lwz r11,9640(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9640);
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bgt cr6,0x82261fdc
  if (cr6.gt) goto loc_82261FDC;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x82261fec
  goto loc_82261FEC;
loc_82261FDC:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r10) - 24904).fault_24904_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).cursor_address());
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8MenuJoin(base, uint32_t(r11)).visible_row_address());
  // subf r4,r11,r10
  r4 = r10 - r11;
loc_82261FEC:
  // bl 0x822617c0
  // (left out: dead in C++)
  { const lifted::Result o = f822617C0(base, r1, r13, r3, r4, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82261a08
  // (left out: dead in C++)
  { const lifted::Result o = f82261A08(base, r1, r13, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8226202c
  goto loc_8226202C;
loc_82261FFC:
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bgt cr6,0x8226202c
  if (cr6.gt) goto loc_8226202C;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x82262018
  if (cr6.eq) goto loc_82262018;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82262018:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_82262028:
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
loc_8226202C:
  // stw r29,9664(r30)
  guest::StoreU32(base, uint32_t(r30) + 9664, uint32_t(r29));
  // stw r31,9656(r30)
  guest::StoreU32(base, uint32_t(r30) + 9656, uint32_t(r31));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82262040: lifted from generated/ by lift.py.
lifted::Result f82262040(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82262040 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r30,r3
  r30 = r3;
  // addi r6,r11,6100
  r6 = uint64_t(int64_t(int64_t(r11) + 6100));
  // mr r27,r4
  r27 = r4;
  // lwz r11,24904(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // lwz r8,48(r11)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // lwz r7,0(r11)
  r7 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8226207C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r31,r10,r11
  r31 = r10 + r11;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822620c0
  if (cr6.eq) goto loc_822620C0;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822620B8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82262188
  if (!cr0.eq) goto loc_82262188;
loc_822620C0:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822620ec
  if (cr6.eq) goto loc_822620EC;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822620E4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82262188
  if (!cr0.eq) goto loc_82262188;
loc_822620EC:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82262118
  if (cr6.eq) goto loc_82262118;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82262110, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82262144
  if (!cr0.eq) goto loc_82262144;
loc_82262118:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822621d4
  if (cr6.eq) goto loc_822621D4;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226213C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822621d4
  if (cr0.eq) goto loc_822621D4;
loc_82262144:
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r3)).cursor_address());
  // cmpwi cr6,r11,99
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 99, so);
  // bne cr6,0x822621d8
  if (!cr6.eq) goto loc_822621D8;
  // lwz r11,9648(r30)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9648_address());
  // lwz r10,9644(r30)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9644_address());
  // addi r11,r11,100
  r11 = uint64_t(int64_t(int64_t(r11) + 100));
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // ble cr6,0x822621d8
  if (!cr6.gt) goto loc_822621D8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82262170, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // lwz r11,9648(r30)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9648_address());
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x822621c0
  goto loc_822621C0;
loc_82262188:
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r3)).cursor_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822621d8
  if (!cr6.eq) goto loc_822621D8;
  // lwz r11,9648(r30)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9648_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x822621d8
  if (!cr6.gt) goto loc_822621D8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822621AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // lwz r11,9648(r30)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9648_address());
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
loc_822621C0:
  // mr r3,r30
  r3 = r30;
  // stw r11,9652(r30)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9652_address(), uint32_t(r11));
  // mr r4,r27
  r4 = r27;
  // bl 0x82261c20
  // (left out: dead in C++)
  { const lifted::Result o = f82261C20(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822624b8
  goto loc_822624B8;
loc_822621D4:
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
loc_822621D8:
  // lwz r11,9640(r30)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9640_address());
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bgt cr6,0x822621ec
  if (cr6.gt) goto loc_822621EC;
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x822621E8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822621f0
  goto loc_822621F0;
loc_822621EC:
  // bl 0x821ce1d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CE1D0, LIFTED_ARGS(0x822621F0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822621F0:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822621FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r25,-1
  r25 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r26,-32127
  r26 = uint64_t(int64_t(-2105475072));
  // beq 0x82262244
  if (cr0.eq) goto loc_82262244;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82262214, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24900(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82262228
  if (cr6.eq) goto loc_82262228;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82262228:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r25,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r25));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r9));
  // b 0x82262474
  goto loc_82262474;
loc_82262244:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82262250, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82262440
  if (!cr0.eq) goto loc_82262440;
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82262264, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82262440
  if (!cr0.eq) goto loc_82262440;
  // lwz r11,24904(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822622e8
  if (!cr6.eq) goto loc_822622E8;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822622a8
  if (cr6.eq) goto loc_822622A8;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822622A0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822622d4
  if (!cr0.eq) goto loc_822622D4;
loc_822622A8:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x822622e8
  if (cr6.eq) goto loc_822622E8;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822622CC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822622e8
  if (cr0.eq) goto loc_822622E8;
loc_822622D4:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822622DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82260ae8
  // (left out: dead in C++)
  r3 = f82260AE8(base, r3);
  // b 0x82262428
  goto loc_82262428;
loc_822622E8:
  // lwz r11,24904(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82262364
  if (!cr6.eq) goto loc_82262364;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82262324
  if (cr6.eq) goto loc_82262324;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226231C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82262350
  if (!cr0.eq) goto loc_82262350;
loc_82262324:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82262364
  if (cr6.eq) goto loc_82262364;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82262348, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82262364
  if (cr0.eq) goto loc_82262364;
loc_82262350:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82262358, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82260bc0
  // (left out: dead in C++)
  r3 = f82260BC0(base, r3);
  // b 0x82262428
  goto loc_82262428;
loc_82262364:
  // li r4,163
  r4 = uint64_t(int64_t(163));
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82262370, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226238c
  if (cr0.eq) goto loc_8226238C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82262380, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82260cb8
  // (left out: dead in C++)
  r3 = f82260CB8(base, r3);
  // b 0x82262428
  goto loc_82262428;
loc_8226238C:
  // li r4,164
  r4 = uint64_t(int64_t(164));
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82262398, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822623b4
  if (cr0.eq) goto loc_822623B4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822623A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82260d38
  // (left out: dead in C++)
  r3 = f82260D38(base, r3);
  // b 0x82262428
  goto loc_82262428;
loc_822623B4:
  // li r4,161
  r4 = uint64_t(int64_t(161));
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822623C0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822623dc
  if (cr0.eq) goto loc_822623DC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822623D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82260df0
  // (left out: dead in C++)
  r3 = f82260DF0(base, r3);
  // b 0x82262428
  goto loc_82262428;
loc_822623DC:
  // li r4,162
  r4 = uint64_t(int64_t(162));
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822623E8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82262404
  if (cr0.eq) goto loc_82262404;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822623F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82260e90
  // (left out: dead in C++)
  r3 = f82260E90(base, r3);
  // b 0x82262428
  goto loc_82262428;
loc_82262404:
  // li r4,157
  r4 = uint64_t(int64_t(157));
  // lwz r3,24904(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82262410, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82262474
  if (cr0.eq) goto loc_82262474;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82262420, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82260f58
  // (left out: dead in C++)
  r3 = f82260F58(base, r3);
loc_82262428:
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x82262474
  if (!cr6.eq) goto loc_82262474;
  // mr r4,r27
  r4 = r27;
  // mr r3,r30
  r3 = r30;
  // bl 0x82261c20
  // (left out: dead in C++)
  { const lifted::Result o = f82261C20(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82262474
  goto loc_82262474;
loc_82262440:
  // lwz r11,24904(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // lwz r10,9640(r30)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r30)).unknown_9640_address());
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // ble cr6,0x82262474
  if (!cr6.gt) goto loc_82262474;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r3,-21596(r28)
  r3 = guest::LoadU32(base, uint32_t(r28) + -21596);
  // mulli r11,r11,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(48))));
  // lwz r10,27348(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 27348);
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // ldx r4,r10,r11
  r4 = guest::LoadU64(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x822952d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XShowGamerCardUI(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, 0x82262474); r3 = o.r3; f1 = o.f1; }
loc_82262474:
  // lwz r11,24904(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24904);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822624b8
  if (cr6.eq) goto loc_822624B8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8226248C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24900(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 24900);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822624a0
  if (cr0.eq) goto loc_822624A0;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822624A0:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r25,16(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address(), uint32_t(r25));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r25,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r25));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address(), uint32_t(r9));
loc_822624B8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822624C0: lifted from generated/ by lift.py.
lifted::Result f822624C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822624C0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822624EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226263c
  if (!cr6.eq) goto loc_8226263C;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24904(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24904);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // bl 0x822617c0
  // (left out: dead in C++)
  { const lifted::Result o = f822617C0(base, r1, r13, r3, r4, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82261a08
  // (left out: dead in C++)
  { const lifted::Result o = f82261A08(base, r1, r13, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r6,959
  r6 = uint64_t(int64_t(959));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82262530, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = lifted::entry::e8211C4F0(base, LIFTED_ARGS(0x8226254C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82262560, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226257C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,38
  r4 = uint64_t(int64_t(38));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82262588, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,39
  r4 = uint64_t(int64_t(39));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822625A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,39
  r4 = uint64_t(int64_t(39));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822625B0, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822625CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822625D8, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822625F4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x82262610, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9644(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9644);
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bgt cr6,0x82262630
  if (cr6.gt) goto loc_82262630;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226262C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82262634
  goto loc_82262634;
loc_82262630:
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82262634, r31)); r3 = o.r3; f1 = o.f1; }
loc_82262634:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_8226263C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82262648, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82262678: lifted from generated/ by lift.py.
lifted::Result f82262678(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82262678 (lifted)");
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
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // mr r28,r4
  r28 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822626A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82262804
  if (!cr6.eq) goto loc_82262804;
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_822626B8:
  // addi r4,r31,23
  r4 = uint64_t(int64_t(int64_t(r31) + 23));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822626C4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,7
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 7, so);
  // blt cr6,0x822626b8
  if (cr6.lt) goto loc_822626B8;
  // lwz r11,9640(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9640);
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // mr r3,r29
  r3 = r29;
  // bgt cr6,0x822626f0
  if (cr6.gt) goto loc_822626F0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x82262700
  goto loc_82262700;
loc_822626F0:
  // lwz r11,24904(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24904);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // subf r4,r11,r10
  r4 = r10 - r11;
loc_82262700:
  // bl 0x822617c0
  // (left out: dead in C++)
  { const lifted::Result o = f822617C0(base, r1, r13, r3, r4, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9644(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9644);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // lwz r11,24904(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24904);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // blt cr6,0x82262740
  if (cr6.lt) goto loc_82262740;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82262734, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24904);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // b 0x82262754
  goto loc_82262754;
loc_82262740:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226274C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24904);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
loc_82262754:
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82262760, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24904);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // subf. r11,r11,r10
  r11 = r10 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82262794
  if (!cr0.eq) goto loc_82262794;
  // lwz r11,9648(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9648);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82262794
  if (!cr6.eq) goto loc_82262794;
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82262790, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822627a4
  goto loc_822627A4;
loc_82262794:
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822627A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_822627A4:
  // lwz r11,24904(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24904);
  // lwz r10,9640(r29)
  r10 = guest::LoadU32(base, uint32_t(r29) + 9640);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // subf r11,r11,r9
  r11 = r9 - r11;
  // addi r9,r10,-7
  r9 = uint64_t(int64_t(int64_t(r10) + -7));
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bne cr6,0x822627ec
  if (!cr6.eq) goto loc_822627EC;
  // lwz r11,9648(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 9648);
  // lwz r9,9644(r29)
  r9 = guest::LoadU32(base, uint32_t(r29) + 9644);
  // add r11,r11,r10
  r11 = r11 + r10;
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bne cr6,0x822627ec
  if (!cr6.eq) goto loc_822627EC;
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822627E8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822627fc
  goto loc_822627FC;
loc_822627EC:
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822627FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_822627FC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_82262804:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82262810, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82262820: lifted from generated/ by lift.py.
lifted::Result f82262820(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82262820 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // mr r27,r4
  r27 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8226284C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82262a6c
  if (!cr6.eq) goto loc_82262A6C;
  // lwz r11,9640(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9640);
  // li r4,31
  r4 = uint64_t(int64_t(31));
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bgt cr6,0x822628e4
  if (cr6.gt) goto loc_822628E4;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82262874, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82262880, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_address());
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82262898, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822628BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822628D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x822617c0
  // (left out: dead in C++)
  { const lifted::Result o = f822617C0(base, r1, r13, r3, r4, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82262a64
  goto loc_82262A64;
loc_822628E4:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822628E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822628F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_row_address());
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8226290C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82262930, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // addi r4,r11,23
  r4 = uint64_t(int64_t(int64_t(r11) + 23));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82262944, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r10,52(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).previous_row_address());
  // lwz r9,48(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // bne cr6,0x8226299c
  if (!cr6.eq) goto loc_8226299C;
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).moved_address());
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82262970
  if (!cr6.eq) goto loc_82262970;
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // b 0x82262980
  goto loc_82262980;
loc_82262970:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x8226299c
  if (!cr6.eq) goto loc_8226299C;
  // li r4,31
  r4 = uint64_t(int64_t(31));
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_82262980:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82262998, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
loc_8226299C:
  // lwz r10,9640(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 9640);
  // mr r5,r29
  r5 = r29;
  // mr r3,r28
  r3 = r28;
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // bgt cr6,0x822629b8
  if (cr6.gt) goto loc_822629B8;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x822629c4
  goto loc_822629C4;
loc_822629B8:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // subf r4,r11,r10
  r4 = r10 - r11;
loc_822629C4:
  // bl 0x822617c0
  // (left out: dead in C++)
  { const lifted::Result o = f822617C0(base, r1, r13, r3, r4, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // subf. r11,r11,r10
  r11 = r10 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822629fc
  if (!cr0.eq) goto loc_822629FC;
  // lwz r11,9648(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9648);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822629fc
  if (!cr6.eq) goto loc_822629FC;
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822629F8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82262a0c
  goto loc_82262A0C;
loc_822629FC:
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82262A0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82262A0C:
  // lwz r11,24904(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24904);
  // lwz r10,9640(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 9640);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).cursor_address());
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Menu(base, uint32_t(r11)).visible_row_address());
  // subf r11,r11,r9
  r11 = r9 - r11;
  // addi r9,r10,-7
  r9 = uint64_t(int64_t(int64_t(r10) + -7));
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bne cr6,0x82262a54
  if (!cr6.eq) goto loc_82262A54;
  // lwz r11,9648(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 9648);
  // lwz r9,9644(r28)
  r9 = guest::LoadU32(base, uint32_t(r28) + 9644);
  // add r11,r11,r10
  r11 = r11 + r10;
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bne cr6,0x82262a54
  if (!cr6.eq) goto loc_82262A54;
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82262A50, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82262a64
  goto loc_82262A64;
loc_82262A54:
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82262A64, r31)); r3 = o.r3; f1 = o.f1; }
loc_82262A64:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r11));
loc_82262A6C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82262A78, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82262E10: lifted from generated/ by lift.py.
lifted::Result f82262E10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82262E10 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9664(r3)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Machine(base, uint32_t(r3)).unknown_9664_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24900);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // bne cr6,0x82262ef8
  if (!cr6.eq) goto loc_82262EF8;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82262ef4
  if (cr0.eq) goto loc_82262EF4;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x82262e90
  if (cr6.eq) goto loc_82262E90;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,28
  r9 = uint64_t(int64_t(28));
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
loc_82262E90:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,8
  r10 = uint64_t(int64_t(8));
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
  // stw r11,9664(r3)
  guest::StoreU32(base, schema::ModeTickNet8Machine(base, uint32_t(r3)).unknown_9664_address(), uint32_t(r11));
  // blr
  return {r3, f1};
loc_82262EF4:
  // b 0x82262040
  { const lifted::Result o = f82262040(base, r1, r13, r3, r4, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82262EF8:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82262f18
  if (cr6.eq) goto loc_82262F18;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82262f14
  if (cr6.eq) goto loc_82262F14;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x82260a30
  { const lifted::Result o = f82260A30(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82262F14:
  // b 0x82261600
  { const lifted::Result o = f82261600(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82262F18:
  // b 0x822614c0
  { const lifted::Result o = f822614C0(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82262F20: lifted from generated/ by lift.py.
lifted::Result f82262F20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82262F20 (lifted)");
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
  // li r11,42
  r11 = uint64_t(int64_t(42));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82262F50, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,24900(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24900);
  // addi r4,r11,24
  r4 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).step_address());
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82262f84
  if (!cr6.eq) goto loc_82262F84;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x8226302c
  if (cr6.eq) goto loc_8226302C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82262F80, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226302c
  goto loc_8226302C;
loc_82262F84:
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x82263008
  if (cr6.lt) goto loc_82263008;
  // beq cr6,0x82262ffc
  if (cr6.eq) goto loc_82262FFC;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x82262ff0
  if (cr6.lt) goto loc_82262FF0;
  // beq cr6,0x82262fc0
  if (cr6.eq) goto loc_82262FC0;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82263018
  if (!cr6.eq) goto loc_82263018;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x82263028
  goto loc_82263028;
loc_82262FC0:
  // mr r3,r30
  r3 = r30;
  // bl 0x82261710
  // (left out: dead in C++)
  { const lifted::Result o = f82261710(base, r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_82262FC8:
  // lwz r11,24900(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24900);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226302c
  if (cr0.eq) goto loc_8226302C;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).queued_step_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82263014
  if (!cr6.eq) goto loc_82263014;
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).next_page_address());
  // stw r8,0(r11)
  guest::StoreU32(base, schema::ModeTickNet8Page(base, uint32_t(r11)).page_address(), uint32_t(r8));
  // b 0x82263024
  goto loc_82263024;
loc_82262FF0:
  // mr r3,r30
  r3 = r30;
  // bl 0x82262820
  // (left out: dead in C++)
  { const lifted::Result o = f82262820(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82262fc8
  goto loc_82262FC8;
loc_82262FFC:
  // mr r3,r30
  r3 = r30;
  // bl 0x82262678
  // (left out: dead in C++)
  { const lifted::Result o = f82262678(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82262fc8
  goto loc_82262FC8;
loc_82263008:
  // mr r3,r30
  r3 = r30;
  // bl 0x822624c0
  // (left out: dead in C++)
  { const lifted::Result o = f822624C0(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82262fc8
  goto loc_82262FC8;
loc_82263014:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_82263018:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
  // stw r8,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r8));
loc_82263024:
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
loc_82263028:
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
loc_8226302C:
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

// sub_82263058: lifted from generated/ by lift.py.
void f82263058(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82263058 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9644(r3)
  guest::StoreU32(base, uint32_t(r3) + 9644, uint32_t(r11));
  // blr
  return;
}

// sub_82263068: lifted from generated/ by lift.py.
void f82263068(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82263068 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9644(r3)
  guest::StoreU32(base, uint32_t(r3) + 9644, uint32_t(r11));
  // blr
  return;
}

// sub_82263078: lifted from generated/ by lift.py.
void f82263078(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82263078 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24908(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82263090
  if (cr6.eq) goto loc_82263090;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82263090:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // stw r8,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r8));
  // stw r7,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r7));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,9648(r3)
  guest::StoreU32(base, uint32_t(r3) + 9648, uint32_t(r10));
  // stw r10,9640(r3)
  guest::StoreU32(base, uint32_t(r3) + 9640, uint32_t(r10));
  // blr
  return;
}

// sub_82263340: lifted from generated/ by lift.py.
void f82263340(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82263340 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82263384
  if (cr6.eq) goto loc_82263384;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24908(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24908);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82263364
  if (cr0.eq) goto loc_82263364;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82263364:
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
loc_82263384:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_82263390: lifted from generated/ by lift.py.
lifted::Result f82263390(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82263390 (lifted)");
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
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822633B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x822633BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822633D0: lifted from generated/ by lift.py.
void f822633D0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822633D0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,9648(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9648);
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // bne cr6,0x82263400
  if (!cr6.eq) goto loc_82263400;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,24924(r9)
  guest::StoreU32(base, uint32_t(r9) + 24924, uint32_t(r10));
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // b 0x82263404
  goto loc_82263404;
loc_82263400:
  // li r9,13
  r9 = uint64_t(int64_t(13));
loc_82263404:
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

// sub_82263508: lifted from generated/ by lift.py.
lifted::Result f82263508(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82263508 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // mr r25,r4
  r25 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82263534, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82263774
  if (!cr6.eq) goto loc_82263774;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,45
  r4 = uint64_t(int64_t(45));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82263560, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,45
  r4 = uint64_t(int64_t(45));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8226356C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24912(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24912);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822635b8
  if (!cr0.eq) goto loc_822635B8;
  // lwz r10,9680(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 9680);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x822635b8
  if (cr6.eq) goto loc_822635B8;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8226359C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = lifted::Call(base, 0x821E7DA0, LIFTED_ARGS(0x822635B4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822635e8
  goto loc_822635E8;
loc_822635B8:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822635D4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822635E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822635E8:
  // lwz r11,9672(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 9672);
  // lwz r10,9668(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 9668);
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // bge cr6,0x8226361c
  if (!cr6.lt) goto loc_8226361C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,61
  r4 = uint64_t(int64_t(61));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82263614, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,61
  r4 = uint64_t(int64_t(61));
  // b 0x82263670
  goto loc_82263670;
loc_8226361C:
  // ble cr6,0x82263650
  if (!cr6.gt) goto loc_82263650;
  // lwz r11,9676(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 9676);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82263650
  if (!cr6.eq) goto loc_82263650;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,62
  r4 = uint64_t(int64_t(62));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82263648, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,62
  r4 = uint64_t(int64_t(62));
  // b 0x82263670
  goto loc_82263670;
loc_82263650:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,64
  r4 = uint64_t(int64_t(64));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8226366C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,64
  r4 = uint64_t(int64_t(64));
loc_82263670:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82263678, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r28,50
  r28 = uint64_t(int64_t(50));
  // lis r27,-32148
  r27 = uint64_t(int64_t(-2106851328));
loc_82263684:
  // lwz r11,29312(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 29312);
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // ble cr6,0x8226373c
  if (!cr6.gt) goto loc_8226373C;
  // lwz r11,9668(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 9668);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // ble cr6,0x822636c8
  if (!cr6.gt) goto loc_822636C8;
  // mr r4,r28
  r4 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822636B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x822636C4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822636d0
  goto loc_822636D0;
loc_822636C8:
  // addi r4,r28,-1
  r4 = uint64_t(int64_t(int64_t(r28) + -1));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822636D0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822636D0:
  // lwz r11,9672(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 9672);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // ble cr6,0x8226370c
  if (!cr6.gt) goto loc_8226370C;
  // addi r29,r28,6
  r29 = uint64_t(int64_t(int64_t(r28) + 6));
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822636FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x82263708, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82263714
  goto loc_82263714;
loc_8226370C:
  // addi r4,r28,5
  r4 = uint64_t(int64_t(int64_t(r28) + 5));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82263714, r31)); r3 = o.r3; f1 = o.f1; }
loc_82263714:
  // addi r29,r30,43
  r29 = uint64_t(int64_t(int64_t(r30) + 43));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // addi r5,r29,-3
  r5 = uint64_t(int64_t(int64_t(r29) + -3));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263728, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82263738, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226375c
  goto loc_8226375C;
loc_8226373C:
  // addi r29,r30,43
  r29 = uint64_t(int64_t(int64_t(r30) + 43));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r5,r29,-3
  r5 = uint64_t(int64_t(int64_t(r29) + -3));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226374C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x8226375C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226375C:
  // addi r28,r28,2
  r28 = uint64_t(int64_t(int64_t(r28) + 2));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r28,56
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 56, so);
  // blt cr6,0x82263684
  if (cr6.lt) goto loc_82263684;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r25)
  guest::StoreU32(base, uint32_t(r25) + 0, uint32_t(r11));
loc_82263774:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82263780, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82263790: lifted from generated/ by lift.py.
lifted::Result f82263790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82263790 (lifted)");
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
  // li r11,43
  r11 = uint64_t(int64_t(43));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822637C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822637f4
  if (!cr6.eq) goto loc_822637F4;
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
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x822637EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_822637F4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82263800, r31)); r3 = o.r3; f1 = o.f1; }
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

lifted::Result e8216C798(uint8_t* base, const lifted::Args& a) {
  return f8216C798(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5]);
}

lifted::Result e8216D328(uint8_t* base, const lifted::Args& a) {
  return f8216D328(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e8216D7B8(uint8_t* base, const lifted::Args& a) {
  return f8216D7B8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8217A758(uint8_t* base, const lifted::Args& a) {
  return f8217A758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B8290(uint8_t* base, const lifted::Args& a) {
  return f821B8290(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B8528(uint8_t* base, const lifted::Args& a) {
  f821B8528(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821B88A0(uint8_t* base, const lifted::Args& a) {
  return f821B88A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82212100(uint8_t* base, const lifted::Args& a) {
  f82212100(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822157E8(uint8_t* base, const lifted::Args& a) {
  f822157E8(base, a.r1, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82215870(uint8_t* base, const lifted::Args& a) {
  return {f82215870(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e822158E8(uint8_t* base, const lifted::Args& a) {
  return f822158E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e82215C68(uint8_t* base, const lifted::Args& a) {
  return {f82215C68(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82215CB8(uint8_t* base, const lifted::Args& a) {
  return {f82215CB8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82215E58(uint8_t* base, const lifted::Args& a) {
  return f82215E58(base, a.r1, a.r13, a.r[1], a.r[2], a.f[0]);
}

lifted::Result e82215F60(uint8_t* base, const lifted::Args& a) {
  return {f82215F60(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]), a.f[0]};
}

lifted::Result e82215FF8(uint8_t* base, const lifted::Args& a) {
  return {f82215FF8(base, a.r1, a.r13, a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e822160A0(uint8_t* base, const lifted::Args& a) {
  return {f822160A0(base, a.r1, a.r13, a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82216118(uint8_t* base, const lifted::Args& a) {
  return {f82216118(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]), a.f[0]};
}

lifted::Result e822161D0(uint8_t* base, const lifted::Args& a) {
  return f822161D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.f[0]);
}

lifted::Result e82216428(uint8_t* base, const lifted::Args& a) {
  return {f82216428(base, a.r1, a.r13, a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82216578(uint8_t* base, const lifted::Args& a) {
  return f82216578(base, a.r1, a.r13, a.r[1], a.f[0]);
}

lifted::Result e82216708(uint8_t* base, const lifted::Args& a) {
  return f82216708(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.f[0]);
}

lifted::Result e82216830(uint8_t* base, const lifted::Args& a) {
  return {f82216830(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82216850(uint8_t* base, const lifted::Args& a) {
  return f82216850(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e82216908(uint8_t* base, const lifted::Args& a) {
  return f82216908(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e82216950(uint8_t* base, const lifted::Args& a) {
  return f82216950(base, a.r1, a.r13, a.r[0], a.r[4]);
}

lifted::Result e82216A40(uint8_t* base, const lifted::Args& a) {
  return f82216A40(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e8225DF98(uint8_t* base, const lifted::Args& a) {
  f8225DF98(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8225DFB8(uint8_t* base, const lifted::Args& a) {
  f8225DFB8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8225E268(uint8_t* base, const lifted::Args& a) {
  f8225E268(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8225E2F8(uint8_t* base, const lifted::Args& a) {
  f8225E2F8(base, a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8225E308(uint8_t* base, const lifted::Args& a) {
  return {f8225E308(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e8225E3B0(uint8_t* base, const lifted::Args& a) {
  return f8225E3B0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8225E418(uint8_t* base, const lifted::Args& a) {
  return f8225E418(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8225E5C8(uint8_t* base, const lifted::Args& a) {
  return f8225E5C8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8225E790(uint8_t* base, const lifted::Args& a) {
  return f8225E790(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8225E8D8(uint8_t* base, const lifted::Args& a) {
  return f8225E8D8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e8225E998(uint8_t* base, const lifted::Args& a) {
  return f8225E998(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8225EA38(uint8_t* base, const lifted::Args& a) {
  return {f8225EA38(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8225EAA8(uint8_t* base, const lifted::Args& a) {
  return f8225EAA8(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e8225EB40(uint8_t* base, const lifted::Args& a) {
  return f8225EB40(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e8225EBC0(uint8_t* base, const lifted::Args& a) {
  return f8225EBC0(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8225EC70(uint8_t* base, const lifted::Args& a) {
  return f8225EC70(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8225EF18(uint8_t* base, const lifted::Args& a) {
  return f8225EF18(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8225F078(uint8_t* base, const lifted::Args& a) {
  return f8225F078(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e8225F100(uint8_t* base, const lifted::Args& a) {
  return f8225F100(base, a.r1, a.r13, a.r[2], a.f[0]);
}

lifted::Result e8225FB88(uint8_t* base, const lifted::Args& a) {
  f8225FB88(base, a.r1, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8225FCB8(uint8_t* base, const lifted::Args& a) {
  return {f8225FCB8(base, a.r[0]), a.f[0]};
}

lifted::Result e8225FD18(uint8_t* base, const lifted::Args& a) {
  return {f8225FD18(base, a.r[0]), a.f[0]};
}

lifted::Result e8225FD78(uint8_t* base, const lifted::Args& a) {
  return {f8225FD78(base, a.r[0]), a.f[0]};
}

lifted::Result e8225FDD0(uint8_t* base, const lifted::Args& a) {
  return {f8225FDD0(base, a.r[0]), a.f[0]};
}

lifted::Result e8225FE30(uint8_t* base, const lifted::Args& a) {
  return {f8225FE30(base, a.r[0]), a.f[0]};
}

lifted::Result e8225FE98(uint8_t* base, const lifted::Args& a) {
  return {f8225FE98(base, a.r[0]), a.f[0]};
}

lifted::Result e8225FF00(uint8_t* base, const lifted::Args& a) {
  return {f8225FF00(base, a.r[0]), a.f[0]};
}

lifted::Result e8225FF68(uint8_t* base, const lifted::Args& a) {
  return {f8225FF68(base, a.r[0]), a.f[0]};
}

lifted::Result e82260170(uint8_t* base, const lifted::Args& a) {
  return {f82260170(base, a.r[0]), a.f[0]};
}

lifted::Result e822601B0(uint8_t* base, const lifted::Args& a) {
  return {f822601B0(base, a.r[0]), a.f[0]};
}

lifted::Result e822601F0(uint8_t* base, const lifted::Args& a) {
  return {f822601F0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822603D8(uint8_t* base, const lifted::Args& a) {
  return f822603D8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82260468(uint8_t* base, const lifted::Args& a) {
  return {f82260468(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82260680(uint8_t* base, const lifted::Args& a) {
  f82260680(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822606B8(uint8_t* base, const lifted::Args& a) {
  f822606B8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822606E8(uint8_t* base, const lifted::Args& a) {
  f822606E8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82260758(uint8_t* base, const lifted::Args& a) {
  return f82260758(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82260908(uint8_t* base, const lifted::Args& a) {
  return f82260908(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822609F0(uint8_t* base, const lifted::Args& a) {
  f822609F0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82260A30(uint8_t* base, const lifted::Args& a) {
  return f82260A30(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82260AE8(uint8_t* base, const lifted::Args& a) {
  return {f82260AE8(base, a.r[0]), a.f[0]};
}

lifted::Result e82260BC0(uint8_t* base, const lifted::Args& a) {
  return {f82260BC0(base, a.r[0]), a.f[0]};
}

lifted::Result e82260CB8(uint8_t* base, const lifted::Args& a) {
  return {f82260CB8(base, a.r[0]), a.f[0]};
}

lifted::Result e82260D38(uint8_t* base, const lifted::Args& a) {
  return {f82260D38(base, a.r[0]), a.f[0]};
}

lifted::Result e82260DF0(uint8_t* base, const lifted::Args& a) {
  return {f82260DF0(base, a.r[0]), a.f[0]};
}

lifted::Result e82260E90(uint8_t* base, const lifted::Args& a) {
  return {f82260E90(base, a.r[0]), a.f[0]};
}

lifted::Result e82260F58(uint8_t* base, const lifted::Args& a) {
  return {f82260F58(base, a.r[0]), a.f[0]};
}

lifted::Result e82261250(uint8_t* base, const lifted::Args& a) {
  f82261250(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822612A0(uint8_t* base, const lifted::Args& a) {
  return f822612A0(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822612E0(uint8_t* base, const lifted::Args& a) {
  f822612E0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82261418(uint8_t* base, const lifted::Args& a) {
  return {f82261418(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e822614C0(uint8_t* base, const lifted::Args& a) {
  return f822614C0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82261600(uint8_t* base, const lifted::Args& a) {
  return f82261600(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82261710(uint8_t* base, const lifted::Args& a) {
  return f82261710(base, a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e822617C0(uint8_t* base, const lifted::Args& a) {
  return f822617C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.f[0]);
}

lifted::Result e82261A08(uint8_t* base, const lifted::Args& a) {
  return f82261A08(base, a.r1, a.r13, a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82261C20(uint8_t* base, const lifted::Args& a) {
  return f82261C20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82261CE8(uint8_t* base, const lifted::Args& a) {
  return {f82261CE8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82261E38(uint8_t* base, const lifted::Args& a) {
  return f82261E38(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82262040(uint8_t* base, const lifted::Args& a) {
  return f82262040(base, a.r1, a.r13, a.r[0], a.r[1], a.r[6], a.f[0]);
}

lifted::Result e822624C0(uint8_t* base, const lifted::Args& a) {
  return f822624C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82262678(uint8_t* base, const lifted::Args& a) {
  return f82262678(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82262820(uint8_t* base, const lifted::Args& a) {
  return f82262820(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82262E10(uint8_t* base, const lifted::Args& a) {
  return f82262E10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82262F20(uint8_t* base, const lifted::Args& a) {
  return f82262F20(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82263058(uint8_t* base, const lifted::Args& a) {
  f82263058(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82263068(uint8_t* base, const lifted::Args& a) {
  f82263068(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82263078(uint8_t* base, const lifted::Args& a) {
  f82263078(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82263340(uint8_t* base, const lifted::Args& a) {
  f82263340(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82263390(uint8_t* base, const lifted::Args& a) {
  return f82263390(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822633D0(uint8_t* base, const lifted::Args& a) {
  f822633D0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82263508(uint8_t* base, const lifted::Args& a) {
  return f82263508(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.f[0]);
}

lifted::Result e82263790(uint8_t* base, const lifted::Args& a) {
  return f82263790(base, a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x8216C798, e8216C798},
    {0x8216D328, e8216D328},
    {0x8216D7B8, e8216D7B8},
    {0x8217A758, e8217A758},
    {0x821B8290, e821B8290},
    {0x821B8528, e821B8528},
    {0x821B88A0, e821B88A0},
    {0x82212100, e82212100},
    {0x822157E8, e822157E8},
    {0x82215870, e82215870},
    {0x822158E8, e822158E8},
    {0x82215C68, e82215C68},
    {0x82215CB8, e82215CB8},
    {0x82215E58, e82215E58},
    {0x82215F60, e82215F60},
    {0x82215FF8, e82215FF8},
    {0x822160A0, e822160A0},
    {0x82216118, e82216118},
    {0x822161D0, e822161D0},
    {0x82216428, e82216428},
    {0x82216578, e82216578},
    {0x82216708, e82216708},
    {0x82216830, e82216830},
    {0x82216850, e82216850},
    {0x82216908, e82216908},
    {0x82216950, e82216950},
    {0x82216A40, e82216A40},
    {0x8225DF98, e8225DF98},
    {0x8225DFB8, e8225DFB8},
    {0x8225E268, e8225E268},
    {0x8225E2F8, e8225E2F8},
    {0x8225E308, e8225E308},
    {0x8225E3B0, e8225E3B0},
    {0x8225E418, e8225E418},
    {0x8225E5C8, e8225E5C8},
    {0x8225E790, e8225E790},
    {0x8225E8D8, e8225E8D8},
    {0x8225E998, e8225E998},
    {0x8225EA38, e8225EA38},
    {0x8225EAA8, e8225EAA8},
    {0x8225EB40, e8225EB40},
    {0x8225EBC0, e8225EBC0},
    {0x8225EC70, e8225EC70},
    {0x8225EF18, e8225EF18},
    {0x8225F078, e8225F078},
    {0x8225F100, e8225F100},
    {0x8225FB88, e8225FB88},
    {0x8225FCB8, e8225FCB8},
    {0x8225FD18, e8225FD18},
    {0x8225FD78, e8225FD78},
    {0x8225FDD0, e8225FDD0},
    {0x8225FE30, e8225FE30},
    {0x8225FE98, e8225FE98},
    {0x8225FF00, e8225FF00},
    {0x8225FF68, e8225FF68},
    {0x82260170, e82260170},
    {0x822601B0, e822601B0},
    {0x822601F0, e822601F0},
    {0x822603D8, e822603D8},
    {0x82260468, e82260468},
    {0x82260680, e82260680},
    {0x822606B8, e822606B8},
    {0x822606E8, e822606E8},
    {0x82260758, e82260758},
    {0x82260908, e82260908},
    {0x822609F0, e822609F0},
    {0x82260A30, e82260A30},
    {0x82260AE8, e82260AE8},
    {0x82260BC0, e82260BC0},
    {0x82260CB8, e82260CB8},
    {0x82260D38, e82260D38},
    {0x82260DF0, e82260DF0},
    {0x82260E90, e82260E90},
    {0x82260F58, e82260F58},
    {0x82261250, e82261250},
    {0x822612A0, e822612A0},
    {0x822612E0, e822612E0},
    {0x82261418, e82261418},
    {0x822614C0, e822614C0},
    {0x82261600, e82261600},
    {0x82261710, e82261710},
    {0x822617C0, e822617C0},
    {0x82261A08, e82261A08},
    {0x82261C20, e82261C20},
    {0x82261CE8, e82261CE8},
    {0x82261E38, e82261E38},
    {0x82262040, e82262040},
    {0x822624C0, e822624C0},
    {0x82262678, e82262678},
    {0x82262820, e82262820},
    {0x82262E10, e82262E10},
    {0x82262F20, e82262F20},
    {0x82263058, e82263058},
    {0x82263068, e82263068},
    {0x82263078, e82263078},
    {0x82263340, e82263340},
    {0x82263390, e82263390},
    {0x822633D0, e822633D0},
    {0x82263508, e82263508},
    {0x82263790, e82263790},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_037)), true);

}  // namespace

}  // namespace lifted
