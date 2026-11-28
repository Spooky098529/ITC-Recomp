// Lift sweep batch 012 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/camera_modes.h"
#include "schema/cameras.h"
#include "schema/career.h"
#include "schema/location_screen.h"
#include "schema/menu_helpers.h"
#include "schema/options_screen.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/race_hud_object.h"
#include "schema/race_screens.h"
#include "schema/race_screens_unseen.h"
#include "schema/race_states.h"
#include "schema/save_interface.h"
#include "schema/tuning_pages.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies_game.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_012, true, "ITC",
              "Run lift sweep batch 012's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_821F0998 game::TuningScreen::InputInteriorList: lifted from generated/ by lift.py.
lifted::Result f821F0998(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821F0998 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
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
  r10 = guest::LoadU32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f09f4
  if (cr6.eq) goto loc_821F09F4;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F09EC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0ca4
  if (!cr0.eq) goto loc_821F0CA4;
loc_821F09F4:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0a20
  if (cr6.eq) goto loc_821F0A20;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0A18, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0ca4
  if (!cr0.eq) goto loc_821F0CA4;
loc_821F0A20:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0a4c
  if (cr6.eq) goto loc_821F0A4C;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0A44, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0c60
  if (!cr0.eq) goto loc_821F0C60;
loc_821F0A4C:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0a78
  if (cr6.eq) goto loc_821F0A78;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0A70, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0c60
  if (!cr0.eq) goto loc_821F0C60;
loc_821F0A78:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0ba8
  if (cr6.eq) goto loc_821F0BA8;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0A9C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f0ba8
  if (cr0.eq) goto loc_821F0BA8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F0AAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // ori r10,r11,33548
  r10 = r11 | 33548;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r7,54
  r7 = uint64_t(int64_t(54));
  // lwzx r10,r31,r10
  r10 = guest::LoadU32(uint32_t(r31) + uint32_t(r10));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,9716(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).command_first_run_address(), uint32_t(r11));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // bne cr6,0x821f0b3c
  if (!cr6.eq) goto loc_821F0B3C;
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // li r9,14
  r9 = uint64_t(int64_t(14));
  // stw r8,9672(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).remove_cursor_address(), uint32_t(r8));
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r7));
  // lbz r7,10008(r31)
  r7 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // stb r7,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r7));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r8,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r8));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r9,0(r11)
  r9 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821f0b34
  if (cr6.eq) goto loc_821F0B34;
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_821F0B34:
  // li r9,63
  r9 = uint64_t(int64_t(63));
  // b 0x821f0c4c
  goto loc_821F0C4C;
loc_821F0B3C:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,55
  r9 = uint64_t(int64_t(55));
  // ori r11,r11,33556
  r11 = r11 | 33556;
  // stwx r8,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r8));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r7));
  // lbz r7,10008(r31)
  r7 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // stb r7,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r7));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r8,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r8));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r9,0(r11)
  r9 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821f0ba0
  if (cr6.eq) goto loc_821F0BA0;
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_821F0BA0:
  // li r9,56
  r9 = uint64_t(int64_t(56));
  // b 0x821f0c4c
  goto loc_821F0C4C;
loc_821F0BA8:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0d14
  if (cr6.eq) goto loc_821F0D14;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0BCC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f0d14
  if (cr0.eq) goto loc_821F0D14;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F0BDC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,9732(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).rewrite_car_address(), uint32_t(r11));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // addi r9,r11,255
  r9 = uint64_t(int64_t(int64_t(r11) + 255));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // clrlwi r9,r9,24
  r9 = uint32_t(r9) & 0xFF;
  // mr r8,r9
  r8 = r9;
  // addi r8,r8,2439
  r8 = uint64_t(int64_t(int64_t(r8) + 2439));
  // stb r9,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r9));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r9,r8,r31
  r9 = guest::LoadU32(uint32_t(r8) + uint32_t(r31));
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r7,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r7));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r9,0(r11)
  r9 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821f0c48
  if (cr6.eq) goto loc_821F0C48;
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_821F0C48:
  // li r9,54
  r9 = uint64_t(int64_t(54));
loc_821F0C4C:
  // stw r9,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // b 0x821f0d14
  goto loc_821F0D14;
loc_821F0C60:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F0C68, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r10,r31,1
  r10 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,-31988
  r10 = uint64_t(int64_t(int64_t(r10) + -31988));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lwz r9,0(r10)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r10) - 33548).interior_pick_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpwi cr6,r9,3
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 3, so);
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33548).interior_pick_address(), uint32_t(r9));
  // ble cr6,0x821f0c94
  if (!cr6.gt) goto loc_821F0C94;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // b 0x821f0cd0
  goto loc_821F0CD0;
loc_821F0C94:
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // bne cr6,0x821f0d04
  if (!cr6.eq) goto loc_821F0D04;
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // b 0x821f0cec
  goto loc_821F0CEC;
loc_821F0CA4:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F0CAC, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r10,r31,1
  r10 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,-31988
  r10 = uint64_t(int64_t(int64_t(r10) + -31988));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lwz r9,0(r10)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r10) - 33548).interior_pick_address());
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33548).interior_pick_address(), uint32_t(r9));
  // bge 0x821f0ce4
  if (!cr0.lt) goto loc_821F0CE4;
  // li r9,3
  r9 = uint64_t(int64_t(3));
loc_821F0CD0:
  // lis r8,0
  r8 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33548).interior_pick_address(), uint32_t(r9));
  // ori r8,r8,33552
  r8 = r8 | 33552;
  // stwx r11,r31,r8
  guest::StoreU32(uint32_t(r31) + uint32_t(r8), uint32_t(r11));
  // b 0x821f0d14
  goto loc_821F0D14;
loc_821F0CE4:
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // bne cr6,0x821f0d04
  if (!cr6.eq) goto loc_821F0D04;
loc_821F0CEC:
  // lis r9,0
  r9 = uint64_t(int64_t(0));
  // stw r11,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33548).interior_pick_address(), uint32_t(r11));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // ori r9,r9,33552
  r9 = r9 | 33552;
  // stwx r8,r31,r9
  guest::StoreU32(uint32_t(r31) + uint32_t(r9), uint32_t(r8));
  // b 0x821f0d14
  goto loc_821F0D14;
loc_821F0D04:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ori r11,r11,33552
  r11 = r11 | 33552;
  // stwx r10,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r10));
loc_821F0D14:
  // mr r3,r31
  r3 = r31;
  // bl 0x821ec038
  // (left out: dead in C++)
  { const lifted::Result o = game::TuningScreen::AnimateInteriorList(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821F0D38 game::TuningScreen::InputInteriorOptions: lifted from generated/ by lift.py.
lifted::Result f821F0D38(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821F0D38 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
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
  r10 = guest::LoadU32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0d94
  if (cr6.eq) goto loc_821F0D94;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0D8C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f10bc
  if (!cr0.eq) goto loc_821F10BC;
loc_821F0D94:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0dc0
  if (cr6.eq) goto loc_821F0DC0;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0DB8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f10bc
  if (!cr0.eq) goto loc_821F10BC;
loc_821F0DC0:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0dec
  if (cr6.eq) goto loc_821F0DEC;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0DE4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1088
  if (!cr0.eq) goto loc_821F1088;
loc_821F0DEC:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0e18
  if (cr6.eq) goto loc_821F0E18;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0E10, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1088
  if (!cr0.eq) goto loc_821F1088;
loc_821F0E18:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0fd4
  if (cr6.eq) goto loc_821F0FD4;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0E3C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f0fd4
  if (cr0.eq) goto loc_821F0FD4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F0E4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // li r10,55
  r10 = uint64_t(int64_t(55));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r3,r31
  r3 = r31;
  // stw r30,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r30));
  // stwx r10,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r10));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r11,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r11));
  // bl 0x821cce30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CCE30, LIFTED_ARGS(0x821F0E7C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821f0ed4
  if (cr0.eq) goto loc_821F0ED4;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,10036(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stb r11,9665(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).confirm_cursor_address(), uint8_t(r11));
  // stw r8,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r8));
  // stw r8,4(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).resume_address(), uint32_t(r8));
  // stw r8,0(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).page_address(), uint32_t(r8));
  // stw r8,8(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).previous_page_address(), uint32_t(r8));
  // stw r9,20(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).step_address(), uint32_t(r9));
  // stw r9,12(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).current_address(), uint32_t(r9));
  // stw r11,24(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).started_address(), uint32_t(r11));
  // lwz r10,10040(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).category_menu_address());
  // stw r11,0(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_cursor_bits(uint32_t(r11));
  // stw r11,4(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_previous_bits(uint32_t(r11));
  // stw r11,40(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_wrapped_bits(uint32_t(r11));
  // stw r11,48(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_visible_row_bits(uint32_t(r11));
  // stw r11,52(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_previous_row_bits(uint32_t(r11));
  // stw r11,32(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_shoulders_step_bits(uint32_t(r11));
  // b 0x821f0f94
  goto loc_821F0F94;
loc_821F0ED4:
  // mr r3,r31
  r3 = r31;
  // bl 0x821cd160
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CD160, LIFTED_ARGS(0x821F0EDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25116(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).money_address());
  // lwz r10,10036(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // cmpw cr6,r11,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r3), so);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // bge cr6,0x821f0f50
  if (!cr6.lt) goto loc_821F0F50;
  // li r8,10
  r8 = uint64_t(int64_t(10));
  // stw r8,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r8));
  // stw r8,4(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).resume_address(), uint32_t(r8));
  // stw r8,0(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).page_address(), uint32_t(r8));
  // stw r8,8(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).previous_page_address(), uint32_t(r8));
  // stw r9,20(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).step_address(), uint32_t(r9));
  // stw r9,12(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).current_address(), uint32_t(r9));
  // stw r11,24(r10)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r10)).started_address(), uint32_t(r11));
  // lwz r10,10040(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).category_menu_address());
  // stw r11,16(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_last_item_bits(uint32_t(r11));
  // stw r30,20(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_wraps_bits(uint32_t(r30));
  // stw r11,0(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_cursor_bits(uint32_t(r11));
  // stw r11,4(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_previous_bits(uint32_t(r11));
  // stw r11,8(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_moved_bits(uint32_t(r11));
  // stw r11,40(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_wrapped_bits(uint32_t(r11));
  // stw r11,48(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_visible_row_bits(uint32_t(r11));
  // stw r11,52(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_previous_row_bits(uint32_t(r11));
  // stw r11,36(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_side_bits(uint32_t(r11));
  // stw r11,64(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_player_bits(uint32_t(r11));
  // stw r11,24(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_up_down_locked_bits(uint32_t(r11));
  // stw r11,28(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_left_right_steps_bits(uint32_t(r11));
  // stw r11,32(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_shoulders_step_bits(uint32_t(r11));
  // b 0x821f0fb0
  goto loc_821F0FB0;
loc_821F0F50:
  // li r8,9
  r8 = uint64_t(int64_t(9));
  // stb r11,9665(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).confirm_cursor_address(), uint8_t(r11));
  // li r7,8
  r7 = uint64_t(int64_t(8));
  // stw r8,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r8));
  // stw r8,4(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_previous_bits(uint32_t(r8));
  // stw r8,0(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_cursor_bits(uint32_t(r8));
  // stw r8,8(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_moved_bits(uint32_t(r8));
  // stw r9,20(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_wraps_bits(uint32_t(r9));
  // stw r9,12(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_first_item_bits(uint32_t(r9));
  // stw r11,24(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_up_down_locked_bits(uint32_t(r11));
  // lbz r8,9665(r31)
  r8 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).confirm_cursor_address());
  // lwz r10,10040(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).category_menu_address());
  // stw r8,0(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_cursor_bits(uint32_t(r8));
  // stw r8,4(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_previous_bits(uint32_t(r8));
  // stw r11,44(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_list_wrapped_bits(uint32_t(r11));
  // stw r11,56(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_first_row_bits(uint32_t(r11));
  // stw r7,60(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_rows_shown_bits(uint32_t(r7));
loc_821F0F94:
  // stw r11,28(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_left_right_steps_bits(uint32_t(r11));
  // stw r11,24(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_up_down_locked_bits(uint32_t(r11));
  // stw r11,64(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_player_bits(uint32_t(r11));
  // stw r11,36(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_side_bits(uint32_t(r11));
  // stw r11,8(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_moved_bits(uint32_t(r11));
  // stw r30,20(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_wraps_bits(uint32_t(r30));
  // stw r30,16(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_last_item_bits(uint32_t(r30));
loc_821F0FB0:
  // stw r11,12(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_first_item_bits(uint32_t(r11));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // beq cr6,0x821f0fcc
  if (cr6.eq) goto loc_821F0FCC;
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821F0FCC:
  // li r10,63
  r10 = uint64_t(int64_t(63));
  // b 0x821f1074
  goto loc_821F1074;
loc_821F0FD4:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f110c
  if (cr6.eq) goto loc_821F110C;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F0FF8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f110c
  if (cr0.eq) goto loc_821F110C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1008, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // addi r10,r11,255
  r10 = uint64_t(int64_t(int64_t(r11) + 255));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // clrlwi r10,r10,24
  r10 = uint32_t(r10) & 0xFF;
  // mr r8,r10
  r8 = r10;
  // addi r8,r8,2439
  r8 = uint64_t(int64_t(int64_t(r8) + 2439));
  // stb r10,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r10));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r8,r31
  r10 = guest::LoadU32(uint32_t(r8) + uint32_t(r31));
  // stw r10,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r10));
  // stw r9,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r7,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r7));
  // stw r10,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // beq cr6,0x821f1070
  if (cr6.eq) goto loc_821F1070;
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821F1070:
  // li r10,55
  r10 = uint64_t(int64_t(55));
loc_821F1074:
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // b 0x821f110c
  goto loc_821F110C;
loc_821F1088:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1090, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // addi r11,r11,-31980
  r11 = uint64_t(int64_t(int64_t(r11) + -31980));
  // stw r30,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r30));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r11) - 33556).unknown_33556_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33556).unknown_33556_address(), uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ble cr6,0x821f1100
  if (!cr6.gt) goto loc_821F1100;
  // b 0x821f10e8
  goto loc_821F10E8;
loc_821F10BC:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F10C4, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // addi r11,r11,-31980
  r11 = uint64_t(int64_t(int64_t(r11) + -31980));
  // stw r30,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r30));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r11) - 33556).unknown_33556_address());
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33556).unknown_33556_address(), uint32_t(r10));
  // bge 0x821f10fc
  if (!cr0.lt) goto loc_821F10FC;
  // li r10,5
  r10 = uint64_t(int64_t(5));
loc_821F10E8:
  // lis r9,0
  r9 = uint64_t(int64_t(0));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33556).unknown_33556_address(), uint32_t(r10));
  // ori r9,r9,33560
  r9 = r9 | 33560;
  // stwx r30,r31,r9
  guest::StoreU32(uint32_t(r31) + uint32_t(r9), uint32_t(r30));
  // b 0x821f110c
  goto loc_821F110C;
loc_821F10FC:
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_821F1100:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,33560
  r11 = r11 | 33560;
  // stwx r10,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r10));
loc_821F110C:
  // mr r3,r31
  r3 = r31;
  // bl 0x821ec7e0
  // (left out: dead in C++)
  { const lifted::Result o = game::TuningScreen::AnimateInteriorOptions(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821F1130 game::TuningScreen::InputInteriorGrid: lifted from generated/ by lift.py.
lifted::Result f821F1130(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_821F1130 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r29,r10,r11
  r29 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x821F1170, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r23,0(r3)
  r23 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // ori r28,r10,33564
  r28 = r10 | 33564;
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f11b0
  if (cr6.eq) goto loc_821F11B0;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F11A8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f166c
  if (!cr0.eq) goto loc_821F166C;
loc_821F11B0:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f11dc
  if (cr6.eq) goto loc_821F11DC;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F11D4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f166c
  if (!cr0.eq) goto loc_821F166C;
loc_821F11DC:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1208
  if (cr6.eq) goto loc_821F1208;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1200, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1634
  if (!cr0.eq) goto loc_821F1634;
loc_821F1208:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1234
  if (cr6.eq) goto loc_821F1234;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F122C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1634
  if (!cr0.eq) goto loc_821F1634;
loc_821F1234:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1260
  if (cr6.eq) goto loc_821F1260;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1258, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f15ec
  if (!cr0.eq) goto loc_821F15EC;
loc_821F1260:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f128c
  if (cr6.eq) goto loc_821F128C;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1284, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f15ec
  if (!cr0.eq) goto loc_821F15EC;
loc_821F128C:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f12b8
  if (cr6.eq) goto loc_821F12B8;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F12B0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f15a0
  if (!cr0.eq) goto loc_821F15A0;
loc_821F12B8:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f12e4
  if (cr6.eq) goto loc_821F12E4;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F12DC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f15a0
  if (!cr0.eq) goto loc_821F15A0;
loc_821F12E4:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f14f4
  if (cr6.eq) goto loc_821F14F4;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1308, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f14f4
  if (cr0.eq) goto loc_821F14F4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1318, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // li r10,56
  r10 = uint64_t(int64_t(56));
  // stw r24,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r24));
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r10,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r10));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // lwzx r10,r31,r28
  r10 = guest::LoadU32(uint32_t(r31) + uint32_t(r28));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // stb r11,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r11));
  // bne cr6,0x821f139c
  if (!cr6.eq) goto loc_821F139C;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // stw r24,9716(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).command_first_run_address(), uint32_t(r24));
  // li r9,58
  r9 = uint64_t(int64_t(58));
  // ori r11,r11,33576
  r11 = r11 | 33576;
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // stwx r30,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r30));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r30,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r30));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r7,0(r11)
  r7 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r7), so);
  // beq cr6,0x821f158c
  if (cr6.eq) goto loc_821F158C;
  // stw r7,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r7));
  // b 0x821f158c
  goto loc_821F158C;
loc_821F139C:
  // mr r3,r31
  r3 = r31;
  // bl 0x821cce30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CCE30, LIFTED_ARGS(0x821F13A4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821f13f8
  if (cr0.eq) goto loc_821F13F8;
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stb r30,9665(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).confirm_cursor_address(), uint8_t(r30));
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r30,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r30));
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).category_menu_address());
  // stw r30,0(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_cursor_bits(uint32_t(r30));
  // stw r30,4(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_bits(uint32_t(r30));
  // stw r30,40(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_wrapped_bits(uint32_t(r30));
  // stw r30,48(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_visible_row_bits(uint32_t(r30));
  // stw r30,52(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_row_bits(uint32_t(r30));
  // stw r30,32(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_shoulders_step_bits(uint32_t(r30));
  // b 0x821f14b4
  goto loc_821F14B4;
loc_821F13F8:
  // mr r3,r31
  r3 = r31;
  // bl 0x821cd160
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CD160, LIFTED_ARGS(0x821F1400, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25116(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).money_address());
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // cmpw cr6,r11,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r3), so);
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // bge cr6,0x821f1470
  if (!cr6.lt) goto loc_821F1470;
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r30,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r30));
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).category_menu_address());
  // stw r30,16(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_last_item_bits(uint32_t(r30));
  // stw r24,20(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_wraps_bits(uint32_t(r24));
  // stw r30,0(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_cursor_bits(uint32_t(r30));
  // stw r30,4(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_bits(uint32_t(r30));
  // stw r30,8(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_moved_bits(uint32_t(r30));
  // stw r30,40(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_wrapped_bits(uint32_t(r30));
  // stw r30,48(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_visible_row_bits(uint32_t(r30));
  // stw r30,52(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_row_bits(uint32_t(r30));
  // stw r30,36(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_side_bits(uint32_t(r30));
  // stw r30,64(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_player_bits(uint32_t(r30));
  // stw r30,24(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_up_down_locked_bits(uint32_t(r30));
  // stw r30,28(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_left_right_steps_bits(uint32_t(r30));
  // stw r30,32(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_shoulders_step_bits(uint32_t(r30));
  // b 0x821f14d0
  goto loc_821F14D0;
loc_821F1470:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stb r30,9665(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).confirm_cursor_address(), uint8_t(r30));
  // li r8,8
  r8 = uint64_t(int64_t(8));
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // stw r9,4(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_bits(uint32_t(r9));
  // stw r9,0(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_cursor_bits(uint32_t(r9));
  // stw r9,8(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_moved_bits(uint32_t(r9));
  // stw r10,20(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_wraps_bits(uint32_t(r10));
  // stw r10,12(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_first_item_bits(uint32_t(r10));
  // stw r30,24(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_up_down_locked_bits(uint32_t(r30));
  // lbz r9,9665(r31)
  r9 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).confirm_cursor_address());
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).category_menu_address());
  // stw r9,0(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_cursor_bits(uint32_t(r9));
  // stw r9,4(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_bits(uint32_t(r9));
  // stw r30,44(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_list_wrapped_bits(uint32_t(r30));
  // stw r30,56(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_first_row_bits(uint32_t(r30));
  // stw r8,60(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_rows_shown_bits(uint32_t(r8));
loc_821F14B4:
  // stw r30,28(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_left_right_steps_bits(uint32_t(r30));
  // stw r30,24(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_up_down_locked_bits(uint32_t(r30));
  // stw r30,64(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_player_bits(uint32_t(r30));
  // stw r30,36(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_side_bits(uint32_t(r30));
  // stw r30,8(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_moved_bits(uint32_t(r30));
  // stw r24,20(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_wraps_bits(uint32_t(r24));
  // stw r24,16(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_last_item_bits(uint32_t(r24));
loc_821F14D0:
  // stw r30,12(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_first_item_bits(uint32_t(r30));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r9,0(r11)
  r9 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821f14ec
  if (cr6.eq) goto loc_821F14EC;
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_821F14EC:
  // li r9,63
  r9 = uint64_t(int64_t(63));
  // b 0x821f158c
  goto loc_821F158C;
loc_821F14F4:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f16b0
  if (cr6.eq) goto loc_821F16B0;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1518, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f16b0
  if (cr0.eq) goto loc_821F16B0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1528, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // stw r24,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r24));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // addi r9,r11,255
  r9 = uint64_t(int64_t(int64_t(r11) + 255));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // clrlwi r9,r9,24
  r9 = uint32_t(r9) & 0xFF;
  // mr r8,r9
  r8 = r9;
  // addi r8,r8,2439
  r8 = uint64_t(int64_t(int64_t(r8) + 2439));
  // stb r9,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r9));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r9,r8,r31
  r9 = guest::LoadU32(uint32_t(r8) + uint32_t(r31));
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r30,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r30));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r9,0(r11)
  r9 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821f1588
  if (cr6.eq) goto loc_821F1588;
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_821F1588:
  // li r9,54
  r9 = uint64_t(int64_t(54));
loc_821F158C:
  // stw r9,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r8));
loc_821F1598:
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // b 0x821f16b0
  goto loc_821F16B0;
loc_821F15A0:
  // add r29,r31,r28
  r29 = r31 + r28;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29) - 33564).interior_page_mode_address());
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821f15b8
  if (cr6.eq) goto loc_821F15B8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F15B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_821F15B8:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29) - 33564).interior_page_mode_address());
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821f16b0
  if (cr6.eq) goto loc_821F16B0;
  // addis r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 65536));
  // stw r24,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r24));
  // addi r11,r11,-31964
  r11 = uint64_t(int64_t(int64_t(r11) + -31964));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r11) - 33572).interior_pair_index_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33572).interior_pair_index_address(), uint32_t(r10));
  // ble cr6,0x821f16b0
  if (!cr6.gt) goto loc_821F16B0;
  // stw r30,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33572).interior_pair_index_address(), uint32_t(r30));
  // b 0x821f16b0
  goto loc_821F16B0;
loc_821F15EC:
  // add r29,r31,r28
  r29 = r31 + r28;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29) - 33564).interior_page_mode_address());
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821f1604
  if (cr6.eq) goto loc_821F1604;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1604, r31)); r3 = o.r3; f1 = o.f1; }
loc_821F1604:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29) - 33564).interior_page_mode_address());
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821f16b0
  if (cr6.eq) goto loc_821F16B0;
  // addis r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 65536));
  // stw r24,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r24));
  // addi r11,r11,-31964
  r11 = uint64_t(int64_t(int64_t(r11) + -31964));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r11) - 33572).interior_pair_index_address());
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33572).interior_pair_index_address(), uint32_t(r10));
  // bge 0x821f16b0
  if (!cr0.lt) goto loc_821F16B0;
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // b 0x821f1598
  goto loc_821F1598;
loc_821F1634:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F163C, r31)); r3 = o.r3; f1 = o.f1; }
  // add r11,r31,r28
  r11 = r31 + r28;
  // stw r24,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r24));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r11) - 33564).interior_page_mode_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33564).interior_page_mode_address(), uint32_t(r10));
  // ble cr6,0x821f16a4
  if (!cr6.gt) goto loc_821F16A4;
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // stw r30,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33564).interior_page_mode_address(), uint32_t(r30));
  // ori r10,r10,33568
  r10 = r10 | 33568;
  // stwx r24,r31,r10
  guest::StoreU32(uint32_t(r31) + uint32_t(r10), uint32_t(r24));
  // b 0x821f16b0
  goto loc_821F16B0;
loc_821F166C:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1674, r31)); r3 = o.r3; f1 = o.f1; }
  // add r11,r31,r28
  r11 = r31 + r28;
  // stw r24,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r24));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r11) - 33564).interior_page_mode_address());
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33564).interior_page_mode_address(), uint32_t(r10));
  // bge 0x821f16a4
  if (!cr0.lt) goto loc_821F16A4;
  // lis r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // ori r9,r9,33568
  r9 = r9 | 33568;
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33564).interior_page_mode_address(), uint32_t(r10));
  // stwx r24,r31,r9
  guest::StoreU32(uint32_t(r31) + uint32_t(r9), uint32_t(r24));
  // b 0x821f16b0
  goto loc_821F16B0;
loc_821F16A4:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,33568
  r11 = r11 | 33568;
  // stwx r30,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r30));
loc_821F16B0:
  // li r4,145
  r4 = uint64_t(int64_t(145));
  // mr r3,r23
  r3 = r23;
  // bl 0x8211bb18
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::NamedAnimationTime(LIFTED_ARGS(0x821F16BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11436(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11436);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bne cr6,0x821f176c
  if (!cr6.eq) goto loc_821F176C;
  // add r25,r31,r28
  r25 = r31 + r28;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r25) - 33564).interior_page_mode_address());
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821f176c
  if (cr6.eq) goto loc_821F176C;
  // mr r29,r30
  r29 = r30;
loc_821F16E0:
  // addi r28,r29,1
  r28 = uint64_t(int64_t(int64_t(r29) + 1));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r23
  r3 = r23;
  // addi r5,r28,8
  r5 = uint64_t(int64_t(int64_t(r28) + 8));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F16F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r27,r3
  r27 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F1700, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r6,r28
  r6 = r28;
  // li r5,379
  r5 = uint64_t(int64_t(379));
  // mr r3,r23
  r3 = r23;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x821F1714, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F1720, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x821cdc38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CDC38, LIFTED_ARGS(0x821F1730, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821f1740
  if (cr0.eq) goto loc_821F1740;
  // stw r24,80(r26)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r26)).visible_address(), uint32_t(r24));
  // b 0x821f1744
  goto loc_821F1744;
loc_821F1740:
  // stw r30,80(r26)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r26)).visible_address(), uint32_t(r30));
loc_821F1744:
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F1750, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r25)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r25) - 33564).interior_page_mode_address());
  // mulli r11,r11,5
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(5))));
  // add r11,r11,r29
  r11 = r11 + r29;
  // mr r29,r28
  r29 = r28;
  // cmpwi cr6,r29,5
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 5, so);
  // stw r11,108(r3)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).texture_entry_address(), uint32_t(r11));
  // blt cr6,0x821f16e0
  if (cr6.lt) goto loc_821F16E0;
loc_821F176C:
  // mr r3,r31
  r3 = r31;
  // bl 0x821ed108
  // (left out: dead in C++)
  { const lifted::Result o = game::TuningScreen::AnimateInteriorGrid(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821F1780 game::TuningScreen::InputPickSlot: lifted from generated/ by lift.py.
lifted::Result f821F1780(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821F1780 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
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
  r10 = guest::LoadU32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f17dc
  if (cr6.eq) goto loc_821F17DC;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F17D4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1a48
  if (!cr0.eq) goto loc_821F1A48;
loc_821F17DC:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1808
  if (cr6.eq) goto loc_821F1808;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1800, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1a48
  if (!cr0.eq) goto loc_821F1A48;
loc_821F1808:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1834
  if (cr6.eq) goto loc_821F1834;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F182C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1a14
  if (!cr0.eq) goto loc_821F1A14;
loc_821F1834:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1860
  if (cr6.eq) goto loc_821F1860;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1858, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1a14
  if (!cr0.eq) goto loc_821F1A14;
loc_821F1860:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1950
  if (cr6.eq) goto loc_821F1950;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1884, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f1950
  if (cr0.eq) goto loc_821F1950;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // ori r11,r11,33576
  r11 = r11 | 33576;
  // lwzx r4,r31,r11
  r4 = guest::LoadU32(uint32_t(r31) + uint32_t(r11));
  // bl 0x821ce188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CE188, LIFTED_ARGS(0x821F18A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821f1944
  if (cr0.eq) goto loc_821F1944;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F18B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lbz r10,10008(r31)
  r10 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // stw r8,9720(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).command_end_after_setup_address(), uint32_t(r8));
loc_821F18C4:
  // addi r11,r10,255
  r11 = uint64_t(int64_t(int64_t(r10) + 255));
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // mr r10,r11
  r10 = r11;
  // stb r11,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r11));
  // addi r11,r10,2439
  r11 = uint64_t(int64_t(int64_t(r10) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r9,r11,r31
  r9 = guest::LoadU32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r9,56
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 56, so);
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // bne cr6,0x821f18c4
  if (!cr6.eq) goto loc_821F18C4;
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r8,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r8));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r9,9652(r31)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r8,0(r11)
  r8 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r9,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r8), so);
  // beq cr6,0x821f1924
  if (cr6.eq) goto loc_821F1924;
  // stw r8,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r8));
loc_821F1924:
  // li r8,57
  r8 = uint64_t(int64_t(57));
  // stw r10,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r10));
  // mr r3,r31
  r3 = r31;
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r8,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r8));
  // bl 0x821e1db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821E1DB0, LIFTED_ARGS(0x821F1940, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821f1a98
  goto loc_821F1A98;
loc_821F1944:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F194C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821f1a98
  goto loc_821F1A98;
loc_821F1950:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1a98
  if (cr6.eq) goto loc_821F1A98;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1974, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f1a98
  if (cr0.eq) goto loc_821F1A98;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1984, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lbz r10,10008(r31)
  r10 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // stw r11,9732(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).rewrite_car_address(), uint32_t(r11));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // stw r8,9720(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).command_end_after_setup_address(), uint32_t(r8));
loc_821F199C:
  // addi r11,r10,255
  r11 = uint64_t(int64_t(int64_t(r10) + 255));
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // mr r10,r11
  r10 = r11;
  // stb r11,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r11));
  // addi r11,r10,2439
  r11 = uint64_t(int64_t(int64_t(r10) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r9,r11,r31
  r9 = guest::LoadU32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r9,56
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 56, so);
  // stw r9,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r9));
  // bne cr6,0x821f199c
  if (!cr6.eq) goto loc_821F199C;
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r8,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r8));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r9,0(r11)
  r9 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821f19fc
  if (cr6.eq) goto loc_821F19FC;
  // stw r9,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r9));
loc_821F19FC:
  // li r9,57
  r9 = uint64_t(int64_t(57));
  // stw r10,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r9));
  // b 0x821f1a98
  goto loc_821F1A98;
loc_821F1A14:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1A1C, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r10,r31,1
  r10 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,-31960
  r10 = uint64_t(int64_t(int64_t(r10) + -31960));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lwz r9,0(r10)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address(), uint32_t(r9));
  // ble cr6,0x821f1a88
  if (!cr6.gt) goto loc_821F1A88;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // b 0x821f1a74
  goto loc_821F1A74;
loc_821F1A48:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1A50, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r10,r31,1
  r10 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,-31960
  r10 = uint64_t(int64_t(int64_t(r10) + -31960));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lwz r9,0(r10)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address());
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address(), uint32_t(r9));
  // bge 0x821f1a88
  if (!cr0.lt) goto loc_821F1A88;
  // li r9,4
  r9 = uint64_t(int64_t(4));
loc_821F1A74:
  // lis r8,0
  r8 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address(), uint32_t(r9));
  // ori r8,r8,33580
  r8 = r8 | 33580;
  // stwx r11,r31,r8
  guest::StoreU32(uint32_t(r31) + uint32_t(r8), uint32_t(r11));
  // b 0x821f1a98
  goto loc_821F1A98;
loc_821F1A88:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ori r11,r11,33580
  r11 = r11 | 33580;
  // stwx r10,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r10));
loc_821F1A98:
  // mr r3,r31
  r3 = r31;
  // bl 0x821edca8
  // (left out: dead in C++)
  { const lifted::Result o = game::TuningScreen::AnimateSlots(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821F1AB8 game::TuningScreen::InputRemoveSlot: lifted from generated/ by lift.py.
lifted::Result f821F1AB8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821F1AB8 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
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
  r10 = guest::LoadU32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1b14
  if (cr6.eq) goto loc_821F1B14;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1B0C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1d70
  if (!cr0.eq) goto loc_821F1D70;
loc_821F1B14:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1b40
  if (cr6.eq) goto loc_821F1B40;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1B38, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1d70
  if (!cr0.eq) goto loc_821F1D70;
loc_821F1B40:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1b6c
  if (cr6.eq) goto loc_821F1B6C;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1B64, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1d3c
  if (!cr0.eq) goto loc_821F1D3C;
loc_821F1B6C:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1b98
  if (cr6.eq) goto loc_821F1B98;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1B90, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f1d3c
  if (!cr0.eq) goto loc_821F1D3C;
loc_821F1B98:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1c88
  if (cr6.eq) goto loc_821F1C88;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1BBC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f1c88
  if (cr0.eq) goto loc_821F1C88;
  // addis r30,r31,1
  r30 = uint64_t(int64_t(int64_t(r31) + 65536));
  // mr r3,r31
  r3 = r31;
  // addi r30,r30,-31960
  r30 = uint64_t(int64_t(int64_t(r30) + -31960));
  // lwz r4,0(r30)
  r4 = guest::LoadU32(schema::TuningScreen(uint32_t(r30) - 33576).interior_slot_address());
  // bl 0x821ce188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CE188, LIFTED_ARGS(0x821F1BD8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821f1c7c
  if (cr0.eq) goto loc_821F1C7C;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r30) - 33576).interior_slot_address());
  // addi r11,r11,8450
  r11 = uint64_t(int64_t(int64_t(r11) + 8450));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r31
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821f1c7c
  if (cr6.eq) goto loc_821F1C7C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1C00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r8,58
  r8 = uint64_t(int64_t(58));
  // li r10,14
  r10 = uint64_t(int64_t(14));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // stw r11,9716(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).command_first_run_address(), uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r8,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r8));
  // lbz r8,10008(r31)
  r8 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // stw r10,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r10));
  // stw r7,9672(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).remove_cursor_address(), uint32_t(r7));
  // stb r8,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r8));
  // stw r10,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
  // stw r9,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r7,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r7));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // beq cr6,0x821f1c74
  if (cr6.eq) goto loc_821F1C74;
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821F1C74:
  // li r10,63
  r10 = uint64_t(int64_t(63));
  // b 0x821f1d28
  goto loc_821F1D28;
loc_821F1C7C:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1C84, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821f1dc0
  goto loc_821F1DC0;
loc_821F1C88:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r30) - 33576).unknown_33580_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f1dc0
  if (cr6.eq) goto loc_821F1DC0;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(schema::TuningScreen(uint32_t(r30) - 33576).interior_slot_address());
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(schema::TuningPagesIndexFault(uint32_t(r11)).fault_40_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F1CAC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f1dc0
  if (cr0.eq) goto loc_821F1DC0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1CBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // addi r10,r11,255
  r10 = uint64_t(int64_t(int64_t(r11) + 255));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // clrlwi r10,r10,24
  r10 = uint32_t(r10) & 0xFF;
  // mr r8,r10
  r8 = r10;
  // addi r8,r8,2439
  r8 = uint64_t(int64_t(int64_t(r8) + 2439));
  // stb r10,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r10));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r8,r31
  r10 = guest::LoadU32(uint32_t(r8) + uint32_t(r31));
  // stw r10,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r10));
  // stw r9,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r7,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r7));
  // stw r10,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // beq cr6,0x821f1d24
  if (cr6.eq) goto loc_821F1D24;
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821F1D24:
  // li r10,57
  r10 = uint64_t(int64_t(57));
loc_821F1D28:
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r9));
  // b 0x821f1dc0
  goto loc_821F1DC0;
loc_821F1D3C:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1D44, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r10,r31,1
  r10 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,-31960
  r10 = uint64_t(int64_t(int64_t(r10) + -31960));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lwz r9,0(r10)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address(), uint32_t(r9));
  // ble cr6,0x821f1db0
  if (!cr6.gt) goto loc_821F1DB0;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // b 0x821f1d9c
  goto loc_821F1D9C;
loc_821F1D70:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F1D78, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r10,r31,1
  r10 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,-31960
  r10 = uint64_t(int64_t(int64_t(r10) + -31960));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lwz r9,0(r10)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address());
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address(), uint32_t(r9));
  // bge 0x821f1db0
  if (!cr0.lt) goto loc_821F1DB0;
  // li r9,4
  r9 = uint64_t(int64_t(4));
loc_821F1D9C:
  // lis r8,0
  r8 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  guest::StoreU32(schema::TuningScreen(uint32_t(r10) - 33576).interior_slot_address(), uint32_t(r9));
  // ori r8,r8,33580
  r8 = r8 | 33580;
  // stwx r11,r31,r8
  guest::StoreU32(uint32_t(r31) + uint32_t(r8), uint32_t(r11));
  // b 0x821f1dc0
  goto loc_821F1DC0;
loc_821F1DB0:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ori r11,r11,33580
  r11 = r11 | 33580;
  // stwx r10,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r10));
loc_821F1DC0:
  // mr r3,r31
  r3 = r31;
  // bl 0x821edca8
  // (left out: dead in C++)
  { const lifted::Result o = game::TuningScreen::AnimateSlots(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821F32C8 game::TuningScreen::PlaceInteriorMarks: lifted from generated/ by lift.py.
uint64_t f821F32C8(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821F32C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x821F32F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lwz r10,18904(r28)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r28)).save_data_address());
  // lwz r29,0(r3)
  r29 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // ori r9,r11,39200
  r9 = r11 | 39200;
  // addi r11,r10,6120
  r11 = uint64_t(int64_t(int64_t(r10) + 6120));
  // lwzx r10,r10,r9
  r10 = guest::LoadU32(uint32_t(r10) + uint32_t(r9));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r9,204(r11)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(schema::CareerSave(uint32_t(r11) - 6120).garage_head_address()))));
  // b 0x821f331c
  goto loc_821F331C;
loc_821F3314:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_821F331C:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x821f3314
  if (!cr0.eq) goto loc_821F3314;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r9) + 2))));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // mr r3,r29
  r3 = r29;
  // addi r31,r11,212
  r31 = uint64_t(int64_t(int64_t(r11) + 212));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3348, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,12(r31)
  r9 = guest::LoadU32(uint32_t(r31) + 12);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mulli r9,r9,56
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(56))));
  // addi r30,r11,-21056
  r30 = uint64_t(int64_t(int64_t(r11) + -21056));
  // mr r11,r3
  r11 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // addi r10,r30,4
  r10 = uint64_t(int64_t(int64_t(r30) + 4));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r29
  r3 = r29;
  // lwax r9,r9,r30
  r9 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r9) + uint32_t(r30)))));
  // std r9,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r9);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r11)).set_texture_set_bits(temp.u32);
  // lwz r9,12(r31)
  r9 = guest::LoadU32(uint32_t(r31) + 12);
  // mulli r9,r9,56
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(56))));
  // lwax r10,r9,r10
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r9) + uint32_t(r10)))));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r11)).set_live_scene_bits(temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F33A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,12(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 12);
  // addi r9,r30,8
  r9 = uint64_t(int64_t(int64_t(r30) + 8));
  // mulli r10,r10,56
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(56))));
  // mr r11,r3
  r11 = r3;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r29
  r3 = r29;
  // lwax r10,r10,r9
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r10) + uint32_t(r9)))));
  // addi r9,r30,12
  r9 = uint64_t(int64_t(int64_t(r30) + 12));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r11)).set_texture_set_bits(temp.u32);
  // lwz r10,12(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 12);
  // mulli r10,r10,56
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(56))));
  // lwax r10,r10,r9
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r10) + uint32_t(r9)))));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r11)).set_live_scene_bits(temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3404, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,12(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 12);
  // addi r10,r30,16
  r10 = uint64_t(int64_t(int64_t(r30) + 16));
  // mulli r11,r11,56
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(56))));
  // addi r9,r30,20
  r9 = uint64_t(int64_t(int64_t(r30) + 20));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwax r11,r11,r10
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r11) + uint32_t(r10)))));
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // ori r10,r10,35708
  r10 = r10 | 35708;
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r3)).set_texture_set_bits(temp.u32);
  // lwz r11,12(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 12);
  // mulli r11,r11,56
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(56))));
  // lwax r11,r11,r9
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r11) + uint32_t(r9)))));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r3)).set_live_scene_bits(temp.u32);
  // lwzx r11,r28,r10
  r11 = guest::LoadU32(uint32_t(r28) + uint32_t(r10));
  // mr r3,r29
  r3 = r29;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bge cr6,0x821f350c
  if (!cr6.lt) goto loc_821F350C;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3470, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,12(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 12);
  // addi r9,r30,24
  r9 = uint64_t(int64_t(int64_t(r30) + 24));
  // mulli r10,r10,56
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(56))));
  // mr r11,r3
  r11 = r3;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r29
  r3 = r29;
  // lwax r10,r10,r9
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r10) + uint32_t(r9)))));
  // addi r9,r30,28
  r9 = uint64_t(int64_t(int64_t(r30) + 28));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r11)).set_texture_set_bits(temp.u32);
  // lwz r10,12(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 12);
  // mulli r10,r10,56
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(56))));
  // lwax r10,r10,r9
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r10) + uint32_t(r9)))));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r11)).set_live_scene_bits(temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F34CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,12(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 12);
  // addi r10,r30,32
  r10 = uint64_t(int64_t(int64_t(r30) + 32));
  // mulli r11,r11,56
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(56))));
  // lwax r11,r11,r10
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r11) + uint32_t(r10)))));
  // addi r10,r30,36
  r10 = uint64_t(int64_t(int64_t(r30) + 36));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r3)).set_texture_set_bits(temp.u32);
  // lwz r11,12(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 12);
  // mulli r11,r11,56
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(56))));
  // lwax r11,r11,r10
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r11) + uint32_t(r10)))));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // b 0x821f35a8
  goto loc_821F35A8;
loc_821F350C:
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3510, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,12(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 12);
  // addi r9,r30,40
  r9 = uint64_t(int64_t(int64_t(r30) + 40));
  // mulli r10,r10,56
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(56))));
  // mr r11,r3
  r11 = r3;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r29
  r3 = r29;
  // lwax r10,r10,r9
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r10) + uint32_t(r9)))));
  // addi r9,r30,44
  r9 = uint64_t(int64_t(int64_t(r30) + 44));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r11)).set_texture_set_bits(temp.u32);
  // lwz r10,12(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 12);
  // mulli r10,r10,56
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(56))));
  // lwax r10,r10,r9
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r10) + uint32_t(r9)))));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r11)).set_live_scene_bits(temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F356C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,12(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 12);
  // addi r10,r30,48
  r10 = uint64_t(int64_t(int64_t(r30) + 48));
  // mulli r11,r11,56
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(56))));
  // lwax r11,r11,r10
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r11) + uint32_t(r10)))));
  // addi r10,r30,52
  r10 = uint64_t(int64_t(int64_t(r30) + 52));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r3)).set_texture_set_bits(temp.u32);
  // lwz r11,12(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 12);
  // mulli r11,r11,56
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(56))));
  // lwax r11,r11,r10
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r11) + uint32_t(r10)))));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
loc_821F35A8:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r3)).set_live_scene_bits(temp.u32);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_821F3A68 game::TuningScreen::AnimatePaint: lifted from generated/ by lift.py.
lifted::Result f821F3A68(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821F3A68 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-48(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x821F3AAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f3adc
  if (cr6.eq) goto loc_821F3ADC;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F3AD4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f3b60
  if (!cr0.eq) goto loc_821F3B60;
loc_821F3ADC:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f3b08
  if (cr6.eq) goto loc_821F3B08;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F3B00, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f3b60
  if (!cr0.eq) goto loc_821F3B60;
loc_821F3B08:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f3b34
  if (cr6.eq) goto loc_821F3B34;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F3B2C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f3b60
  if (!cr0.eq) goto loc_821F3B60;
loc_821F3B34:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f3c14
  if (cr6.eq) goto loc_821F3C14;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F3B58, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f3c14
  if (cr0.eq) goto loc_821F3C14;
loc_821F3B60:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3B70, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3B78, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // lwa r10,30444(r29)
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_hue_address()))));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-18988(r10)
  temp.u32 = guest::LoadU32(uint32_t(r10) + -18988);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).x_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3BB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3BB8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // lwa r10,30448(r29)
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_saturation_address()))));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lfs f31,-18992(r10)
  temp.u32 = guest::LoadU32(uint32_t(r10) + -18992);
  f31 = double(temp.f32);
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).x_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3BF0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3BF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwa r11,30452(r29)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_value_address()))));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).x_address(), temp.u32);
loc_821F3C14:
  // lwz r11,9708(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).refresh_pending_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821f3fd0
  if (cr6.eq) goto loc_821F3FD0;
  // lwz r11,30464(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_previous_0_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821f3c94
  if (cr6.lt) goto loc_821F3C94;
  // beq cr6,0x821f3c8c
  if (cr6.eq) goto loc_821F3C8C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821f3c84
  if (cr6.lt) goto loc_821F3C84;
  // beq cr6,0x821f3c64
  if (cr6.eq) goto loc_821F3C64;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821f3ca0
  if (!cr6.lt) goto loc_821F3CA0;
  // lwz r11,30472(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_previous_2_address());
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821f3c5c
  if (!cr6.eq) goto loc_821F3C5C;
  // li r4,178
  r4 = uint64_t(int64_t(178));
  // b 0x821f3c9c
  goto loc_821F3C9C;
loc_821F3C5C:
  // li r4,179
  r4 = uint64_t(int64_t(179));
  // b 0x821f3c9c
  goto loc_821F3C9C;
loc_821F3C64:
  // lwz r11,30468(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_previous_1_address());
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821f3c7c
  if (!cr6.eq) goto loc_821F3C7C;
  // li r4,215
  r4 = uint64_t(int64_t(215));
  // b 0x821f3c9c
  goto loc_821F3C9C;
loc_821F3C7C:
  // li r4,216
  r4 = uint64_t(int64_t(216));
  // b 0x821f3c9c
  goto loc_821F3C9C;
loc_821F3C84:
  // li r4,214
  r4 = uint64_t(int64_t(214));
  // b 0x821f3c98
  goto loc_821F3C98;
loc_821F3C8C:
  // li r4,213
  r4 = uint64_t(int64_t(213));
  // b 0x821f3c98
  goto loc_821F3C98;
loc_821F3C94:
  // li r4,212
  r4 = uint64_t(int64_t(212));
loc_821F3C98:
  // mr r3,r31
  r3 = r31;
loc_821F3C9C:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x821F3CA0, r31)); r3 = o.r3; f1 = o.f1; }
loc_821F3CA0:
  // lwz r11,30440(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_row_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821f3f70
  if (cr6.lt) goto loc_821F3F70;
  // beq cr6,0x821f3f18
  if (cr6.eq) goto loc_821F3F18;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821f3ec0
  if (cr6.lt) goto loc_821F3EC0;
  // beq cr6,0x821f3e34
  if (cr6.eq) goto loc_821F3E34;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821f4020
  if (!cr6.lt) goto loc_821F4020;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3CD4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3CDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r10)).alpha_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3CFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3D04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3D0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,68(r10)
  temp.f32 = float(f31);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r10)).alpha_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3D2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3D34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3D3C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).alpha_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3D54, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3D5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3D64, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).alpha_address(), temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3D7C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3D88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3D90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).alpha_address(), temp.u32);
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3DA4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3DAC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // stw r28,80(r3)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).visible_address(), uint32_t(r28));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3DC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3DC8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,80(r3)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).visible_address(), uint32_t(r28));
  // lwz r11,30460(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_buttons_address());
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821f3e08
  if (!cr6.eq) goto loc_821F3E08;
  // li r4,178
  r4 = uint64_t(int64_t(178));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x821F3DE4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,178
  r4 = uint64_t(int64_t(178));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x821F3E00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,178
  r4 = uint64_t(int64_t(178));
  // b 0x821f3fc4
  goto loc_821F3FC4;
loc_821F3E08:
  // li r4,179
  r4 = uint64_t(int64_t(179));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x821F3E10, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,179
  r4 = uint64_t(int64_t(179));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x821F3E2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,179
  r4 = uint64_t(int64_t(179));
  // b 0x821f3fc4
  goto loc_821F3FC4;
loc_821F3E34:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3E44, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3E4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).alpha_address(), temp.u32);
  // lwz r11,30456(r29)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_choice_address());
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821f3e94
  if (!cr6.eq) goto loc_821F3E94;
  // li r4,215
  r4 = uint64_t(int64_t(215));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x821F3E70, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,215
  r4 = uint64_t(int64_t(215));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x821F3E8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,215
  r4 = uint64_t(int64_t(215));
  // b 0x821f3fc4
  goto loc_821F3FC4;
loc_821F3E94:
  // li r4,216
  r4 = uint64_t(int64_t(216));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x821F3E9C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,216
  r4 = uint64_t(int64_t(216));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x821F3EB8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,216
  r4 = uint64_t(int64_t(216));
  // b 0x821f3fc4
  goto loc_821F3FC4;
loc_821F3EC0:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3ED0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3ED8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,214
  r4 = uint64_t(int64_t(214));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r10)).alpha_address(), temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x821F3EF4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,214
  r4 = uint64_t(int64_t(214));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x821F3F10, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,214
  r4 = uint64_t(int64_t(214));
  // b 0x821f3fc4
  goto loc_821F3FC4;
loc_821F3F18:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3F28, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3F30, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,213
  r4 = uint64_t(int64_t(213));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r10)).alpha_address(), temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x821F3F4C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,213
  r4 = uint64_t(int64_t(213));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x821F3F68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,213
  r4 = uint64_t(int64_t(213));
  // b 0x821f3fc4
  goto loc_821F3FC4;
loc_821F3F70:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F3F80, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F3F88, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,212
  r4 = uint64_t(int64_t(212));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r10)).alpha_address(), temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x821F3FA4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,212
  r4 = uint64_t(int64_t(212));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x821F3FC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,212
  r4 = uint64_t(int64_t(212));
loc_821F3FC4:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(LIFTED_ARGS(0x821F3FCC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821f4020
  goto loc_821F4020;
loc_821F3FD0:
  // lwz r11,4(r30)
  r11 = schema::UiMovie(uint32_t(r30)).file_1_bits();
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f3ffc
  if (cr6.eq) goto loc_821F3FFC;
  // lwz r3,0(r30)
  r3 = schema::UiMovie(uint32_t(r30)).file_0_bits();
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(schema::TuningMovieFirstNode(uint32_t(r11)).clip_0_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F3FF4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f4020
  if (!cr0.eq) goto loc_821F4020;
loc_821F3FFC:
  // lwz r11,4(r30)
  r11 = schema::UiMovie(uint32_t(r30)).file_1_bits();
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f4020
  if (cr6.eq) goto loc_821F4020;
  // lwz r3,0(r30)
  r3 = schema::UiMovie(uint32_t(r30)).file_0_bits();
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(schema::TuningMovieFirstNode(uint32_t(r11)).clip_0_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F4020, r31)); r3 = o.r3; f1 = o.f1; }
loc_821F4020:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F4030, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F4038, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // lwz r6,30444(r29)
  r6 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_hue_address());
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x821F404C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F405C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F4064, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // lwz r6,30448(r29)
  r6 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_saturation_address());
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x821F4078, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x821F4088, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x821F4090, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // lwz r6,30452(r29)
  r6 = guest::LoadU32(schema::TuningScreen(uint32_t(r29)).paint_value_address());
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x821F40A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x821e9488
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821E9488, LIFTED_ARGS(0x821F40AC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821F5288 game::TuningScreen::InputPaint: lifted from generated/ by lift.py.
lifted::Result f821F5288(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821F5288 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
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
  r10 = guest::LoadU32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f52e4
  if (cr6.eq) goto loc_821F52E4;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F52DC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5b38
  if (!cr0.eq) goto loc_821F5B38;
loc_821F52E4:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5310
  if (cr6.eq) goto loc_821F5310;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5308, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5b38
  if (!cr0.eq) goto loc_821F5B38;
loc_821F5310:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f533c
  if (cr6.eq) goto loc_821F533C;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5334, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5b08
  if (!cr0.eq) goto loc_821F5B08;
loc_821F533C:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5368
  if (cr6.eq) goto loc_821F5368;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5360, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5b08
  if (!cr0.eq) goto loc_821F5B08;
loc_821F5368:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5394
  if (cr6.eq) goto loc_821F5394;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F538C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f58c0
  if (!cr0.eq) goto loc_821F58C0;
loc_821F5394:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f53c0
  if (cr6.eq) goto loc_821F53C0;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F53B8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f58c0
  if (!cr0.eq) goto loc_821F58C0;
loc_821F53C0:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f53ec
  if (cr6.eq) goto loc_821F53EC;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F53E4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5650
  if (!cr0.eq) goto loc_821F5650;
loc_821F53EC:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5418
  if (cr6.eq) goto loc_821F5418;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5410, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5650
  if (!cr0.eq) goto loc_821F5650;
loc_821F5418:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f55d8
  if (cr6.eq) goto loc_821F55D8;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F543C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f55d8
  if (cr0.eq) goto loc_821F55D8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F544C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30440(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_row_address());
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821f55b4
  if (!cr6.eq) goto loc_821F55B4;
  // lwz r10,30460(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821f54f8
  if (!cr6.eq) goto loc_821F54F8;
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // li r7,18
  r7 = uint64_t(int64_t(18));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stb r9,9667(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).keep_cursor_address(), uint8_t(r9));
  // li r8,12
  r8 = uint64_t(int64_t(12));
  // stw r6,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r6));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stwx r7,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r7));
  // lbz r7,10008(r31)
  r7 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // stw r8,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r8));
  // stb r7,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r7));
  // stw r8,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r8));
  // stw r8,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r8));
  // stw r8,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r8));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r9,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r9));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r9,9652(r31)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r8,0(r11)
  r8 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r9,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r8), so);
  // beq cr6,0x821f54d4
  if (cr6.eq) goto loc_821F54D4;
  // stw r8,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r8));
loc_821F54D4:
  // lis r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r10));
  // li r8,63
  r8 = uint64_t(int64_t(63));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // ori r7,r7,36824
  r7 = r7 | 36824;
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r8,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r8));
  // stwx r6,r31,r7
  guest::StoreU32(uint32_t(r31) + uint32_t(r7), uint32_t(r6));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F54F8:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821f55b4
  if (!cr6.eq) goto loc_821F55B4;
  // lwz r10,30460(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821f55a0
  if (!cr6.eq) goto loc_821F55A0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r7,18
  r7 = uint64_t(int64_t(18));
  // li r8,13
  r8 = uint64_t(int64_t(13));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // stb r9,9668(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).discard_cursor_address(), uint8_t(r9));
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r7));
  // lbz r7,10008(r31)
  r7 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // stw r8,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r8));
  // stb r7,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r7));
  // stw r8,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r8));
  // stw r8,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r8));
  // stw r8,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r8));
  // stw r10,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r9,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r9));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r7,0(r11)
  r7 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r8,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r7), so);
  // beq cr6,0x821f557c
  if (cr6.eq) goto loc_821F557C;
  // stw r7,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r7));
loc_821F557C:
  // lis r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r10));
  // li r7,63
  r7 = uint64_t(int64_t(63));
  // stw r8,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r8));
  // ori r6,r6,36824
  r6 = r6 | 36824;
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r7,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r7));
  // stwx r9,r31,r6
  guest::StoreU32(uint32_t(r31) + uint32_t(r6), uint32_t(r9));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F55A0:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821f55b4
  if (!cr6.eq) goto loc_821F55B4;
  // lwz r10,30460(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821f5b78
  if (cr6.eq) goto loc_821F5B78;
loc_821F55B4:
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r11,30464(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_0_address(), uint32_t(r11));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // lwz r11,30460(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r9,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r9));
  // stw r8,30440(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_row_address(), uint32_t(r8));
  // stw r7,30460(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address(), uint32_t(r7));
  // b 0x821f5640
  goto loc_821F5640;
loc_821F55D8:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5b78
  if (cr6.eq) goto loc_821F5B78;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F55FC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f5b78
  if (cr0.eq) goto loc_821F5B78;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F560C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30440(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_row_address());
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821f5624
  if (!cr6.eq) goto loc_821F5624;
  // lwz r10,30460(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821f5b78
  if (cr6.eq) goto loc_821F5B78;
loc_821F5624:
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // stw r11,30464(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_0_address(), uint32_t(r11));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // lwz r11,30460(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // stw r6,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r6));
  // stw r9,30440(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_row_address(), uint32_t(r9));
  // stw r6,30460(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address(), uint32_t(r6));
loc_821F5640:
  // lwz r10,30456(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_choice_address());
  // stw r11,30472(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_2_address(), uint32_t(r11));
  // stw r10,30468(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_1_address(), uint32_t(r10));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F5650:
  // lwz r11,30440(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_row_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // stw r11,30464(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_0_address(), uint32_t(r11));
  // blt cr6,0x821f5814
  if (cr6.lt) goto loc_821F5814;
  // beq cr6,0x821f5764
  if (cr6.eq) goto loc_821F5764;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821f56c0
  if (cr6.lt) goto loc_821F56C0;
  // beq cr6,0x821f569c
  if (cr6.eq) goto loc_821F569C;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821f5b78
  if (!cr6.lt) goto loc_821F5B78;
  // lwz r11,30460(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // stw r11,30472(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_2_address(), uint32_t(r11));
  // stw r10,30460(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address(), uint32_t(r10));
  // ble cr6,0x821f5b68
  if (!cr6.gt) goto loc_821F5B68;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821F5694:
  // stw r11,30460(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address(), uint32_t(r11));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F569C:
  // lwz r11,30456(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_choice_address());
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // stw r11,30468(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_1_address(), uint32_t(r11));
  // stw r10,30456(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_choice_address(), uint32_t(r10));
  // ble cr6,0x821f5b68
  if (!cr6.gt) goto loc_821F5B68;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821F56B8:
  // stw r11,30456(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_choice_address(), uint32_t(r11));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F56C0:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f56f4
  if (cr6.eq) goto loc_821F56F4;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F56E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f5700
  if (!cr6.lt) goto loc_821F5700;
loc_821F56F4:
  // lwz r11,30452(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_value_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821f5748
  goto loc_821F5748;
loc_821F5700:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5734
  if (cr6.eq) goto loc_821F5734;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5724, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f5740
  if (!cr6.lt) goto loc_821F5740;
loc_821F5734:
  // lwz r11,30452(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_value_address());
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // b 0x821f5748
  goto loc_821F5748;
loc_821F5740:
  // lwz r11,30452(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_value_address());
  // addi r11,r11,5
  r11 = uint64_t(int64_t(int64_t(r11) + 5));
loc_821F5748:
  // stw r11,30452(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_value_address(), uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // ble cr6,0x821f5808
  if (!cr6.gt) goto loc_821F5808;
  // li r11,255
  r11 = uint64_t(int64_t(255));
loc_821F575C:
  // stw r11,30452(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_value_address(), uint32_t(r11));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F5764:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5798
  if (cr6.eq) goto loc_821F5798;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5788, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f57a4
  if (!cr6.lt) goto loc_821F57A4;
loc_821F5798:
  // lwz r11,30448(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821f57ec
  goto loc_821F57EC;
loc_821F57A4:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f57d8
  if (cr6.eq) goto loc_821F57D8;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F57C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f57e4
  if (!cr6.lt) goto loc_821F57E4;
loc_821F57D8:
  // lwz r11,30448(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address());
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // b 0x821f57ec
  goto loc_821F57EC;
loc_821F57E4:
  // lwz r11,30448(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address());
  // addi r11,r11,5
  r11 = uint64_t(int64_t(int64_t(r11) + 5));
loc_821F57EC:
  // stw r11,30448(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address(), uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // ble cr6,0x821f5808
  if (!cr6.gt) goto loc_821F5808;
  // li r11,255
  r11 = uint64_t(int64_t(255));
loc_821F5800:
  // stw r11,30448(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address(), uint32_t(r11));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F5808:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F5810, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F5814:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F581C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5850
  if (cr6.eq) goto loc_821F5850;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5840, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f585c
  if (!cr6.lt) goto loc_821F585C;
loc_821F5850:
  // lwz r11,30444(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821f58a4
  goto loc_821F58A4;
loc_821F585C:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5890
  if (cr6.eq) goto loc_821F5890;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5880, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f589c
  if (!cr6.lt) goto loc_821F589C;
loc_821F5890:
  // lwz r11,30444(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address());
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // b 0x821f58a4
  goto loc_821F58A4;
loc_821F589C:
  // lwz r11,30444(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address());
  // addi r11,r11,5
  r11 = uint64_t(int64_t(int64_t(r11) + 5));
loc_821F58A4:
  // stw r11,30444(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address(), uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,359
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 359, so);
  // ble cr6,0x821f5b78
  if (!cr6.gt) goto loc_821F5B78;
  // addi r11,r11,-360
  r11 = uint64_t(int64_t(int64_t(r11) + -360));
loc_821F58B8:
  // stw r11,30444(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address(), uint32_t(r11));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F58C0:
  // lwz r11,30440(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_row_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // stw r11,30464(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_0_address(), uint32_t(r11));
  // blt cr6,0x821f5a60
  if (cr6.lt) goto loc_821F5A60;
  // beq cr6,0x821f59c0
  if (cr6.eq) goto loc_821F59C0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821f5920
  if (cr6.lt) goto loc_821F5920;
  // beq cr6,0x821f5904
  if (cr6.eq) goto loc_821F5904;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821f5b78
  if (!cr6.lt) goto loc_821F5B78;
  // lwz r11,30460(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30472(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_2_address(), uint32_t(r11));
  // stw r10,30460(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address(), uint32_t(r10));
  // bge 0x821f5b68
  if (!cr0.lt) goto loc_821F5B68;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821f5694
  goto loc_821F5694;
loc_821F5904:
  // lwz r11,30456(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_choice_address());
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30468(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_1_address(), uint32_t(r11));
  // stw r10,30456(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_choice_address(), uint32_t(r10));
  // bge 0x821f5b68
  if (!cr0.lt) goto loc_821F5B68;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821f56b8
  goto loc_821F56B8;
loc_821F5920:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5954
  if (cr6.eq) goto loc_821F5954;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5944, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f5960
  if (!cr6.lt) goto loc_821F5960;
loc_821F5954:
  // lwz r11,30452(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_value_address());
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x821f59a8
  goto loc_821F59A8;
loc_821F5960:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5994
  if (cr6.eq) goto loc_821F5994;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5984, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f59a0
  if (!cr6.lt) goto loc_821F59A0;
loc_821F5994:
  // lwz r11,30452(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_value_address());
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // b 0x821f59a8
  goto loc_821F59A8;
loc_821F59A0:
  // lwz r11,30452(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_value_address());
  // addi r11,r11,-5
  r11 = uint64_t(int64_t(int64_t(r11) + -5));
loc_821F59A8:
  // stw r11,30452(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_value_address(), uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821f5808
  if (!cr6.lt) goto loc_821F5808;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821f575c
  goto loc_821F575C;
loc_821F59C0:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f59f4
  if (cr6.eq) goto loc_821F59F4;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F59E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f5a00
  if (!cr6.lt) goto loc_821F5A00;
loc_821F59F4:
  // lwz r11,30448(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address());
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x821f5a48
  goto loc_821F5A48;
loc_821F5A00:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5a34
  if (cr6.eq) goto loc_821F5A34;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5A24, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f5a40
  if (!cr6.lt) goto loc_821F5A40;
loc_821F5A34:
  // lwz r11,30448(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address());
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // b 0x821f5a48
  goto loc_821F5A48;
loc_821F5A40:
  // lwz r11,30448(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address());
  // addi r11,r11,-5
  r11 = uint64_t(int64_t(int64_t(r11) + -5));
loc_821F5A48:
  // stw r11,30448(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_saturation_address(), uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821f5808
  if (!cr6.lt) goto loc_821F5808;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821f5800
  goto loc_821F5800;
loc_821F5A60:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F5A68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5a9c
  if (cr6.eq) goto loc_821F5A9C;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5A8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f5aa8
  if (!cr6.lt) goto loc_821F5AA8;
loc_821F5A9C:
  // lwz r11,30444(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address());
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x821f5af0
  goto loc_821F5AF0;
loc_821F5AA8:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5adc
  if (cr6.eq) goto loc_821F5ADC;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5ACC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f5ae8
  if (!cr6.lt) goto loc_821F5AE8;
loc_821F5ADC:
  // lwz r11,30444(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address());
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // b 0x821f5af0
  goto loc_821F5AF0;
loc_821F5AE8:
  // lwz r11,30444(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address());
  // addi r11,r11,-5
  r11 = uint64_t(int64_t(int64_t(r11) + -5));
loc_821F5AF0:
  // stw r11,30444(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_hue_address(), uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge 0x821f5b78
  if (!cr0.lt) goto loc_821F5B78;
  // addi r11,r11,360
  r11 = uint64_t(int64_t(int64_t(r11) + 360));
  // b 0x821f58b8
  goto loc_821F58B8;
loc_821F5B08:
  // lwz r11,30456(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_choice_address());
  // lwz r10,30460(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // stw r11,30468(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_1_address(), uint32_t(r11));
  // lwz r11,30440(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_row_address());
  // stw r10,30472(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_2_address(), uint32_t(r10));
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // stw r11,30464(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_0_address(), uint32_t(r11));
  // stw r10,30440(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_row_address(), uint32_t(r10));
  // ble cr6,0x821f5b68
  if (!cr6.gt) goto loc_821F5B68;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // b 0x821f5b60
  goto loc_821F5B60;
loc_821F5B38:
  // lwz r11,30456(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_choice_address());
  // lwz r10,30460(r31)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_buttons_address());
  // stw r11,30468(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_1_address(), uint32_t(r11));
  // lwz r11,30440(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).paint_row_address());
  // stw r10,30472(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_2_address(), uint32_t(r10));
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30464(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_previous_0_address(), uint32_t(r11));
  // stw r10,30440(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_row_address(), uint32_t(r10));
  // bge 0x821f5b68
  if (!cr0.lt) goto loc_821F5B68;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821F5B60:
  // stw r11,30440(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).paint_row_address(), uint32_t(r11));
  // b 0x821f5b78
  goto loc_821F5B78;
loc_821F5B68:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F5B70, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
loc_821F5B78:
  // mr r3,r31
  r3 = r31;
  // bl 0x821f3a68
  // (left out: dead in C++)
  { const lifted::Result o = game::TuningScreen::AnimatePaint(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821F5B98 game::TuningScreen::InputInteriorModes: lifted from generated/ by lift.py.
lifted::Result f821F5B98(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821F5B98 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
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
  r10 = guest::LoadU32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5bec
  if (cr6.eq) goto loc_821F5BEC;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5BE4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5f10
  if (!cr0.eq) goto loc_821F5F10;
loc_821F5BEC:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5c18
  if (cr6.eq) goto loc_821F5C18;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5C10, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5f10
  if (!cr0.eq) goto loc_821F5F10;
loc_821F5C18:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5c44
  if (cr6.eq) goto loc_821F5C44;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5C3C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5ecc
  if (!cr0.eq) goto loc_821F5ECC;
loc_821F5C44:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5c70
  if (cr6.eq) goto loc_821F5C70;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5C68, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5ecc
  if (!cr0.eq) goto loc_821F5ECC;
loc_821F5C70:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5dc4
  if (cr6.eq) goto loc_821F5DC4;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5C94, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f5dc4
  if (cr0.eq) goto loc_821F5DC4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F5CA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // ori r11,r11,33540
  r11 = r11 | 33540;
  // stw r28,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r28));
  // lwzx r11,r31,r11
  r11 = guest::LoadU32(uint32_t(r31) + uint32_t(r11));
  // stw r28,9716(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).command_first_run_address(), uint32_t(r28));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f5d3c
  if (!cr0.eq) goto loc_821F5D3C;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // ori r11,r11,33548
  r11 = r11 | 33548;
  // li r9,53
  r9 = uint64_t(int64_t(53));
  // li r10,54
  r10 = uint64_t(int64_t(54));
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // stwx r30,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r30));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r9,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r9));
  // lbz r9,10008(r31)
  r9 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stw r10,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r10));
  // stb r9,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r9));
  // stw r10,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
  // stw r29,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r29));
  // stw r29,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r29));
  // stw r30,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r30));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r9,9652(r31)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // beq cr6,0x821f5d34
  if (cr6.eq) goto loc_821F5D34;
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821F5D34:
  // li r10,55
  r10 = uint64_t(int64_t(55));
  // b 0x821f5eb8
  goto loc_821F5EB8;
loc_821F5D3C:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821f5f5c
  if (!cr6.eq) goto loc_821F5F5C;
  // mr r3,r31
  r3 = r31;
  // bl 0x821f32c8
  // (left out: dead in C++)
  r3 = game::TuningScreen::PlaceInteriorMarks(r1, r13, r3);
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // ori r11,r11,33564
  r11 = r11 | 33564;
  // li r9,53
  r9 = uint64_t(int64_t(53));
  // li r10,56
  r10 = uint64_t(int64_t(56));
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // stwx r30,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r30));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r9,r11,r31
  guest::StoreU32(uint32_t(r11) + uint32_t(r31), uint32_t(r9));
  // lbz r9,10008(r31)
  r9 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stw r10,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r10));
  // stb r9,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r9));
  // stw r10,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
  // stw r29,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r29));
  // stw r29,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r29));
  // stw r30,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r30));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r9,9652(r31)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // beq cr6,0x821f5dbc
  if (cr6.eq) goto loc_821F5DBC;
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821F5DBC:
  // li r10,57
  r10 = uint64_t(int64_t(57));
  // b 0x821f5eb8
  goto loc_821F5EB8;
loc_821F5DC4:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f5f5c
  if (cr6.eq) goto loc_821F5F5C;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821F5DE8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f5f5c
  if (cr0.eq) goto loc_821F5F5C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F5DF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(schema::TuningScreen(uint32_t(r31)).history_depth_address());
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // addi r10,r11,255
  r10 = uint64_t(int64_t(int64_t(r11) + 255));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // clrlwi r10,r10,24
  r10 = uint32_t(r10) & 0xFF;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r9,r10
  r9 = r10;
  // stw r28,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r28));
  // mr r3,r31
  r3 = r31;
  // addi r9,r9,2439
  r9 = uint64_t(int64_t(int64_t(r9) + 2439));
  // stb r10,10008(r31)
  guest::StoreU8(schema::TuningScreen(uint32_t(r31)).history_depth_address(), uint8_t(r10));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r9,r31
  r10 = guest::LoadU32(uint32_t(r9) + uint32_t(r31));
  // stw r10,9652(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).state_address(), uint32_t(r10));
  // stw r29,20(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).step_address(), uint32_t(r29));
  // stw r29,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r29));
  // stw r30,24(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).started_address(), uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r10));
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
  // bl 0x821ccdb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CCDB0, LIFTED_ARGS(0x821F5E50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x821ccd38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821CCD38, LIFTED_ARGS(0x821F5E5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).category_menu_address());
  // addi r10,r27,-1
  r10 = uint64_t(int64_t(int64_t(r27) + -1));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // stw r30,12(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_first_item_bits(uint32_t(r30));
  // stw r10,16(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_last_item_bits(uint32_t(r10));
  // stw r28,20(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_wraps_bits(uint32_t(r28));
  // stw r30,8(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_moved_bits(uint32_t(r30));
  // stw r3,0(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_cursor_bits(uint32_t(r3));
  // stw r3,4(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_bits(uint32_t(r3));
  // stw r30,44(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_list_wrapped_bits(uint32_t(r30));
  // stw r30,56(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_first_row_bits(uint32_t(r30));
  // stw r9,60(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_rows_shown_bits(uint32_t(r9));
  // stw r30,36(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_side_bits(uint32_t(r30));
  // stw r30,64(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_player_bits(uint32_t(r30));
  // stw r30,24(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_up_down_locked_bits(uint32_t(r30));
  // stw r30,28(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_left_right_steps_bits(uint32_t(r30));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).queue_address());
  // lwz r9,9652(r31)
  r9 = guest::LoadU32(schema::TuningScreen(uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningCommandQueue(uint32_t(r11)).page_address());
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // beq cr6,0x821f5eb4
  if (cr6.eq) goto loc_821F5EB4;
  // stw r10,8(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821F5EB4:
  // li r10,2
  r10 = uint64_t(int64_t(2));
loc_821F5EB8:
  // stw r10,12(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r29,16(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).next_address(), uint32_t(r29));
  // stw r9,4(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).resume_address(), uint32_t(r9));
  // stw r29,0(r11)
  guest::StoreU32(schema::TuningCommandQueue(uint32_t(r11)).page_address(), uint32_t(r29));
  // b 0x821f5f5c
  goto loc_821F5F5C;
loc_821F5ECC:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F5ED4, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // addi r11,r11,-31996
  r11 = uint64_t(int64_t(int64_t(r11) + -31996));
  // stw r28,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r28));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r11) - 33540).interior_mode_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33540).interior_mode_address(), uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ble cr6,0x821f5f50
  if (!cr6.gt) goto loc_821F5F50;
  // lis r9,0
  r9 = uint64_t(int64_t(0));
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33540).interior_mode_address(), uint32_t(r10));
  // ori r9,r9,33544
  r9 = r9 | 33544;
  // stwx r28,r31,r9
  guest::StoreU32(uint32_t(r31) + uint32_t(r9), uint32_t(r28));
  // b 0x821f5f5c
  goto loc_821F5F5C;
loc_821F5F10:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x821F5F18, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // addi r11,r11,-31996
  r11 = uint64_t(int64_t(int64_t(r11) + -31996));
  // stw r28,9708(r31)
  guest::StoreU32(schema::TuningScreen(uint32_t(r31)).refresh_pending_address(), uint32_t(r28));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(schema::TuningScreen(uint32_t(r11) - 33540).interior_mode_address());
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r10,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33540).interior_mode_address(), uint32_t(r10));
  // bge 0x821f5f4c
  if (!cr0.lt) goto loc_821F5F4C;
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // stw r28,0(r11)
  guest::StoreU32(schema::TuningScreen(uint32_t(r11) - 33540).interior_mode_address(), uint32_t(r28));
  // ori r10,r10,33544
  r10 = r10 | 33544;
  // stwx r28,r31,r10
  guest::StoreU32(uint32_t(r31) + uint32_t(r10), uint32_t(r28));
  // b 0x821f5f5c
  goto loc_821F5F5C;
loc_821F5F4C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_821F5F50:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,33544
  r11 = r11 | 33544;
  // stwx r10,r31,r11
  guest::StoreU32(uint32_t(r31) + uint32_t(r11), uint32_t(r10));
loc_821F5F5C:
  // mr r3,r31
  r3 = r31;
  // bl 0x821eba00
  // (left out: dead in C++)
  { const lifted::Result o = game::TuningScreen::AnimateInteriorModes(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8220B9E8 game::RaceScreens::RunMovie18: lifted from generated/ by lift.py.
lifted::Result f8220B9E8(uint64_t r1, uint64_t r13, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8220B9E8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,18
  r11 = uint64_t(int64_t(18));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8220BA0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8220BA18, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8220CE68 game::PauseMenu::SetArrowB: lifted from generated/ by lift.py.
uint64_t f8220CE68(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8220CE68 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r31,r3
  r31 = r3;
  // lwz r11,-31536(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31536);
  // lwz r3,0(r11)
  r3 = schema::PauseMenuScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8220CE98, r31)); r3 = o.r3; f1 = o.f1; }
  // extsw r11,r31
  r11 = uint64_t(int64_t(int32_t(r31)));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8220CEB4, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).y_address(), temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8220D1D8 game::PauseMenu::SetArrowC: lifted from generated/ by lift.py.
uint64_t f8220D1D8(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8220D1D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r31,r3
  r31 = r3;
  // lwz r11,-31536(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31536);
  // lwz r3,0(r11)
  r3 = schema::PauseMenuScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8220D208, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8220D210, r31)); r3 = o.r3; f1 = o.f1; }
  // extsw r11,r31
  r11 = uint64_t(int64_t(int32_t(r31)));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8220D22C, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).y_address(), temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8220DF30 game::PauseMenu::MoveVolume: lifted from generated/ by lift.py.
lifted::Result f8220DF30(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8220DF30 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r26,r3
  r26 = r3;
  // lwz r11,-31536(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31536);
  // lwz r10,0(r26)
  r10 = guest::LoadU32(schema::PauseMenuStarted(uint32_t(r26)).started_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r30,0(r11)
  r30 = schema::PauseMenuScreen(uint32_t(r11)).movie_bits();
  // bne cr6,0x8220e0e0
  if (!cr6.eq) goto loc_8220E0E0;
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31524(r28)
  r3 = guest::LoadU32(uint32_t(r28) + -31524);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::GetPreviousSelection(LIFTED_ARGS(0x8220DF64, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r3,-31524(r28)
  r3 = guest::LoadU32(uint32_t(r28) + -31524);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8220DF70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // blt cr6,0x8220dfe8
  if (cr6.lt) goto loc_8220DFE8;
  // beq cr6,0x8220dfe0
  if (cr6.eq) goto loc_8220DFE0;
  // cmplwi cr6,r29,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 3, so);
  // bge cr6,0x8220dff8
  if (!cr6.lt) goto loc_8220DFF8;
  // lwz r11,-31532(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31532);
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r11,144(r11)
  r11 = schema::PauseMenuData(uint32_t(r11)).last_item_ok_bits();
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8220dfac
  if (cr6.eq) goto loc_8220DFAC;
  // li r5,3
  r5 = uint64_t(int64_t(3));
loc_8220DFAC:
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x8220DFB0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x8220dfc0
  if (!cr6.eq) goto loc_8220DFC0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8220dfcc
  goto loc_8220DFCC;
loc_8220DFC0:
  // cmpwi cr6,r27,1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 1, so);
  // bne cr6,0x8220dfd0
  if (!cr6.eq) goto loc_8220DFD0;
  // li r3,40
  r3 = uint64_t(int64_t(40));
loc_8220DFCC:
  // bl 0x8220ce68
  // (left out: dead in C++)
  r3 = game::PauseMenu::SetArrowB(r1, r13, r3);
loc_8220DFD0:
  // lwz r11,-31532(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31532);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,144(r11)
  schema::PauseMenuData(uint32_t(r11)).set_last_item_ok_bits(uint32_t(r10));
  // b 0x8220dff8
  goto loc_8220DFF8;
loc_8220DFE0:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // b 0x8220dfec
  goto loc_8220DFEC;
loc_8220DFE8:
  // li r5,1
  r5 = uint64_t(int64_t(1));
loc_8220DFEC:
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x8220DFF8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8220DFF8:
  // cmplwi cr6,r27,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 1, so);
  // blt cr6,0x8220e04c
  if (cr6.lt) goto loc_8220E04C;
  // beq cr6,0x8220e044
  if (cr6.eq) goto loc_8220E044;
  // cmplwi cr6,r27,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 3, so);
  // bge cr6,0x8220e074
  if (!cr6.lt) goto loc_8220E074;
  // lwz r11,-31532(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31532);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // lwz r11,144(r11)
  r11 = schema::PauseMenuData(uint32_t(r11)).last_item_ok_bits();
  // mr r3,r30
  r3 = r30;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8220e03c
  if (!cr6.eq) goto loc_8220E03C;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // b 0x8220e068
  goto loc_8220E068;
loc_8220E03C:
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // b 0x8220e068
  goto loc_8220E068;
loc_8220E044:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // b 0x8220e050
  goto loc_8220E050;
loc_8220E04C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
loc_8220E050:
  // mr r3,r30
  r3 = r30;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_8220E068:
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x8220E06C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x8220E074, r31)); r3 = o.r3; f1 = o.f1; }
loc_8220E074:
  // li r4,94
  r4 = uint64_t(int64_t(94));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8220E080, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,95
  r4 = uint64_t(int64_t(95));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8220E08C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // beq cr6,0x8220e0d8
  if (cr6.eq) goto loc_8220E0D8;
  // cmpwi cr6,r27,2
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 2, so);
  // beq cr6,0x8220e0d8
  if (cr6.eq) goto loc_8220E0D8;
  // lwz r11,-31524(r28)
  r11 = guest::LoadU32(uint32_t(r28) + -31524);
  // lwz r11,24(r11)
  r11 = schema::MenuList(uint32_t(r11)).move_bits();
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8220e0b4
  if (!cr6.eq) goto loc_8220E0B4;
  // li r4,95
  r4 = uint64_t(int64_t(95));
  // b 0x8220e0c0
  goto loc_8220E0C0;
loc_8220E0B4:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x8220e0d8
  if (!cr6.eq) goto loc_8220E0D8;
  // li r4,94
  r4 = uint64_t(int64_t(94));
loc_8220E0C0:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8220E0D8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8220E0D8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  guest::StoreU32(schema::PauseMenuStarted(uint32_t(r26)).started_address(), uint32_t(r11));
loc_8220E0E0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8220E0EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8220E0F8 game::PauseMenu::SetVolume: lifted from generated/ by lift.py.
lifted::Result f8220E0F8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8220E0F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r28,r3
  r28 = r3;
  // lwz r11,-31536(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31536);
  // lwz r10,0(r28)
  r10 = guest::LoadU32(schema::PauseMenuStarted(uint32_t(r28)).started_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r30,0(r11)
  r30 = schema::PauseMenuScreen(uint32_t(r11)).movie_bits();
  // bne cr6,0x8220e1e0
  if (!cr6.eq) goto loc_8220E1E0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31524(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -31524);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8220E12C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bne cr6,0x8220e194
  if (!cr6.eq) goto loc_8220E194;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31532(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31532);
  // lwz r11,144(r11)
  r11 = schema::PauseMenuData(uint32_t(r11)).last_item_ok_bits();
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8220e160
  if (!cr6.eq) goto loc_8220E160;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x8220E158, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // b 0x8220e16c
  goto loc_8220E16C;
loc_8220E160:
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x8220E168, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,3
  r5 = uint64_t(int64_t(3));
loc_8220E16C:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x8220E188, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x8220E190, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8220e1d8
  goto loc_8220E1D8;
loc_8220E194:
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8220E19C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8220E1B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // mr r3,r31
  r3 = r31;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31532(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31532);
  // lwz r4,52(r11)
  r4 = schema::PauseMenuData(uint32_t(r11)).music_volume_bits();
  // bl 0x8220d040
  // (left out: dead in C++)
  { const lifted::Result o = game::PauseMenu::ShowVolume(LIFTED_ARGS(0x8220E1C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31532(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31532);
  // mr r3,r29
  r3 = r29;
  // lwz r4,56(r11)
  r4 = schema::PauseMenuData(uint32_t(r11)).effects_volume_bits();
  // bl 0x8220d040
  // (left out: dead in C++)
  { const lifted::Result o = game::PauseMenu::ShowVolume(LIFTED_ARGS(0x8220E1D8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8220E1D8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(schema::PauseMenuStarted(uint32_t(r28)).started_address(), uint32_t(r11));
loc_8220E1E0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8220E1EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822116A0: lifted from generated/ by lift.py.
uint64_t f822116A0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822116A0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemSet(LIFTED_ARGS(0x822116CC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x822a5b78
  // (left out: dead in C++)
  { const lifted::Result o = game::Texture::GetLevelDesc(LIFTED_ARGS(0x822116DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 104);
  // lwz r10,108(r1)
  r10 = guest::LoadU32(uint32_t(r1) + 108);
  // stw r30,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r30));
  // stw r11,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r10,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r10));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82211768 game::RivalLogo::ReleaseXpr: lifted from generated/ by lift.py.
lifted::Result f82211768(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82211768 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // stw r11,0(r31)
  guest::StoreU32(schema::RivalLogoAt20(uint32_t(r31)).unknown_0_address(), uint32_t(r11));
  // bl 0x825970a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::Release_825970A8(LIFTED_ARGS(0x8221178C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r10,28(r31)
  guest::StoreU32(schema::RivalLogoAt20(uint32_t(r31)).unknown_28_address(), uint32_t(r10));
  // stw r11,56(r31)
  guest::StoreU32(schema::RivalLogoAt20(uint32_t(r31)).unknown_56_address(), uint32_t(r11));
  // stw r11,60(r31)
  guest::StoreU32(schema::RivalLogoAt20(uint32_t(r31)).unknown_60_address(), uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822117B8: lifted from generated/ by lift.py.
lifted::Result f822117B8(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822117B8 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r6
  r31 = r6;
  // addi r30,r31,4
  r30 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r3,r30
  r3 = r30;
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // ori r11,r11,8
  r11 = r11 | 8;
  // stw r11,28(r31)
  guest::StoreU32(uint32_t(r31) + 28, uint32_t(r11));
  // bl 0x82596f68
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::Create(LIFTED_ARGS(0x822117E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x82597118
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::GetTexture(LIFTED_ARGS(0x822117F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r31,32
  r3 = uint64_t(int64_t(int64_t(r31) + 32));
  // bl 0x822116a0
  // (left out: dead in C++)
  r3 = f822116A0(r1, r13, r3, r4);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x82597118
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::GetTexture(LIFTED_ARGS(0x8221180C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r31,44
  r3 = uint64_t(int64_t(int64_t(r31) + 44));
  // bl 0x822116a0
  // (left out: dead in C++)
  r3 = f822116A0(r1, r13, r3, r4);
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // rlwinm r11,r11,0,29,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFF7;
  // stw r11,28(r31)
  guest::StoreU32(uint32_t(r31) + 28, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // ori r11,r11,16
  r11 = r11 | 16;
  // stw r11,28(r31)
  guest::StoreU32(uint32_t(r31) + 28, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82211878 game::RivalLogo::LoadXpr: lifted from generated/ by lift.py.
lifted::Result f82211878(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82211878 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32223
  r11 = uint64_t(int64_t(-2111766528));
  // mr r3,r4
  r3 = r4;
  // addi r5,r11,6072
  r5 = uint64_t(int64_t(int64_t(r11) + 6072));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r11,28(r31)
  r11 = guest::LoadU32(schema::RivalLogoAt20_2(uint32_t(r31)).unknown_28_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r31
  r6 = r31;
  // ori r11,r11,4
  r11 = r11 | 4;
  // stw r11,28(r31)
  guest::StoreU32(schema::RivalLogoAt20_2(uint32_t(r31)).unknown_28_address(), uint32_t(r11));
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r4,20788(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 20788);
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = game::Archive::Load(LIFTED_ARGS(0x822118BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r31)
  r11 = guest::LoadU32(schema::RivalLogoAt20_2(uint32_t(r31)).unknown_28_address());
  // rlwinm r11,r11,0,30,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFFB;
  // stw r11,28(r31)
  guest::StoreU32(schema::RivalLogoAt20_2(uint32_t(r31)).unknown_28_address(), uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82211CE0: lifted from generated/ by lift.py.
void f82211CE0() {
  ITC_NATIVE_RUNNING("sub_82211CE0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,-31492(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31492);
  // stw r10,36(r11)
  schema::RaceDialogData(uint32_t(r11)).set_close_asked_bits(uint32_t(r10));
  // blr
  return;
}

// sub_82211CF8: lifted from generated/ by lift.py.
lifted::Result f82211CF8(uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82211CF8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x82292710
  { const lifted::Result o = lifted::Call(0x82292710, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82211D00: lifted from generated/ by lift.py.
lifted::Result f82211D00(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82211D00 (lifted)");
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // b 0x82212888
  { const lifted::Result o = game::RaceHud::StopCountdown(r1, r13, r3, r7, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82211E90: lifted from generated/ by lift.py.
uint64_t f82211E90(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82211E90 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // mr r31,r30
  r31 = r30;
  // lwz r11,-31492(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31492);
  // lwz r11,32(r11)
  r11 = schema::RaceDialogData(uint32_t(r11)).showing_bits();
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82211ec4
  if (!cr6.eq) goto loc_82211EC4;
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82211EC4:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,19984(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 19984);
  // lwz r10,340(r11)
  r10 = guest::LoadU32(schema::Kind6Menu(uint32_t(r11)).input_on_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82211ee0
  if (!cr6.eq) goto loc_82211EE0;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r30,344(r11)
  guest::StoreU32(schema::Kind6Menu(uint32_t(r11)).upload_request_address(), uint32_t(r30));
loc_82211EE0:
  // bl 0x8221abe0
  // (left out: dead in C++)
  r3 = game::AfterRaceMenu::NotOnList();
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82211efc
  if (!cr0.eq) goto loc_82211EFC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,-31468(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31468);
  // stw r30,20(r11)
  schema::AfterRaceMenuData(uint32_t(r11)).set_unknown_20_bits(uint32_t(r30));
loc_82211EFC:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82211F18: lifted from generated/ by lift.py.
void f82211F18() {
  ITC_NATIVE_RUNNING("sub_82211F18 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lwz r11,-31468(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31468);
  // stw r10,20(r11)
  schema::AfterRaceMenuData(uint32_t(r11)).set_unknown_20_bits(uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,19984(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 19984);
  // stw r9,352(r11)
  guest::StoreU32(schema::Kind6Menu(uint32_t(r11)).reset_request_address(), uint32_t(r9));
  // blr
  return;
}

// sub_822125B8: lifted from generated/ by lift.py.
uint64_t f822125B8(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822125B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  guest::StoreU64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822125DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r5,21
  r5 = uint64_t(int64_t(21));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // stw r30,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r30));
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822125F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r30,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r30));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82212608, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,68(r3)
  temp.f32 = float(f31);
  schema::UiMovie(uint32_t(r3)).store_at(68, 4, temp.u32);
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82212624, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r11,104(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(104, 4));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x8221265c
  if (!cr6.gt) goto loc_8221265C;
loc_82212634:
  // lwz r11,100(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(100, 4));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
  // add r5,r11,r30
  r5 = r11 + r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82212648, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,68(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  schema::UiMovie(uint32_t(r3)).store_at(68, 4, temp.u32);
  // lwz r11,104(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(104, 4));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x82212634
  if (cr6.lt) goto loc_82212634;
loc_8221265C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_822126C8: lifted from generated/ by lift.py.
uint64_t f822126C8(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822126C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
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
  // lwz r3,620(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_620_bits();
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822126F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,620(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_620_bits();
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(LIFTED_ARGS(0x82212704, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82212868: lifted from generated/ by lift.py.
lifted::Result f82212868(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82212868 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,620(r3)
  r3 = schema::RaceHudAt16_3(uint32_t(r3)).unknown_620_bits();
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x8211df20
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82212888 game::RaceHud::StopCountdown: lifted from generated/ by lift.py.
lifted::Result f82212888(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82212888 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,612(r11)
  r3 = schema::RaceHud(uint32_t(r11)).countdown_bits();
  // stw r10,436(r11)
  schema::RaceHud(uint32_t(r11)).set_countdown_running_bits(uint32_t(r10));
  // b 0x8211de38
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82212958: lifted from generated/ by lift.py.
lifted::Result f82212958(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82212958 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,456(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_456_bits();
  // lwz r3,604(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_604_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82212984
  if (cr6.eq) goto loc_82212984;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82212980, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82212988
  goto loc_82212988;
loc_82212984:
  // bl 0x8211d280
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Draw_8211D280(LIFTED_ARGS(0x82212988, r31)); r3 = o.r3; f1 = o.f1; }
loc_82212988:
  // lwz r3,636(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_636_bits();
  // bl 0x8211d280
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Draw_8211D280(LIFTED_ARGS(0x82212990, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82212A38: lifted from generated/ by lift.py.
lifted::Result f82212A38(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82212A38 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,612(r3)
  r3 = schema::RaceHudAt16_3(uint32_t(r3)).unknown_612_bits();
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x8211e1b8
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82212AE8: lifted from generated/ by lift.py.
lifted::Result f82212AE8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82212AE8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,620(r3)
  r3 = schema::RaceHudAt16_3(uint32_t(r3)).unknown_620_bits();
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x8211e1b8
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82213860: lifted from generated/ by lift.py.
uint64_t f82213860(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82213860 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r25,r4
  r25 = r4;
  // mr r28,r3
  r28 = r3;
  // mr r27,r5
  r27 = r5;
  // lhz r11,0(r25)
  r11 = guest::LoadU16(schema::RaceHudPackedTime(uint32_t(r25)).sign_minutes_address());
  // clrlwi r26,r11,17
  r26 = uint32_t(r11) & 0x7FFF;
  // mr r31,r26
  r31 = r26;
  // cmpwi cr6,r26,59
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 59, so);
  // ble cr6,0x82213890
  if (!cr6.gt) goto loc_82213890;
  // li r31,59
  r31 = uint64_t(int64_t(59));
loc_82213890:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // li r29,10
  r29 = uint64_t(int64_t(10));
loc_82213898:
  // divw r11,r31,r29
  r11 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // lwz r3,632(r28)
  r3 = schema::RaceHud(uint32_t(r28)).splits_bits();
  // add r5,r30,r27
  r5 = r30 + r27;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r24,r11,1
  r24 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822138B8, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // divw r31,r31,r29
  r31 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // stw r24,108(r3)
  schema::UiMovie(uint32_t(r3)).store_at(108, 4, uint32_t(r24));
  // bge 0x82213898
  if (!cr0.lt) goto loc_82213898;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(schema::RaceHudPackedTime(uint32_t(r25)).packed_address());
  // cmpwi cr6,r26,59
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 59, so);
  // rlwinm r31,r11,22,26,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 22) & 0x3F;
  // ble cr6,0x822138dc
  if (!cr6.gt) goto loc_822138DC;
  // li r31,59
  r31 = uint64_t(int64_t(59));
loc_822138DC:
  // li r30,3
  r30 = uint64_t(int64_t(3));
loc_822138E0:
  // divw r11,r31,r29
  r11 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // lwz r3,632(r28)
  r3 = schema::RaceHud(uint32_t(r28)).splits_bits();
  // add r5,r30,r27
  r5 = r30 + r27;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r24,r11,1
  r24 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82213900, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // divw r31,r31,r29
  r31 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // stw r24,108(r3)
  schema::UiMovie(uint32_t(r3)).store_at(108, 4, uint32_t(r24));
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // bge cr6,0x822138e0
  if (!cr6.lt) goto loc_822138E0;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(schema::RaceHudPackedTime(uint32_t(r25)).packed_address());
  // cmpwi cr6,r26,59
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 59, so);
  // clrlwi r31,r11,22
  r31 = uint32_t(r11) & 0x3FF;
  // ble cr6,0x82213928
  if (!cr6.gt) goto loc_82213928;
  // li r31,999
  r31 = uint64_t(int64_t(999));
loc_82213928:
  // li r30,6
  r30 = uint64_t(int64_t(6));
loc_8221392C:
  // divw r11,r31,r29
  r11 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // lwz r3,632(r28)
  r3 = schema::RaceHud(uint32_t(r28)).splits_bits();
  // add r5,r30,r27
  r5 = r30 + r27;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r26,r11,1
  r26 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221394C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // divw r31,r31,r29
  r31 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // stw r26,108(r3)
  schema::UiMovie(uint32_t(r3)).store_at(108, 4, uint32_t(r26));
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // bge cr6,0x8221392c
  if (!cr6.lt) goto loc_8221392C;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
}

// sub_82213968: lifted from generated/ by lift.py.
uint64_t f82213968(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82213968 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r25,r4
  r25 = r4;
  // mr r28,r3
  r28 = r3;
  // mr r27,r5
  r27 = r5;
  // lhz r11,0(r25)
  r11 = guest::LoadU16(schema::RaceHudPackedTime(uint32_t(r25)).sign_minutes_address());
  // clrlwi r26,r11,17
  r26 = uint32_t(r11) & 0x7FFF;
  // mr r31,r26
  r31 = r26;
  // cmpwi cr6,r26,59
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 59, so);
  // ble cr6,0x82213998
  if (!cr6.gt) goto loc_82213998;
  // li r31,59
  r31 = uint64_t(int64_t(59));
loc_82213998:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // li r29,10
  r29 = uint64_t(int64_t(10));
loc_822139A0:
  // divw r11,r31,r29
  r11 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // lwz r3,632(r28)
  r3 = schema::RaceHud(uint32_t(r28)).splits_bits();
  // add r5,r30,r27
  r5 = r30 + r27;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r24,r11,2
  r24 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822139C0, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // divw r31,r31,r29
  r31 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // stw r24,108(r3)
  schema::UiMovie(uint32_t(r3)).store_at(108, 4, uint32_t(r24));
  // bge 0x822139a0
  if (!cr0.lt) goto loc_822139A0;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(schema::RaceHudPackedTime(uint32_t(r25)).packed_address());
  // cmpwi cr6,r26,59
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 59, so);
  // rlwinm r31,r11,22,26,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 22) & 0x3F;
  // ble cr6,0x822139e4
  if (!cr6.gt) goto loc_822139E4;
  // li r31,59
  r31 = uint64_t(int64_t(59));
loc_822139E4:
  // li r30,3
  r30 = uint64_t(int64_t(3));
loc_822139E8:
  // divw r11,r31,r29
  r11 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // lwz r3,632(r28)
  r3 = schema::RaceHud(uint32_t(r28)).splits_bits();
  // add r5,r30,r27
  r5 = r30 + r27;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r24,r11,2
  r24 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82213A08, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // divw r31,r31,r29
  r31 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // stw r24,108(r3)
  schema::UiMovie(uint32_t(r3)).store_at(108, 4, uint32_t(r24));
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // bge cr6,0x822139e8
  if (!cr6.lt) goto loc_822139E8;
  // lwz r11,0(r25)
  r11 = guest::LoadU32(schema::RaceHudPackedTime(uint32_t(r25)).packed_address());
  // cmpwi cr6,r26,59
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 59, so);
  // clrlwi r31,r11,22
  r31 = uint32_t(r11) & 0x3FF;
  // ble cr6,0x82213a30
  if (!cr6.gt) goto loc_82213A30;
  // li r31,999
  r31 = uint64_t(int64_t(999));
loc_82213A30:
  // li r30,6
  r30 = uint64_t(int64_t(6));
loc_82213A34:
  // divw r11,r31,r29
  r11 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // lwz r3,632(r28)
  r3 = schema::RaceHud(uint32_t(r28)).splits_bits();
  // add r5,r30,r27
  r5 = r30 + r27;
  // mulli r11,r11,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(10))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r26,r11,2
  r26 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82213A54, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // divw r31,r31,r29
  r31 = uint32_t((int32_t(r29) && !(int32_t(r31) == INT32_MIN && int32_t(r29) == -1)) ? int32_t(r31) / int32_t(r29) : 0);
  // stw r26,108(r3)
  schema::UiMovie(uint32_t(r3)).store_at(108, 4, uint32_t(r26));
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // bge cr6,0x82213a34
  if (!cr6.lt) goto loc_82213A34;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
}

// sub_82213A70: lifted from generated/ by lift.py.
lifted::Result f82213A70(uint64_t r1, uint64_t r13, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82213A70 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r31,r4
  r31 = r4;
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(LIFTED_ARGS(0x82213A8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r8,r10,30320
  r8 = uint64_t(int64_t(int64_t(r10) + 30320));
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // addi r5,r8,14
  r5 = uint64_t(int64_t(int64_t(r8) + 14));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,256
  r8 = uint64_t(int64_t(256));
  // mr r7,r31
  r7 = r31;
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r5,r11,r5
  r5 = r11 + r5;
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = game::WideCharToMultiByte(LIFTED_ARGS(0x82213AC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82213AE0: lifted from generated/ by lift.py.
uint64_t f82213AE0(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82213AE0 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // mr r28,r3
  r28 = r3;
  // mr r26,r24
  r26 = r24;
  // mr r27,r24
  r27 = r24;
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 23340);
  // mr r31,r24
  r31 = r24;
  // addis r30,r11,6
  r30 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r30,r30,-25880
  r30 = uint64_t(int64_t(int64_t(r30) + -25880));
  // addis r29,r30,5
  r29 = uint64_t(int64_t(int64_t(r30) + 327680));
  // addi r29,r29,-24608
  r29 = uint64_t(int64_t(int64_t(r29) + -24608));
  // lwz r11,0(r29)
  r11 = guest::LoadU32(schema::RaceData(uint32_t(r29) - 670408).results_count_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x82213b60
  if (!cr0.gt) goto loc_82213B60;
loc_82213B24:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(LIFTED_ARGS(0x82213B30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3924(r3)
  r11 = guest::LoadU32(schema::RacerResultsRecord(uint32_t(r3)).finish_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82213b4c
  if (cr6.eq) goto loc_82213B4C;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82213b50
  if (!cr6.eq) goto loc_82213B50;
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // b 0x82213b50
  goto loc_82213B50;
loc_82213B4C:
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
loc_82213B50:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(schema::RaceData(uint32_t(r29) - 670408).results_count_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x82213b24
  if (cr6.lt) goto loc_82213B24;
loc_82213B60:
  // lwz r11,16(r28)
  r11 = schema::RaceHud(uint32_t(r28)).player_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82213b78
  if (cr6.eq) goto loc_82213B78;
  // mr r11,r27
  r11 = r27;
  // mr r27,r26
  r27 = r26;
  // mr r26,r11
  r26 = r11;
loc_82213B78:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,636(r28)
  r3 = schema::RaceHud(uint32_t(r28)).movie19_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82213B88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r30,r3
  r30 = r3;
  // lwz r3,636(r28)
  r3 = schema::RaceHud(uint32_t(r28)).movie19_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82213B9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r30)
  r11 = uint32_t(schema::UiMovie(uint32_t(r30)).load_at(104, 4));
  // mr r29,r3
  r29 = r3;
  // mr r31,r24
  r31 = r24;
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x82213bec
  if (!cr6.gt) goto loc_82213BEC;
loc_82213BB4:
  // lwz r11,100(r30)
  r11 = uint32_t(schema::UiMovie(uint32_t(r30)).load_at(100, 4));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,636(r28)
  r3 = schema::RaceHud(uint32_t(r28)).movie19_bits();
  // add r5,r11,r31
  r5 = r11 + r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82213BC8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r27,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r31), so);
  // ble cr6,0x82213bd8
  if (!cr6.gt) goto loc_82213BD8;
  // stw r25,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r25));
  // b 0x82213bdc
  goto loc_82213BDC;
loc_82213BD8:
  // stw r24,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r24));
loc_82213BDC:
  // lwz r11,104(r30)
  r11 = uint32_t(schema::UiMovie(uint32_t(r30)).load_at(104, 4));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x82213bb4
  if (cr6.lt) goto loc_82213BB4;
loc_82213BEC:
  // lwz r11,104(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(104, 4));
  // mr r31,r24
  r31 = r24;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x82213c34
  if (!cr6.gt) goto loc_82213C34;
loc_82213BFC:
  // lwz r11,100(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(100, 4));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,636(r28)
  r3 = schema::RaceHud(uint32_t(r28)).movie19_bits();
  // add r5,r11,r31
  r5 = r11 + r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82213C10, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r26,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r31), so);
  // ble cr6,0x82213c20
  if (!cr6.gt) goto loc_82213C20;
  // stw r25,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r25));
  // b 0x82213c24
  goto loc_82213C24;
loc_82213C20:
  // stw r24,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r24));
loc_82213C24:
  // lwz r11,104(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(104, 4));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x82213bfc
  if (cr6.lt) goto loc_82213BFC;
loc_82213C34:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
}

// sub_82214BA8: lifted from generated/ by lift.py.
uint64_t f82214BA8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82214BA8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r10,41
  r10 = uint64_t(int64_t(41));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r25,r4
  r25 = r4;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r28,r3
  r28 = r3;
  // stw r10,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r10));
  // li r10,49
  r10 = uint64_t(int64_t(49));
  // stw r26,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r26));
  // mr r27,r26
  r27 = r26;
  // addi r30,r1,96
  r30 = uint64_t(int64_t(int64_t(r1) + 96));
  // subf r24,r11,r25
  r24 = r25 - r11;
  // stw r10,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r10));
  // li r10,57
  r10 = uint64_t(int64_t(57));
  // stw r10,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r10));
  // li r10,65
  r10 = uint64_t(int64_t(65));
  // stw r10,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r10));
loc_82214BF4:
  // lwz r11,60(r25)
  r11 = guest::LoadU32(schema::RaceHudRacers(uint32_t(r25)).unknown_60_address());
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // ble cr6,0x82214c44
  if (!cr6.gt) goto loc_82214C44;
  // lwzx r10,r24,r30
  r10 = guest::LoadU32(uint32_t(r24) + uint32_t(r30));
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r10,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r10));
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x820ff790
  // (left out: dead in C++)
  r3 = game::Time::Clamp(r1, r13, r3);
  // mr r11,r3
  r11 = r3;
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x820ff458
  // (left out: dead in C++)
  { const lifted::Result o = game::Time::Add(LIFTED_ARGS(0x82214C28, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r5,0(r30)
  r5 = guest::LoadU32(uint32_t(r30) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82213968
  // (left out: dead in C++)
  r3 = f82213968(r1, r13, r3, r4, r5);
  // b 0x82214c6c
  goto loc_82214C6C;
loc_82214C44:
  // lwz r29,0(r30)
  r29 = guest::LoadU32(uint32_t(r30) + 0);
  // mr r31,r26
  r31 = r26;
loc_82214C4C:
  // add r5,r31,r29
  r5 = r31 + r29;
  // lwz r3,632(r28)
  r3 = schema::RaceHudAt16(uint32_t(r28)).unknown_632_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82214C5C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // stw r26,108(r3)
  schema::UiMovie(uint32_t(r3)).store_at(108, 4, uint32_t(r26));
  // cmpwi cr6,r31,7
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 7, so);
  // blt cr6,0x82214c4c
  if (cr6.lt) goto loc_82214C4C;
loc_82214C6C:
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r27,4
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 4, so);
  // blt cr6,0x82214bf4
  if (cr6.lt) goto loc_82214BF4;
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
}

// sub_82214C88: lifted from generated/ by lift.py.
lifted::Result f82214C88(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82214C88 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-48(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 23340);
  // lwz r3,632(r31)
  r3 = schema::RaceHud(uint32_t(r31)).splits_bits();
  // addis r29,r11,6
  r29 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r29,r29,-27104
  r29 = uint64_t(int64_t(int64_t(r29) + -27104));
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(LIFTED_ARGS(0x82214CB4, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,860(r29)
  r11 = guest::LoadU16(schema::RaceData(uint32_t(r29) - 366112).stages_course_record_count_address());
  // addi r30,r31,496
  r30 = uint64_t(int64_t(int64_t(r31) + 496));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // mr r3,r30
  r3 = r30;
  // beq cr6,0x82214cd8
  if (cr6.eq) goto loc_82214CD8;
  // addi r4,r29,1144
  r4 = uint64_t(int64_t(int64_t(r29) + 1144));
  // li r5,76
  r5 = uint64_t(int64_t(76));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82214CD4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82214cdc
  goto loc_82214CDC;
loc_82214CD8:
  // bl 0x820ff5a0
  // (left out: dead in C++)
  f820FF5A0(r1, r13, r3);
loc_82214CDC:
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820ff628
  // (left out: dead in C++)
  r3 = f820FF628(r1, r13, r3, r4);
  // mr r4,r3
  r4 = r3;
  // li r5,25
  r5 = uint64_t(int64_t(25));
  // mr r3,r31
  r3 = r31;
  // bl 0x82213860
  // (left out: dead in C++)
  r3 = f82213860(r1, r13, r3, r4, r5);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,632(r31)
  r3 = schema::RaceHud(uint32_t(r31)).splits_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82214D08, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,68(r3)
  temp.f32 = float(f31);
  schema::UiMovie(uint32_t(r3)).store_at(68, 4, temp.u32);
  // lwz r3,632(r31)
  r3 = schema::RaceHud(uint32_t(r31)).splits_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82214D24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // mr r30,r28
  r30 = r28;
  // lwz r11,104(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(104, 4));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x82214d64
  if (!cr6.gt) goto loc_82214D64;
loc_82214D3C:
  // lwz r11,100(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(100, 4));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,632(r31)
  r3 = schema::RaceHud(uint32_t(r31)).splits_bits();
  // add r5,r30,r11
  r5 = r30 + r11;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82214D50, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,68(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  schema::UiMovie(uint32_t(r3)).store_at(68, 4, temp.u32);
  // lwz r11,104(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(104, 4));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x82214d3c
  if (cr6.lt) goto loc_82214D3C;
loc_82214D64:
  // stw r28,572(r31)
  schema::RaceHud(uint32_t(r31)).set_unknown_572_bits(uint32_t(r28));
  // stw r28,584(r31)
  schema::RaceHud(uint32_t(r31)).set_unknown_584_bits(uint32_t(r28));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82214E78: lifted from generated/ by lift.py.
lifted::Result f82214E78(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82214E78 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1136(r1)
  ea = -1136 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,444(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_444_bits();
  // lwz r3,3932(r11)
  r3 = guest::LoadU32(schema::RaceHudRacersPart(uint32_t(r11)).unknown_3932_address());
  // bl 0x82101760
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceTime::FromTicks(LIFTED_ARGS(0x82214E94, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // stw r11,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x82213860
  // (left out: dead in C++)
  r3 = f82213860(r1, r13, r3, r4, r5);
  // lwz r11,444(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_444_bits();
  // mr r3,r31
  r3 = r31;
  // lwz r10,836(r11)
  r10 = guest::LoadU32(schema::RaceHudRacersPart(uint32_t(r11)).unknown_836_address());
  // mulli r10,r10,76
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(76))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r4,r11,24
  r4 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x82214ba8
  // (left out: dead in C++)
  r3 = f82214BA8(r1, r13, r3, r4);
  // lwz r11,444(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_444_bits();
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r11,4
  r4 = uint64_t(int64_t(int64_t(r11) + 4));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,972
  r5 = uint64_t(int64_t(972));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 23340);
  // addis r27,r11,6
  r27 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r27,r27,-27104
  r27 = uint64_t(int64_t(int64_t(r27) + -27104));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82214EEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,584(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_584_bits();
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82214f3c
  if (!cr6.eq) goto loc_82214F3C;
  // lwz r11,576(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_576_bits();
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,576(r31)
  schema::RaceHudAt16_3(uint32_t(r31)).set_unknown_576_bits(uint32_t(r11));
  // bne 0x82214f3c
  if (!cr0.eq) goto loc_82214F3C;
  // lwz r11,580(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_580_bits();
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r29,584(r31)
  schema::RaceHudAt16_3(uint32_t(r31)).set_unknown_584_bits(uint32_t(r29));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bne cr6,0x82214f38
  if (!cr6.eq) goto loc_82214F38;
  // li r4,1
  r4 = uint64_t(int64_t(1));
loc_82214F38:
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82214F3C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82214F3C:
  // lwz r11,928(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 928);
  // mulli r10,r11,76
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(76))));
  // addi r11,r1,176
  r11 = uint64_t(int64_t(int64_t(r1) + 176));
  // lwzx r26,r10,r11
  r26 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // lwz r11,572(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_572_bits();
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // beq cr6,0x8221509c
  if (cr6.eq) goto loc_8221509C;
  // stw r29,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r29));
  // mr r28,r29
  r28 = r29;
  // stw r29,88(r1)
  guest::StoreU32(uint32_t(r1) + 88, uint32_t(r29));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x82214fb4
  if (cr6.lt) goto loc_82214FB4;
  // addi r11,r1,116
  r11 = uint64_t(int64_t(int64_t(r1) + 116));
  // add r30,r10,r11
  r30 = r10 + r11;
loc_82214F74:
  // lwz r10,0(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 0);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r10));
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x820ff790
  // (left out: dead in C++)
  r3 = game::Time::Clamp(r1, r13, r3);
  // mr r11,r3
  r11 = r3;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,84
  r3 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x820ff458
  // (left out: dead in C++)
  { const lifted::Result o = game::Time::Add(LIFTED_ARGS(0x82214F9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,572(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_572_bits();
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // stw r3,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r3));
  // cmpw cr6,r28,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r11), so);
  // ble cr6,0x82214f74
  if (!cr6.gt) goto loc_82214F74;
loc_82214FB4:
  // lhz r11,860(r27)
  r11 = guest::LoadU16(schema::RaceData(uint32_t(r27) - 366112).stages_course_record_count_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82215014
  if (cr6.eq) goto loc_82215014;
  // lwz r11,572(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_572_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x82215020
  if (cr6.lt) goto loc_82215020;
  // addi r30,r31,496
  r30 = uint64_t(int64_t(int64_t(r31) + 496));
loc_82214FD0:
  // lwz r10,0(r30)
  r10 = schema::RaceHudAt16_3(uint32_t(r30) - 496).unknown_496_bits();
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r10));
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x820ff790
  // (left out: dead in C++)
  r3 = game::Time::Clamp(r1, r13, r3);
  // mr r11,r3
  r11 = r3;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x820ff458
  // (left out: dead in C++)
  { const lifted::Result o = game::Time::Add(LIFTED_ARGS(0x82214FF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,572(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_572_bits();
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // stw r3,88(r1)
  guest::StoreU32(uint32_t(r1) + 88, uint32_t(r3));
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // ble cr6,0x82214fd0
  if (!cr6.gt) goto loc_82214FD0;
  // b 0x82215020
  goto loc_82215020;
loc_82215014:
  // lis r11,59
  r11 = uint64_t(int64_t(3866624));
  // ori r11,r11,61415
  r11 = r11 | 61415;
  // stw r11,88(r1)
  guest::StoreU32(uint32_t(r1) + 88, uint32_t(r11));
loc_82215020:
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r3,r1,84
  r3 = uint64_t(int64_t(int64_t(r1) + 84));
  // bl 0x82162230
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82162230, LIFTED_ARGS(0x8221502C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r30,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r30));
  // bl 0x82213860
  // (left out: dead in C++)
  r3 = f82213860(r1, r13, r3, r4, r5);
  // li r11,60
  r11 = uint64_t(int64_t(60));
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,576(r31)
  schema::RaceHudAt16_3(uint32_t(r31)).set_unknown_576_bits(uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82215058, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82215064, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,632(r31)
  r3 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_632_bits();
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
  // rlwinm. r30,r30,1,31,31
  r30 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne 0x82215088
  if (!cr0.eq) goto loc_82215088;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_82215088:
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8221508C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r30,580(r31)
  schema::RaceHudAt16_3(uint32_t(r31)).set_unknown_580_bits(uint32_t(r30));
  // stw r26,572(r31)
  schema::RaceHudAt16_3(uint32_t(r31)).set_unknown_572_bits(uint32_t(r26));
  // stw r11,584(r31)
  schema::RaceHudAt16_3(uint32_t(r31)).set_unknown_584_bits(uint32_t(r11));
loc_8221509C:
  // addi r1,r1,1136
  r1 = uint64_t(int64_t(int64_t(r1) + 1136));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822150A8: lifted from generated/ by lift.py.
lifted::Result f822150A8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822150A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r5,16(r31)
  r5 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_16_bits();
  // xori r30,r5,1
  r30 = r5 ^ 1;
  // bl 0x82214790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82214790, LIFTED_ARGS(0x822150D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x82214790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82214790, LIFTED_ARGS(0x822150E0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822150F8: lifted from generated/ by lift.py.
uint64_t f822150F8(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822150F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,444(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_444_bits();
  // lwz r3,3932(r11)
  r3 = guest::LoadU32(schema::RaceHudRacersPart(uint32_t(r11)).unknown_3932_address());
  // bl 0x82101760
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceTime::FromTicks(LIFTED_ARGS(0x82215118, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,33
  r5 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82213860
  // (left out: dead in C++)
  r3 = f82213860(r1, r13, r3, r4, r5);
  // lwz r11,444(r31)
  r11 = schema::RaceHudAt16_3(uint32_t(r31)).unknown_444_bits();
  // mr r3,r31
  r3 = r31;
  // lwz r10,836(r11)
  r10 = guest::LoadU32(schema::RaceHudRacersPart(uint32_t(r11)).unknown_836_address());
  // mulli r10,r10,76
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(76))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r4,r11,24
  r4 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x82214ba8
  // (left out: dead in C++)
  r3 = f82214BA8(r1, r13, r3, r4);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82215520: lifted from generated/ by lift.py.
lifted::Result f82215520(uint64_t r1, uint64_t r13, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82215520 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r4
  r31 = r4;
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82215578
  if (!cr6.eq) goto loc_82215578;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29448
  r11 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r5,4(r11)
  r5 = guest::LoadU32(uint32_t(r11) + 4);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmplw cr6,r5,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r11), so);
  // ble cr6,0x82215568
  if (!cr6.gt) goto loc_82215568;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r5,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r11), so);
loc_82215568:
  // blt cr6,0x82215570
  if (cr6.lt) goto loc_82215570;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82215570:
  // addi r5,r5,14
  r5 = uint64_t(int64_t(int64_t(r5) + 14));
  // b 0x82215584
  goto loc_82215584;
loc_82215578:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // addi r5,r11,14
  r5 = uint64_t(int64_t(int64_t(r11) + 14));
loc_82215584:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,256
  r8 = uint64_t(int64_t(256));
  // mr r7,r31
  r7 = r31;
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = game::WideCharToMultiByte(LIFTED_ARGS(0x822155A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822155C0: lifted from generated/ by lift.py.
lifted::Result f822155C0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822155C0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
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
  guest::StoreU64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f29));
  // stfd f30,-64(r1)
  guest::StoreU64(uint32_t(r1) + -64, std::bit_cast<uint64_t>(f30));
  // stfd f31,-56(r1)
  guest::StoreU64(uint32_t(r1) + -56, std::bit_cast<uint64_t>(f31));
  // stwu r1,-432(r1)
  ea = -432 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r31,r3
  r31 = r3;
  // bl 0x82213638
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82213638, LIFTED_ARGS(0x822155E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,604(r31)
  r3 = schema::RaceHud(uint32_t(r31)).racers_movie_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // stw r11,452(r31)
  schema::RaceHud(uint32_t(r31)).set_racer_count_bits(uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822155FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r30,100(r29)
  r30 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(100, 4));
  // lwz r11,104(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(104, 4));
  // add r11,r30,r11
  r11 = r30 + r11;
  // b 0x82215638
  goto loc_82215638;
loc_82215614:
  // mr r5,r30
  r5 = r30;
  // lwz r3,604(r31)
  r3 = schema::RaceHud(uint32_t(r31)).racers_movie_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82215624, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r27,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r27));
  // lwz r11,100(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(100, 4));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // lwz r10,104(r29)
  r10 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(104, 4));
  // add r11,r11,r10
  r11 = r11 + r10;
loc_82215638:
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x82215614
  if (cr6.lt) goto loc_82215614;
  // li r30,1
  r30 = uint64_t(int64_t(1));
loc_82215644:
  // addi r5,r30,2
  r5 = uint64_t(int64_t(int64_t(r30) + 2));
  // lwz r3,604(r31)
  r3 = schema::RaceHud(uint32_t(r31)).racers_movie_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82215654, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // stw r11,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r11));
  // blt cr6,0x82215644
  if (cr6.lt) goto loc_82215644;
  // li r30,1
  r30 = uint64_t(int64_t(1));
loc_8221566C:
  // addi r5,r30,2
  r5 = uint64_t(int64_t(int64_t(r30) + 2));
  // lwz r3,604(r31)
  r3 = schema::RaceHud(uint32_t(r31)).racers_movie_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221567C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r27,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r27));
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // blt cr6,0x8221566c
  if (cr6.lt) goto loc_8221566C;
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r27
  r30 = r27;
  // lfs f29,-2268(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r9) + -2268);
  f29 = double(temp.f32);
  // lfs f30,10848(r10)
  temp.u32 = guest::LoadU32(uint32_t(r10) + 10848);
  f30 = double(temp.f32);
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
loc_822156A8:
  // addi r28,r30,1
  r28 = uint64_t(int64_t(int64_t(r30) + 1));
  // lwz r3,604(r31)
  r3 = schema::RaceHud(uint32_t(r31)).racers_movie_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r5,r28
  r5 = r28;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822156BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x822156d8
  if (!cr6.eq) goto loc_822156D8;
  // bl 0x82213a70
  // (left out: dead in C++)
  { const lifted::Result o = f82213A70(r1, r13, r4, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822156dc
  goto loc_822156DC;
loc_822156D8:
  // bl 0x82215520
  // (left out: dead in C++)
  { const lifted::Result o = f82215520(r1, r13, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_822156DC:
  // lwz r11,100(r29)
  r11 = uint32_t(schema::UiMovie(uint32_t(r29)).load_at(100, 4));
  // mr r6,r3
  r6 = r3;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,604(r31)
  r3 = schema::RaceHud(uint32_t(r31)).racers_movie_bits();
  // addi r5,r11,5
  r5 = uint64_t(int64_t(int64_t(r11) + 5));
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // lwz r10,16(r31)
  r10 = schema::RaceHud(uint32_t(r31)).player_bits();
  // lwz r9,444(r31)
  r9 = schema::RaceHud(uint32_t(r31)).racers_bits();
  // add r11,r11,r31
  r11 = r11 + r31;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x82215710
  if (cr6.eq) goto loc_82215710;
  // xori r10,r10,1
  r10 = r10 ^ 1;
loc_82215710:
  // mulli r10,r10,972
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(972))));
  // add r10,r10,r9
  r10 = r10 + r9;
  // mr r30,r28
  r30 = r28;
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // lwz r10,788(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 788);
  // stfs f31,260(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(uint32_t(r11) + 260, temp.u32);
  // stfs f31,244(r11)
  temp.f32 = float(f31);
  guest::StoreU32(uint32_t(r11) + 244, temp.u32);
  // stw r27,276(r11)
  guest::StoreU32(uint32_t(r11) + 276, uint32_t(r27));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // stw r27,280(r11)
  guest::StoreU32(uint32_t(r11) + 280, uint32_t(r27));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,264(r11)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r11) + 264, temp.u32);
  // stfs f0,268(r11)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r11) + 268, temp.u32);
  // stfs f0,252(r11)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r11) + 252, temp.u32);
  // fmuls f13,f0,f30
  f13 = double(float(f0 * f30));
  // stfs f13,248(r11)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r11) + 248, temp.u32);
  // fmuls f0,f0,f29
  f0 = double(float(f0 * f29));
  // stfs f0,256(r11)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r11) + 256, temp.u32);
  // blt cr6,0x822156a8
  if (cr6.lt) goto loc_822156A8;
  // mr r3,r31
  r3 = r31;
  // bl 0x82213ae0
  // (left out: dead in C++)
  r3 = f82213AE0(r1, r13, r3);
  // mr r30,r27
  r30 = r27;
loc_82215774:
  // mr r5,r30
  r5 = r30;
  // lwz r3,604(r31)
  r3 = schema::RaceHud(uint32_t(r31)).racers_movie_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x82215784, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // blt cr6,0x82215774
  if (cr6.lt) goto loc_82215774;
  // stw r27,456(r31)
  schema::RaceHud(uint32_t(r31)).set_any_bar_low_bits(uint32_t(r27));
  // addi r1,r1,432
  r1 = uint64_t(int64_t(int64_t(r1) + 432));
  // lfd f29,-72(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -72)));
  // lfd f30,-64(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -64)));
  // lfd f31,-56(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822159F0: lifted from generated/ by lift.py.
uint64_t f822159F0(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822159F0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r9,336(r11)
  schema::RaceHudAt656(uint32_t(r11)).set_unknown_336_bits(uint32_t(r9));
  // stw r10,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r10));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82215A20, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82215A3C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82215A50: lifted from generated/ by lift.py.
lifted::Result f82215A50(uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82215A50 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82215A80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82215A94, r31)); r3 = o.r3; f1 = o.f1; }
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r6,-32255
  r6 = uint64_t(int64_t(-2113863680));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r11,r31,272
  r11 = uint64_t(int64_t(int64_t(r31) + 272));
  // stw r9,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r9));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f11,-2268(r6)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r6) + -2268);
  f11 = double(temp.f32);
  // mr r10,r8
  r10 = r8;
  // lfs f12,10848(r7)
  temp.u32 = guest::LoadU32(uint32_t(r7) + 10848);
  f12 = double(temp.f32);
  // lfs f13,2612(r9)
  temp.u32 = guest::LoadU32(uint32_t(r9) + 2612);
  f13 = double(temp.f32);
loc_82215AC0:
  // lwz r9,344(r31)
  r9 = schema::SplitHud(uint32_t(r31)).results_bits();
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r10,r10,972
  r10 = uint64_t(int64_t(int64_t(r10) + 972));
  // cmpwi cr6,r10,1944
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1944, so);
  // lwa r9,788(r9)
  r9 = uint64_t(int64_t(int32_t(guest::LoadU32(schema::RacerResultsRecord(uint32_t(r9)).e0_sp_address()))));
  // stfs f13,-8(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_264_bits(temp.u32);
  // stfs f13,-24(r11)
  temp.f32 = float(f13);
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_248_bits(temp.u32);
  // stw r8,8(r11)
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_280_bits(uint32_t(r8));
  // stw r8,12(r11)
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_284_bits(uint32_t(r8));
  // std r9,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r9);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,-4(r11)
  temp.f32 = float(f0);
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_268_bits(temp.u32);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_272_bits(temp.u32);
  // stfs f0,-16(r11)
  temp.f32 = float(f0);
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_256_bits(temp.u32);
  // fmuls f10,f0,f12
  f10 = double(float(f0 * f12));
  // stfs f10,-20(r11)
  temp.f32 = float(f10);
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_252_bits(temp.u32);
  // fmuls f0,f0,f11
  f0 = double(float(f0 * f11));
  // stfs f0,-12(r11)
  temp.f32 = float(f0);
  schema::SplitHud(uint32_t(r11) - 272).set_unknown_260_bits(temp.u32);
  // addi r11,r11,40
  r11 = uint64_t(int64_t(int64_t(r11) + 40));
  // blt cr6,0x82215ac0
  if (cr6.lt) goto loc_82215AC0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82215B24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82215B30, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82215B48: lifted from generated/ by lift.py.
lifted::Result f82215B48(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82215B48 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r25,r3
  r25 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82215B70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82215B84, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // addi r27,r25,328
  r27 = uint64_t(int64_t(int64_t(r25) + 328));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // li r11,14
  r11 = uint64_t(int64_t(14));
  // stw r24,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r24));
  // stw r11,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,88(r1)
  guest::StoreU32(uint32_t(r1) + 88, uint32_t(r11));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r11));
loc_82215BB8:
  // lwz r11,344(r25)
  r11 = schema::RaceHudAt656(uint32_t(r25)).unknown_344_bits();
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // add r11,r11,r26
  r11 = r11 + r26;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(schema::RacerResultsRecord(uint32_t(r11)).e0_place_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,0(r27)
  schema::RaceHudAt656(uint32_t(r27) - 328).set_unknown_328_bits(uint32_t(r11));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r5,r30,r11
  r5 = guest::LoadU32(uint32_t(r30) + uint32_t(r11));
  // blt 0x82215c40
  if (cr0.lt) goto loc_82215C40;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82215BE4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // stw r24,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r24));
  // lwzx r29,r30,r11
  r29 = guest::LoadU32(uint32_t(r30) + uint32_t(r11));
loc_82215BF4:
  // add r4,r29,r28
  r4 = r29 + r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82215C00, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmpwi cr6,r28,2
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 2, so);
  // blt cr6,0x82215bf4
  if (cr6.lt) goto loc_82215BF4;
  // lwz r11,0(r27)
  r11 = schema::RaceHudAt656(uint32_t(r27) - 328).unknown_328_bits();
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // add r4,r29,r11
  r4 = r29 + r11;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82215C2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = schema::RaceHudAt656(uint32_t(r27) - 328).unknown_328_bits();
  // mr r3,r31
  r3 = r31;
  // add r4,r29,r11
  r4 = r29 + r11;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(LIFTED_ARGS(0x82215C3C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82215c4c
  goto loc_82215C4C;
loc_82215C40:
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82215C44, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r11));
loc_82215C4C:
  // addi r26,r26,972
  r26 = uint64_t(int64_t(int64_t(r26) + 972));
  // addi r27,r27,4
  r27 = uint64_t(int64_t(int64_t(r27) + 4));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r26,1944
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 1944, so);
  // blt cr6,0x82215bb8
  if (cr6.lt) goto loc_82215BB8;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82215D08: lifted from generated/ by lift.py.
lifted::Result f82215D08(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82215D08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r9,336(r11)
  schema::SplitHud(uint32_t(r11)).set_countdown_running_bits(uint32_t(r9));
  // stw r10,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r10));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82215D38, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82215D44, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82215D58: lifted from generated/ by lift.py.
lifted::Result f82215D58(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82215D58 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
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
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82215D84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82215D90, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82216810: lifted from generated/ by lift.py.
lifted::Result f82216810(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82216810 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // stw r4,340(r3)
  schema::RaceHudAt656(uint32_t(r3)).set_unknown_340_bits(uint32_t(r4));
  // cmplwi cr6,r4,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 1, so);
  // blt cr6,0x82216824
  if (cr6.lt) goto loc_82216824;
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x82215b48
  { const lifted::Result o = f82215B48(r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82216824:
  // b 0x82215a50
  { const lifted::Result o = f82215A50(r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822194A0 game::CareerResults::Close2: lifted from generated/ by lift.py.
lifted::Result f822194A0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822194A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = schema::CareerResultsScreen(uint32_t(r3)).kind_bits();
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82219500
  if (!cr6.eq) goto loc_82219500;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // stw r11,0(r3)
  schema::CareerResultsScreen(uint32_t(r3)).set_kind_bits(uint32_t(r11));
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x822194DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
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
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x82219500, r31)); r3 = o.r3; f1 = o.f1; }
loc_82219500:
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82219510, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82219530 game::CareerResults::Close3: lifted from generated/ by lift.py.
lifted::Result f82219530(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82219530 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = schema::CareerResultsScreen(uint32_t(r3)).kind_bits();
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82219590
  if (!cr6.eq) goto loc_82219590;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // stw r11,0(r3)
  schema::CareerResultsScreen(uint32_t(r3)).set_kind_bits(uint32_t(r11));
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x8221956C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x82219590, r31)); r3 = o.r3; f1 = o.f1; }
loc_82219590:
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822195A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822195C0 game::CareerResults::Next2: lifted from generated/ by lift.py.
lifted::Result f822195C0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822195C0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = schema::CareerResultsScreen(uint32_t(r3)).kind_bits();
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82219648
  if (!cr6.eq) goto loc_82219648;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r3)
  schema::CareerResultsScreen(uint32_t(r3)).set_kind_bits(uint32_t(r11));
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // lwz r10,0(r11)
  r10 = schema::CareerResultsScreen(uint32_t(r11)).kind_bits();
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x82219614
  if (cr6.lt) goto loc_82219614;
  // beq cr6,0x8221960c
  if (cr6.eq) goto loc_8221960C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x82219628
  if (!cr6.lt) goto loc_82219628;
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // b 0x82219618
  goto loc_82219618;
loc_8221960C:
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // b 0x82219618
  goto loc_82219618;
loc_82219614:
  // li r5,11
  r5 = uint64_t(int64_t(11));
loc_82219618:
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x82219624, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
loc_82219628:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x82219648, r31)); r3 = o.r3; f1 = o.f1; }
loc_82219648:
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82219658, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82219678 game::CareerResults::Next3: lifted from generated/ by lift.py.
lifted::Result f82219678(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82219678 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = schema::CareerResultsScreen(uint32_t(r3)).kind_bits();
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82219700
  if (!cr6.eq) goto loc_82219700;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r3)
  schema::CareerResultsScreen(uint32_t(r3)).set_kind_bits(uint32_t(r11));
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // lwz r10,0(r11)
  r10 = schema::CareerResultsScreen(uint32_t(r11)).kind_bits();
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x822196cc
  if (cr6.lt) goto loc_822196CC;
  // beq cr6,0x822196c4
  if (cr6.eq) goto loc_822196C4;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x822196e0
  if (!cr6.lt) goto loc_822196E0;
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // b 0x822196d0
  goto loc_822196D0;
loc_822196C4:
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // b 0x822196d0
  goto loc_822196D0;
loc_822196CC:
  // li r5,11
  r5 = uint64_t(int64_t(11));
loc_822196D0:
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x822196DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
loc_822196E0:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x82219700, r31)); r3 = o.r3; f1 = o.f1; }
loc_82219700:
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82219710, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82219730 game::CareerResults::Next4: lifted from generated/ by lift.py.
lifted::Result f82219730(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82219730 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = schema::CareerResultsScreen(uint32_t(r3)).kind_bits();
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8221977c
  if (!cr6.eq) goto loc_8221977C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r11,0(r3)
  schema::CareerResultsScreen(uint32_t(r3)).set_kind_bits(uint32_t(r11));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x8221977C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8221977C:
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8221978C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82219D00 game::CareerResults::SetEntry: lifted from generated/ by lift.py.
lifted::Result f82219D00(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82219D00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-56(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(uint32_t(r1) + -56, std::bit_cast<uint64_t>(f30));
  // stfd f31,-48(r1)
  guest::StoreU64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,172(r31)
  r11 = schema::CareerResultsScreen(uint32_t(r31)).entry_bits();
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82219d30
  if (!cr0.eq) goto loc_82219D30;
  // lfs f30,24(r31)
  temp.u32 = schema::CareerResultsScreen(uint32_t(r31)).entry_value_bits();
  f30 = double(temp.f32);
  // lwz r28,28(r31)
  r28 = schema::CareerResultsScreen(uint32_t(r31)).entry_points_bits();
  // b 0x82219d48
  goto loc_82219D48;
loc_82219D30:
  // addi r10,r11,3
  r10 = uint64_t(int64_t(int64_t(r11) + 3));
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // ldx r10,r10,r31
  r10 = guest::LoadU64(uint32_t(r10) + uint32_t(r31));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfs f30,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r1) + 80);
  f30 = double(temp.f32);
  // lwz r28,84(r1)
  r28 = guest::LoadU32(uint32_t(r1) + 84);
loc_82219D48:
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // ldx r11,r11,r31
  r11 = guest::LoadU64(uint32_t(r11) + uint32_t(r31));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82219D68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r30,r3
  r30 = r3;
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82219D7C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219D8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1974
  r5 = uint64_t(int64_t(1974));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219D9C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219DA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1975
  r5 = uint64_t(int64_t(1975));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219DB8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219DC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1973
  r5 = uint64_t(int64_t(1973));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219DD4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219DE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1982
  r5 = uint64_t(int64_t(1982));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219DF0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219DFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2051
  r5 = uint64_t(int64_t(2051));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219E0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219E18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2051
  r5 = uint64_t(int64_t(2051));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219E28, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219E34, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r4,r3
  r4 = r3;
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r5,2052
  r5 = uint64_t(int64_t(2052));
  // lfd f31,-23280(r11)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r11) + -23280)));
  // fmul f0,f30,f31
  f0 = f30 * f31;
  // frsp f1,f0
  f1 = double(float(f0));
  // stfd f1,40(r1)
  guest::StoreU64(uint32_t(r1) + 40, std::bit_cast<uint64_t>(f1));
  // ld r6,40(r1)
  r6 = guest::LoadU64(uint32_t(r1) + 40);
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219E5C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219E68, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r28
  r5 = r28;
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211ca18
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeNumberText(LIFTED_ARGS(0x82219E78, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219E84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // lwz r5,168(r31)
  r5 = schema::CareerResultsScreen(uint32_t(r31)).total_bits();
  // bl 0x8211ca18
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeNumberText(LIFTED_ARGS(0x82219E94, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1974
  r5 = uint64_t(int64_t(1974));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219EB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219EBC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1975
  r5 = uint64_t(int64_t(1975));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219ECC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219ED8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1973
  r5 = uint64_t(int64_t(1973));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219EE8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219EF4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1982
  r5 = uint64_t(int64_t(1982));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219F04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219F10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2051
  r5 = uint64_t(int64_t(2051));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219F20, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219F2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2051
  r5 = uint64_t(int64_t(2051));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219F3C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219F48, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f30,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r1) + 80);
  f30 = double(temp.f32);
  // fmul f0,f30,f31
  f0 = f30 * f31;
  // mr r4,r3
  r4 = r3;
  // li r5,2052
  r5 = uint64_t(int64_t(2052));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // frsp f1,f0
  f1 = double(float(f0));
  // stfd f1,40(r1)
  guest::StoreU64(uint32_t(r1) + 40, std::bit_cast<uint64_t>(f1));
  // ld r6,40(r1)
  r6 = guest::LoadU64(uint32_t(r1) + 40);
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82219F6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219F78, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,84(r1)
  r30 = guest::LoadU32(uint32_t(r1) + 84);
  // mr r4,r3
  r4 = r3;
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // mr r5,r30
  r5 = r30;
  // bl 0x8211ca18
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeNumberText(LIFTED_ARGS(0x82219F8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82219F98, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,168(r31)
  r11 = schema::CareerResultsScreen(uint32_t(r31)).total_bits();
  // mr r4,r3
  r4 = r3;
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // add r5,r11,r30
  r5 = r11 + r30;
  // bl 0x8211ca18
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeNumberText(LIFTED_ARGS(0x82219FAC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1974
  r6 = uint64_t(int64_t(1974));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82219FC0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r6,1975
  r6 = uint64_t(int64_t(1975));
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82219FD4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1973
  r6 = uint64_t(int64_t(1973));
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82219FE8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1982
  r6 = uint64_t(int64_t(1982));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82219FFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2051
  r6 = uint64_t(int64_t(2051));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221A010, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2051
  r6 = uint64_t(int64_t(2051));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221A024, r31)); r3 = o.r3; f1 = o.f1; }
  // fmul f0,f30,f31
  lifted::ScalarFloatMode();
  f0 = f30 * f31;
  // li r6,2052
  r6 = uint64_t(int64_t(2052));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // frsp f1,f0
  f1 = double(float(f0));
  // stfd f1,48(r1)
  guest::StoreU64(uint32_t(r1) + 48, std::bit_cast<uint64_t>(f1));
  // ld r7,48(r1)
  r7 = guest::LoadU64(uint32_t(r1) + 48);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221A048, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r30
  r6 = r30;
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211c740
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNumberText(LIFTED_ARGS(0x8221A05C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,168(r31)
  r11 = schema::CareerResultsScreen(uint32_t(r31)).total_bits();
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // lwz r3,180(r31)
  r3 = schema::CareerResultsScreen(uint32_t(r31)).movie_bits();
  // add r6,r11,r30
  r6 = r11 + r30;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8211c740
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNumberText(LIFTED_ARGS(0x8221A074, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,168(r31)
  r11 = schema::CareerResultsScreen(uint32_t(r31)).total_bits();
  // add r11,r11,r30
  r11 = r11 + r30;
  // stw r11,168(r31)
  schema::CareerResultsScreen(uint32_t(r31)).set_total_bits(uint32_t(r11));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f30,-56(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -56)));
  // lfd f31,-48(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8221A110 game::CareerResults::NextEntry: lifted from generated/ by lift.py.
lifted::Result f8221A110(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8221A110 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = schema::CareerResultsScreen(uint32_t(r3)).kind_bits();
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8221a178
  if (!cr6.eq) goto loc_8221A178;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r11,0(r3)
  schema::CareerResultsScreen(uint32_t(r3)).set_kind_bits(uint32_t(r11));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x8221A160, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31488(r31)
  r3 = guest::LoadU32(uint32_t(r31) + -31488);
  // bl 0x82219d00
  // (left out: dead in C++)
  { const lifted::Result o = game::CareerResults::SetEntry(r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // lwz r10,172(r11)
  r10 = schema::CareerResultsScreen(uint32_t(r11)).entry_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,172(r11)
  schema::CareerResultsScreen(uint32_t(r11)).set_entry_bits(uint32_t(r10));
loc_8221A178:
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8221A188, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x8221a1d4
  if (cr0.eq) goto loc_8221A1D4;
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221A1A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r3)).store_at(68, 4, temp.u32);
  // lwz r11,-31488(r31)
  r11 = guest::LoadU32(uint32_t(r31) + -31488);
  // lwz r3,180(r11)
  r3 = schema::CareerResultsScreen(uint32_t(r11)).movie_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221A1C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  schema::UiMovie(uint32_t(r3)).store_at(68, 4, temp.u32);
loc_8221A1D4:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8221A8A8 game::AfterRaceMenu::ShowItem: lifted from generated/ by lift.py.
lifted::Result f8221A8A8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8221A8A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,0(r3)
  r10 = schema::AfterRaceMenuScreen(uint32_t(r3)).movie_bits();
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r11,-31472(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31472);
  // lwz r30,0(r11)
  r30 = schema::AfterRaceMenuScreen(uint32_t(r11)).movie_bits();
  // bne cr6,0x8221a928
  if (!cr6.eq) goto loc_8221A928;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // stw r11,0(r3)
  schema::AfterRaceMenuScreen(uint32_t(r3)).set_movie_bits(uint32_t(r11));
  // lwz r3,-31460(r31)
  r3 = guest::LoadU32(uint32_t(r31) + -31460);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8221A8E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x8221A90C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31460(r31)
  r3 = guest::LoadU32(uint32_t(r31) + -31460);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8221A914, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211ddd8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Loop(LIFTED_ARGS(0x8221A928, r31)); r3 = o.r3; f1 = o.f1; }
loc_8221A928:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8221A934, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8221AAD8 game::AfterRaceMenu::ShowListItem: lifted from generated/ by lift.py.
lifted::Result f8221AAD8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8221AAD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r29,r3
  r29 = r3;
  // lwz r11,-31472(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31472);
  // lwz r10,0(r29)
  r10 = schema::AfterRaceMenuScreen(uint32_t(r29)).movie_bits();
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r30,0(r11)
  r30 = schema::AfterRaceMenuScreen(uint32_t(r11)).movie_bits();
  // bne cr6,0x8221ab44
  if (!cr6.eq) goto loc_8221AB44;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31460(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -31460);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8221AB0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r31,r11,2
  r31 = uint64_t(int64_t(int64_t(r11) + 2));
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8221AB30, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(LIFTED_ARGS(0x8221AB3C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  schema::AfterRaceMenuScreen(uint32_t(r29)).set_movie_bits(uint32_t(r11));
loc_8221AB44:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8221AB50, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8221AB60 game::AfterRaceMenu::HideListItem: lifted from generated/ by lift.py.
lifted::Result f8221AB60(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8221AB60 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r31,r3
  r31 = r3;
  // lwz r11,-31472(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31472);
  // lwz r10,0(r31)
  r10 = schema::AfterRaceMenuScreen(uint32_t(r31)).movie_bits();
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r30,0(r11)
  r30 = schema::AfterRaceMenuScreen(uint32_t(r11)).movie_bits();
  // bne cr6,0x8221abb4
  if (!cr6.eq) goto loc_8221ABB4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31460(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -31460);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8221AB9C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r30
  r3 = r30;
  // addi r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8221ABAC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  schema::AfterRaceMenuScreen(uint32_t(r31)).set_movie_bits(uint32_t(r11));
loc_8221ABB4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8221ABC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8221ABE0 game::AfterRaceMenu::NotOnList: lifted from generated/ by lift.py.
uint64_t f8221ABE0() {
  ITC_NATIVE_RUNNING("sub_8221ABE0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r11,-31464(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31464);
  // lwz r11,0(r11)
  r11 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_8221ACC0 game::AfterRaceMenu::ShowList: lifted from generated/ by lift.py.
lifted::Result f8221ACC0(uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8221ACC0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,25072(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 25072);
  // lwz r26,20(r11)
  r26 = schema::SaveBuffers(uint32_t(r11)).count_bits();
  // lwz r28,12(r11)
  r28 = schema::SaveBuffers(uint32_t(r11)).records_bits();
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r26,6
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 6, so);
  // mr r23,r26
  r23 = r26;
  // lwz r11,-31472(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31472);
  // lwz r31,0(r11)
  r31 = schema::AfterRaceMenuScreen(uint32_t(r11)).movie_bits();
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31460(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31460);
  // lwz r25,4(r11)
  r25 = schema::MenuList(uint32_t(r11)).scroll_bits();
  // blt cr6,0x8221ad04
  if (cr6.lt) goto loc_8221AD04;
  // li r23,6
  r23 = uint64_t(int64_t(6));
loc_8221AD04:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221AD14, r31)); r3 = o.r3; f1 = o.f1; }
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // mr r27,r3
  r27 = r3;
  // mr r30,r24
  r30 = r24;
loc_8221AD20:
  // addi r4,r30,5
  r4 = uint64_t(int64_t(int64_t(r30) + 5));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8221AD2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8221AD38, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 6, so);
  // blt cr6,0x8221ad20
  if (cr6.lt) goto loc_8221AD20;
  // mr r29,r24
  r29 = r24;
  // cmpwi cr6,r23,0
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // ble cr6,0x8221ada0
  if (!cr6.gt) goto loc_8221ADA0;
  // mulli r11,r25,36
  r11 = uint64_t(int64_t(static_cast<int64_t>(r25 * static_cast<uint64_t>(36))));
  // add r11,r11,r28
  r11 = r11 + r28;
  // addi r30,r11,28
  r30 = uint64_t(int64_t(int64_t(r11) + 28));
loc_8221AD5C:
  // addi r4,r29,5
  r4 = uint64_t(int64_t(int64_t(r29) + 5));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8221AD68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r6,0(r30)
  r6 = guest::LoadU32(uint32_t(r30) + 0);
  // mr r4,r3
  r4 = r3;
  // cmpwi r6,0
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // mr r3,r31
  r3 = r31;
  // bne 0x8221ad88
  if (!cr0.eq) goto loc_8221AD88;
  // li r5,472
  r5 = uint64_t(int64_t(472));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8221AD84, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8221ad90
  goto loc_8221AD90;
loc_8221AD88:
  // li r5,471
  r5 = uint64_t(int64_t(471));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8221AD90, r31)); r3 = o.r3; f1 = o.f1; }
loc_8221AD90:
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,36
  r30 = uint64_t(int64_t(int64_t(r30) + 36));
  // cmpw cr6,r29,r23
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r23), so);
  // blt cr6,0x8221ad5c
  if (cr6.lt) goto loc_8221AD5C;
loc_8221ADA0:
  // cmpwi cr6,r26,6
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 6, so);
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bgt cr6,0x8221adc4
  if (cr6.gt) goto loc_8221ADC4;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221ADB8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // stw r24,80(r11)
  schema::UiMovie(uint32_t(r11)).store_at(80, 4, uint32_t(r24));
  // b 0x8221ae10
  goto loc_8221AE10;
loc_8221ADC4:
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // bne cr6,0x8221adf4
  if (!cr6.eq) goto loc_8221ADF4;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221ADD0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r24,80(r11)
  schema::UiMovie(uint32_t(r11)).store_at(80, 4, uint32_t(r24));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221ADE8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r11));
  // b 0x8221ae4c
  goto loc_8221AE4C;
loc_8221ADF4:
  // addi r11,r26,-6
  r11 = uint64_t(int64_t(int64_t(r26) + -6));
  // cmpw cr6,r25,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r25), int32_t(r11), so);
  // bne cr6,0x8221ae28
  if (!cr6.eq) goto loc_8221AE28;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221AE04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,80(r11)
  schema::UiMovie(uint32_t(r11)).store_at(80, 4, uint32_t(r10));
loc_8221AE10:
  // mr r3,r31
  r3 = r31;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221AE20, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r24,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r24));
  // b 0x8221ae4c
  goto loc_8221AE4C;
loc_8221AE28:
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221AE2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r30,80(r11)
  schema::UiMovie(uint32_t(r11)).store_at(80, 4, uint32_t(r30));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221AE48, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,80(r3)
  schema::UiMovie(uint32_t(r3)).store_at(80, 4, uint32_t(r30));
loc_8221AE4C:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8221AE58 game::AfterRaceMenu::ShowReplay: lifted from generated/ by lift.py.
lifted::Result f8221AE58(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8221AE58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,59
  r10 = uint64_t(int64_t(3866624));
  // lwz r29,16(r31)
  r29 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_time_bits();
  // lwz r11,-31472(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31472);
  // rlwinm r9,r29,0,1,15
  r9 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0x7FFF0000;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // lwz r30,0(r11)
  r30 = schema::AfterRaceMenuScreen(uint32_t(r11)).movie_bits();
  // ble cr6,0x8221ae94
  if (!cr6.gt) goto loc_8221AE94;
  // lis r11,59
  r11 = uint64_t(int64_t(3866624));
  // ori r11,r11,61415
  r11 = r11 | 61415;
  // rlwimi r29,r11,0,1,31
  r29 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x7FFFFFFF) | (r29 & 0xFFFFFFFF80000000);
loc_8221AE94:
  // lwz r11,28(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_number_bits();
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r3,r30
  r3 = r30;
  // beq cr6,0x8221b034
  if (cr6.eq) goto loc_8221B034;
  // lhz r11,10(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_minute_bits();
  // li r6,530
  r6 = uint64_t(int64_t(530));
  // lhz r10,8(r31)
  r10 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_hour_bits();
  // lhz r9,6(r31)
  r9 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_day_bits();
  // lhz r8,2(r31)
  r8 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_month_bits();
  // lhz r7,0(r31)
  r7 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_year_bits();
  // stw r11,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221AECC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_mode_bits();
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // addi r6,r11,473
  r6 = uint64_t(int64_t(int64_t(r11) + 473));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221AEE4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r9,r29,22
  r9 = uint32_t(r29) & 0x3FF;
  // rlwinm r8,r29,22,26,31
  r8 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 22) & 0x3F;
  // rlwinm r7,r29,16,17,31
  r7 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 16) & 0x7FFF;
  // li r6,528
  r6 = uint64_t(int64_t(528));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221AF04, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_course_bits();
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // addi r6,r11,1646
  r6 = uint64_t(int64_t(int64_t(r11) + 1646));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221AF1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_course_bits();
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // addi r6,r11,1790
  r6 = uint64_t(int64_t(int64_t(r11) + 1790));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221AF34, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_course_bits();
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // addi r6,r11,1718
  r6 = uint64_t(int64_t(int64_t(r11) + 1718));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221AF4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_mode_bits();
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8221affc
  if (cr6.lt) goto loc_8221AFFC;
  // beq cr6,0x8221af68
  if (cr6.eq) goto loc_8221AF68;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x8221afd8
  if (cr6.lt) goto loc_8221AFD8;
  // bne cr6,0x8221b0b4
  if (!cr6.eq) goto loc_8221B0B4;
loc_8221AF68:
  // li r11,395
  r11 = uint64_t(int64_t(395));
  // li r5,14
  r5 = uint64_t(int64_t(14));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r11,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,396
  r11 = uint64_t(int64_t(396));
  // stw r11,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r11));
  // li r11,397
  r11 = uint64_t(int64_t(397));
  // stw r11,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r11));
  // li r11,398
  r11 = uint64_t(int64_t(398));
  // stw r11,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r11));
  // li r11,401
  r11 = uint64_t(int64_t(401));
  // stw r11,112(r1)
  guest::StoreU32(uint32_t(r1) + 112, uint32_t(r11));
  // li r11,402
  r11 = uint64_t(int64_t(402));
  // stw r11,116(r1)
  guest::StoreU32(uint32_t(r1) + 116, uint32_t(r11));
  // li r11,404
  r11 = uint64_t(int64_t(404));
  // stw r11,120(r1)
  guest::StoreU32(uint32_t(r1) + 120, uint32_t(r11));
  // li r11,403
  r11 = uint64_t(int64_t(403));
  // stw r11,124(r1)
  guest::StoreU32(uint32_t(r1) + 124, uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221AFB8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r10,108(r11)
  schema::UiMovie(uint32_t(r11)).store_at(108, 4, uint32_t(r10));
  // lwz r11,32(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_result_bits();
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r6,r11,r9
  r6 = guest::LoadU32(uint32_t(r11) + uint32_t(r9));
  // b 0x8221b020
  goto loc_8221B020;
loc_8221AFD8:
  // li r5,14
  r5 = uint64_t(int64_t(14));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221AFE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r6,477
  r6 = uint64_t(int64_t(477));
  // stw r10,108(r11)
  schema::UiMovie(uint32_t(r11)).store_at(108, 4, uint32_t(r10));
  // b 0x8221b020
  goto loc_8221B020;
loc_8221AFFC:
  // li r5,14
  r5 = uint64_t(int64_t(14));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221B00C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,108(r11)
  schema::UiMovie(uint32_t(r11)).store_at(108, 4, uint32_t(r10));
  // lwz r11,32(r31)
  r11 = schema::AfterRaceMenuScreen(uint32_t(r31)).fault_replay_result_bits();
  // addi r6,r11,4650
  r6 = uint64_t(int64_t(int64_t(r11) + 4650));
loc_8221B020:
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8221B030, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8221b0b4
  goto loc_8221B0B4;
loc_8221B034:
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8221B038, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8221B048, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8221B058, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8221B068, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8221B078, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8221B088, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,14
  r5 = uint64_t(int64_t(14));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221B098, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r10,108(r11)
  schema::UiMovie(uint32_t(r11)).store_at(108, 4, uint32_t(r10));
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8221B0B4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8221B0B4:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8221B3C8 game::AfterRaceMenu::MoveList: lifted from generated/ by lift.py.
lifted::Result f8221B3C8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8221B3C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-88(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(uint32_t(r1) + -88, std::bit_cast<uint64_t>(f31));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r23,r3
  r23 = r3;
  // lwz r11,-31472(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31472);
  // lwz r10,0(r23)
  r10 = schema::AfterRaceMenuScreen(uint32_t(r23)).movie_bits();
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r31,0(r11)
  r31 = schema::AfterRaceMenuScreen(uint32_t(r11)).movie_bits();
  // bne cr6,0x8221b5a8
  if (!cr6.eq) goto loc_8221B5A8;
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31460(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -31460);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8221B400, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // lwz r3,-31460(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -31460);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::GetPreviousSelection(LIFTED_ARGS(0x8221B40C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31460(r30)
  r11 = guest::LoadU32(uint32_t(r30) + -31460);
  // mr r10,r3
  r10 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r10,2
  r4 = uint64_t(int64_t(int64_t(r10) + 2));
  // lwz r29,4(r11)
  r29 = schema::MenuList(uint32_t(r11)).scroll_bits();
  // lwz r9,8(r11)
  r9 = schema::MenuList(uint32_t(r11)).scroll_before_bits();
  // lwz r25,32(r11)
  r25 = schema::MenuList(uint32_t(r11)).wrapped_bits();
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // subf r24,r9,r29
  r24 = r29 - r9;
  // lwz r11,25072(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 25072);
  // lwz r26,12(r11)
  r26 = schema::SaveBuffers(uint32_t(r11)).records_bits();
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8221B43C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r28,2
  r27 = uint64_t(int64_t(int64_t(r28) + 2));
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
  // mr r4,r27
  r4 = r27;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8221B45C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r27
  r4 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(LIFTED_ARGS(0x8221B468, r31)); r3 = o.r3; f1 = o.f1; }
  // add r11,r29,r28
  r11 = r29 + r28;
  // mulli r11,r11,36
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(36))));
  // add r3,r11,r26
  r3 = r11 + r26;
  // bl 0x8221ae58
  // (left out: dead in C++)
  { const lifted::Result o = game::AfterRaceMenu::ShowReplay(r1, r13, r3, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8221B484, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8221B490, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // beq cr6,0x8221b4d0
  if (cr6.eq) goto loc_8221B4D0;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221B4A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r28,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8221B4C8, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).y_address(), temp.u32);
  // b 0x8221b554
  goto loc_8221B554;
loc_8221B4D0:
  // lwz r11,-31460(r30)
  r11 = guest::LoadU32(uint32_t(r30) + -31460);
  // lwz r30,24(r11)
  r30 = schema::MenuList(uint32_t(r11)).move_bits();
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221B4DC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x8221b510
  if (!cr6.lt) goto loc_8221B510;
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8221B4EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,36
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(36))));
  // add r11,r11,r26
  r11 = r11 + r26;
  // mr r4,r3
  r4 = r3;
  // li r5,471
  r5 = uint64_t(int64_t(471));
  // mr r3,r31
  r3 = r31;
  // lwz r6,28(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 28);
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8221B508, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // b 0x8221b53c
  goto loc_8221B53C;
loc_8221B510:
  // ble cr6,0x8221b554
  if (!cr6.gt) goto loc_8221B554;
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8221B51C, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,36
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(36))));
  // add r11,r11,r26
  r11 = r11 + r26;
  // mr r4,r3
  r4 = r3;
  // li r5,471
  r5 = uint64_t(int64_t(471));
  // mr r3,r31
  r3 = r31;
  // lwz r6,244(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 244);
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8221B538, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,13
  r4 = uint64_t(int64_t(13));
loc_8221B53C:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8221B554, r31)); r3 = o.r3; f1 = o.f1; }
loc_8221B554:
  // cmpwi cr6,r24,0
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 0, so);
  // beq cr6,0x8221b5a0
  if (cr6.eq) goto loc_8221B5A0;
  // li r4,25
  r4 = uint64_t(int64_t(25));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8221B568, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8221B574, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8221acc0
  // (left out: dead in C++)
  { const lifted::Result o = game::AfterRaceMenu::ShowList(r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r24,0
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 0, so);
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
  // li r4,25
  r4 = uint64_t(int64_t(25));
  // bgt cr6,0x8221b59c
  if (cr6.gt) goto loc_8221B59C;
  // li r4,24
  r4 = uint64_t(int64_t(24));
loc_8221B59C:
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8221B5A0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8221B5A0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r23)
  schema::AfterRaceMenuScreen(uint32_t(r23)).set_movie_bits(uint32_t(r11));
loc_8221B5A8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8221B5B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-88(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -88)));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8221B6A0 game::AfterRaceMenu::SetUpList: lifted from generated/ by lift.py.
lifted::Result f8221B6A0(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8221B6A0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,25072(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 25072);
  // lwz r10,-31472(r10)
  r10 = guest::LoadU32(uint32_t(r10) + -31472);
  // lwz r4,20(r11)
  r4 = schema::SaveBuffers(uint32_t(r11)).count_bits();
  // lwz r30,0(r10)
  r30 = schema::AfterRaceMenuScreen(uint32_t(r10)).movie_bits();
  // lwz r29,12(r11)
  r29 = schema::SaveBuffers(uint32_t(r11)).records_bits();
  // cmpwi cr6,r4,6
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 6, so);
  // mr r5,r4
  r5 = r4;
  // blt cr6,0x8221b6dc
  if (cr6.lt) goto loc_8221B6DC;
  // li r5,6
  r5 = uint64_t(int64_t(6));
loc_8221B6DC:
  // li r3,159
  r3 = uint64_t(int64_t(159));
  // li r11,158
  r11 = uint64_t(int64_t(158));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r10,156
  r10 = uint64_t(int64_t(156));
  // li r9,155
  r9 = uint64_t(int64_t(155));
  // li r8,153
  r8 = uint64_t(int64_t(153));
  // stw r3,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r3));
  // li r7,154
  r7 = uint64_t(int64_t(154));
  // stw r11,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r11));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r3,-31460(r31)
  r3 = guest::LoadU32(uint32_t(r31) + -31460);
  // bl 0x8220bac0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Init(LIFTED_ARGS(0x8221B70C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31460(r31)
  r3 = guest::LoadU32(uint32_t(r31) + -31460);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8221B714, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8221B728, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // std r11,96(r1)
  guest::StoreU64(uint32_t(r1) + 96, r11);
  // lfd f0,96(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 96)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8221B748, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).y_address(), temp.u32);
  // bl 0x8221acc0
  // (left out: dead in C++)
  { const lifted::Result o = game::AfterRaceMenu::ShowList(r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,36
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(36))));
  // add r3,r11,r29
  r3 = r11 + r29;
  // bl 0x8221ae58
  // (left out: dead in C++)
  { const lifted::Result o = game::AfterRaceMenu::ShowReplay(r1, r13, r3, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8221B918 game::AfterRaceMenu::OpenList: lifted from generated/ by lift.py.
lifted::Result f8221B918(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8221B918 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r30,r3
  r30 = r3;
  // lwz r11,-31472(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31472);
  // lwz r10,0(r30)
  r10 = schema::AfterRaceMenuScreen(uint32_t(r30)).movie_bits();
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r31,0(r11)
  r31 = schema::AfterRaceMenuScreen(uint32_t(r11)).movie_bits();
  // bne cr6,0x8221b99c
  if (!cr6.eq) goto loc_8221B99C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31460(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -31460);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8221B954, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r11,r3
  r11 = r3;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // lwz r10,-31468(r10)
  r10 = guest::LoadU32(uint32_t(r10) + -31468);
  // stw r11,16(r10)
  schema::AfterRaceMenuData(uint32_t(r10)).set_selection_bits(uint32_t(r11));
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x8221B974, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,51
  r4 = uint64_t(int64_t(51));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8221B990, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8221b6a0
  // (left out: dead in C++)
  { const lifted::Result o = game::AfterRaceMenu::SetUpList(r1, r13); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  schema::AfterRaceMenuScreen(uint32_t(r30)).set_movie_bits(uint32_t(r11));
loc_8221B99C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8221B9A8, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8221B9C8 game::AfterRaceMenu::CloseList: lifted from generated/ by lift.py.
lifted::Result f8221B9C8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8221B9C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r30,r3
  r30 = r3;
  // lwz r11,-31472(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31472);
  // lwz r10,0(r30)
  r10 = schema::AfterRaceMenuScreen(uint32_t(r30)).movie_bits();
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r31,0(r11)
  r31 = schema::AfterRaceMenuScreen(uint32_t(r11)).movie_bits();
  // bne cr6,0x8221ba3c
  if (!cr6.eq) goto loc_8221BA3C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31460(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -31460);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuList::Selection(LIFTED_ARGS(0x8221BA04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8221BA14, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,52
  r4 = uint64_t(int64_t(52));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8221BA30, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8221b5c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8221B5C8, LIFTED_ARGS(0x8221BA34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  schema::AfterRaceMenuScreen(uint32_t(r30)).set_movie_bits(uint32_t(r11));
loc_8221BA3C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8221BA48, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82222038 game::TitleScreen::SetUpAttractRace: lifted from generated/ by lift.py.
lifted::Result f82222038(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82222038 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(uint32_t(r1) + -4096);
  // stwu r1,-4128(r1)
  ea = -4128 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,27
  r11 = uint64_t(int64_t(27));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r9,92
  r9 = uint64_t(int64_t(92));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r11,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r11));
  // li r11,289
  r11 = uint64_t(int64_t(289));
  // stw r10,112(r1)
  guest::StoreU32(uint32_t(r1) + 112, uint32_t(r10));
  // stw r10,116(r1)
  guest::StoreU32(uint32_t(r1) + 116, uint32_t(r10));
  // stw r9,132(r1)
  guest::StoreU32(uint32_t(r1) + 132, uint32_t(r9));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r10,148(r1)
  guest::StoreU32(uint32_t(r1) + 148, uint32_t(r10));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r11,120(r1)
  guest::StoreU32(uint32_t(r1) + 120, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r31,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r31));
  // stw r31,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r31));
  // stw r9,140(r1)
  guest::StoreU32(uint32_t(r1) + 140, uint32_t(r9));
  // li r9,366
  r9 = uint64_t(int64_t(366));
  // stw r10,152(r1)
  guest::StoreU32(uint32_t(r1) + 152, uint32_t(r10));
  // li r10,304
  r10 = uint64_t(int64_t(304));
  // stw r11,124(r1)
  guest::StoreU32(uint32_t(r1) + 124, uint32_t(r11));
  // stw r11,128(r1)
  guest::StoreU32(uint32_t(r1) + 128, uint32_t(r11));
  // stw r11,160(r1)
  guest::StoreU32(uint32_t(r1) + 160, uint32_t(r11));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r31,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r31));
  // stw r31,136(r1)
  guest::StoreU32(uint32_t(r1) + 136, uint32_t(r31));
  // stw r9,144(r1)
  guest::StoreU32(uint32_t(r1) + 144, uint32_t(r9));
  // stw r10,156(r1)
  guest::StoreU32(uint32_t(r1) + 156, uint32_t(r10));
  // stw r11,164(r1)
  guest::StoreU32(uint32_t(r1) + 164, uint32_t(r11));
  // bl 0x82293568
  // (left out: dead in C++)
  { const lifted::Result o = game::rand_82293568(LIFTED_ARGS(0x822220C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,6
  r28 = uint64_t(int64_t(6));
  // divw r11,r3,r28
  r11 = uint32_t((int32_t(r28) && !(int32_t(r3) == INT32_MIN && int32_t(r28) == -1)) ? int32_t(r3) / int32_t(r28) : 0);
  // mulli r11,r11,6
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(6))));
  // subf r29,r11,r3
  r29 = r3 - r11;
  // bl 0x821021a8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceData::ResetGlobal(LIFTED_ARGS(0x822220D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-27104
  r3 = uint64_t(int64_t(int64_t(r3) + -27104));
  // bl 0x82101900
  // (left out: dead in C++)
  { const lifted::Result o = game::Stages::Clear(LIFTED_ARGS(0x822220E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // addi r8,r1,104
  r8 = uint64_t(int64_t(int64_t(r1) + 104));
  // ori r10,r11,15088
  r10 = r11 | 15088;
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // mulli r29,r29,12
  r29 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(12))));
  // lwzx r3,r29,r8
  r3 = guest::LoadU32(uint32_t(r29) + uint32_t(r8));
  // ori r9,r11,15092
  r9 = r11 | 15092;
  // lwz r11,23340(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 23340);
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // stwx r31,r11,r10
  guest::StoreU32(uint32_t(r11) + uint32_t(r10), uint32_t(r31));
  // lwz r11,23340(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 23340);
  // stwx r28,r11,r9
  guest::StoreU32(uint32_t(r11) + uint32_t(r9), uint32_t(r28));
  // bl 0x82101d38
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::SetRoute(LIFTED_ARGS(0x8222211C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r31,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r31));
  // bl 0x821128a0
  // (left out: dead in C++)
  game::Bits24::SetAll(r3);
  // lwz r10,80(r1)
  r10 = guest::LoadU32(uint32_t(r1) + 80);
  // lwz r11,23340(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 23340);
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // stw r10,20(r11)
  guest::StoreU32(schema::RaceData(uint32_t(r11)).branch_flags_address(), uint32_t(r10));
  // bl 0x820ff918
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820FF918(LIFTED_ARGS(0x8222213C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r3,r29,r11
  r3 = guest::LoadU32(uint32_t(r29) + uint32_t(r11));
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Item(LIFTED_ARGS(0x82222148, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(LIFTED_ARGS(0x82222154, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r31,160
  r4 = uint64_t(int64_t(int64_t(r31) + 160));
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x820cdce0
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::FromBuild(LIFTED_ARGS(0x82222160, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(LIFTED_ARGS(0x82222168, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // bl 0x821017c0
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::AddRacer(LIFTED_ARGS(0x82222180, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x82101828
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::AddStage(LIFTED_ARGS(0x82222188, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,100
  r11 = uint64_t(int64_t(int64_t(r1) + 100));
  // lwzx r3,r29,r11
  r3 = guest::LoadU32(uint32_t(r29) + uint32_t(r11));
  // bl 0x820ff968
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820FF968(LIFTED_ARGS(0x82222194, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82102048
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::PlaceRacers(LIFTED_ARGS(0x82222198, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,4128
  r1 = uint64_t(int64_t(int64_t(r1) + 4128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822271B8 game::LocationScreen::OnMessage15: lifted from generated/ by lift.py.
void f822271B8() {
  ITC_NATIVE_RUNNING("sub_822271B8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // lwz r10,20048(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 20048);
  // stw r9,880(r10)
  guest::StoreU32(schema::LocationScreen(uint32_t(r10)).queued_page_address(), uint32_t(r9));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // stw r8,884(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).queued_transition_address(), uint32_t(r8));
  // blr
  return;
}

// sub_822271D8 game::LocationScreen::OnMessage20: lifted from generated/ by lift.py.
void f822271D8() {
  ITC_NATIVE_RUNNING("sub_822271D8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // lwz r10,20048(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 20048);
  // stw r9,880(r10)
  guest::StoreU32(schema::LocationScreen(uint32_t(r10)).queued_page_address(), uint32_t(r9));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // stw r8,884(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).queued_transition_address(), uint32_t(r8));
  // blr
  return;
}

// sub_82227310 game::LocationScreen::NextResultTextB: lifted from generated/ by lift.py.
uint64_t f82227310(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82227310 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82227328, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-25880
  r3 = uint64_t(int64_t(int64_t(r3) + -25880));
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(LIFTED_ARGS(0x82227344, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,20072(r9)
  r11 = guest::LoadU32(uint32_t(r9) + 20072);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // bne cr6,0x822273a8
  if (!cr6.eq) goto loc_822273A8;
  // lwz r8,3924(r3)
  r8 = guest::LoadU32(schema::RacerResultsRecord(uint32_t(r3)).finish_address());
  // lwz r3,980(r3)
  r3 = guest::LoadU32(schema::RacerResultsRecord(uint32_t(r3)).e1_item_address());
  // cmpwi cr6,r8,1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 1, so);
  // stw r3,20068(r10)
  guest::StoreU32(uint32_t(r10) + 20068, uint32_t(r3));
  // beq cr6,0x82227378
  if (cr6.eq) goto loc_82227378;
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // b 0x82227398
  goto loc_82227398;
loc_82227378:
  // mulli r10,r3,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(12))));
  // lis r8,1
  r8 = uint64_t(int64_t(65536));
  // add r10,r10,r31
  r10 = r10 + r31;
  // ori r8,r8,39418
  r8 = r8 | 39418;
  // lhax r10,r10,r8
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r10) + uint32_t(r8)))));
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // addi r4,r10,10
  r4 = uint64_t(int64_t(int64_t(r10) + 10));
loc_82227398:
  // stw r4,20064(r11)
  guest::StoreU32(uint32_t(r11) + 20064, uint32_t(r4));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,20072(r9)
  guest::StoreU32(uint32_t(r9) + 20072, uint32_t(r11));
  // b 0x822273b0
  goto loc_822273B0;
loc_822273A8:
  // lwz r3,20068(r10)
  r3 = guest::LoadU32(uint32_t(r10) + 20068);
  // lwz r4,20064(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 20064);
loc_822273B0:
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82227400
  if (cr6.eq) goto loc_82227400;
  // bl 0x821171b0
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Find20(LIFTED_ARGS(0x822273BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20048);
  // lwz r11,872(r11)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).entry_offset_address());
  // add r3,r11,r3
  r3 = r11 + r3;
  // bl 0x82117210
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry20(LIFTED_ARGS(0x822273D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r3)
  r11 = guest::LoadU32(schema::LocationScreenEntry20(uint32_t(r3)).next_address());
  // lwz r10,20048(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 20048);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822273f0
  if (cr0.eq) goto loc_822273F0;
  // lwz r9,872(r10)
  r9 = guest::LoadU32(schema::LocationScreen(uint32_t(r10)).entry_offset_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // add r11,r9,r11
  r11 = r9 + r11;
  // b 0x82227418
  goto loc_82227418;
loc_822273F0:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,876(r10)
  guest::StoreU32(schema::LocationScreen(uint32_t(r10)).entry_table_address(), uint32_t(r11));
  // lwz r10,20048(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 20048);
  // b 0x82227414
  goto loc_82227414;
loc_82227400:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,20048(r10)
  r9 = guest::LoadU32(uint32_t(r10) + 20048);
  // stw r11,876(r9)
  guest::StoreU32(schema::LocationScreen(uint32_t(r9)).entry_table_address(), uint32_t(r11));
  // lwz r10,20048(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 20048);
loc_82227414:
  // mr r3,r11
  r3 = r11;
loc_82227418:
  // stw r11,872(r10)
  guest::StoreU32(schema::LocationScreen(uint32_t(r10)).entry_offset_address(), uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82227780 game::LocationScreen::PollSceneTeardown: lifted from generated/ by lift.py.
lifted::Result f82227780(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82227780 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r3,11844(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 11844);
  // lwz r11,856(r31)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r31)).teardown_started_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822277d4
  if (!cr6.eq) goto loc_822277D4;
  // bl 0x820c5798
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::LoadsDone(LIFTED_ARGS(0x822277B4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822277fc
  if (cr0.eq) goto loc_822277FC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r11,856(r31)
  guest::StoreU32(schema::LocationScreen(uint32_t(r31)).teardown_started_address(), uint32_t(r11));
  // stw r10,860(r31)
  guest::StoreU32(schema::LocationScreen(uint32_t(r31)).teardown_countdown_address(), uint32_t(r10));
  // bl 0x820c4d88
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::StartTeardown(LIFTED_ARGS(0x822277D0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822277f8
  goto loc_822277F8;
loc_822277D4:
  // lwz r11,860(r31)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r31)).teardown_countdown_address());
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,860(r31)
  guest::StoreU32(schema::LocationScreen(uint32_t(r31)).teardown_countdown_address(), uint32_t(r11));
  // ble 0x822277ec
  if (!cr0.gt) goto loc_822277EC;
  // bl 0x820c9608
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::Update(LIFTED_ARGS(0x822277E8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822277f8
  goto loc_822277F8;
loc_822277EC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // stw r11,856(r31)
  guest::StoreU32(schema::LocationScreen(uint32_t(r31)).teardown_started_address(), uint32_t(r11));
loc_822277F8:
  // mr r3,r30
  r3 = r30;
loc_822277FC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82227A50 game::LocationScreen::Create: lifted from generated/ by lift.py.
lifted::Result f82227A50(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82227A50 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-320(r1)
  ea = -320 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r25,r3
  r25 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r31,9608(r25)
  guest::StoreU32(schema::LocationScreen(uint32_t(r25)).machine_loaded_address(), uint32_t(r31));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82227A70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { r3 = schema::PageObject::New().address; f1 = 0; }  // host_alloc 0x82227A88 (PageObject)
  // mr r11,r3
  r11 = r3;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,912
  r4 = uint64_t(int64_t(912));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,20040(r10)
  guest::StoreU32(uint32_t(r10) + 20040, uint32_t(r11));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r31,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r10,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(LIFTED_ARGS(0x82227AC4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // stw r3,20048(r30)
  guest::StoreU32(uint32_t(r30) + 20048, uint32_t(r3));
  // bl 0x822279a8
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::Reset(LIFTED_ARGS(0x82227AD0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // li r22,9
  r22 = uint64_t(int64_t(9));
  // lwz r10,6108(r26)
  r10 = guest::LoadU32(schema::SaveData(uint32_t(r26)).location_address());
  // li r24,4
  r24 = uint64_t(int64_t(4));
  // li r29,7
  r29 = uint64_t(int64_t(7));
  // li r23,2
  r23 = uint64_t(int64_t(2));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // stw r10,864(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).location_address(), uint32_t(r10));
  // lwz r28,20048(r30)
  r28 = guest::LoadU32(uint32_t(r30) + 20048);
  // lwz r11,864(r28)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r28)).location_address());
  // cmpwi cr6,r11,14
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 14, so);
  // beq cr6,0x82227b7c
  if (cr6.eq) goto loc_82227B7C;
  // cmpwi cr6,r11,15
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 15, so);
  // beq cr6,0x82227b70
  if (cr6.eq) goto loc_82227B70;
  // cmpwi cr6,r11,16
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 16, so);
  // beq cr6,0x82227b64
  if (cr6.eq) goto loc_82227B64;
  // cmpwi cr6,r11,17
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 17, so);
  // beq cr6,0x82227b50
  if (cr6.eq) goto loc_82227B50;
  // cmpwi cr6,r11,18
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 18, so);
  // beq cr6,0x82227b3c
  if (cr6.eq) goto loc_82227B3C;
  // cmpwi cr6,r11,19
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 19, so);
  // bne cr6,0x82227b8c
  if (!cr6.eq) goto loc_82227B8C;
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,852(r28)
  guest::StoreU32(schema::LocationScreen(uint32_t(r28)).location_index_address(), uint32_t(r11));
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // stw r22,804(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).group_kind_address(), uint32_t(r22));
  // b 0x82227b88
  goto loc_82227B88;
loc_82227B3C:
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stw r24,852(r28)
  guest::StoreU32(schema::LocationScreen(uint32_t(r28)).location_index_address(), uint32_t(r24));
loc_82227B44:
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // stw r10,804(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).group_kind_address(), uint32_t(r10));
  // b 0x82227b88
  goto loc_82227B88;
loc_82227B50:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,852(r28)
  guest::StoreU32(schema::LocationScreen(uint32_t(r28)).location_index_address(), uint32_t(r11));
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // stw r29,804(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).group_kind_address(), uint32_t(r29));
  // b 0x82227b88
  goto loc_82227B88;
loc_82227B64:
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stw r23,852(r28)
  guest::StoreU32(schema::LocationScreen(uint32_t(r28)).location_index_address(), uint32_t(r23));
  // b 0x82227b44
  goto loc_82227B44;
loc_82227B70:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r27,852(r28)
  guest::StoreU32(schema::LocationScreen(uint32_t(r28)).location_index_address(), uint32_t(r27));
  // b 0x82227b44
  goto loc_82227B44;
loc_82227B7C:
  // stw r31,852(r28)
  guest::StoreU32(schema::LocationScreen(uint32_t(r28)).location_index_address(), uint32_t(r31));
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // stw r24,804(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).group_kind_address(), uint32_t(r24));
loc_82227B88:
  // lwz r28,20048(r30)
  r28 = guest::LoadU32(uint32_t(r30) + 20048);
loc_82227B8C:
  // lwz r11,852(r28)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r28)).location_index_address());
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r5,100
  r5 = uint64_t(int64_t(100));
  // mulli r11,r11,100
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(100))));
  // add r11,r11,r26
  r11 = r11 + r26;
  // addis r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 131072));
  // addi r4,r4,-21216
  r4 = uint64_t(int64_t(int64_t(r4) + -21216));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82227BAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,224(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 224);
  // stw r11,812(r28)
  guest::StoreU32(schema::LocationScreen(uint32_t(r28)).list_count_address(), uint32_t(r11));
  // bl 0x821434e8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::Current(LIFTED_ARGS(0x82227BB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // lwz r10,804(r11)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).group_kind_address());
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // cmplwi cr6,r10,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 5, so);
  // bgt cr6,0x82227bdc
  if (cr6.gt) goto loc_82227BDC;
  // addi r10,r10,7
  r10 = uint64_t(int64_t(int64_t(r10) + 7));
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // lhzx r10,r10,r3
  r10 = guest::LoadU16(uint32_t(r10) + uint32_t(r3));
  // b 0x82227be0
  goto loc_82227BE0;
loc_82227BDC:
  // lhz r10,84(r3)
  r10 = schema::RaceEvent(uint32_t(r3)).group4_open_bits();
loc_82227BE0:
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,68
  r4 = uint64_t(int64_t(68));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,808(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).opponent_count_address(), uint32_t(r10));
  // bl 0x82295398
  // (left out: dead in C++)
  { r3 = schema::FrontEndMenu::New().address; f1 = 0; }  // host_alloc 0x82227BFC (FrontEndMenu)
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // stw r3,20044(r11)
  guest::StoreU32(uint32_t(r11) + 20044, uint32_t(r3));
  // stw r31,12(r3)
  guest::StoreU32(uint32_t(r3) + 12, uint32_t(r31));
  // stw r24,16(r3)
  guest::StoreU32(uint32_t(r3) + 16, uint32_t(r24));
  // stw r27,20(r3)
  guest::StoreU32(uint32_t(r3) + 20, uint32_t(r27));
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // stw r31,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r31));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r31,40(r3)
  guest::StoreU32(uint32_t(r3) + 40, uint32_t(r31));
  // stw r31,48(r3)
  guest::StoreU32(uint32_t(r3) + 48, uint32_t(r31));
  // stw r31,52(r3)
  guest::StoreU32(uint32_t(r3) + 52, uint32_t(r31));
  // stw r31,36(r3)
  guest::StoreU32(uint32_t(r3) + 36, uint32_t(r31));
  // stw r31,64(r3)
  guest::StoreU32(uint32_t(r3) + 64, uint32_t(r31));
  // stw r31,24(r3)
  guest::StoreU32(uint32_t(r3) + 24, uint32_t(r31));
  // stw r31,28(r3)
  guest::StoreU32(uint32_t(r3) + 28, uint32_t(r31));
  // stw r31,32(r3)
  guest::StoreU32(uint32_t(r3) + 32, uint32_t(r31));
  // lwz r10,20048(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 20048);
  // lwz r10,816(r10)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r10)).item2_taken_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82227c5c
  if (!cr6.eq) goto loc_82227C5C;
  // lwz r11,20044(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20044);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // stw r23,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r23));
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
loc_82227C5C:
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,68
  r4 = uint64_t(int64_t(68));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { r3 = schema::FrontEndMenu::New().address; f1 = 0; }  // host_alloc 0x82227C70 (FrontEndMenu)
  // mr r11,r3
  r11 = r3;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,68
  r4 = uint64_t(int64_t(68));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,20052(r10)
  guest::StoreU32(uint32_t(r10) + 20052, uint32_t(r11));
  // lwz r10,20048(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 20048);
  // lwz r10,812(r10)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r10)).list_count_address());
  // stw r31,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r31));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r27,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r27));
  // stw r31,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  guest::StoreU32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,36(r11)
  guest::StoreU32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(uint32_t(r11) + 32, uint32_t(r31));
  // bl 0x82295398
  // (left out: dead in C++)
  { r3 = schema::FrontEndMenu::New().address; f1 = 0; }  // host_alloc 0x82227CD4 (FrontEndMenu)
  // mr r11,r3
  r11 = r3;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,68
  r4 = uint64_t(int64_t(68));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,20056(r10)
  guest::StoreU32(uint32_t(r10) + 20056, uint32_t(r11));
  // lwz r10,20048(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 20048);
  // lwz r10,808(r10)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r10)).opponent_count_address());
  // stw r31,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r31));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r31,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r31));
  // stw r31,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  guest::StoreU32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  guest::StoreU32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,52(r11)
  guest::StoreU32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  guest::StoreU32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(uint32_t(r11) + 32, uint32_t(r31));
  // bl 0x82295398
  // (left out: dead in C++)
  { r3 = schema::FrontEndMenu::New().address; f1 = 0; }  // host_alloc 0x82227D38 (FrontEndMenu)
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // stw r3,20060(r11)
  guest::StoreU32(uint32_t(r11) + 20060, uint32_t(r3));
  // stw r31,12(r3)
  guest::StoreU32(uint32_t(r3) + 12, uint32_t(r31));
  // stw r23,16(r3)
  guest::StoreU32(uint32_t(r3) + 16, uint32_t(r23));
  // stw r27,20(r3)
  guest::StoreU32(uint32_t(r3) + 20, uint32_t(r27));
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // stw r31,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r31));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r31,40(r3)
  guest::StoreU32(uint32_t(r3) + 40, uint32_t(r31));
  // stw r31,48(r3)
  guest::StoreU32(uint32_t(r3) + 48, uint32_t(r31));
  // stw r31,52(r3)
  guest::StoreU32(uint32_t(r3) + 52, uint32_t(r31));
  // stw r31,36(r3)
  guest::StoreU32(uint32_t(r3) + 36, uint32_t(r31));
  // stw r31,64(r3)
  guest::StoreU32(uint32_t(r3) + 64, uint32_t(r31));
  // stw r31,24(r3)
  guest::StoreU32(uint32_t(r3) + 24, uint32_t(r31));
  // stw r31,28(r3)
  guest::StoreU32(uint32_t(r3) + 28, uint32_t(r31));
  // stw r31,32(r3)
  guest::StoreU32(uint32_t(r3) + 32, uint32_t(r31));
  // bl 0x820f2f88
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::Create(LIFTED_ARGS(0x82227D7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,20048(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 20048);
  // lwz r11,21788(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 21788);
  // stw r11,552(r10)
  guest::StoreU32(schema::LocationScreen(uint32_t(r10)).scenery_address(), uint32_t(r11));
  // bl 0x820c8a50
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::Create(LIFTED_ARGS(0x82227D90, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r30,11844(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 11844);
  // bl 0x820d0d50
  // (left out: dead in C++)
  { const lifted::Result o = game::SceneSetUp::Ctor(LIFTED_ARGS(0x82227DA0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // std r31,0(r11)
  guest::StoreU64(uint32_t(r11) + 0, r31);
  // std r31,8(r11)
  guest::StoreU64(uint32_t(r11) + 8, r31);
  // std r31,16(r11)
  guest::StoreU64(uint32_t(r11) + 16, r31);
  // std r31,24(r11)
  guest::StoreU64(uint32_t(r11) + 24, r31);
  // std r31,32(r11)
  guest::StoreU64(uint32_t(r11) + 32, r31);
  // stw r31,40(r11)
  guest::StoreU32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r27,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r27));
  // stw r29,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r29));
  // stw r29,88(r1)
  guest::StoreU32(uint32_t(r1) + 88, uint32_t(r29));
  // stw r29,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r29));
  // stw r29,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r29));
  // stw r29,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r29));
  // stw r29,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r29));
  // stw r31,112(r1)
  guest::StoreU32(uint32_t(r1) + 112, uint32_t(r31));
  // bl 0x820c8de8
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::SetUp(LIFTED_ARGS(0x82227DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(LIFTED_ARGS(0x82227DF4, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820ab358
  // (left out: dead in C++)
  { const lifted::Result o = game::Viewports::Init(LIFTED_ARGS(0x82227DF8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820aa1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraPasses::EnableForView(LIFTED_ARGS(0x82227DFC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82150078
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraAnims::Clear(LIFTED_ARGS(0x82227E00, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9612(r25)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r25)).machine_reenter_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82227e14
  if (cr6.eq) goto loc_82227E14;
  // stw r27,9604(r25)
  guest::StoreU32(schema::LocationScreen(uint32_t(r25)).machine_state_address(), uint32_t(r27));
  // b 0x82227e18
  goto loc_82227E18;
loc_82227E14:
  // stw r22,9604(r25)
  guest::StoreU32(schema::LocationScreen(uint32_t(r25)).machine_state_address(), uint32_t(r22));
loc_82227E18:
  // addi r1,r1,320
  r1 = uint64_t(int64_t(int64_t(r1) + 320));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82228068 game::LocationScreen::Page8: lifted from generated/ by lift.py.
uint64_t f82228068(uint64_t r1, uint64_t r13, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82228068 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r31,r4
  r31 = r4;
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82228084, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // lwz r11,852(r11)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).location_index_address());
  // addi r11,r11,2943
  r11 = uint64_t(int64_t(int64_t(r11) + 2943));
  // rlwinm r10,r11,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // lhax r11,r10,r3
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r10) + uint32_t(r3)))));
  // cmpwi cr6,r11,9999
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9999, so);
  // bge cr6,0x822280ac
  if (!cr6.lt) goto loc_822280AC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // sthx r11,r10,r3
  guest::StoreU16(uint32_t(r10) + uint32_t(r3), uint16_t(r11));
loc_822280AC:
  // bl 0x82116588
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82116588, LIFTED_ARGS(0x822280B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,13
  r11 = uint64_t(int64_t(13));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // stw r11,9620(r31)
  guest::StoreU32(schema::LocationScreenMachine(uint32_t(r31)).next_state_address(), uint32_t(r11));
  // stw r10,9604(r31)
  guest::StoreU32(schema::LocationScreenMachine(uint32_t(r31)).state_address(), uint32_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82228220 game::LocationScreen::Free: lifted from generated/ by lift.py.
lifted::Result f82228220(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82228220 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r26,-32127
  r26 = uint64_t(int64_t(-2105475072));
  // lwz r3,20048(r26)
  r3 = guest::LoadU32(uint32_t(r26) + 20048);
  // bl 0x82227780
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::PollSceneTeardown(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822282dc
  if (cr0.eq) goto loc_822282DC;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // ori r31,r11,32768
  r31 = r11 | 32768;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r5,r31
  r5 = r31;
  // lwz r3,20040(r27)
  r3 = guest::LoadU32(uint32_t(r27) + 20040);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(LIFTED_ARGS(0x8222825C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // mr r5,r31
  r5 = r31;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,20044(r28)
  r3 = guest::LoadU32(uint32_t(r28) + 20044);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(LIFTED_ARGS(0x82228270, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r5,r31
  r5 = r31;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,20052(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 20052);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(LIFTED_ARGS(0x82228284, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r31
  r5 = r31;
  // lwz r3,20048(r26)
  r3 = guest::LoadU32(uint32_t(r26) + 20048);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(LIFTED_ARGS(0x82228294, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r5,r31
  r5 = r31;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,20056(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 20056);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(LIFTED_ARGS(0x822282A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r31
  r5 = r31;
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,20060(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20060);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(LIFTED_ARGS(0x822282BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,20040(r27)
  guest::StoreU32(uint32_t(r27) + 20040, uint32_t(r11));
  // stw r11,20044(r28)
  guest::StoreU32(uint32_t(r28) + 20044, uint32_t(r11));
  // stw r11,20048(r26)
  guest::StoreU32(uint32_t(r26) + 20048, uint32_t(r11));
  // stw r11,20052(r29)
  guest::StoreU32(uint32_t(r29) + 20052, uint32_t(r11));
  // stw r11,20056(r30)
  guest::StoreU32(uint32_t(r30) + 20056, uint32_t(r11));
  // stw r11,20060(r31)
  guest::StoreU32(uint32_t(r31) + 20060, uint32_t(r11));
loc_822282DC:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822282E8 game::CourseView::Draw: lifted from generated/ by lift.py.
lifted::Result f822282E8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_822282E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82228318, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82228328, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-21600(r30)
  r11 = guest::LoadU32(uint32_t(r30) + -21600);
  // mr r30,r3
  r30 = r3;
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = lifted::VtableWord(uint32_t(r11));
  // lwz r11,32(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82228344, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82228354, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // stw r3,-18152(r10)
  guest::StoreU32(uint32_t(r10) + -18152, uint32_t(r3));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r3,r11
  r3 = r11;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // lfs f13,2608(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // stfs f0,-18188(r10)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r10) + -18188, temp.u32);
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // stw r30,-18148(r10)
  guest::StoreU32(uint32_t(r10) + -18148, uint32_t(r30));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,11496(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 11496);
  // lwz r30,0(r10)
  r30 = schema::Viewports(uint32_t(r10)).first_bits();
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 96, temp.u32);
  // stfs f13,100(r1)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r1) + 100, temp.u32);
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 104, temp.u32);
  // stfs f13,108(r1)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r1) + 108, temp.u32);
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 80, temp.u32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 84, temp.u32);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 88, temp.u32);
  // stfs f13,92(r1)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r1) + 92, temp.u32);
  // bl 0x820a0968
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraPass::Enable(LIFTED_ARGS(0x822283B8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r5,r31,608
  r5 = uint64_t(int64_t(int64_t(r31) + 608));
  // addi r4,r31,592
  r4 = uint64_t(int64_t(int64_t(r31) + 592));
  // mr r3,r30
  r3 = r30;
  // bl 0x820aae48
  // (left out: dead in C++)
  { const lifted::Result o = game::Viewport::Set(LIFTED_ARGS(0x822283D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r3,r30
  r3 = r30;
  // lfs f1,8748(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 8748);
  f1 = double(temp.f32);
  // bl 0x820aa368
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraPasses::SetNear(LIFTED_ARGS(0x822283E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r3,r30
  r3 = r30;
  // lfs f0,8140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 8140);
  f0 = double(temp.f32);
  // stfs f0,72(r30)
  temp.f32 = float(f0);
  guest::StoreU32(schema::Viewport(uint32_t(r30)).entries_address() + 72, temp.u32);
  // lfs f1,664(r31)
  temp.u32 = guest::LoadU32(schema::CourseView(uint32_t(r31)).unknown_664_address());
  f1 = double(temp.f32);
  // bl 0x820aa2d0
  // (left out: dead in C++)
  { const lifted::Result o = game::Viewport::SetRoll(LIFTED_ARGS(0x822283F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // lfs f1,668(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(schema::CourseView(uint32_t(r31)).unknown_668_address());
  f1 = double(temp.f32);
  // bl 0x820aae90
  // (left out: dead in C++)
  { const lifted::Result o = game::Viewport::SetZoom(LIFTED_ARGS(0x82228404, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8209f8a0
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraPasses::RecordBegins(LIFTED_ARGS(0x82228408, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8209f900
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraPasses::RecordEnds(LIFTED_ARGS(0x8222840C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,552(r31)
  r3 = guest::LoadU32(schema::CourseView(uint32_t(r31)).scenery_address());
  // bl 0x820f4558
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::Update(LIFTED_ARGS(0x82228414, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,552(r31)
  r3 = guest::LoadU32(schema::CourseView(uint32_t(r31)).scenery_address());
  // bl 0x820f6068
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::Draw(LIFTED_ARGS(0x8222841C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82228548 game::LocationScreen::Destroy: lifted from generated/ by lift.py.
lifted::Result f82228548(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82228548 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,9620(r3)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r3)).machine_next_state_address());
  // stw r11,9604(r3)
  guest::StoreU32(schema::LocationScreen(uint32_t(r3)).machine_state_address(), uint32_t(r11));
  // bl 0x821507d8
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraAnims::FreeAll(LIFTED_ARGS(0x82228560, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820aa430
  // (left out: dead in C++)
  { const lifted::Result o = game::Viewports::Release(LIFTED_ARGS(0x82228564, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820f2af8
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::Destroy(LIFTED_ARGS(0x82228568, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,20048(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 20048);
  // bl 0x82228498
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::FreePictures(LIFTED_ARGS(0x82228574, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82228A90 game::LocationScreen::Page1: lifted from generated/ by lift.py.
lifted::Result f82228A90(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82228A90 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r3,20052(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20052);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82228AA8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,20052(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20052);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82228AB4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // beq 0x82228aec
  if (cr0.eq) goto loc_82228AEC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228ACC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82228ae0
  if (cr6.eq) goto loc_82228AE0;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228AE0:
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // b 0x82228b24
  goto loc_82228B24;
loc_82228AEC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,20052(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20052);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82228AF8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82228b34
  if (cr0.eq) goto loc_82228B34;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228B08, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82228b1c
  if (cr0.eq) goto loc_82228B1C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228B1C:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_82228B24:
  // stw r29,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
loc_82228B34:
  // lwz r11,20052(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20052);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82228b78
  if (cr6.eq) goto loc_82228B78;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228B4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82228b60
  if (cr6.eq) goto loc_82228B60;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228B60:
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r29,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
loc_82228B78:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82228B80 game::LocationScreen::Page3: lifted from generated/ by lift.py.
lifted::Result f82228B80(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82228B80 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r3,20056(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20056);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82228B98, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,20056(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20056);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82228BA4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // beq 0x82228bdc
  if (cr0.eq) goto loc_82228BDC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228BBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x82228bd0
  if (cr6.eq) goto loc_82228BD0;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228BD0:
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // b 0x82228c14
  goto loc_82228C14;
loc_82228BDC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,20056(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20056);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82228BE8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82228c24
  if (cr0.eq) goto loc_82228C24;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228BF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82228c0c
  if (cr0.eq) goto loc_82228C0C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228C0C:
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_82228C14:
  // stw r29,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
loc_82228C24:
  // lwz r11,20056(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20056);
  // lwz r11,36(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 36);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82228cb4
  if (cr0.eq) goto loc_82228CB4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82228c58
  if (!cr6.eq) goto loc_82228C58;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // lwz r10,808(r11)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).opponent_count_address());
  // lwz r11,800(r11)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).picked_opponent_address());
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // b 0x82228c70
  goto loc_82228C70;
loc_82228C58:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82228cb4
  if (!cr6.eq) goto loc_82228CB4;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // lwz r11,800(r11)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).picked_opponent_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
loc_82228C70:
  // bne cr6,0x82228c80
  if (!cr6.eq) goto loc_82228C80;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228C7C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82228cb4
  goto loc_82228CB4;
loc_82228C80:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228C88, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82228c9c
  if (cr6.eq) goto loc_82228C9C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228C9C:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r29,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r29));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
loc_82228CB4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82228CC0 game::LocationScreen::Page5: lifted from generated/ by lift.py.
lifted::Result f82228CC0(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82228CC0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r3,20060(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 20060);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82228CD8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,20060(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 20060);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82228CE4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // beq 0x82228d80
  if (cr0.eq) goto loc_82228D80;
  // lwz r11,20060(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20060);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82228d58
  if (cr6.lt) goto loc_82228D58;
  // beq cr6,0x82228d18
  if (cr6.eq) goto loc_82228D18;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82228dc8
  if (!cr6.lt) goto loc_82228DC8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82228d98
  goto loc_82228D98;
loc_82228D18:
  // bl 0x82227708
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::OpponentHasCard(LIFTED_ARGS(0x82228D1C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82228d4c
  if (cr0.eq) goto loc_82228D4C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228D2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x82228d40
  if (cr6.eq) goto loc_82228D40;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228D40:
  // li r10,17
  r10 = uint64_t(int64_t(17));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // b 0x82228db8
  goto loc_82228DB8;
loc_82228D4C:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228D54, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82228dc8
  goto loc_82228DC8;
loc_82228D58:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228D60, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // beq cr6,0x82228d74
  if (cr6.eq) goto loc_82228D74;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228D74:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // b 0x82228db8
  goto loc_82228DB8;
loc_82228D80:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,20060(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 20060);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82228D8C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82228dc8
  if (cr0.eq) goto loc_82228DC8;
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_82228D98:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228D9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82228db0
  if (cr6.eq) goto loc_82228DB0;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228DB0:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r10,9
  r10 = uint64_t(int64_t(9));
loc_82228DB8:
  // stw r29,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
loc_82228DC8:
  // lwz r11,20060(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20060);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82228e0c
  if (cr6.eq) goto loc_82228E0C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228DE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20040(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82228df4
  if (cr6.eq) goto loc_82228DF4;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228DF4:
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r29,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
loc_82228E0C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82228E18 game::LocationScreen::Page14: lifted from generated/ by lift.py.
lifted::Result f82228E18(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82228E18 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,20044(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 20044);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82228E34, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82228e78
  if (cr0.eq) goto loc_82228E78;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82228E44, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82228e5c
  if (cr6.eq) goto loc_82228E5C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82228E5C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
loc_82228E78:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82229578 game::LocationScreen::OnMessage6: lifted from generated/ by lift.py.
lifted::Result f82229578(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82229578 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r31,r11,23756
  r31 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822295AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(LIFTED_ARGS(0x822295B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,30
  r11 = uint64_t(int64_t(30));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822295C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(LIFTED_ARGS(0x822295D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r30)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r30)).machine_delete_movie_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822295f4
  if (cr6.eq) goto loc_822295F4;
  // li r3,21
  r3 = uint64_t(int64_t(21));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(LIFTED_ARGS(0x822295E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,30
  r3 = uint64_t(int64_t(30));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(LIFTED_ARGS(0x822295EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9608(r30)
  guest::StoreU32(schema::LocationScreen(uint32_t(r30)).machine_loaded_address(), uint32_t(r11));
loc_822295F4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82229610 game::LocationScreen::OnMessage3: lifted from generated/ by lift.py.
void f82229610(uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82229610 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).away_state_address());
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82229670
  if (!cr6.eq) goto loc_82229670;
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = schema::LocationScreenDispatcher(uint32_t(r11) - 204).message_count_bits();
  // lwz r6,604(r11)
  r6 = schema::LocationScreenDispatcher(uint32_t(r11) - 204).message_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::LocationScreenDispatcher(uint32_t(r11) - 204).set_message_count_bits(uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r7));
loc_82229670:
  // stw r9,9604(r3)
  guest::StoreU32(schema::LocationScreen(uint32_t(r3)).machine_state_address(), uint32_t(r9));
  // blr
  return;
}

// sub_82229678 game::LocationScreen::Unload: lifted from generated/ by lift.py.
lifted::Result f82229678(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82229678 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r29,r10,23756
  r29 = uint64_t(int64_t(int64_t(r10) + 23756));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // li r10,21
  r10 = uint64_t(int64_t(21));
  // mr r28,r4
  r28 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r29
  r3 = r29;
  // stw r9,9604(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).machine_state_address(), uint32_t(r9));
  // stw r10,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r10));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822296B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x82229700
  if (cr0.eq) goto loc_82229700;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(LIFTED_ARGS(0x822296C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // lwz r6,544(r11)
  r6 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).default_texture_a_address());
  // bl 0x8211aaa8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetPictureTexture(LIFTED_ARGS(0x822296E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // lwz r6,548(r11)
  r6 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).default_texture_b_address());
  // bl 0x8211aaa8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetPictureTexture(LIFTED_ARGS(0x822296F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,21
  r3 = uint64_t(int64_t(21));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(LIFTED_ARGS(0x82229700, r31)); r3 = o.r3; f1 = o.f1; }
loc_82229700:
  // li r11,30
  r11 = uint64_t(int64_t(30));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r29
  r3 = r29;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82229714, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8222972c
  if (cr0.eq) goto loc_8222972C;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(LIFTED_ARGS(0x82229724, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,30
  r3 = uint64_t(int64_t(30));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(LIFTED_ARGS(0x8222972C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222972C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x8209db90
  // (left out: dead in C++)
  { const lifted::Result o = game::Archive::CancelQueued(LIFTED_ARGS(0x82229734, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // lwz r3,552(r11)
  r3 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).scenery_address());
  // bl 0x820f2b38
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::Unload(LIFTED_ARGS(0x82229740, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r28,204
  r11 = uint64_t(int64_t(int64_t(r28) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = schema::LocationScreenDispatcher(uint32_t(r11) - 204).message_count_bits();
  // lwz r6,604(r11)
  r6 = schema::LocationScreenDispatcher(uint32_t(r11) - 204).message_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::LocationScreenDispatcher(uint32_t(r11) - 204).set_message_count_bits(uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r7));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82229798 game::LocationScreen::Page2: lifted from generated/ by lift.py.
lifted::Result f82229798(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82229798 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822297BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r30,0(r3)
  r30 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r11,20052(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20052);
  // stw r29,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r29));
  // lwz r3,20052(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20052);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x822297D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,20052(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20052);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x822297E4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8222987c
  if (cr0.eq) goto loc_8222987C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x822297F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d880
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::RevealText(LIFTED_ARGS(0x82229804, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8222984c
  if (!cr0.eq) goto loc_8222984C;
  // lwz r11,20052(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20052);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82229830
  if (cr6.eq) goto loc_82229830;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229830:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r29));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x8222987c
  goto loc_8222987C;
loc_8222984C:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d8d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211D8D8(LIFTED_ARGS(0x8222985C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // beq 0x82229878
  if (cr0.eq) goto loc_82229878;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82229874, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8222987c
  goto loc_8222987C;
loc_82229878:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8222987C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222987C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82229888 game::LocationScreen::Page4: lifted from generated/ by lift.py.
lifted::Result f82229888(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82229888 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-3136(r1)
  ea = -3136 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822298B4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r30,0(r11)
  r30 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r11)).value_address());
  // lwz r3,20056(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20056);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x822298C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,20056(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20056);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x822298D4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229a20
  if (cr0.eq) goto loc_82229A20;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x822298E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d880
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::RevealText(LIFTED_ARGS(0x822298F4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x822299ec
  if (!cr0.eq) goto loc_822299EC;
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20048);
  // lwz r11,800(r11)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).picked_opponent_address());
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x821434e8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::Current(LIFTED_ARGS(0x82229910, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20048);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,804
  r4 = uint64_t(int64_t(int64_t(r11) + 804));
  // bl 0x821431c0
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::GroupItem(LIFTED_ARGS(0x82229920, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82229930, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82227430
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::NextOpponentText(LIFTED_ARGS(0x82229934, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20048);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229984
  if (cr0.eq) goto loc_82229984;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,888(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).more_opponent_text_address(), uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x82229960
  if (cr6.eq) goto loc_82229960;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229960:
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x82229a2c
  goto loc_82229A2C;
loc_82229984:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lha r3,96(r1)
  r3 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r1) + 96))));
  // stw r10,888(r11)
  guest::StoreU32(uint32_t(r11) + 888, uint32_t(r10));
  // bl 0x821435c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Items::Available(LIFTED_ARGS(0x82229994, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // beq 0x822299c0
  if (cr0.eq) goto loc_822299C0;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x822299b4
  if (cr6.eq) goto loc_822299B4;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_822299B4:
  // li r9,14
  r9 = uint64_t(int64_t(14));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // b 0x822299d4
  goto loc_822299D4;
loc_822299C0:
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x822299cc
  if (cr6.eq) goto loc_822299CC;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_822299CC:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // li r8,3
  r8 = uint64_t(int64_t(3));
loc_822299D4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x82229a2c
  goto loc_82229A2C;
loc_822299EC:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d8d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211D8D8(LIFTED_ARGS(0x822299FC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // beq 0x82229a18
  if (cr0.eq) goto loc_82229A18;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82229A14, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82229a2c
  goto loc_82229A2C;
loc_82229A18:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82229A1C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82229a2c
  goto loc_82229A2C;
loc_82229A20:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,20056(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20056);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82229A2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82229A2C:
  // addi r1,r1,3136
  r1 = uint64_t(int64_t(int64_t(r1) + 3136));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82229A48 game::LocationScreen::Page6: lifted from generated/ by lift.py.
lifted::Result f82229A48(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82229A48 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // mr r30,r3
  r30 = r3;
  // lwz r3,20060(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20060);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82229A6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,20060(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20060);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82229A78, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82229a94
  if (!cr0.eq) goto loc_82229A94;
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,20060(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20060);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82229A8C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229ad0
  if (cr0.eq) goto loc_82229AD0;
loc_82229A94:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82229A9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82229ab4
  if (cr6.eq) goto loc_82229AB4;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229AB4:
  // li r9,18
  r9 = uint64_t(int64_t(18));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
loc_82229AD0:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r11) - 204).group_kind_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(schema::LocationScreen(uint32_t(r11) - 204).opponent_count_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11) - 204).group_kind_address(), uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r7));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82229B38 game::LocationScreen::Page7: lifted from generated/ by lift.py.
void f82229B38(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82229B38 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // lwz r10,20048(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 20048);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r6,4(r10)
  guest::StoreU32(schema::LocationScreen(uint32_t(r10)).away_state_address(), uint32_t(r6));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r11) - 204).group_kind_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(schema::LocationScreen(uint32_t(r11) - 204).opponent_count_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11) - 204).group_kind_address(), uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r7));
  // blr
  return;
}

// sub_82229B98 game::LocationScreen::Page15: lifted from generated/ by lift.py.
lifted::Result f82229B98(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82229B98 (lifted)");
  uint64_t r3 = 0, r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82229BC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r31,0(r11)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r11)).value_address());
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82229BD4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // addis r30,r11,2
  r30 = uint64_t(int64_t(int64_t(r11) + 131072));
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // addi r30,r30,-14472
  r30 = uint64_t(int64_t(int64_t(r30) + -14472));
  // lwz r3,20044(r10)
  r3 = guest::LoadU32(uint32_t(r10) + 20044);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82229BF0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229cd8
  if (cr0.eq) goto loc_82229CD8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82229C00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d880
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::RevealText(LIFTED_ARGS(0x82229C10, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82229ca8
  if (!cr0.eq) goto loc_82229CA8;
  // bl 0x82227310
  // (left out: dead in C++)
  r3 = game::LocationScreen::NextResultTextB(r1, r13);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229c48
  if (cr0.eq) goto loc_82229C48;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,15
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 15, so);
  // beq cr6,0x82229c3c
  if (cr6.eq) goto loc_82229C3C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229C3C:
  // li r9,32
  r9 = uint64_t(int64_t(32));
  // li r8,15
  r8 = uint64_t(int64_t(15));
  // b 0x82229c8c
  goto loc_82229C8C;
loc_82229C48:
  // lwz r11,768(r30)
  r11 = guest::LoadU32(schema::SaveData(uint32_t(r30) - 116600).unlocks_shown_count_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // bne cr6,0x82229c78
  if (!cr6.eq) goto loc_82229C78;
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82229c6c
  if (cr0.eq) goto loc_82229C6C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229C6C:
  // li r9,29
  r9 = uint64_t(int64_t(29));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // b 0x82229c8c
  goto loc_82229C8C;
loc_82229C78:
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x82229c84
  if (cr6.eq) goto loc_82229C84;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229C84:
  // li r9,25
  r9 = uint64_t(int64_t(25));
  // li r8,13
  r8 = uint64_t(int64_t(13));
loc_82229C8C:
  // li r10,24
  r10 = uint64_t(int64_t(24));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x82229cd8
  goto loc_82229CD8;
loc_82229CA8:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d8d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211D8D8(LIFTED_ARGS(0x82229CB8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // beq 0x82229cd4
  if (cr0.eq) goto loc_82229CD4;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82229CD0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82229cd8
  goto loc_82229CD8;
loc_82229CD4:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82229CD8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82229CD8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82229CF0 game::LocationScreen::Page12: lifted from generated/ by lift.py.
lifted::Result f82229CF0(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82229CF0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82229D1C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r31,0(r11)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r11)).value_address());
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82229D2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // addis r30,r11,2
  r30 = uint64_t(int64_t(int64_t(r11) + 131072));
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // addi r30,r30,-14472
  r30 = uint64_t(int64_t(int64_t(r30) + -14472));
  // lwz r3,20044(r10)
  r3 = guest::LoadU32(uint32_t(r10) + 20044);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82229D48, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229e80
  if (cr0.eq) goto loc_82229E80;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82229D58, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d880
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::RevealText(LIFTED_ARGS(0x82229D68, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82229e50
  if (!cr0.eq) goto loc_82229E50;
  // bl 0x822271f8
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::NextResultText(LIFTED_ARGS(0x82229D74, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229da4
  if (cr0.eq) goto loc_82229DA4;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x82229d94
  if (cr6.eq) goto loc_82229D94;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229D94:
  // li r10,34
  r10 = uint64_t(int64_t(34));
  // li r9,33
  r9 = uint64_t(int64_t(33));
  // li r8,12
  r8 = uint64_t(int64_t(12));
  // b 0x82229e38
  goto loc_82229E38;
loc_82229DA4:
  // lwz r11,768(r30)
  r11 = guest::LoadU32(schema::SaveData(uint32_t(r30) - 116600).unlocks_shown_count_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82229e14
  if (!cr6.eq) goto loc_82229E14;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // lwz r11,820(r11)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).won_card_item_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // beq cr6,0x82229dfc
  if (cr6.eq) goto loc_82229DFC;
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x82229ddc
  if (cr6.eq) goto loc_82229DDC;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229DDC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,24
  r9 = uint64_t(int64_t(24));
  // li r8,8
  r8 = uint64_t(int64_t(8));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x82229e80
  goto loc_82229E80;
loc_82229DFC:
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82229e08
  if (cr0.eq) goto loc_82229E08;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229E08:
  // li r9,29
  r9 = uint64_t(int64_t(29));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // b 0x82229e34
  goto loc_82229E34;
loc_82229E14:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x82229e2c
  if (cr6.eq) goto loc_82229E2C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229E2C:
  // li r9,25
  r9 = uint64_t(int64_t(25));
  // li r8,13
  r8 = uint64_t(int64_t(13));
loc_82229E34:
  // li r10,24
  r10 = uint64_t(int64_t(24));
loc_82229E38:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x82229e80
  goto loc_82229E80;
loc_82229E50:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d8d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211D8D8(LIFTED_ARGS(0x82229E60, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // beq 0x82229e7c
  if (cr0.eq) goto loc_82229E7C;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82229E78, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82229e80
  goto loc_82229E80;
loc_82229E7C:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82229E80, r31)); r3 = o.r3; f1 = o.f1; }
loc_82229E80:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82229E98 game::LocationScreen::Page13: lifted from generated/ by lift.py.
lifted::Result f82229E98(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82229E98 (lifted)");
  uint64_t r3 = 0, r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82229EC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r31,0(r11)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r11)).value_address());
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82229ED4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // addis r30,r11,2
  r30 = uint64_t(int64_t(int64_t(r11) + 131072));
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // addi r30,r30,-14472
  r30 = uint64_t(int64_t(int64_t(r30) + -14472));
  // lwz r3,20044(r10)
  r3 = guest::LoadU32(uint32_t(r10) + 20044);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82229EF0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8222a048
  if (cr0.eq) goto loc_8222A048;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82229F00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d880
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::RevealText(LIFTED_ARGS(0x82229F10, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8222a018
  if (!cr0.eq) goto loc_8222A018;
  // bl 0x82227618
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::NextUnlockEntry(LIFTED_ARGS(0x82229F1C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229f48
  if (cr0.eq) goto loc_82229F48;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x82229f3c
  if (cr6.eq) goto loc_82229F3C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229F3C:
  // li r10,31
  r10 = uint64_t(int64_t(31));
  // li r9,30
  r9 = uint64_t(int64_t(30));
  // b 0x82229f74
  goto loc_82229F74;
loc_82229F48:
  // bl 0x822281d8
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::HasMoreUnlocks(LIFTED_ARGS(0x82229F4C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82229f90
  if (cr0.eq) goto loc_82229F90;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x82229f6c
  if (cr6.eq) goto loc_82229F6C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229F6C:
  // li r10,26
  r10 = uint64_t(int64_t(26));
  // li r9,25
  r9 = uint64_t(int64_t(25));
loc_82229F74:
  // li r8,13
  r8 = uint64_t(int64_t(13));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r9,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x8222a048
  goto loc_8222A048;
loc_82229F90:
  // lis r8,-32127
  r8 = uint64_t(int64_t(-2105475072));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r11,20048(r8)
  r11 = guest::LoadU32(uint32_t(r8) + 20048);
  // lwz r11,820(r11)
  r11 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).won_card_item_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // beq cr6,0x82229fe0
  if (cr6.eq) goto loc_82229FE0;
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x82229fc0
  if (cr6.eq) goto loc_82229FC0;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229FC0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r7,26
  r7 = uint64_t(int64_t(26));
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r7,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r7));
  // stw r6,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r6));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x8222a008
  goto loc_8222A008;
loc_82229FE0:
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82229fec
  if (cr0.eq) goto loc_82229FEC;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82229FEC:
  // li r10,26
  r10 = uint64_t(int64_t(26));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // li r7,29
  r7 = uint64_t(int64_t(29));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r7,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r7));
  // stw r6,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r6));
loc_8222A008:
  // lwz r11,20048(r8)
  r11 = guest::LoadU32(uint32_t(r8) + 20048);
  // stw r9,868(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).unlock_position_address(), uint32_t(r9));
  // stw r9,768(r30)
  guest::StoreU32(schema::SaveData(uint32_t(r30) - 116600).unlocks_shown_count_address(), uint32_t(r9));
  // b 0x8222a048
  goto loc_8222A048;
loc_8222A018:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d8d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211D8D8(LIFTED_ARGS(0x8222A028, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // beq 0x8222a044
  if (cr0.eq) goto loc_8222A044;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x8222A040, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8222a048
  goto loc_8222A048;
loc_8222A044:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8222A048, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222A048:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8222A218 game::LocationScreen::Transition32: lifted from generated/ by lift.py.
lifted::Result f8222A218(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222A218 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-3168(r1)
  ea = -3168 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
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
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222A240, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(uint32_t(r26) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222a408
  if (!cr6.eq) goto loc_8222A408;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x8222A258, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-25880
  r3 = uint64_t(int64_t(int64_t(r3) + -25880));
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(LIFTED_ARGS(0x8222A274, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r30,980(r29)
  r30 = guest::LoadU32(uint32_t(r29) + 980);
  // mr r3,r30
  r3 = r30;
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Item(LIFTED_ARGS(0x8222A284, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8222A294, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3924(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 3924);
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // beq cr6,0x8222a2b0
  if (cr6.eq) goto loc_8222A2B0;
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // b 0x8222a300
  goto loc_8222A300;
loc_8222A2B0:
  // mulli r11,r30,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(12))));
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // add r11,r11,r28
  r11 = r11 + r28;
  // ori r10,r10,39418
  r10 = r10 | 39418;
  // lhzx r11,r11,r10
  r11 = guest::LoadU16(uint32_t(r11) + uint32_t(r10));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8222a2fc
  if (cr6.eq) goto loc_8222A2FC;
  // lbz r11,131(r1)
  r11 = guest::LoadU8(uint32_t(r1) + 131);
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x8222a2f0
  if (cr6.eq) goto loc_8222A2F0;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x8222a2f0
  if (cr6.eq) goto loc_8222A2F0;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x8222a300
  if (!cr6.eq) goto loc_8222A300;
loc_8222A2F0:
  // lwz r11,20048(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20048);
  // stw r27,820(r11)
  guest::StoreU32(uint32_t(r11) + 820, uint32_t(r27));
  // b 0x8222a300
  goto loc_8222A300;
loc_8222A2FC:
  // li r4,11
  r4 = uint64_t(int64_t(11));
loc_8222A300:
  // mr r3,r30
  r3 = r30;
  // bl 0x821171b0
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Find20(LIFTED_ARGS(0x8222A308, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20048);
  // lwz r11,872(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 872);
  // add r3,r11,r3
  r3 = r11 + r3;
  // bl 0x82117210
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry20(LIFTED_ARGS(0x8222A318, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20048);
  // mr r30,r3
  r30 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lwz r11,872(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 872);
  // addi r6,r11,5918
  r6 = uint64_t(int64_t(int64_t(r11) + 5918));
  // lwz r11,12(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 12);
  // addi r29,r11,4650
  r29 = uint64_t(int64_t(int64_t(r11) + 4650));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222A340, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r29
  r6 = r29;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222A354, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8222A364, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 16);
  // bl 0x82229108
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::PictureA(LIFTED_ARGS(0x8222A36C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211aaa8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetPictureTexture(LIFTED_ARGS(0x8222A380, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 16);
  // bl 0x82229158
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::PictureB(LIFTED_ARGS(0x8222A388, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211aaa8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetPictureTexture(LIFTED_ARGS(0x8222A39C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222A3A8, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222A3C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x8222A3CC, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::ScreenFades::FadeIn(LIFTED_ARGS(0x8222A3E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,95
  r4 = uint64_t(int64_t(95));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222A400, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222a150
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222A150, LIFTED_ARGS(0x8222A404, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r27,0(r26)
  guest::StoreU32(uint32_t(r26) + 0, uint32_t(r27));
loc_8222A408:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222A414, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x8222a464
  if (cr0.eq) goto loc_8222A464;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d8d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211D8D8(LIFTED_ARGS(0x8222A444, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // beq 0x8222a460
  if (cr0.eq) goto loc_8222A460;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x8222A45C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8222a464
  goto loc_8222A464;
loc_8222A460:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8222A464, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222A464:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,3168
  r1 = uint64_t(int64_t(int64_t(r1) + 3168));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8222AAB0 game::LocationScreen::Transition30: lifted from generated/ by lift.py.
lifted::Result f8222AAB0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222AAB0 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222AAD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222abc4
  if (!cr6.eq) goto loc_8222ABC4;
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20048);
  // lwz r4,872(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 872);
  // lwz r3,876(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 876);
  // bl 0x82117190
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry56(LIFTED_ARGS(0x8222AAFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 12);
  // addi r6,r11,4650
  r6 = uint64_t(int64_t(int64_t(r11) + 4650));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222AB18, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20048);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lwz r11,872(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 872);
  // addi r6,r11,6683
  r6 = uint64_t(int64_t(int64_t(r11) + 6683));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222AB34, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 16);
  // bl 0x82229108
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::PictureA(LIFTED_ARGS(0x8222AB3C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211aaa8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetPictureTexture(LIFTED_ARGS(0x8222AB50, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 16);
  // bl 0x82229158
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::PictureB(LIFTED_ARGS(0x8222AB58, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211aaa8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetPictureTexture(LIFTED_ARGS(0x8222AB6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222AB78, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222AB94, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x8222AB9C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,95
  r4 = uint64_t(int64_t(95));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222ABB8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222a150
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222A150, LIFTED_ARGS(0x8222ABBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(uint32_t(r28) + 0, uint32_t(r11));
loc_8222ABC4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222ABD0, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x8222ac0c
  if (cr0.eq) goto loc_8222AC0C;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d8d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211D8D8(LIFTED_ARGS(0x8222ABEC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // beq 0x8222ac08
  if (cr0.eq) goto loc_8222AC08;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x8222AC04, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8222ac0c
  goto loc_8222AC0C;
loc_8222AC08:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8222AC0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222AC0C:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8222AC78 game::LocationScreen::Transition27: lifted from generated/ by lift.py.
lifted::Result f8222AC78(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222AC78 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-3136(r1)
  ea = -3136 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222ACA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222ad30
  if (!cr6.eq) goto loc_8222AD30;
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20048);
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x821434e8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::Current(LIFTED_ARGS(0x8222ACC4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20048);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,804
  r4 = uint64_t(int64_t(int64_t(r11) + 804));
  // bl 0x821431c0
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::GroupItem(LIFTED_ARGS(0x8222ACD4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8222ACE4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222AD00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // lha r3,96(r1)
  r3 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r1) + 96))));
  // bl 0x821171b0
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Find20(LIFTED_ARGS(0x8222AD0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // bl 0x82117210
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry20(LIFTED_ARGS(0x8222AD14, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r31,5918
  r6 = uint64_t(int64_t(int64_t(r31) + 5918));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222AD28, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(uint32_t(r29) + 0, uint32_t(r11));
loc_8222AD30:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222AD3C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,3136
  r1 = uint64_t(int64_t(int64_t(r1) + 3136));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8222BF18 game::LocationScreen::Transition19: lifted from generated/ by lift.py.
lifted::Result f8222BF18(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8222BF18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222BF44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222bf5c
  if (!cr6.eq) goto loc_8222BF5C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_8222BF5C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222BF64, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8222BF80 game::LocationScreen::Transition35: lifted from generated/ by lift.py.
lifted::Result f8222BF80(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8222BF80 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222BFAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222bfc4
  if (!cr6.eq) goto loc_8222BFC4;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_8222BFC4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222BFCC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8222CA48 game::LocationScreen::Page0: lifted from generated/ by lift.py.
lifted::Result f8222CA48(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222CA48 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // mr r30,r3
  r30 = r3;
  // lwz r3,20044(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20044);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x8222CA6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,20044(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20044);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8222CA78, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8222ccac
  if (cr0.eq) goto loc_8222CCAC;
  // lwz r11,20044(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20044);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8222cc24
  if (cr6.lt) goto loc_8222CC24;
  // beq cr6,0x8222cbf0
  if (cr6.eq) goto loc_8222CBF0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x8222cbb4
  if (cr6.lt) goto loc_8222CBB4;
  // beq cr6,0x8222cac8
  if (cr6.eq) goto loc_8222CAC8;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x8222cd48
  if (!cr6.lt) goto loc_8222CD48;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222CAC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,31
  r6 = uint64_t(int64_t(31));
  // b 0x8222cae4
  goto loc_8222CAE4;
loc_8222CAC8:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222CAE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,30
  r6 = uint64_t(int64_t(30));
loc_8222CAE4:
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222CAF8, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222CB0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r6,3
  r6 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222CB20, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // ori r10,r10,40960
  r10 = r10 | 40960;
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stwx r6,r31,r10
  guest::StoreU32(uint32_t(r31) + uint32_t(r10), uint32_t(r6));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r11) - 204).group_kind_address());
  // lwz r6,604(r11)
  r6 = guest::LoadU32(schema::LocationScreen(uint32_t(r11) - 204).opponent_count_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11) - 204).group_kind_address(), uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r7));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x8222cb94
  if (cr6.eq) goto loc_8222CB94;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_8222CB94:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8222CBAC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222a060
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::ShowPictures(LIFTED_ARGS(0x8222CBB0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8222cd48
  goto loc_8222CD48;
loc_8222CBB4:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // lwz r10,892(r11)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r11)).special_car_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8222cc64
  if (!cr6.eq) goto loc_8222CC64;
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r9,816(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).item2_taken_address(), uint32_t(r9));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x8222cbe8
  if (cr6.eq) goto loc_8222CBE8;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_8222CBE8:
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // b 0x8222cc8c
  goto loc_8222CC8C;
loc_8222CBF0:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,20048(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20048);
  // stw r10,800(r11)
  guest::StoreU32(schema::LocationScreen(uint32_t(r11)).picked_opponent_address(), uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x8222cc18
  if (cr6.eq) goto loc_8222CC18;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_8222CC18:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // b 0x8222cc8c
  goto loc_8222CC8C;
loc_8222CC24:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x8222CC2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,100
  r5 = uint64_t(int64_t(100));
  // lwz r10,20048(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 20048);
  // lwz r10,852(r10)
  r10 = guest::LoadU32(schema::LocationScreen(uint32_t(r10)).location_index_address());
  // mulli r10,r10,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(100))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addis r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 131072));
  // addi r4,r4,-21216
  r4 = uint64_t(int64_t(int64_t(r4) + -21216));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8222CC58, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,192(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 192);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8222cc6c
  if (!cr6.eq) goto loc_8222CC6C;
loc_8222CC64:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x8222cca4
  goto loc_8222CCA4;
loc_8222CC6C:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8222cc84
  if (cr6.eq) goto loc_8222CC84;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_8222CC84:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // li r8,1
  r8 = uint64_t(int64_t(1));
loc_8222CC8C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
loc_8222CCA4:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8222CCA8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8222cd48
  goto loc_8222CD48;
loc_8222CCAC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,20044(r31)
  r3 = schema::UiMovie(uint32_t(r31)).fault_location_menu_bits();
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8222CCB8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8222ccfc
  if (cr0.eq) goto loc_8222CCFC;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8222CCC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20044(r31)
  r11 = schema::UiMovie(uint32_t(r31)).fault_location_menu_bits();
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8222ccf4
  if (cr0.eq) goto loc_8222CCF4;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_8222CCF4:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // b 0x8222cd30
  goto loc_8222CD30;
loc_8222CCFC:
  // lwz r11,20044(r31)
  r11 = schema::UiMovie(uint32_t(r31)).fault_location_menu_bits();
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8222cd48
  if (cr6.eq) goto loc_8222CD48;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8222CD14, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,20040(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20040);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8222cd2c
  if (cr0.eq) goto loc_8222CD2C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_8222CD2C:
  // li r9,2
  r9 = uint64_t(int64_t(2));
loc_8222CD30:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
loc_8222CD48:
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8222CEB0 game::LocationScreen::Transition0: lifted from generated/ by lift.py.
lifted::Result f8222CEB0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222CEB0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222CED8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222cf2c
  if (!cr6.eq) goto loc_8222CF2C;
  // lwz r11,20048(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20048);
  // lwz r11,816(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 816);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8222cf04
  if (cr6.eq) goto loc_8222CF04;
  // li r3,80
  r3 = uint64_t(int64_t(80));
  // bl 0x8222bfe8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222BFE8, LIFTED_ARGS(0x8222CF04, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222CF04:
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
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x8222CF24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_8222CF2C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222CF38, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x8222cfe0
  if (cr0.eq) goto loc_8222CFE0;
  // lwz r11,20048(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20048);
  // lwz r11,816(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 816);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222cf94
  if (!cr6.eq) goto loc_8222CF94;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x8222CF5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,20048(r29)
  r10 = guest::LoadU32(uint32_t(r29) + 20048);
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,100
  r5 = uint64_t(int64_t(100));
  // lwz r10,852(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 852);
  // mulli r10,r10,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(100))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addis r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 131072));
  // addi r4,r4,-21216
  r4 = uint64_t(int64_t(int64_t(r4) + -21216));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8222CF84, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,192(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 192);
  // li r6,573
  r6 = uint64_t(int64_t(573));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // b 0x8222cf98
  goto loc_8222CF98;
loc_8222CF94:
  // li r6,575
  r6 = uint64_t(int64_t(575));
loc_8222CF98:
  // mr r3,r30
  r3 = r30;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222CFA8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,300
  r6 = uint64_t(int64_t(300));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // bl 0x8211c4f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C4F0(LIFTED_ARGS(0x8222CFC4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x8222CFD8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222ac18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222AC18, LIFTED_ARGS(0x8222CFDC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222a150
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222A150, LIFTED_ARGS(0x8222CFE0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222CFE0:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8222D3B8 game::LocationScreen::Transition31: lifted from generated/ by lift.py.
lifted::Result f8222D3B8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222D3B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222D3E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222d41c
  if (!cr6.eq) goto loc_8222D41C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,98
  r4 = uint64_t(int64_t(98));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222D414, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_8222D41C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222D428, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x8222d43c
  if (cr0.eq) goto loc_8222D43C;
  // bl 0x8222cd60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222CD60, LIFTED_ARGS(0x8222D438, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222ac18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222AC18, LIFTED_ARGS(0x8222D43C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222D43C:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8222D458 game::LocationScreen::Transition28: lifted from generated/ by lift.py.
lifted::Result f8222D458(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222D458 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222D488, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222d4c4
  if (!cr6.eq) goto loc_8222D4C4;
  // bl 0x8222cd60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222CD60, LIFTED_ARGS(0x8222D49C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222b200
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222B200, LIFTED_ARGS(0x8222D4A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222D4BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_8222D4C4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222D4D0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8222d4dc
  if (!cr0.eq) goto loc_8222D4DC;
  // bl 0x8222ac18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222AC18, LIFTED_ARGS(0x8222D4DC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222D4DC:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8222D4F8 game::LocationScreen::Transition3: lifted from generated/ by lift.py.
lifted::Result f8222D4F8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8222D4F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222D520, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,573
  r10 = uint64_t(int64_t(573));
  // lwz r11,0(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r10,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r10));
  // li r10,574
  r10 = uint64_t(int64_t(574));
  // stw r10,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r10));
  // li r10,575
  r10 = uint64_t(int64_t(575));
  // stw r10,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r10));
  // li r10,576
  r10 = uint64_t(int64_t(576));
  // stw r10,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r10));
  // li r10,577
  r10 = uint64_t(int64_t(577));
  // stw r10,112(r1)
  guest::StoreU32(uint32_t(r1) + 112, uint32_t(r10));
  // bne cr6,0x8222d5f4
  if (!cr6.eq) goto loc_8222D5F4;
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222D564, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222D570, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,160
  r3 = uint64_t(int64_t(160));
  // bl 0x8222bfe8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222BFE8, LIFTED_ARGS(0x8222D578, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,20044(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20044);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 4);
  // lwz r28,0(r11)
  r28 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r4,r10,13
  r4 = uint64_t(int64_t(int64_t(r10) + 13));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222D594, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r28,13
  r4 = uint64_t(int64_t(int64_t(r28) + 13));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222D5B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x8222D5B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20044(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20044);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r6,r11,r10
  r6 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222D5DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x8222D5EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(uint32_t(r29) + 0, uint32_t(r11));
loc_8222D5F4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222D600, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8222DCE8 game::LocationScreen::Transition6: lifted from generated/ by lift.py.
lifted::Result f8222DCE8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8222DCE8 (lifted)");
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
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222DD10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222dec8
  if (!cr6.eq) goto loc_8222DEC8;
  // li r4,62
  r4 = uint64_t(int64_t(62));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222DD2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,63
  r4 = uint64_t(int64_t(63));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222DD38, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // lwz r11,20052(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20052);
  // lwz r10,20048(r28)
  r10 = guest::LoadU32(uint32_t(r28) + 20048);
  // lwz r9,4(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 4);
  // lwz r10,812(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 812);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x8222dd78
  if (!cr0.eq) goto loc_8222DD78;
  // lwz r8,0(r11)
  r8 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r7,r10,-1
  r7 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpw cr6,r8,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r7), so);
  // bne cr6,0x8222dd78
  if (!cr6.eq) goto loc_8222DD78;
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // mulli r3,r11,40
  r3 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(40))));
loc_8222DD70:
  // bl 0x8222c058
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222C058, LIFTED_ARGS(0x8222DD74, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8222ddd0
  goto loc_8222DDD0;
loc_8222DD78:
  // lwz r8,0(r11)
  r8 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne cr6,0x8222dd98
  if (!cr6.eq) goto loc_8222DD98;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // bne cr6,0x8222dd98
  if (!cr6.eq) goto loc_8222DD98;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8222dd70
  goto loc_8222DD70;
loc_8222DD98:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x8222ddac
  if (!cr6.eq) goto loc_8222DDAC;
  // li r4,63
  r4 = uint64_t(int64_t(63));
  // b 0x8222ddb8
  goto loc_8222DDB8;
loc_8222DDAC:
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8222ddd4
  if (!cr6.eq) goto loc_8222DDD4;
  // li r4,62
  r4 = uint64_t(int64_t(62));
loc_8222DDB8:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222DDD0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222DDD0:
  // lwz r11,20052(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20052);
loc_8222DDD4:
  // lwz r10,4(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 4);
  // mr r3,r31
  r3 = r31;
  // lwz r30,0(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r4,r10,52
  r4 = uint64_t(int64_t(int64_t(r10) + 52));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222DDE8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r30,52
  r4 = uint64_t(int64_t(int64_t(r30) + 52));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222DE04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x8222DE0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x8222DE14, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,20048(r28)
  r10 = guest::LoadU32(uint32_t(r28) + 20048);
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,100
  r5 = uint64_t(int64_t(100));
  // lwz r10,852(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 852);
  // mulli r10,r10,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(100))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addis r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 131072));
  // addi r4,r4,-21216
  r4 = uint64_t(int64_t(int64_t(r4) + -21216));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8222DE3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20052(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20052);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // lwz r3,4(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 4);
  // bl 0x82117190
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry56(LIFTED_ARGS(0x8222DE5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r3,16(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 16);
  // bl 0x82229108
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::PictureA(LIFTED_ARGS(0x8222DE68, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211aaa8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetPictureTexture(LIFTED_ARGS(0x8222DE7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 16);
  // bl 0x82229158
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::PictureB(LIFTED_ARGS(0x8222DE84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211aaa8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetPictureTexture(LIFTED_ARGS(0x8222DE98, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8222DEA4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8222DEC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(uint32_t(r27) + 0, uint32_t(r11));
loc_8222DEC8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222DED4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8222E1A0 game::LocationScreen::Transition10: lifted from generated/ by lift.py.
lifted::Result f8222E1A0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8222E1A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,21
  r11 = uint64_t(int64_t(21));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8222E1C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8222e340
  if (!cr6.eq) goto loc_8222E340;
  // bl 0x8222cd60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222CD60, LIFTED_ARGS(0x8222E1DC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8222a150
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222A150, LIFTED_ARGS(0x8222E1E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8222E1F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8222E200, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Finish(LIFTED_ARGS(0x8222E210, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,574
  r6 = uint64_t(int64_t(574));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8222E224, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x8222E234, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,40
  r3 = uint64_t(int64_t(40));
  // bl 0x8222bfe8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222BFE8, LIFTED_ARGS(0x8222E23C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lfs f0,2612(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r9) + 2612);
  f0 = double(temp.f32);
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfs f13,2608(r7)
  temp.u32 = guest::LoadU32(uint32_t(r7) + 2608);
  f13 = double(temp.f32);
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 96, temp.u32);
  // lis r7,-32157
  r7 = uint64_t(int64_t(-2107441152));
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 100, temp.u32);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 104, temp.u32);
  // addi r7,r7,25880
  r7 = uint64_t(int64_t(int64_t(r7) + 25880));
  // stfs f13,108(r1)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r1) + 108, temp.u32);
  // ld r6,0(r11)
  r6 = guest::LoadU64(uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(uint32_t(r11) + 8);
  // stfs f0,112(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 112, temp.u32);
  // stfs f0,116(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 116, temp.u32);
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 120, temp.u32);
  // stfs f13,124(r1)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r1) + 124, temp.u32);
  // ld r4,0(r10)
  r4 = guest::LoadU64(uint32_t(r10) + 0);
  // stfs f13,140(r1)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r1) + 140, temp.u32);
  // ld r10,8(r10)
  r10 = guest::LoadU64(uint32_t(r10) + 8);
  // stfs f13,156(r1)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r1) + 156, temp.u32);
  // std r6,0(r9)
  guest::StoreU64(uint32_t(r9) + 0, r6);
  // std r11,8(r9)
  guest::StoreU64(uint32_t(r9) + 8, r11);
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // std r4,0(r8)
  guest::StoreU64(uint32_t(r8) + 0, r4);
  // std r10,8(r8)
  guest::StoreU64(uint32_t(r8) + 8, r10);
  // stfs f0,160(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 160, temp.u32);
  // stfs f0,164(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 164, temp.u32);
  // lwz r11,852(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 852);
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // lwzx r4,r11,r7
  r4 = guest::LoadU32(uint32_t(r11) + uint32_t(r7));
  // bl 0x82150960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82150960, LIFTED_ARGS(0x8222E2D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // ld r7,0(r10)
  r7 = guest::LoadU64(uint32_t(r10) + 0);
  // std r7,592(r11)
  guest::StoreU64(uint32_t(r11) + 592, r7);
  // ld r10,8(r10)
  r10 = guest::LoadU64(uint32_t(r10) + 8);
  // std r10,600(r11)
  guest::StoreU64(uint32_t(r11) + 600, r10);
  // ld r10,0(r8)
  r10 = guest::LoadU64(uint32_t(r8) + 0);
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // std r10,608(r11)
  guest::StoreU64(uint32_t(r11) + 608, r10);
  // ld r10,8(r8)
  r10 = guest::LoadU64(uint32_t(r8) + 8);
  // std r10,616(r11)
  guest::StoreU64(uint32_t(r11) + 616, r10);
  // lfs f0,160(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r1) + 160);
  f0 = double(temp.f32);
  // lwz r11,20048(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 20048);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // stfs f0,664(r11)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r11) + 664, temp.u32);
  // mr r3,r31
  r3 = r31;
  // lfs f0,164(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 164);
  f0 = double(temp.f32);
  // stfs f0,668(r11)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r11) + 668, temp.u32);
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Play(LIFTED_ARGS(0x8222E338, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(uint32_t(r29) + 0, uint32_t(r11));
loc_8222E340:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8222E34C, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x8222e35c
  if (cr0.eq) goto loc_8222E35C;
  // bl 0x8222ac18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8222AC18, LIFTED_ARGS(0x8222E35C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222E35C:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

lifted::Result e821F0998(const lifted::Args& a) {
  return f821F0998(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821F0D38(const lifted::Args& a) {
  return f821F0D38(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821F1130(const lifted::Args& a) {
  return f821F1130(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e821F1780(const lifted::Args& a) {
  return f821F1780(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821F1AB8(const lifted::Args& a) {
  return f821F1AB8(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821F32C8(const lifted::Args& a) {
  return {f821F32C8(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821F3A68(const lifted::Args& a) {
  return f821F3A68(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821F5288(const lifted::Args& a) {
  return f821F5288(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821F5B98(const lifted::Args& a) {
  return f821F5B98(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8220B9E8(const lifted::Args& a) {
  return f8220B9E8(a.r1, a.r13, a.r[5], a.f[0]);
}

lifted::Result e8220CE68(const lifted::Args& a) {
  return {f8220CE68(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8220D1D8(const lifted::Args& a) {
  return {f8220D1D8(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8220DF30(const lifted::Args& a) {
  return f8220DF30(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8220E0F8(const lifted::Args& a) {
  return f8220E0F8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822116A0(const lifted::Args& a) {
  return {f822116A0(a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82211768(const lifted::Args& a) {
  return f82211768(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822117B8(const lifted::Args& a) {
  return f822117B8(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82211878(const lifted::Args& a) {
  return f82211878(a.r1, a.r13, a.r[0], a.r[1], a.r[6]);
}

lifted::Result e82211CE0(const lifted::Args& a) {
  f82211CE0();
  return {a.r[0], a.f[0]};
}

lifted::Result e82211CF8(const lifted::Args& a) {
  return f82211CF8(a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82211D00(const lifted::Args& a) {
  return f82211D00(a.r1, a.r13, a.r[0], a.r[4], a.f[0], a.lr);
}

lifted::Result e82211E90(const lifted::Args& a) {
  return {f82211E90(a.r1, a.r13), a.f[0]};
}

lifted::Result e82211F18(const lifted::Args& a) {
  f82211F18();
  return {a.r[0], a.f[0]};
}

lifted::Result e822125B8(const lifted::Args& a) {
  return {f822125B8(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822126C8(const lifted::Args& a) {
  return {f822126C8(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82212868(const lifted::Args& a) {
  return f82212868(a.r1, a.r13, a.r[0], a.r[4], a.f[0], a.lr);
}

lifted::Result e82212888(const lifted::Args& a) {
  return f82212888(a.r1, a.r13, a.r[0], a.r[4], a.f[0], a.lr);
}

lifted::Result e82212958(const lifted::Args& a) {
  return f82212958(a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82212A38(const lifted::Args& a) {
  return f82212A38(a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.lr);
}

lifted::Result e82212AE8(const lifted::Args& a) {
  return f82212AE8(a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.lr);
}

lifted::Result e82213860(const lifted::Args& a) {
  return {f82213860(a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82213968(const lifted::Args& a) {
  return {f82213968(a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82213A70(const lifted::Args& a) {
  return f82213A70(a.r1, a.r13, a.r[1], a.f[0]);
}

lifted::Result e82213AE0(const lifted::Args& a) {
  return {f82213AE0(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82214BA8(const lifted::Args& a) {
  return {f82214BA8(a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82214C88(const lifted::Args& a) {
  return f82214C88(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82214E78(const lifted::Args& a) {
  return f82214E78(a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e822150A8(const lifted::Args& a) {
  return f822150A8(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822150F8(const lifted::Args& a) {
  return {f822150F8(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82215520(const lifted::Args& a) {
  return f82215520(a.r1, a.r13, a.r[1], a.f[0]);
}

lifted::Result e822155C0(const lifted::Args& a) {
  return f822155C0(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822159F0(const lifted::Args& a) {
  return {f822159F0(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82215A50(const lifted::Args& a) {
  return f82215A50(a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82215B48(const lifted::Args& a) {
  return f82215B48(a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e82215D08(const lifted::Args& a) {
  return f82215D08(a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e82215D58(const lifted::Args& a) {
  return f82215D58(a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e82216810(const lifted::Args& a) {
  return f82216810(a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e822194A0(const lifted::Args& a) {
  return f822194A0(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82219530(const lifted::Args& a) {
  return f82219530(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822195C0(const lifted::Args& a) {
  return f822195C0(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82219678(const lifted::Args& a) {
  return f82219678(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82219730(const lifted::Args& a) {
  return f82219730(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82219D00(const lifted::Args& a) {
  return f82219D00(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8221A110(const lifted::Args& a) {
  return f8221A110(a.r1, a.r13, a.r[0], a.r[5], a.r[7], a.f[0]);
}

lifted::Result e8221A8A8(const lifted::Args& a) {
  return f8221A8A8(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e8221AAD8(const lifted::Args& a) {
  return f8221AAD8(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e8221AB60(const lifted::Args& a) {
  return f8221AB60(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8221ABE0(const lifted::Args& a) {
  return {f8221ABE0(), a.f[0]};
}

lifted::Result e8221ACC0(const lifted::Args& a) {
  return f8221ACC0(a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8221AE58(const lifted::Args& a) {
  return f8221AE58(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8221B3C8(const lifted::Args& a) {
  return f8221B3C8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8221B6A0(const lifted::Args& a) {
  return f8221B6A0(a.r1, a.r13);
}

lifted::Result e8221B918(const lifted::Args& a) {
  return f8221B918(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8221B9C8(const lifted::Args& a) {
  return f8221B9C8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82222038(const lifted::Args& a) {
  return f82222038(a.r1, a.r13);
}

lifted::Result e822271B8(const lifted::Args& a) {
  f822271B8();
  return {a.r[0], a.f[0]};
}

lifted::Result e822271D8(const lifted::Args& a) {
  f822271D8();
  return {a.r[0], a.f[0]};
}

lifted::Result e82227310(const lifted::Args& a) {
  return {f82227310(a.r1, a.r13), a.f[0]};
}

lifted::Result e82227780(const lifted::Args& a) {
  return f82227780(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82227A50(const lifted::Args& a) {
  return f82227A50(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82228068(const lifted::Args& a) {
  return {f82228068(a.r1, a.r13, a.r[1]), a.f[0]};
}

lifted::Result e82228220(const lifted::Args& a) {
  return f82228220(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822282E8(const lifted::Args& a) {
  return f822282E8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e82228548(const lifted::Args& a) {
  return f82228548(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82228A90(const lifted::Args& a) {
  return f82228A90(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82228B80(const lifted::Args& a) {
  return f82228B80(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82228CC0(const lifted::Args& a) {
  return f82228CC0(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82228E18(const lifted::Args& a) {
  return f82228E18(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82229578(const lifted::Args& a) {
  return f82229578(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82229610(const lifted::Args& a) {
  f82229610(a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82229678(const lifted::Args& a) {
  return f82229678(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82229798(const lifted::Args& a) {
  return f82229798(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82229888(const lifted::Args& a) {
  return f82229888(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82229A48(const lifted::Args& a) {
  return f82229A48(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82229B38(const lifted::Args& a) {
  f82229B38(a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82229B98(const lifted::Args& a) {
  return f82229B98(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82229CF0(const lifted::Args& a) {
  return f82229CF0(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82229E98(const lifted::Args& a) {
  return f82229E98(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8222A218(const lifted::Args& a) {
  return f8222A218(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8222AAB0(const lifted::Args& a) {
  return f8222AAB0(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8222AC78(const lifted::Args& a) {
  return f8222AC78(a.r1, a.r13, a.r[0], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8222BF18(const lifted::Args& a) {
  return f8222BF18(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e8222BF80(const lifted::Args& a) {
  return f8222BF80(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e8222CA48(const lifted::Args& a) {
  return f8222CA48(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8222CEB0(const lifted::Args& a) {
  return f8222CEB0(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8222D3B8(const lifted::Args& a) {
  return f8222D3B8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8222D458(const lifted::Args& a) {
  return f8222D458(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8222D4F8(const lifted::Args& a) {
  return f8222D4F8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8222DCE8(const lifted::Args& a) {
  return f8222DCE8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8222E1A0(const lifted::Args& a) {
  return f8222E1A0(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x821F0998, e821F0998},
    {0x821F0D38, e821F0D38},
    {0x821F1130, e821F1130},
    {0x821F1780, e821F1780},
    {0x821F1AB8, e821F1AB8},
    {0x821F32C8, e821F32C8},
    {0x821F3A68, e821F3A68},
    {0x821F5288, e821F5288},
    {0x821F5B98, e821F5B98},
    {0x8220B9E8, e8220B9E8},
    {0x8220CE68, e8220CE68},
    {0x8220D1D8, e8220D1D8},
    {0x8220DF30, e8220DF30},
    {0x8220E0F8, e8220E0F8},
    {0x822116A0, e822116A0},
    {0x82211768, e82211768},
    {0x822117B8, e822117B8},
    {0x82211878, e82211878},
    {0x82211CE0, e82211CE0},
    {0x82211CF8, e82211CF8},
    {0x82211D00, e82211D00},
    {0x82211E90, e82211E90},
    {0x82211F18, e82211F18},
    {0x822125B8, e822125B8},
    {0x822126C8, e822126C8},
    {0x82212868, e82212868},
    {0x82212888, e82212888},
    {0x82212958, e82212958},
    {0x82212A38, e82212A38},
    {0x82212AE8, e82212AE8},
    {0x82213860, e82213860},
    {0x82213968, e82213968},
    {0x82213A70, e82213A70},
    {0x82213AE0, e82213AE0},
    {0x82214BA8, e82214BA8},
    {0x82214C88, e82214C88},
    {0x82214E78, e82214E78},
    {0x822150A8, e822150A8},
    {0x822150F8, e822150F8},
    {0x82215520, e82215520},
    {0x822155C0, e822155C0},
    {0x822159F0, e822159F0},
    {0x82215A50, e82215A50},
    {0x82215B48, e82215B48},
    {0x82215D08, e82215D08},
    {0x82215D58, e82215D58},
    {0x82216810, e82216810},
    {0x822194A0, e822194A0},
    {0x82219530, e82219530},
    {0x822195C0, e822195C0},
    {0x82219678, e82219678},
    {0x82219730, e82219730},
    {0x82219D00, e82219D00},
    {0x8221A110, e8221A110},
    {0x8221A8A8, e8221A8A8},
    {0x8221AAD8, e8221AAD8},
    {0x8221AB60, e8221AB60},
    {0x8221ABE0, e8221ABE0},
    {0x8221ACC0, e8221ACC0},
    {0x8221AE58, e8221AE58},
    {0x8221B3C8, e8221B3C8},
    {0x8221B6A0, e8221B6A0},
    {0x8221B918, e8221B918},
    {0x8221B9C8, e8221B9C8},
    {0x82222038, e82222038},
    {0x822271B8, e822271B8},
    {0x822271D8, e822271D8},
    {0x82227310, e82227310},
    {0x82227780, e82227780},
    {0x82227A50, e82227A50},
    {0x82228068, e82228068},
    {0x82228220, e82228220},
    {0x822282E8, e822282E8},
    {0x82228548, e82228548},
    {0x82228A90, e82228A90},
    {0x82228B80, e82228B80},
    {0x82228CC0, e82228CC0},
    {0x82228E18, e82228E18},
    {0x82229578, e82229578},
    {0x82229610, e82229610},
    {0x82229678, e82229678},
    {0x82229798, e82229798},
    {0x82229888, e82229888},
    {0x82229A48, e82229A48},
    {0x82229B38, e82229B38},
    {0x82229B98, e82229B98},
    {0x82229CF0, e82229CF0},
    {0x82229E98, e82229E98},
    {0x8222A218, e8222A218},
    {0x8222AAB0, e8222AAB0},
    {0x8222AC78, e8222AC78},
    {0x8222BF18, e8222BF18},
    {0x8222BF80, e8222BF80},
    {0x8222CA48, e8222CA48},
    {0x8222CEB0, e8222CEB0},
    {0x8222D3B8, e8222D3B8},
    {0x8222D458, e8222D458},
    {0x8222D4F8, e8222D4F8},
    {0x8222DCE8, e8222DCE8},
    {0x8222E1A0, e8222E1A0},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_012)), true);

}  // namespace

}  // namespace lifted
