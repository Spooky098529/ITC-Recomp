// Lift sweep batch 040 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/boot_mode.h"
#include "schema/menu_helpers.h"
#include "schema/network_mode.h"
#include "schema/options_screen.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies_game.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_040, true, "ITC",
              "Run lift sweep batch 040's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_8227E088: lifted from generated/ by lift.py.
void f8227E088(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8227E088 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8227e0cc
  if (cr6.eq) goto loc_8227E0CC;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24988(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24988);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8227e0ac
  if (cr0.eq) goto loc_8227E0AC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227E0AC:
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
loc_8227E0CC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_8227E0D8: lifted from generated/ by lift.py.
lifted::Result f8227E0D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8227E0D8 (lifted)");
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
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227E0FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x8227E104, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8227E1D8: lifted from generated/ by lift.py.
void f8227E1D8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8227E1D8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r9,40
  r9 = uint64_t(int64_t(40));
  // lwz r11,24996(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24996);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
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
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // beq cr6,0x8227e234
  if (cr6.eq) goto loc_8227E234;
  // li r9,41
  r9 = uint64_t(int64_t(41));
loc_8227E234:
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9640(r3)
  guest::StoreU32(base, uint32_t(r3) + 9640, uint32_t(r11));
  // blr
  return;
}

// sub_8227E248: lifted from generated/ by lift.py.
lifted::Result f8227E248(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8227E248 (lifted)");
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
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r3,24992(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24992);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x8227E268, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24992(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24992);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8227E274, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8227e2e4
  if (cr0.eq) goto loc_8227E2E4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227E288, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24992(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24992);
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r9,39
  r9 = uint64_t(int64_t(39));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
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
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // beq cr6,0x8227e3b4
  if (cr6.eq) goto loc_8227E3B4;
  // b 0x8227e3b0
  goto loc_8227E3B0;
loc_8227E2E4:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24992(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 24992);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8227E2F0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8227e3bc
  if (cr0.eq) goto loc_8227E3BC;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227E300, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24996(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24996);
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x8227e36c
  if (cr6.lt) goto loc_8227E36C;
  // beq cr6,0x8227e320
  if (cr6.eq) goto loc_8227E320;
  // cmplwi cr6,r11,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 9, so);
  // blt cr6,0x8227e36c
  if (cr6.lt) goto loc_8227E36C;
  // bne cr6,0x8227e3b8
  if (!cr6.eq) goto loc_8227E3B8;
loc_8227E320:
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r9,41
  r9 = uint64_t(int64_t(41));
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
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // b 0x8227e3b4
  goto loc_8227E3B4;
loc_8227E36C:
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
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
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
loc_8227E3B0:
  // li r9,40
  r9 = uint64_t(int64_t(40));
loc_8227E3B4:
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
loc_8227E3B8:
  // stw r28,9640(r30)
  guest::StoreU32(base, uint32_t(r30) + 9640, uint32_t(r28));
loc_8227E3BC:
  // lwz r11,9640(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 9640);
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8227e400
  if (!cr6.eq) goto loc_8227E400;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227E3D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24988(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24988);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8227e3ec
  if (cr6.eq) goto loc_8227E3EC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227E3EC:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r30,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r30));
  // stw r28,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r28));
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_8227E400:
  // lwz r11,24992(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24992);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8227e440
  if (cr6.eq) goto loc_8227E440;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227E418, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24988(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24988);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8227e42c
  if (cr0.eq) goto loc_8227E42C;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227E42C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r28,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r28));
  // stw r30,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r30));
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_8227E440:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8227E448: lifted from generated/ by lift.py.
uint64_t f8227E448(uint8_t* base, uint64_t r1, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8227E448 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  // std r30,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  guest::StoreU64(base, uint32_t(r1) + -8, r31);
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r9,33
  r9 = uint64_t(int64_t(33));
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

// sub_8227E4F0: lifted from generated/ by lift.py.
lifted::Result f8227E4F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8227E4F0 (lifted)");
  uint64_t r0 = 0, r3 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r28,r4
  r28 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227E518, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8227e668
  if (!cr6.eq) goto loc_8227E668;
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r11,24996(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24996);
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x8227e54c
  if (cr6.lt) goto loc_8227E54C;
  // beq cr6,0x8227e5a8
  if (cr6.eq) goto loc_8227E5A8;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // blt cr6,0x8227e54c
  if (cr6.lt) goto loc_8227E54C;
  // cmplwi cr6,r11,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 9, so);
  // bne cr6,0x8227e574
  if (!cr6.eq) goto loc_8227E574;
loc_8227E54C:
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8227E560, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,3
  r6 = uint64_t(int64_t(3));
loc_8227E564:
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8227E574, r31)); r3 = o.r3; f1 = o.f1; }
loc_8227E574:
  // lwz r11,24996(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24996);
  // cmplwi cr6,r11,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 9, so);
  // bgt cr6,0x8227e608
  if (cr6.gt) goto loc_8227E608;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6520
  r12 = uint64_t(int64_t(int64_t(r12) + 6520));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // lis r12,-32216
  r12 = uint64_t(int64_t(-2111307776));
  // addi r12,r12,-6744
  r12 = uint64_t(int64_t(int64_t(r12) + -6744));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_8227E5C4;
  case 1:
  goto loc_8227E5CC;
  case 2:
  goto loc_8227E5D4;
  case 3:
  goto loc_8227E5DC;
  case 4:
  goto loc_8227E5E4;
  case 5:
  goto loc_8227E5CC;
  case 6:
  goto loc_8227E5D4;
  case 7:
  goto loc_8227E5EC;
  case 8:
  goto loc_8227E608;
  case 9:
  goto loc_8227E5F4;
  default:
  std::abort();
  }
loc_8227E5A8:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8227E5BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // b 0x8227e564
  goto loc_8227E564;
loc_8227E5C4:
  // li r6,45
  r6 = uint64_t(int64_t(45));
  // b 0x8227e5f8
  goto loc_8227E5F8;
loc_8227E5CC:
  // li r6,41
  r6 = uint64_t(int64_t(41));
  // b 0x8227e5f8
  goto loc_8227E5F8;
loc_8227E5D4:
  // li r6,42
  r6 = uint64_t(int64_t(42));
  // b 0x8227e5f8
  goto loc_8227E5F8;
loc_8227E5DC:
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // b 0x8227e5f8
  goto loc_8227E5F8;
loc_8227E5E4:
  // li r6,54
  r6 = uint64_t(int64_t(54));
  // b 0x8227e5f8
  goto loc_8227E5F8;
loc_8227E5EC:
  // li r6,55
  r6 = uint64_t(int64_t(55));
  // b 0x8227e5f8
  goto loc_8227E5F8;
loc_8227E5F4:
  // li r6,46
  r6 = uint64_t(int64_t(46));
loc_8227E5F8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8227E608, r31)); r3 = o.r3; f1 = o.f1; }
loc_8227E608:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8227E624, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24996(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24996);
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x8227e664
  if (!cr6.eq) goto loc_8227E664;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x8227E63C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r11,r11,63600
  r11 = r11 | 63600;
  // lwzx r11,r3,r11
  r11 = guest::LoadU32(base, uint32_t(r3) + uint32_t(r11));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8227e664
  if (!cr6.eq) goto loc_8227E664;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24992(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24992);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_8227E664:
  // stw r30,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r30));
loc_8227E668:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8227E674, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8227E688: lifted from generated/ by lift.py.
lifted::Result f8227E688(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8227E688 (lifted)");
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
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227E6B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8227e71c
  if (!cr6.eq) goto loc_8227E71C;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8227E6CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8227E6D8, r31)); r3 = o.r3; f1 = o.f1; }
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
  // lwz r11,24992(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24992);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,1
  r4 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8227E700, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24992(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24992);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,1
  r4 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8227E714, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_8227E71C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8227E728, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8227E738: lifted from generated/ by lift.py.
lifted::Result f8227E738(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8227E738 (lifted)");
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
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227E768, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8227e79c
  if (!cr6.eq) goto loc_8227E79C;
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8227E794, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8227E79C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8227E7A8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8227E7C8: lifted from generated/ by lift.py.
uint64_t f8227E7C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8227E7C8 (lifted)");
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
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227E7F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9640(r31)
  r11 = guest::LoadU32(base, schema::NetModeBlockAt125536(base, uint32_t(r31)).unknown_9640_address());
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8227e818
  if (cr6.eq) goto loc_8227E818;
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8227E80C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq 0x8227e81c
  if (cr0.eq) goto loc_8227E81C;
loc_8227E818:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8227E81C:
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

// sub_8227E830: lifted from generated/ by lift.py.
lifted::Result f8227E830(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8227E830 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24988(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24988);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8227e84c
  if (cr6.lt) goto loc_8227E84C;
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x8227e448
  r3 = f8227E448(base, r1, r4);
  return {r3, f1};
loc_8227E84C:
  // b 0x8227e248
  { const lifted::Result o = f8227E248(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8227E858: lifted from generated/ by lift.py.
lifted::Result f8227E858(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8227E858 (lifted)");
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
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227E888, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,24988(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24988);
  // addi r4,r11,24
  r4 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8227e8bc
  if (!cr6.eq) goto loc_8227E8BC;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8227e954
  if (cr6.eq) goto loc_8227E954;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8227E8B8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8227e954
  goto loc_8227E954;
loc_8227E8BC:
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x8227e930
  if (cr6.lt) goto loc_8227E930;
  // beq cr6,0x8227e924
  if (cr6.eq) goto loc_8227E924;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x8227e8f4
  if (cr6.lt) goto loc_8227E8F4;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x8227e940
  if (!cr6.eq) goto loc_8227E940;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x8227e950
  goto loc_8227E950;
loc_8227E8F4:
  // mr r3,r30
  r3 = r30;
  // bl 0x8227e738
  // (left out: dead in C++)
  { const lifted::Result o = f8227E738(base, r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_8227E8FC:
  // lwz r11,24988(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24988);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8227e954
  if (cr0.eq) goto loc_8227E954;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x8227e93c
  if (!cr6.eq) goto loc_8227E93C;
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // b 0x8227e94c
  goto loc_8227E94C;
loc_8227E924:
  // mr r3,r30
  r3 = r30;
  // bl 0x8227e688
  // (left out: dead in C++)
  { const lifted::Result o = f8227E688(base, r1, r13, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8227e8fc
  goto loc_8227E8FC;
loc_8227E930:
  // mr r3,r30
  r3 = r30;
  // bl 0x8227e4f0
  // (left out: dead in C++)
  { const lifted::Result o = f8227E4F0(base, r1, r13, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8227e8fc
  goto loc_8227E8FC;
loc_8227E93C:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_8227E940:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
  // stw r8,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r8));
loc_8227E94C:
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
loc_8227E950:
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
loc_8227E954:
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

// sub_8227E970: lifted from generated/ by lift.py.
lifted::Result f8227E970(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8227E970 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r11,r3
  r11 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // stw r31,9640(r11)
  guest::StoreU32(base, uint32_t(r11) + 9640, uint32_t(r31));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227E994, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24988(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24988);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8227e9ac
  if (cr0.eq) goto loc_8227E9AC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227E9AC:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // lwz r11,24992(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 24992);
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24996(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24996);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x8227ea38
  if (cr0.lt) goto loc_8227EA38;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // ble cr6,0x8227ea28
  if (!cr6.gt) goto loc_8227EA28;
  // addi r11,r11,-5
  r11 = uint64_t(int64_t(int64_t(r11) + -5));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bgt cr6,0x8227ea38
  if (cr6.gt) goto loc_8227EA38;
loc_8227EA28:
  // lwz r11,24992(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 24992);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
loc_8227EA38:
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

// sub_8227ECF8: lifted from generated/ by lift.py.
lifted::Result f8227ECF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8227ECF8 (lifted)");
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
  // beq cr6,0x8227ed48
  if (cr6.eq) goto loc_8227ED48;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227ED18, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25000(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25000);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8227ed30
  if (cr6.eq) goto loc_8227ED30;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227ED30:
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
loc_8227ED48:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8227EF18: lifted from generated/ by lift.py.
void f8227EF18(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8227EF18 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8227ef5c
  if (cr6.eq) goto loc_8227EF5C;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25000(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25000);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8227ef3c
  if (cr0.eq) goto loc_8227EF3C;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227EF3C:
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
loc_8227EF5C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_8227EF68: lifted from generated/ by lift.py.
lifted::Result f8227EF68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8227EF68 (lifted)");
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
  // li r11,17
  r11 = uint64_t(int64_t(17));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227EF8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x8227EF94, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8227EFA8: lifted from generated/ by lift.py.
void f8227EFA8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8227EFA8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,9640(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9640);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bnelr cr6
  if (!cr6.eq) return;
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,39
  r9 = uint64_t(int64_t(39));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25000(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25000);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8227f018
  if (cr6.eq) goto loc_8227F018;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227F018:
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
  // blr
  return;
}

// sub_8227F0F8: lifted from generated/ by lift.py.
void f8227F0F8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8227F0F8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,41
  r9 = uint64_t(int64_t(41));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // li r4,1
  r4 = uint64_t(int64_t(1));
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
  // stw r4,9640(r3)
  guest::StoreU32(base, uint32_t(r3) + 9640, uint32_t(r4));
  // blr
  return;
}

// sub_8227F150: lifted from generated/ by lift.py.
lifted::Result f8227F150(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8227F150 (lifted)");
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
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r30,r4
  r30 = r4;
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // mr r31,r3
  r31 = r3;
  // lwz r3,25004(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 25004);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8227F174, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8227f194
  if (!cr0.eq) goto loc_8227F194;
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,25004(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 25004);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x8227F18C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8227f1ec
  if (cr0.eq) goto loc_8227F1EC;
loc_8227F194:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227F19C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,39
  r9 = uint64_t(int64_t(39));
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
  // stw r28,9640(r31)
  guest::StoreU32(base, uint32_t(r31) + 9640, uint32_t(r28));
loc_8227F1EC:
  // lwz r11,9640(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9640);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8227f22c
  if (!cr6.eq) goto loc_8227F22C;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227F200, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25000(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25000);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8227f218
  if (cr6.eq) goto loc_8227F218;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227F218:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r28,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r28));
  // stw r28,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r28));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_8227F22C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8227F238: lifted from generated/ by lift.py.
uint64_t f8227F238(uint8_t* base, uint64_t r1, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8227F238 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  // std r30,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  guest::StoreU64(base, uint32_t(r1) + -8, r31);
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r9,32
  r9 = uint64_t(int64_t(32));
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

// sub_8227F2E0: lifted from generated/ by lift.py.
lifted::Result f8227F2E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8227F2E0 (lifted)");
  uint64_t r0 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // li r11,17
  r11 = uint64_t(int64_t(17));
  // mr r30,r4
  r30 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227F30C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8227f3e4
  if (!cr6.eq) goto loc_8227F3E4;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25008(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25008);
  // cmplwi cr6,r11,14
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 14, so);
  // bgt cr6,0x8227f3b8
  if (cr6.gt) goto loc_8227F3B8;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6544
  r12 = uint64_t(int64_t(int64_t(r12) + 6544));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // lis r12,-32216
  r12 = uint64_t(int64_t(-2111307776));
  // addi r12,r12,-3244
  r12 = uint64_t(int64_t(int64_t(r12) + -3244));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_8227F354;
  return {r3, f1};
  case 1:
  goto loc_8227F364;
  case 2:
  goto loc_frag_8227F354;
  return {r3, f1};
  case 3:
  goto loc_8227F35C;
  case 4:
  goto loc_8227F364;
  case 5:
  goto loc_8227F36C;
  case 6:
  goto loc_8227F36C;
  case 7:
  goto loc_8227F374;
  case 8:
  goto loc_8227F37C;
  case 9:
  goto loc_8227F384;
  case 10:
  goto loc_8227F38C;
  case 11:
  goto loc_8227F394;
  case 12:
  goto loc_8227F39C;
  case 13:
  goto loc_8227F39C;
  case 14:
  goto loc_8227F3A4;
  default:
  std::abort();
  }
loc_8227F35C:
  // li r6,40
  r6 = uint64_t(int64_t(40));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F364:
  // li r6,38
  r6 = uint64_t(int64_t(38));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F36C:
  // li r6,43
  r6 = uint64_t(int64_t(43));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F374:
  // li r6,44
  r6 = uint64_t(int64_t(44));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F37C:
  // li r6,57
  r6 = uint64_t(int64_t(57));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F384:
  // li r6,47
  r6 = uint64_t(int64_t(47));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F38C:
  // li r6,56
  r6 = uint64_t(int64_t(56));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F394:
  // li r6,95
  r6 = uint64_t(int64_t(95));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F39C:
  // li r6,53
  r6 = uint64_t(int64_t(53));
  // b 0x8227f3a8
  goto loc_8227F3A8;
loc_8227F3A4:
  // li r6,91
  r6 = uint64_t(int64_t(91));
loc_8227F3A8:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8227F3B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8227F3B8:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8227F3D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,9644(r29)
  guest::StoreU32(base, uint32_t(r29) + 9644, uint32_t(r11));
  // stw r10,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r10));
loc_8227F3E4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8227F3F0, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_frag_8227F354:
  // li r6,35
  r6 = uint64_t(int64_t(35));
  // b 0x8227f3a8
  goto loc_8227F3A8_8227F354;
loc_8227F3A8_8227F354:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8227F3B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8227F3D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,9644(r29)
  guest::StoreU32(base, uint32_t(r29) + 9644, uint32_t(r11));
  // stw r10,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r10));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8227F3F0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8227F400: lifted from generated/ by lift.py.
lifted::Result f8227F400(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8227F400 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r11,17
  r11 = uint64_t(int64_t(17));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227F42C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8227f464
  if (!cr6.eq) goto loc_8227F464;
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
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8227F458, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9644(r29)
  guest::StoreU32(base, uint32_t(r29) + 9644, uint32_t(r11));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8227F464:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8227F470, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8227F480: lifted from generated/ by lift.py.
uint64_t f8227F480(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8227F480 (lifted)");
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
  // li r11,17
  r11 = uint64_t(int64_t(17));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227F4AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9644(r31)
  r11 = guest::LoadU32(base, schema::NetModeBlockAt115888(base, uint32_t(r31)).unknown_9644_address());
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8227f4d0
  if (cr6.eq) goto loc_8227F4D0;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BC18, LIFTED_ARGS(0x8227F4C4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq 0x8227f4d4
  if (cr0.eq) goto loc_8227F4D4;
loc_8227F4D0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8227F4D4:
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

// sub_8227F4E8: lifted from generated/ by lift.py.
lifted::Result f8227F4E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8227F4E8 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25000(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25000);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8227f510
  if (cr6.lt) goto loc_8227F510;
  // beq cr6,0x8227f50c
  if (cr6.eq) goto loc_8227F50C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x8227ecf8
  { const lifted::Result o = f8227ECF8(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_8227F50C:
  // b 0x8227f238
  r3 = f8227F238(base, r1, r4);
  return {r3, f1};
loc_8227F510:
  // b 0x8227f150
  { const lifted::Result o = f8227F150(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8227F518: lifted from generated/ by lift.py.
lifted::Result f8227F518(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8227F518 (lifted)");
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
  // li r11,17
  r11 = uint64_t(int64_t(17));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8227F548, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,25000(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25000);
  // addi r4,r11,24
  r4 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8227f57c
  if (!cr6.eq) goto loc_8227F57C;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8227f600
  if (cr6.eq) goto loc_8227F600;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8227F578, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8227f600
  goto loc_8227F600;
loc_8227F57C:
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x8227f5dc
  if (cr6.lt) goto loc_8227F5DC;
  // beq cr6,0x8227f5ac
  if (cr6.eq) goto loc_8227F5AC;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x8227f5ec
  if (!cr6.eq) goto loc_8227F5EC;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x8227f5fc
  goto loc_8227F5FC;
loc_8227F5AC:
  // mr r3,r30
  r3 = r30;
  // bl 0x8227f400
  // (left out: dead in C++)
  { const lifted::Result o = f8227F400(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_8227F5B4:
  // lwz r11,25000(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25000);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8227f600
  if (cr0.eq) goto loc_8227F600;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x8227f5e8
  if (!cr6.eq) goto loc_8227F5E8;
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // b 0x8227f5f8
  goto loc_8227F5F8;
loc_8227F5DC:
  // mr r3,r30
  r3 = r30;
  // bl 0x8227f2e0
  // (left out: dead in C++)
  { const lifted::Result o = f8227F2E0(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8227f5b4
  goto loc_8227F5B4;
loc_8227F5E8:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_8227F5EC:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
  // stw r8,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r8));
loc_8227F5F8:
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
loc_8227F5FC:
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
loc_8227F600:
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

// sub_8227F618: lifted from generated/ by lift.py.
lifted::Result f8227F618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8227F618 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r11,r3
  r11 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // stw r31,9640(r11)
  guest::StoreU32(base, uint32_t(r11) + 9640, uint32_t(r31));
  // stw r31,9644(r11)
  guest::StoreU32(base, uint32_t(r11) + 9644, uint32_t(r31));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8227F640, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25008(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25008);
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x8227f680
  if (cr6.eq) goto loc_8227F680;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // beq cr6,0x8227f680
  if (cr6.eq) goto loc_8227F680;
  // cmpwi cr6,r11,13
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 13, so);
  // beq cr6,0x8227f680
  if (cr6.eq) goto loc_8227F680;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25000(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25000);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8227f678
  if (cr0.eq) goto loc_8227F678;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227F678:
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // b 0x8227f6a0
  goto loc_8227F6A0;
loc_8227F680:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25000(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25000);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x8227f698
  if (cr6.eq) goto loc_8227F698;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_8227F698:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
loc_8227F6A0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
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

// sub_8227F9D8: lifted from generated/ by lift.py.
void f8227F9D8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8227F9D8 (lifted)");
  uint64_t r0 = 0, r10 = 0, r11 = 0, r12 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r3,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 7, so);
  // bgtlr cr6
  if (cr6.gt) return;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6672
  r12 = uint64_t(int64_t(int64_t(r12) + 6672));
  // lbzx r0,r12,r3
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r3));
  // lis r12,-32216
  r12 = uint64_t(int64_t(-2111307776));
  // addi r12,r12,-1528
  r12 = uint64_t(int64_t(int64_t(r12) + -1528));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r3)) {
  case 0:
  goto loc_frag_8227FA08;
  return;
  case 1:
  goto loc_8227FA28;
  case 2:
  goto loc_8227FA48;
  case 3:
  goto loc_8227FA68;
  case 4:
  goto loc_8227FA88;
  case 5:
  goto loc_8227FAA8;
  case 6:
  goto loc_8227FAC8;
  case 7:
  goto loc_8227FAE8;
  default:
  std::abort();
  }
loc_8227FA28:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // beq cr6,0x8227fa40
  if (cr6.eq) goto loc_8227FA40;
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FA40:
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FA48:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // beq cr6,0x8227fa60
  if (cr6.eq) goto loc_8227FA60;
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FA60:
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FA68:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // beq cr6,0x8227fa80
  if (cr6.eq) goto loc_8227FA80;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FA80:
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FA88:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // beq cr6,0x8227faa0
  if (cr6.eq) goto loc_8227FAA0;
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FAA0:
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FAA8:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // beq cr6,0x8227fac0
  if (cr6.eq) goto loc_8227FAC0;
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FAC0:
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FAC8:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // beq cr6,0x8227fae0
  if (cr6.eq) goto loc_8227FAE0;
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FAE0:
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // b 0x8227fb00
  goto loc_8227FB00;
loc_8227FAE8:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // bne cr6,0x8227fb00
  if (!cr6.eq) goto loc_8227FB00;
  // li r10,14
  r10 = uint64_t(int64_t(14));
loc_8227FB00:
  // stw r10,188(r11)
  guest::StoreU32(base, schema::Session(base, uint32_t(r11)).view_id_address(), uint32_t(r10));
  // blr
  return;
loc_frag_8227FA08:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // beq cr6,0x8227fa20
  if (cr6.eq) goto loc_8227FA20_8227FA08;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // b 0x8227fb00
  goto loc_8227FB00_8227FA08;
loc_8227FA20_8227FA08:
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // b 0x8227fb00
  goto loc_8227FB00_8227FA08;
loc_8227FB00_8227FA08:
  // stw r10,188(r11)
  guest::StoreU32(base, uint32_t(r11) + 188, uint32_t(r10));
  // blr
  return;
}

// sub_8227FB08: lifted from generated/ by lift.py.
uint64_t f8227FB08(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8227FB08 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // stw r9,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r9));
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // stw r10,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r10));
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // stw r10,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r10));
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // stw r9,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r9));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x8227FB4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,63600
  r10 = r10 | 63600;
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20);
  // stwx r11,r3,r10
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r10), uint32_t(r11));
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

// sub_8227FB78: lifted from generated/ by lift.py.
uint64_t f8227FB78(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8227FB78 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // stw r9,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r9));
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // stw r10,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r10));
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // stw r10,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r10));
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // stw r8,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r8));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x8227FBC0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,63600
  r10 = r10 | 63600;
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20);
  // stwx r11,r3,r10
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r10), uint32_t(r11));
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

// sub_8227FBE8: lifted from generated/ by lift.py.
void f8227FBE8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8227FBE8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // lwz r10,25020(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 25020);
  // stw r9,40(r10)
  guest::StoreU32(base, uint32_t(r10) + 40, uint32_t(r9));
  // lwz r11,25020(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25020);
  // stw r8,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r8));
  // blr
  return;
}

// sub_8227FC08: lifted from generated/ by lift.py.
void f8227FC08(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8227FC08 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r7,-32127
  r7 = uint64_t(int64_t(-2105475072));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,25020(r7)
  r10 = guest::LoadU32(base, uint32_t(r7) + 25020);
  // lwz r11,25012(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25012);
  // lwz r9,40(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 40);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r6,44(r10)
  r6 = guest::LoadU32(base, uint32_t(r10) + 44);
  // cmpw cr6,r9,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r8), so);
  // beq cr6,0x8227fc30
  if (cr6.eq) goto loc_8227FC30;
  // stw r8,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r8));
loc_8227FC30:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // cmpwi cr6,r6,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), -1, so);
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // bne cr6,0x8227fc4c
  if (!cr6.eq) goto loc_8227FC4C;
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // b 0x8227fc58
  goto loc_8227FC58;
loc_8227FC4C:
  // stw r6,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r6));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_8227FC58:
  // lwz r11,25020(r7)
  r11 = guest::LoadU32(base, uint32_t(r7) + 25020);
  // stw r10,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r10));
  // lwz r11,25020(r7)
  r11 = guest::LoadU32(base, uint32_t(r7) + 25020);
  // stw r10,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r10));
  // blr
  return;
}

// sub_8227FC80: lifted from generated/ by lift.py.
lifted::Result f8227FC80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8227FC80 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r9,64(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 64);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21596
  r11 = uint64_t(int64_t(int64_t(r11) + -21596));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r10,-21632
  r10 = uint64_t(int64_t(int64_t(r10) + -21632));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwzx r11,r9,r11
  r11 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r11));
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r31,r11,r10
  r31 = r11 + r10;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8227fcf0
  if (cr6.eq) goto loc_8227FCF0;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,161
  r4 = uint64_t(int64_t(161));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8227FCE0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8227fcf0
  if (cr0.eq) goto loc_8227FCF0;
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // b 0x8227fd20
  goto loc_8227FD20;
loc_8227FCF0:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8227fd20
  if (cr6.eq) goto loc_8227FD20;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,162
  r4 = uint64_t(int64_t(162));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8227FD14, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8227fd20
  if (cr0.eq) goto loc_8227FD20;
  // li r30,1
  r30 = uint64_t(int64_t(1));
loc_8227FD20:
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

// sub_8227FFC8: lifted from generated/ by lift.py.
void f8227FFC8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8227FFC8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82280048
  if (cr6.eq) goto loc_82280048;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // lwz r11,25020(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25020);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25012(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25012);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8228001c
  if (cr6.eq) goto loc_8228001C;
  // beq 0x82280004
  if (cr0.eq) goto loc_82280004;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82280004:
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
  // b 0x82280038
  goto loc_82280038;
loc_8228001C:
  // beq 0x82280024
  if (cr0.eq) goto loc_82280024;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82280024:
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
loc_82280038:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r9));
  // blr
  return;
loc_82280048:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_82280058: lifted from generated/ by lift.py.
lifted::Result f82280058(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82280058 (lifted)");
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
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82280084, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x8228008C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9616);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822800a8
  if (cr6.eq) goto loc_822800A8;
  // li r3,37
  r3 = uint64_t(int64_t(37));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(base, LIFTED_ARGS(0x822800A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9608(r31)
  guest::StoreU32(base, uint32_t(r31) + 9608, uint32_t(r11));
loc_822800A8:
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

// sub_822800C0: lifted from generated/ by lift.py.
uint64_t f822800C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822800C0 (lifted)");
  uint64_t r0 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r30,r3
  r30 = r3;
  // lwz r11,25020(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25020);
  // lwz r10,24(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x822801ac
  if (cr6.eq) goto loc_822801AC;
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // stw r10,26244(r9)
  guest::StoreU32(base, uint32_t(r9) + 26244, uint32_t(r10));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r10,20(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 20);
  // stw r10,28216(r9)
  guest::StoreU32(base, uint32_t(r9) + 28216, uint32_t(r10));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // stw r10,26240(r9)
  guest::StoreU32(base, uint32_t(r9) + 26240, uint32_t(r10));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lwz r11,25016(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25016);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x8227f9d8
  // (left out: dead in C++)
  f8227F9D8(base, r3, r4);
  // lwz r10,25016(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25016);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bgt cr6,0x82280198
  if (cr6.gt) goto loc_82280198;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6680
  r12 = uint64_t(int64_t(int64_t(r12) + 6680));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // lis r12,-32216
  r12 = uint64_t(int64_t(-2111307776));
  // addi r12,r12,340
  r12 = uint64_t(int64_t(int64_t(r12) + 340));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_82280154;
  return r3;
  case 1:
  goto loc_8228015C;
  case 2:
  goto loc_82280164;
  case 3:
  goto loc_8228016C;
  case 4:
  goto loc_82280174;
  case 5:
  goto loc_8228017C;
  case 6:
  goto loc_82280184;
  case 7:
  goto loc_8228018C;
  default:
  std::abort();
  }
loc_8228015C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x82280190
  goto loc_82280190;
loc_82280164:
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // b 0x82280190
  goto loc_82280190;
loc_8228016C:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // b 0x82280190
  goto loc_82280190;
loc_82280174:
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // b 0x82280190
  goto loc_82280190;
loc_8228017C:
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // b 0x82280190
  goto loc_82280190;
loc_82280184:
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // b 0x82280190
  goto loc_82280190;
loc_8228018C:
  // li r11,7
  r11 = uint64_t(int64_t(7));
loc_82280190:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // stw r11,26208(r9)
  guest::StoreU32(base, uint32_t(r9) + 26208, uint32_t(r11));
loc_82280198:
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // lwz r11,25020(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25020);
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // b 0x82280208
  goto loc_82280208;
loc_822801AC:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // lwz r10,28220(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 28220);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r6,160(r10)
  guest::StoreU32(base, schema::Session(base, uint32_t(r10)).unknown_160_address(), uint32_t(r6));
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
loc_82280208:
  // stw r9,9604(r30)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r30)).state_address(), uint32_t(r9));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
loc_frag_82280154:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x82280190
  goto loc_82280190_82280154;
loc_82280190_82280154:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // stw r11,26208(r9)
  guest::StoreU32(base, uint32_t(r9) + 26208, uint32_t(r11));
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // lwz r11,25020(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25020);
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // b 0x82280208
  goto loc_82280208_82280154;
loc_82280208_82280154:
  // stw r9,9604(r30)
  guest::StoreU32(base, uint32_t(r30) + 9604, uint32_t(r9));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_822802C8: lifted from generated/ by lift.py.
lifted::Result f822802C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822802C8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r31,r3
  r31 = r3;
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x822802E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822802F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,2
  r28 = uint64_t(int64_t(2));
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // beq 0x8228038c
  if (cr0.eq) goto loc_8228038C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82280310, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,24996(r10)
  guest::StoreU32(base, uint32_t(r10) + 24996, uint32_t(r11));
  // lwz r10,25012(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 25012);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82280330
  if (cr6.eq) goto loc_82280330;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_82280330:
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // stw r28,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r28));
  // stw r29,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r29));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // stw r29,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r29));
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // mr r8,r26
  r8 = r26;
  // mr r7,r26
  r7 = r26;
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
  // b 0x822803d0
  goto loc_822803D0;
loc_8228038C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82280398, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822803d0
  if (cr0.eq) goto loc_822803D0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822803A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25012(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 25012);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x822803bc
  if (cr6.eq) goto loc_822803BC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822803BC:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r29,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r29));
  // stw r29,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
loc_822803D0:
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82280410
  if (cr6.eq) goto loc_82280410;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822803E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25012(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 25012);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822803fc
  if (cr0.eq) goto loc_822803FC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822803FC:
  // stw r28,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r28));
  // stw r29,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r29));
  // stw r26,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r26));
  // stw r29,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r29));
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
loc_82280410:
  // lwz r11,36(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 36);
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82280468
  if (cr6.eq) goto loc_82280468;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82280428, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // lwz r11,25012(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 25012);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82280450
  if (cr0.eq) goto loc_82280450;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82280450:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r29,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r29));
  // stw r26,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r26));
  // stw r29,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
loc_82280468:
  // bl 0x8227fc80
  // (left out: dead in C++)
  { const lifted::Result o = f8227FC80(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8228050c
  if (cr0.eq) goto loc_8228050C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8228047C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // bl 0x8227fc80
  // (left out: dead in C++)
  { const lifted::Result o = f8227FC80(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // add. r10,r10,r3
  r10 = r10 + r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge 0x822804a0
  if (!cr0.lt) goto loc_822804A0;
  // mr r10,r28
  r10 = r28;
  // b 0x822804e0
  goto loc_822804E0;
loc_822804A0:
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // bl 0x8227fc80
  // (left out: dead in C++)
  { const lifted::Result o = f8227FC80(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // add r10,r10,r3
  r10 = r10 + r3;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // ble cr6,0x822804c8
  if (!cr6.gt) goto loc_822804C8;
  // mr r10,r26
  r10 = r26;
  // b 0x822804e0
  goto loc_822804E0;
loc_822804C8:
  // lwz r3,25016(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // bl 0x8227fc80
  // (left out: dead in C++)
  { const lifted::Result o = f8227FC80(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25020(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25020);
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // add r10,r10,r3
  r10 = r10 + r3;
loc_822804E0:
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,25012(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 25012);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822804f8
  if (cr0.eq) goto loc_822804F8;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822804F8:
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stw r29,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r29));
  // stw r26,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r26));
  // stw r29,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_8228050C:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82280518: lifted from generated/ by lift.py.
void f82280518(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82280518 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,25
  r9 = uint64_t(int64_t(25));
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
  return;
}

// sub_82280568: lifted from generated/ by lift.py.
lifted::Result f82280568(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82280568 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82280598, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822805cc
  if (!cr6.eq) goto loc_822805CC;
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
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x822805C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_822805CC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822805D8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82280618: lifted from generated/ by lift.py.
lifted::Result f82280618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82280618 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82280648, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82280690
  if (!cr6.eq) goto loc_82280690;
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
  // lwz r11,25016(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25016);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,12
  r4 = uint64_t(int64_t(int64_t(r11) + 12));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82280680, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82280688, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82280690:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8228069C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822806B8: lifted from generated/ by lift.py.
lifted::Result f822806B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822806B8 (lifted)");
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822806DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822806F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280704, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r29,r3
  r29 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,25020(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25020);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82280750
  if (cr6.eq) goto loc_82280750;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82280728, r31)); r3 = o.r3; f1 = o.f1; }
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
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82280744, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // b 0x8228077c
  goto loc_8228077C;
loc_82280750:
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82280758, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82280774, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_8228077C:
  // mr r3,r31
  r3 = r31;
  // stw r11,108(r30)
  guest::StoreU32(base, uint32_t(r30) + 108, uint32_t(r11));
  // stw r10,108(r29)
  guest::StoreU32(base, uint32_t(r29) + 108, uint32_t(r10));
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8228078C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82280798: lifted from generated/ by lift.py.
uint64_t f82280798(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82280798 (lifted)");
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
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822807C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822807D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r11,r3
  r11 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // lwz r10,25020(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 25020);
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // stw r10,108(r11)
  guest::StoreU32(base, uint32_t(r11) + 108, uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822807FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r5,19
  r5 = uint64_t(int64_t(19));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r10,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280818, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25020(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25020);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // stw r11,108(r3)
  guest::StoreU32(base, uint32_t(r3) + 108, uint32_t(r11));
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

// sub_82280840: lifted from generated/ by lift.py.
lifted::Result f82280840(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82280840 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82280864, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,25016(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r4,r11,79
  r4 = uint64_t(int64_t(int64_t(r11) + 79));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82280880, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25016(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25016);
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
  // addi r4,r11,79
  r4 = uint64_t(int64_t(int64_t(r11) + 79));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822808A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x822808AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r11,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r11));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // stw r11,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r11));
  // li r26,27
  r26 = uint64_t(int64_t(27));
  // stw r11,176(r1)
  guest::StoreU32(base, uint32_t(r1) + 176, uint32_t(r11));
  // stw r11,184(r1)
  guest::StoreU32(base, uint32_t(r1) + 184, uint32_t(r11));
  // stw r11,192(r1)
  guest::StoreU32(base, uint32_t(r1) + 192, uint32_t(r11));
  // stw r11,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r11));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r10,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r10));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r29,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r29));
  // stw r29,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r29));
  // stw r9,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r9));
  // stw r11,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r11));
  // stw r11,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r11));
  // stw r11,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r11));
  // stw r11,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r11));
  // stw r11,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r11));
  // stw r11,172(r1)
  guest::StoreU32(base, uint32_t(r1) + 172, uint32_t(r11));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r10,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r10));
  // stw r10,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r10));
  // stw r10,188(r1)
  guest::StoreU32(base, uint32_t(r1) + 188, uint32_t(r10));
  // stw r10,200(r1)
  guest::StoreU32(base, uint32_t(r1) + 200, uint32_t(r10));
  // li r10,25
  r10 = uint64_t(int64_t(25));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // stw r8,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r8));
  // stw r29,196(r1)
  guest::StoreU32(base, uint32_t(r1) + 196, uint32_t(r29));
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // stw r10,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r10));
  // li r10,26
  r10 = uint64_t(int64_t(26));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r10,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r10));
  // stw r11,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r11));
loc_82280954:
  // mr r5,r28
  r5 = r28;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280964, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r27,r3
  r27 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82280970, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r27
  r3 = r27;
  // stw r29,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82280984, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r27
  r3 = r27;
  // stw r29,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82280998, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r27
  r3 = r27;
  // stw r29,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822809AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r5,r28
  r5 = r28;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r29,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822809C4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r28,5
  r5 = uint64_t(int64_t(int64_t(r28) + 5));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822809D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // stw r29,80(r3)
  guest::StoreU32(base, uint32_t(r3) + 80, uint32_t(r29));
  // bne 0x82280954
  if (!cr0.eq) goto loc_82280954;
  // lwz r11,25016(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280A08, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25016(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // addi r10,r1,176
  r10 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r9
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82280A2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r9,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r9));
  // lwz r11,25016(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82280A5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25016(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,6
  r5 = uint64_t(int64_t(int64_t(r11) + 6));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82280A80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25016(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82280ad0
  if (cr6.eq) goto loc_82280AD0;
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82280AAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25016(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25016);
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,6
  r5 = uint64_t(int64_t(int64_t(r11) + 6));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82280AD0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82280AD0:
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82280AD8: lifted from generated/ by lift.py.
uint64_t f82280AD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82280AD8 (lifted)");
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
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82280B08, r31)); r3 = o.r3; f1 = o.f1; }
  // clrldi r11,r31,32
  r11 = r31 & 0xFFFFFFFF;
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,8
  r4 = uint64_t(int64_t(8));
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
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280B2C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82280B48: lifted from generated/ by lift.py.
lifted::Result f82280B48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82280B48 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82280B74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r31
  r6 = r31;
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82280B8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82280B9C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82280BB0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82280BC8: lifted from generated/ by lift.py.
uint64_t f82280BC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82280BC8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82280BF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bne cr6,0x82280c18
  if (!cr6.eq) goto loc_82280C18;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82280C14, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82280c3c
  goto loc_82280C3C;
loc_82280C18:
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // bne cr6,0x82280c38
  if (!cr6.eq) goto loc_82280C38;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82280C24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82280C34, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82280c4c
  goto loc_82280C4C;
loc_82280C38:
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82280C3C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82280C3C:
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82280C4C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82280C4C:
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

// sub_82280C68: lifted from generated/ by lift.py.
lifted::Result f82280C68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82280C68 (lifted)");
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
  // lis r7,-32216
  r7 = uint64_t(int64_t(-2111307776));
  // lis r8,-32240
  r8 = uint64_t(int64_t(-2112880640));
  // lis r9,-32240
  r9 = uint64_t(int64_t(-2112880640));
  // lis r10,-32216
  r10 = uint64_t(int64_t(-2111307776));
  // lwz r11,25012(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25012);
  // addi r7,r7,712
  r7 = uint64_t(int64_t(int64_t(r7) + 712));
  // addi r8,r8,25920
  r8 = uint64_t(int64_t(int64_t(r8) + 25920));
  // addi r9,r9,25920
  r9 = uint64_t(int64_t(int64_t(r9) + 25920));
  // addi r10,r10,1304
  r10 = uint64_t(int64_t(int64_t(r10) + 1304));
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
  // beq cr6,0x82280cd8
  if (cr6.eq) goto loc_82280CD8;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82280CD8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82280CD8:
  // bl 0x82293460
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82293460, LIFTED_ARGS(0x82280CDC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82280CF0: lifted from generated/ by lift.py.
lifted::Result f82280CF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82280CF0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82280D14, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // mr r3,r31
  r3 = r31;
  // stw r31,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r31));
  // bl 0x8211aba8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ResetPose(base, LIFTED_ARGS(0x82280D24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x82280D2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lwz r30,108(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 108);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // lwz r10,25020(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 25020);
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r9,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r9));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // addi r11,r11,31692
  r11 = uint64_t(int64_t(int64_t(r11) + 31692));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r28,80(r1)
  r28 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // lwz r8,88(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 88);
  // stw r31,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r31));
  // stw r10,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r10));
  // stw r8,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r8));
  // lwz r8,92(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 92);
  // stw r10,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r10));
  // lwz r29,104(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 104);
  // stw r8,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r8));
  // lwz r8,96(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 96);
  // stw r8,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r8));
  // lwz r8,100(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 100);
  // stw r8,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r8));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25012(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25012);
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r31));
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r31,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r31));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25016(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25016);
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // stw r27,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r27));
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
loc_82280E30:
  // addi r5,r29,5
  r5 = uint64_t(int64_t(int64_t(r29) + 5));
  // lwz r6,0(r28)
  r6 = guest::LoadU32(base, uint32_t(r28) + 0);
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82280E44, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // cmpwi cr6,r29,6
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 6, so);
  // blt cr6,0x82280e30
  if (cr6.lt) goto loc_82280E30;
  // li r6,402
  r6 = uint64_t(int64_t(402));
  // li r5,55
  r5 = uint64_t(int64_t(55));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82280E68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,401
  r6 = uint64_t(int64_t(401));
  // li r5,56
  r5 = uint64_t(int64_t(56));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82280E7C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,407
  r6 = uint64_t(int64_t(407));
  // li r5,57
  r5 = uint64_t(int64_t(57));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82280E90, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,408
  r6 = uint64_t(int64_t(408));
  // li r5,58
  r5 = uint64_t(int64_t(58));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82280EA4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r27
  r29 = r27;
  // li r26,27
  r26 = uint64_t(int64_t(27));
loc_82280EAC:
  // mr r5,r29
  r5 = r29;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280EBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r28,r3
  r28 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82280EC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r28
  r3 = r28;
  // stw r31,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82280EDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r28
  r3 = r28;
  // stw r31,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82280EF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r28
  r3 = r28;
  // stw r31,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82280F04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r5,r29
  r5 = r29;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // stw r31,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r31));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82280F1C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r29,5
  r5 = uint64_t(int64_t(int64_t(r29) + 5));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82280F2C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // bne 0x82280eac
  if (!cr0.eq) goto loc_82280EAC;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82280F4C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82280F60, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82280F74, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(base, LIFTED_ARGS(0x82280F88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82280F98, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82280FA8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280FB8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280FCC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82280FE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82280FF4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82281004, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,322
  r5 = uint64_t(int64_t(322));
  // mr r4,r26
  r4 = r26;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82281014, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82281024, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82281038, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,549
  r3 = uint64_t(int64_t(549));
  // stw r27,108(r29)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 44, uint32_t(r27));
  // stw r27,108(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 44, uint32_t(r27));
  // bl 0x82280b48
  // (left out: dead in C++)
  { const lifted::Result o = f82280B48(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r10,112(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 112);
  // lwz r11,25020(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25020);
  // stw r27,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r27));
  // lwz r11,25020(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25020);
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82281068: lifted from generated/ by lift.py.
lifted::Result f82281068(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82281068 (lifted)");
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
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82281098, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8228118c
  if (!cr6.eq) goto loc_8228118C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,109
  r4 = uint64_t(int64_t(109));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822810C4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822810E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x822810E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,101
  r4 = uint64_t(int64_t(101));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82281104, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8228110C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,102
  r4 = uint64_t(int64_t(102));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82281128, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82281130, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,103
  r4 = uint64_t(int64_t(103));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228114C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82281154, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82280798
  // (left out: dead in C++)
  r3 = f82280798(base, r1, r13);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25020(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25020);
  // lwz r3,28(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 28);
  // bl 0x82280bc8
  // (left out: dead in C++)
  r3 = f82280BC8(base, r1, r13, r3);
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x82281184, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_8228118C:
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
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822811B4, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822812c0
  if (cr0.eq) goto loc_822812C0;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x822812c0
  if (cr6.eq) goto loc_822812C0;
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822811E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x822811EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,107
  r4 = uint64_t(int64_t(107));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82281208, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82281210, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,108
  r4 = uint64_t(int64_t(108));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228122C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82281234, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,79
  r4 = uint64_t(int64_t(79));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82281250, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82281258, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,75
  r4 = uint64_t(int64_t(75));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82281274, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x8228127C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82281298, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x822812A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822812B0, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82280840
  // (left out: dead in C++)
  { const lifted::Result o = f82280840(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x822806b8
  // (left out: dead in C++)
  { const lifted::Result o = f822806B8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x822812c4
  goto loc_822812C4;
loc_822812C0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_822812C4:
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

// sub_822812E0: lifted from generated/ by lift.py.
lifted::Result f822812E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822812E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82281310, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8228132c
  if (!cr6.eq) goto loc_8228132C;
  // bl 0x822806b8
  // (left out: dead in C++)
  { const lifted::Result o = f822806B8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8228132C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82281338, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82281358: lifted from generated/ by lift.py.
lifted::Result f82281358(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82281358 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82281388, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822813a4
  if (!cr6.eq) goto loc_822813A4;
  // bl 0x82280798
  // (left out: dead in C++)
  r3 = f82280798(base, r1, r13);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_822813A4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822813B0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822813D0: lifted from generated/ by lift.py.
lifted::Result f822813D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822813D0 (lifted)");
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
  // lwz r31,25020(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 25020);
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82281400, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8228162c
  if (!cr6.eq) goto loc_8228162C;
  // li r4,34
  r4 = uint64_t(int64_t(34));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8228141C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82281428, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,35
  r4 = uint64_t(int64_t(35));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82281434, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82281440, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 32);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // stw r11,36(r31)
  guest::StoreU32(base, uint32_t(r31) + 36, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25016(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25016);
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x8228148c
  if (!cr0.eq) goto loc_8228148C;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // bne cr6,0x8228148c
  if (!cr6.eq) goto loc_8228148C;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r3,200
  r3 = uint64_t(int64_t(200));
  // stw r11,28(r31)
  guest::StoreU32(base, uint32_t(r31) + 28, uint32_t(r11));
  // stw r10,32(r31)
  guest::StoreU32(base, uint32_t(r31) + 32, uint32_t(r10));
  // stw r29,36(r31)
  guest::StoreU32(base, uint32_t(r31) + 36, uint32_t(r29));
  // b 0x822814b4
  goto loc_822814B4;
loc_8228148C:
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // bne cr6,0x822814bc
  if (!cr6.eq) goto loc_822814BC;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x822814bc
  if (!cr6.eq) goto loc_822814BC;
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r29,28(r31)
  guest::StoreU32(base, uint32_t(r31) + 28, uint32_t(r29));
  // stw r29,32(r31)
  guest::StoreU32(base, uint32_t(r31) + 32, uint32_t(r29));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,36(r31)
  guest::StoreU32(base, uint32_t(r31) + 36, uint32_t(r11));
loc_822814B4:
  // bl 0x82280ad8
  // (left out: dead in C++)
  r3 = f82280AD8(base, r1, r13, r3);
  // b 0x822815b0
  goto loc_822815B0;
loc_822814BC:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8228152c
  if (!cr6.eq) goto loc_8228152C;
  // lwz r10,28(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 28);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x822814f4
  if (!cr6.eq) goto loc_822814F4;
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // bne cr6,0x822814f4
  if (!cr6.eq) goto loc_822814F4;
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r8,1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 1, so);
  // bne cr6,0x822814f4
  if (!cr6.eq) goto loc_822814F4;
  // li r4,34
  r4 = uint64_t(int64_t(34));
loc_822814EC:
  // stw r27,28(r31)
  guest::StoreU32(base, uint32_t(r31) + 28, uint32_t(r27));
  // b 0x82281598
  goto loc_82281598;
loc_822814F4:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x8228151c
  if (!cr6.eq) goto loc_8228151C;
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bne cr6,0x8228151c
  if (!cr6.eq) goto loc_8228151C;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8228151c
  if (!cr6.eq) goto loc_8228151C;
  // li r4,34
  r4 = uint64_t(int64_t(34));
  // stw r29,28(r31)
  guest::StoreU32(base, uint32_t(r31) + 28, uint32_t(r29));
  // b 0x82281598
  goto loc_82281598;
loc_8228151C:
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 32);
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x82281594
  goto loc_82281594;
loc_8228152C:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x822815b0
  if (!cr6.eq) goto loc_822815B0;
  // lwz r10,28(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 28);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x8228155c
  if (!cr0.eq) goto loc_8228155C;
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r8,6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 6, so);
  // bne cr6,0x8228155c
  if (!cr6.eq) goto loc_8228155C;
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // bne cr6,0x8228155c
  if (!cr6.eq) goto loc_8228155C;
  // li r4,35
  r4 = uint64_t(int64_t(35));
  // b 0x822814ec
  goto loc_822814EC;
loc_8228155C:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82281588
  if (!cr6.eq) goto loc_82281588;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bne cr6,0x82281588
  if (!cr6.eq) goto loc_82281588;
  // cmpwi cr6,r9,6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 6, so);
  // bne cr6,0x82281588
  if (!cr6.eq) goto loc_82281588;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r4,35
  r4 = uint64_t(int64_t(35));
  // stw r11,28(r31)
  guest::StoreU32(base, uint32_t(r31) + 28, uint32_t(r11));
  // b 0x82281598
  goto loc_82281598;
loc_82281588:
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 32);
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
loc_82281594:
  // stw r11,32(r31)
  guest::StoreU32(base, uint32_t(r31) + 32, uint32_t(r11));
loc_82281598:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822815B0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822815B0:
  // bl 0x82280840
  // (left out: dead in C++)
  { const lifted::Result o = f82280840(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // addi r28,r11,31692
  r28 = uint64_t(int64_t(int64_t(r11) + 31692));
loc_822815BC:
  // lwz r11,28(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28);
  // addi r5,r29,5
  r5 = uint64_t(int64_t(int64_t(r29) + 5));
  // li r4,8
  r4 = uint64_t(int64_t(8));
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
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822815DC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 6, so);
  // blt cr6,0x822815bc
  if (cr6.lt) goto loc_822815BC;
  // lwz r11,36(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 36);
  // mr r3,r30
  r3 = r30;
  // addi r4,r11,12
  r4 = uint64_t(int64_t(int64_t(r11) + 12));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822815F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 32);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r11,12
  r4 = uint64_t(int64_t(int64_t(r11) + 12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82281618, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82281620, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,28(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 28);
  // bl 0x82280bc8
  // (left out: dead in C++)
  r3 = f82280BC8(base, r1, r13, r3);
  // stw r27,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r27));
loc_8228162C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82281638, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82281648: lifted from generated/ by lift.py.
lifted::Result f82281648(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82281648 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,6688
  r6 = uint64_t(int64_t(int64_t(r11) + 6688));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8228166C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // addi r29,r11,31724
  r29 = uint64_t(int64_t(int64_t(r11) + 31724));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r29
  r31 = r29;
  // addi r28,r11,-24512
  r28 = uint64_t(int64_t(int64_t(r11) + -24512));
loc_82281684:
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
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8228169C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r11,r29,32
  r11 = uint64_t(int64_t(int64_t(r29) + 32));
  // addi r30,r30,15
  r30 = uint64_t(int64_t(int64_t(r30) + 15));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x82281684
  if (cr6.lt) goto loc_82281684;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r3,135
  r3 = uint64_t(int64_t(135));
  // lwz r11,25016(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25016);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
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
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822816D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822816F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // lis r10,-32216
  r10 = uint64_t(int64_t(-2111307776));
  // lis r4,-32216
  r4 = uint64_t(int64_t(-2111307776));
  // addi r10,r10,4952
  r10 = uint64_t(int64_t(int64_t(r10) + 4952));
  // lis r5,-32216
  r5 = uint64_t(int64_t(-2111307776));
  // lwz r11,25012(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25012);
  // lis r6,-32216
  r6 = uint64_t(int64_t(-2111307776));
  // lis r7,-32216
  r7 = uint64_t(int64_t(-2111307776));
  // lis r8,-32163
  r8 = uint64_t(int64_t(-2107834368));
  // lis r9,-32216
  r9 = uint64_t(int64_t(-2111307776));
  // stw r10,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r10));
  // addi r4,r4,4200
  r4 = uint64_t(int64_t(int64_t(r4) + 4200));
  // addi r5,r5,1560
  r5 = uint64_t(int64_t(int64_t(r5) + 1560));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // addi r6,r6,5072
  r6 = uint64_t(int64_t(int64_t(r6) + 5072));
  // addi r7,r7,1384
  r7 = uint64_t(int64_t(int64_t(r7) + 1384));
  // addi r8,r8,-31856
  r8 = uint64_t(int64_t(int64_t(r8) + -31856));
  // addi r9,r9,4832
  r9 = uint64_t(int64_t(int64_t(r9) + 4832));
  // stw r4,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r4));
  // stw r5,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r5));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // stw r6,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r6));
  // stw r7,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r7));
  // stw r8,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r8));
  // stw r9,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r9));
  // beq cr6,0x822817e4
  if (cr6.eq) goto loc_822817E4;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x822817c0
  if (cr6.lt) goto loc_822817C0;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8228177C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25012(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25012);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822817f8
  if (cr0.eq) goto loc_822817F8;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x822817a4
  if (!cr6.eq) goto loc_822817A4;
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // b 0x822817b4
  goto loc_822817B4;
loc_822817A4:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_822817A8:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r8,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r8));
  // stw r10,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r10));
loc_822817B4:
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
loc_822817B8:
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
  // b 0x822817f8
  goto loc_822817F8;
loc_822817C0:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x822817a8
  if (!cr6.eq) goto loc_822817A8;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r8,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x822817b8
  goto loc_822817B8;
loc_822817E4:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x822817f8
  if (cr6.eq) goto loc_822817F8;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822817F8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822817F8:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82281800: lifted from generated/ by lift.py.
lifted::Result f82281800(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82281800 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0;
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
  // bl 0x82280cf0
  // (left out: dead in C++)
  { const lifted::Result o = f82280CF0(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25012(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25012);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82281828
  if (cr0.eq) goto loc_82281828;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82281828:
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
  // blr
  return {r3, f1};
}

// sub_822820A8: lifted from generated/ by lift.py.
void f822820A8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822820A8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // lwz r9,104(r11)
  r9 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_104_address());
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x822820c8
  if (!cr6.eq) goto loc_822820C8;
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // b 0x822820e4
  goto loc_822820E4;
loc_822820C8:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r9,25028(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 25028);
  // lwz r9,0(r9)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r9)).page_address());
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // bnelr cr6
  if (!cr6.eq) return;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,6
  r8 = uint64_t(int64_t(6));
loc_822820E4:
  // stw r9,20(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_20_address(), uint32_t(r9));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // stw r8,24(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_24_address(), uint32_t(r8));
  // blr
  return;
}

// sub_822820F8: lifted from generated/ by lift.py.
void f822820F8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822820F8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // lwz r9,104(r11)
  r9 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_104_address());
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x82282124
  if (!cr6.eq) goto loc_82282124;
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // stw r9,20(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_20_address(), uint32_t(r9));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // stw r8,24(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_24_address(), uint32_t(r8));
  // blr
  return;
loc_82282124:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r9,20(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_20_address(), uint32_t(r9));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // stw r9,24(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_24_address(), uint32_t(r9));
  // blr
  return;
}

// sub_82282138: lifted from generated/ by lift.py.
uint64_t f82282138(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82282138 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r6,r11,6752
  r6 = uint64_t(int64_t(int64_t(r11) + 6752));
  // li r4,300
  r4 = uint64_t(int64_t(300));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // b 0x82106540
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82282150: lifted from generated/ by lift.py.
void f82282150(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82282150 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27348(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8228217c
  if (cr6.eq) goto loc_8228217C;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mulli r10,r3,48
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(48))));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // ld r10,16(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 16);
  // b 0x82282180
  goto loc_82282180;
loc_8228217C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_82282180:
  // li r8,1000
  r8 = uint64_t(int64_t(1000));
  // li r9,60
  r9 = uint64_t(int64_t(60));
  // divdu r11,r10,r8
  r11 = r8 ? r10 / r8 : 0;
  // divdu r8,r10,r8
  r8 = r8 ? r10 / r8 : 0;
  // mulli r8,r8,1000
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(1000))));
  // subf r10,r8,r10
  r10 = r10 - r8;
  // stw r10,0(r4)
  guest::StoreU32(base, schema::NetRankingTime(base, uint32_t(r4)).milliseconds_address(), uint32_t(r10));
  // divdu r10,r11,r9
  r10 = r9 ? r11 / r9 : 0;
  // divdu r9,r11,r9
  r9 = r9 ? r11 / r9 : 0;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // mulli r9,r9,60
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(60))));
  // stw r10,8(r4)
  guest::StoreU32(base, schema::NetRankingTime(base, uint32_t(r4)).minutes_address(), uint32_t(r10));
  // subf r11,r9,r11
  r11 = r11 - r9;
  // cmpwi cr6,r10,60
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 60, so);
  // stw r11,4(r4)
  guest::StoreU32(base, schema::NetRankingTime(base, uint32_t(r4)).seconds_address(), uint32_t(r11));
  // bltlr cr6
  if (cr6.lt) return;
  // li r11,59
  r11 = uint64_t(int64_t(59));
  // li r10,999
  r10 = uint64_t(int64_t(999));
  // stw r11,8(r4)
  guest::StoreU32(base, schema::NetRankingTime(base, uint32_t(r4)).minutes_address(), uint32_t(r11));
  // stw r11,4(r4)
  guest::StoreU32(base, schema::NetRankingTime(base, uint32_t(r4)).seconds_address(), uint32_t(r11));
  // stw r10,0(r4)
  guest::StoreU32(base, schema::NetRankingTime(base, uint32_t(r4)).milliseconds_address(), uint32_t(r10));
  // blr
  return;
}

// sub_822821D8: lifted from generated/ by lift.py.
uint64_t f822821D8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_822821D8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r11,27348(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82282208
  if (cr6.eq) goto loc_82282208;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // b 0x8228220c
  goto loc_8228220C;
loc_82282208:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
loc_8228220C:
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,80(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).row_count_address(), uint32_t(r10));
  // lwz r11,27348(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82282230
  if (cr6.eq) goto loc_82282230;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // b 0x82282234
  goto loc_82282234;
loc_82282230:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
loc_82282234:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,76(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).row_limit_address(), uint32_t(r10));
  // bl 0x822914a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822914A8, LIFTED_ARGS(0x82282240, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r3,48(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_48_address(), uint32_t(r3));
  // lwz r11,27348(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82282264
  if (cr6.eq) goto loc_82282264;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // b 0x82282268
  goto loc_82282268;
loc_82282264:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
loc_82282268:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,52(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_52_address(), uint32_t(r10));
  // lwz r11,27348(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82282298
  if (cr6.eq) goto loc_82282298;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mulli r11,r9,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(48))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r11,-40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -40);
  // b 0x8228229c
  goto loc_8228229C;
loc_82282298:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
loc_8228229C:
  // lwz r10,25040(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r11,56(r10)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r10)).unknown_56_address(), uint32_t(r11));
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

// sub_822822C0: lifted from generated/ by lift.py.
uint64_t f822822C0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822822C0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // lwz r10,27348(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 27348);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x822822e4
  if (cr6.eq) goto loc_822822E4;
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 8);
  // b 0x822822e8
  goto loc_822822E8;
loc_822822E4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
loc_822822E8:
  // subf r10,r10,r4
  r10 = r4 - r10;
  // lwz r9,80(r11)
  r9 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).row_count_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // mr r3,r7
  r3 = r7;
  // addi r8,r10,7
  r8 = uint64_t(int64_t(int64_t(r10) + 7));
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // stw r10,68(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).first_row_address(), uint32_t(r10));
  // stw r8,72(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).last_row_address(), uint32_t(r8));
  // bgt cr6,0x8228231c
  if (cr6.gt) goto loc_8228231C;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r9,72(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).last_row_address(), uint32_t(r9));
  // b 0x8228234c
  goto loc_8228234C;
loc_8228231C:
  // lwz r10,76(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).row_limit_address());
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // ble cr6,0x82282338
  if (!cr6.gt) goto loc_82282338;
  // addi r9,r10,-7
  r9 = uint64_t(int64_t(int64_t(r10) + -7));
  // stw r10,72(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).last_row_address(), uint32_t(r10));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r9,68(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).first_row_address(), uint32_t(r9));
loc_82282338:
  // lwz r10,68(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).first_row_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // stw r10,72(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).last_row_address(), uint32_t(r10));
loc_8228234C:
  // stw r7,68(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).first_row_address(), uint32_t(r7));
  // blr
  return r3;
}

// sub_82282630: lifted from generated/ by lift.py.
lifted::Result f82282630(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82282630 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82291520
  // (left out: dead in C++)
  { const lifted::Result o = f82291520(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = guest::LoadU32(base, schema::NetMachine(base, uint32_t(r31)).loaded_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822826cc
  if (cr6.eq) goto loc_822826CC;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_0_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x822826a0
  if (cr6.eq) goto loc_822826A0;
  // beq 0x82282688
  if (cr0.eq) goto loc_82282688;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82282688:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // b 0x822826bc
  goto loc_822826BC;
loc_822826A0:
  // beq 0x822826a8
  if (cr0.eq) goto loc_822826A8;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822826A8:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r8,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r8));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r10));
loc_822826BC:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // stw r9,9604(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).state_address(), uint32_t(r9));
  // b 0x822826d4
  goto loc_822826D4;
loc_822826CC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r31)
  guest::StoreU32(base, schema::NetMachine(base, uint32_t(r31)).state_address(), uint32_t(r11));
loc_822826D4:
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

// sub_822826E8: lifted from generated/ by lift.py.
void f822826E8(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822826E8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // stw r9,12(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_12_address(), uint32_t(r9));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,25040(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // lwz r9,25028(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r9,0(r9)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r9)).page_address());
  // stw r9,16(r10)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r10)).unknown_16_address(), uint32_t(r9));
  // lwz r10,25028(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r10)).page_address());
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82282720
  if (cr6.eq) goto loc_82282720;
  // stw r11,8(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).previous_page_address(), uint32_t(r11));
loc_82282720:
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // stw r6,12(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).step_address(), uint32_t(r6));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r6,16(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).queued_step_address(), uint32_t(r6));
  // stw r8,0(r10)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r10)).page_address(), uint32_t(r8));
  // li r8,0
  r8 = uint64_t(int64_t(0));
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

// sub_82282788: lifted from generated/ by lift.py.
lifted::Result f82282788(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82282788 (lifted)");
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822827B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x822827BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 9616);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822827d8
  if (cr6.eq) goto loc_822827D8;
  // li r3,36
  r3 = uint64_t(int64_t(36));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(base, LIFTED_ARGS(0x822827D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9608(r31)
  guest::StoreU32(base, uint32_t(r31) + 9608, uint32_t(r11));
loc_822827D8:
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

// sub_822827F0: lifted from generated/ by lift.py.
void f822827F0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822827F0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,9
  r9 = uint64_t(int64_t(9));
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
  // stw r9,9604(r3)
  guest::StoreU32(base, uint32_t(r3) + 9604, uint32_t(r9));
  // blr
  return;
}

// sub_822828F8: lifted from generated/ by lift.py.
lifted::Result f822828F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_822828F8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // mr r30,r3
  r30 = r3;
  // lwz r11,28220(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 28220);
  // lwz r10,180(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 180);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r10));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x82295028
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserGetSigninState(base, LIFTED_ARGS(0x8228292C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // beq cr6,0x82282980
  if (cr6.eq) goto loc_82282980;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,25028(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x8228294c
  if (cr6.eq) goto loc_8228294C;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_8228294C:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // stw r9,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r9));
  // stw r11,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r11));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r10));
  // lwz r11,28220(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 28220);
  // stw r10,180(r11)
  guest::StoreU32(base, uint32_t(r11) + 180, uint32_t(r10));
  // b 0x82282b70
  goto loc_82282B70;
loc_82282980:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82282b58
  if (cr6.eq) goto loc_82282B58;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82282ac0
  if (cr6.eq) goto loc_82282AC0;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // beq cr6,0x822829d4
  if (cr6.eq) goto loc_822829D4;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x822829c0
  if (cr6.eq) goto loc_822829C0;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,6796
  r6 = uint64_t(int64_t(int64_t(r11) + 6796));
  // b 0x822829c8
  goto loc_822829C8;
loc_822829C0:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,6788
  r6 = uint64_t(int64_t(int64_t(r11) + 6788));
loc_822829C8:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822829CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // b 0x82282a34
  goto loc_82282A34;
loc_822829D4:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,6780
  r6 = uint64_t(int64_t(int64_t(r11) + 6780));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822829E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // ori r10,r11,52528
  r10 = r11 | 52528;
  // lwz r11,28220(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 28220);
  // lwzx r10,r11,r10
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82282a30
  if (cr6.eq) goto loc_82282A30;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,160(r11)
  guest::StoreU32(base, uint32_t(r11) + 160, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,25028(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,13
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 13, so);
  // beq cr6,0x82282a18
  if (cr6.eq) goto loc_82282A18;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_82282A18:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // stw r9,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r9));
  // stw r11,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r11));
  // b 0x82282b70
  goto loc_82282B70;
loc_82282A30:
  // li r9,4
  r9 = uint64_t(int64_t(4));
loc_82282A34:
  // lwz r10,25040(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r9,20(r10)
  guest::StoreU32(base, uint32_t(r10) + 20, uint32_t(r9));
  // lwz r10,25040(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r11,24(r10)
  guest::StoreU32(base, uint32_t(r10) + 24, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r9,25028(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 25028);
  // lwz r10,0(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82282a60
  if (cr6.eq) goto loc_82282A60;
  // stw r10,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r10));
loc_82282A60:
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // addi r10,r30,204
  r10 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,37
  r8 = uint64_t(int64_t(37));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r7,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r7));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,12(r9)
  guest::StoreU32(base, uint32_t(r9) + 12, uint32_t(r11));
  // stw r11,16(r9)
  guest::StoreU32(base, uint32_t(r9) + 16, uint32_t(r11));
  // lwz r11,600(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 600);
  // lwz r9,604(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r9,r9,r11
  r9 = r9 + r11;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r11,600(r10)
  guest::StoreU32(base, uint32_t(r10) + 600, uint32_t(r11));
  // divwu r5,r9,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r9) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r11,r5,r9
  r11 = r9 - r5;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // stw r7,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r7));
  // stw r6,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r6));
  // b 0x82282b70
  goto loc_82282B70;
loc_82282AC0:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,25028(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82282b24
  if (cr6.eq) goto loc_82282B24;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_82282B24:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // addi r6,r9,6772
  r6 = uint64_t(int64_t(int64_t(r9) + 6772));
  // stw r8,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r8));
  // stw r11,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r11));
  // lwz r10,25040(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r7,20(r10)
  guest::StoreU32(base, uint32_t(r10) + 20, uint32_t(r7));
  // lwz r10,25040(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r11,24(r10)
  guest::StoreU32(base, uint32_t(r10) + 24, uint32_t(r11));
  // b 0x82282b60
  goto loc_82282B60;
loc_82282B58:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,6764
  r6 = uint64_t(int64_t(int64_t(r11) + 6764));
loc_82282B60:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82282B70, r31)); r3 = o.r3; f1 = o.f1; }
loc_82282B70:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82282B78: lifted from generated/ by lift.py.
void f82282B78(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82282B78 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,24
  r9 = uint64_t(int64_t(24));
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
  return;
}

// sub_82282BC8: lifted from generated/ by lift.py.
lifted::Result f82282BC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82282BC8 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // mr r28,r4
  r28 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r31,r6
  r31 = r6;
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x82295028
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserGetSigninState(base, LIFTED_ARGS(0x82282BF0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // beq cr6,0x82282c28
  if (cr6.eq) goto loc_82282C28;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82282c10
  if (cr6.eq) goto loc_82282C10;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82282C10:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // b 0x82282cf0
  goto loc_82282CF0;
loc_82282C28:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq cr6,0x82282c8c
  if (cr6.eq) goto loc_82282C8C;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // mr r8,r30
  r8 = r30;
  // mr r7,r30
  r7 = r30;
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_bits();
  // lwz r6,604(r11)
  r6 = schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(base, uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
loc_82282C8C:
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x822927b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822927B0, LIFTED_ARGS(0x82282C94, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,28220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28220);
  // stw r30,180(r11)
  guest::StoreU32(base, schema::Session(base, uint32_t(r11)).read_state_address(), uint32_t(r30));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // lwz r4,8(r11)
  r4 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_8_address());
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25032);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // bl 0x8227f9d8
  // (left out: dead in C++)
  f8227F9D8(base, r3, r4);
  // lwz r11,28220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28220);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // stw r10,160(r11)
  guest::StoreU32(base, schema::Session(base, uint32_t(r11)).unknown_160_address(), uint32_t(r10));
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // ori r10,r11,52528
  r10 = r11 | 52528;
  // lwz r11,28220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28220);
  // beq cr6,0x82282ce4
  if (cr6.eq) goto loc_82282CE4;
  // stwx r28,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r28));
  // b 0x82282cf0
  goto loc_82282CF0;
loc_82282CE4:
  // stwx r30,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r30));
  // lwz r11,28220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28220);
  // stw r27,192(r11)
  guest::StoreU32(base, schema::Session(base, uint32_t(r11)).unknown_192_address(), uint32_t(r27));
loc_82282CF0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82282CF8: lifted from generated/ by lift.py.
uint64_t f82282CF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82282CF8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
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
  // lwz r11,24704(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24704);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // beq cr6,0x82282dd4
  if (cr6.eq) goto loc_82282DD4;
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,28220(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // bl 0x82292338
  // (left out: dead in C++)
  r3 = f82292338(base, r1, r13, r3);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // beq 0x82282da0
  if (cr0.eq) goto loc_82282DA0;
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r9,104(r11)
  guest::StoreU32(base, uint32_t(r11) + 104, uint32_t(r9));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82282dc4
  if (cr6.eq) goto loc_82282DC4;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // b 0x82282dc4
  goto loc_82282DC4;
loc_82282DA0:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r10,104(r11)
  guest::StoreU32(base, uint32_t(r11) + 104, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82282dc0
  if (cr6.eq) goto loc_82282DC0;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82282DC0:
  // li r9,2
  r9 = uint64_t(int64_t(2));
loc_82282DC4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
loc_82282DD4:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82282DE8: lifted from generated/ by lift.py.
uint64_t f82282DE8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82282DE8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82282E0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282E20, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82282f08
  if (cr0.eq) goto loc_82282F08;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,80(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 16, uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282E40, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282E54, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82282f08
  if (cr6.eq) goto loc_82282F08;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82282f08
  if (cr6.eq) goto loc_82282F08;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r28,108(r30)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r30)).draw_context_address() + 44, uint32_t(r28));
  // stw r28,108(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 44, uint32_t(r28));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282E84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82282f08
  if (cr0.eq) goto loc_82282F08;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // stw r29,80(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 16, uint32_t(r29));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282EB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82282f08
  if (cr6.eq) goto loc_82282F08;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82282f08
  if (cr6.eq) goto loc_82282F08;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // stw r29,108(r30)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r30)).draw_context_address() + 44, uint32_t(r29));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r29,108(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 44, uint32_t(r29));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82282f08
  if (cr0.eq) goto loc_82282F08;
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // stw r28,80(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 16, uint32_t(r28));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282EFC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82282f08
  if (cr0.eq) goto loc_82282F08;
  // stw r28,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r28));
loc_82282F08:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82282F10: lifted from generated/ by lift.py.
uint64_t f82282F10(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82282F10 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82282F34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r28,0(r3)
  r28 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r31,r1,96
  r31 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r30,r29
  r30 = r29;
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r29,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r29));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stw r11,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r11));
loc_82282F60:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r5,0(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r3,r28
  r3 = r28;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282F70, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82282f8c
  if (cr0.eq) goto loc_82282F8C;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r29,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r29));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplwi cr6,r30,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 4, so);
  // blt cr6,0x82282f60
  if (cr6.lt) goto loc_82282F60;
loc_82282F8C:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82282F98: lifted from generated/ by lift.py.
uint64_t f82282F98(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82282F98 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82282FC4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x82282f10
  // (left out: dead in C++)
  r3 = f82282F10(base, r1, r13);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282FDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8228302c
  if (cr0.eq) goto loc_8228302C;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r10,80(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 16, uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82282FFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82283010, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8228302c
  if (cr6.eq) goto loc_8228302C;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8228302c
  if (cr6.eq) goto loc_8228302C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,108(r30)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r30)).draw_context_address() + 44, uint32_t(r11));
  // stw r11,108(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 44, uint32_t(r11));
loc_8228302C:
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

// sub_82283048: lifted from generated/ by lift.py.
uint64_t f82283048(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82283048 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8228306C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // bl 0x82282f10
  // (left out: dead in C++)
  r3 = f82282F10(base, r1, r13);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82283084, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822830d0
  if (cr0.eq) goto loc_822830D0;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,80(r11)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r11)).draw_context_address() + 16, uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822830A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822830B8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x822830d0
  if (cr6.eq) goto loc_822830D0;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x822830d0
  if (cr6.eq) goto loc_822830D0;
  // stw r29,108(r30)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r30)).draw_context_address() + 44, uint32_t(r29));
  // stw r29,108(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 44, uint32_t(r29));
loc_822830D0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_822830D8: lifted from generated/ by lift.py.
lifted::Result f822830D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822830D8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822830FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // mr r3,r31
  r3 = r31;
  // stw r31,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r31));
  // bl 0x8211aba8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ResetPose(base, LIFTED_ARGS(0x8228310C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(base, LIFTED_ARGS(0x82283114, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // stw r10,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r10));
  // stw r10,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r10));
  // stw r10,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r10));
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stw r9,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r9));
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // stw r10,76(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).row_limit_address(), uint32_t(r10));
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // stw r9,48(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_48_address(), uint32_t(r9));
  // stw r31,96(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_96_address(), uint32_t(r31));
  // stw r31,92(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_92_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_32_address(), uint32_t(r31));
  // stw r10,52(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_52_address(), uint32_t(r10));
  // stw r9,104(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_104_address(), uint32_t(r9));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_36_address(), uint32_t(r31));
  // stw r31,12(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_12_address(), uint32_t(r31));
  // stw r27,40(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_40_address(), uint32_t(r27));
  // stw r31,44(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_44_address(), uint32_t(r31));
  // stw r9,80(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).row_count_address(), uint32_t(r9));
  // stw r31,16(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_16_address(), uint32_t(r31));
  // lwz r10,84(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 84);
  // stw r10,64(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_64_address(), uint32_t(r10));
  // lwz r10,88(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 88);
  // stw r10,60(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_60_address(), uint32_t(r10));
  // rotlwi r10,r9,0
  r10 = __builtin_rotateleft32(uint32_t(r9), 0);
  // stw r10,24(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_24_address(), uint32_t(r10));
  // rotlwi r10,r9,0
  r10 = __builtin_rotateleft32(uint32_t(r9), 0);
  // stw r10,20(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_20_address(), uint32_t(r10));
  // lwz r10,100(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 100);
  // stw r10,28(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_28_address(), uint32_t(r10));
  // lwz r10,104(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 104);
  // stw r10,100(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_100_address(), uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // stw r27,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r27));
  // stw r31,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r31));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25032);
  // stw r10,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r10));
  // lis r10,-32157
  r10 = uint64_t(int64_t(-2107441152));
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
  // lwz r30,108(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 108);
  // addi r28,r10,31768
  r28 = uint64_t(int64_t(int64_t(r10) + 31768));
  // stw r31,36(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).side_address(), uint32_t(r31));
  // mr r29,r31
  r29 = r31;
  // stw r31,64(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).player_address(), uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).up_down_locked_address(), uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).left_right_steps_address(), uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).shoulders_step_address(), uint32_t(r31));
loc_82283248:
  // addi r5,r29,5
  r5 = uint64_t(int64_t(int64_t(r29) + 5));
  // lwz r6,0(r28)
  r6 = guest::LoadU32(base, uint32_t(r28) + 0);
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8228325C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // cmpwi cr6,r29,6
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 6, so);
  // blt cr6,0x82283248
  if (cr6.lt) goto loc_82283248;
  // mr r29,r27
  r29 = r27;
  // li r26,27
  r26 = uint64_t(int64_t(27));
loc_82283274:
  // mr r5,r29
  r5 = r29;
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82283284, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r28,r3
  r28 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283290, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r28
  r3 = r28;
  // stw r31,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822832A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r28
  r3 = r28;
  // stw r31,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822832B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r28
  r3 = r28;
  // stw r31,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822832CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r5,r29
  r5 = r29;
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // stw r31,80(r11)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r11)).visible_address(), uint32_t(r31));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822832E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r29,5
  r5 = uint64_t(int64_t(int64_t(r29) + 5));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822832F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // stw r31,80(r3)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r3)).draw_context_address() + 16, uint32_t(r31));
  // bne 0x82283274
  if (!cr0.eq) goto loc_82283274;
  // li r6,402
  r6 = uint64_t(int64_t(402));
  // li r5,55
  r5 = uint64_t(int64_t(55));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82283318, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,401
  r6 = uint64_t(int64_t(401));
  // li r5,56
  r5 = uint64_t(int64_t(56));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8228332C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,407
  r6 = uint64_t(int64_t(407));
  // li r5,57
  r5 = uint64_t(int64_t(57));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82283340, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,408
  r6 = uint64_t(int64_t(408));
  // li r5,58
  r5 = uint64_t(int64_t(58));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x82283354, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82282de8
  // (left out: dead in C++)
  r3 = f82282DE8(base, r1, r13);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82283368, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8228337C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82283390, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822833A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r26
  r4 = r26;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822833B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,322
  r5 = uint64_t(int64_t(322));
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822833C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,97
  r4 = uint64_t(int64_t(97));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822833D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,98
  r4 = uint64_t(int64_t(98));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822833DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r29,112(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 112);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // mr r3,r30
  r3 = r30;
  // li r4,97
  r4 = uint64_t(int64_t(97));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8228340c
  if (!cr6.eq) goto loc_8228340C;
  // li r4,98
  r4 = uint64_t(int64_t(98));
loc_8228340C:
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82283410, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82283418, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,965
  r6 = uint64_t(int64_t(965));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x8228342C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x82283440, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82283450, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // stw r27,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r27));
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82283468: lifted from generated/ by lift.py.
lifted::Result f82283468(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82283468 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x8228348C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // mr r11,r28
  r11 = r28;
loc_82283498:
  // addi r29,r11,1
  r29 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // mr r5,r29
  r5 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822834AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822834B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,531
  r5 = uint64_t(int64_t(531));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822834C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822834D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,534
  r5 = uint64_t(int64_t(534));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822834E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822834F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,534
  r5 = uint64_t(int64_t(534));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283500, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8228350C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,534
  r5 = uint64_t(int64_t(534));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8228351C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283528, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,533
  r5 = uint64_t(int64_t(533));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283538, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283544, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8228354C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r29
  r11 = r29;
  // stw r28,108(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).texture_entry_address(), uint32_t(r28));
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // blt cr6,0x82283498
  if (cr6.lt) goto loc_82283498;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_44_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82283624
  if (!cr6.eq) goto loc_82283624;
  // li r5,31
  r5 = uint64_t(int64_t(31));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82283580, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8228358C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,531
  r5 = uint64_t(int64_t(531));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8228359C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822835A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,534
  r5 = uint64_t(int64_t(534));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822835B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822835C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,534
  r5 = uint64_t(int64_t(534));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822835D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822835E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,534
  r5 = uint64_t(int64_t(534));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822835F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822835FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,533
  r5 = uint64_t(int64_t(533));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x8228360C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283618, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283620, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,108(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).texture_entry_address(), uint32_t(r28));
loc_82283624:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82283630: lifted from generated/ by lift.py.
lifted::Result f82283630(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82283630 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82283654, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r25,-32148
  r25 = uint64_t(int64_t(-2106851328));
  // lwz r29,0(r3)
  r29 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // lwz r11,27348(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82283678
  if (cr6.eq) goto loc_82283678;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // b 0x8228367c
  goto loc_8228367C;
loc_82283678:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
loc_8228367C:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r8,r9,-1
  r8 = uint64_t(int64_t(int64_t(r9) + -1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r29
  r3 = r29;
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // lwz r10,68(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).first_row_address());
  // lwz r9,72(r11)
  r9 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).last_row_address());
  // lwz r7,76(r11)
  r7 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).row_limit_address());
  // mr r26,r10
  r26 = r10;
  // subf r21,r10,r9
  r21 = r9 - r10;
  // cmpwi cr6,r7,7
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 7, so);
  // bge cr6,0x822836c0
  if (!cr6.lt) goto loc_822836C0;
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822836B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r29
  r3 = r29;
  // b 0x822836e0
  goto loc_822836E0;
loc_822836C0:
  // add. r10,r10,r8
  r10 = r10 + r8;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x822836d0
  if (!cr0.eq) goto loc_822836D0;
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // b 0x822836e4
  goto loc_822836E4;
loc_822836D0:
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // add r10,r9,r8
  r10 = r9 + r8;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x822836ec
  if (!cr6.eq) goto loc_822836EC;
loc_822836E0:
  // li r5,34
  r5 = uint64_t(int64_t(34));
loc_822836E4:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822836E8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82283704
  goto loc_82283704;
loc_822836EC:
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822836F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,34
  r5 = uint64_t(int64_t(34));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82283704, r31)); r3 = o.r3; f1 = o.f1; }
loc_82283704:
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // cmplwi cr6,r21,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), 0, so);
  // mr r11,r24
  r11 = r24;
  // beq cr6,0x822838b0
  if (cr6.eq) goto loc_822838B0;
  // mulli r28,r26,48
  r28 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(48))));
loc_82283718:
  // addi r22,r11,1
  r22 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r29
  r3 = r29;
  // mr r5,r22
  r5 = r22;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8228372C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // stw r11,80(r31)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r31)).draw_context_address() + 16, uint32_t(r11));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283740, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 27348);
  // mr r27,r3
  r27 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82283768
  if (cr6.eq) goto loc_82283768;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r28
  r11 = r11 + r28;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // lwz r3,40(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 40);
  // b 0x8228376c
  goto loc_8228376C;
loc_82283768:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_8228376C:
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x82283770, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 27348);
  // mr r23,r3
  r23 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82283798
  if (cr6.eq) goto loc_82283798;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r28
  r11 = r11 + r28;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // lwz r30,64(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 64);
  // b 0x8228379c
  goto loc_8228379C;
loc_82283798:
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
loc_8228379C:
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r24,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r24));
  // mr r3,r26
  r3 = r26;
  // stw r24,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r24));
  // stw r24,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r24));
  // bl 0x82282150
  // (left out: dead in C++)
  f82282150(base, r3, r4);
  // li r5,528
  r5 = uint64_t(int64_t(528));
  // mr r4,r27
  r4 = r27;
  // lwz r8,88(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 88);
  // mr r3,r29
  r3 = r29;
  // lwz r7,92(r1)
  r7 = guest::LoadU32(base, uint32_t(r1) + 92);
  // lwz r6,96(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 96);
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822837D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822837DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r30,1646
  r5 = uint64_t(int64_t(int64_t(r30) + 1646));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822837EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822837F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r30,1718
  r5 = uint64_t(int64_t(int64_t(r30) + 1718));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283808, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283814, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 27348);
  // mr r4,r3
  r4 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82283838
  if (cr6.eq) goto loc_82283838;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r28
  r11 = r11 + r28;
  // addi r5,r11,24
  r5 = uint64_t(int64_t(int64_t(r11) + 24));
  // b 0x8228383c
  goto loc_8228383C;
loc_82283838:
  // mr r5,r24
  r5 = r24;
loc_8228383C:
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c8a0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeCharText(base, LIFTED_ARGS(0x82283844, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283850, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 27348);
  // mr r4,r3
  r4 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82283874
  if (cr6.eq) goto loc_82283874;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r28
  r11 = r11 + r28;
  // lwz r6,8(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 8);
  // b 0x82283878
  goto loc_82283878;
loc_82283874:
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
loc_82283878:
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283884, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283890, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283898, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r22
  r11 = r22;
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // stw r23,108(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).texture_entry_address(), uint32_t(r23));
  // addi r28,r28,48
  r28 = uint64_t(int64_t(int64_t(r28) + 48));
  // cmplw cr6,r11,r21
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r21), so);
  // blt cr6,0x82283718
  if (cr6.lt) goto loc_82283718;
loc_822838B0:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822838B8: lifted from generated/ by lift.py.
lifted::Result f822838B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822838B8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822838DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r25,-32127
  r25 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,31
  r5 = uint64_t(int64_t(31));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,25040(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 25040);
  // lwz r10,48(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_48_address());
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_52_address());
  // subf r11,r11,r10
  r11 = r10 - r11;
  // addi r28,r11,1
  r28 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82283908, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r5,31
  r5 = uint64_t(int64_t(31));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8228391C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r23,1
  r23 = uint64_t(int64_t(1));
  // mr r30,r3
  r30 = r3;
  // stw r23,80(r29)
  guest::StoreU32(base, schema::UiMovie(base, uint32_t(r29)).draw_context_address() + 16, uint32_t(r23));
  // lwz r11,25040(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 25040);
  // lwz r10,44(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_44_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82283b00
  if (!cr6.eq) goto loc_82283B00;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_48_address());
  // lis r22,-32127
  r22 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82283a44
  if (cr6.eq) goto loc_82283A44;
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // lwz r11,27348(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82283974
  if (cr6.eq) goto loc_82283974;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mulli r10,r28,48
  r10 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(48))));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // lwz r3,40(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 40);
  // b 0x82283978
  goto loc_82283978;
loc_82283974:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_82283978:
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x8228397C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 27348);
  // mr r24,r3
  r24 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822839a8
  if (cr6.eq) goto loc_822839A8;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mulli r10,r28,48
  r10 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(48))));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // lwz r29,64(r11)
  r29 = guest::LoadU32(base, uint32_t(r11) + 64);
  // b 0x822839ac
  goto loc_822839AC;
loc_822839A8:
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
loc_822839AC:
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822839B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r27,r3
  r27 = r3;
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r3,r28
  r3 = r28;
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // bl 0x82282150
  // (left out: dead in C++)
  f82282150(base, r3, r4);
  // li r5,528
  r5 = uint64_t(int64_t(528));
  // lwz r8,88(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 88);
  // mr r4,r27
  r4 = r27;
  // lwz r7,92(r1)
  r7 = guest::LoadU32(base, uint32_t(r1) + 92);
  // mr r3,r31
  r3 = r31;
  // lwz r6,96(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 96);
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x822839F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283A00, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27348(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 27348);
  // mr r4,r3
  r4 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82283a28
  if (cr6.eq) goto loc_82283A28;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mulli r10,r28,48
  r10 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(48))));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r6,8(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 8);
  // b 0x82283a2c
  goto loc_82283A2C;
loc_82283A28:
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
loc_82283A2C:
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283A38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 25040);
  // stw r23,44(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_44_address(), uint32_t(r23));
  // b 0x82283a90
  goto loc_82283A90;
loc_82283A44:
  // bl 0x82291478
  // (left out: dead in C++)
  r3 = f82291478(base);
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8225FFC8, LIFTED_ARGS(0x82283A4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28220(r22)
  r11 = guest::LoadU32(base, uint32_t(r22) + 28220);
  // mr r24,r3
  r24 = r3;
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // lwz r29,216(r11)
  r29 = guest::LoadU32(base, schema::Session(base, uint32_t(r11)).ta_car_word_address());
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283A64, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,531
  r5 = uint64_t(int64_t(531));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283A74, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283A80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,533
  r5 = uint64_t(int64_t(533));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283A90, r31)); r3 = o.r3; f1 = o.f1; }
loc_82283A90:
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283A9C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r29,1646
  r5 = uint64_t(int64_t(int64_t(r29) + 1646));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283AAC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283AB8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r29,1718
  r5 = uint64_t(int64_t(int64_t(r29) + 1718));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(base, LIFTED_ARGS(0x82283AC8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283AD4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28220(r22)
  r11 = guest::LoadU32(base, uint32_t(r22) + 28220);
  // mr r4,r3
  r4 = r3;
  // addi r5,r11,248
  r5 = uint64_t(int64_t(int64_t(r11) + 248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c8a0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeCharText(base, LIFTED_ARGS(0x82283AE8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283AF4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82283AFC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r24,108(r3)
  guest::StoreU32(base, schema::UiMovieNode(base, uint32_t(r3)).texture_entry_address(), uint32_t(r24));
loc_82283B00:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82283B08: lifted from generated/ by lift.py.
lifted::Result f82283B08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82283B08 (lifted)");
  uint64_t r0 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82283B2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r28,0(r3)
  r28 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,48(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_48_address());
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x82283b50
  if (cr6.eq) goto loc_82283B50;
  // stw r6,44(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_44_address(), uint32_t(r6));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
loc_82283B50:
  // lwz r10,76(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).row_limit_address());
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // bge cr6,0x82283b64
  if (!cr6.lt) goto loc_82283B64;
  // mr r9,r6
  r9 = r6;
  // b 0x82283b6c
  goto loc_82283B6C;
loc_82283B64:
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // li r9,1
  r9 = uint64_t(int64_t(1));
loc_82283B6C:
  // stw r10,84(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_84_address(), uint32_t(r10));
  // lwz r3,25040(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r4,80(r3)
  r4 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r3)).row_count_address());
  // lwz r11,32(r3)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r3)).unknown_32_address());
  // addi r30,r4,-1
  r30 = uint64_t(int64_t(int64_t(r4) + -1));
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bgt cr6,0x82284028
  if (cr6.gt) goto loc_82284028;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6736
  r12 = uint64_t(int64_t(int64_t(r12) + 6736));
  // rlwinm r0,r11,1,0,30
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // lhzx r0,r12,r0
  r0 = guest::LoadU16(base, uint32_t(r12) + uint32_t(r0));
  // lis r12,-32216
  r12 = uint64_t(int64_t(-2111307776));
  // addi r12,r12,15280
  r12 = uint64_t(int64_t(int64_t(r12) + 15280));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_82283BB0;
  return {r3, f1};
  case 1:
  goto loc_82283D5C;
  case 2:
  goto loc_82283D88;
  case 3:
  goto loc_82283D9C;
  case 4:
  goto loc_82283DD0;
  case 5:
  goto loc_82283DE8;
  case 6:
  goto loc_82283E64;
  case 7:
  goto loc_82283EF8;
  default:
  std::abort();
  }
loc_82283BD0:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // stw r6,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r6));
  // stw r6,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r6));
  // b 0x82283ff4
  goto loc_82283FF4;
loc_82283D5C:
  // lis r5,-32127
  r5 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 25036);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 0);
loc_82283D68:
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r6,60(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_60_address(), uint32_t(r6));
  // stw r6,64(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_64_address(), uint32_t(r6));
  // lwz r11,25036(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 25036);
  // stw r4,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r4));
  // stw r4,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r4));
  // b 0x82283ff4
  goto loc_82283FF4;
loc_82283D88:
  // lis r5,-32127
  r5 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 25036);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,-7
  r4 = uint64_t(int64_t(int64_t(r11) + -7));
  // b 0x82283d68
  goto loc_82283D68;
loc_82283D9C:
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25036);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r5,r11,7
  r5 = uint64_t(int64_t(int64_t(r11) + 7));
  // cmplw cr6,r5,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r30), so);
  // ble cr6,0x82283dbc
  if (!cr6.gt) goto loc_82283DBC;
  // mr r5,r30
  r5 = r30;
  // b 0x82283f70
  goto loc_82283F70;
loc_82283DBC:
  // beq cr6,0x82283f70
  if (cr6.eq) goto loc_82283F70;
  // lwz r11,60(r3)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r3)).unknown_60_address());
  // subf r4,r11,r5
  r4 = r5 - r11;
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // b 0x82283fe8
  goto loc_82283FE8;
loc_82283DD0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r6,60(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_60_address(), uint32_t(r6));
  // stw r6,64(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_64_address(), uint32_t(r6));
  // b 0x82283bd0
  goto loc_82283BD0;
loc_82283DE8:
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,72(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).last_row_address());
  // lwz r9,68(r11)
  r9 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).first_row_address());
  // lwz r8,80(r11)
  r8 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).row_count_address());
  // subf r10,r9,r10
  r10 = r10 - r9;
  // cmpwi cr6,r8,7
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 7, so);
  // bgt cr6,0x82283e0c
  if (cr6.gt) goto loc_82283E0C;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
loc_82283E0C:
  // mr r9,r10
  r9 = r10;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x82283e20
  if (!cr6.lt) goto loc_82283E20;
  // mr r9,r6
  r9 = r6;
  // b 0x82283e2c
  goto loc_82283E2C;
loc_82283E20:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // ble cr6,0x82283e2c
  if (!cr6.gt) goto loc_82283E2C;
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_82283E2C:
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x82283e3c
  if (!cr6.lt) goto loc_82283E3C;
  // mr r10,r6
  r10 = r6;
  // b 0x82283e48
  goto loc_82283E48;
loc_82283E3C:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // ble cr6,0x82283e48
  if (!cr6.gt) goto loc_82283E48;
  // li r10,6
  r10 = uint64_t(int64_t(6));
loc_82283E48:
  // stw r9,60(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_60_address(), uint32_t(r9));
  // stw r10,64(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_64_address(), uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // b 0x82283ff4
  goto loc_82283FF4;
loc_82283E64:
  // lis r5,-32127
  r5 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 25036);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addic. r4,r11,-100
  ca = uint32_t(r11) > 99;
  r4 = uint64_t(int64_t(int64_t(r11) + -100));
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bge 0x82283e7c
  if (!cr0.lt) goto loc_82283E7C;
  // mr r4,r6
  r4 = r6;
loc_82283E7C:
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lwz r11,25036(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 25036);
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // cmplwi cr6,r30,100
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 100, so);
  // stw r6,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r6));
  // stw r30,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r30));
  // stw r6,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r6));
  // stw r4,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r4));
  // stw r4,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r4));
  // stw r6,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r6));
  // stw r6,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r6));
  // stw r6,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r6));
  // stw r10,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r10));
  // stw r6,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r6));
  // stw r6,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r6));
  // stw r6,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r6));
  // stw r6,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r6));
  // lwz r3,25040(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // bge cr6,0x82283ee0
  if (!cr6.lt) goto loc_82283EE0;
loc_82283EC8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r6,60(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_60_address(), uint32_t(r6));
  // stw r6,64(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_64_address(), uint32_t(r6));
  // b 0x82284024
  goto loc_82284024;
loc_82283EE0:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82283ec8
  if (cr6.eq) goto loc_82283EC8;
  // lwz r11,60(r3)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r3)).unknown_60_address());
  // subf r4,r11,r4
  r4 = r4 - r11;
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // b 0x82284024
  goto loc_82284024;
loc_82283EF8:
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r5,r30
  r5 = r30;
  // lwz r11,25036(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25036);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,100
  r11 = uint64_t(int64_t(int64_t(r11) + 100));
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bgt cr6,0x82283f18
  if (cr6.gt) goto loc_82283F18;
  // mr r5,r11
  r5 = r11;
loc_82283F18:
  // cmplwi cr6,r30,100
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 100, so);
  // bge cr6,0x82283f68
  if (!cr6.lt) goto loc_82283F68;
  // mr r4,r5
  r4 = r5;
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // cmplwi cr6,r30,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 7, so);
  // bge cr6,0x82283f78
  if (!cr6.lt) goto loc_82283F78;
  // lwz r9,25040(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // mr r11,r30
  r11 = r30;
  // mr r10,r30
  r10 = r30;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x82283f4c
  if (!cr6.lt) goto loc_82283F4C;
  // mr r10,r6
  r10 = r6;
  // b 0x82283f58
  goto loc_82283F58;
loc_82283F4C:
  // cmpwi cr6,r30,6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 6, so);
  // ble cr6,0x82283f58
  if (!cr6.gt) goto loc_82283F58;
  // li r10,6
  r10 = uint64_t(int64_t(6));
loc_82283F58:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82283fcc
  if (cr6.lt) goto loc_82283FCC;
  // cmpwi cr6,r30,6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 6, so);
  // b 0x82283fd8
  goto loc_82283FD8;
loc_82283F68:
  // cmplw cr6,r5,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r30), so);
  // bne cr6,0x82283f8c
  if (!cr6.eq) goto loc_82283F8C;
loc_82283F70:
  // mr r4,r5
  r4 = r5;
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
loc_82283F78:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stw r10,60(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_60_address(), uint32_t(r10));
  // stw r10,64(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_64_address(), uint32_t(r10));
  // b 0x82283fe8
  goto loc_82283FE8;
loc_82283F8C:
  // lwz r11,60(r3)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r3)).unknown_60_address());
  // subf r4,r11,r5
  r4 = r5 - r11;
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // subf r11,r5,r30
  r11 = r30 - r5;
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // bge cr6,0x82283fe8
  if (!cr6.lt) goto loc_82283FE8;
  // lwz r9,25040(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // mr. r10,r11
  r10 = r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge 0x82283fb8
  if (!cr0.lt) goto loc_82283FB8;
  // mr r10,r6
  r10 = r6;
  // b 0x82283fc4
  goto loc_82283FC4;
loc_82283FB8:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // ble cr6,0x82283fc4
  if (!cr6.gt) goto loc_82283FC4;
  // li r10,6
  r10 = uint64_t(int64_t(6));
loc_82283FC4:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x82283fd4
  if (!cr6.lt) goto loc_82283FD4;
loc_82283FCC:
  // mr r11,r6
  r11 = r6;
  // b 0x82283fe0
  goto loc_82283FE0;
loc_82283FD4:
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
loc_82283FD8:
  // ble cr6,0x82283fe0
  if (!cr6.gt) goto loc_82283FE0;
  // li r11,6
  r11 = uint64_t(int64_t(6));
loc_82283FE0:
  // stw r11,64(r9)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r9)).unknown_64_address(), uint32_t(r11));
  // stw r10,60(r9)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r9)).unknown_60_address(), uint32_t(r10));
loc_82283FE8:
  // lwz r11,25036(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25036);
  // stw r5,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r5));
  // stw r5,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r5));
loc_82283FF4:
  // stw r6,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r6));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stw r30,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r30));
  // stw r6,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r6));
  // stw r6,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r6));
  // stw r6,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r6));
  // stw r6,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r6));
  // stw r10,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r10));
  // stw r6,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r6));
  // stw r6,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r6));
  // stw r6,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r6));
  // stw r6,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r6));
loc_82284024:
  // lwz r3,25040(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25040);
loc_82284028:
  // mr r30,r6
  r30 = r6;
  // stw r6,32(r3)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r3)).unknown_32_address(), uint32_t(r6));
loc_82284030:
  // addi r4,r30,119
  r4 = uint64_t(int64_t(int64_t(r30) + 119));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8228403C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 7, so);
  // blt cr6,0x82284030
  if (cr6.lt) goto loc_82284030;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_60_address());
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228406C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284074, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,29
  r5 = uint64_t(int64_t(29));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r28
  r3 = r28;
  // lwz r11,25032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25032);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r11)).cursor_address());
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // addi r11,r11,31768
  r11 = uint64_t(int64_t(int64_t(r11) + 31768));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822840A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r5,30
  r5 = uint64_t(int64_t(30));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r28
  r3 = r28;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_8_address());
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r6,r11,1
  r6 = r11 ^ 1;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822840C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,966
  r6 = uint64_t(int64_t(966));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822840DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x822840F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82284100, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_frag_82283BB0:
  // lwz r11,48(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 48);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82283be4
  if (!cr6.eq) goto loc_82283BE4_82283BB0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r6,60(r3)
  guest::StoreU32(base, uint32_t(r3) + 60, uint32_t(r6));
  // stw r6,64(r3)
  guest::StoreU32(base, uint32_t(r3) + 64, uint32_t(r6));
  // lwz r3,25040(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // stw r6,0(r11)
  guest::StoreU32(base, schema::UiMovieTableValue(base, uint32_t(r11)).value_address(), uint32_t(r6));
  // stw r6,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r6));
  // b 0x82283ff4
  goto loc_82283FF4_82283BB0;
loc_82283BE4_82283BB0:
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x82283ccc
  if (cr6.eq) goto loc_82283CCC_82283BB0;
  // addic. r4,r11,-4
  ca = uint32_t(r11) > 3;
  r4 = uint64_t(int64_t(int64_t(r11) + -4));
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bge 0x82283bf8
  if (!cr0.lt) goto loc_82283BF8_82283BB0;
  // mr r4,r6
  r4 = r6;
loc_82283BF8_82283BB0:
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lwz r9,25040(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82283c4c
  if (cr6.eq) goto loc_82283C4C_82283BB0;
  // lwz r11,68(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 68);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82283c4c
  if (cr6.eq) goto loc_82283C4C_82283BB0;
  // lwz r11,48(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 48);
  // subf r10,r11,r30
  r10 = r30 - r11;
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // bge cr6,0x82283c3c
  if (!cr6.lt) goto loc_82283C3C_82283BB0;
  // subf r11,r30,r11
  r11 = r11 - r30;
  // addic. r11,r11,6
  ca = uint32_t(r11) > 4294967289;
  r11 = uint64_t(int64_t(int64_t(r11) + 6));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r10,r11
  r10 = r11;
  // blt 0x82283c58
  if (cr0.lt) goto loc_82283C58_82283BB0;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // b 0x82283c64
  goto loc_82283C64_82283BB0;
loc_82283C3C_82283BB0:
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,60(r9)
  guest::StoreU32(base, uint32_t(r9) + 60, uint32_t(r11));
  // stw r11,64(r9)
  guest::StoreU32(base, uint32_t(r9) + 64, uint32_t(r11));
  // b 0x82283c90
  goto loc_82283C90_82283BB0;
loc_82283C4C_82283BB0:
  // lwz r11,48(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 48);
  // mr. r10,r11
  r10 = r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge 0x82283c60
  if (!cr0.lt) goto loc_82283C60_82283BB0;
loc_82283C58_82283BB0:
  // mr r10,r6
  r10 = r6;
  // b 0x82283c6c
  goto loc_82283C6C_82283BB0;
loc_82283C60_82283BB0:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
loc_82283C64_82283BB0:
  // ble cr6,0x82283c6c
  if (!cr6.gt) goto loc_82283C6C_82283BB0;
  // li r10,6
  r10 = uint64_t(int64_t(6));
loc_82283C6C_82283BB0:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x82283c7c
  if (!cr6.lt) goto loc_82283C7C_82283BB0;
  // mr r11,r6
  r11 = r6;
  // b 0x82283c88
  goto loc_82283C88_82283BB0;
loc_82283C7C_82283BB0:
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // ble cr6,0x82283c88
  if (!cr6.gt) goto loc_82283C88_82283BB0;
  // li r11,6
  r11 = uint64_t(int64_t(6));
loc_82283C88_82283BB0:
  // stw r11,64(r9)
  guest::StoreU32(base, uint32_t(r9) + 64, uint32_t(r11));
  // stw r10,60(r9)
  guest::StoreU32(base, uint32_t(r9) + 60, uint32_t(r10));
loc_82283C90_82283BB0:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // lwz r10,48(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 48);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // stw r6,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r6));
  // stw r30,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r30));
  // stw r6,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r6));
  // stw r6,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r6));
  // stw r6,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r6));
  // stw r6,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r6));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // b 0x82284014
  goto loc_82284014_82283BB0;
loc_82283CCC_82283BB0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,48(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mr r9,r10
  r9 = r10;
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge 0x82283cf0
  if (!cr0.lt) goto loc_82283CF0_82283BB0;
  // mr r10,r6
  r10 = r6;
  // b 0x82283cfc
  goto loc_82283CFC_82283BB0;
loc_82283CF0_82283BB0:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // ble cr6,0x82283cfc
  if (!cr6.gt) goto loc_82283CFC_82283BB0;
  // li r10,6
  r10 = uint64_t(int64_t(6));
loc_82283CFC_82283BB0:
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bge cr6,0x82283d0c
  if (!cr6.lt) goto loc_82283D0C_82283BB0;
  // mr r9,r6
  r9 = r6;
  // b 0x82283d18
  goto loc_82283D18_82283BB0;
loc_82283D0C_82283BB0:
  // cmpwi cr6,r9,6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 6, so);
  // ble cr6,0x82283d18
  if (!cr6.gt) goto loc_82283D18_82283BB0;
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_82283D18_82283BB0:
  // stw r9,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r9));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // stw r10,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,25040(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // lwz r10,48(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 48);
  // stw r6,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r6));
  // stw r30,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r30));
  // stw r6,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r6));
  // stw r6,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r6));
  // stw r6,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r6));
  // stw r6,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r6));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // b 0x82284014
  goto loc_82284014_82283BB0;
loc_82283FF4_82283BB0:
  // stw r6,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r6));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stw r30,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r30));
  // stw r6,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r6));
  // stw r6,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r6));
  // stw r6,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r6));
  // stw r6,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r6));
  // stw r10,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r10));
loc_82284014_82283BB0:
  // stw r6,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r6));
  // stw r6,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r6));
  // stw r6,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r6));
  // stw r6,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r6));
  // lwz r3,25040(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // mr r30,r6
  r30 = r6;
  // stw r6,32(r3)
  guest::StoreU32(base, uint32_t(r3) + 32, uint32_t(r6));
loc_82284030_82283BB0:
  // addi r4,r30,119
  r4 = uint64_t(int64_t(int64_t(r30) + 119));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8228403C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 7, so);
  // blt cr6,0x82284030
  if (cr6.lt) goto loc_82284030_82283BB0;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228406C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284074, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,29
  r5 = uint64_t(int64_t(29));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r28
  r3 = r28;
  // lwz r11,25032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25032);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // addi r11,r11,31768
  r11 = uint64_t(int64_t(int64_t(r11) + 31768));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822840A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r5,30
  r5 = uint64_t(int64_t(30));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r28
  r3 = r28;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r6,r11,1
  r6 = r11 ^ 1;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822840C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,966
  r6 = uint64_t(int64_t(966));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(base, LIFTED_ARGS(0x822840DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(base, LIFTED_ARGS(0x822840F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x82284100, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82284108: lifted from generated/ by lift.py.
lifted::Result f82284108(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82284108 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82284138, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8228416c
  if (!cr6.eq) goto loc_8228416C;
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
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x82284164, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8228416C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82284178, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822841B8: lifted from generated/ by lift.py.
lifted::Result f822841B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822841B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822841E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82284230
  if (!cr6.eq) goto loc_82284230;
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
  // lwz r11,25032(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25032);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r4,r11,59
  r4 = uint64_t(int64_t(int64_t(r11) + 59));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82284220, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284228, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82284230:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x8228423C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82284258: lifted from generated/ by lift.py.
lifted::Result f82284258(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82284258 (lifted)");
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82284288, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82284338
  if (!cr6.eq) goto loc_82284338;
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
  { const lifted::Result o = game::UiMovie::ResetNode(base, LIFTED_ARGS(0x822842AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,138
  r4 = uint64_t(int64_t(138));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822842C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,33
  r4 = uint64_t(int64_t(33));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822842E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,132
  r4 = uint64_t(int64_t(132));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822842F0, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228430C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284314, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x82284330, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_82284338:
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
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82284360, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8228439c
  if (cr0.eq) goto loc_8228439C;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x8228439c
  if (cr6.eq) goto loc_8228439C;
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82284384, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,34
  r5 = uint64_t(int64_t(34));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82284394, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x822843a0
  goto loc_822843A0;
loc_8228439C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_822843A0:
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

// sub_822843B8: lifted from generated/ by lift.py.
lifted::Result f822843B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822843B8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822843DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,25032(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25032);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r4,r11,36
  r4 = uint64_t(int64_t(int64_t(r11) + 36));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822843F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25032(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25032);
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
  // addi r4,r11,36
  r4 = uint64_t(int64_t(int64_t(r11) + 36));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x8228441C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284424, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r11,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r11));
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // stw r11,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r11));
  // li r26,27
  r26 = uint64_t(int64_t(27));
  // stw r11,176(r1)
  guest::StoreU32(base, uint32_t(r1) + 176, uint32_t(r11));
  // mr r28,r25
  r28 = r25;
  // stw r11,184(r1)
  guest::StoreU32(base, uint32_t(r1) + 184, uint32_t(r11));
  // stw r11,192(r1)
  guest::StoreU32(base, uint32_t(r1) + 192, uint32_t(r11));
  // stw r11,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r11));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r10,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r10));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r29,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r29));
  // stw r29,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r29));
  // stw r9,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r9));
  // stw r11,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r11));
  // stw r11,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r11));
  // stw r11,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r11));
  // stw r11,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r11));
  // stw r11,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r11));
  // stw r11,172(r1)
  guest::StoreU32(base, uint32_t(r1) + 172, uint32_t(r11));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r10,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r10));
  // stw r10,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r10));
  // stw r10,188(r1)
  guest::StoreU32(base, uint32_t(r1) + 188, uint32_t(r10));
  // stw r10,200(r1)
  guest::StoreU32(base, uint32_t(r1) + 200, uint32_t(r10));
  // li r10,25
  r10 = uint64_t(int64_t(25));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // stw r8,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r8));
  // stw r29,196(r1)
  guest::StoreU32(base, uint32_t(r1) + 196, uint32_t(r29));
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // stw r10,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r10));
  // li r10,26
  r10 = uint64_t(int64_t(26));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r10,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r10));
  // stw r11,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r11));
loc_822844D0:
  // mr r5,r28
  r5 = r28;
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x822844E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r27,r3
  r27 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822844EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r27
  r3 = r27;
  // stw r29,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82284500, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r27
  r3 = r27;
  // stw r29,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82284514, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r27
  r3 = r27;
  // stw r29,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x82284528, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r5,r28
  r5 = r28;
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // stw r29,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82284540, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r28,5
  r5 = uint64_t(int64_t(int64_t(r28) + 5));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82284550, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // stw r29,80(r3)
  guest::StoreU32(base, uint32_t(r3) + 80, uint32_t(r29));
  // bne 0x822844d0
  if (!cr0.eq) goto loc_822844D0;
  // lwz r11,25032(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25032);
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82284584, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25032(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25032);
  // addi r10,r1,176
  r10 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r9
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x822845A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // stw r25,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r25));
  // lwz r11,25032(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25032);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822845D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25032(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25032);
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822845F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25032(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25032);
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,6
  r5 = uint64_t(int64_t(int64_t(r11) + 6));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8228461C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r25,80(r3)
  guest::StoreU32(base, uint32_t(r3) + 80, uint32_t(r25));
  // lwz r11,25032(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25032);
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r9,r11,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r9,r10
  r10 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r10));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x8228465c
  if (cr6.eq) goto loc_8228465C;
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r5,r11,6
  r5 = uint64_t(int64_t(int64_t(r11) + 6));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x82284658, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r25,80(r3)
  guest::StoreU32(base, uint32_t(r3) + 80, uint32_t(r25));
loc_8228465C:
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82284668: lifted from generated/ by lift.py.
lifted::Result f82284668(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82284668 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82284690, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 0);
  // li r4,97
  r4 = uint64_t(int64_t(97));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822846A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,98
  r4 = uint64_t(int64_t(98));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822846AC, r31)); r3 = o.r3; f1 = o.f1; }
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
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // mr r3,r31
  r3 = r31;
  // li r4,97
  r4 = uint64_t(int64_t(97));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822846dc
  if (!cr6.eq) goto loc_822846DC;
  // li r4,98
  r4 = uint64_t(int64_t(98));
loc_822846DC:
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822846E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x822846E8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82284700: lifted from generated/ by lift.py.
lifted::Result f82284700(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82284700 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82284730, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82284764
  if (!cr6.eq) goto loc_82284764;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // bl 0x82284668
  // (left out: dead in C++)
  { const lifted::Result o = f82284668(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_82284764:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82284770, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82284790: lifted from generated/ by lift.py.
lifted::Result f82284790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82284790 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822847B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822848f0
  if (!cr6.eq) goto loc_822848F0;
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822847DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822847E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,60(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 60);
  // stw r10,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82284850
  if (!cr6.eq) goto loc_82284850;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r9,60(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r9,60(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 60);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bge cr6,0x822848c4
  if (!cr6.lt) goto loc_822848C4;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r3,25040(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 0);
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822848c4
  if (cr0.eq) goto loc_822848C4;
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // b 0x822848a8
  goto loc_822848A8;
loc_82284850:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x822848c4
  if (!cr6.eq) goto loc_822848C4;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r9,60(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r9,84(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 84);
  // lwz r8,60(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 60);
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // blt cr6,0x822848c4
  if (cr6.lt) goto loc_822848C4;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r3,25040(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r10,60(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 60);
  // subf r4,r10,r11
  r4 = r11 - r10;
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822848c4
  if (cr0.eq) goto loc_822848C4;
  // li r4,127
  r4 = uint64_t(int64_t(127));
loc_822848A8:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822848C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r27,1
  r27 = uint64_t(int64_t(1));
loc_822848C4:
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_822848C8:
  // addi r4,r29,119
  r4 = uint64_t(int64_t(int64_t(r29) + 119));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822848D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 7, so);
  // blt cr6,0x822848c8
  if (cr6.lt) goto loc_822848C8;
  // bl 0x82283630
  // (left out: dead in C++)
  { const lifted::Result o = f82283630(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
  // b 0x822848f4
  goto loc_822848F4;
loc_822848F0:
  // li r27,1
  r27 = uint64_t(int64_t(1));
loc_822848F4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82284900, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r29,r11,27,31,31
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x82284938
  if (cr0.eq) goto loc_82284938;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
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
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82284930, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284938, r31)); r3 = o.r3; f1 = o.f1; }
loc_82284938:
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x82284990
  if (!cr6.eq) goto loc_82284990;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_82284944:
  // addi r4,r29,119
  r4 = uint64_t(int64_t(int64_t(r29) + 119));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284950, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 7, so);
  // blt cr6,0x82284944
  if (cr6.lt) goto loc_82284944;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
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
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82284980, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284988, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82284994
  goto loc_82284994;
loc_82284990:
  // mr r3,r29
  r3 = r29;
loc_82284994:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822849A0: lifted from generated/ by lift.py.
lifted::Result f822849A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822849A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822849C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r28,0(r3)
  r28 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r26,r31
  r26 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82284c0c
  if (!cr6.eq) goto loc_82284C0C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27348(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82284a04
  if (cr6.eq) goto loc_82284A04;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // b 0x82284a08
  goto loc_82284A08;
loc_82284A04:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
loc_82284A08:
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r28
  r3 = r28;
  // addi r29,r11,-1
  r29 = uint64_t(int64_t(int64_t(r11) + -1));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284A18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284A24, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lis r7,-32127
  r7 = uint64_t(int64_t(-2105475072));
  // lwz r10,60(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 60);
  // stw r10,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r10));
  // lwz r11,25036(r7)
  r11 = guest::LoadU32(base, uint32_t(r7) + 25036);
  // lwz r8,25040(r30)
  r8 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r9,36(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 36);
  // lwz r10,80(r8)
  r10 = guest::LoadU32(base, uint32_t(r8) + 80);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // bne cr6,0x82284ad8
  if (!cr6.eq) goto loc_82284AD8;
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // addic. r9,r9,-7
  ca = uint32_t(r9) > 6;
  r9 = uint64_t(int64_t(int64_t(r9) + -7));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r31));
  // stw r31,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r31));
  // bge 0x82284ab0
  if (!cr0.lt) goto loc_82284AB0;
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // stw r31,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r31));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // lwz r3,25040(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // stw r31,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r31));
  // b 0x82284be0
  goto loc_82284BE0;
loc_82284AB0:
  // li r8,8
  r8 = uint64_t(int64_t(8));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r8));
  // lwz r3,25040(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,25036(r7)
  r11 = guest::LoadU32(base, uint32_t(r7) + 25036);
  // lwz r10,60(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 60);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // subf r4,r10,r11
  r4 = r11 - r10;
  // b 0x82284bdc
  goto loc_82284BDC;
loc_82284AD8:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bne cr6,0x82284be0
  if (!cr6.eq) goto loc_82284BE0;
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r9,r9,7
  r9 = uint64_t(int64_t(int64_t(r9) + 7));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bgt cr6,0x82284b94
  if (cr6.gt) goto loc_82284B94;
  // cmplwi cr6,r10,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 7, so);
  // bge cr6,0x82284b40
  if (!cr6.lt) goto loc_82284B40;
  // mr r11,r10
  r11 = r10;
  // mr r9,r10
  r9 = r10;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x82284b10
  if (!cr6.lt) goto loc_82284B10;
  // mr r9,r31
  r9 = r31;
  // b 0x82284b1c
  goto loc_82284B1C;
loc_82284B10:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // ble cr6,0x82284b1c
  if (!cr6.gt) goto loc_82284B1C;
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_82284B1C:
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x82284b2c
  if (!cr6.lt) goto loc_82284B2C;
  // mr r11,r31
  r11 = r31;
  // b 0x82284b38
  goto loc_82284B38;
loc_82284B2C:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // ble cr6,0x82284b38
  if (!cr6.gt) goto loc_82284B38;
  // li r11,6
  r11 = uint64_t(int64_t(6));
loc_82284B38:
  // stw r9,60(r8)
  guest::StoreU32(base, uint32_t(r8) + 60, uint32_t(r9));
  // b 0x82284b48
  goto loc_82284B48;
loc_82284B40:
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,60(r8)
  guest::StoreU32(base, uint32_t(r8) + 60, uint32_t(r11));
loc_82284B48:
  // stw r11,64(r8)
  guest::StoreU32(base, uint32_t(r8) + 64, uint32_t(r11));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // lwz r11,25036(r7)
  r11 = guest::LoadU32(base, uint32_t(r7) + 25036);
  // mr r4,r10
  r4 = r10;
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r31));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r31,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r31));
  // stw r31,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r31));
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // stw r31,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r31));
  // lwz r3,25040(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // b 0x82284bdc
  goto loc_82284BDC;
loc_82284B94:
  // li r8,8
  r8 = uint64_t(int64_t(8));
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r31));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r31,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r31));
  // stw r31,56(r11)
  guest::StoreU32(base, uint32_t(r11) + 56, uint32_t(r31));
  // stw r8,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r8));
  // stw r31,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(base, uint32_t(r11) + 28, uint32_t(r31));
  // lwz r3,25040(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,68(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 68);
  // add r11,r11,r29
  r11 = r11 + r29;
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
loc_82284BDC:
  // bl 0x822822c0
  // (left out: dead in C++)
  r3 = f822822C0(base, r3, r4);
loc_82284BE0:
  // mr r29,r31
  r29 = r31;
loc_82284BE4:
  // addi r4,r29,119
  r4 = uint64_t(int64_t(int64_t(r29) + 119));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284BF0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 7, so);
  // blt cr6,0x82284be4
  if (cr6.lt) goto loc_82284BE4;
  // bl 0x82283630
  // (left out: dead in C++)
  { const lifted::Result o = f82283630(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r11));
  // b 0x82284c10
  goto loc_82284C10;
loc_82284C0C:
  // li r26,1
  r26 = uint64_t(int64_t(1));
loc_82284C10:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82284C1C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r29,r11,27,31,31
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x82284c54
  if (cr0.eq) goto loc_82284C54;
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82284C4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284C54, r31)); r3 = o.r3; f1 = o.f1; }
loc_82284C54:
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // bne cr6,0x82284ca8
  if (!cr6.eq) goto loc_82284CA8;
loc_82284C5C:
  // addi r4,r31,119
  r4 = uint64_t(int64_t(int64_t(r31) + 119));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284C68, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmplwi cr6,r31,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 7, so);
  // blt cr6,0x82284c5c
  if (cr6.lt) goto loc_82284C5C;
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82284C98, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284CA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82284cac
  goto loc_82284CAC;
loc_82284CA8:
  // mr r3,r29
  r3 = r29;
loc_82284CAC:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82284CB8: lifted from generated/ by lift.py.
lifted::Result f82284CB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82284CB8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r3
  r28 = r3;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82284CE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r29,0(r3)
  r29 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82284df8
  if (!cr6.eq) goto loc_82284DF8;
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284CFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284D08, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,48(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 48);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // bne cr6,0x82284d50
  if (!cr6.eq) goto loc_82284D50;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r10));
  // b 0x82284da4
  goto loc_82284DA4;
loc_82284D50:
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r9,25040(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,48(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 48);
  // addic. r11,r11,-4
  ca = uint32_t(r11) > 3;
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge 0x82284d6c
  if (!cr0.lt) goto loc_82284D6C;
  // mr r11,r10
  r11 = r10;
loc_82284D6C:
  // stw r11,52(r9)
  guest::StoreU32(base, uint32_t(r9) + 52, uint32_t(r11));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r9,52(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 52);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x82284d94
  if (!cr6.eq) goto loc_82284D94;
  // lwz r9,48(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 48);
  // stw r9,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r9));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r9,64(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 64);
  // b 0x82284da0
  goto loc_82284DA0;
loc_82284D94:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // stw r9,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r9));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
loc_82284DA0:
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
loc_82284DA4:
  // mr r30,r10
  r30 = r10;
loc_82284DA8:
  // addi r4,r30,119
  r4 = uint64_t(int64_t(int64_t(r30) + 119));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284DB4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 7, so);
  // blt cr6,0x82284da8
  if (cr6.lt) goto loc_82284DA8;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
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
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82284DE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82283630
  // (left out: dead in C++)
  { const lifted::Result o = f82283630(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_82284DF8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82284E04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82284E10: lifted from generated/ by lift.py.
lifted::Result f82284E10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82284E10 (lifted)");
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82284E38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82284ef8
  if (!cr6.eq) goto loc_82284EF8;
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284E54, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284E60, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,60(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 60);
  // stw r10,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r10));
  // lwz r11,25036(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 25036);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addic. r10,r10,-100
  ca = uint32_t(r10) > 99;
  r10 = uint64_t(int64_t(int64_t(r10) + -100));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge 0x82284e88
  if (!cr0.lt) goto loc_82284E88;
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_82284E88:
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r3,r30
  r3 = r30;
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // lwz r11,25036(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 25036);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,64(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 64);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284EC4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // mr r3,r30
  r3 = r30;
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82284EE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82284EF0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_82284EF8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82284F04, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82283630
  // (left out: dead in C++)
  { const lifted::Result o = f82283630(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82284F18: lifted from generated/ by lift.py.
lifted::Result f82284F18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82284F18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r3
  r28 = r3;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82284F40, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82285094
  if (!cr6.eq) goto loc_82285094;
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284F5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284F68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284F74, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284F80, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82284F8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r8,25040(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r9,76(r8)
  r9 = guest::LoadU32(base, uint32_t(r8) + 76);
  // lwz r7,0(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // addi r7,r7,100
  r7 = uint64_t(int64_t(int64_t(r7) + 100));
  // bgt cr6,0x82284fcc
  if (cr6.gt) goto loc_82284FCC;
  // stw r29,52(r8)
  guest::StoreU32(base, uint32_t(r8) + 52, uint32_t(r29));
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r10));
  // b 0x82285030
  goto loc_82285030;
loc_82284FCC:
  // cmpw cr6,r7,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r9), so);
  // bge cr6,0x82285004
  if (!cr6.lt) goto loc_82285004;
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,25036(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r9,60(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 60);
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // add r10,r9,r10
  r10 = r9 + r10;
  // stw r10,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r29,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r29));
  // b 0x82285030
  goto loc_82285030;
loc_82285004:
  // addi r10,r9,-1
  r10 = uint64_t(int64_t(int64_t(r9) + -1));
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,76(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 76);
  // addi r10,r10,-7
  r10 = uint64_t(int64_t(int64_t(r10) + -7));
  // stw r10,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r8,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r8));
loc_82285030:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,60(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 60);
  // stw r10,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r10));
loc_8228503C:
  // addi r4,r29,119
  r4 = uint64_t(int64_t(int64_t(r29) + 119));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82285048, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 7, so);
  // blt cr6,0x8228503c
  if (cr6.lt) goto loc_8228503C;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
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
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82285078, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // mr r3,r30
  r3 = r30;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x8228508C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_82285094:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822850A0, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82283630
  // (left out: dead in C++)
  { const lifted::Result o = f82283630(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822850B0: lifted from generated/ by lift.py.
lifted::Result f822850B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822850B0 (lifted)");
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822850D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82285188
  if (!cr6.eq) goto loc_82285188;
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822850F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82285100, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,60(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 60);
  // stw r10,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,64(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 64);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x82285154, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // mr r3,r30
  r3 = r30;
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x82285178, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x82285180, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_82285188:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x82285194, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82283630
  // (left out: dead in C++)
  { const lifted::Result o = f82283630(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822851A8: lifted from generated/ by lift.py.
lifted::Result f822851A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822851A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r3
  r28 = r3;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x822851D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822852bc
  if (!cr6.eq) goto loc_822852BC;
  // li r4,127
  r4 = uint64_t(int64_t(127));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822851EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x822851F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,60(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 60);
  // stw r10,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r9,25040(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r9,76(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 76);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r9,76(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 76);
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // bgt cr6,0x82285254
  if (cr6.gt) goto loc_82285254;
  // stw r29,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r29));
  // lwz r11,25036(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r10));
  // b 0x82285270
  goto loc_82285270;
loc_82285254:
  // lwz r10,25036(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 25036);
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // addi r10,r10,-6
  r10 = uint64_t(int64_t(int64_t(r10) + -6));
  // stw r10,52(r11)
  guest::StoreU32(base, uint32_t(r11) + 52, uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r9,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r9));
loc_82285270:
  // addi r4,r29,119
  r4 = uint64_t(int64_t(int64_t(r29) + 119));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x8228527C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 7, so);
  // blt cr6,0x82285270
  if (cr6.lt) goto loc_82285270;
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
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
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // addi r4,r11,119
  r4 = uint64_t(int64_t(int64_t(r11) + 119));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x822852AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(base, LIFTED_ARGS(0x822852B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_822852BC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x822852C8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82283630
  // (left out: dead in C++)
  { const lifted::Result o = f82283630(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822852D8: lifted from generated/ by lift.py.
uint64_t f822852D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822852D8 (lifted)");
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82285308, r31)); r3 = o.r3; f1 = o.f1; }
  // clrldi r11,r31,32
  r11 = r31 & 0xFFFFFFFF;
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,13
  r4 = uint64_t(int64_t(13));
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
  { const lifted::Result o = game::UiMovie::FindNode(base, LIFTED_ARGS(0x8228532C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82285348: lifted from generated/ by lift.py.
uint64_t f82285348(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82285348 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
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
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82285378, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // li r5,25
  r5 = uint64_t(int64_t(25));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bne cr6,0x82285398
  if (!cr6.eq) goto loc_82285398;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82285394, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822853bc
  goto loc_822853BC;
loc_82285398:
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // bne cr6,0x822853b8
  if (!cr6.eq) goto loc_822853B8;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822853A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,26
  r5 = uint64_t(int64_t(26));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x822853B4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822853cc
  goto loc_822853CC;
loc_822853B8:
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822853BC, r31)); r3 = o.r3; f1 = o.f1; }
loc_822853BC:
  // li r5,26
  r5 = uint64_t(int64_t(26));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x822853CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_822853CC:
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

// sub_822853E8: lifted from generated/ by lift.py.
uint64_t f822853E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822853E8 (lifted)");
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
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x82285414, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // beq cr6,0x82285454
  if (cr6.eq) goto loc_82285454;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82285430, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82285440, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,34
  r5 = uint64_t(int64_t(34));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(base, LIFTED_ARGS(0x82285450, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82285478
  goto loc_82285478;
loc_82285454:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82285458, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82285468, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,34
  r5 = uint64_t(int64_t(34));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(base, LIFTED_ARGS(0x82285478, r31)); r3 = o.r3; f1 = o.f1; }
loc_82285478:
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

// sub_82285490: lifted from generated/ by lift.py.
lifted::Result f82285490(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82285490 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x822830d8
  // (left out: dead in C++)
  { const lifted::Result o = f822830D8(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,28220(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28220);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822855e0
  if (cr6.eq) goto loc_822855E0;
  // lwz r11,244(r11)
  r11 = guest::LoadU32(base, schema::Session(base, uint32_t(r11)).ranking_after_race_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822855e0
  if (cr0.eq) goto loc_822855E0;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // lwz r9,25040(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // lwz r11,28228(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28228);
  // stw r11,8(r9)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r9)).unknown_8_address(), uint32_t(r11));
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lwz r11,25032(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 25032);
  // stw r31,12(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).first_item_address(), uint32_t(r31));
  // stw r8,16(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).last_item_address(), uint32_t(r8));
  // stw r7,20(r11)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r11)).wraps_address(), uint32_t(r7));
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
  // lwz r9,25032(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 25032);
  // lwz r8,28232(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 28232);
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, schema::FrontEndMenu(base, uint32_t(r9)).cursor_address());
  // stw r8,0(r9)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r9)).cursor_address(), uint32_t(r8));
  // stw r7,4(r9)
  guest::StoreU32(base, schema::FrontEndMenu(base, uint32_t(r9)).previous_address(), uint32_t(r7));
  // lwz r9,28232(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 28232);
  // lwz r8,25040(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // stw r9,100(r8)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r8)).unknown_100_address(), uint32_t(r9));
  // lwz r9,28232(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 28232);
  // lwz r8,25040(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // stw r9,96(r8)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r8)).unknown_96_address(), uint32_t(r9));
  // lwz r11,28232(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28232);
  // addi r9,r11,-5
  r9 = uint64_t(int64_t(int64_t(r11) + -5));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // stw r9,92(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_92_address(), uint32_t(r9));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // lwz r9,92(r11)
  r9 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_92_address());
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bge cr6,0x8228557c
  if (!cr6.lt) goto loc_8228557C;
  // stw r31,92(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_92_address(), uint32_t(r31));
  // b 0x8228558c
  goto loc_8228558C;
loc_8228557C:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r9,100(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_100_address(), uint32_t(r9));
  // lwz r11,25040(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 25040);
  // stw r9,96(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_96_address(), uint32_t(r9));
loc_8228558C:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x822855b8
  if (cr6.eq) goto loc_822855B8;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822855B8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // li r8,12
  r8 = uint64_t(int64_t(12));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // lwz r11,28220(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28220);
  // stw r31,244(r11)
  guest::StoreU32(base, schema::Session(base, uint32_t(r11)).ranking_after_race_address(), uint32_t(r31));
  // b 0x82285614
  goto loc_82285614;
loc_822855E0:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822855f8
  if (cr0.eq) goto loc_822855F8;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_822855F8:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r31,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r10));
  // stw r31,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r31));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
loc_82285614:
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

// sub_82285630: lifted from generated/ by lift.py.
lifted::Result f82285630(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82285630 (lifted)");
  uint64_t r3 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // lis r26,-32127
  r26 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,40(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_40_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82285680
  if (!cr6.eq) goto loc_82285680;
  // lwz r11,25028(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82285670
  if (cr6.eq) goto loc_82285670;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82285670:
  // stw r5,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r5));
  // stw r30,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r30));
  // stw r30,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r30));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
loc_82285680:
  // lwz r25,104(r11)
  r25 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_104_address());
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // stw r30,104(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_104_address(), uint32_t(r30));
  // cmpwi r25,0
  cr0 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // beq 0x8228584c
  if (cr0.eq) goto loc_8228584C;
  // cmpwi cr6,r25,1
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 1, so);
  // bne cr6,0x822856a4
  if (!cr6.eq) goto loc_822856A4;
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // b 0x822856b0
  goto loc_822856B0;
loc_822856A4:
  // cmpwi cr6,r25,2
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 2, so);
  // bne cr6,0x822856c4
  if (!cr6.eq) goto loc_822856C4;
  // li r10,11
  r10 = uint64_t(int64_t(11));
loc_822856B0:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r10,20(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_20_address(), uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r30,24(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_24_address(), uint32_t(r30));
  // b 0x8228584c
  goto loc_8228584C;
loc_822856C4:
  // cmpwi cr6,r25,3
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 3, so);
  // bne cr6,0x82285744
  if (!cr6.eq) goto loc_82285744;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r4,50
  r4 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,25008(r9)
  guest::StoreU32(base, uint32_t(r9) + 25008, uint32_t(r10));
  // li r9,18
  r9 = uint64_t(int64_t(18));
  // lwz r10,600(r11)
  r10 = schema::NetModeDispatcher(base, uint32_t(r11) - 204).ring_count_bits();
  // li r3,1
  r3 = uint64_t(int64_t(1));
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
  // divwu r4,r6,r4
  r4 = uint32_t(uint32_t(r4) ? uint32_t(r6) / uint32_t(r4) : 0);
  // mulli r4,r4,50
  r4 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(50))));
  // subf r10,r4,r6
  r10 = r6 - r4;
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
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r5,20(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_20_address(), uint32_t(r5));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r30,24(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_24_address(), uint32_t(r30));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r3,104(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_104_address(), uint32_t(r3));
  // b 0x8228584c
  goto loc_8228584C;
loc_82285744:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // stw r30,80(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).row_count_address(), uint32_t(r30));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r30,76(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).row_limit_address(), uint32_t(r30));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r30,48(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_48_address(), uint32_t(r30));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r30,52(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_52_address(), uint32_t(r30));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r30,56(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_56_address(), uint32_t(r30));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_28_address());
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // blt cr6,0x8228582c
  if (cr6.lt) goto loc_8228582C;
  // beq cr6,0x82285824
  if (cr6.eq) goto loc_82285824;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x822857f4
  if (cr6.lt) goto loc_822857F4;
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
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 604);
  // li r28,1
  r28 = uint64_t(int64_t(1));
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
  // b 0x8228582c
  goto loc_8228582C;
loc_822857F4:
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // ori r10,r11,52528
  r10 = r11 | 52528;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8228582c
  if (!cr6.eq) goto loc_8228582C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822853e8
  // (left out: dead in C++)
  r3 = f822853E8(base, r1, r13, r3);
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x8228582c
  goto loc_8228582C;
loc_82285824:
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // bl 0x822821d8
  // (left out: dead in C++)
  r3 = f822821D8(base, r1, r13);
loc_8228582C:
  // bl 0x82283468
  // (left out: dead in C++)
  { const lifted::Result o = f82283468(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x8228584c
  if (!cr6.eq) goto loc_8228584C;
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // beq cr6,0x8228584c
  if (cr6.eq) goto loc_8228584C;
  // bl 0x82283b08
  // (left out: dead in C++)
  { const lifted::Result o = f82283B08(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82283630
  // (left out: dead in C++)
  { const lifted::Result o = f82283630(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x822838b8
  // (left out: dead in C++)
  { const lifted::Result o = f822838B8(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_8228584C:
  // lwz r10,25040(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r11,25028(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 25028);
  // lwz r9,20(r10)
  r9 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r10)).unknown_20_address());
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r10)).unknown_24_address());
  // cmpw cr6,r9,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r8), so);
  // beq cr6,0x8228586c
  if (cr6.eq) goto loc_8228586C;
  // stw r8,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r8));
loc_8228586C:
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // stw r30,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r30));
  // bne cr6,0x82285884
  if (!cr6.eq) goto loc_82285884;
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r30,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r30));
  // b 0x82285890
  goto loc_82285890;
loc_82285884:
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r9));
  // stw r30,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r30));
loc_82285890:
  // cmpwi cr6,r25,3
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 3, so);
  // bne cr6,0x822858a0
  if (!cr6.eq) goto loc_822858A0;
  // bl 0x82282f98
  // (left out: dead in C++)
  r3 = f82282F98(base, r1, r13);
  // b 0x822858a4
  goto loc_822858A4;
loc_822858A0:
  // bl 0x82282de8
  // (left out: dead in C++)
  r3 = f82282DE8(base, r1, r13);
loc_822858A4:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x822858b0
  if (cr6.eq) goto loc_822858B0;
  // bl 0x82283048
  // (left out: dead in C++)
  r3 = f82283048(base, r1, r13);
loc_822858B0:
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,28(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_28_address(), uint32_t(r10));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r30,20(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_20_address(), uint32_t(r30));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // stw r30,24(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_24_address(), uint32_t(r30));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822858D8: lifted from generated/ by lift.py.
lifted::Result f822858D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_822858D8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // stw r30,12(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_12_address(), uint32_t(r30));
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_16_address());
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // bne cr6,0x822859ac
  if (!cr6.eq) goto loc_822859AC;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r28,50
  r28 = uint64_t(int64_t(50));
  // mr r8,r30
  r8 = r30;
  // mr r7,r30
  r7 = r30;
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_count_bits();
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r31,604(r11)
  r31 = schema::ModeDispatcher(base, uint32_t(r11) - 204).ring_start_bits();
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // add r31,r31,r10
  r31 = r31 + r10;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(base, uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
  // divwu r28,r31,r28
  r28 = uint32_t(uint32_t(r28) ? uint32_t(r31) / uint32_t(r28) : 0);
  // mulli r28,r28,50
  r28 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(50))));
  // subf r10,r28,r31
  r10 = r31 - r28;
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
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x82285988
  if (cr6.eq) goto loc_82285988;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_82285988:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).next_page_address(), uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // bl 0x82282f10
  // (left out: dead in C++)
  r3 = f82282F10(base, r1, r13);
  // b 0x822859d4
  goto loc_822859D4;
loc_822859AC:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::PageObject(base, uint32_t(r11)).page_address());
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // beq cr6,0x822859c4
  if (cr6.eq) goto loc_822859C4;
  // stw r9,8(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_822859C4:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r10,0(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::PageObject(base, uint32_t(r11)).queued_step_address(), uint32_t(r9));
loc_822859D4:
  // lwz r11,25040(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25040);
  // stw r30,16(r11)
  guest::StoreU32(base, schema::NetRankingData(base, uint32_t(r11)).unknown_16_address(), uint32_t(r30));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822859E8: lifted from generated/ by lift.py.
lifted::Result f822859E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822859E8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,25032(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 25032);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x82285A04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,25032(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 25032);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82285A10, r31)); r3 = o.r3; f1 = o.f1; }
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // beq 0x82285acc
  if (cr0.eq) goto loc_82285ACC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82285A2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r26,50
  r26 = uint64_t(int64_t(50));
  // mr r8,r27
  r8 = r27;
  // mr r7,r27
  r7 = r27;
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r31,604(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 604);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // add r31,r31,r10
  r31 = r31 + r10;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r10));
  // divwu r26,r31,r26
  r26 = uint32_t(uint32_t(r26) ? uint32_t(r31) / uint32_t(r26) : 0);
  // mulli r26,r26,50
  r26 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(50))));
  // subf r10,r26,r31
  r10 = r31 - r26;
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
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25028(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x82285aac
  if (cr6.eq) goto loc_82285AAC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285AAC:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r28,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r28));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // bl 0x82282f10
  // (left out: dead in C++)
  r3 = f82282F10(base, r1, r13);
  // b 0x82285b10
  goto loc_82285B10;
loc_82285ACC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,25032(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 25032);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82285AD8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82285b10
  if (cr0.eq) goto loc_82285B10;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82285AE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25028(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82285afc
  if (cr6.eq) goto loc_82285AFC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285AFC:
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
loc_82285B10:
  // lwz r11,25032(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25032);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82285b54
  if (cr6.eq) goto loc_82285B54;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82285B28, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25028(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82285b3c
  if (cr0.eq) goto loc_82285B3C;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285B3C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r28,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r28));
  // stw r27,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r27));
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,25032(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 25032);
loc_82285B54:
  // lwz r11,36(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 36);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82285b90
  if (cr6.eq) goto loc_82285B90;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82285B68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25028(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82285b7c
  if (cr0.eq) goto loc_82285B7C;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285B7C:
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // stw r28,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r28));
  // stw r27,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r27));
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
loc_82285B90:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82285B98: lifted from generated/ by lift.py.
lifted::Result f82285B98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82285B98 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // bl 0x82295028
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserGetSigninState(base, LIFTED_ARGS(0x82285BB4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // beq cr6,0x82285c10
  if (cr6.eq) goto loc_82285C10;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82285bd4
  if (cr6.eq) goto loc_82285BD4;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285BD4:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r9,25040(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // stw r10,40(r9)
  guest::StoreU32(base, uint32_t(r9) + 40, uint32_t(r10));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // stw r10,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // stw r10,180(r11)
  guest::StoreU32(base, uint32_t(r11) + 180, uint32_t(r10));
  // b 0x82285c78
  goto loc_82285C78;
loc_82285C10:
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // lwz r10,36(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 36);
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,30
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 30, so);
  // stw r9,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r9));
  // blt cr6,0x82285c78
  if (cr6.lt) goto loc_82285C78;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x82285c44
  if (cr6.eq) goto loc_82285C44;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285C44:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r9,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r9));
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25040);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,36(r11)
  guest::StoreU32(base, uint32_t(r11) + 36, uint32_t(r10));
loc_82285C78:
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

// sub_82285C90: lifted from generated/ by lift.py.
lifted::Result f82285C90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82285C90 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,27348(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82285cd4
  if (cr6.eq) goto loc_82285CD4;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mulli r11,r11,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(48))));
  // ldx r4,r10,r11
  r4 = guest::LoadU64(base, uint32_t(r10) + uint32_t(r11));
loc_82285CD4:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,28220(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // bl 0x822923d8
  // (left out: dead in C++)
  { const lifted::Result o = f822923D8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82285d18
  if (cr0.eq) goto loc_82285D18;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x82285d00
  if (cr6.eq) goto loc_82285D00;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285D00:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // b 0x82285db0
  goto loc_82285DB0;
loc_82285D18:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,14
  r10 = uint64_t(int64_t(14));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
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
  // li r4,1
  r4 = uint64_t(int64_t(1));
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
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // stw r4,104(r11)
  guest::StoreU32(base, uint32_t(r11) + 104, uint32_t(r4));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82285d98
  if (cr6.eq) goto loc_82285D98;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285D98:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // bl 0x82282f98
  // (left out: dead in C++)
  r3 = f82282F98(base, r1, r13);
loc_82285DB0:
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

// sub_82285DC8: lifted from generated/ by lift.py.
uint64_t f82285DC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82285DC8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r31,r3
  r31 = r3;
  // lwz r3,28220(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 28220);
  // bl 0x822923b8
  // (left out: dead in C++)
  r3 = f822923B8(base);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82285f40
  if (cr0.eq) goto loc_82285F40;
  // lwz r11,28220(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28220);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82285e0c
  if (cr6.eq) goto loc_82285E0C;
  // lwz r11,556(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 556);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82285ea8
  if (!cr6.eq) goto loc_82285EA8;
loc_82285E0C:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,14
  r10 = uint64_t(int64_t(14));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
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
  // li r4,1
  r4 = uint64_t(int64_t(1));
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
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // stw r4,104(r11)
  guest::StoreU32(base, uint32_t(r11) + 104, uint32_t(r4));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82285e8c
  if (cr6.eq) goto loc_82285E8C;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285E8C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // bl 0x82282f98
  // (left out: dead in C++)
  r3 = f82282F98(base, r1, r13);
  // b 0x82285f40
  goto loc_82285F40;
loc_82285EA8:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,24996(r9)
  guest::StoreU32(base, uint32_t(r9) + 24996, uint32_t(r10));
  // li r9,19
  r9 = uint64_t(int64_t(19));
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
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // stw r10,104(r11)
  guest::StoreU32(base, uint32_t(r11) + 104, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82285f28
  if (cr6.eq) goto loc_82285F28;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82285F28:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // bl 0x82282de8
  // (left out: dead in C++)
  r3 = f82282DE8(base, r1, r13);
loc_82285F40:
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

// sub_82285F58: lifted from generated/ by lift.py.
lifted::Result f82285F58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82285F58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,27348(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82285f9c
  if (cr6.eq) goto loc_82285F9C;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25036(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25036);
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mulli r11,r11,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(48))));
  // ldx r4,r10,r11
  r4 = guest::LoadU64(base, uint32_t(r10) + uint32_t(r11));
loc_82285F9C:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,28220(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // bl 0x82292518
  // (left out: dead in C++)
  { const lifted::Result o = f82292518(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // beq 0x8228603c
  if (cr0.eq) goto loc_8228603C;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,8
  r10 = uint64_t(int64_t(8));
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
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x82286020
  if (cr6.eq) goto loc_82286020;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82286020:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // bl 0x82282f10
  // (left out: dead in C++)
  r3 = f82282F10(base, r1, r13);
  // b 0x822860c4
  goto loc_822860C4;
loc_8228603C:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // li r4,1
  r4 = uint64_t(int64_t(1));
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25040(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25040);
  // stw r4,104(r11)
  guest::StoreU32(base, uint32_t(r11) + 104, uint32_t(r4));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25028(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x822860ac
  if (cr6.eq) goto loc_822860AC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822860AC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // bl 0x82282f98
  // (left out: dead in C++)
  r3 = f82282F98(base, r1, r13);
loc_822860C4:
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

// sub_822860D8: lifted from generated/ by lift.py.
lifted::Result f822860D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822860D8 (lifted)");
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // mr r27,r3
  r27 = r3;
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x822860F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82286100, r31)); r3 = o.r3; f1 = o.f1; }
  // li r26,-1
  r26 = uint64_t(int64_t(-1));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r25,6
  r25 = uint64_t(int64_t(6));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // beq 0x8228617c
  if (cr0.eq) goto loc_8228617C;
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r11,25036(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r11,20
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 20, so);
  // blt cr6,0x82286148
  if (cr6.lt) goto loc_82286148;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82286144, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822861d0
  goto loc_822861D0;
loc_82286148:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82286150, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25028(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x82286164
  if (cr6.eq) goto loc_82286164;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82286164:
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stw r26,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r26));
  // stw r26,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r26));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x82282f10
  // (left out: dead in C++)
  r3 = f82282F10(base, r1, r13);
  // b 0x822861d0
  goto loc_822861D0;
loc_8228617C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82286188, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822861d0
  if (cr0.eq) goto loc_822861D0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82286198, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,28220(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28220);
  // stw r29,160(r11)
  guest::StoreU32(base, uint32_t(r11) + 160, uint32_t(r29));
  // lwz r11,25028(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822861b8
  if (cr0.eq) goto loc_822861B8;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822861B8:
  // stw r25,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r25));
  // stw r26,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r26));
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r26,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r26));
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // stw r29,44(r11)
  guest::StoreU32(base, uint32_t(r11) + 44, uint32_t(r29));
loc_822861D0:
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82286278
  if (cr6.eq) goto loc_82286278;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822861F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,25040(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,80(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r11,25036(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // lwz r9,52(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 52);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bgt cr6,0x82286248
  if (cr6.gt) goto loc_82286248;
  // lwz r10,56(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 56);
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // blt cr6,0x82286248
  if (cr6.lt) goto loc_82286248;
  // lwz r11,25028(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x82286240
  if (cr6.eq) goto loc_82286240;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82286240:
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // b 0x82286374
  goto loc_82286374;
loc_82286248:
  // addi r3,r11,-50
  r3 = uint64_t(int64_t(int64_t(r11) + -50));
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bge cr6,0x82286258
  if (!cr6.lt) goto loc_82286258;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_82286258:
  // mr r6,r27
  r6 = r27;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_8228626C:
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // stw r10,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r10));
  // b 0x822865c8
  goto loc_822865C8;
loc_82286278:
  // lwz r11,36(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 36);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x822862f0
  if (!cr6.eq) goto loc_822862F0;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8228628C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,25040(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,80(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r11,25036(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // lwz r9,52(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 52);
  // lwz r10,56(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 56);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,-7
  r11 = uint64_t(int64_t(int64_t(r11) + -7));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // blt cr6,0x8228635c
  if (cr6.lt) goto loc_8228635C;
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bgt cr6,0x822862c8
  if (cr6.gt) goto loc_822862C8;
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // bge cr6,0x8228635c
  if (!cr6.lt) goto loc_8228635C;
loc_822862C8:
  // addi r3,r11,-50
  r3 = uint64_t(int64_t(int64_t(r11) + -50));
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bge cr6,0x822862d8
  if (!cr6.lt) goto loc_822862D8;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_822862D8:
  // mr r6,r27
  r6 = r27;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x8228626c
  goto loc_8228626C;
loc_822862F0:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8228638c
  if (!cr6.eq) goto loc_8228638C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82286300, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,25040(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r8,80(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 80);
  // cmpwi cr6,r8,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r9,25036(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // cmpwi cr6,r8,100
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 100, so);
  // lwz r11,56(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 56);
  // lwz r9,0(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 0);
  // ble cr6,0x8228635c
  if (!cr6.gt) goto loc_8228635C;
  // lwz r10,76(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 76);
  // cmpwi cr6,r10,100
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 100, so);
  // blt cr6,0x8228635c
  if (cr6.lt) goto loc_8228635C;
  // subf r10,r9,r11
  r10 = r11 - r9;
  // addi r10,r10,-7
  r10 = uint64_t(int64_t(int64_t(r10) + -7));
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // bge cr6,0x8228635c
  if (!cr6.lt) goto loc_8228635C;
  // mr r6,r27
  r6 = r27;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r11,-50
  r3 = uint64_t(int64_t(int64_t(r11) + -50));
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // b 0x8228626c
  goto loc_8228626C;
loc_8228635C:
  // lwz r11,25028(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x82286370
  if (cr6.eq) goto loc_82286370;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_82286370:
  // li r10,14
  r10 = uint64_t(int64_t(14));
loc_82286374:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // stw r10,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r10));
  // stw r26,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r26));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r26,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r26));
  // b 0x82286634
  goto loc_82286634;
loc_8228638C:
  // li r4,163
  r4 = uint64_t(int64_t(163));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82286394, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822863d4
  if (cr0.eq) goto loc_822863D4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822863A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r10,80(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r10));
loc_822863C0:
  // mr r6,r27
  r6 = r27;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x822865c8
  goto loc_822865C8;
loc_822863D4:
  // li r4,164
  r4 = uint64_t(int64_t(164));
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822863E0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8228642c
  if (cr0.eq) goto loc_8228642C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822863F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r3,r11,-100
  r3 = uint64_t(int64_t(int64_t(r11) + -100));
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bge cr6,0x82286414
  if (!cr6.lt) goto loc_82286414;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_82286414:
  // mr r6,r27
  r6 = r27;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // b 0x8228626c
  goto loc_8228626C;
loc_8228642C:
  // li r4,161
  r4 = uint64_t(int64_t(161));
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82286438, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822864a4
  if (cr0.eq) goto loc_822864A4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82286448, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,25040(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,80(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r11,25036(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r10,-149
  r3 = uint64_t(int64_t(int64_t(r10) + -149));
  // lwz r11,27348(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27348);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82286480
  if (cr6.eq) goto loc_82286480;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // b 0x82286484
  goto loc_82286484;
loc_82286480:
  // mr r11,r26
  r11 = r26;
loc_82286484:
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // blt cr6,0x82286490
  if (cr6.lt) goto loc_82286490;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_82286490:
  // cmplwi cr6,r3,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 1, so);
  // bge cr6,0x8228649c
  if (!cr6.lt) goto loc_8228649C;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_8228649C:
  // stw r25,32(r9)
  guest::StoreU32(base, uint32_t(r9) + 32, uint32_t(r25));
  // b 0x822863c0
  goto loc_822863C0;
loc_822864A4:
  // li r4,162
  r4 = uint64_t(int64_t(162));
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822864B0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8228651c
  if (cr0.eq) goto loc_8228651C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x822864C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,25040(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,80(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r10,25036(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r11,100
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 100, so);
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // bgt cr6,0x822864f0
  if (cr6.gt) goto loc_822864F0;
loc_822864E8:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82286510
  goto loc_82286510;
loc_822864F0:
  // addi r8,r10,100
  r8 = uint64_t(int64_t(int64_t(r10) + 100));
  // cmplw cr6,r8,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r11), so);
  // blt cr6,0x8228650c
  if (cr6.lt) goto loc_8228650C;
  // addi r3,r11,-100
  r3 = uint64_t(int64_t(int64_t(r11) + -100));
  // cmplwi cr6,r3,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 1, so);
  // bge cr6,0x82286510
  if (!cr6.lt) goto loc_82286510;
  // b 0x822864e8
  goto loc_822864E8;
loc_8228650C:
  // addi r3,r10,50
  r3 = uint64_t(int64_t(int64_t(r10) + 50));
loc_82286510:
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,32(r9)
  guest::StoreU32(base, uint32_t(r9) + 32, uint32_t(r11));
  // b 0x822863c0
  goto loc_822863C0;
loc_8228651C:
  // li r4,157
  r4 = uint64_t(int64_t(157));
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x82286528, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822865f0
  if (cr0.eq) goto loc_822865F0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x82286538, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r10,80(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // addi r11,r27,204
  r11 = uint64_t(int64_t(int64_t(r27) + 204));
  // li r27,50
  r27 = uint64_t(int64_t(50));
  // mr r8,r29
  r8 = r29;
  // mr r7,r29
  r7 = r29;
  // stw r10,25052(r9)
  guest::StoreU32(base, uint32_t(r9) + 25052, uint32_t(r10));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r31,604(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 604);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // add r31,r31,r10
  r31 = r31 + r10;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r10));
  // divwu r27,r31,r27
  r27 = uint32_t(uint32_t(r27) ? uint32_t(r31) / uint32_t(r27) : 0);
  // mulli r27,r27,50
  r27 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(50))));
  // subf r10,r27,r31
  r10 = r31 - r27;
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
  // bl 0x82282bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82282BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25040(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // stw r29,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r29));
loc_822865C8:
  // lwz r11,25028(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 25028);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x822865dc
  if (cr6.eq) goto loc_822865DC;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
loc_822865DC:
  // stw r25,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r25));
  // stw r26,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r26));
  // stw r26,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r26));
  // bl 0x82282f10
  // (left out: dead in C++)
  r3 = f82282F10(base, r1, r13);
  // b 0x82286634
  goto loc_82286634;
loc_822865F0:
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // lwz r3,25036(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x822865FC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82286634
  if (cr0.eq) goto loc_82286634;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x8228660C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,25040(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 25040);
  // lwz r11,80(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82286634
  if (cr6.eq) goto loc_82286634;
  // lwz r11,25036(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 25036);
  // lwz r10,52(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 52);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // addi r3,r11,1
  r3 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x82292768
  // (left out: dead in C++)
  { const lifted::Result o = f82292768(base, r1, r13, r3, r6, r7, r8, f1, 0x82286634); r3 = o.r3; f1 = o.f1; }
loc_82286634:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

lifted::Result e8227E088(uint8_t* base, const lifted::Args& a) {
  f8227E088(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8227E0D8(uint8_t* base, const lifted::Args& a) {
  return f8227E0D8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8227E1D8(uint8_t* base, const lifted::Args& a) {
  f8227E1D8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8227E248(uint8_t* base, const lifted::Args& a) {
  return f8227E248(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8227E448(uint8_t* base, const lifted::Args& a) {
  return {f8227E448(base, a.r1, a.r[1]), a.f[0]};
}

lifted::Result e8227E4F0(uint8_t* base, const lifted::Args& a) {
  return f8227E4F0(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8227E688(uint8_t* base, const lifted::Args& a) {
  return f8227E688(base, a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8227E738(uint8_t* base, const lifted::Args& a) {
  return f8227E738(base, a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e8227E7C8(uint8_t* base, const lifted::Args& a) {
  return {f8227E7C8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8227E830(uint8_t* base, const lifted::Args& a) {
  return f8227E830(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8227E858(uint8_t* base, const lifted::Args& a) {
  return f8227E858(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8227E970(uint8_t* base, const lifted::Args& a) {
  return f8227E970(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8227ECF8(uint8_t* base, const lifted::Args& a) {
  return f8227ECF8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8227EF18(uint8_t* base, const lifted::Args& a) {
  f8227EF18(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8227EF68(uint8_t* base, const lifted::Args& a) {
  return f8227EF68(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8227EFA8(uint8_t* base, const lifted::Args& a) {
  f8227EFA8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8227F0F8(uint8_t* base, const lifted::Args& a) {
  f8227F0F8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8227F150(uint8_t* base, const lifted::Args& a) {
  return f8227F150(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8227F238(uint8_t* base, const lifted::Args& a) {
  return {f8227F238(base, a.r1, a.r[1]), a.f[0]};
}

lifted::Result e8227F2E0(uint8_t* base, const lifted::Args& a) {
  return f8227F2E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8227F400(uint8_t* base, const lifted::Args& a) {
  return f8227F400(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.f[0]);
}

lifted::Result e8227F480(uint8_t* base, const lifted::Args& a) {
  return {f8227F480(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8227F4E8(uint8_t* base, const lifted::Args& a) {
  return f8227F4E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8227F518(uint8_t* base, const lifted::Args& a) {
  return f8227F518(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8227F618(uint8_t* base, const lifted::Args& a) {
  return f8227F618(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8227F9D8(uint8_t* base, const lifted::Args& a) {
  f8227F9D8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8227FB08(uint8_t* base, const lifted::Args& a) {
  return {f8227FB08(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e8227FB78(uint8_t* base, const lifted::Args& a) {
  return {f8227FB78(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e8227FBE8(uint8_t* base, const lifted::Args& a) {
  f8227FBE8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8227FC08(uint8_t* base, const lifted::Args& a) {
  f8227FC08(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8227FC80(uint8_t* base, const lifted::Args& a) {
  return f8227FC80(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8227FFC8(uint8_t* base, const lifted::Args& a) {
  f8227FFC8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82280058(uint8_t* base, const lifted::Args& a) {
  return f82280058(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822800C0(uint8_t* base, const lifted::Args& a) {
  return {f822800C0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e822802C8(uint8_t* base, const lifted::Args& a) {
  return f822802C8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82280518(uint8_t* base, const lifted::Args& a) {
  f82280518(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82280568(uint8_t* base, const lifted::Args& a) {
  return f82280568(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82280618(uint8_t* base, const lifted::Args& a) {
  return f82280618(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e822806B8(uint8_t* base, const lifted::Args& a) {
  return f822806B8(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e82280798(uint8_t* base, const lifted::Args& a) {
  return {f82280798(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82280840(uint8_t* base, const lifted::Args& a) {
  return f82280840(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e82280AD8(uint8_t* base, const lifted::Args& a) {
  return {f82280AD8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82280B48(uint8_t* base, const lifted::Args& a) {
  return f82280B48(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82280BC8(uint8_t* base, const lifted::Args& a) {
  return {f82280BC8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82280C68(uint8_t* base, const lifted::Args& a) {
  return f82280C68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e82280CF0(uint8_t* base, const lifted::Args& a) {
  return f82280CF0(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82281068(uint8_t* base, const lifted::Args& a) {
  return f82281068(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e822812E0(uint8_t* base, const lifted::Args& a) {
  return f822812E0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82281358(uint8_t* base, const lifted::Args& a) {
  return f82281358(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e822813D0(uint8_t* base, const lifted::Args& a) {
  return f822813D0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82281648(uint8_t* base, const lifted::Args& a) {
  return f82281648(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e82281800(uint8_t* base, const lifted::Args& a) {
  return f82281800(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822820A8(uint8_t* base, const lifted::Args& a) {
  f822820A8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e822820F8(uint8_t* base, const lifted::Args& a) {
  f822820F8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e82282138(uint8_t* base, const lifted::Args& a) {
  return {f82282138(base, a.r1, a.r13, a.lr), a.f[0]};
}

lifted::Result e82282150(uint8_t* base, const lifted::Args& a) {
  f82282150(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822821D8(uint8_t* base, const lifted::Args& a) {
  return {f822821D8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e822822C0(uint8_t* base, const lifted::Args& a) {
  return {f822822C0(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82282630(uint8_t* base, const lifted::Args& a) {
  return f82282630(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e822826E8(uint8_t* base, const lifted::Args& a) {
  f822826E8(base, a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82282788(uint8_t* base, const lifted::Args& a) {
  return f82282788(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822827F0(uint8_t* base, const lifted::Args& a) {
  f822827F0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822828F8(uint8_t* base, const lifted::Args& a) {
  return f822828F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e82282B78(uint8_t* base, const lifted::Args& a) {
  f82282B78(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82282BC8(uint8_t* base, const lifted::Args& a) {
  return f82282BC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82282CF8(uint8_t* base, const lifted::Args& a) {
  return {f82282CF8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82282DE8(uint8_t* base, const lifted::Args& a) {
  return {f82282DE8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82282F10(uint8_t* base, const lifted::Args& a) {
  return {f82282F10(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82282F98(uint8_t* base, const lifted::Args& a) {
  return {f82282F98(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82283048(uint8_t* base, const lifted::Args& a) {
  return {f82283048(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e822830D8(uint8_t* base, const lifted::Args& a) {
  return f822830D8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82283468(uint8_t* base, const lifted::Args& a) {
  return f82283468(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82283630(uint8_t* base, const lifted::Args& a) {
  return f82283630(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e822838B8(uint8_t* base, const lifted::Args& a) {
  return f822838B8(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82283B08(uint8_t* base, const lifted::Args& a) {
  return f82283B08(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e82284108(uint8_t* base, const lifted::Args& a) {
  return f82284108(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e822841B8(uint8_t* base, const lifted::Args& a) {
  return f822841B8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82284258(uint8_t* base, const lifted::Args& a) {
  return f82284258(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822843B8(uint8_t* base, const lifted::Args& a) {
  return f822843B8(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e82284668(uint8_t* base, const lifted::Args& a) {
  return f82284668(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e82284700(uint8_t* base, const lifted::Args& a) {
  return f82284700(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82284790(uint8_t* base, const lifted::Args& a) {
  return f82284790(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822849A0(uint8_t* base, const lifted::Args& a) {
  return f822849A0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82284CB8(uint8_t* base, const lifted::Args& a) {
  return f82284CB8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82284E10(uint8_t* base, const lifted::Args& a) {
  return f82284E10(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82284F18(uint8_t* base, const lifted::Args& a) {
  return f82284F18(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822850B0(uint8_t* base, const lifted::Args& a) {
  return f822850B0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822851A8(uint8_t* base, const lifted::Args& a) {
  return f822851A8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822852D8(uint8_t* base, const lifted::Args& a) {
  return {f822852D8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82285348(uint8_t* base, const lifted::Args& a) {
  return {f82285348(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822853E8(uint8_t* base, const lifted::Args& a) {
  return {f822853E8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82285490(uint8_t* base, const lifted::Args& a) {
  return f82285490(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82285630(uint8_t* base, const lifted::Args& a) {
  return f82285630(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822858D8(uint8_t* base, const lifted::Args& a) {
  return f822858D8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e822859E8(uint8_t* base, const lifted::Args& a) {
  return f822859E8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82285B98(uint8_t* base, const lifted::Args& a) {
  return f82285B98(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82285C90(uint8_t* base, const lifted::Args& a) {
  return f82285C90(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82285DC8(uint8_t* base, const lifted::Args& a) {
  return {f82285DC8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82285F58(uint8_t* base, const lifted::Args& a) {
  return f82285F58(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822860D8(uint8_t* base, const lifted::Args& a) {
  return f822860D8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x8227E088, e8227E088},
    {0x8227E0D8, e8227E0D8},
    {0x8227E1D8, e8227E1D8},
    {0x8227E248, e8227E248},
    {0x8227E448, e8227E448},
    {0x8227E4F0, e8227E4F0},
    {0x8227E688, e8227E688},
    {0x8227E738, e8227E738},
    {0x8227E7C8, e8227E7C8},
    {0x8227E830, e8227E830},
    {0x8227E858, e8227E858},
    {0x8227E970, e8227E970},
    {0x8227ECF8, e8227ECF8},
    {0x8227EF18, e8227EF18},
    {0x8227EF68, e8227EF68},
    {0x8227EFA8, e8227EFA8},
    {0x8227F0F8, e8227F0F8},
    {0x8227F150, e8227F150},
    {0x8227F238, e8227F238},
    {0x8227F2E0, e8227F2E0},
    {0x8227F400, e8227F400},
    {0x8227F480, e8227F480},
    {0x8227F4E8, e8227F4E8},
    {0x8227F518, e8227F518},
    {0x8227F618, e8227F618},
    {0x8227F9D8, e8227F9D8},
    {0x8227FB08, e8227FB08},
    {0x8227FB78, e8227FB78},
    {0x8227FBE8, e8227FBE8},
    {0x8227FC08, e8227FC08},
    {0x8227FC80, e8227FC80},
    {0x8227FFC8, e8227FFC8},
    {0x82280058, e82280058},
    {0x822800C0, e822800C0},
    {0x822802C8, e822802C8},
    {0x82280518, e82280518},
    {0x82280568, e82280568},
    {0x82280618, e82280618},
    {0x822806B8, e822806B8},
    {0x82280798, e82280798},
    {0x82280840, e82280840},
    {0x82280AD8, e82280AD8},
    {0x82280B48, e82280B48},
    {0x82280BC8, e82280BC8},
    {0x82280C68, e82280C68},
    {0x82280CF0, e82280CF0},
    {0x82281068, e82281068},
    {0x822812E0, e822812E0},
    {0x82281358, e82281358},
    {0x822813D0, e822813D0},
    {0x82281648, e82281648},
    {0x82281800, e82281800},
    {0x822820A8, e822820A8},
    {0x822820F8, e822820F8},
    {0x82282138, e82282138},
    {0x82282150, e82282150},
    {0x822821D8, e822821D8},
    {0x822822C0, e822822C0},
    {0x82282630, e82282630},
    {0x822826E8, e822826E8},
    {0x82282788, e82282788},
    {0x822827F0, e822827F0},
    {0x822828F8, e822828F8},
    {0x82282B78, e82282B78},
    {0x82282BC8, e82282BC8},
    {0x82282CF8, e82282CF8},
    {0x82282DE8, e82282DE8},
    {0x82282F10, e82282F10},
    {0x82282F98, e82282F98},
    {0x82283048, e82283048},
    {0x822830D8, e822830D8},
    {0x82283468, e82283468},
    {0x82283630, e82283630},
    {0x822838B8, e822838B8},
    {0x82283B08, e82283B08},
    {0x82284108, e82284108},
    {0x822841B8, e822841B8},
    {0x82284258, e82284258},
    {0x822843B8, e822843B8},
    {0x82284668, e82284668},
    {0x82284700, e82284700},
    {0x82284790, e82284790},
    {0x822849A0, e822849A0},
    {0x82284CB8, e82284CB8},
    {0x82284E10, e82284E10},
    {0x82284F18, e82284F18},
    {0x822850B0, e822850B0},
    {0x822851A8, e822851A8},
    {0x822852D8, e822852D8},
    {0x82285348, e82285348},
    {0x822853E8, e822853E8},
    {0x82285490, e82285490},
    {0x82285630, e82285630},
    {0x822858D8, e822858D8},
    {0x822859E8, e822859E8},
    {0x82285B98, e82285B98},
    {0x82285C90, e82285C90},
    {0x82285DC8, e82285DC8},
    {0x82285F58, e82285F58},
    {0x822860D8, e822860D8},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_040)), true);

}  // namespace

}  // namespace lifted
