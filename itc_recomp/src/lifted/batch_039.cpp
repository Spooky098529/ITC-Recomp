// Lift sweep batch 039 (docs/loops/lift-sweep.md): written by
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
#include "schema/race_event.h"
#include "schema/stand_models.h"
#include "schema/system_services.h"
#include "schema/tuning_screen.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_039, true, "ITC",
              "Run lift sweep batch 039's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_8226E8B8: lifted from generated/ by lift.py.
lifted::Result f8226E8B8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226E8B8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
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
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8226E8E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226ea68
  if (!cr6.eq) goto loc_8226EA68;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8226E904, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226E910, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,320
  r5 = uint64_t(int64_t(320));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226E920, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226E92C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,321
  r5 = uint64_t(int64_t(321));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226E93C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226E948, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,322
  r5 = uint64_t(int64_t(322));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226E958, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,15
  r5 = uint64_t(int64_t(15));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8226E96C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8226E980, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8226E990, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r6,9688(r31)
  r6 = guest::LoadU32(uint32_t(r31) + 9688);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // mr r5,r6
  r5 = r6;
  // bl 0x8226c3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C3B0(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r6,9692(r31)
  r6 = guest::LoadU32(uint32_t(r31) + 9692);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // mr r5,r6
  r5 = r6;
  // bl 0x8226c3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C3B0(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24932(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24932);
  // lwz r4,0(r11)
  r4 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // bl 0x8226c798
  // (left out: dead in C++)
  { const lifted::Result o = f8226C798(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 9656);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addic. r11,r11,1
  ca = uint32_t(r11) > 4294967294;
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8226ea04
  if (cr0.eq) goto loc_8226EA04;
loc_8226E9E0:
  // lwz r11,9672(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 9672);
  // mr r3,r30
  r3 = r30;
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8226E9F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 9656);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8226e9e0
  if (cr6.lt) goto loc_8226E9E0;
loc_8226EA04:
  // mr r3,r30
  r3 = r30;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8226EA14, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24932);
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9668);
  // lwz r29,0(r11)
  r29 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EA24, r31)); r3 = o.r3; f1 = o.f1; }
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
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // li r4,263
  r4 = uint64_t(int64_t(263));
  // mr r3,r30
  r3 = r30;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).y_address(), temp.u32);
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8226EA60, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(uint32_t(r27) + 0, uint32_t(r11));
loc_8226EA68:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8226EA74, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8226EA88: lifted from generated/ by lift.py.
lifted::Result f8226EA88(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226EA88 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
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
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8226EAB4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226eb20
  if (!cr6.eq) goto loc_8226EB20;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24932);
  // lwz r4,0(r11)
  r4 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // bl 0x8226c500
  // (left out: dead in C++)
  { const lifted::Result o = f8226C500(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9708(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 9708);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226eaf0
  if (!cr6.eq) goto loc_8226EAF0;
  // bl 0x82270bf8
  // (left out: dead in C++)
  r3 = f82270BF8(r1, r13);
  // b 0x8226eaf4
  goto loc_8226EAF4;
loc_8226EAF0:
  // bl 0x82270a80
  // (left out: dead in C++)
  r3 = f82270A80(r1, r13);
loc_8226EAF4:
  // mr r3,r31
  r3 = r31;
  // bl 0x8226bbb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BBB8(r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,265
  r4 = uint64_t(int64_t(265));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8226EB18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(uint32_t(r30) + 0, uint32_t(r11));
loc_8226EB20:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8226EB2C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8226EEC8: lifted from generated/ by lift.py.
lifted::Result f8226EEC8(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226EEC8 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
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
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8226EEF4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,19
  r5 = uint64_t(int64_t(19));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8226EF08, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::memset(LIFTED_ARGS(0x8226EF1C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EF28, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,317
  r5 = uint64_t(int64_t(317));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226EF38, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EF44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,318
  r5 = uint64_t(int64_t(318));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226EF54, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EF60, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,319
  r5 = uint64_t(int64_t(319));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226EF70, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r22,-32127
  r22 = uint64_t(int64_t(-2105475072));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24848(r22)
  r11 = guest::LoadU32(uint32_t(r22) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226efcc
  if (!cr6.eq) goto loc_8226EFCC;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EF8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(LIFTED_ARGS(0x8226EF9C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EFA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(LIFTED_ARGS(0x8226EFB8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EFC4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // b 0x8226f00c
  goto loc_8226F00C;
loc_8226EFCC:
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EFD0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(LIFTED_ARGS(0x8226EFE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226EFEC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(LIFTED_ARGS(0x8226EFFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F008, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
loc_8226F00C:
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A630(LIFTED_ARGS(0x8226F018, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // bne cr6,0x8226f344
  if (!cr6.eq) goto loc_8226F344;
  // li r21,0
  r21 = uint64_t(int64_t(0));
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // bne cr6,0x8226f0a8
  if (!cr6.eq) goto loc_8226F0A8;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r11,30320
  r4 = uint64_t(int64_t(int64_t(r11) + 30320));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8226F044, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r23,844(r11)
  r23 = guest::LoadU32(uint32_t(r11) + 844);
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(LIFTED_ARGS(0x8226F058, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r11,r1,272
  r11 = uint64_t(int64_t(int64_t(r1) + 272));
  // rlwinm r10,r29,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
  // lwzx r27,r10,r11
  r27 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // lwz r3,-21596(r28)
  r3 = guest::LoadU32(uint32_t(r28) + -21596);
  // bl 0x82260908
  // (left out: dead in C++)
  { const lifted::Result o = game::Session::IsActive(r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8226f11c
  if (!cr6.eq) goto loc_8226F11C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r4,-21596(r28)
  r4 = guest::LoadU32(uint32_t(r28) + -21596);
  // lwz r3,29856(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 29856);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,52(r11)
  r11 = guest::LoadU32(schema::VoiceEngine(uint32_t(r11)).slot_is_headset_present_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8226F098, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8226f11c
  if (!cr6.eq) goto loc_8226F11C;
  // mr r27,r24
  r27 = r24;
  // b 0x8226f11c
  goto loc_8226F11C;
loc_8226F0A8:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,29448
  r10 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226f0c8
  if (!cr6.gt) goto loc_8226F0C8;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226F0C8:
  // blt cr6,0x8226f0d0
  if (cr6.lt) goto loc_8226F0D0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226F0D0:
  // mr r4,r11
  r4 = r11;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8226F0E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r21
  r29 = r21;
  // lbz r10,27404(r11)
  r10 = guest::LoadU8(uint32_t(r11) + 27404);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // subf r10,r24,r10
  r10 = r10 - r24;
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -21596);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,27416
  r11 = uint64_t(int64_t(int64_t(r11) + 27416));
  // lwz r23,12(r11)
  r23 = guest::LoadU32(uint32_t(r11) + 12);
  // cntlzw r11,r10
  r11 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r27,r11,27,31,31
  r27 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x82260908
  // (left out: dead in C++)
  { const lifted::Result o = game::Session::IsActive(r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8226f11c
  if (!cr6.eq) goto loc_8226F11C;
  // mr r27,r21
  r27 = r21;
loc_8226F11C:
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8226F12C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,412
  r11 = uint64_t(int64_t(int64_t(r1) + 412));
  // rlwinm r28,r29,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // mr r26,r3
  r26 = r3;
  // lwzx r3,r28,r11
  r3 = guest::LoadU32(uint32_t(r28) + uint32_t(r11));
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8225FFC8, LIFTED_ARGS(0x8226F140, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F150, r31)); r3 = o.r3; f1 = o.f1; }
  // subf r10,r24,r27
  r10 = r27 - r24;
  // mr r11,r3
  r11 = r3;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // stw r25,108(r11)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).texture_entry_address(), uint32_t(r25));
  // xori r6,r10,1
  r6 = r10 ^ 1;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x8226F178, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::WideCharToMultiByte(LIFTED_ARGS(0x8226F1A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F1B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,480
  r5 = uint64_t(int64_t(int64_t(r1) + 480));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c8a0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeCharText(LIFTED_ARGS(0x8226F1C0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,428
  r11 = uint64_t(int64_t(int64_t(r1) + 428));
  // addi r9,r1,444
  r9 = uint64_t(int64_t(int64_t(r1) + 444));
  // lis r10,152
  r10 = uint64_t(int64_t(9961472));
  // ori r10,r10,38527
  r10 = r10 | 38527;
  // lwzx r11,r28,r11
  r11 = guest::LoadU32(uint32_t(r28) + uint32_t(r11));
  // lwzx r29,r28,r9
  r29 = guest::LoadU32(uint32_t(r28) + uint32_t(r9));
  // mr r26,r10
  r26 = r10;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bgt cr6,0x8226f1e8
  if (cr6.gt) goto loc_8226F1E8;
  // mr r26,r11
  r26 = r11;
loc_8226F1E8:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8226f1fc
  if (cr6.eq) goto loc_8226F1FC;
  // divwu r27,r11,r29
  r27 = uint32_t(uint32_t(r29) ? uint32_t(r11) / uint32_t(r29) : 0);
  // twllei r29,0
  if (int32_t(r29) == 0 || uint32_t(r29) < 0u) lifted::Trap(0);
  // b 0x8226f200
  goto loc_8226F200;
loc_8226F1FC:
  // mr r27,r21
  r27 = r21;
loc_8226F200:
  // addi r11,r1,460
  r11 = uint64_t(int64_t(int64_t(r1) + 460));
  // lwzx r28,r28,r11
  r28 = guest::LoadU32(uint32_t(r28) + uint32_t(r11));
  // cmplwi cr6,r28,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 9999, so);
  // ble cr6,0x8226f214
  if (!cr6.gt) goto loc_8226F214;
  // li r28,9999
  r28 = uint64_t(int64_t(9999));
loc_8226F214:
  // cmplwi cr6,r29,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 9999, so);
  // ble cr6,0x8226f220
  if (!cr6.gt) goto loc_8226F220;
  // li r29,9999
  r29 = uint64_t(int64_t(9999));
loc_8226F220:
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F22C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226F240, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F24C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226F260, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F26C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x8226F284, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24848(r22)
  r11 = guest::LoadU32(uint32_t(r22) + 24848);
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r3,r30
  r3 = r30;
  // bne cr6,0x8226f2cc
  if (!cr6.eq) goto loc_8226F2CC;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F29C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // stw r24,80(r11)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).visible_address(), uint32_t(r24));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F2B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // stw r24,80(r11)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).visible_address(), uint32_t(r24));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F2C4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r24,80(r3)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).visible_address(), uint32_t(r24));
  // b 0x8226f2fc
  goto loc_8226F2FC;
loc_8226F2CC:
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F2D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // stw r21,80(r11)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).visible_address(), uint32_t(r21));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F2E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // stw r21,80(r11)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).visible_address(), uint32_t(r21));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F2F8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r21,80(r3)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).visible_address(), uint32_t(r21));
loc_8226F2FC:
  // cmplwi cr6,r23,71
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 71, so);
  // li r5,43
  r5 = uint64_t(int64_t(43));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bgt cr6,0x8226f440
  if (cr6.gt) goto loc_8226F440;
  // addi r6,r23,1646
  r6 = uint64_t(int64_t(int64_t(r23) + 1646));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8226F318, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r23,1790
  r6 = uint64_t(int64_t(int64_t(r23) + 1790));
  // li r5,44
  r5 = uint64_t(int64_t(44));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8226F32C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r23,1718
  r6 = uint64_t(int64_t(int64_t(r23) + 1718));
  // li r5,45
  r5 = uint64_t(int64_t(45));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8226F340, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226f464
  goto loc_8226F464;
loc_8226F344:
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8226F354, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8225FFC8, LIFTED_ARGS(0x8226F360, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F370, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // stw r28,108(r11)
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).texture_entry_address(), uint32_t(r28));
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x8226F38C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F398, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8226F3A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F3B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8226F3BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F3C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8226F3D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F3E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8226F3EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F3F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8226F404, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F410, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8226F41C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x8226F428, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8226F434, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,43
  r5 = uint64_t(int64_t(43));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
loc_8226F440:
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8226F444, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,44
  r5 = uint64_t(int64_t(44));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8226F454, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,45
  r5 = uint64_t(int64_t(45));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearText(LIFTED_ARGS(0x8226F464, r31)); r3 = o.r3; f1 = o.f1; }
loc_8226F464:
  // addi r1,r1,832
  r1 = uint64_t(int64_t(int64_t(r1) + 832));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226F470: lifted from generated/ by lift.py.
lifted::Result f8226F470(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226F470 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r30,r11,30752
  r30 = uint64_t(int64_t(int64_t(r11) + 30752));
  // mr r29,r4
  r29 = r4;
  // lwz r11,88(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8226f4f0
  if (cr6.eq) goto loc_8226F4F0;
  // lwz r11,9632(r31)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r31)).again_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226f4f0
  if (!cr6.eq) goto loc_8226F4F0;
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,14
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 14, so);
  // beq cr6,0x8226f4c0
  if (cr6.eq) goto loc_8226F4C0;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F4C0:
  // li r10,14
  r10 = uint64_t(int64_t(14));
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r10));
  // stw r28,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r28));
  // stw r28,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r28));
  // bl 0x82270a80
  // (left out: dead in C++)
  r3 = f82270A80(r1, r13);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // stw r11,88(r30)
  guest::StoreU32(uint32_t(r30) + 88, uint32_t(r11));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8226f6fc
  goto loc_8226F6FC;
loc_8226F4F0:
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r3,24932(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 24932);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x8226F4FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8226eec8
  // (left out: dead in C++)
  { const lifted::Result o = f8226EEC8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8226bcb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BCB8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8226bbb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BBB8(r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24932(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8226F530, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // beq 0x8226f624
  if (cr0.eq) goto loc_8226F624;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8226F548, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24932);
  // lwz r11,0(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8226f5f8
  if (cr6.lt) goto loc_8226F5F8;
  // beq cr6,0x8226f5d0
  if (cr6.eq) goto loc_8226F5D0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x8226f624
  if (!cr6.lt) goto loc_8226F624;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // addi r11,r29,204
  r11 = uint64_t(int64_t(int64_t(r29) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,24996(r9)
  guest::StoreU32(uint32_t(r9) + 24996, uint32_t(r10));
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_count_bits();
  // mr r3,r31
  r3 = r31;
  // lwz r6,604(r11)
  r6 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  // bl 0x82270b28
  // (left out: dead in C++)
  r3 = f82270B28(r1, r13);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9644(r31)
  guest::StoreU32(schema::NetMachine(uint32_t(r31)).unknown_9644_address(), uint32_t(r11));
  // b 0x8226f624
  goto loc_8226F624;
loc_8226F5D0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9720(r31)
  guest::StoreU32(schema::NetMachine(uint32_t(r31)).unknown_9720_address(), uint32_t(r11));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x8226f5ec
  if (cr6.eq) goto loc_8226F5EC;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F5EC:
  // li r10,28
  r10 = uint64_t(int64_t(28));
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // b 0x8226f614
  goto loc_8226F614;
loc_8226F5F8:
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x8226f60c
  if (cr6.eq) goto loc_8226F60C;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F60C:
  // li r10,19
  r10 = uint64_t(int64_t(19));
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_8226F614:
  // stw r28,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r28));
  // stw r9,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r9));
  // stw r28,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r28));
  // stw r10,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r10));
loc_8226F624:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24932(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8226F630, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,5
  r31 = uint64_t(int64_t(5));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226f690
  if (cr0.eq) goto loc_8226F690;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8226F644, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24932);
  // lwz r10,0(r11)
  r10 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x8226f6c0
  if (cr6.eq) goto loc_8226F6C0;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // stw r10,4(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_bits(uint32_t(r10));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,0(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_cursor_bits(uint32_t(r10));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x8226f678
  if (cr6.eq) goto loc_8226F678;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F678:
  // li r10,17
  r10 = uint64_t(int64_t(17));
  // stw r28,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r28));
  // stw r31,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r31));
  // stw r28,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r28));
  // stw r10,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r10));
  // b 0x8226f6bc
  goto loc_8226F6BC;
loc_8226F690:
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // lwz r3,24932(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8226F69C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226f6bc
  if (cr0.eq) goto loc_8226F6BC;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // ld r4,144(r11)
  r4 = guest::LoadU64(uint32_t(r11) + 144);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -21596);
  // bl 0x822952d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XShowGamerCardUI(r1, r13, r3, r4, r6, r7, r8, r9, r10, 0x8226F6BC); r3 = o.r3; f1 = o.f1; }
loc_8226F6BC:
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24932);
loc_8226F6C0:
  // lwz r11,8(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).moved_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8226f6fc
  if (cr6.eq) goto loc_8226F6FC;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8226F6D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x8226f6e8
  if (cr6.eq) goto loc_8226F6E8;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F6E8:
  // li r10,18
  r10 = uint64_t(int64_t(18));
  // stw r28,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r28));
  // stw r31,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r31));
  // stw r28,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r28));
  // stw r10,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r10));
loc_8226F6FC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226F708: lifted from generated/ by lift.py.
lifted::Result f8226F708(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226F708 (lifted)");
  uint64_t r11 = 0, r12 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e58
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(uint32_t(r1) + -4096);
  // stwu r1,-4320(r1)
  ea = -4320 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r21,-32127
  r21 = uint64_t(int64_t(-2105475072));
  // mr r29,r3
  r29 = r3;
  // mr r24,r4
  r24 = r4;
  // lwz r3,24932(r21)
  r3 = guest::LoadU32(uint32_t(r21) + 24932);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x8226F72C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r29
  r3 = r29;
  // bl 0x8226bcb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BCB8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r23,r11,29448
  r23 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 4);
  // lwz r10,8(r23)
  r10 = guest::LoadU32(uint32_t(r23) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226f75c
  if (!cr6.gt) goto loc_8226F75C;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226F75C:
  // subf r11,r11,r10
  r11 = r10 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8226f784
  if (cr0.eq) goto loc_8226F784;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8226eec8
  // (left out: dead in C++)
  { const lifted::Result o = f8226EEC8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8226F784:
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,220
  r4 = uint64_t(int64_t(220));
  // li r3,300
  r3 = uint64_t(int64_t(300));
  // lwz r11,27368(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 27368);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226f7ac
  if (!cr6.eq) goto loc_8226F7AC;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,6360
  r6 = uint64_t(int64_t(int64_t(r11) + 6360));
  // b 0x8226f7b4
  goto loc_8226F7B4;
loc_8226F7AC:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,6348
  r6 = uint64_t(int64_t(int64_t(r11) + 6348));
loc_8226F7B4:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(LIFTED_ARGS(0x8226F7B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,27368(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 27368);
  // li r18,18
  r18 = uint64_t(int64_t(18));
  // li r25,19
  r25 = uint64_t(int64_t(19));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // li r17,21
  r17 = uint64_t(int64_t(21));
  // li r26,-1
  r26 = uint64_t(int64_t(-1));
  // li r22,4
  r22 = uint64_t(int64_t(4));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r19,22
  r19 = uint64_t(int64_t(22));
  // li r20,24
  r20 = uint64_t(int64_t(24));
  // li r16,23
  r16 = uint64_t(int64_t(23));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // bne cr6,0x8226f8f4
  if (!cr6.eq) goto loc_8226F8F4;
  // lwz r11,9716(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9716_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226f934
  if (!cr6.eq) goto loc_8226F934;
  // lwz r11,9708(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226f934
  if (!cr6.eq) goto loc_8226F934;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,27416
  r11 = uint64_t(int64_t(int64_t(r11) + 27416));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 12);
  // cmpwi cr6,r11,72
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 72, so);
  // bne cr6,0x8226f848
  if (!cr6.eq) goto loc_8226F848;
  // lwz r11,4(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 4);
  // lwz r10,8(r23)
  r10 = guest::LoadU32(uint32_t(r23) + 8);
  // stb r18,338(r1)
  guest::StoreU8(uint32_t(r1) + 338, uint8_t(r18));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226f838
  if (!cr6.gt) goto loc_8226F838;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226F838:
  // blt cr6,0x8226f840
  if (cr6.lt) goto loc_8226F840;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226F840:
  // addi r3,r1,336
  r3 = uint64_t(int64_t(int64_t(r1) + 336));
  // b 0x8226f8c8
  goto loc_8226F8C8;
loc_8226F848:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,28184
  r11 = uint64_t(int64_t(int64_t(r11) + 28184));
  // lwz r11,344(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 344);
  // cmpwi cr6,r11,-999
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -999, so);
  // bne cr6,0x8226f888
  if (!cr6.eq) goto loc_8226F888;
  // lwz r11,4(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 4);
  // lwz r10,8(r23)
  r10 = guest::LoadU32(uint32_t(r23) + 8);
  // stb r25,1618(r1)
  guest::StoreU8(uint32_t(r1) + 1618, uint8_t(r25));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226f878
  if (!cr6.gt) goto loc_8226F878;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226F878:
  // blt cr6,0x8226f880
  if (cr6.lt) goto loc_8226F880;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226F880:
  // addi r3,r1,1616
  r3 = uint64_t(int64_t(int64_t(r1) + 1616));
  // b 0x8226f8c8
  goto loc_8226F8C8;
loc_8226F888:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,28664
  r11 = uint64_t(int64_t(int64_t(r11) + 28664));
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // cmpwi cr6,r11,-999
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -999, so);
  // bne cr6,0x8226f8d4
  if (!cr6.eq) goto loc_8226F8D4;
  // li r11,20
  r11 = uint64_t(int64_t(20));
  // lwz r10,8(r23)
  r10 = guest::LoadU32(uint32_t(r23) + 8);
  // stb r11,2898(r1)
  guest::StoreU8(uint32_t(r1) + 2898, uint8_t(r11));
  // lwz r11,4(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226f8bc
  if (!cr6.gt) goto loc_8226F8BC;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226F8BC:
  // blt cr6,0x8226f8c4
  if (cr6.lt) goto loc_8226F8C4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226F8C4:
  // addi r3,r1,2896
  r3 = uint64_t(int64_t(int64_t(r1) + 2896));
loc_8226F8C8:
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(LIFTED_ARGS(0x8226F8D0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226fa50
  goto loc_8226FA50;
loc_8226F8D4:
  // stw r28,9716(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9716_address(), uint32_t(r28));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226f8ec
  if (cr6.eq) goto loc_8226F8EC;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F8EC:
  // stw r17,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r17));
  // b 0x8226fa44
  goto loc_8226FA44;
loc_8226F8F4:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226f934
  if (!cr6.eq) goto loc_8226F934;
  // lwz r11,9716(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9716_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226f934
  if (!cr6.eq) goto loc_8226F934;
  // lwz r11,9708(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226f934
  if (!cr6.eq) goto loc_8226F934;
  // stw r31,9716(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9716_address(), uint32_t(r31));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226f92c
  if (cr6.eq) goto loc_8226F92C;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F92C:
  // stw r19,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r19));
  // b 0x8226fa44
  goto loc_8226FA44;
loc_8226F934:
  // lwz r11,4(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 4);
  // lwz r10,8(r23)
  r10 = guest::LoadU32(uint32_t(r23) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226f94c
  if (!cr6.gt) goto loc_8226F94C;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226F94C:
  // subf r11,r11,r10
  r11 = r10 - r11;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // li r3,300
  r3 = uint64_t(int64_t(300));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8226f9f0
  if (cr0.eq) goto loc_8226F9F0;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,6332
  r6 = uint64_t(int64_t(int64_t(r11) + 6332));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(LIFTED_ARGS(0x8226F974, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9716(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9716_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226f9b0
  if (!cr6.eq) goto loc_8226F9B0;
  // mr r11,r31
  r11 = r31;
  // stw r31,9716(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9716_address(), uint32_t(r31));
  // stw r31,9708(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address(), uint32_t(r31));
  // stw r11,27368(r30)
  guest::StoreU32(uint32_t(r30) + 27368, uint32_t(r11));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226f9a4
  if (cr6.eq) goto loc_8226F9A4;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F9A4:
  // stw r19,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r19));
  // stw r20,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r20));
  // b 0x8226f9dc
  goto loc_8226F9DC;
loc_8226F9B0:
  // lwz r11,9708(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226fa50
  if (!cr6.eq) goto loc_8226FA50;
  // stw r31,9708(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address(), uint32_t(r31));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226f9d4
  if (cr6.eq) goto loc_8226F9D4;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226F9D4:
  // stw r20,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r20));
  // stw r26,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r26));
loc_8226F9DC:
  // mr r3,r29
  r3 = r29;
  // stw r22,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r22));
  // stw r26,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r26));
  // bl 0x82270a80
  // (left out: dead in C++)
  r3 = f82270A80(r1, r13);
  // b 0x8226fa50
  goto loc_8226FA50;
loc_8226F9F0:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r6,r11,6320
  r6 = uint64_t(int64_t(int64_t(r11) + 6320));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(LIFTED_ARGS(0x8226F9FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9708(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226fa50
  if (!cr6.eq) goto loc_8226FA50;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // stw r28,9708(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address(), uint32_t(r28));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(r3);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(r3);
  // li r3,6
  r3 = uint64_t(int64_t(6));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(r3);
  // mr r3,r29
  r3 = r29;
  // bl 0x82270bf8
  // (left out: dead in C++)
  r3 = f82270BF8(r1, r13);
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226fa40
  if (cr6.eq) goto loc_8226FA40;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FA40:
  // stw r16,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r16));
loc_8226FA44:
  // stw r26,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r26));
  // stw r22,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r22));
  // stw r26,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r26));
loc_8226FA50:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24932(r21)
  r3 = guest::LoadU32(uint32_t(r21) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8226FA5C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // beq 0x8226fce8
  if (cr0.eq) goto loc_8226FCE8;
  // lwz r11,24932(r21)
  r11 = guest::LoadU32(uint32_t(r21) + 24932);
  // lwz r11,0(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8226fc00
  if (cr6.lt) goto loc_8226FC00;
  // beq cr6,0x8226fb60
  if (cr6.eq) goto loc_8226FB60;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x8226fb2c
  if (cr6.lt) goto loc_8226FB2C;
  // beq cr6,0x8226fafc
  if (cr6.eq) goto loc_8226FAFC;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x8226fd54
  if (!cr6.lt) goto loc_8226FD54;
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // mr r10,r28
  r10 = r28;
  // addi r11,r24,204
  r11 = uint64_t(int64_t(int64_t(r24) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // mr r8,r31
  r8 = r31;
  // mr r7,r31
  r7 = r31;
  // stw r10,24996(r9)
  guest::StoreU32(uint32_t(r9) + 24996, uint32_t(r10));
  // mr r9,r25
  r9 = r25;
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_count_bits();
  // mr r3,r29
  r3 = r29;
  // lwz r6,604(r11)
  r6 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  // bl 0x82270b28
  // (left out: dead in C++)
  r3 = f82270B28(r1, r13);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r31,9644(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9644_address(), uint32_t(r31));
  // b 0x8226fce0
  goto loc_8226FCE0;
loc_8226FAFC:
  // lwz r11,9708(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8226fcdc
  if (!cr6.eq) goto loc_8226FCDC;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 28180);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8226fcdc
  if (!cr6.eq) goto loc_8226FCDC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8226FB20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-21596(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21596);
  // bl 0x82148f80
  // (left out: dead in C++)
  { const lifted::Result o = f82148F80(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8226FB28); r3 = o.r3; f1 = o.f1; }
  // b 0x8226fd54
  goto loc_8226FD54;
loc_8226FB2C:
  // stw r28,9720(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9720_address(), uint32_t(r28));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x8226fb44
  if (cr6.eq) goto loc_8226FB44;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FB44:
  // li r10,28
  r10 = uint64_t(int64_t(28));
  // stw r26,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r26));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r25,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r25));
  // stw r26,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r26));
  // stw r10,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r10));
  // b 0x8226fce0
  goto loc_8226FCE0;
loc_8226FB60:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r31,9664(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9664_address(), uint32_t(r31));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stw r31,9668(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9668_address(), uint32_t(r31));
  // li r9,177
  r9 = uint64_t(int64_t(177));
  // li r8,136
  r8 = uint64_t(int64_t(136));
  // stw r11,9656(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9656_address(), uint32_t(r11));
  // li r11,135
  r11 = uint64_t(int64_t(135));
  // stw r10,9660(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9660_address(), uint32_t(r10));
  // stw r9,9672(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9672_address(), uint32_t(r9));
  // stw r8,9676(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9676_address(), uint32_t(r8));
  // stw r11,9680(r29)
  guest::StoreU32(schema::NetMachine(uint32_t(r29)).unknown_9680_address(), uint32_t(r11));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x8226fba4
  if (cr6.eq) goto loc_8226FBA4;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FBA4:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r28,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r28));
  // li r9,26
  r9 = uint64_t(int64_t(26));
  // stw r26,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r26));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r10));
  // stw r9,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r9));
  // lwz r11,24932(r21)
  r11 = guest::LoadU32(uint32_t(r21) + 24932);
  // stw r31,12(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_first_item_bits(uint32_t(r31));
  // stw r10,16(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_last_item_bits(uint32_t(r10));
  // stw r28,20(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_wraps_bits(uint32_t(r28));
  // stw r31,0(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_cursor_bits(uint32_t(r31));
  // stw r31,4(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_bits(uint32_t(r31));
  // stw r31,8(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_moved_bits(uint32_t(r31));
  // stw r31,40(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_wrapped_bits(uint32_t(r31));
  // stw r31,48(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_visible_row_bits(uint32_t(r31));
  // stw r31,52(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_previous_row_bits(uint32_t(r31));
  // stw r31,36(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_side_bits(uint32_t(r31));
  // stw r31,64(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_player_bits(uint32_t(r31));
  // stw r31,24(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_up_down_locked_bits(uint32_t(r31));
  // stw r31,28(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_left_right_steps_bits(uint32_t(r31));
  // stw r31,32(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_shoulders_step_bits(uint32_t(r31));
  // b 0x8226fce0
  goto loc_8226FCE0;
loc_8226FC00:
  // lwz r11,9716(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9716_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226fcdc
  if (!cr6.eq) goto loc_8226FCDC;
  // lwz r11,4(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 4);
  // lwz r10,8(r23)
  r10 = guest::LoadU32(uint32_t(r23) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226fc24
  if (!cr6.gt) goto loc_8226FC24;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226FC24:
  // blt cr6,0x8226fc2c
  if (cr6.lt) goto loc_8226FC2C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226FC2C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,256
  r8 = uint64_t(int64_t(256));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // addi r5,r11,14
  r5 = uint64_t(int64_t(int64_t(r11) + 14));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = game::WideCharToMultiByte(LIFTED_ARGS(0x8226FC50, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 4);
  // lwz r10,8(r23)
  r10 = guest::LoadU32(uint32_t(r23) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226fc68
  if (!cr6.gt) goto loc_8226FC68;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226FC68:
  // blt cr6,0x8226fc70
  if (cr6.lt) goto loc_8226FC70;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226FC70:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226fc80
  if (!cr6.gt) goto loc_8226FC80;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226FC80:
  // blt cr6,0x8226fc88
  if (cr6.lt) goto loc_8226FC88;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226FC88:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r5,332(r11)
  r5 = guest::LoadU32(uint32_t(r11) + 332);
  // ld r4,144(r11)
  r4 = guest::LoadU64(uint32_t(r11) + 144);
  // bl 0x82260680
  // (left out: dead in C++)
  f82260680(r3, r4, r5);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 28180);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8226fcb0
  if (!cr6.eq) goto loc_8226FCB0;
  // bl 0x8214afe8
  // (left out: dead in C++)
  { const lifted::Result o = f8214AFE8(r1, r13); r3 = o.r3; f1 = o.f1; }
  // b 0x8226fcb4
  goto loc_8226FCB4;
loc_8226FCB0:
  // bl 0x8214b0a8
  // (left out: dead in C++)
  { const lifted::Result o = f8214B0A8(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
loc_8226FCB4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8226FCBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x8226fcd0
  if (cr6.eq) goto loc_8226FCD0;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FCD0:
  // li r10,28
  r10 = uint64_t(int64_t(28));
  // stw r18,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r18));
  // b 0x8226fd48
  goto loc_8226FD48;
loc_8226FCDC:
  // li r3,3
  r3 = uint64_t(int64_t(3));
loc_8226FCE0:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8226FCE4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8226fd54
  goto loc_8226FD54;
loc_8226FCE8:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24932(r21)
  r3 = guest::LoadU32(uint32_t(r21) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8226FCF4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226fd54
  if (cr0.eq) goto loc_8226FD54;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8226FD04, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r21)
  r11 = guest::LoadU32(uint32_t(r21) + 24932);
  // lwz r11,0(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x8226fd54
  if (cr6.eq) goto loc_8226FD54;
  // lwz r11,24932(r21)
  r11 = guest::LoadU32(uint32_t(r21) + 24932);
  // mr r10,r11
  r10 = r11;
  // mr r9,r11
  r9 = r11;
  // lwz r10,0(r10)
  r10 = schema::FrontEndMenu(uint32_t(r10)).cursor_bits();
  // stw r22,0(r11)
  schema::FrontEndMenu(uint32_t(r11)).set_cursor_bits(uint32_t(r22));
  // stw r10,4(r9)
  schema::FrontEndMenu(uint32_t(r9)).set_previous_bits(uint32_t(r10));
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226fd40
  if (cr6.eq) goto loc_8226FD40;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FD40:
  // li r10,17
  r10 = uint64_t(int64_t(17));
  // stw r22,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r22));
loc_8226FD48:
  // stw r26,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r26));
  // stw r26,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r26));
  // stw r10,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r10));
loc_8226FD54:
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // lwz r3,24932(r21)
  r3 = guest::LoadU32(uint32_t(r21) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8226FD60, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8226fda0
  if (cr0.eq) goto loc_8226FDA0;
  // lwz r11,9708(r29)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r29)).unknown_9708_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8226fda0
  if (!cr6.eq) goto loc_8226FDA0;
  // lwz r11,4(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 4);
  // lwz r10,8(r23)
  r10 = guest::LoadU32(uint32_t(r23) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8226fd8c
  if (!cr6.gt) goto loc_8226FD8C;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_8226FD8C:
  // blt cr6,0x8226fd94
  if (cr6.lt) goto loc_8226FD94;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8226FD94:
  // lwz r3,-21596(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21596);
  // ld r4,144(r11)
  r4 = guest::LoadU64(uint32_t(r11) + 144);
  // bl 0x822952d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XShowGamerCardUI(r1, r13, r3, r4, r6, r7, r8, r9, r10, 0x8226FDA0); r3 = o.r3; f1 = o.f1; }
loc_8226FDA0:
  // lwz r11,24932(r21)
  r11 = guest::LoadU32(uint32_t(r21) + 24932);
  // lwz r11,8(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).moved_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8226fe60
  if (cr6.eq) goto loc_8226FE60;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8226FDB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24928(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24928);
  // lwz r10,12(r11)
  r10 = schema::PageObject(uint32_t(r11)).step_bits();
  // cmpwi cr6,r10,21
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 21, so);
  // beq cr6,0x8226fe40
  if (cr6.eq) goto loc_8226FE40;
  // cmpwi cr6,r10,22
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 22, so);
  // beq cr6,0x8226fe28
  if (cr6.eq) goto loc_8226FE28;
  // cmpwi cr6,r10,23
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 23, so);
  // beq cr6,0x8226fe10
  if (cr6.eq) goto loc_8226FE10;
  // cmpwi cr6,r10,24
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 24, so);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // beq cr6,0x8226fdfc
  if (cr6.eq) goto loc_8226FDFC;
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226fdf0
  if (cr6.eq) goto loc_8226FDF0;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FDF0:
  // stw r18,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r18));
  // stw r26,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r26));
  // b 0x8226fe58
  goto loc_8226FE58;
loc_8226FDFC:
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226fe08
  if (cr6.eq) goto loc_8226FE08;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FE08:
  // stw r20,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r20));
  // b 0x8226fe54
  goto loc_8226FE54;
loc_8226FE10:
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226fe20
  if (cr6.eq) goto loc_8226FE20;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FE20:
  // stw r16,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r16));
  // b 0x8226fe54
  goto loc_8226FE54;
loc_8226FE28:
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226fe38
  if (cr6.eq) goto loc_8226FE38;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FE38:
  // stw r19,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r19));
  // b 0x8226fe54
  goto loc_8226FE54;
loc_8226FE40:
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8226fe50
  if (cr6.eq) goto loc_8226FE50;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FE50:
  // stw r17,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r17));
loc_8226FE54:
  // stw r18,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r18));
loc_8226FE58:
  // stw r26,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r26));
  // stw r22,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r22));
loc_8226FE60:
  // addi r1,r1,4320
  r1 = uint64_t(int64_t(int64_t(r1) + 4320));
  // b 0x825a6ea8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8226FE68: lifted from generated/ by lift.py.
lifted::Result f8226FE68(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8226FE68 (lifted)");
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
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(uint32_t(r1) + -4096);
  // stwu r1,-6768(r1)
  ea = -6768 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r30,r11,30752
  r30 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,88(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8226fee4
  if (cr6.eq) goto loc_8226FEE4;
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,14
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 14, so);
  // beq cr6,0x8226feb4
  if (cr6.eq) goto loc_8226FEB4;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FEB4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,14
  r9 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r10));
  // stw r9,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r9));
  // stw r10,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r10));
  // bl 0x82270a80
  // (left out: dead in C++)
  r3 = f82270A80(r1, r13);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // stw r11,88(r30)
  guest::StoreU32(uint32_t(r30) + 88, uint32_t(r11));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270100
  goto loc_82270100;
loc_8226FEE4:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8226eec8
  // (left out: dead in C++)
  { const lifted::Result o = f8226EEC8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8226bcb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BCB8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8226bbb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BBB8(r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4102
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4102, so);
  // bne cr6,0x8226ff7c
  if (!cr6.eq) goto loc_8226FF7C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r31,r11,29464
  r31 = uint64_t(int64_t(int64_t(r11) + 29464));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,256
  r8 = uint64_t(int64_t(256));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // addi r5,r31,14
  r5 = uint64_t(int64_t(int64_t(r31) + 14));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = game::WideCharToMultiByte(LIFTED_ARGS(0x8226FF48, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r5,332(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 332);
  // ld r4,144(r31)
  r4 = guest::LoadU64(uint32_t(r31) + 144);
  // bl 0x82260680
  // (left out: dead in C++)
  f82260680(r3, r4, r5);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x8226ff70
  if (cr6.eq) goto loc_8226FF70;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8226FF70:
  // li r9,28
  r9 = uint64_t(int64_t(28));
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // b 0x822700bc
  goto loc_822700BC;
loc_8226FF7C:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,27416
  r11 = uint64_t(int64_t(int64_t(r11) + 27416));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 12);
  // cmpwi cr6,r11,72
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 72, so);
  // bne cr6,0x8226ffac
  if (!cr6.eq) goto loc_8226FFAC;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,4176
  r3 = uint64_t(int64_t(int64_t(r1) + 4176));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // li r11,18
  r11 = uint64_t(int64_t(18));
  // stb r11,4178(r1)
  guest::StoreU8(uint32_t(r1) + 4178, uint8_t(r11));
  // b 0x82270008
  goto loc_82270008;
loc_8226FFAC:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,28184
  r11 = uint64_t(int64_t(int64_t(r11) + 28184));
  // lwz r11,344(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 344);
  // cmpwi cr6,r11,-999
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -999, so);
  // bne cr6,0x8226ffdc
  if (!cr6.eq) goto loc_8226FFDC;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,336
  r3 = uint64_t(int64_t(int64_t(r1) + 336));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // li r11,19
  r11 = uint64_t(int64_t(19));
  // stb r11,338(r1)
  guest::StoreU8(uint32_t(r1) + 338, uint8_t(r11));
  // b 0x82270008
  goto loc_82270008;
loc_8226FFDC:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,28664
  r11 = uint64_t(int64_t(int64_t(r11) + 28664));
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // cmpwi cr6,r11,-999
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -999, so);
  // bne cr6,0x82270010
  if (!cr6.eq) goto loc_82270010;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,1616
  r3 = uint64_t(int64_t(int64_t(r1) + 1616));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // li r11,20
  r11 = uint64_t(int64_t(20));
  // stb r11,1618(r1)
  guest::StoreU8(uint32_t(r1) + 1618, uint8_t(r11));
loc_82270008:
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(LIFTED_ARGS(0x8227000C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82270100
  goto loc_82270100;
loc_82270010:
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r11,27368(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 27368);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82270058
  if (!cr6.eq) goto loc_82270058;
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lwz r11,-29860(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -29860);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82270100
  if (cr6.eq) goto loc_82270100;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,2896
  r3 = uint64_t(int64_t(int64_t(r1) + 2896));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stb r11,2898(r1)
  guest::StoreU8(uint32_t(r1) + 2898, uint8_t(r11));
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(LIFTED_ARGS(0x8227004C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27368(r31)
  guest::StoreU32(uint32_t(r31) + 27368, uint32_t(r11));
  // b 0x82270100
  goto loc_82270100;
loc_82270058:
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24932(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82270068, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822700d4
  if (cr0.eq) goto loc_822700D4;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82270078, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,5456
  r3 = uint64_t(int64_t(int64_t(r1) + 5456));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // li r11,17
  r11 = uint64_t(int64_t(17));
  // stb r11,5458(r1)
  guest::StoreU8(uint32_t(r1) + 5458, uint8_t(r11));
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(LIFTED_ARGS(0x82270094, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,27368(r31)
  guest::StoreU32(uint32_t(r31) + 27368, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24928);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x822700b4
  if (cr6.eq) goto loc_822700B4;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_822700B4:
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // li r8,5
  r8 = uint64_t(int64_t(5));
loc_822700BC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r9));
  // stw r10,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r10));
  // stw r8,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r8));
  // stw r10,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r10));
  // b 0x82270100
  goto loc_82270100;
loc_822700D4:
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // lwz r3,24932(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 24932);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x822700E0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82270100
  if (cr0.eq) goto loc_82270100;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // ld r4,144(r11)
  r4 = guest::LoadU64(uint32_t(r11) + 144);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -21596);
  // bl 0x822952d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XShowGamerCardUI(r1, r13, r3, r4, r6, r7, r8, r9, r10, 0x82270100); r3 = o.r3; f1 = o.f1; }
loc_82270100:
  // addi r1,r1,6768
  r1 = uint64_t(int64_t(int64_t(r1) + 6768));
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

// sub_82270118: lifted from generated/ by lift.py.
lifted::Result f82270118(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82270118 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
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
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270144, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822701e4
  if (!cr6.eq) goto loc_822701E4;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24932);
  // lwz r11,0(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x82270174
  if (!cr6.eq) goto loc_82270174;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x8226c500
  // (left out: dead in C++)
  { const lifted::Result o = f8226C500(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82270174:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8226eec8
  // (left out: dead in C++)
  { const lifted::Result o = f8226EEC8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9672(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 9672);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,3
  r4 = uint64_t(int64_t(int64_t(r11) + 3));
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8211BC18, LIFTED_ARGS(0x82270194, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x822701ac
  if (!cr6.eq) goto loc_822701AC;
  // lwz r11,9672(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 9672);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,3
  r4 = uint64_t(int64_t(int64_t(r11) + 3));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822701AC, r31)); r3 = o.r3; f1 = o.f1; }
loc_822701AC:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822701BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x822701C4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = lifted::Call(0x821E7DA0, LIFTED_ARGS(0x822701DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(uint32_t(r29) + 0, uint32_t(r11));
loc_822701E4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822701F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82270200: lifted from generated/ by lift.py.
lifted::Result f82270200(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82270200 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
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
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8227022C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822702a4
  if (!cr6.eq) goto loc_822702A4;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24932);
  // lwz r11,0(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x8227025c
  if (!cr6.eq) goto loc_8227025C;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r29
  r3 = r29;
  // bl 0x8226c500
  // (left out: dead in C++)
  { const lifted::Result o = f8226C500(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8227025C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x8227026C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270274, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r7,67
  r7 = uint64_t(int64_t(67));
  // li r6,235
  r6 = uint64_t(int64_t(235));
  // li r5,205
  r5 = uint64_t(int64_t(205));
  // mr r3,r30
  r3 = r30;
  // bl 0x821e7da0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x821E7DA0, LIFTED_ARGS(0x8227028C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8226eec8
  // (left out: dead in C++)
  { const lifted::Result o = f8226EEC8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_822702A4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822702B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822702C0: lifted from generated/ by lift.py.
lifted::Result f822702C0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822702C0 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // bl 0x822718c0
  // (left out: dead in C++)
  { const lifted::Result o = f822718C0(r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9644(r30)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r30)).unknown_9644_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24928(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24928);
  // lwz r11,0(r11)
  r11 = schema::PageObject(uint32_t(r11)).page_bits();
  // bne cr6,0x82270484
  if (!cr6.eq) goto loc_82270484;
  // cmplwi cr6,r11,19
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 19, so);
  // bgt cr6,0x82270510
  if (cr6.gt) goto loc_82270510;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6248
  r12 = uint64_t(int64_t(int64_t(r12) + 6248));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32217
  r12 = uint64_t(int64_t(-2111373312));
  // addi r12,r12,808
  r12 = uint64_t(int64_t(int64_t(r12) + 808));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_82270328;
  return {r3, f1};
  case 1:
  goto loc_82270338;
  case 2:
  goto loc_82270348;
  case 3:
  goto loc_82270358;
  case 4:
  goto loc_82270368;
  case 5:
  goto loc_82270378;
  case 6:
  goto loc_82270388;
  case 7:
  goto loc_82270510;
  case 8:
  goto loc_82270510;
  case 9:
  goto loc_82270510;
  case 10:
  goto loc_82270510;
  case 11:
  goto loc_82270510;
  case 12:
  goto loc_82270510;
  case 13:
  goto loc_82270398;
  case 14:
  goto loc_822703A8;
  case 15:
  goto loc_8227040C;
  case 16:
  goto loc_82270414;
  case 17:
  goto loc_8227041C;
  case 18:
  goto loc_82270424;
  case 19:
  goto loc_82270434;
  default:
  std::abort();
  }
loc_82270338:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x82269dd0
  // (left out: dead in C++)
  { const lifted::Result o = f82269DD0(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_82270348:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x822692d8
  // (left out: dead in C++)
  { const lifted::Result o = f822692D8(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_82270358:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226a210
  // (left out: dead in C++)
  { const lifted::Result o = f8226A210(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_82270368:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226f708
  // (left out: dead in C++)
  { const lifted::Result o = f8226F708(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_82270378:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226f470
  // (left out: dead in C++)
  { const lifted::Result o = f8226F470(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_82270388:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226fe68
  // (left out: dead in C++)
  { const lifted::Result o = f8226FE68(r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_82270398:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226aa48
  // (left out: dead in C++)
  { const lifted::Result o = f8226AA48(r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_822703A8:
  // li r10,5
  r10 = uint64_t(int64_t(5));
loc_822703AC:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,25008(r9)
  guest::StoreU32(uint32_t(r9) + 25008, uint32_t(r10));
  // li r9,18
  r9 = uint64_t(int64_t(18));
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_count_bits();
  // lwz r6,604(r11)
  r6 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9644(r30)
  guest::StoreU32(schema::NetMachine(uint32_t(r30)).unknown_9644_address(), uint32_t(r11));
  // b 0x82270510
  goto loc_82270510;
loc_8227040C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x822703ac
  goto loc_822703AC;
loc_82270414:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // b 0x822703ac
  goto loc_822703AC;
loc_8227041C:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // b 0x822703ac
  goto loc_822703AC;
loc_82270424:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226ab48
  // (left out: dead in C++)
  { const lifted::Result o = f8226AB48(r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_82270434:
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
  r10 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_count_bits();
  // lwz r6,604(r11)
  r6 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  // b 0x82270510
  goto loc_82270510;
loc_82270484:
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x82270504
  if (cr6.eq) goto loc_82270504;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x822704f4
  if (cr6.eq) goto loc_822704F4;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // beq cr6,0x822704e4
  if (cr6.eq) goto loc_822704E4;
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // beq cr6,0x822704d4
  if (cr6.eq) goto loc_822704D4;
  // cmpwi cr6,r11,11
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 11, so);
  // beq cr6,0x822704c4
  if (cr6.eq) goto loc_822704C4;
  // cmpwi cr6,r11,12
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 12, so);
  // bne cr6,0x82270510
  if (!cr6.eq) goto loc_82270510;
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x82268a78
  // (left out: dead in C++)
  { const lifted::Result o = f82268A78(r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_822704C4:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x82268928
  // (left out: dead in C++)
  { const lifted::Result o = f82268928(r1, r13, r3, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_822704D4:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226a778
  // (left out: dead in C++)
  { const lifted::Result o = f8226A778(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510;
loc_822704E4:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226a940
  // (left out: dead in C++)
  r3 = f8226A940(r1, r13, r3, r4);
  // b 0x82270510
  goto loc_82270510;
loc_822704F4:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226a660
  // (left out: dead in C++)
  r3 = f8226A660(r1, r13, r3, r4);
  // b 0x82270510
  goto loc_82270510;
loc_82270504:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8226a500
  // (left out: dead in C++)
  { const lifted::Result o = f8226A500(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82270510:
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
loc_frag_82270328:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x82269988
  // (left out: dead in C++)
  { const lifted::Result o = f82269988(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82270510
  goto loc_82270510_82270328;
loc_82270510_82270328:
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

// sub_82270528: lifted from generated/ by lift.py.
lifted::Result f82270528(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82270528 (lifted)");
  uint64_t r0 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270558, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r10,24928(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24928);
  // addi r4,r10,24
  r4 = uint64_t(int64_t(int64_t(r10) + 24));
  // lwz r11,12(r10)
  r11 = schema::PageObject(uint32_t(r10)).step_bits();
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82270594
  if (!cr6.eq) goto loc_82270594;
  // lwz r11,0(r10)
  r11 = schema::PageObject(uint32_t(r10)).page_bits();
  // cmpwi cr6,r11,19
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 19, so);
  // beq cr6,0x82270778
  if (cr6.eq) goto loc_82270778;
  // cmpwi cr6,r11,18
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 18, so);
  // beq cr6,0x82270778
  if (cr6.eq) goto loc_82270778;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82270590, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82270778
  goto loc_82270778;
loc_82270594:
  // cmplwi cr6,r11,28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 28, so);
  // bgt cr6,0x8227073c
  if (cr6.gt) goto loc_8227073C;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6272
  r12 = uint64_t(int64_t(int64_t(r12) + 6272));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32217
  r12 = uint64_t(int64_t(-2111373312));
  // addi r12,r12,1476
  r12 = uint64_t(int64_t(int64_t(r12) + 1476));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_822705C4;
  return {r3, f1};
  case 1:
  goto loc_822705F4;
  case 2:
  goto loc_82270600;
  case 3:
  goto loc_8227060C;
  case 4:
  goto loc_8227060C;
  case 5:
  goto loc_8227061C;
  case 6:
  goto loc_82270628;
  case 7:
  goto loc_82270634;
  case 8:
  goto loc_82270644;
  case 9:
  goto loc_8227064C;
  case 10:
  goto loc_82270658;
  case 11:
  goto loc_82270664;
  case 12:
  goto loc_82270670;
  case 13:
  goto loc_8227067C;
  case 14:
  goto loc_82270688;
  case 15:
  goto loc_82270694;
  case 16:
  goto loc_822706A0;
  case 17:
  goto loc_822706AC;
  case 18:
  goto loc_822706B8;
  case 19:
  goto loc_822706C4;
  case 20:
  goto loc_822706D0;
  case 21:
  goto loc_822706DC;
  case 22:
  goto loc_822706E8;
  case 23:
  goto loc_822706F4;
  case 24:
  goto loc_82270700;
  case 25:
  goto loc_8227070C;
  case 26:
  goto loc_82270718;
  case 27:
  goto loc_82270724;
  case 28:
  goto loc_82270730;
  default:
  std::abort();
  }
loc_822705CC:
  // lwz r10,24928(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24928);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82270778
  if (cr0.eq) goto loc_82270778;
  // lwz r9,16(r10)
  r9 = schema::PageObject(uint32_t(r10)).queued_step_bits();
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82270760
  if (!cr6.eq) goto loc_82270760;
  // lwz r8,4(r10)
  r8 = schema::PageObject(uint32_t(r10)).next_page_bits();
  // stw r8,0(r10)
  schema::PageObject(uint32_t(r10)).set_page_bits(uint32_t(r8));
  // b 0x82270770
  goto loc_82270770;
loc_822705F4:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226abe0
  // (left out: dead in C++)
  { const lifted::Result o = f8226ABE0(r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270600:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226dad8
  // (left out: dead in C++)
  { const lifted::Result o = f8226DAD8(r1, r13, r3, r4, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_8227060C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8226c9e0
  // (left out: dead in C++)
  { const lifted::Result o = f8226C9E0(r1, r13, r3, r4, r5, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_8227061C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226acb0
  // (left out: dead in C++)
  { const lifted::Result o = f8226ACB0(r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270628:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226dc18
  // (left out: dead in C++)
  { const lifted::Result o = f8226DC18(r1, r13, r3, r4, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270634:
  // li r5,1
  r5 = uint64_t(int64_t(1));
loc_82270638:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226c898
  // (left out: dead in C++)
  { const lifted::Result o = f8226C898(r1, r13, r3, r4, r5, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270644:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // b 0x82270638
  goto loc_82270638;
loc_8227064C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226dda0
  // (left out: dead in C++)
  { const lifted::Result o = f8226DDA0(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270658:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226adb0
  // (left out: dead in C++)
  { const lifted::Result o = f8226ADB0(r1, r13, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270664:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226ae88
  // (left out: dead in C++)
  { const lifted::Result o = f8226AE88(r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270670:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226e000
  // (left out: dead in C++)
  { const lifted::Result o = f8226E000(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_8227067C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226af50
  // (left out: dead in C++)
  { const lifted::Result o = f8226AF50(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270688:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226afd8
  // (left out: dead in C++)
  { const lifted::Result o = f8226AFD8(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270694:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226b120
  // (left out: dead in C++)
  { const lifted::Result o = f8226B120(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_822706A0:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226b1a8
  // (left out: dead in C++)
  { const lifted::Result o = f8226B1A8(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_822706AC:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226b2f0
  // (left out: dead in C++)
  { const lifted::Result o = f8226B2F0(r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_822706B8:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226e1c8
  // (left out: dead in C++)
  { const lifted::Result o = f8226E1C8(r1, r13, r3, r4, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_822706C4:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226b4d0
  // (left out: dead in C++)
  { const lifted::Result o = f8226B4D0(r1, r13, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_822706D0:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226b620
  // (left out: dead in C++)
  { const lifted::Result o = f8226B620(r1, r13, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_822706DC:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226e3c0
  // (left out: dead in C++)
  { const lifted::Result o = f8226E3C0(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_822706E8:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226e4d8
  // (left out: dead in C++)
  { const lifted::Result o = f8226E4D8(r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_822706F4:
  // mr r3,r30
  r3 = r30;
  // bl 0x82270118
  // (left out: dead in C++)
  { const lifted::Result o = f82270118(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270700:
  // mr r3,r30
  r3 = r30;
  // bl 0x82270200
  // (left out: dead in C++)
  { const lifted::Result o = f82270200(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_8227070C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226e620
  // (left out: dead in C++)
  { const lifted::Result o = f8226E620(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270718:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226e8b8
  // (left out: dead in C++)
  { const lifted::Result o = f8226E8B8(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270724:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226ea88
  // (left out: dead in C++)
  { const lifted::Result o = f8226EA88(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_82270730:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226b780
  // (left out: dead in C++)
  { const lifted::Result o = f8226B780(r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822705cc
  goto loc_822705CC;
loc_8227073C:
  // lwz r9,16(r10)
  r9 = guest::LoadU32(uint32_t(r10) + 16);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82270760
  if (!cr6.eq) goto loc_82270760;
  // lwz r9,4(r10)
  r9 = guest::LoadU32(uint32_t(r10) + 4);
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // stw r9,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,12(r10)
  guest::StoreU32(uint32_t(r10) + 12, uint32_t(r8));
  // b 0x82270774
  goto loc_82270774;
loc_82270760:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // lwz r9,16(r10)
  r9 = guest::LoadU32(uint32_t(r10) + 16);
  // stw r8,16(r10)
  guest::StoreU32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r11,20(r10)
  guest::StoreU32(uint32_t(r10) + 20, uint32_t(r11));
loc_82270770:
  // stw r9,12(r10)
  guest::StoreU32(uint32_t(r10) + 12, uint32_t(r9));
loc_82270774:
  // stw r11,24(r10)
  guest::StoreU32(uint32_t(r10) + 24, uint32_t(r11));
loc_82270778:
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
loc_frag_822705C4:
  // mr r3,r30
  r3 = r30;
  // bl 0x8226d7b0
  // (left out: dead in C++)
  { const lifted::Result o = f8226D7B0(r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r10,24928(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24928);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82270778
  if (cr0.eq) goto loc_82270778_822705C4;
  // lwz r9,16(r10)
  r9 = guest::LoadU32(uint32_t(r10) + 16);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82270760
  if (!cr6.eq) goto loc_82270760_822705C4;
  // lwz r8,4(r10)
  r8 = guest::LoadU32(uint32_t(r10) + 4);
  // stw r8,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r8));
  // b 0x82270770
  goto loc_82270770_822705C4;
loc_82270760_822705C4:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // lwz r9,16(r10)
  r9 = guest::LoadU32(uint32_t(r10) + 16);
  // stw r8,16(r10)
  guest::StoreU32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r11,20(r10)
  guest::StoreU32(uint32_t(r10) + 20, uint32_t(r11));
loc_82270770_822705C4:
  // stw r9,12(r10)
  guest::StoreU32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,24(r10)
  guest::StoreU32(uint32_t(r10) + 24, uint32_t(r11));
loc_82270778_822705C4:
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

// sub_82270790: lifted from generated/ by lift.py.
uint64_t f82270790(uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82270790 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // addi r11,r4,-1
  r11 = uint64_t(int64_t(int64_t(r4) + -1));
  // mr r3,r5
  r3 = r5;
  // cmpw cr6,r11,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r6), so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // addi r3,r6,1
  r3 = uint64_t(int64_t(int64_t(r6) + 1));
  // blr
  return r3;
}

// sub_822707A8: lifted from generated/ by lift.py.
uint64_t f822707A8(uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_822707A8 (lifted)");
  uint64_t r3 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmpw cr6,r5,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r5), int32_t(r6), so);
  // addi r3,r4,-1
  r3 = uint64_t(int64_t(int64_t(r4) + -1));
  // beqlr cr6
  if (cr6.eq) return r3;
  // addi r3,r6,-1
  r3 = uint64_t(int64_t(int64_t(r6) + -1));
  // blr
  return r3;
}

// sub_822707C0: lifted from generated/ by lift.py.
uint64_t f822707C0(uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822707C0 (lifted)");
  uint64_t r3 = 0;
  // addi r3,r4,416
  r3 = uint64_t(int64_t(int64_t(r4) + 416));
  // blr
  return r3;
}

// sub_822707C8: lifted from generated/ by lift.py.
uint64_t f822707C8(uint64_t r1, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_822707C8 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r11,395
  r11 = uint64_t(int64_t(395));
  // cmpwi cr6,r4,6
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 6, so);
  // stw r11,-32(r1)
  guest::StoreU32(uint32_t(r1) + -32, uint32_t(r11));
  // li r11,396
  r11 = uint64_t(int64_t(396));
  // stw r11,-28(r1)
  guest::StoreU32(uint32_t(r1) + -28, uint32_t(r11));
  // li r11,397
  r11 = uint64_t(int64_t(397));
  // stw r11,-24(r1)
  guest::StoreU32(uint32_t(r1) + -24, uint32_t(r11));
  // li r11,398
  r11 = uint64_t(int64_t(398));
  // stw r11,-20(r1)
  guest::StoreU32(uint32_t(r1) + -20, uint32_t(r11));
  // li r11,401
  r11 = uint64_t(int64_t(401));
  // stw r11,-16(r1)
  guest::StoreU32(uint32_t(r1) + -16, uint32_t(r11));
  // li r11,402
  r11 = uint64_t(int64_t(402));
  // stw r11,-12(r1)
  guest::StoreU32(uint32_t(r1) + -12, uint32_t(r11));
  // bne cr6,0x82270818
  if (!cr6.eq) goto loc_82270818;
  // cmpwi cr6,r5,1
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 1, so);
  // bne cr6,0x82270810
  if (!cr6.eq) goto loc_82270810;
  // li r3,420
  r3 = uint64_t(int64_t(420));
  // blr
  return r3;
loc_82270810:
  // li r3,416
  r3 = uint64_t(int64_t(416));
  // blr
  return r3;
loc_82270818:
  // rlwinm r11,r4,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,-32
  r10 = uint64_t(int64_t(int64_t(r1) + -32));
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // blr
  return r3;
}

// sub_82270828: lifted from generated/ by lift.py.
uint64_t f82270828(uint64_t r1, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82270828 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0;
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // rlwinm r11,r4,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // stw r9,-16(r1)
  guest::StoreU32(uint32_t(r1) + -16, uint32_t(r9));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,-12(r1)
  guest::StoreU32(uint32_t(r1) + -12, uint32_t(r9));
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // blr
  return r3;
}

// sub_82270848: lifted from generated/ by lift.py.
lifted::Result f82270848(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82270848 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
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
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270874, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // lwz r4,9680(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9680);
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82270884, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // lwz r4,9676(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9676);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82270890, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // bne cr6,0x822708b4
  if (!cr6.eq) goto loc_822708B4;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,9672(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 9672);
  // mr r3,r30
  r3 = r30;
  // lwz r11,24932(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24932);
  // lwz r11,4(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).previous_bits();
  // add r4,r10,r11
  r4 = r10 + r11;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822708B4, r31)); r3 = o.r3; f1 = o.f1; }
loc_822708B4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822708C0: lifted from generated/ by lift.py.
uint64_t f822708C0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822708C0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(ea, uint32_t(r1));
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
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822708EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24932);
  // lwz r10,8(r11)
  r10 = schema::FrontEndMenu(uint32_t(r11)).moved_bits();
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8227099c
  if (!cr6.eq) goto loc_8227099C;
  // lwz r10,9656(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 9656);
  // lwz r9,4(r11)
  r9 = schema::FrontEndMenu(uint32_t(r11)).previous_bits();
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x82270954
  if (!cr6.eq) goto loc_82270954;
  // lwz r11,0(r11)
  r11 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82270954
  if (!cr6.eq) goto loc_82270954;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82270930, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24932);
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9668);
  // lwz r30,0(r11)
  r30 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270940, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // b 0x822709f0
  goto loc_822709F0;
loc_82270954:
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // bne cr6,0x82270994
  if (!cr6.eq) goto loc_82270994;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x82270994
  if (!cr6.eq) goto loc_82270994;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82270970, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24932);
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9668);
  // lwz r30,0(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270980, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // b 0x822709f0
  goto loc_822709F0;
loc_82270994:
  // lwz r4,9680(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9680);
  // b 0x82270a04
  goto loc_82270A04;
loc_8227099C:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82270a18
  if (!cr6.eq) goto loc_82270A18;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82270a00
  if (!cr6.eq) goto loc_82270A00;
  // lwz r10,9656(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 9656);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x82270a00
  if (!cr6.eq) goto loc_82270A00;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822709D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24932);
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9668);
  // lwz r30,0(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x822709E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
loc_822709F0:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r3)).y_address(), temp.u32);
  // b 0x82270a18
  goto loc_82270A18;
loc_82270A00:
  // lwz r4,9676(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9676);
loc_82270A04:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82270A18, r31)); r3 = o.r3; f1 = o.f1; }
loc_82270A18:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82270A20: lifted from generated/ by lift.py.
uint64_t f82270A20(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82270A20 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270A44, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r11,24848(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82270a68
  if (cr6.eq) goto loc_82270A68;
  // li r6,1
  r6 = uint64_t(int64_t(1));
loc_82270A68:
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270A6C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82270A80: lifted from generated/ by lift.py.
uint64_t f82270A80(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82270A80 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270AA8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82270ABC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82270ACC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82270ADC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82270AEC, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270B00, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270B14, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82270B28: lifted from generated/ by lift.py.
uint64_t f82270B28(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82270B28 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270B50, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82270B64, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82270B74, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82270B84, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82270B94, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270BA8, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270BBC, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270BD0, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270BE4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82270BF8: lifted from generated/ by lift.py.
uint64_t f82270BF8(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82270BF8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270C20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82270C34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82270C44, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82270C54, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82270C64, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270C78, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270C8C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270CA0, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270CB4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82270CC8: lifted from generated/ by lift.py.
uint64_t f82270CC8(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82270CC8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270CF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82270D04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82270D14, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82270D24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82270D34, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270D48, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270D5C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270D70, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270D84, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270D98, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82270DAC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82270DC0: lifted from generated/ by lift.py.
lifted::Result f82270DC0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82270DC0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
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
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270DE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82270DFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270E08, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,310
  r5 = uint64_t(int64_t(310));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82270E18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270E24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,311
  r5 = uint64_t(int64_t(311));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82270E34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270E40, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,110
  r5 = uint64_t(int64_t(110));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82270E50, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270E5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,313
  r5 = uint64_t(int64_t(313));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82270E6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270E78, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,115
  r5 = uint64_t(int64_t(115));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82270E88, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24932);
  // lwz r4,0(r11)
  r4 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // bl 0x8226c500
  // (left out: dead in C++)
  { const lifted::Result o = f8226C500(r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8226bbb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BBB8(r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8226bcb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BCB8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8226eec8
  // (left out: dead in C++)
  { const lifted::Result o = f8226EEC8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82270a20
  // (left out: dead in C++)
  r3 = f82270A20(r1, r13);
  // mr r3,r31
  r3 = r31;
  // lwz r5,9664(r30)
  r5 = guest::LoadU32(uint32_t(r30) + 9664);
  // lwz r4,9660(r30)
  r4 = guest::LoadU32(uint32_t(r30) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82270EDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24932);
  // lwz r4,9668(r30)
  r4 = guest::LoadU32(uint32_t(r30) + 9668);
  // lwz r29,0(r11)
  r29 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270EEC, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).y_address(), temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82270F18, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82270F34, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82270F40: lifted from generated/ by lift.py.
lifted::Result f82270F40(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82270F40 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,39
  r11 = uint64_t(int64_t(39));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82270F68, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r30,0(r11)
  r30 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r11)).value_address());
  // bl 0x82270bf8
  // (left out: dead in C++)
  r3 = f82270BF8(r1, r13);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82270F88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270F94, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,308
  r5 = uint64_t(int64_t(308));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82270FA4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270FB0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,110
  r5 = uint64_t(int64_t(110));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82270FC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270FCC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,115
  r5 = uint64_t(int64_t(115));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82270FDC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82270FE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x82270FF4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82271000, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ClearNodeText(LIFTED_ARGS(0x8227100C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24932);
  // lwz r4,0(r11)
  r4 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // bl 0x8226c618
  // (left out: dead in C++)
  { const lifted::Result o = f8226C618(r1, r13, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8226bbb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BBB8(r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8226bcb8
  // (left out: dead in C++)
  { const lifted::Result o = f8226BCB8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8226eec8
  // (left out: dead in C++)
  { const lifted::Result o = f8226EEC8(r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82270a20
  // (left out: dead in C++)
  r3 = f82270A20(r1, r13);
  // mr r3,r30
  r3 = r30;
  // lwz r5,9664(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 9664);
  // lwz r4,9660(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9660);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82271060, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24932(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24932);
  // lwz r4,9668(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 9668);
  // lwz r29,0(r11)
  r29 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82271070, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // mr r11,r3
  r11 = r3;
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r30
  r3 = r30;
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  guest::StoreU32(schema::UiMovieNode(uint32_t(r11)).y_address(), temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8227109C, r31)); r3 = o.r3; f1 = o.f1; }
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
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822710B8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822710C0: lifted from generated/ by lift.py.
lifted::Result f822710C0(uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_822710C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-4032(r1)
  ea = -4032 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r3,r1,2128
  r3 = uint64_t(int64_t(int64_t(r1) + 2128));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // addi r4,r11,832
  r4 = uint64_t(int64_t(int64_t(r11) + 832));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x822710E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(LIFTED_ARGS(0x822710F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,2128(r1)
  r11 = guest::LoadU16(uint32_t(r1) + 2128);
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,2144
  r4 = uint64_t(int64_t(int64_t(r1) + 2144));
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // sth r11,80(r1)
  guest::StoreU16(uint32_t(r1) + 80, uint16_t(r11));
  // lhz r11,2130(r1)
  r11 = guest::LoadU16(uint32_t(r1) + 2130);
  // sth r11,82(r1)
  guest::StoreU16(uint32_t(r1) + 82, uint16_t(r11));
  // lhz r11,2132(r1)
  r11 = guest::LoadU16(uint32_t(r1) + 2132);
  // sth r11,84(r1)
  guest::StoreU16(uint32_t(r1) + 84, uint16_t(r11));
  // lwz r11,2136(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 2136);
  // rlwinm r10,r11,16,16,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF;
  // sth r11,88(r1)
  guest::StoreU16(uint32_t(r1) + 88, uint16_t(r11));
  // lwz r11,2140(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 2140);
  // sth r10,86(r1)
  guest::StoreU16(uint32_t(r1) + 86, uint16_t(r10));
  // stw r11,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82271134, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r4,r1,2224
  r4 = uint64_t(int64_t(int64_t(r1) + 2224));
  // li r5,192
  r5 = uint64_t(int64_t(192));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82271144, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,368
  r10 = uint64_t(int64_t(int64_t(r1) + 368));
  // addi r11,r1,3696
  r11 = uint64_t(int64_t(int64_t(r1) + 3696));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // mtctr r9
  ctr = r9;
loc_82271154:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r9,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r9));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x82271154
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82271154;
  // addi r3,r1,448
  r3 = uint64_t(int64_t(int64_t(r1) + 448));
  // addi r4,r1,3776
  r4 = uint64_t(int64_t(int64_t(r1) + 3776));
  // li r5,236
  r5 = uint64_t(int64_t(236));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82271178, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,3716
  r11 = uint64_t(int64_t(int64_t(r1) + 3716));
  // addi r10,r1,388
  r10 = uint64_t(int64_t(int64_t(r1) + 388));
  // li r9,15
  r9 = uint64_t(int64_t(15));
  // mtctr r9
  ctr = r9;
loc_82271188:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r9,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r9));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x82271188
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82271188;
  // addi r11,r1,2416
  r11 = uint64_t(int64_t(int64_t(r1) + 2416));
  // addi r10,r1,684
  r10 = uint64_t(int64_t(int64_t(r1) + 684));
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // mtctr r9
  ctr = r9;
loc_822711AC:
  // ld r9,0(r11)
  r9 = guest::LoadU64(uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  guest::StoreU64(uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x822711ac
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_822711AC;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r8,13
  r8 = uint64_t(int64_t(13));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // rlwinm r10,r31,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 3) & 0xFFFFFFF8;
  // addi r9,r11,144
  r9 = uint64_t(int64_t(int64_t(r11) + 144));
  // addi r3,r1,867
  r3 = uint64_t(int64_t(int64_t(r1) + 867));
  // stb r8,850(r1)
  guest::StoreU8(uint32_t(r1) + 850, uint8_t(r8));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r11,0(r11)
  r11 = guest::LoadU64(uint32_t(r11) + 0);
  // li r5,764
  r5 = uint64_t(int64_t(764));
  // rldicl r8,r11,8,56
  r8 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // stb r11,858(r1)
  guest::StoreU8(uint32_t(r1) + 858, uint8_t(r11));
  // stb r8,851(r1)
  guest::StoreU8(uint32_t(r1) + 851, uint8_t(r8));
  // rldicl r8,r11,40,24
  r8 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r8,852(r1)
  guest::StoreU32(uint32_t(r1) + 852, uint32_t(r8));
  // rldicl r8,r11,56,8
  r8 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // ldx r11,r10,r9
  r11 = guest::LoadU64(uint32_t(r10) + uint32_t(r9));
  // rldicl r10,r11,8,56
  r10 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // sth r8,856(r1)
  guest::StoreU16(uint32_t(r1) + 856, uint16_t(r8));
  // stb r11,866(r1)
  guest::StoreU8(uint32_t(r1) + 866, uint8_t(r11));
  // stb r10,859(r1)
  guest::StoreU8(uint32_t(r1) + 859, uint8_t(r10));
  // rldicl r10,r11,40,24
  r10 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r10,860(r1)
  guest::StoreU32(uint32_t(r1) + 860, uint32_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,864(r1)
  guest::StoreU16(uint32_t(r1) + 864, uint16_t(r10));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82271228, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82271290
  if (!cr6.eq) goto loc_82271290;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,29448
  r10 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8227125c
  if (!cr6.gt) goto loc_8227125C;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_8227125C:
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
  // beq 0x822712a4
  if (cr0.eq) goto loc_822712A4;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82271284
  if (!cr6.gt) goto loc_82271284;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_82271284:
  // blt cr6,0x82271298
  if (cr6.lt) goto loc_82271298;
  // twi 31,r0,22
  lifted::Trap(22);
  // b 0x82271298
  goto loc_82271298;
loc_82271290:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
loc_82271298:
  // addi r3,r1,848
  r3 = uint64_t(int64_t(int64_t(r1) + 848));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(LIFTED_ARGS(0x822712A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_822712A4:
  // addi r1,r1,4032
  r1 = uint64_t(int64_t(int64_t(r1) + 4032));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822712B8: lifted from generated/ by lift.py.
lifted::Result f822712B8(uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_822712B8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-3760(r1)
  ea = -3760 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r3,r1,1840
  r3 = uint64_t(int64_t(int64_t(r1) + 1840));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // addi r4,r11,832
  r4 = uint64_t(int64_t(int64_t(r11) + 832));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x822712E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(LIFTED_ARGS(0x822712EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,480
  r5 = uint64_t(int64_t(480));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(LIFTED_ARGS(0x82271300, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82271304:
  // addi r10,r1,2288
  r10 = uint64_t(int64_t(int64_t(r1) + 2288));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r4,r31,r10
  r4 = r31 + r10;
  // add r3,r31,r11
  r3 = r31 + r11;
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8227131C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,2448
  r10 = uint64_t(int64_t(int64_t(r1) + 2448));
  // addi r11,r1,240
  r11 = uint64_t(int64_t(int64_t(r1) + 240));
  // add r4,r31,r10
  r4 = r31 + r10;
  // add r3,r31,r11
  r3 = r31 + r11;
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82271334, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,32
  r31 = uint64_t(int64_t(int64_t(r31) + 32));
  // cmpwi cr6,r31,160
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 160, so);
  // blt cr6,0x82271304
  if (cr6.lt) goto loc_82271304;
  // addi r11,r1,2608
  r11 = uint64_t(int64_t(int64_t(r1) + 2608));
  // addi r10,r1,400
  r10 = uint64_t(int64_t(int64_t(r1) + 400));
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // mtctr r9
  ctr = r9;
loc_82271350:
  // ld r9,0(r11)
  r9 = guest::LoadU64(uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  guest::StoreU64(uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x82271350
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82271350;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r8,14
  r8 = uint64_t(int64_t(14));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // rlwinm r10,r30,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 3) & 0xFFFFFFF8;
  // addi r9,r11,144
  r9 = uint64_t(int64_t(int64_t(r11) + 144));
  // addi r3,r1,579
  r3 = uint64_t(int64_t(int64_t(r1) + 579));
  // stb r8,562(r1)
  guest::StoreU8(uint32_t(r1) + 562, uint8_t(r8));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r11,0(r11)
  r11 = guest::LoadU64(uint32_t(r11) + 0);
  // li r5,480
  r5 = uint64_t(int64_t(480));
  // rldicl r8,r11,8,56
  r8 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // stb r11,570(r1)
  guest::StoreU8(uint32_t(r1) + 570, uint8_t(r11));
  // stb r8,563(r1)
  guest::StoreU8(uint32_t(r1) + 563, uint8_t(r8));
  // rldicl r8,r11,40,24
  r8 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r8,564(r1)
  guest::StoreU32(uint32_t(r1) + 564, uint32_t(r8));
  // rldicl r8,r11,56,8
  r8 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // ldx r11,r10,r9
  r11 = guest::LoadU64(uint32_t(r10) + uint32_t(r9));
  // rldicl r10,r11,8,56
  r10 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // sth r8,568(r1)
  guest::StoreU16(uint32_t(r1) + 568, uint16_t(r8));
  // stb r11,578(r1)
  guest::StoreU8(uint32_t(r1) + 578, uint8_t(r11));
  // stb r10,571(r1)
  guest::StoreU8(uint32_t(r1) + 571, uint8_t(r10));
  // rldicl r10,r11,40,24
  r10 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r10,572(r1)
  guest::StoreU32(uint32_t(r1) + 572, uint32_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,576(r1)
  guest::StoreU16(uint32_t(r1) + 576, uint16_t(r10));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x822713CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82271434
  if (!cr6.eq) goto loc_82271434;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,29448
  r10 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82271400
  if (!cr6.gt) goto loc_82271400;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_82271400:
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
  // beq 0x82271448
  if (cr0.eq) goto loc_82271448;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82271428
  if (!cr6.gt) goto loc_82271428;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_82271428:
  // blt cr6,0x8227143c
  if (cr6.lt) goto loc_8227143C;
  // twi 31,r0,22
  lifted::Trap(22);
  // b 0x8227143c
  goto loc_8227143C;
loc_82271434:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
loc_8227143C:
  // addi r3,r1,560
  r3 = uint64_t(int64_t(int64_t(r1) + 560));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(LIFTED_ARGS(0x82271448, r31)); r3 = o.r3; f1 = o.f1; }
loc_82271448:
  // addi r1,r1,3760
  r1 = uint64_t(int64_t(int64_t(r1) + 3760));
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

// sub_82271460: lifted from generated/ by lift.py.
lifted::Result f82271460(uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82271460 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-3920(r1)
  ea = -3920 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r3,r1,2000
  r3 = uint64_t(int64_t(int64_t(r1) + 2000));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // addi r4,r11,832
  r4 = uint64_t(int64_t(int64_t(r11) + 832));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8227148C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(LIFTED_ARGS(0x82271494, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,640
  r5 = uint64_t(int64_t(640));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,1360
  r3 = uint64_t(int64_t(int64_t(r1) + 1360));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(LIFTED_ARGS(0x822714A8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,2928
  r10 = uint64_t(int64_t(int64_t(r1) + 2928));
  // addi r9,r1,1360
  r9 = uint64_t(int64_t(int64_t(r1) + 1360));
  // li r11,20
  r11 = uint64_t(int64_t(20));
  // mtctr r11
  ctr = r11;
loc_822714B8:
  // ld r11,0(r10)
  r11 = guest::LoadU64(uint32_t(r10) + 0);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // std r11,0(r9)
  guest::StoreU64(uint32_t(r9) + 0, r11);
  // addi r9,r9,8
  r9 = uint64_t(int64_t(int64_t(r9) + 8));
  // bdnz 0x822714b8
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_822714B8;
  // addi r11,r1,3088
  r11 = uint64_t(int64_t(int64_t(r1) + 3088));
  // addi r10,r1,1520
  r10 = uint64_t(int64_t(int64_t(r1) + 1520));
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // mtctr r9
  ctr = r9;
loc_822714DC:
  // ld r9,0(r11)
  r9 = guest::LoadU64(uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // std r9,0(r10)
  guest::StoreU64(uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x822714dc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_822714DC;
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_822714F4:
  // addi r10,r1,3248
  r10 = uint64_t(int64_t(int64_t(r1) + 3248));
  // addi r11,r1,1680
  r11 = uint64_t(int64_t(int64_t(r1) + 1680));
  // add r4,r31,r10
  r4 = r31 + r10;
  // add r3,r31,r11
  r3 = r31 + r11;
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8227150C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,3408
  r10 = uint64_t(int64_t(int64_t(r1) + 3408));
  // addi r11,r1,1840
  r11 = uint64_t(int64_t(int64_t(r1) + 1840));
  // add r4,r31,r10
  r4 = r31 + r10;
  // add r3,r31,r11
  r3 = r31 + r11;
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82271524, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,32
  r31 = uint64_t(int64_t(int64_t(r31) + 32));
  // cmpwi cr6,r31,160
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 160, so);
  // blt cr6,0x822714f4
  if (cr6.lt) goto loc_822714F4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r8,15
  r8 = uint64_t(int64_t(15));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // rlwinm r10,r30,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 3) & 0xFFFFFFF8;
  // addi r9,r11,144
  r9 = uint64_t(int64_t(int64_t(r11) + 144));
  // addi r3,r1,99
  r3 = uint64_t(int64_t(int64_t(r1) + 99));
  // stb r8,82(r1)
  guest::StoreU8(uint32_t(r1) + 82, uint8_t(r8));
  // addi r4,r1,1360
  r4 = uint64_t(int64_t(int64_t(r1) + 1360));
  // ld r11,0(r11)
  r11 = guest::LoadU64(uint32_t(r11) + 0);
  // li r5,640
  r5 = uint64_t(int64_t(640));
  // rldicl r8,r11,8,56
  r8 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // stb r11,90(r1)
  guest::StoreU8(uint32_t(r1) + 90, uint8_t(r11));
  // stb r8,83(r1)
  guest::StoreU8(uint32_t(r1) + 83, uint8_t(r8));
  // rldicl r8,r11,40,24
  r8 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r8,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r8));
  // rldicl r8,r11,56,8
  r8 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // ldx r11,r10,r9
  r11 = guest::LoadU64(uint32_t(r10) + uint32_t(r9));
  // rldicl r10,r11,8,56
  r10 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // sth r8,88(r1)
  guest::StoreU16(uint32_t(r1) + 88, uint16_t(r8));
  // stb r11,98(r1)
  guest::StoreU8(uint32_t(r1) + 98, uint8_t(r11));
  // stb r10,91(r1)
  guest::StoreU8(uint32_t(r1) + 91, uint8_t(r10));
  // rldicl r10,r11,40,24
  r10 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r10,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,96(r1)
  guest::StoreU16(uint32_t(r1) + 96, uint16_t(r10));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82271598, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82271600
  if (!cr6.eq) goto loc_82271600;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,29448
  r10 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822715cc
  if (!cr6.gt) goto loc_822715CC;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_822715CC:
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
  // beq 0x82271614
  if (cr0.eq) goto loc_82271614;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822715f4
  if (!cr6.gt) goto loc_822715F4;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_822715F4:
  // blt cr6,0x82271608
  if (cr6.lt) goto loc_82271608;
  // twi 31,r0,22
  lifted::Trap(22);
  // b 0x82271608
  goto loc_82271608;
loc_82271600:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
loc_82271608:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(LIFTED_ARGS(0x82271614, r31)); r3 = o.r3; f1 = o.f1; }
loc_82271614:
  // addi r1,r1,3920
  r1 = uint64_t(int64_t(int64_t(r1) + 3920));
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

// sub_82271630: lifted from generated/ by lift.py.
lifted::Result f82271630(uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82271630 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-1376(r1)
  ea = -1376 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82271700
  if (!cr6.eq) goto loc_82271700;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,29448
  r31 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r9,4(r31)
  r9 = guest::LoadU32(uint32_t(r31) + 4);
  // lwz r8,8(r31)
  r8 = guest::LoadU32(uint32_t(r31) + 8);
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // ble cr6,0x82271674
  if (!cr6.gt) goto loc_82271674;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_82271674:
  // subf r11,r9,r8
  r11 = r8 - r9;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82271700
  if (cr0.eq) goto loc_82271700;
  // li r11,24
  r11 = uint64_t(int64_t(24));
  // stb r11,82(r1)
  guest::StoreU8(uint32_t(r1) + 82, uint8_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,29308(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 29308);
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // bne cr6,0x822716cc
  if (!cr6.eq) goto loc_822716CC;
  // bl 0x82293568
  // (left out: dead in C++)
  { const lifted::Result o = game::rand_82293568(LIFTED_ARGS(0x822716A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // lwz r8,8(r31)
  r8 = guest::LoadU32(uint32_t(r31) + 8);
  // divw r11,r3,r11
  r11 = uint32_t((int32_t(r11) && !(int32_t(r3) == INT32_MIN && int32_t(r11) == -1)) ? int32_t(r3) / int32_t(r11) : 0);
  // mulli r9,r11,6
  r9 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(6))));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,27412
  r10 = uint64_t(int64_t(int64_t(r11) + 27412));
  // subf r11,r9,r3
  r11 = r3 - r9;
  // lwz r9,4(r31)
  r9 = guest::LoadU32(uint32_t(r31) + 4);
  // b 0x822716d4
  goto loc_822716D4;
loc_822716CC:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r10,r10,27412
  r10 = uint64_t(int64_t(int64_t(r10) + 27412));
loc_822716D4:
  // stw r11,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r11));
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // stb r11,83(r1)
  guest::StoreU8(uint32_t(r1) + 83, uint8_t(r11));
  // ble cr6,0x822716ec
  if (!cr6.gt) goto loc_822716EC;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
loc_822716EC:
  // blt cr6,0x822716f4
  if (cr6.lt) goto loc_822716F4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_822716F4:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,8(r9)
  r4 = guest::LoadU32(uint32_t(r9) + 8);
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(LIFTED_ARGS(0x82271700, r31)); r3 = o.r3; f1 = o.f1; }
loc_82271700:
  // addi r1,r1,1376
  r1 = uint64_t(int64_t(int64_t(r1) + 1376));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82271718: lifted from generated/ by lift.py.
lifted::Result f82271718(uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82271718 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-1376(r1)
  ea = -1376 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x822717c0
  if (!cr6.eq) goto loc_822717C0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,29448
  r10 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82271758
  if (!cr6.gt) goto loc_82271758;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_82271758:
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
  // beq 0x822717c0
  if (cr0.eq) goto loc_822717C0;
  // li r9,25
  r9 = uint64_t(int64_t(25));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // stb r9,82(r1)
  guest::StoreU8(uint32_t(r1) + 82, uint8_t(r9));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lwz r9,29308(r9)
  r9 = guest::LoadU32(uint32_t(r9) + 29308);
  // stb r9,83(r1)
  guest::StoreU8(uint32_t(r1) + 83, uint8_t(r9));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lwz r9,29312(r9)
  r9 = guest::LoadU32(uint32_t(r9) + 29312);
  // stb r9,84(r1)
  guest::StoreU8(uint32_t(r1) + 84, uint8_t(r9));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lwz r9,29316(r9)
  r9 = guest::LoadU32(uint32_t(r9) + 29316);
  // stb r9,85(r1)
  guest::StoreU8(uint32_t(r1) + 85, uint8_t(r9));
  // ble cr6,0x822717ac
  if (!cr6.gt) goto loc_822717AC;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_822717AC:
  // blt cr6,0x822717b4
  if (cr6.lt) goto loc_822717B4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_822717B4:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(LIFTED_ARGS(0x822717C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822717C0:
  // addi r1,r1,1376
  r1 = uint64_t(int64_t(int64_t(r1) + 1376));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822717D0: lifted from generated/ by lift.py.
lifted::Result f822717D0(uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_822717D0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-1376(r1)
  ea = -1376 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(LIFTED_ARGS(0x822717E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,23
  r10 = uint64_t(int64_t(23));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // stb r10,82(r1)
  guest::StoreU8(uint32_t(r1) + 82, uint8_t(r10));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,176
  r11 = uint64_t(int64_t(int64_t(r11) + 176));
  // addi r10,r10,30752
  r10 = uint64_t(int64_t(int64_t(r10) + 30752));
  // lwz r10,4(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 4);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // rlwinm r10,r3,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // stb r11,83(r1)
  guest::StoreU8(uint32_t(r1) + 83, uint8_t(r11));
  // bne cr6,0x82271894
  if (!cr6.eq) goto loc_82271894;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,29448
  r10 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82271838
  if (!cr6.gt) goto loc_82271838;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_82271838:
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
  // beq 0x822718b0
  if (cr0.eq) goto loc_822718B0;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82271860
  if (!cr6.gt) goto loc_82271860;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_82271860:
  // blt cr6,0x82271868
  if (cr6.lt) goto loc_82271868;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82271868:
  // lwz r9,8(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 8);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x822718b0
  if (cr6.eq) goto loc_822718B0;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82271884
  if (!cr6.gt) goto loc_82271884;
  // twi 31,r0,22
  lifted::Trap(22);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
loc_82271884:
  // blt cr6,0x8227188c
  if (cr6.lt) goto loc_8227188C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8227188C:
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // b 0x822718a8
  goto loc_822718A8;
loc_82271894:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 8);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x822718b0
  if (cr6.eq) goto loc_822718B0;
loc_822718A8:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(LIFTED_ARGS(0x822718B0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822718B0:
  // addi r1,r1,1376
  r1 = uint64_t(int64_t(int64_t(r1) + 1376));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822718C0: lifted from generated/ by lift.py.
lifted::Result f822718C0(uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_822718C0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r11,24876(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24876);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822719a8
  if (!cr6.eq) goto loc_822719A8;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24880(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24880);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82271908
  if (!cr6.eq) goto loc_82271908;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,24880(r11)
  guest::StoreU32(uint32_t(r11) + 24880, uint32_t(r10));
  // b 0x82271990
  goto loc_82271990;
loc_82271908:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24884(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24884);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82271928
  if (!cr6.eq) goto loc_82271928;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // stw r10,24884(r11)
  guest::StoreU32(uint32_t(r11) + 24884, uint32_t(r10));
  // b 0x82271990
  goto loc_82271990;
loc_82271928:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24888(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24888);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82271948
  if (!cr6.eq) goto loc_82271948;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // stw r10,24888(r11)
  guest::StoreU32(uint32_t(r11) + 24888, uint32_t(r10));
  // b 0x82271990
  goto loc_82271990;
loc_82271948:
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,31040(r9)
  r11 = guest::LoadU32(uint32_t(r9) + 31040);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82271980
  if (cr6.eq) goto loc_82271980;
  // lwz r11,24892(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 24892);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82271980
  if (cr6.eq) goto loc_82271980;
  // lwz r11,9728(r30)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r30)).unknown_9728_address());
  // li r8,30
  r8 = uint64_t(int64_t(30));
  // divwu r8,r11,r8
  r8 = uint32_t(uint32_t(r8) ? uint32_t(r11) / uint32_t(r8) : 0);
  // mulli r8,r8,30
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(30))));
  // subf. r11,r8,r11
  r11 = r11 - r8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82271994
  if (!cr0.eq) goto loc_82271994;
loc_82271980:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,6
  r3 = uint64_t(int64_t(6));
  // stw r11,31040(r9)
  guest::StoreU32(uint32_t(r9) + 31040, uint32_t(r11));
  // stw r11,24892(r10)
  guest::StoreU32(uint32_t(r10) + 24892, uint32_t(r11));
loc_82271990:
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(r3);
loc_82271994:
  // lwz r11,9728(r30)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r30)).unknown_9728_address());
  // cmplwi cr6,r11,92
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 92, so);
  // bge cr6,0x822719a8
  if (!cr6.lt) goto loc_822719A8;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,9728(r30)
  guest::StoreU32(schema::NetMachine(uint32_t(r30)).unknown_9728_address(), uint32_t(r11));
loc_822719A8:
  // lwz r11,24876(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24876);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82271aac
  if (cr6.lt) goto loc_82271AAC;
  // beq cr6,0x82271a90
  if (cr6.eq) goto loc_82271A90;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82271a78
  if (cr6.lt) goto loc_82271A78;
  // beq cr6,0x82271a68
  if (cr6.eq) goto loc_82271A68;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // blt cr6,0x82271a4c
  if (cr6.lt) goto loc_82271A4C;
  // beq cr6,0x82271a30
  if (cr6.eq) goto loc_82271A30;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bge cr6,0x82271b14
  if (!cr6.lt) goto loc_82271B14;
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lwz r11,-29856(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -29856);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // beq cr6,0x822719fc
  if (cr6.eq) goto loc_822719FC;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82271a24
  if (cr6.eq) goto loc_82271A24;
loc_822719FC:
  // lis r10,-32158
  r10 = uint64_t(int64_t(-2107506688));
  // lwz r10,-29860(r10)
  r10 = guest::LoadU32(uint32_t(r10) + -29860);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x82271b14
  if (cr6.eq) goto loc_82271B14;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82271b14
  if (!cr6.eq) goto loc_82271B14;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27300(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 27300);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82271b14
  if (!cr6.eq) goto loc_82271B14;
loc_82271A24:
  // mr r3,r30
  r3 = r30;
  // bl 0x822717d0
  // (left out: dead in C++)
  { const lifted::Result o = f822717D0(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82271a70
  goto loc_82271A70;
loc_82271A30:
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lwz r11,-29856(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -29856);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82271b14
  if (cr6.eq) goto loc_82271B14;
  // mr r3,r30
  r3 = r30;
  // bl 0x82271718
  // (left out: dead in C++)
  { const lifted::Result o = f82271718(r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82271a70
  goto loc_82271A70;
loc_82271A4C:
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lwz r11,-29856(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -29856);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82271b14
  if (cr6.eq) goto loc_82271B14;
  // mr r3,r30
  r3 = r30;
  // bl 0x82271630
  // (left out: dead in C++)
  { const lifted::Result o = f82271630(r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82271a70
  goto loc_82271A70;
loc_82271A68:
  // mr r3,r30
  r3 = r30;
  // bl 0x82271460
  // (left out: dead in C++)
  { const lifted::Result o = f82271460(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
loc_82271A70:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82271aa4
  goto loc_82271AA4;
loc_82271A78:
  // mr r3,r30
  r3 = r30;
  // bl 0x822712b8
  // (left out: dead in C++)
  { const lifted::Result o = f822712B8(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(r3);
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x82271aa4
  goto loc_82271AA4;
loc_82271A90:
  // mr r3,r30
  r3 = r30;
  // bl 0x822710c0
  // (left out: dead in C++)
  { const lifted::Result o = f822710C0(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(r3);
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_82271AA4:
  // bl 0x822606e8
  // (left out: dead in C++)
  f822606E8(r3);
  // b 0x82271b14
  goto loc_82271B14;
loc_82271AAC:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,27384(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 27384);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82271ad0
  if (!cr6.eq) goto loc_82271AD0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r11,27384(r10)
  guest::StoreU32(uint32_t(r10) + 27384, uint32_t(r11));
  // bl 0x822710c0
  // (left out: dead in C++)
  { const lifted::Result o = f822710C0(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82271b14
  goto loc_82271B14;
loc_82271AD0:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,27388(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 27388);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82271af4
  if (!cr6.eq) goto loc_82271AF4;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r11,27388(r10)
  guest::StoreU32(uint32_t(r10) + 27388, uint32_t(r11));
  // bl 0x822712b8
  // (left out: dead in C++)
  { const lifted::Result o = f822712B8(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82271b14
  goto loc_82271B14;
loc_82271AF4:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,27392(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 27392);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82271b14
  if (!cr6.eq) goto loc_82271B14;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r11,27392(r10)
  guest::StoreU32(uint32_t(r10) + 27392, uint32_t(r11));
  // bl 0x82271460
  // (left out: dead in C++)
  { const lifted::Result o = f82271460(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
loc_82271B14:
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

// sub_82271B30: lifted from generated/ by lift.py.
uint64_t f82271B30(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82271B30 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // bne cr6,0x82271b84
  if (!cr6.eq) goto loc_82271B84;
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r30,2800(r11)
  guest::StoreU32(uint32_t(r11) + 2800, uint32_t(r30));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82271B70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,63600
  r10 = r10 | 63600;
  // lwz r11,2800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 2800);
  // stwx r11,r3,r10
  guest::StoreU32(uint32_t(r3) + uint32_t(r10), uint32_t(r11));
loc_82271B84:
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r30,2816(r11)
  guest::StoreU32(uint32_t(r11) + 2816, uint32_t(r30));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r10,2804(r11)
  guest::StoreU32(uint32_t(r11) + 2804, uint32_t(r10));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r10,2808(r11)
  guest::StoreU32(uint32_t(r11) + 2808, uint32_t(r10));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r30,816(r11)
  guest::StoreU32(uint32_t(r11) + 816, uint32_t(r30));
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

// sub_82271BC0: lifted from generated/ by lift.py.
uint64_t f82271BC0(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82271BC0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // beq cr6,0x82271c24
  if (cr6.eq) goto loc_82271C24;
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bne cr6,0x82271c64
  if (!cr6.eq) goto loc_82271C64;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24960(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24960);
  // beq cr6,0x82271c14
  if (cr6.eq) goto loc_82271C14;
  // li r9,5
  r9 = uint64_t(int64_t(5));
loc_82271C14:
  // stw r9,2804(r10)
  guest::StoreU32(uint32_t(r10) + 2804, uint32_t(r9));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // stw r8,2808(r11)
  guest::StoreU32(uint32_t(r11) + 2808, uint32_t(r8));
  // b 0x82271c64
  goto loc_82271C64;
loc_82271C24:
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r9,2800(r11)
  guest::StoreU32(uint32_t(r11) + 2800, uint32_t(r9));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r10,2804(r11)
  guest::StoreU32(uint32_t(r11) + 2804, uint32_t(r10));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r10,2808(r11)
  guest::StoreU32(uint32_t(r11) + 2808, uint32_t(r10));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82271C50, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,63600
  r10 = r10 | 63600;
  // lwz r11,2800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 2800);
  // stwx r11,r3,r10
  guest::StoreU32(uint32_t(r3) + uint32_t(r10), uint32_t(r11));
loc_82271C64:
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

// sub_82271C78: lifted from generated/ by lift.py.
void f82271C78() {
  ITC_NATIVE_RUNNING("sub_82271C78 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24960(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24960);
  // beq cr6,0x82271ca0
  if (cr6.eq) goto loc_82271CA0;
  // li r9,5
  r9 = uint64_t(int64_t(5));
loc_82271CA0:
  // stw r9,2804(r10)
  guest::StoreU32(uint32_t(r10) + 2804, uint32_t(r9));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // stw r8,2808(r11)
  guest::StoreU32(uint32_t(r11) + 2808, uint32_t(r8));
  // blr
  return;
}

// sub_82271CB0: lifted from generated/ by lift.py.
void f82271CB0() {
  ITC_NATIVE_RUNNING("sub_82271CB0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r7,-32127
  r7 = uint64_t(int64_t(-2105475072));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24960(r7)
  r10 = guest::LoadU32(uint32_t(r7) + 24960);
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r9,2804(r10)
  r9 = guest::LoadU32(uint32_t(r10) + 2804);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r6,2808(r10)
  r6 = guest::LoadU32(uint32_t(r10) + 2808);
  // cmpw cr6,r9,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r8), so);
  // beq cr6,0x82271cd8
  if (cr6.eq) goto loc_82271CD8;
  // stw r8,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r8));
loc_82271CD8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // cmpwi cr6,r6,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), -1, so);
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // bne cr6,0x82271cf4
  if (!cr6.eq) goto loc_82271CF4;
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // b 0x82271d00
  goto loc_82271D00;
loc_82271CF4:
  // stw r6,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r6));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
loc_82271D00:
  // lwz r11,24960(r7)
  r11 = guest::LoadU32(uint32_t(r7) + 24960);
  // stw r10,2804(r11)
  guest::StoreU32(uint32_t(r11) + 2804, uint32_t(r10));
  // lwz r11,24960(r7)
  r11 = guest::LoadU32(uint32_t(r7) + 24960);
  // stw r10,2808(r11)
  guest::StoreU32(uint32_t(r11) + 2808, uint32_t(r10));
  // blr
  return;
}

// sub_82271D50: lifted from generated/ by lift.py.
void f82271D50(uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82271D50 (lifted)");
  uint64_t r0 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, ctr = 0;
  double f0 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  lifted::Temp temp{};
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,80(r3)
  guest::StoreU32(uint32_t(r3) + 80, uint32_t(r11));
  // lwz r10,712(r4)
  r10 = guest::LoadU32(uint32_t(r4) + 712);
  // stw r10,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r10));
  // lwz r10,716(r4)
  r10 = guest::LoadU32(uint32_t(r4) + 716);
  // stw r10,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r10));
  // lwz r10,724(r4)
  r10 = guest::LoadU32(uint32_t(r4) + 724);
  // stw r10,12(r3)
  guest::StoreU32(uint32_t(r3) + 12, uint32_t(r10));
  // lwz r10,720(r4)
  r10 = guest::LoadU32(uint32_t(r4) + 720);
  // stw r10,16(r3)
  guest::StoreU32(uint32_t(r3) + 16, uint32_t(r10));
  // lwz r10,8432(r4)
  r10 = guest::LoadU32(uint32_t(r4) + 8432);
  // stw r10,20(r3)
  guest::StoreU32(uint32_t(r3) + 20, uint32_t(r10));
  // lbz r10,769(r4)
  r10 = guest::LoadU8(uint32_t(r4) + 769);
  // stw r10,24(r3)
  guest::StoreU32(uint32_t(r3) + 24, uint32_t(r10));
  // lbz r10,770(r4)
  r10 = guest::LoadU8(uint32_t(r4) + 770);
  // stw r10,28(r3)
  guest::StoreU32(uint32_t(r3) + 28, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lbz r9,708(r4)
  r9 = guest::LoadU8(uint32_t(r4) + 708);
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6400
  r12 = uint64_t(int64_t(int64_t(r12) + 6400));
  // lbzx r0,r12,r9
  r0 = guest::LoadU8(uint32_t(r12) + uint32_t(r9));
  // lis r12,-32217
  r12 = uint64_t(int64_t(-2111373312));
  // addi r12,r12,7616
  r12 = uint64_t(int64_t(int64_t(r12) + 7616));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r9)) {
  case 0:
  goto loc_frag_82271DC0;
  return;
  case 1:
  goto loc_82271DC8;
  case 2:
  goto loc_82271DD0;
  case 3:
  goto loc_82271DD8;
  case 4:
  goto loc_frag_82271DC0;
  return;
  case 5:
  goto loc_82271DE0;
  case 6:
  goto loc_82271DE8;
  case 7:
  goto loc_82271DF0;
  case 8:
  goto loc_frag_82271DC0;
  return;
  case 9:
  goto loc_82271DC8;
  case 10:
  goto loc_82271DD0;
  case 11:
  goto loc_82271DD8;
  case 12:
  goto loc_frag_82271DC0;
  return;
  case 13:
  goto loc_82271DE0;
  case 14:
  goto loc_82271DE8;
  case 15:
  goto loc_82271DF0;
  default:
  std::abort();
  }
loc_82271DC8:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // b 0x82271df4
  goto loc_82271DF4;
loc_82271DD0:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // b 0x82271df4
  goto loc_82271DF4;
loc_82271DD8:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // b 0x82271df4
  goto loc_82271DF4;
loc_82271DE0:
  // stw r10,32(r3)
  guest::StoreU32(uint32_t(r3) + 32, uint32_t(r10));
  // b 0x82271df8
  goto loc_82271DF8;
loc_82271DE8:
  // stw r11,32(r3)
  guest::StoreU32(uint32_t(r3) + 32, uint32_t(r11));
  // b 0x82271df8
  goto loc_82271DF8;
loc_82271DF0:
  // li r9,5
  r9 = uint64_t(int64_t(5));
loc_82271DF4:
  // stw r9,32(r3)
  guest::StoreU32(uint32_t(r3) + 32, uint32_t(r9));
loc_82271DF8:
  // lbz r9,709(r4)
  r9 = guest::LoadU8(uint32_t(r4) + 709);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne 0x82271e0c
  if (!cr0.eq) goto loc_82271E0C;
  // stw r10,36(r3)
  guest::StoreU32(uint32_t(r3) + 36, uint32_t(r10));
  // b 0x82271e10
  goto loc_82271E10;
loc_82271E0C:
  // stw r11,36(r3)
  guest::StoreU32(uint32_t(r3) + 36, uint32_t(r11));
loc_82271E10:
  // lwz r11,648(r4)
  r11 = guest::LoadU32(uint32_t(r4) + 648);
  // addi r10,r3,44
  r10 = uint64_t(int64_t(int64_t(r3) + 44));
  // addi r9,r3,48
  r9 = uint64_t(int64_t(int64_t(r3) + 48));
  // addi r8,r3,56
  r8 = uint64_t(int64_t(int64_t(r3) + 56));
  // stw r11,40(r3)
  guest::StoreU32(uint32_t(r3) + 40, uint32_t(r11));
  // lfs f0,652(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r4) + 652);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r10
  guest::StoreU32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lfs f0,656(r4)
  temp.u32 = guest::LoadU32(uint32_t(r4) + 656);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r9
  guest::StoreU32(uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lfs f0,660(r4)
  temp.u32 = guest::LoadU32(uint32_t(r4) + 660);
  f0 = double(temp.f32);
  // stfs f0,52(r3)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r3) + 52, temp.u32);
  // lfs f0,664(r4)
  temp.u32 = guest::LoadU32(uint32_t(r4) + 664);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r8
  guest::StoreU32(uint32_t(r8), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_frag_82271DC0:
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // b 0x82271df4
  goto loc_82271DF4_82271DC0;
loc_82271DF4_82271DC0:
  // stw r9,32(r3)
  guest::StoreU32(uint32_t(r3) + 32, uint32_t(r9));
  // lbz r9,709(r4)
  r9 = guest::LoadU8(uint32_t(r4) + 709);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne 0x82271e0c
  if (!cr0.eq) goto loc_82271E0C_82271DC0;
  // stw r10,36(r3)
  guest::StoreU32(uint32_t(r3) + 36, uint32_t(r10));
  // b 0x82271e10
  goto loc_82271E10_82271DC0;
loc_82271E0C_82271DC0:
  // stw r11,36(r3)
  guest::StoreU32(uint32_t(r3) + 36, uint32_t(r11));
loc_82271E10_82271DC0:
  // lwz r11,648(r4)
  r11 = guest::LoadU32(uint32_t(r4) + 648);
  // addi r10,r3,44
  r10 = uint64_t(int64_t(int64_t(r3) + 44));
  // addi r9,r3,48
  r9 = uint64_t(int64_t(int64_t(r3) + 48));
  // addi r8,r3,56
  r8 = uint64_t(int64_t(int64_t(r3) + 56));
  // stw r11,40(r3)
  guest::StoreU32(uint32_t(r3) + 40, uint32_t(r11));
  // lfs f0,652(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r4) + 652);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r10
  guest::StoreU32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lfs f0,656(r4)
  temp.u32 = guest::LoadU32(uint32_t(r4) + 656);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r9
  guest::StoreU32(uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lfs f0,660(r4)
  temp.u32 = guest::LoadU32(uint32_t(r4) + 660);
  f0 = double(temp.f32);
  // stfs f0,52(r3)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r3) + 52, temp.u32);
  // lfs f0,664(r4)
  temp.u32 = guest::LoadU32(uint32_t(r4) + 664);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r8
  guest::StoreU32(uint32_t(r8), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
}

// sub_82271E58: lifted from generated/ by lift.py.
lifted::Result f82271E58(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82271E58 (lifted)");
  uint64_t r0 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(uint32_t(r1) + -4096);
  // stwu r1,-6944(r1)
  ea = -6944 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,1152
  r3 = uint64_t(int64_t(int64_t(r1) + 1152));
  // mr r30,r4
  r30 = r4;
  // bl 0x820aef80
  // (left out: dead in C++)
  { const lifted::Result o = game::CarData::Ctor(LIFTED_ARGS(0x82271E78, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3008
  r3 = uint64_t(int64_t(int64_t(r1) + 3008));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(LIFTED_ARGS(0x82271E80, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3008
  r3 = uint64_t(int64_t(int64_t(r1) + 3008));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // mr r4,r30
  r4 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82271E90, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3008
  r3 = uint64_t(int64_t(int64_t(r1) + 3008));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(LIFTED_ARGS(0x82271E98, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3008
  r3 = uint64_t(int64_t(int64_t(r1) + 3008));
  // bl 0x820c4550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820C4550(LIFTED_ARGS(0x82271EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r4,r1,3008
  r4 = uint64_t(int64_t(int64_t(r1) + 3008));
  // addi r3,r1,1152
  r3 = uint64_t(int64_t(int64_t(r1) + 1152));
  // bl 0x820c27e8
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::SetupFromCar(LIFTED_ARGS(0x82271EB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // addi r10,r30,1568
  r10 = uint64_t(int64_t(int64_t(r30) + 1568));
  // addi r9,r31,60
  r9 = uint64_t(int64_t(int64_t(r31) + 60));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r8,80(r31)
  guest::StoreU32(uint32_t(r31) + 80, uint32_t(r8));
  // mtctr r11
  ctr = r11;
loc_82271EC8:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r11,0(r9)
  guest::StoreU32(uint32_t(r9) + 0, uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x82271ec8
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82271EC8;
  // lfs f0,1824(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1824);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,1552(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1552);
  f13 = double(temp.f32);
  // fadds f13,f13,f0
  f13 = double(float(f13 + f0));
  // lfs f12,1892(r30)
  temp.u32 = guest::LoadU32(uint32_t(r30) + 1892);
  f12 = double(temp.f32);
  // fctiwz f12,f12
  f12 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f12) ? int64_t(0x80000000U) : (f12 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f12)))));
  // stfiwx f12,0,r31
  guest::StoreU32(uint32_t(r31), uint32_t(std::bit_cast<uint64_t>(f12)));
  // lfs f12,1152(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1152);
  f12 = double(temp.f32);
  // lfs f0,8680(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // addi r11,r31,4
  r11 = uint64_t(int64_t(int64_t(r31) + 4));
  // fmadds f0,f13,f0,f12
  f0 = double(float(std::fma(f13, f0, f12)));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  guest::StoreU32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,76(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 76);
  // stw r11,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r11));
  // lwz r11,84(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 84);
  // stw r11,12(r31)
  guest::StoreU32(uint32_t(r31) + 12, uint32_t(r11));
  // lwz r11,80(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 80);
  // stw r11,16(r31)
  guest::StoreU32(uint32_t(r31) + 16, uint32_t(r11));
  // lwz r11,12(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 12);
  // stw r11,20(r31)
  guest::StoreU32(uint32_t(r31) + 20, uint32_t(r11));
  // lbz r11,129(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 129);
  // stw r11,24(r31)
  guest::StoreU32(uint32_t(r31) + 24, uint32_t(r11));
  // lbz r11,130(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 130);
  // stw r11,28(r31)
  guest::StoreU32(uint32_t(r31) + 28, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lbz r10,68(r29)
  r10 = guest::LoadU8(uint32_t(r29) + 68);
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,6408
  r12 = uint64_t(int64_t(int64_t(r12) + 6408));
  // lbzx r0,r12,r10
  r0 = guest::LoadU8(uint32_t(r12) + uint32_t(r10));
  // lis r12,-32217
  r12 = uint64_t(int64_t(-2111373312));
  // addi r12,r12,8048
  r12 = uint64_t(int64_t(int64_t(r12) + 8048));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r10)) {
  case 0:
  goto loc_frag_82271F70;
  return {r3, f1};
  case 1:
  goto loc_82271F78;
  case 2:
  goto loc_82271F80;
  case 3:
  goto loc_82271F88;
  case 4:
  goto loc_frag_82271F70;
  return {r3, f1};
  case 5:
  goto loc_82271F90;
  case 6:
  goto loc_82271F98;
  case 7:
  goto loc_82271FA0;
  default:
  std::abort();
  }
loc_82271F78:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x82271fa4
  goto loc_82271FA4;
loc_82271F80:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // b 0x82271fa4
  goto loc_82271FA4;
loc_82271F88:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // b 0x82271fa4
  goto loc_82271FA4;
loc_82271F90:
  // stw r11,32(r31)
  guest::StoreU32(uint32_t(r31) + 32, uint32_t(r11));
  // b 0x82271fa8
  goto loc_82271FA8;
loc_82271F98:
  // stw r8,32(r31)
  guest::StoreU32(uint32_t(r31) + 32, uint32_t(r8));
  // b 0x82271fa8
  goto loc_82271FA8;
loc_82271FA0:
  // li r10,5
  r10 = uint64_t(int64_t(5));
loc_82271FA4:
  // stw r10,32(r31)
  guest::StoreU32(uint32_t(r31) + 32, uint32_t(r10));
loc_82271FA8:
  // lbz r10,69(r29)
  r10 = guest::LoadU8(uint32_t(r29) + 69);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x82271fbc
  if (!cr0.eq) goto loc_82271FBC;
  // stw r11,36(r31)
  guest::StoreU32(uint32_t(r31) + 36, uint32_t(r11));
  // b 0x82271fc0
  goto loc_82271FC0;
loc_82271FBC:
  // stw r8,36(r31)
  guest::StoreU32(uint32_t(r31) + 36, uint32_t(r8));
loc_82271FC0:
  // lwz r11,8(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8);
  // addi r4,r1,2420
  r4 = uint64_t(int64_t(int64_t(r1) + 2420));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,40(r31)
  guest::StoreU32(uint32_t(r31) + 40, uint32_t(r11));
  // bl 0x820ce7d8
  // (left out: dead in C++)
  { const lifted::Result o = game::PowerCurve::FromEngine(LIFTED_ARGS(0x82271FD4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820ce1d8
  // (left out: dead in C++)
  { const lifted::Result o = game::PowerCurve::ShiftPeak(LIFTED_ARGS(0x82271FE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,1136(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1136);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fctiwz f13,f0
  f13 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // lfs f0,1140(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1140);
  f0 = double(temp.f32);
  // addi r10,r31,44
  r10 = uint64_t(int64_t(int64_t(r31) + 44));
  // fctiwz f12,f0
  f12 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // lfs f11,1128(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1128);
  f11 = double(temp.f32);
  // addi r9,r31,48
  r9 = uint64_t(int64_t(int64_t(r31) + 48));
  // addi r8,r31,56
  r8 = uint64_t(int64_t(int64_t(r31) + 56));
  // lfs f0,-24896(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + -24896);
  f0 = double(temp.f32);
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // stfs f0,52(r31)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r31) + 52, temp.u32);
  // lfs f0,1132(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1132);
  f0 = double(temp.f32);
  // stfiwx f13,0,r10
  guest::StoreU32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f13)));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f12,0,r9
  guest::StoreU32(uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f12)));
  // stfiwx f0,0,r8
  guest::StoreU32(uint32_t(r8), uint32_t(std::bit_cast<uint64_t>(f0)));
  // addi r1,r1,6944
  r1 = uint64_t(int64_t(int64_t(r1) + 6944));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_frag_82271F70:
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // b 0x82271fa4
  goto loc_82271FA4_82271F70;
loc_82271FA4_82271F70:
  // stw r10,32(r31)
  guest::StoreU32(uint32_t(r31) + 32, uint32_t(r10));
  // lbz r10,69(r29)
  r10 = guest::LoadU8(uint32_t(r29) + 69);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x82271fbc
  if (!cr0.eq) goto loc_82271FBC_82271F70;
  // stw r11,36(r31)
  guest::StoreU32(uint32_t(r31) + 36, uint32_t(r11));
  // b 0x82271fc0
  goto loc_82271FC0_82271F70;
loc_82271FBC_82271F70:
  // stw r8,36(r31)
  guest::StoreU32(uint32_t(r31) + 36, uint32_t(r8));
loc_82271FC0_82271F70:
  // lwz r11,8(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8);
  // addi r4,r1,2420
  r4 = uint64_t(int64_t(int64_t(r1) + 2420));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,40(r31)
  guest::StoreU32(uint32_t(r31) + 40, uint32_t(r11));
  // bl 0x820ce7d8
  // (left out: dead in C++)
  { const lifted::Result o = game::PowerCurve::FromEngine(LIFTED_ARGS(0x82271FD4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820ce1d8
  // (left out: dead in C++)
  { const lifted::Result o = game::PowerCurve::ShiftPeak(LIFTED_ARGS(0x82271FE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,1136(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1136);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fctiwz f13,f0
  f13 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // lfs f0,1140(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1140);
  f0 = double(temp.f32);
  // addi r10,r31,44
  r10 = uint64_t(int64_t(int64_t(r31) + 44));
  // fctiwz f12,f0
  f12 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // lfs f11,1128(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1128);
  f11 = double(temp.f32);
  // addi r9,r31,48
  r9 = uint64_t(int64_t(int64_t(r31) + 48));
  // addi r8,r31,56
  r8 = uint64_t(int64_t(int64_t(r31) + 56));
  // lfs f0,-24896(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + -24896);
  f0 = double(temp.f32);
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // stfs f0,52(r31)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r31) + 52, temp.u32);
  // lfs f0,1132(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1132);
  f0 = double(temp.f32);
  // stfiwx f13,0,r10
  guest::StoreU32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f13)));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f12,0,r9
  guest::StoreU32(uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f12)));
  // stfiwx f0,0,r8
  guest::StoreU32(uint32_t(r8), uint32_t(std::bit_cast<uint64_t>(f0)));
  // addi r1,r1,6944
  r1 = uint64_t(int64_t(int64_t(r1) + 6944));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82272460: lifted from generated/ by lift.py.
void f82272460(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82272460 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // stw r10,804(r11)
  guest::StoreU32(uint32_t(r11) + 804, uint32_t(r10));
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82272488
  if (cr6.eq) goto loc_82272488;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,9604(r3)
  guest::StoreU32(uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
loc_82272488:
  // stw r10,9604(r3)
  guest::StoreU32(uint32_t(r3) + 9604, uint32_t(r10));
  // blr
  return;
}

// sub_82272490: lifted from generated/ by lift.py.
void f82272490(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82272490 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // stw r10,804(r11)
  guest::StoreU32(uint32_t(r11) + 804, uint32_t(r10));
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // bne cr6,0x822724b4
  if (!cr6.eq) goto loc_822724B4;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_822724B4:
  // stw r11,9604(r3)
  guest::StoreU32(uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_822724C0: lifted from generated/ by lift.py.
lifted::Result f822724C0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_822724C0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-2000(r1)
  ea = -2000 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // bl 0x820cd7f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Clear(LIFTED_ARGS(0x822724E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,8432(r30)
  r4 = guest::LoadU32(uint32_t(r30) + 8432);
  // bl 0x820cd8a8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::SetModel(LIFTED_ARGS(0x822724F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r5,r31
  r5 = r31;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31776(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x82179a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82179A60, LIFTED_ARGS(0x82272508, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,2000
  r1 = uint64_t(int64_t(int64_t(r1) + 2000));
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

// sub_82272520: lifted from generated/ by lift.py.
uint64_t f82272520(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82272520 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82272548, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82272560, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82272574, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82272588, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x8227259C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x822725AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x822725BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822725CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822725DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x822725f4
  if (cr6.eq) goto loc_822725F4;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_822725F4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
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

// sub_82272620: lifted from generated/ by lift.py.
lifted::Result f82272620(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82272620 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // addi r31,r11,23756
  r31 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82272654, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82272664
  if (cr0.eq) goto loc_82272664;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(LIFTED_ARGS(0x82272664, r31)); r3 = o.r3; f1 = o.f1; }
loc_82272664:
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82272678, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82272688
  if (cr0.eq) goto loc_82272688;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(LIFTED_ARGS(0x82272688, r31)); r3 = o.r3; f1 = o.f1; }
loc_82272688:
  // lwz r11,9616(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 9616);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822726ac
  if (cr6.eq) goto loc_822726AC;
  // li r3,33
  r3 = uint64_t(int64_t(33));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(LIFTED_ARGS(0x8227269C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,34
  r3 = uint64_t(int64_t(34));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Delete(LIFTED_ARGS(0x822726A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9608(r30)
  guest::StoreU32(uint32_t(r30) + 9608, uint32_t(r11));
loc_822726AC:
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

// sub_822726C8: lifted from generated/ by lift.py.
uint64_t f822726C8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822726C8 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r28,r3
  r28 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r30,24960(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r11,804(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 804);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82272768
  if (cr6.lt) goto loc_82272768;
  // beq cr6,0x82272704
  if (cr6.eq) goto loc_82272704;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x822727cc
  if (!cr6.lt) goto loc_822727CC;
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // b 0x82272784
  goto loc_82272784;
loc_82272704:
  // lwz r11,2816(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 2816);
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82272754
  if (cr6.eq) goto loc_82272754;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r4,r30,832
  r4 = uint64_t(int64_t(int64_t(r30) + 832));
  // li r5,1968
  r5 = uint64_t(int64_t(1968));
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r10,r10,-2
  r10 = uint64_t(int64_t(int64_t(r10) + -2));
  // addi r3,r11,26248
  r3 = uint64_t(int64_t(int64_t(r11) + 26248));
  // cntlzw r9,r10
  r9 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // rlwinm r11,r9,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // stw r11,28224(r10)
  guest::StoreU32(uint32_t(r10) + 28224, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82272748, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,844(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 844);
  // lwz r11,28220(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 28220);
  // stw r10,216(r11)
  guest::StoreU32(schema::Session(uint32_t(r11)).ta_car_word_address(), uint32_t(r10));
loc_82272754:
  // lwz r10,28220(r29)
  r10 = guest::LoadU32(uint32_t(r29) + 28220);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stw r6,160(r10)
  guest::StoreU32(schema::Session(uint32_t(r10)).unknown_160_address(), uint32_t(r6));
  // b 0x82272784
  goto loc_82272784;
loc_82272768:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 27304);
  // addi r11,r11,-4096
  r11 = uint64_t(int64_t(int64_t(r11) + -4096));
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // ble cr6,0x82272784
  if (!cr6.gt) goto loc_82272784;
  // li r9,12
  r9 = uint64_t(int64_t(12));
loc_82272784:
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(uint32_t(r11) + 600, uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r7));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
loc_822727CC:
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r11,9604(r28)
  guest::StoreU32(uint32_t(r28) + 9604, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82272968: lifted from generated/ by lift.py.
lifted::Result f82272968(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82272968 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // lwz r3,24944(r28)
  r3 = guest::LoadU32(uint32_t(r28) + 24944);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82272980, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24944(r28)
  r3 = guest::LoadU32(uint32_t(r28) + 24944);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x8227298C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r26,-1
  r26 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // beq 0x82272a88
  if (cr0.eq) goto loc_82272A88;
  // lwz r11,24944(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24944);
  // lwz r29,0(r11)
  r29 = guest::LoadU32(uint32_t(r11) + 0);
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x822729cc
  if (cr6.lt) goto loc_822729CC;
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // beq cr6,0x822729c4
  if (cr6.eq) goto loc_822729C4;
  // lwz r11,828(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 828);
  // b 0x822729d0
  goto loc_822729D0;
loc_822729C4:
  // lwz r11,824(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 824);
  // b 0x822729d0
  goto loc_822729D0;
loc_822729CC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_822729D0:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82272a7c
  if (cr6.eq) goto loc_82272A7C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x822729E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x82272a00
  if (cr6.lt) goto loc_82272A00;
  // beq cr6,0x822729f8
  if (cr6.eq) goto loc_822729F8;
  // addi r10,r11,540
  r10 = uint64_t(int64_t(int64_t(r11) + 540));
  // b 0x82272a04
  goto loc_82272A04;
loc_822729F8:
  // addi r10,r11,280
  r10 = uint64_t(int64_t(int64_t(r11) + 280));
  // b 0x82272a04
  goto loc_82272A04;
loc_82272A00:
  // addi r10,r11,20
  r10 = uint64_t(int64_t(int64_t(r11) + 20));
loc_82272A04:
  // stw r10,800(r11)
  guest::StoreU32(uint32_t(r11) + 800, uint32_t(r10));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24960(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 24960);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lwz r11,24948(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24948);
  // lwz r10,800(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 800);
  // lwz r10,256(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 256);
  // stw r31,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r31));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r9,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r9));
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
  // stw r31,36(r11)
  guest::StoreU32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  guest::StoreU32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,32(r11)
  guest::StoreU32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,24940(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x82272a70
  if (cr6.eq) goto loc_82272A70;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82272A70:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // b 0x82272ac0
  goto loc_82272AC0;
loc_82272A7C:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82272A84, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82272b74
  goto loc_82272B74;
loc_82272A88:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24944(r28)
  r3 = guest::LoadU32(uint32_t(r28) + 24944);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82272A94, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82272ad0
  if (cr0.eq) goto loc_82272AD0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82272AA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24940(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x82272ab8
  if (cr6.eq) goto loc_82272AB8;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82272AB8:
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // li r9,8
  r9 = uint64_t(int64_t(8));
loc_82272AC0:
  // stw r26,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r26));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r26,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r26));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
loc_82272AD0:
  // lwz r11,24944(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24944);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82272b10
  if (cr6.eq) goto loc_82272B10;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82272AE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24940(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82272afc
  if (cr0.eq) goto loc_82272AFC;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82272AFC:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r26,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r26));
  // stw r31,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r26,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r26));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
loc_82272B10:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82272B28, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r10,824(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 824);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82272b54
  if (!cr6.eq) goto loc_82272B54;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x82272B50, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
loc_82272B54:
  // lwz r11,828(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 828);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82272b74
  if (!cr6.eq) goto loc_82272B74;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x82272B74, r31)); r3 = o.r3; f1 = o.f1; }
loc_82272B74:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82272B80: lifted from generated/ by lift.py.
uint64_t f82272B80(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82272B80 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r3
  r26 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82272B98, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // mr r29,r3
  r29 = r3;
  // lwz r11,24948(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24948);
  // lwz r30,0(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // addi r3,r11,832
  r3 = uint64_t(int64_t(int64_t(r11) + 832));
  // lwz r10,800(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 800);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24956(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24956);
  // lwz r28,0(r11)
  r28 = guest::LoadU32(uint32_t(r11) + 0);
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // lwz r27,8432(r11)
  r27 = guest::LoadU32(uint32_t(r11) + 8432);
  // bl 0x82110448
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageCar::Reset(LIFTED_ARGS(0x82272BD4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // blt cr6,0x82272c4c
  if (cr6.lt) goto loc_82272C4C;
  // addi r11,r29,6120
  r11 = uint64_t(int64_t(int64_t(r29) + 6120));
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // mr r9,r30
  r9 = r30;
  // ori r10,r10,39200
  r10 = r10 | 39200;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // stwx r30,r29,r10
  guest::StoreU32(uint32_t(r29) + uint32_t(r10), uint32_t(r30));
  // lha r10,204(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r11) + 204))));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // beq cr6,0x82272c24
  if (cr6.eq) goto loc_82272C24;
loc_82272C10:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r10) + 0))));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // bne 0x82272c10
  if (!cr0.eq) goto loc_82272C10;
loc_82272C24:
  // lha r9,2(r10)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r10) + 2))));
  // li r5,1968
  r5 = uint64_t(int64_t(1968));
  // lwz r10,24960(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24960);
  // addi r3,r10,832
  r3 = uint64_t(int64_t(int64_t(r10) + 832));
  // mr r10,r9
  r10 = r9;
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r4,r11,212
  r4 = uint64_t(int64_t(int64_t(r11) + 212));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82272C48, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82272c84
  goto loc_82272C84;
loc_82272C4C:
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // mr r4,r27
  r4 = r27;
  // addi r3,r11,832
  r3 = uint64_t(int64_t(int64_t(r11) + 832));
  // bl 0x821104d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::SetModelAndClass(LIFTED_ARGS(0x82272C5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // extsh r10,r28
  r10 = uint64_t(int64_t(int16_t(r28)));
  // addi r11,r11,832
  r11 = uint64_t(int64_t(int64_t(r11) + 832));
  // lhz r9,4(r11)
  r9 = guest::LoadU16(uint32_t(r11) + 4);
  // cmplwi cr6,r9,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 65535, so);
  // bne cr6,0x82272c78
  if (!cr6.eq) goto loc_82272C78;
  // sth r10,4(r11)
  guest::StoreU16(uint32_t(r11) + 4, uint16_t(r10));
loc_82272C78:
  // lhz r9,0(r11)
  r9 = guest::LoadU16(uint32_t(r11) + 0);
  // sth r10,0(r11)
  guest::StoreU16(uint32_t(r11) + 0, uint16_t(r10));
  // sth r9,2(r11)
  guest::StoreU16(uint32_t(r11) + 2, uint16_t(r9));
loc_82272C84:
  // lwz r10,24960(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24960);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r11,r26,204
  r11 = uint64_t(int64_t(int64_t(r26) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,21
  r9 = uint64_t(int64_t(21));
  // mr r7,r8
  r7 = r8;
  // stw r8,8(r10)
  guest::StoreU32(uint32_t(r10) + 8, uint32_t(r8));
  // lwz r10,24960(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r8,12(r10)
  guest::StoreU32(uint32_t(r10) + 12, uint32_t(r8));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(uint32_t(r11) + 600, uint32_t(r10));
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
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82272CE8: lifted from generated/ by lift.py.
void f82272CE8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82272CE8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24960(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r9,8(r10)
  r9 = guest::LoadU32(uint32_t(r10) + 8);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x82272d18
  if (cr6.eq) goto loc_82272D18;
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r9,804(r10)
  guest::StoreU32(uint32_t(r10) + 804, uint32_t(r9));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r10,-21696
  r10 = uint64_t(int64_t(int64_t(r10) + -21696));
  // stw r9,16(r10)
  guest::StoreU32(uint32_t(r10) + 16, uint32_t(r9));
  // lwz r10,24960(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24960);
loc_82272D18:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,21
  r9 = uint64_t(int64_t(21));
  // mr r8,r7
  r8 = r7;
  // stw r7,12(r10)
  guest::StoreU32(uint32_t(r10) + 12, uint32_t(r7));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(uint32_t(r11) + 600, uint32_t(r10));
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

// sub_82272D70: lifted from generated/ by lift.py.
lifted::Result f82272D70(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82272D70 (lifted)");
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
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r3,24948(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24948);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82272D88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24948(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24948);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82272D94, r31)); r3 = o.r3; f1 = o.f1; }
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // beq 0x82272dd8
  if (cr0.eq) goto loc_82272DD8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82272DAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24940(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82272dc0
  if (cr6.eq) goto loc_82272DC0;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82272DC0:
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stw r9,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // b 0x82272e18
  goto loc_82272E18;
loc_82272DD8:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24948(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24948);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82272DE4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82272e20
  if (cr0.eq) goto loc_82272E20;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82272DF4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24940(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82272e08
  if (cr0.eq) goto loc_82272E08;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82272E08:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
loc_82272E18:
  // stw r29,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
loc_82272E20:
  // lwz r11,24948(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24948);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82272e64
  if (cr6.eq) goto loc_82272E64;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82272E38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24940(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x82272e4c
  if (cr6.eq) goto loc_82272E4C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82272E4C:
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // stw r29,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
loc_82272E64:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82272E7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lwz r11,808(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 808);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82272ea8
  if (!cr6.eq) goto loc_82272EA8;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82272EA4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82272eac
  goto loc_82272EAC;
loc_82272EA8:
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82272EAC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82272EAC:
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // lwz r10,800(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r9,808(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 808);
  // lwz r11,256(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 256);
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // bne cr6,0x82272edc
  if (!cr6.eq) goto loc_82272EDC;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82272ED8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82272ee0
  goto loc_82272EE0;
loc_82272EDC:
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82272EE0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82272EE0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82272EE8: lifted from generated/ by lift.py.
lifted::Result f82272EE8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82272EE8 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r31,r3
  r31 = r3;
  // lwz r3,24956(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 24956);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82272F04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24956(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 24956);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82272F10, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // beq 0x82272fec
  if (cr0.eq) goto loc_82272FEC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82272F28, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r11,804(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 804);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82272f64
  if (!cr0.eq) goto loc_82272F64;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,24996(r10)
  guest::StoreU32(uint32_t(r10) + 24996, uint32_t(r11));
  // lwz r11,24940(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x82272f5c
  if (cr6.eq) goto loc_82272F5C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82272F5C:
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // b 0x82272f90
  goto loc_82272F90;
loc_82272F64:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82272f9c
  if (!cr6.eq) goto loc_82272F9C;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,24996(r10)
  guest::StoreU32(uint32_t(r10) + 24996, uint32_t(r11));
  // lwz r11,24940(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x82272f8c
  if (cr6.eq) goto loc_82272F8C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82272F8C:
  // li r10,10
  r10 = uint64_t(int64_t(10));
loc_82272F90:
  // stw r28,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r28));
  // stw r28,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r28));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
loc_82272F9C:
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(uint32_t(r11) + 600, uint32_t(r10));
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
  // b 0x82273034
  goto loc_82273034;
loc_82272FEC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24956(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 24956);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82272FF8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82273034
  if (cr0.eq) goto loc_82273034;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82273008, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24940(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x8227301c
  if (cr6.eq) goto loc_8227301C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_8227301C:
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // stw r28,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r28));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r28,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
loc_82273034:
  // lwz r11,24956(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24956);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82273078
  if (cr6.eq) goto loc_82273078;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8227304C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24940(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x82273060
  if (cr6.eq) goto loc_82273060;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82273060:
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // stw r28,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r28));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // stw r28,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r28));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
loc_82273078:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82273080: lifted from generated/ by lift.py.
uint64_t f82273080(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82273080 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822730A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822730C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822730D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822730E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822730F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82273104, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82273114, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82273128: lifted from generated/ by lift.py.
lifted::Result f82273128(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82273128 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8227314C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r30,5
  r30 = uint64_t(int64_t(5));
  // li r29,3
  r29 = uint64_t(int64_t(3));
loc_82273158:
  // addi r6,r30,119
  r6 = uint64_t(int64_t(int64_t(r30) + 119));
  // mr r5,r30
  r5 = r30;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8227316C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x82273158
  if (!cr0.eq) goto loc_82273158;
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x8227318C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822731A0, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822731B4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822731C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822731D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822731E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822731FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82273210, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r6,804(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 804);
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x8227322C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24944(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r6,r11,930
  r6 = uint64_t(int64_t(int64_t(r11) + 930));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8227324C, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x82273260, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x82273270, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,24960(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 24960);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // stw r9,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,24960(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r9,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r9));
  // stw r11,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r11));
  // stw r11,4(r10)
  guest::StoreU32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r10,24960(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r8,2804(r10)
  guest::StoreU32(uint32_t(r10) + 2804, uint32_t(r8));
  // stw r8,2808(r10)
  guest::StoreU32(uint32_t(r10) + 2808, uint32_t(r8));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // stw r11,12(r10)
  guest::StoreU32(uint32_t(r10) + 12, uint32_t(r11));
  // stw r9,2800(r10)
  guest::StoreU32(uint32_t(r10) + 2800, uint32_t(r9));
  // stw r11,808(r10)
  guest::StoreU32(uint32_t(r10) + 808, uint32_t(r11));
  // stw r11,812(r10)
  guest::StoreU32(uint32_t(r10) + 812, uint32_t(r11));
  // stw r11,816(r10)
  guest::StoreU32(uint32_t(r10) + 816, uint32_t(r11));
  // stw r11,2812(r10)
  guest::StoreU32(uint32_t(r10) + 2812, uint32_t(r11));
  // stw r11,88(r1)
  guest::StoreU32(uint32_t(r1) + 88, uint32_t(r11));
  // stw r11,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r11));
  // stw r11,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r11));
  // stw r11,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r11));
  // stw r11,2816(r10)
  guest::StoreU32(uint32_t(r10) + 2816, uint32_t(r11));
  // stw r11,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r11));
  // lwz r10,24944(r29)
  r10 = guest::LoadU32(uint32_t(r29) + 24944);
  // stw r11,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r11));
  // stw r11,112(r1)
  guest::StoreU32(uint32_t(r1) + 112, uint32_t(r11));
  // stw r11,116(r1)
  guest::StoreU32(uint32_t(r1) + 116, uint32_t(r11));
  // stw r11,120(r1)
  guest::StoreU32(uint32_t(r1) + 120, uint32_t(r11));
  // stw r11,12(r10)
  guest::StoreU32(uint32_t(r10) + 12, uint32_t(r11));
  // rotlwi r11,r9,0
  r11 = __builtin_rotateleft32(uint32_t(r9), 0);
  // stw r8,16(r10)
  guest::StoreU32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r11,20(r10)
  guest::StoreU32(uint32_t(r10) + 20, uint32_t(r11));
  // lwz r11,84(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 84);
  // stw r11,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r11));
  // lwz r11,88(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 88);
  // stw r11,4(r10)
  guest::StoreU32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r11,92(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 92);
  // stw r11,8(r10)
  guest::StoreU32(uint32_t(r10) + 8, uint32_t(r11));
  // lwz r11,96(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 96);
  // stw r11,40(r10)
  guest::StoreU32(uint32_t(r10) + 40, uint32_t(r11));
  // lwz r11,100(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 100);
  // stw r11,48(r10)
  guest::StoreU32(uint32_t(r10) + 48, uint32_t(r11));
  // lwz r11,104(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 104);
  // stw r11,52(r10)
  guest::StoreU32(uint32_t(r10) + 52, uint32_t(r11));
  // lwz r11,108(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 108);
  // stw r11,36(r10)
  guest::StoreU32(uint32_t(r10) + 36, uint32_t(r11));
  // lwz r11,112(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 112);
  // stw r11,64(r10)
  guest::StoreU32(uint32_t(r10) + 64, uint32_t(r11));
  // lwz r11,116(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 116);
  // stw r11,24(r10)
  guest::StoreU32(uint32_t(r10) + 24, uint32_t(r11));
  // lwz r11,120(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 120);
  // stw r11,28(r10)
  guest::StoreU32(uint32_t(r10) + 28, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,32(r10)
  guest::StoreU32(uint32_t(r10) + 32, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24948(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 24948);
  // stw r11,12(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_first_item_bits(uint32_t(r11));
  // stw r8,16(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_last_item_bits(uint32_t(r8));
  // stw r9,20(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_wraps_bits(uint32_t(r9));
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
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24952(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 24952);
  // stw r11,12(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_first_item_bits(uint32_t(r11));
  // stw r9,16(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_last_item_bits(uint32_t(r9));
  // stw r9,20(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_wraps_bits(uint32_t(r9));
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
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,24956(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 24956);
  // stw r11,12(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_first_item_bits(uint32_t(r11));
  // stw r9,16(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_last_item_bits(uint32_t(r9));
  // stw r9,20(r10)
  schema::FrontEndMenu(uint32_t(r10)).set_wraps_bits(uint32_t(r9));
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
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82273418: lifted from generated/ by lift.py.
lifted::Result f82273418(uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82273418 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8227343C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // lwz r28,0(r3)
  r28 = guest::LoadU32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,24960(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24960);
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r26,256(r11)
  r26 = guest::LoadU32(uint32_t(r11) + 256);
loc_82273454:
  // cmpw cr6,r31,r26
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r26), so);
  // addi r5,r31,5
  r5 = uint64_t(int64_t(int64_t(r31) + 5));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r28
  r3 = r28;
  // bge cr6,0x82273470
  if (!cr6.lt) goto loc_82273470;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x8227346C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82273474
  goto loc_82273474;
loc_82273470:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82273474, r31)); r3 = o.r3; f1 = o.f1; }
loc_82273474:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,4
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 4, so);
  // blt cr6,0x82273454
  if (cr6.lt) goto loc_82273454;
  // cmpwi cr6,r26,4
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 4, so);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r28
  r3 = r28;
  // ble cr6,0x8227349c
  if (!cr6.gt) goto loc_8227349C;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82273498, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822734a0
  goto loc_822734A0;
loc_8227349C:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822734A0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822734A0:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_822734A4:
  // cmpw cr6,r31,r26
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r26), so);
  // bge cr6,0x822735a0
  if (!cr6.lt) goto loc_822735A0;
  // lwz r11,24960(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24960);
  // addi r5,r31,5
  r5 = uint64_t(int64_t(int64_t(r31) + 5));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r28
  r3 = r28;
  // lwz r11,808(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 808);
  // add r29,r11,r31
  r29 = r11 + r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x822734C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24960);
  // rlwinm r10,r29,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r30,r3
  r30 = r3;
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwzx r29,r11,r10
  r29 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x822734E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8432(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8432);
  // mr r10,r3
  r10 = r3;
  // cmpwi cr6,r11,35
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 35, so);
  // bge cr6,0x82273504
  if (!cr6.lt) goto loc_82273504;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82273530
  if (!cr6.eq) goto loc_82273530;
loc_822734FC:
  // li r11,35
  r11 = uint64_t(int64_t(35));
  // b 0x82273530
  goto loc_82273530;
loc_82273504:
  // cmpwi cr6,r11,43
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 43, so);
  // beq cr6,0x822734fc
  if (cr6.eq) goto loc_822734FC;
  // addi r9,r11,-47
  r9 = uint64_t(int64_t(int64_t(r11) + -47));
  // cmplwi cr6,r9,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 9, so);
  // bgt cr6,0x82273520
  if (cr6.gt) goto loc_82273520;
  // addi r11,r11,-11
  r11 = uint64_t(int64_t(int64_t(r11) + -11));
  // b 0x82273530
  goto loc_82273530;
loc_82273520:
  // cmpwi cr6,r11,71
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 71, so);
  // li r11,44
  r11 = uint64_t(int64_t(44));
  // beq cr6,0x82273530
  if (cr6.eq) goto loc_82273530;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
loc_82273530:
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // stw r11,108(r10)
  guest::StoreU32(uint32_t(r10) + 108, uint32_t(r11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273540, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,769(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 769);
  // mr r4,r3
  r4 = r3;
  // mr r3,r28
  r3 = r28;
  // addi r5,r11,1934
  r5 = uint64_t(int64_t(int64_t(r11) + 1934));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82273554, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273560, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8432(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8432);
  // mr r4,r3
  r4 = r3;
  // mr r3,r28
  r3 = r28;
  // addi r5,r11,1646
  r5 = uint64_t(int64_t(int64_t(r11) + 1646));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82273574, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273580, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8432(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8432);
  // mr r4,r3
  r4 = r3;
  // mr r3,r28
  r3 = r28;
  // addi r5,r11,1718
  r5 = uint64_t(int64_t(int64_t(r11) + 1718));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82273594, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,4
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 4, so);
  // blt cr6,0x822734a4
  if (cr6.lt) goto loc_822734A4;
loc_822735A0:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822735A8: lifted from generated/ by lift.py.
lifted::Result f822735A8(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822735A8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-48(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f30));
  // stfd f31,-40(r1)
  guest::StoreU64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(uint32_t(r1) + -4096);
  // stwu r1,-6976(r1)
  ea = -6976 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822735D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24948(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24948);
  // li r6,426
  r6 = uint64_t(int64_t(426));
  // lwz r10,24960(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 24960);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r10,800(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 800);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r30,r10,r11
  r30 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // lbz r11,709(r30)
  r11 = guest::LoadU8(uint32_t(r30) + 709);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8227361c
  if (cr0.eq) goto loc_8227361C;
  // li r6,427
  r6 = uint64_t(int64_t(427));
loc_8227361C:
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273620, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,770(r30)
  r11 = guest::LoadU8(uint32_t(r30) + 770);
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // addi r6,r11,421
  r6 = uint64_t(int64_t(int64_t(r11) + 421));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273638, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24948(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24948);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r29,0(r11)
  r29 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82273648, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,6120
  r11 = uint64_t(int64_t(int64_t(r3) + 6120));
  // mr r9,r29
  r9 = r29;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // lha r10,204(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r11) + 204))));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // beq cr6,0x82273678
  if (cr6.eq) goto loc_82273678;
loc_82273664:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r10) + 0))));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // bne 0x82273664
  if (!cr0.eq) goto loc_82273664;
loc_82273678:
  // lis r9,-32127
  r9 = uint64_t(int64_t(-2105475072));
  // lha r10,2(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r10) + 2))));
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // lwz r9,24944(r9)
  r9 = guest::LoadU32(uint32_t(r9) + 24944);
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r9,0(r9)
  r9 = guest::LoadU32(uint32_t(r9) + 0);
  // addi r29,r11,212
  r29 = uint64_t(int64_t(int64_t(r11) + 212));
  // cmplwi cr6,r9,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 2, so);
  // blt cr6,0x8227373c
  if (cr6.lt) goto loc_8227373C;
  // addi r3,r1,1168
  r3 = uint64_t(int64_t(int64_t(r1) + 1168));
  // bl 0x820aef80
  // (left out: dead in C++)
  { const lifted::Result o = game::CarData::Ctor(LIFTED_ARGS(0x822736A4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3024
  r3 = uint64_t(int64_t(int64_t(r1) + 3024));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(LIFTED_ARGS(0x822736AC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3024
  r3 = uint64_t(int64_t(int64_t(r1) + 3024));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // mr r4,r29
  r4 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x822736BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3024
  r3 = uint64_t(int64_t(int64_t(r1) + 3024));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(LIFTED_ARGS(0x822736C4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3024
  r3 = uint64_t(int64_t(int64_t(r1) + 3024));
  // bl 0x820c4550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820C4550(LIFTED_ARGS(0x822736CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r4,r1,3024
  r4 = uint64_t(int64_t(int64_t(r1) + 3024));
  // addi r3,r1,1168
  r3 = uint64_t(int64_t(int64_t(r1) + 1168));
  // bl 0x820c27e8
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::SetupFromCar(LIFTED_ARGS(0x822736DC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,2436
  r4 = uint64_t(int64_t(int64_t(r1) + 2436));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820ce7d8
  // (left out: dead in C++)
  { const lifted::Result o = game::PowerCurve::FromEngine(LIFTED_ARGS(0x822736E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820ce1d8
  // (left out: dead in C++)
  { const lifted::Result o = game::PowerCurve::ShiftPeak(LIFTED_ARGS(0x822736F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,1840(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1840);
  f0 = double(temp.f32);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f13,1568(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1568);
  f13 = double(temp.f32);
  // lwz r7,8(r29)
  r7 = guest::LoadU32(uint32_t(r29) + 8);
  // fadds f12,f13,f0
  f12 = double(float(f13 + f0));
  // lfs f11,1144(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1144);
  f11 = double(temp.f32);
  // lfs f31,1152(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1152);
  f31 = double(temp.f32);
  // lfs f0,8680(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,-24896(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + -24896);
  f13 = double(temp.f32);
  // fmuls f30,f11,f13
  f30 = double(float(f11 * f13));
  // lfs f13,1168(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 1168);
  f13 = double(temp.f32);
  // fmadds f0,f12,f0,f13
  f0 = double(float(std::fma(f12, f0, f13)));
  // fctidz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x8000000000000000ULL) : (f0 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r10
  guest::StoreU32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r29,80(r1)
  r29 = guest::LoadU32(uint32_t(r1) + 80);
  // b 0x8227374c
  goto loc_8227374C;
loc_8227373C:
  // lwz r7,648(r30)
  r7 = guest::LoadU32(uint32_t(r30) + 648);
  // lfs f31,652(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r30) + 652);
  f31 = double(temp.f32);
  // lwz r29,712(r30)
  r29 = guest::LoadU32(uint32_t(r30) + 712);
  // lfs f30,660(r30)
  temp.u32 = guest::LoadU32(uint32_t(r30) + 660);
  f30 = double(temp.f32);
loc_8227374C:
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273760, r31)); r3 = o.r3; f1 = o.f1; }
  // clrldi r11,r29,32
  r11 = r29 & 0xFFFFFFFF;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r3,r31
  r3 = r31;
  // frsp f13,f0
  f13 = double(float(f0));
  // lfd f0,-24832(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r11) + -24832)));
  // fmul f0,f13,f0
  f0 = f13 * f0;
  // frsp f0,f0
  f0 = double(float(f0));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r10
  guest::StoreU32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r7,80(r1)
  r7 = guest::LoadU32(uint32_t(r1) + 80);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822737A8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // fctiwz f0,f31
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f31) ? int64_t(0x80000000U) : (f31 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f31)))));
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // stfiwx f0,0,r11
  guest::StoreU32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r7,80(r1)
  r7 = guest::LoadU32(uint32_t(r1) + 80);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822737CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r6,436
  r6 = uint64_t(int64_t(436));
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // lfd f0,-24824(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r11) + -24824)));
  // fmul f0,f30,f0
  f0 = f30 * f0;
  // frsp f1,f0
  f1 = double(float(f0));
  // stfd f1,48(r1)
  guest::StoreU64(uint32_t(r1) + 48, std::bit_cast<uint64_t>(f1));
  // ld r7,48(r1)
  r7 = guest::LoadU64(uint32_t(r1) + 48);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822737F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,769(r30)
  r11 = guest::LoadU8(uint32_t(r30) + 769);
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r6,r11,1934
  r6 = uint64_t(int64_t(int64_t(r11) + 1934));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273810, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8432(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 8432);
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // addi r6,r11,1646
  r6 = uint64_t(int64_t(int64_t(r11) + 1646));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273828, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8432(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 8432);
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // addi r6,r11,1718
  r6 = uint64_t(int64_t(int64_t(r11) + 1718));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273840, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,438
  r6 = uint64_t(int64_t(438));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273854, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,440
  r6 = uint64_t(int64_t(440));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273868, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,437
  r6 = uint64_t(int64_t(437));
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8227387C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,439
  r6 = uint64_t(int64_t(439));
  // li r5,14
  r5 = uint64_t(int64_t(14));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273890, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,6976
  r1 = uint64_t(int64_t(int64_t(r1) + 6976));
  // lfd f30,-48(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822738A0: lifted from generated/ by lift.py.
uint64_t f822738A0(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_822738A0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-72(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f30));
  // stfd f31,-64(r1)
  guest::StoreU64(uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822738CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r26,0(r3)
  r26 = guest::LoadU32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_822738D4:
  // addi r5,r31,5
  r5 = uint64_t(int64_t(int64_t(r31) + 5));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822738E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,9
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 9, so);
  // blt cr6,0x822738d4
  if (cr6.lt) goto loc_822738D4;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24948(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24948);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwz r9,24960(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r11,800(r9)
  r11 = guest::LoadU32(uint32_t(r9) + 800);
  // lwzx r10,r10,r11
  r10 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // lwz r11,8376(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 8376);
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // blt cr6,0x82273920
  if (cr6.lt) goto loc_82273920;
  // li r11,8
  r11 = uint64_t(int64_t(8));
loc_82273920:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,820(r9)
  guest::StoreU32(uint32_t(r9) + 820, uint32_t(r11));
  // ble cr6,0x82273aac
  if (!cr6.gt) goto loc_82273AAC;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r31,r10,8123
  r31 = uint64_t(int64_t(int64_t(r10) + 8123));
  // mr r29,r11
  r29 = r11;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r9) + 2608);
  f0 = double(temp.f32);
  // li r30,5
  r30 = uint64_t(int64_t(5));
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 124, temp.u32);
  // lis r27,-32256
  r27 = uint64_t(int64_t(-2113929216));
  // lis r28,-32256
  r28 = uint64_t(int64_t(-2113929216));
  // lfs f30,3140(r10)
  temp.u32 = guest::LoadU32(uint32_t(r10) + 3140);
  f30 = double(temp.f32);
  // lfs f31,8704(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 8704);
  f31 = double(temp.f32);
loc_8227395C:
  // lhz r10,-3(r31)
  r10 = guest::LoadU16(uint32_t(r31) + -3);
  // lbz r11,-1(r31)
  r11 = guest::LoadU8(uint32_t(r31) + -1);
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // lhz r9,-7(r31)
  r9 = guest::LoadU16(uint32_t(r31) + -7);
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // std r10,88(r1)
  guest::StoreU64(uint32_t(r1) + 88, r10);
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lbz r10,0(r31)
  r10 = guest::LoadU8(uint32_t(r31) + 0);
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,116(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 116, temp.u32);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // stfs f13,112(r1)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r1) + 112, temp.u32);
  // bne cr6,0x822739d4
  if (!cr6.eq) goto loc_822739D4;
  // lwz r11,12276(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 12276);
  // add. r11,r10,r11
  r11 = r10 + r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x822739c0
  if (cr0.lt) goto loc_822739C0;
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // ble cr6,0x822739c4
  if (!cr6.gt) goto loc_822739C4;
  // li r11,255
  r11 = uint64_t(int64_t(255));
  // b 0x822739c4
  goto loc_822739C4;
loc_822739C0:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_822739C4:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,96(r1)
  guest::StoreU64(uint32_t(r1) + 96, r11);
  // lfd f0,96(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 96)));
  // b 0x82273a00
  goto loc_82273A00;
loc_822739D4:
  // lwz r11,12280(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 12280);
  // add. r11,r10,r11
  r11 = r10 + r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x822739f0
  if (cr0.lt) goto loc_822739F0;
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // ble cr6,0x822739f4
  if (!cr6.gt) goto loc_822739F4;
  // li r11,255
  r11 = uint64_t(int64_t(255));
  // b 0x822739f4
  goto loc_822739F4;
loc_822739F0:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_822739F4:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,104(r1)
  guest::StoreU64(uint32_t(r1) + 104, r11);
  // lfd f0,104(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 104)));
loc_82273A00:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,144
  r4 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 120, temp.u32);
  // ld r9,0(r11)
  r9 = guest::LoadU64(uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(uint32_t(r11) + 8);
  // std r9,0(r10)
  guest::StoreU64(uint32_t(r10) + 0, r9);
  // std r11,8(r10)
  guest::StoreU64(uint32_t(r10) + 8, r11);
  // bl 0x820d0d60
  // (left out: dead in C++)
  { const lifted::Result o = game::Color::HsvToRgb(LIFTED_ARGS(0x82273A34, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82273A44, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273A4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,128(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r1) + 128);
  f0 = double(temp.f32);
  // lfs f13,132(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 132);
  f13 = double(temp.f32);
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // lfs f12,136(r1)
  temp.u32 = guest::LoadU32(uint32_t(r1) + 136);
  f12 = double(temp.f32);
  // fmuls f13,f13,f30
  f13 = double(float(f13 * f30));
  // fmuls f12,f12,f30
  f12 = double(float(f12 * f30));
  // mr r11,r3
  r11 = r3;
  // mr r5,r30
  r5 = r30;
  // addi r10,r11,56
  r10 = uint64_t(int64_t(int64_t(r11) + 56));
  // addi r9,r11,60
  r9 = uint64_t(int64_t(int64_t(r11) + 60));
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r26
  r3 = r26;
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r10
  guest::StoreU32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // fctiwz f13,f13
  f13 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f13) ? int64_t(0x80000000U) : (f13 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f13)))));
  // stfiwx f13,0,r9
  guest::StoreU32(uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f13)));
  // fctiwz f12,f12
  f12 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f12) ? int64_t(0x80000000U) : (f12 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f12)))));
  // stfiwx f12,0,r11
  guest::StoreU32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f12)));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82273A9C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r31,r31,20
  r31 = uint64_t(int64_t(int64_t(r31) + 20));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x8227395c
  if (!cr0.eq) goto loc_8227395C;
loc_82273AAC:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfd f30,-72(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -72)));
  // lfd f31,-64(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82273AC0: lifted from generated/ by lift.py.
lifted::Result f82273AC0(uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82273AC0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82273AE4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24960(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24960);
  // lwz r10,800(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r11,808(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 808);
  // lwz r10,256(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 256);
  // divwu r9,r11,r10
  r9 = uint32_t(uint32_t(r10) ? uint32_t(r11) / uint32_t(r10) : 0);
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // mullw r10,r9,r10
  r10 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r10))));
  // subf r28,r10,r11
  r28 = r11 - r10;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82273B1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24960);
  // rlwinm r10,r28,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r30,r3
  r30 = r3;
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwzx r29,r11,r10
  r29 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273B38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8432(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8432);
  // mr r10,r3
  r10 = r3;
  // cmpwi cr6,r11,35
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 35, so);
  // bge cr6,0x82273b58
  if (!cr6.lt) goto loc_82273B58;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82273b84
  if (!cr6.eq) goto loc_82273B84;
loc_82273B50:
  // li r11,35
  r11 = uint64_t(int64_t(35));
  // b 0x82273b84
  goto loc_82273B84;
loc_82273B58:
  // cmpwi cr6,r11,43
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 43, so);
  // beq cr6,0x82273b50
  if (cr6.eq) goto loc_82273B50;
  // addi r9,r11,-47
  r9 = uint64_t(int64_t(int64_t(r11) + -47));
  // cmplwi cr6,r9,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 9, so);
  // bgt cr6,0x82273b74
  if (cr6.gt) goto loc_82273B74;
  // addi r11,r11,-11
  r11 = uint64_t(int64_t(int64_t(r11) + -11));
  // b 0x82273b84
  goto loc_82273B84;
loc_82273B74:
  // cmpwi cr6,r11,71
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 71, so);
  // li r11,44
  r11 = uint64_t(int64_t(44));
  // beq cr6,0x82273b84
  if (cr6.eq) goto loc_82273B84;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
loc_82273B84:
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // stw r11,108(r10)
  guest::StoreU32(uint32_t(r10) + 108, uint32_t(r11));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273B94, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,769(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 769);
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r5,r11,1934
  r5 = uint64_t(int64_t(int64_t(r11) + 1934));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82273BA8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273BB4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8432(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8432);
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r5,r11,1646
  r5 = uint64_t(int64_t(int64_t(r11) + 1646));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82273BC8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273BD4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8432(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8432);
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r5,r11,1718
  r5 = uint64_t(int64_t(int64_t(r11) + 1718));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeText(LIFTED_ARGS(0x82273BE8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82273BF0: lifted from generated/ by lift.py.
uint64_t f82273BF0(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82273BF0 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82273C20, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82273C30, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(40))));
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
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82273C50, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(uint32_t(r3) + 16, temp.u32);
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

// sub_82273C70: lifted from generated/ by lift.py.
lifted::Result f82273C70(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82273C70 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82273CA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82273d0c
  if (!cr6.eq) goto loc_82273D0C;
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
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82273CD8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82273CE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,76
  r4 = uint64_t(int64_t(76));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82273CFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82273D04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(uint32_t(r30) + 0, uint32_t(r11));
loc_82273D0C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82273D18, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r10,824(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 824);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82273d44
  if (!cr6.eq) goto loc_82273D44;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x82273D40, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
loc_82273D44:
  // lwz r11,828(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 828);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82273d64
  if (!cr6.eq) goto loc_82273D64;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x82273D64, r31)); r3 = o.r3; f1 = o.f1; }
loc_82273D64:
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

// sub_82273D80: lifted from generated/ by lift.py.
lifted::Result f82273D80(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82273D80 (lifted)");
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
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r31,r3
  r31 = r3;
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r10,816(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 816);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // li r10,33
  r10 = uint64_t(int64_t(33));
  // stw r10,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r10));
  // li r10,34
  r10 = uint64_t(int64_t(34));
  // stw r10,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r10));
  // lwzx r11,r11,r9
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r9));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82273DD4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82273e08
  if (!cr6.eq) goto loc_82273E08;
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
  r3 = guest::LoadU32(uint32_t(r11) + 7152);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(LIFTED_ARGS(0x82273E00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_82273E08:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82273E14, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82273E58: lifted from generated/ by lift.py.
lifted::Result f82273E58(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82273E58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82273E80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 0);
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82273ff4
  if (!cr6.eq) goto loc_82273FF4;
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // stw r29,812(r11)
  guest::StoreU32(uint32_t(r11) + 812, uint32_t(r29));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r29,808(r11)
  guest::StoreU32(uint32_t(r11) + 808, uint32_t(r29));
  // bl 0x82273418
  // (left out: dead in C++)
  { const lifted::Result o = f82273418(r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_82273EAC:
  // addi r4,r29,64
  r4 = uint64_t(int64_t(int64_t(r29) + 64));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82273EB8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 4, so);
  // blt cr6,0x82273eac
  if (cr6.lt) goto loc_82273EAC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,83
  r4 = uint64_t(int64_t(83));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82273EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r11,256(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 256);
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x82273f38
  if (cr6.lt) goto loc_82273F38;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82273F04, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,74
  r4 = uint64_t(int64_t(74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82273F10, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,74
  r4 = uint64_t(int64_t(74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82273F2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82273F34, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82273f3c
  goto loc_82273F3C;
loc_82273F38:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82273F3C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82273F3C:
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // lwz r10,808(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 808);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82273f5c
  if (!cr0.eq) goto loc_82273F5C;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // b 0x82273f74
  goto loc_82273F74;
loc_82273F5C:
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r11,256(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 256);
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x82273f7c
  if (!cr6.eq) goto loc_82273F7C;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_82273F74:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82273F78, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82273f94
  goto loc_82273F94;
loc_82273F7C:
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82273F84, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82273F94, r31)); r3 = o.r3; f1 = o.f1; }
loc_82273F94:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24948(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24948);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r10,800(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 800);
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,8432(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8432);
  // addi r6,r11,1862
  r6 = uint64_t(int64_t(int64_t(r11) + 1862));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82273FC8, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x82273FDC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x82273FEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(uint32_t(r28) + 0, uint32_t(r11));
loc_82273FF4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82274000, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r29,r11,27,31,31
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x82274038
  if (cr0.eq) goto loc_82274038;
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,812(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 812);
  // addi r4,r11,64
  r4 = uint64_t(int64_t(int64_t(r11) + 64));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82274030, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82274038, r31)); r3 = o.r3; f1 = o.f1; }
loc_82274038:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82274048: lifted from generated/ by lift.py.
lifted::Result f82274048(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82274048 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274078, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822740f0
  if (!cr6.eq) goto loc_822740F0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822740A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r6,r11,930
  r6 = uint64_t(int64_t(int64_t(r11) + 930));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822740C4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x822740D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x822740E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(uint32_t(r30) + 0, uint32_t(r11));
loc_822740F0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822740FC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82274120: lifted from generated/ by lift.py.
lifted::Result f82274120(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82274120 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r22,r3
  r22 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274148, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r10,0(r22)
  r10 = guest::LoadU32(uint32_t(r22) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r11,256(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 256);
  // addi r26,r11,-1
  r26 = uint64_t(int64_t(int64_t(r11) + -1));
  // bne cr6,0x82274360
  if (!cr6.eq) goto loc_82274360;
  // lis r23,-32127
  r23 = uint64_t(int64_t(-2105475072));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // mr r29,r24
  r29 = r24;
  // lwz r11,24948(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 24948);
  // lwz r28,4(r11)
  r28 = guest::LoadU32(uint32_t(r11) + 4);
  // lwz r27,0(r11)
  r27 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r25,8(r11)
  r25 = guest::LoadU32(uint32_t(r11) + 8);
loc_82274188:
  // addi r4,r29,64
  r4 = uint64_t(int64_t(int64_t(r29) + 64));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82274194, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 4, so);
  // blt cr6,0x82274188
  if (cr6.lt) goto loc_82274188;
  // li r4,69
  r4 = uint64_t(int64_t(69));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822741AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,70
  r4 = uint64_t(int64_t(70));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822741B8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r28,r26
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r26), so);
  // bne cr6,0x822741dc
  if (!cr6.eq) goto loc_822741DC;
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x822741dc
  if (!cr6.eq) goto loc_822741DC;
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r24,812(r11)
  guest::StoreU32(uint32_t(r11) + 812, uint32_t(r24));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r24,808(r11)
  guest::StoreU32(uint32_t(r11) + 808, uint32_t(r24));
  // b 0x822742d4
  goto loc_822742D4;
loc_822741DC:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x8227421c
  if (!cr6.eq) goto loc_8227421C;
  // cmpw cr6,r27,r26
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r26), so);
  // bne cr6,0x8227421c
  if (!cr6.eq) goto loc_8227421C;
  // cmpwi cr6,r26,3
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 3, so);
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // bgt cr6,0x822741fc
  if (cr6.gt) goto loc_822741FC;
  // mr r9,r26
  r9 = r26;
loc_822741FC:
  // lwz r10,24960(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24960);
  // addic. r11,r26,-3
  ca = uint32_t(r26) > 2;
  r11 = uint64_t(int64_t(int64_t(r26) + -3));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r9,812(r10)
  guest::StoreU32(uint32_t(r10) + 812, uint32_t(r9));
  // bge 0x82274210
  if (!cr0.lt) goto loc_82274210;
  // mr r11,r24
  r11 = r24;
loc_82274210:
  // lwz r10,24960(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r11,808(r10)
  guest::StoreU32(uint32_t(r10) + 808, uint32_t(r11));
  // b 0x822742d4
  goto loc_822742D4;
loc_8227421C:
  // cmpwi cr6,r25,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r25), -1, so);
  // bne cr6,0x82274234
  if (!cr6.eq) goto loc_82274234;
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lwz r10,812(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 812);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // b 0x82274248
  goto loc_82274248;
loc_82274234:
  // cmpwi cr6,r25,1
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 1, so);
  // bne cr6,0x8227424c
  if (!cr6.eq) goto loc_8227424C;
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lwz r10,812(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 812);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
loc_82274248:
  // stw r10,812(r11)
  guest::StoreU32(uint32_t(r11) + 812, uint32_t(r10));
loc_8227424C:
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lwz r9,812(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 812);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bge 0x82274278
  if (!cr0.lt) goto loc_82274278;
  // lwz r10,808(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 808);
  // li r4,70
  r4 = uint64_t(int64_t(70));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,808(r11)
  guest::StoreU32(uint32_t(r11) + 808, uint32_t(r10));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r24,812(r11)
  guest::StoreU32(uint32_t(r11) + 812, uint32_t(r24));
  // b 0x822742b8
  goto loc_822742B8;
loc_82274278:
  // cmpwi cr6,r26,3
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 3, so);
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // bgt cr6,0x82274288
  if (cr6.gt) goto loc_82274288;
  // mr r10,r26
  r10 = r26;
loc_82274288:
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // ble cr6,0x822742d4
  if (!cr6.gt) goto loc_822742D4;
  // lwz r10,808(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 808);
  // cmpwi cr6,r26,3
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 3, so);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,808(r11)
  guest::StoreU32(uint32_t(r11) + 808, uint32_t(r10));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // bgt cr6,0x822742ac
  if (cr6.gt) goto loc_822742AC;
  // mr r10,r26
  r10 = r26;
loc_822742AC:
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // li r4,69
  r4 = uint64_t(int64_t(69));
  // stw r10,812(r11)
  guest::StoreU32(uint32_t(r11) + 812, uint32_t(r10));
loc_822742B8:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822742D0, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82273ac0
  // (left out: dead in C++)
  { const lifted::Result o = f82273AC0(r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_822742D4:
  // bl 0x82273418
  // (left out: dead in C++)
  { const lifted::Result o = f82273418(r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,812(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 812);
  // mr r3,r30
  r3 = r30;
  // addi r4,r11,64
  r4 = uint64_t(int64_t(int64_t(r11) + 64));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822742FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82274304, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24948(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 24948);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lwz r10,800(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 800);
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,8432(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8432);
  // addi r6,r11,1862
  r6 = uint64_t(int64_t(int64_t(r11) + 1862));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82274334, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x82274348, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x82274358, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r22)
  guest::StoreU32(uint32_t(r22) + 0, uint32_t(r11));
loc_82274360:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8227436C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82274378: lifted from generated/ by lift.py.
lifted::Result f82274378(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82274378 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822743A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822743dc
  if (!cr6.eq) goto loc_822743DC;
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
  r3 = guest::LoadU32(uint32_t(r11) + 7152);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(LIFTED_ARGS(0x822743D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_822743DC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822743E8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82274428: lifted from generated/ by lift.py.
lifted::Result f82274428(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82274428 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274458, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822744e8
  if (!cr6.eq) goto loc_822744E8;
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
  r3 = guest::LoadU32(uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeIn(LIFTED_ARGS(0x82274484, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24948(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24948);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r10,800(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 800);
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,8432(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8432);
  // addi r6,r11,1862
  r6 = uint64_t(int64_t(int64_t(r11) + 1862));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822744BC, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x822744D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x822744E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(uint32_t(r30) + 0, uint32_t(r11));
loc_822744E8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822744F4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82274538: lifted from generated/ by lift.py.
uint64_t f82274538(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82274538 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274568, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82274578, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(40))));
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
  { const lifted::Result o = game::UiMovie::Child(LIFTED_ARGS(0x82274598, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(uint32_t(r3) + 16, temp.u32);
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

// sub_822745B8: lifted from generated/ by lift.py.
lifted::Result f822745B8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822745B8 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822745f8
  if (!cr6.eq) goto loc_822745F8;
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
  r3 = guest::LoadU32(uint32_t(r11) + 7152);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(LIFTED_ARGS(0x822745F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_822745F8:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r31,r11,23756
  r31 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274614, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82274620, r31)); r3 = o.r3; f1 = o.f1; }
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
  // rlwinm. r27,r11,27,31,31
  r27 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // beq 0x822746cc
  if (cr0.eq) goto loc_822746CC;
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274654, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r29,0(r3)
  r29 = guest::LoadU32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // mr r28,r31
  r28 = r31;
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r31,816(r11)
  guest::StoreU32(uint32_t(r11) + 816, uint32_t(r31));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24948(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24948);
  // stw r31,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  guest::StoreU32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  guest::StoreU32(uint32_t(r11) + 32, uint32_t(r31));
loc_82274680:
  // addi r4,r28,64
  r4 = uint64_t(int64_t(int64_t(r28) + 64));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8227468C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmplwi cr6,r28,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 4, so);
  // blt cr6,0x82274680
  if (cr6.lt) goto loc_82274680;
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
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
  // lwz r11,812(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 812);
  // addi r4,r11,64
  r4 = uint64_t(int64_t(int64_t(r11) + 64));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822746BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x822746C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r31,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r31));
loc_822746CC:
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822746D8: lifted from generated/ by lift.py.
lifted::Result f822746D8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822746D8 (lifted)");
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274700, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(uint32_t(r26) + 0);
  // lwz r28,0(r3)
  r28 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822748cc
  if (!cr6.eq) goto loc_822748CC;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r10,24960(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r11,24948(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24948);
  // lwz r6,800(r10)
  r6 = guest::LoadU32(uint32_t(r10) + 800);
  // lwz r9,4(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 4);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r7,8(r11)
  r7 = guest::LoadU32(uint32_t(r11) + 8);
  // lwz r11,256(r6)
  r11 = guest::LoadU32(uint32_t(r6) + 256);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bne cr6,0x8227475c
  if (!cr6.eq) goto loc_8227475C;
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne cr6,0x8227475c
  if (!cr6.eq) goto loc_8227475C;
  // stw r31,812(r10)
  guest::StoreU32(uint32_t(r10) + 812, uint32_t(r31));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r31,808(r11)
  guest::StoreU32(uint32_t(r11) + 808, uint32_t(r31));
  // b 0x82274810
  goto loc_82274810;
loc_8227475C:
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x82274798
  if (!cr6.eq) goto loc_82274798;
  // cmpw cr6,r8,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r11), so);
  // bne cr6,0x82274798
  if (!cr6.eq) goto loc_82274798;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // bgt cr6,0x8227477c
  if (cr6.gt) goto loc_8227477C;
  // mr r9,r11
  r9 = r11;
loc_8227477C:
  // addic. r11,r11,-3
  ca = uint32_t(r11) > 2;
  r11 = uint64_t(int64_t(int64_t(r11) + -3));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r9,812(r10)
  guest::StoreU32(uint32_t(r10) + 812, uint32_t(r9));
  // bge 0x8227478c
  if (!cr0.lt) goto loc_8227478C;
  // mr r11,r31
  r11 = r31;
loc_8227478C:
  // lwz r10,24960(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r11,808(r10)
  guest::StoreU32(uint32_t(r10) + 808, uint32_t(r11));
  // b 0x82274810
  goto loc_82274810;
loc_82274798:
  // cmpwi cr6,r7,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), -1, so);
  // bne cr6,0x822747d4
  if (!cr6.eq) goto loc_822747D4;
  // lwz r11,812(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 812);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,812(r10)
  guest::StoreU32(uint32_t(r10) + 812, uint32_t(r11));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r10,812(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 812);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x82274810
  if (!cr6.lt) goto loc_82274810;
  // lwz r10,808(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 808);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,808(r11)
  guest::StoreU32(uint32_t(r11) + 808, uint32_t(r10));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r31,812(r11)
  guest::StoreU32(uint32_t(r11) + 812, uint32_t(r31));
  // b 0x82274810
  goto loc_82274810;
loc_822747D4:
  // cmpwi cr6,r7,1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 1, so);
  // bne cr6,0x82274810
  if (!cr6.eq) goto loc_82274810;
  // lwz r11,812(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 812);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,812(r10)
  guest::StoreU32(uint32_t(r10) + 812, uint32_t(r11));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r10,812(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 812);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // ble cr6,0x82274810
  if (!cr6.gt) goto loc_82274810;
  // lwz r10,808(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 808);
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,808(r11)
  guest::StoreU32(uint32_t(r11) + 808, uint32_t(r10));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r9,812(r11)
  guest::StoreU32(uint32_t(r11) + 812, uint32_t(r9));
loc_82274810:
  // bl 0x82273418
  // (left out: dead in C++)
  { const lifted::Result o = f82273418(r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x822735a8
  // (left out: dead in C++)
  { const lifted::Result o = f822735A8(r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x822738a0
  // (left out: dead in C++)
  r3 = f822738A0(r1, r13);
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r28
  r3 = r28;
  // stw r31,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r31));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,24960(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r11,24956(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24956);
  // lwz r10,820(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 820);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r31,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r29,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r29));
  // stw r31,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
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
  // lwz r11,24948(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24948);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,8432(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8432);
  // addi r6,r11,1862
  r6 = uint64_t(int64_t(int64_t(r11) + 1862));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822748A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x822748B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x822748C8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r26)
  guest::StoreU32(uint32_t(r26) + 0, uint32_t(r29));
loc_822748CC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822748D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822748E8: lifted from generated/ by lift.py.
lifted::Result f822748E8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822748E8 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274910, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822749c4
  if (!cr6.eq) goto loc_822749C4;
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // li r30,9
  r30 = uint64_t(int64_t(9));
  // addi r29,r11,31540
  r29 = uint64_t(int64_t(int64_t(r11) + 31540));
loc_8227492C:
  // mr r3,r31
  r3 = r31;
  // lwz r4,0(r29)
  r4 = guest::LoadU32(uint32_t(r29) + 0);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82274938, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x8227492c
  if (!cr0.eq) goto loc_8227492C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,114
  r4 = uint64_t(int64_t(114));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82274960, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24948(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24948);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r10,800(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 800);
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,8432(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8432);
  // addi r6,r11,1862
  r6 = uint64_t(int64_t(int64_t(r11) + 1862));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82274998, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x822749AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x822749BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(uint32_t(r28) + 0, uint32_t(r11));
loc_822749C4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822749D0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822749E0: lifted from generated/ by lift.py.
lifted::Result f822749E0(uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_822749E0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274A08, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r4,47
  r4 = uint64_t(int64_t(47));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82274A18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82274A24, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82274A40, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82274A5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82274A64, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82274A78: lifted from generated/ by lift.py.
lifted::Result f82274A78(uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82274A78 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274AA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // li r4,47
  r4 = uint64_t(int64_t(47));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82274AB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82274ABC, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82274AD8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,46
  r4 = uint64_t(int64_t(46));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82274AE4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82274AF8: lifted from generated/ by lift.py.
lifted::Result f82274AF8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82274AF8 (lifted)");
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
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274B24, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82274b3c
  if (!cr6.eq) goto loc_82274B3C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_82274B3C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82274B44, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82274B60: lifted from generated/ by lift.py.
lifted::Result f82274B60(uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82274B60 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r10,-31636(r10)
  r10 = guest::LoadU32(uint32_t(r10) + -31636);
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // addi r30,r10,16
  r30 = uint64_t(int64_t(int64_t(r10) + 16));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r10,24948(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 24948);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // lwz r31,0(r10)
  r31 = guest::LoadU32(uint32_t(r10) + 0);
  // blt cr6,0x82274c10
  if (cr6.lt) goto loc_82274C10;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82274BA8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,6120
  r11 = uint64_t(int64_t(int64_t(r3) + 6120));
  // mr r9,r31
  r9 = r31;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // lha r10,204(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r11) + 204))));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // beq cr6,0x82274bd8
  if (cr6.eq) goto loc_82274BD8;
loc_82274BC4:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r10) + 0))));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // bne 0x82274bc4
  if (!cr0.eq) goto loc_82274BC4;
loc_82274BD8:
  // lha r10,2(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r10) + 2))));
  // mr r3,r30
  r3 = r30;
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r31,r11,212
  r31 = uint64_t(int64_t(int64_t(r11) + 212));
  // mr r4,r31
  r4 = r31;
  // bl 0x82271e58
  // (left out: dead in C++)
  { const lifted::Result o = f82271E58(r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // addi r5,r30,96
  r5 = uint64_t(int64_t(int64_t(r30) + 96));
  // mr r4,r31
  r4 = r31;
  // lwz r11,-31776(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x82179a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82179A60, LIFTED_ARGS(0x82274C0C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82274c34
  goto loc_82274C34;
loc_82274C10:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // rlwinm r10,r31,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // mr r3,r30
  r3 = r30;
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwzx r4,r11,r10
  r4 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // bl 0x82271d50
  // (left out: dead in C++)
  f82271D50(r3, r4);
  // addi r3,r30,96
  r3 = uint64_t(int64_t(int64_t(r30) + 96));
  // bl 0x822724c0
  // (left out: dead in C++)
  { const lifted::Result o = f822724C0(r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_82274C34:
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

// sub_82274C50: lifted from generated/ by lift.py.
uint64_t f82274C50(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82274C50 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274C80, r31)); r3 = o.r3; f1 = o.f1; }
  // clrldi r11,r31,32
  r11 = r31 & 0xFFFFFFFF;
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82274CA4, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(uint32_t(r3) + 16, temp.u32);
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

// sub_82274CC0: lifted from generated/ by lift.py.
uint64_t f82274CC0(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82274CC0 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274CF0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrldi r11,r31,32
  r11 = r31 & 0xFFFFFFFF;
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82274D14, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(uint32_t(r3) + 16, temp.u32);
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

// sub_82274D30: lifted from generated/ by lift.py.
uint64_t f82274D30(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82274D30 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82274D60, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,6
  r11 = uint64_t(int64_t(int64_t(r31) + 6));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FindNode(LIFTED_ARGS(0x82274D88, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(uint32_t(r3) + 16, temp.u32);
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

// sub_82274DA8: lifted from generated/ by lift.py.
lifted::Result f82274DA8(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82274DA8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r12,-2
  r12 = uint64_t(int64_t(-131072));
  // ori r12,r12,32320
  r12 = r12 | 32320;
  // bl 0x825a9934
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwux r1,r1,r12
  ea = uint32_t(r1) + uint32_t(r12);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x82274DC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // mr r27,r3
  r27 = r3;
  // mr r29,r31
  r29 = r31;
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // stw r31,276(r11)
  guest::StoreU32(uint32_t(r11) + 276, uint32_t(r31));
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // stw r31,536(r11)
  guest::StoreU32(uint32_t(r11) + 536, uint32_t(r31));
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // stw r31,796(r11)
  guest::StoreU32(uint32_t(r11) + 796, uint32_t(r31));
loc_82274DF4:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x82274e28
  if (cr6.lt) goto loc_82274E28;
  // clrlwi r10,r29,27
  r10 = uint32_t(r29) & 0x1F;
  // srawi r11,r29,5
  ca = (int32_t(r29) < 0) & ((uint32_t(r29) & 0x1F) != 0);
  r11 = uint64_t(int64_t(int32_t(r29) >> 5));
  // addi r11,r11,26185
  r11 = uint64_t(int64_t(int64_t(r11) + 26185));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r27
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r27));
  // slw r10,r26,r10
  r10 = uint8_t(r10) & 0x20 ? 0 : (uint32_t(r26) << (uint8_t(r10) & 0x3F));
  // and r11,r10,r11
  r11 = r10 & r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // b 0x82274e2c
  goto loc_82274E2C;
loc_82274E28:
  // mr r11,r31
  r11 = r31;
loc_82274E2C:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82274ebc
  if (cr0.eq) goto loc_82274EBC;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,8448
  r4 = uint64_t(int64_t(8448));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(LIFTED_ARGS(0x82274E48, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x820c9e98
  // (left out: dead in C++)
  { const lifted::Result o = game::Params::GetCarSpec(LIFTED_ARGS(0x82274E54, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,8432
  r5 = uint64_t(int64_t(8432));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82274E64, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,771(r30)
  r11 = guest::LoadU8(schema::StandModelsSpecCopy(uint32_t(r30)).unknown_771_address());
  // stw r29,8432(r30)
  guest::StoreU32(schema::StandModelsSpecCopy(uint32_t(r30)).car_id_address(), uint32_t(r29));
  // extsb. r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82274e80
  if (!cr0.eq) goto loc_82274E80;
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // b 0x82274e90
  goto loc_82274E90;
loc_82274E80:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82274ea8
  if (!cr6.eq) goto loc_82274EA8;
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // addi r11,r11,280
  r11 = uint64_t(int64_t(int64_t(r11) + 280));
loc_82274E90:
  // lwz r10,256(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 256);
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,256(r11)
  guest::StoreU32(uint32_t(r11) + 256, uint32_t(r9));
  // stwx r30,r10,r11
  guest::StoreU32(uint32_t(r10) + uint32_t(r11), uint32_t(r30));
  // b 0x82274ebc
  goto loc_82274EBC;
loc_82274EA8:
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // mr r3,r30
  r3 = r30;
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(LIFTED_ARGS(0x82274EBC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82274EBC:
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r29,72
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 72, so);
  // blt cr6,0x82274df4
  if (cr6.lt) goto loc_82274DF4;
  // lis r5,1
  r5 = uint64_t(int64_t(65536));
  // addi r4,r27,6120
  r4 = uint64_t(int64_t(int64_t(r27) + 6120));
  // ori r5,r5,33076
  r5 = r5 | 33076;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82274EDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lha r27,288(r1)
  r27 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r1) + 288))));
  // cmplwi r27,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq 0x82274fc0
  if (cr0.eq) goto loc_82274FC0;
  // lha r11,284(r1)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r1) + 284))));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r29,r11,r10
  r29 = r11 + r10;
loc_82274EF8:
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,8448
  r4 = uint64_t(int64_t(8448));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(LIFTED_ARGS(0x82274F0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r10,r31
  r10 = r31;
  // mr r11,r29
  r11 = r29;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82274f38
  if (cr6.eq) goto loc_82274F38;
loc_82274F20:
  // lha r11,0(r11)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r11) + 0))));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r9
  r11 = r11 + r9;
  // bne 0x82274f20
  if (!cr0.eq) goto loc_82274F20;
loc_82274F38:
  // lha r11,2(r11)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r11) + 2))));
  // addi r10,r1,304
  r10 = uint64_t(int64_t(int64_t(r1) + 304));
  // mulli r11,r11,1968
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1968))));
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // bl 0x820c9e98
  // (left out: dead in C++)
  { const lifted::Result o = game::Params::GetCarSpec(LIFTED_ARGS(0x82274F4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,8432
  r5 = uint64_t(int64_t(8432));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82274F5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r31
  r10 = r31;
  // mr r11,r29
  r11 = r29;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82274f84
  if (cr6.eq) goto loc_82274F84;
loc_82274F6C:
  // lha r11,0(r11)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r11) + 0))));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r9
  r11 = r11 + r9;
  // bne 0x82274f6c
  if (!cr0.eq) goto loc_82274F6C;
loc_82274F84:
  // lha r11,2(r11)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r11) + 2))));
  // addi r10,r1,304
  r10 = uint64_t(int64_t(int64_t(r1) + 304));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // mulli r11,r11,1968
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1968))));
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // cmplw cr6,r31,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r27), so);
  // stw r11,8432(r30)
  guest::StoreU32(schema::StandModelsSpecCopy(uint32_t(r30)).car_id_address(), uint32_t(r11));
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // addi r11,r11,540
  r11 = uint64_t(int64_t(int64_t(r11) + 540));
  // lwz r10,256(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 256);
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,256(r11)
  guest::StoreU32(uint32_t(r11) + 256, uint32_t(r9));
  // stwx r30,r10,r11
  guest::StoreU32(uint32_t(r10) + uint32_t(r11), uint32_t(r30));
  // blt cr6,0x82274ef8
  if (cr6.lt) goto loc_82274EF8;
loc_82274FC0:
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // lwz r10,536(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 536);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82274fd8
  if (cr6.eq) goto loc_82274FD8;
  // stw r26,824(r11)
  guest::StoreU32(uint32_t(r11) + 824, uint32_t(r26));
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
loc_82274FD8:
  // lwz r10,796(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 796);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82274fe8
  if (cr6.eq) goto loc_82274FE8;
  // stw r26,828(r11)
  guest::StoreU32(uint32_t(r11) + 828, uint32_t(r26));
loc_82274FE8:
  // lwz r1,0(r1)
  r1 = guest::LoadU32(uint32_t(r1) + 0);
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82274FF0: lifted from generated/ by lift.py.
lifted::Result f82274FF0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82274FF0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-4016(r1)
  ea = -4016 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r29,r3
  r29 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // lwz r11,-31776(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31776);
  // lwz r31,0(r11)
  r31 = guest::LoadU32(uint32_t(r11) + 0);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,11844(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 11844);
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r5,8(r11)
  r5 = guest::LoadU32(uint32_t(r11) + 8);
  // bl 0x820c5c20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x820C5C20, LIFTED_ARGS(0x82275024, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,24948(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24948);
  // lwz r10,24960(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 24960);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r10,800(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 800);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // lwz r30,8432(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 8432);
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(LIFTED_ARGS(0x82275050, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820cd8a8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::SetModel(LIFTED_ARGS(0x8227505C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3968(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 3968);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,3968(r1)
  guest::StoreU32(uint32_t(r1) + 3968, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(LIFTED_ARGS(0x82275070, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x82178de8
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetPlates(LIFTED_ARGS(0x82275088, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // bl 0x8217fdc8
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetCarClass(LIFTED_ARGS(0x82275098, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,4016
  r1 = uint64_t(int64_t(int64_t(r1) + 4016));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822750A0: lifted from generated/ by lift.py.
lifted::Result f822750A0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822750A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-4032(r1)
  ea = -4032 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 12);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82275270
  if (cr6.eq) goto loc_82275270;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82275270
  if (cr6.eq) goto loc_82275270;
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r28)
  r11 = guest::LoadU32(uint32_t(r28) + -31776);
  // lwz r27,0(r11)
  r27 = schema::StandHolder(uint32_t(r11)).stage_block_0_bits();
  // mr r3,r27
  r3 = r27;
  // bl 0x82179be0
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::IsBusy(LIFTED_ARGS(0x822750E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r10,2812(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 2812);
  // beq 0x8227510c
  if (cr0.eq) goto loc_8227510C;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82275128
  if (!cr6.eq) goto loc_82275128;
  // bl 0x822749e0
  // (left out: dead in C++)
  { const lifted::Result o = f822749E0(r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // b 0x8227511c
  goto loc_8227511C;
loc_8227510C:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82275128
  if (!cr6.eq) goto loc_82275128;
  // bl 0x82274a78
  // (left out: dead in C++)
  { const lifted::Result o = f82274A78(r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_8227511C:
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r10,2812(r11)
  guest::StoreU32(uint32_t(r11) + 2812, uint32_t(r10));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
loc_82275128:
  // lwz r10,16(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 16);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82275154
  if (cr6.eq) goto loc_82275154;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r27
  r3 = r27;
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetDrawWord(LIFTED_ARGS(0x82275140, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // bl 0x8217fe60
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::Draw(LIFTED_ARGS(0x82275150, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82275270
  goto loc_82275270;
loc_82275154:
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r10,24948(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 24948);
  // lwz r10,0(r10)
  r10 = schema::FrontEndMenu(uint32_t(r10)).cursor_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // lwz r29,8432(r11)
  r29 = guest::LoadU32(uint32_t(r11) + 8432);
  // mr r3,r29
  r3 = r29;
  // bl 0x8215aad0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarModels::BlockOf(LIFTED_ARGS(0x82275178, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r27
  r3 = r27;
  // bl 0x8217f880
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::ReloadModel(LIFTED_ARGS(0x82275184, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // stw r3,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r3));
  // lwz r11,24960(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24960);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82275270
  if (cr6.eq) goto loc_82275270;
  // lwz r11,-31776(r28)
  r11 = guest::LoadU32(uint32_t(r28) + -31776);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r28,0(r11)
  r28 = schema::StandHolder(uint32_t(r11)).stage_block_0_bits();
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(LIFTED_ARGS(0x822751AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x8227521c
  if (!cr6.eq) goto loc_8227521C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(LIFTED_ARGS(0x822751C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24948(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24948);
  // lwz r10,0(r11)
  r10 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
  // addi r11,r3,6120
  r11 = uint64_t(int64_t(int64_t(r3) + 6120));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r9,204(r11)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(schema::SaveData(uint32_t(r11) - 6120).garage_head_address()))));
  // b 0x822751e8
  goto loc_822751E8;
loc_822751E0:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_822751E8:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x822751e0
  if (!cr0.eq) goto loc_822751E0;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r9) + 2))));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r4,r11,212
  r4 = uint64_t(int64_t(int64_t(r11) + 212));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x82275210, r31)); r3 = o.r3; f1 = o.f1; }
  // lha r31,80(r1)
  r31 = uint64_t(int64_t(int16_t(guest::LoadU16(uint32_t(r1) + 80))));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x82275238
  goto loc_82275238;
loc_8227521C:
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820cd8a8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::SetModel(LIFTED_ARGS(0x82275228, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,24956(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24956);
  // lwz r31,0(r11)
  r31 = schema::FrontEndMenu(uint32_t(r11)).cursor_bits();
loc_82275238:
  // mr r3,r28
  r3 = r28;
  // bl 0x82178de8
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetPlates(LIFTED_ARGS(0x82275240, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3968(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 3968);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,3968(r1)
  guest::StoreU32(uint32_t(r1) + 3968, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(LIFTED_ARGS(0x82275254, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r31
  r5 = r31;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // bl 0x8217fd58
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::SetCar(LIFTED_ARGS(0x82275264, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r27
  r3 = r27;
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = game::GarageStage::Command(LIFTED_ARGS(0x82275270, r31)); r3 = o.r3; f1 = o.f1; }
loc_82275270:
  // addi r1,r1,4032
  r1 = uint64_t(int64_t(int64_t(r1) + 4032));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82275278: lifted from generated/ by lift.py.
lifted::Result f82275278(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82275278 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = schema::StandHolder(uint32_t(r11)).stage_block_0_bits();
  // lwz r11,0(r3)
  r11 = guest::LoadU32(schema::GarageStage(uint32_t(r3)).index_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(schema::GarageStage(uint32_t(r11)).model_claimed_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822752ac
  if (!cr6.eq) goto loc_822752AC;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8217fcb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8217FCB0, LIFTED_ARGS(0x822752AC, r31)); r3 = o.r3; f1 = o.f1; }
loc_822752AC:
  // bl 0x82274da8
  // (left out: dead in C++)
  { const lifted::Result o = f82274DA8(r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82273128
  // (left out: dead in C++)
  { const lifted::Result o = f82273128(r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822752cc
  if (cr0.eq) goto loc_822752CC;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_822752CC:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r8,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r8));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822752F8: lifted from generated/ by lift.py.
lifted::Result f822752F8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822752F8 (lifted)");
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
  // lwz r3,24952(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24952);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x8227531C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24952(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24952);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82275328, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822754d8
  if (cr0.eq) goto loc_822754D8;
  // lwz r11,24952(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24952);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x822753c8
  if (cr6.lt) goto loc_822753C8;
  // bne cr6,0x822755c0
  if (!cr6.eq) goto loc_822755C0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8227534C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82274b60
  // (left out: dead in C++)
  { const lifted::Result o = f82274B60(r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,34
  r9 = uint64_t(int64_t(34));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(uint32_t(r11) + 600, uint32_t(r10));
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
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x822753b4
  if (cr6.eq) goto loc_822753B4;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_822753B4:
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // b 0x822755bc
  goto loc_822755BC;
loc_822753C8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x822753D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82275408
  if (!cr6.eq) goto loc_82275408;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x822753fc
  if (cr6.eq) goto loc_822753FC;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_822753FC:
  // li r9,14
  r9 = uint64_t(int64_t(14));
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // b 0x822755ac
  goto loc_822755AC;
loc_82275408:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r11,804(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 804);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82275448
  if (!cr0.eq) goto loc_82275448;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,24996(r10)
  guest::StoreU32(uint32_t(r10) + 24996, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x82275440
  if (cr6.eq) goto loc_82275440;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82275440:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // b 0x82275478
  goto loc_82275478;
loc_82275448:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82275488
  if (!cr6.eq) goto loc_82275488;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,24996(r10)
  guest::StoreU32(uint32_t(r10) + 24996, uint32_t(r11));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x82275474
  if (cr6.eq) goto loc_82275474;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82275474:
  // li r9,10
  r9 = uint64_t(int64_t(10));
loc_82275478:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
loc_82275488:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(uint32_t(r11) + 600, uint32_t(r10));
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
  // b 0x822755c0
  goto loc_822755C0;
loc_822754D8:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24952(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24952);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x822754E4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82275530
  if (cr0.eq) goto loc_82275530;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x822754F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x8227550c
  if (cr6.eq) goto loc_8227550C;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_8227550C:
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // li r9,9
  r9 = uint64_t(int64_t(9));
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
  // b 0x822755c0
  goto loc_822755C0;
loc_82275530:
  // lwz r11,24952(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24952);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82275568
  if (cr6.eq) goto loc_82275568;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82275548, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x82275560
  if (cr6.eq) goto loc_82275560;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_82275560:
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // b 0x822755a8
  goto loc_822755A8;
loc_82275568:
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r3,24948(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24948);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82275574, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24948(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24948);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x822755c0
  if (cr6.eq) goto loc_822755C0;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x8227558C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x822755a4
  if (cr6.eq) goto loc_822755A4;
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
loc_822755A4:
  // li r9,13
  r9 = uint64_t(int64_t(13));
loc_822755A8:
  // li r8,5
  r8 = uint64_t(int64_t(5));
loc_822755AC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r8,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
loc_822755BC:
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
loc_822755C0:
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

// sub_822755D8: lifted from generated/ by lift.py.
lifted::Result f822755D8(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822755D8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82275604, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // li r6,123
  r6 = uint64_t(int64_t(123));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8227561C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,112
  r6 = uint64_t(int64_t(112));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82275630, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82275644, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82275658, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x8227566C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82275680, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82275690, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x822756A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r6,804(r11)
  r6 = guest::LoadU32(uint32_t(r11) + 804);
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822756BC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x822735a8
  // (left out: dead in C++)
  { const lifted::Result o = f822735A8(r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x822738a0
  // (left out: dead in C++)
  r3 = f822738A0(r1, r13);
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r10,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r10));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // stw r8,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r8));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r9,24960(r30)
  r9 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r11,24956(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24956);
  // lwz r9,820(r9)
  r9 = guest::LoadU32(uint32_t(r9) + 820);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  guest::StoreU32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  guest::StoreU32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  guest::StoreU32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  guest::StoreU32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,64(r11)
  guest::StoreU32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  guest::StoreU32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  guest::StoreU32(uint32_t(r11) + 32, uint32_t(r10));
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

// sub_82275740: lifted from generated/ by lift.py.
lifted::Result f82275740(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82275740 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82275768, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82275830
  if (!cr6.eq) goto loc_82275830;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,82
  r4 = uint64_t(int64_t(82));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275794, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8211BC18, LIFTED_ARGS(0x822757A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x822757cc
  if (!cr0.eq) goto loc_822757CC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822757C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x822757CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_822757CC:
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8211BC18, LIFTED_ARGS(0x822757D8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82275804
  if (!cr0.eq) goto loc_82275804;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822757FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82275804, r31)); r3 = o.r3; f1 = o.f1; }
loc_82275804:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82273bf0
  // (left out: dead in C++)
  r3 = f82273BF0(r1, r13, r3);
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
  r3 = guest::LoadU32(uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeIn(LIFTED_ARGS(0x82275828, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(uint32_t(r30) + 0, uint32_t(r11));
loc_82275830:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8227583C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // bne 0x82275870
  if (!cr0.eq) goto loc_82275870;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // beq cr6,0x82275874
  if (cr6.eq) goto loc_82275874;
loc_82275870:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82275874:
  // clrlwi. r29,r11,24
  r29 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x822758c8
  if (cr0.eq) goto loc_822758C8;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
  // lwz r10,824(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 824);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x822758a8
  if (!cr6.eq) goto loc_822758A8;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x822758A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24960);
loc_822758A8:
  // lwz r11,828(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 828);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822758c8
  if (!cr6.eq) goto loc_822758C8;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x822758C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822758C8:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822758D8: lifted from generated/ by lift.py.
lifted::Result f822758D8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822758D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,33
  r11 = uint64_t(int64_t(33));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82275900, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // lwz r10,0(r27)
  r10 = guest::LoadU32(uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r11,24944(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24944);
  // lwz r29,0(r11)
  r29 = guest::LoadU32(uint32_t(r11) + 0);
  // bne cr6,0x82275b28
  if (!cr6.eq) goto loc_82275B28;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275930, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x82275950
  if (cr6.lt) goto loc_82275950;
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // beq cr6,0x82275948
  if (cr6.eq) goto loc_82275948;
  // lwz r11,828(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 828);
  // b 0x82275954
  goto loc_82275954;
loc_82275948:
  // lwz r11,824(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 824);
  // b 0x82275954
  goto loc_82275954;
loc_82275950:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_82275954:
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
  // mr r3,r31
  r3 = r31;
  // beq cr6,0x8227598c
  if (cr6.eq) goto loc_8227598C;
  // lwz r11,24944(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275980, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82275988, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822759c8
  goto loc_822759C8;
loc_8227598C:
  // li r4,31
  r4 = uint64_t(int64_t(31));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275994, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,31
  r4 = uint64_t(int64_t(31));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822759A0, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822759BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822759C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822759C8:
  // mr r6,r29
  r6 = r29;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822759DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r29
  r6 = r29;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x822759F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,77
  r4 = uint64_t(int64_t(77));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822759FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,77
  r4 = uint64_t(int64_t(77));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275A18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275A24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275A30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24944(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24944);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 4);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82275a58
  if (!cr0.eq) goto loc_82275A58;
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r9,2
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 2, so);
  // bne cr6,0x82275a58
  if (!cr6.eq) goto loc_82275A58;
  // li r3,80
  r3 = uint64_t(int64_t(80));
loc_82275A50:
  // bl 0x82274c50
  // (left out: dead in C++)
  r3 = f82274C50(r1, r13, r3);
  // b 0x82275aac
  goto loc_82275AAC;
loc_82275A58:
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x82275a74
  if (!cr6.eq) goto loc_82275A74;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82275a74
  if (!cr6.eq) goto loc_82275A74;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82275a50
  goto loc_82275A50;
loc_82275A74:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x82275a88
  if (!cr6.eq) goto loc_82275A88;
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // b 0x82275a94
  goto loc_82275A94;
loc_82275A88:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82275ab0
  if (!cr6.eq) goto loc_82275AB0;
  // li r4,18
  r4 = uint64_t(int64_t(18));
loc_82275A94:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275AAC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82275AAC:
  // lwz r11,24944(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24944);
loc_82275AB0:
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x82275ad0
  if (cr6.lt) goto loc_82275AD0;
  // lwz r10,24960(r28)
  r10 = guest::LoadU32(uint32_t(r28) + 24960);
  // beq cr6,0x82275ac8
  if (cr6.eq) goto loc_82275AC8;
  // lwz r10,828(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 828);
  // b 0x82275ad4
  goto loc_82275AD4;
loc_82275AC8:
  // lwz r10,824(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 824);
  // b 0x82275ad4
  goto loc_82275AD4;
loc_82275AD0:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_82275AD4:
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // beq cr6,0x82275af4
  if (cr6.eq) goto loc_82275AF4;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r6,r11,930
  r6 = uint64_t(int64_t(int64_t(r11) + 930));
  // b 0x82275af8
  goto loc_82275AF8;
loc_82275AF4:
  // li r6,584
  r6 = uint64_t(int64_t(584));
loc_82275AF8:
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82275AFC, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x82275B10, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x82275B20, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(uint32_t(r27) + 0, uint32_t(r11));
loc_82275B28:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82275B34, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
  // lwz r10,824(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 824);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82275b5c
  if (!cr6.eq) goto loc_82275B5C;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x82275B58, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24960(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24960);
loc_82275B5C:
  // lwz r11,828(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 828);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82275b7c
  if (!cr6.eq) goto loc_82275B7C;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x82275B7C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82275B7C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82275B88: lifted from generated/ by lift.py.
lifted::Result f82275B88(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82275B88 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82275BB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(uint32_t(r26) + 0);
  // lis r27,-32127
  r27 = uint64_t(int64_t(-2105475072));
  // lwz r30,0(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // bne cr6,0x82275d88
  if (!cr6.eq) goto loc_82275D88;
  // bl 0x822755d8
  // (left out: dead in C++)
  { const lifted::Result o = f822755D8(r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r28,r31
  r28 = r31;
loc_82275BD4:
  // addi r4,r28,7
  r4 = uint64_t(int64_t(int64_t(r28) + 7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275BE0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmplwi cr6,r28,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 10, so);
  // blt cr6,0x82275bd4
  if (cr6.lt) goto loc_82275BD4;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8211BC18, LIFTED_ARGS(0x82275BF8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82275c24
  if (!cr0.eq) goto loc_82275C24;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275C1C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82275C24, r31)); r3 = o.r3; f1 = o.f1; }
loc_82275C24:
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,24952(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24952);
  // stw r31,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r29,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r29,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r29));
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
  // bl 0x82274538
  // (left out: dead in C++)
  r3 = f82274538(r1, r13, r3);
  // lwz r11,24960(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24960);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,112
  r4 = uint64_t(int64_t(112));
  // mr r3,r30
  r3 = r30;
  // stw r29,816(r11)
  guest::StoreU32(uint32_t(r11) + 816, uint32_t(r29));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275C90, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24952(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24952);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // mr r3,r30
  r3 = r30;
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275CB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82275CBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,49
  r4 = uint64_t(int64_t(49));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275CC8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,50
  r4 = uint64_t(int64_t(50));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275CD4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r28,-32127
  r28 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // lwz r11,24948(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24948);
  // stw r29,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r29));
  // stw r31,28(r11)
  guest::StoreU32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r29,32(r11)
  guest::StoreU32(uint32_t(r11) + 32, uint32_t(r29));
  // lwz r11,24960(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24960);
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r11,256(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 256);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // ble cr6,0x82275d10
  if (!cr6.gt) goto loc_82275D10;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82275D0C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82275d14
  goto loc_82275D14;
loc_82275D10:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x82275D14, r31)); r3 = o.r3; f1 = o.f1; }
loc_82275D14:
  // lwz r11,24948(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 24948);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r11,24960(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24960);
  // lwz r10,800(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 800);
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,8432(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8432);
  // addi r6,r11,1862
  r6 = uint64_t(int64_t(int64_t(r11) + 1862));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82275D44, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x82275D58, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x82275D68, r31)); r3 = o.r3; f1 = o.f1; }
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
  r3 = guest::LoadU32(uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeIn(LIFTED_ARGS(0x82275D84, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r26)
  guest::StoreU32(uint32_t(r26) + 0, uint32_t(r29));
loc_82275D88:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82275D94, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r11,20708(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // bne 0x82275dc8
  if (!cr0.eq) goto loc_82275DC8;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82275dc8
  if (!cr6.eq) goto loc_82275DC8;
  // mr r31,r29
  r31 = r29;
loc_82275DC8:
  // clrlwi. r31,r31,24
  r31 = uint32_t(r31) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x82275e2c
  if (cr0.eq) goto loc_82275E2C;
  // lwz r11,24960(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 24960);
  // lwz r11,800(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 800);
  // lwz r11,256(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 256);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // ble cr6,0x82275e2c
  if (!cr6.gt) goto loc_82275E2C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,49
  r4 = uint64_t(int64_t(49));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275E00, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82275E08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,50
  r4 = uint64_t(int64_t(50));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275E24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82275E2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82275E2C:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82275E38: lifted from generated/ by lift.py.
lifted::Result f82275E38(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82275E38 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82275E60, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82275f40
  if (!cr6.eq) goto loc_82275F40;
  // lis r30,-32127
  r30 = uint64_t(int64_t(-2105475072));
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24952(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24952);
  // lwz r29,0(r11)
  r29 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r28,4(r11)
  r28 = guest::LoadU32(uint32_t(r11) + 4);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275E8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275E98, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r28,7
  r4 = uint64_t(int64_t(int64_t(r28) + 7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82275EA4, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275EC0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82275EC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24952(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24952);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82275ef8
  if (!cr6.eq) goto loc_82275EF8;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x82275ef0
  if (!cr6.eq) goto loc_82275EF0;
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // bne cr6,0x82275ef0
  if (!cr6.eq) goto loc_82275EF0;
  // li r3,40
  r3 = uint64_t(int64_t(40));
  // b 0x82275f14
  goto loc_82275F14;
loc_82275EF0:
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // b 0x82275f20
  goto loc_82275F20;
loc_82275EF8:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82275f38
  if (!cr6.eq) goto loc_82275F38;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x82275f1c
  if (!cr6.eq) goto loc_82275F1C;
  // cmplwi cr6,r28,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 1, so);
  // bne cr6,0x82275f1c
  if (!cr6.eq) goto loc_82275F1C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82275F14:
  // bl 0x82274cc0
  // (left out: dead in C++)
  r3 = f82274CC0(r1, r13, r3);
  // b 0x82275f38
  goto loc_82275F38;
loc_82275F1C:
  // li r4,18
  r4 = uint64_t(int64_t(18));
loc_82275F20:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275F38, r31)); r3 = o.r3; f1 = o.f1; }
loc_82275F38:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(uint32_t(r27) + 0, uint32_t(r11));
loc_82275F40:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82275F4C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82275F58: lifted from generated/ by lift.py.
lifted::Result f82275F58(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82275F58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82275F80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822760a4
  if (!cr6.eq) goto loc_822760A4;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,113
  r4 = uint64_t(int64_t(113));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82275FAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24944(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24944);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r6,r11,933
  r6 = uint64_t(int64_t(int64_t(r11) + 933));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82275FCC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ScrollText(LIFTED_ARGS(0x82275FE0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(LIFTED_ARGS(0x82275FF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r29,r11,31576
  r29 = uint64_t(int64_t(int64_t(r11) + 31576));
loc_82275FFC:
  // addi r11,r29,-36
  r11 = uint64_t(int64_t(int64_t(r29) + -36));
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r30,r11
  r4 = guest::LoadU32(uint32_t(r30) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8227600C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwzx r4,r30,r29
  r4 = guest::LoadU32(uint32_t(r30) + uint32_t(r29));
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82276028, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r30,r29
  r4 = guest::LoadU32(uint32_t(r30) + uint32_t(r29));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82276034, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmplwi cr6,r30,36
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 36, so);
  // blt cr6,0x82275ffc
  if (cr6.lt) goto loc_82275FFC;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r11,24956(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 24956);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 0);
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,24956(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 24956);
  // lwz r30,0(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 0);
  // rlwinm r3,r30,5,0,26
  r3 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 5) & 0xFFFFFFE0;
  // bl 0x82274d30
  // (left out: dead in C++)
  r3 = f82274D30(r1, r13, r3);
  // addi r11,r29,-36
  r11 = uint64_t(int64_t(int64_t(r29) + -36));
  // rlwinm r10,r30,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
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
  // lwzx r4,r10,r11
  r4 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8227608C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x82276094, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82274ff0
  // (left out: dead in C++)
  { const lifted::Result o = f82274FF0(r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(uint32_t(r28) + 0, uint32_t(r11));
loc_822760A4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822760B0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822760C0: lifted from generated/ by lift.py.
lifted::Result f822760C0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822760C0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r24,r3
  r24 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,34
  r11 = uint64_t(int64_t(34));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822760E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r24)
  r11 = guest::LoadU32(uint32_t(r24) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82276230
  if (!cr6.eq) goto loc_82276230;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lis r25,-32127
  r25 = uint64_t(int64_t(-2105475072));
  // li r4,106
  r4 = uint64_t(int64_t(106));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r10,820(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 820);
  // lwz r11,24956(r25)
  r11 = guest::LoadU32(uint32_t(r25) + 24956);
  // addi r27,r10,-1
  r27 = uint64_t(int64_t(int64_t(r10) + -1));
  // lwz r28,0(r11)
  r28 = guest::LoadU32(uint32_t(r11) + 0);
  // lwz r26,4(r11)
  r26 = guest::LoadU32(uint32_t(r11) + 4);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82276124, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,107
  r4 = uint64_t(int64_t(107));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82276130, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r29,r11,31576
  r29 = uint64_t(int64_t(int64_t(r11) + 31576));
loc_8227613C:
  // addi r11,r29,-36
  r11 = uint64_t(int64_t(int64_t(r29) + -36));
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r30,r11
  r4 = guest::LoadU32(uint32_t(r30) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x8227614C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwzx r4,r30,r29
  r4 = guest::LoadU32(uint32_t(r30) + uint32_t(r29));
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x82276168, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r30,r29
  r4 = guest::LoadU32(uint32_t(r30) + uint32_t(r29));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x82276174, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmplwi cr6,r30,36
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 36, so);
  // blt cr6,0x8227613c
  if (cr6.lt) goto loc_8227613C;
  // addi r11,r29,-36
  r11 = uint64_t(int64_t(int64_t(r29) + -36));
  // rlwinm r10,r28,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
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
  // lwzx r4,r10,r11
  r4 = guest::LoadU32(uint32_t(r10) + uint32_t(r11));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822761A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopLast(LIFTED_ARGS(0x822761AC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r27,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r26), so);
  // bne cr6,0x822761c4
  if (!cr6.eq) goto loc_822761C4;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x822761c4
  if (!cr6.eq) goto loc_822761C4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x822761d8
  goto loc_822761D8;
loc_822761C4:
  // cmplw cr6,r27,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r28), so);
  // bne cr6,0x822761e0
  if (!cr6.eq) goto loc_822761E0;
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // bne cr6,0x822761e0
  if (!cr6.eq) goto loc_822761E0;
  // rlwinm r3,r28,5,0,26
  r3 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 5) & 0xFFFFFFE0;
loc_822761D8:
  // bl 0x82274d30
  // (left out: dead in C++)
  r3 = f82274D30(r1, r13, r3);
  // b 0x8227621c
  goto loc_8227621C;
loc_822761E0:
  // lwz r11,24956(r25)
  r11 = guest::LoadU32(uint32_t(r25) + 24956);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x822761f8
  if (!cr6.eq) goto loc_822761F8;
  // li r4,106
  r4 = uint64_t(int64_t(106));
  // b 0x82276204
  goto loc_82276204;
loc_822761F8:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8227621c
  if (!cr6.eq) goto loc_8227621C;
  // li r4,107
  r4 = uint64_t(int64_t(107));
loc_82276204:
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8227621C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8227621C:
  // lwz r11,24956(r25)
  r11 = guest::LoadU32(uint32_t(r25) + 24956);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // bl 0x82274ff0
  // (left out: dead in C++)
  { const lifted::Result o = f82274FF0(r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r24)
  guest::StoreU32(uint32_t(r24) + 0, uint32_t(r11));
loc_82276230:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x8227623C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82276248: lifted from generated/ by lift.py.
lifted::Result f82276248(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82276248 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r27,r4
  r27 = r4;
  // lis r28,-32217
  r28 = uint64_t(int64_t(-2111373312));
  // lis r29,-32240
  r29 = uint64_t(int64_t(-2112880640));
  // lis r30,-32240
  r30 = uint64_t(int64_t(-2112880640));
  // lwz r11,24940(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24940);
  // lis r31,-32217
  r31 = uint64_t(int64_t(-2111373312));
  // lis r4,-32217
  r4 = uint64_t(int64_t(-2111373312));
  // lis r5,-32217
  r5 = uint64_t(int64_t(-2111373312));
  // lis r6,-32217
  r6 = uint64_t(int64_t(-2111373312));
  // lis r7,-32217
  r7 = uint64_t(int64_t(-2111373312));
  // lis r8,-32217
  r8 = uint64_t(int64_t(-2111373312));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // lis r9,-32240
  r9 = uint64_t(int64_t(-2112880640));
  // lis r10,-32240
  r10 = uint64_t(int64_t(-2112880640));
  // addi r28,r28,10600
  r28 = uint64_t(int64_t(int64_t(r28) + 10600));
  // addi r29,r29,25920
  r29 = uint64_t(int64_t(int64_t(r29) + 25920));
  // addi r30,r30,25920
  r30 = uint64_t(int64_t(int64_t(r30) + 25920));
  // addi r31,r31,11136
  r31 = uint64_t(int64_t(int64_t(r31) + 11136));
  // addi r4,r4,11632
  r4 = uint64_t(int64_t(int64_t(r4) + 11632));
  // addi r5,r5,21240
  r5 = uint64_t(int64_t(int64_t(r5) + 21240));
  // stw r28,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r28));
  // addi r6,r6,12416
  r6 = uint64_t(int64_t(int64_t(r6) + 12416));
  // stw r29,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r29));
  // addi r7,r7,12008
  r7 = uint64_t(int64_t(int64_t(r7) + 12008));
  // stw r30,88(r1)
  guest::StoreU32(uint32_t(r1) + 88, uint32_t(r30));
  // addi r8,r8,11496
  r8 = uint64_t(int64_t(int64_t(r8) + 11496));
  // stw r31,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r31));
  // addi r9,r9,25920
  r9 = uint64_t(int64_t(int64_t(r9) + 25920));
  // stw r4,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r4));
  // addi r10,r10,25920
  r10 = uint64_t(int64_t(int64_t(r10) + 25920));
  // stw r5,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r5));
  // stw r6,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r6));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // stw r7,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r7));
  // stw r8,112(r1)
  guest::StoreU32(uint32_t(r1) + 112, uint32_t(r8));
  // stw r9,116(r1)
  guest::StoreU32(uint32_t(r1) + 116, uint32_t(r9));
  // stw r10,120(r1)
  guest::StoreU32(uint32_t(r1) + 120, uint32_t(r10));
  // beq cr6,0x8227630c
  if (cr6.eq) goto loc_8227630C;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r3
  r4 = r3;
  // mr r3,r27
  r3 = r27;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8227630C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8227630C:
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r11,804(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 804);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82276324
  if (!cr6.eq) goto loc_82276324;
  // bl 0x82293460
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82293460, LIFTED_ARGS(0x82276324, r31)); r3 = o.r3; f1 = o.f1; }
loc_82276324:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82276330: lifted from generated/ by lift.py.
lifted::Result f82276330(uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82276330 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,24960(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24960);
  // lwz r10,816(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 816);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // li r10,33
  r10 = uint64_t(int64_t(33));
  // stw r10,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r10));
  // li r10,34
  r10 = uint64_t(int64_t(34));
  // stw r10,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r10));
  // lwzx r11,r11,r9
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r9));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82276378, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // lis r10,-32217
  r10 = uint64_t(int64_t(-2111373312));
  // lis r20,-32217
  r20 = uint64_t(int64_t(-2111373312));
  // addi r10,r10,19192
  r10 = uint64_t(int64_t(int64_t(r10) + 19192));
  // lis r21,-32217
  r21 = uint64_t(int64_t(-2111373312));
  // lwz r11,24940(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24940);
  // lis r22,-32217
  r22 = uint64_t(int64_t(-2111373312));
  // lis r23,-32217
  r23 = uint64_t(int64_t(-2111373312));
  // lis r24,-32163
  r24 = uint64_t(int64_t(-2107834368));
  // lis r25,-32217
  r25 = uint64_t(int64_t(-2111373312));
  // stw r10,164(r1)
  guest::StoreU32(uint32_t(r1) + 164, uint32_t(r10));
  // lis r26,-32217
  r26 = uint64_t(int64_t(-2111373312));
  // lis r27,-32217
  r27 = uint64_t(int64_t(-2111373312));
  // lwz r10,12(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 12);
  // lis r28,-32217
  r28 = uint64_t(int64_t(-2111373312));
  // lis r29,-32217
  r29 = uint64_t(int64_t(-2111373312));
  // lis r30,-32217
  r30 = uint64_t(int64_t(-2111373312));
  // lis r4,-32217
  r4 = uint64_t(int64_t(-2111373312));
  // lis r5,-32217
  r5 = uint64_t(int64_t(-2111373312));
  // lis r6,-32217
  r6 = uint64_t(int64_t(-2111373312));
  // lis r7,-32217
  r7 = uint64_t(int64_t(-2111373312));
  // lis r8,-32217
  r8 = uint64_t(int64_t(-2111373312));
  // lis r9,-32217
  r9 = uint64_t(int64_t(-2111373312));
  // addi r20,r20,22336
  r20 = uint64_t(int64_t(int64_t(r20) + 22336));
  // addi r21,r21,15472
  r21 = uint64_t(int64_t(int64_t(r21) + 15472));
  // addi r22,r22,22744
  r22 = uint64_t(int64_t(int64_t(r22) + 22744));
  // addi r23,r23,15744
  r23 = uint64_t(int64_t(int64_t(r23) + 15744));
  // addi r24,r24,-31856
  r24 = uint64_t(int64_t(int64_t(r24) + -31856));
  // addi r25,r25,15960
  r25 = uint64_t(int64_t(int64_t(r25) + 15960));
  // stw r20,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r20));
  // addi r26,r26,16456
  r26 = uint64_t(int64_t(int64_t(r26) + 16456));
  // stw r21,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r21));
  // addi r27,r27,16672
  r27 = uint64_t(int64_t(int64_t(r27) + 16672));
  // stw r22,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r22));
  // addi r28,r28,17272
  r28 = uint64_t(int64_t(int64_t(r28) + 17272));
  // stw r23,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r23));
  // addi r29,r29,17448
  r29 = uint64_t(int64_t(int64_t(r29) + 17448));
  // stw r24,112(r1)
  guest::StoreU32(uint32_t(r1) + 112, uint32_t(r24));
  // addi r30,r30,23432
  r30 = uint64_t(int64_t(int64_t(r30) + 23432));
  // stw r25,116(r1)
  guest::StoreU32(uint32_t(r1) + 116, uint32_t(r25));
  // addi r4,r4,17848
  r4 = uint64_t(int64_t(int64_t(r4) + 17848));
  // stw r26,120(r1)
  guest::StoreU32(uint32_t(r1) + 120, uint32_t(r26));
  // addi r5,r5,24120
  r5 = uint64_t(int64_t(int64_t(r5) + 24120));
  // stw r27,124(r1)
  guest::StoreU32(uint32_t(r1) + 124, uint32_t(r27));
  // addi r6,r6,18136
  r6 = uint64_t(int64_t(int64_t(r6) + 18136));
  // stw r28,128(r1)
  guest::StoreU32(uint32_t(r1) + 128, uint32_t(r28));
  // addi r7,r7,24408
  r7 = uint64_t(int64_t(int64_t(r7) + 24408));
  // stw r29,132(r1)
  guest::StoreU32(uint32_t(r1) + 132, uint32_t(r29));
  // addi r8,r8,18664
  r8 = uint64_t(int64_t(int64_t(r8) + 18664));
  // stw r30,136(r1)
  guest::StoreU32(uint32_t(r1) + 136, uint32_t(r30));
  // addi r9,r9,24768
  r9 = uint64_t(int64_t(int64_t(r9) + 24768));
  // stw r4,140(r1)
  guest::StoreU32(uint32_t(r1) + 140, uint32_t(r4));
  // stw r5,144(r1)
  guest::StoreU32(uint32_t(r1) + 144, uint32_t(r5));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // stw r6,148(r1)
  guest::StoreU32(uint32_t(r1) + 148, uint32_t(r6));
  // stw r7,152(r1)
  guest::StoreU32(uint32_t(r1) + 152, uint32_t(r7));
  // stw r8,156(r1)
  guest::StoreU32(uint32_t(r1) + 156, uint32_t(r8));
  // stw r9,160(r1)
  guest::StoreU32(uint32_t(r1) + 160, uint32_t(r9));
  // beq cr6,0x822764f0
  if (cr6.eq) goto loc_822764F0;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x822764cc
  if (cr6.lt) goto loc_822764CC;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwzx r11,r10,r9
  r11 = guest::LoadU32(uint32_t(r10) + uint32_t(r9));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82276488, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24940(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24940);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82276504
  if (cr0.eq) goto loc_82276504;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x822764b0
  if (!cr6.eq) goto loc_822764B0;
  // lwz r8,4(r11)
  r8 = guest::LoadU32(uint32_t(r11) + 4);
  // stw r8,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r8));
  // b 0x822764c0
  goto loc_822764C0;
loc_822764B0:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_822764B4:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 16);
  // stw r8,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r10,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r10));
loc_822764C0:
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
loc_822764C4:
  // stw r10,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r10));
  // b 0x82276504
  goto loc_82276504;
loc_822764CC:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x822764b4
  if (!cr6.eq) goto loc_822764B4;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 4);
  // stw r8,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x822764c4
  goto loc_822764C4;
loc_822764F0:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x82276504
  if (cr6.eq) goto loc_82276504;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82276504, r31)); r3 = o.r3; f1 = o.f1; }
loc_82276504:
  // bl 0x822750a0
  // (left out: dead in C++)
  { const lifted::Result o = f822750A0(r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82276E50: lifted from generated/ by lift.py.
void f82276E50(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82276E50 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9640(r3)
  guest::StoreU32(uint32_t(r3) + 9640, uint32_t(r11));
  // blr
  return;
}

// sub_82277018: lifted from generated/ by lift.py.
void f82277018(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82277018 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8227705c
  if (cr6.eq) goto loc_8227705C;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24968(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24968);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8227703c
  if (cr0.eq) goto loc_8227703C;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_8227703C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r10,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r10));
  // stw r9,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r9));
  // stw r10,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r10));
  // stw r8,9604(r3)
  guest::StoreU32(uint32_t(r3) + 9604, uint32_t(r8));
  // blr
  return;
loc_8227705C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_82277078: lifted from generated/ by lift.py.
lifted::Result f82277078(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82277078 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,41
  r11 = uint64_t(int64_t(41));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x8227709C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishAll(LIFTED_ARGS(0x822770A4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82277178: lifted from generated/ by lift.py.
lifted::Result f82277178(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82277178 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r29,-32127
  r29 = uint64_t(int64_t(-2105475072));
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r3,24972(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 24972);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(LIFTED_ARGS(0x82277198, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,24972(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 24972);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x822771A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82277214
  if (cr0.eq) goto loc_82277214;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x822771B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24972(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24972);
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r9,39
  r9 = uint64_t(int64_t(39));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_count_bits();
  // lwz r6,604(r11)
  r6 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r7));
  // beq cr6,0x82277278
  if (cr6.eq) goto loc_82277278;
  // b 0x82277274
  goto loc_82277274;
loc_82277214:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,24972(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 24972);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82277220, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82277284
  if (cr0.eq) goto loc_82277284;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82277230, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_count_bits();
  // lwz r6,604(r11)
  r6 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r7));
loc_82277274:
  // li r9,40
  r9 = uint64_t(int64_t(40));
loc_82277278:
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r28,9640(r30)
  guest::StoreU32(schema::NetMachine(uint32_t(r30)).unknown_9640_address(), uint32_t(r28));
  // b 0x822772b4
  goto loc_822772B4;
loc_82277284:
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // lwz r3,24972(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 24972);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(LIFTED_ARGS(0x82277290, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822772b4
  if (cr0.eq) goto loc_822772B4;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r3,24896(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 24896);
  // bl 0x8225fcb8
  // (left out: dead in C++)
  r3 = f8225FCB8(r3);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r3
  r4 = r3;
  // lwz r3,-21596(r11)
  r3 = guest::LoadU32(uint32_t(r11) + -21596);
  // bl 0x822952d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XShowGamerCardUI(r1, r13, r3, r4, r6, r7, r8, r9, r10, 0x822772B4); r3 = o.r3; f1 = o.f1; }
loc_822772B4:
  // lwz r11,9640(r30)
  r11 = guest::LoadU32(schema::NetMachine(uint32_t(r30)).unknown_9640_address());
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x822772f8
  if (!cr6.eq) goto loc_822772F8;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x822772D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24968(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24968);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x822772e4
  if (cr6.eq) goto loc_822772E4;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_822772E4:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r30,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r30));
  // stw r28,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r28));
  // stw r30,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r30));
  // stw r10,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r10));
loc_822772F8:
  // lwz r11,24972(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 24972);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82277338
  if (cr6.eq) goto loc_82277338;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82277310, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24968(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24968);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82277324
  if (cr0.eq) goto loc_82277324;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_82277324:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r28,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r28));
  // stw r30,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r30));
  // stw r30,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r30));
  // stw r10,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r10));
loc_82277338:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82277340: lifted from generated/ by lift.py.
uint64_t f82277340(uint64_t r1, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82277340 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  // std r30,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  guest::StoreU64(uint32_t(r1) + -8, r31);
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r9,31
  r9 = uint64_t(int64_t(31));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_count_bits();
  // li r6,38
  r6 = uint64_t(int64_t(38));
  // lwz r3,604(r11)
  r3 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_start_bits();
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
  schema::ModeDispatcher(uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,4(r10)
  guest::StoreU32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r7,8(r10)
  guest::StoreU32(uint32_t(r10) + 8, uint32_t(r7));
  // lwz r10,600(r11)
  r10 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_count_bits();
  // lwz r9,604(r11)
  r9 = schema::ModeDispatcher(uint32_t(r11) - 204).ring_start_bits();
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  schema::ModeDispatcher(uint32_t(r11) - 204).set_ring_count_bits(uint32_t(r10));
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
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r6));
  // stw r5,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r5));
  // stw r4,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r4));
  // ld r30,-16(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -16);
  // ld r31,-8(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -8);
  // blr
  return r3;
}

// sub_822773E8: lifted from generated/ by lift.py.
lifted::Result f822773E8(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822773E8 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r23,r4
  r23 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,41
  r11 = uint64_t(int64_t(41));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82277410, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82277764
  if (!cr6.eq) goto loc_82277764;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // li r6,36
  r6 = uint64_t(int64_t(36));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // lwz r30,24896(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 24896);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8227743C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,23
  r5 = uint64_t(int64_t(23));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82277450, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,3
  r6 = uint64_t(int64_t(3));
  // li r5,26
  r5 = uint64_t(int64_t(26));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82277464, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,317
  r6 = uint64_t(int64_t(317));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82277478, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,318
  r6 = uint64_t(int64_t(318));
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8227748C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,319
  r6 = uint64_t(int64_t(319));
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822774A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r24,-32127
  r24 = uint64_t(int64_t(-2105475072));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24848(r24)
  r11 = guest::LoadU32(uint32_t(r24) + 24848);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x822774e0
  if (!cr6.eq) goto loc_822774E0;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x822774C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x822774D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // b 0x82277500
  goto loc_82277500;
loc_822774E0:
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x822774E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x822774FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2
  r6 = uint64_t(int64_t(2));
loc_82277500:
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeColor(LIFTED_ARGS(0x82277510, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x8225fb88
  // (left out: dead in C++)
  f8225FB88(r1, r3, r4);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x8227752C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c2c8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetCharText(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8225fd18
  // (left out: dead in C++)
  r3 = f8225FD18(r3);
  // bl 0x8225ffc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8225FFC8, LIFTED_ARGS(0x8227754C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a708
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetNodeTexture(LIFTED_ARGS(0x82277560, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8225fe30
  // (left out: dead in C++)
  r3 = f8225FE30(r3);
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x8225ff00
  // (left out: dead in C++)
  r3 = f8225FF00(r3);
  // lis r11,152
  r11 = uint64_t(int64_t(9961472));
  // mr r27,r3
  r27 = r3;
  // ori r11,r11,38527
  r11 = r11 | 38527;
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // mr r25,r11
  r25 = r11;
  // bgt cr6,0x82277590
  if (cr6.gt) goto loc_82277590;
  // mr r25,r29
  r25 = r29;
loc_82277590:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x822775a4
  if (cr6.eq) goto loc_822775A4;
  // divwu r26,r29,r27
  r26 = uint32_t(uint32_t(r27) ? uint32_t(r29) / uint32_t(r27) : 0);
  // twllei r27,0
  if (int32_t(r27) == 0 || uint32_t(r27) < 0u) lifted::Trap(0);
  // b 0x822775a8
  goto loc_822775A8;
loc_822775A4:
  // li r26,0
  r26 = uint64_t(int64_t(0));
loc_822775A8:
  // mr r3,r30
  r3 = r30;
  // bl 0x8225fe98
  // (left out: dead in C++)
  r3 = f8225FE98(r3);
  // cmpwi cr6,r3,9999
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 9999, so);
  // ble cr6,0x822775c0
  if (!cr6.gt) goto loc_822775C0;
  // li r28,9999
  r28 = uint64_t(int64_t(9999));
  // b 0x822775cc
  goto loc_822775CC;
loc_822775C0:
  // mr r3,r30
  r3 = r30;
  // bl 0x8225fe98
  // (left out: dead in C++)
  r3 = f8225FE98(r3);
  // mr r28,r3
  r28 = r3;
loc_822775CC:
  // cmplwi cr6,r27,9999
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 9999, so);
  // li r29,9999
  r29 = uint64_t(int64_t(9999));
  // bgt cr6,0x822775dc
  if (cr6.gt) goto loc_822775DC;
  // mr r29,r27
  r29 = r27;
loc_822775DC:
  // mr r7,r25
  r7 = r25;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822775F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r7,r26
  r7 = r26;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x8227760C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r29
  r8 = r29;
  // mr r7,r28
  r7 = r28;
  // li r6,325
  r6 = uint64_t(int64_t(325));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82277628, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24848(r24)
  r11 = guest::LoadU32(uint32_t(r24) + 24848);
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x82277668
  if (!cr6.eq) goto loc_82277668;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82277644, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82277654, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac00
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::ShowNode(LIFTED_ARGS(0x82277664, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8227768c
  goto loc_8227768C;
loc_82277668:
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8227766C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8227767C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ac48
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::HideNode(LIFTED_ARGS(0x8227768C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8227768C:
  // li r11,395
  r11 = uint64_t(int64_t(395));
  // mr r3,r30
  r3 = r30;
  // stw r11,112(r1)
  guest::StoreU32(uint32_t(r1) + 112, uint32_t(r11));
  // li r11,396
  r11 = uint64_t(int64_t(396));
  // stw r11,116(r1)
  guest::StoreU32(uint32_t(r1) + 116, uint32_t(r11));
  // li r11,397
  r11 = uint64_t(int64_t(397));
  // stw r11,120(r1)
  guest::StoreU32(uint32_t(r1) + 120, uint32_t(r11));
  // li r11,398
  r11 = uint64_t(int64_t(398));
  // stw r11,124(r1)
  guest::StoreU32(uint32_t(r1) + 124, uint32_t(r11));
  // li r11,401
  r11 = uint64_t(int64_t(401));
  // stw r11,128(r1)
  guest::StoreU32(uint32_t(r1) + 128, uint32_t(r11));
  // li r11,402
  r11 = uint64_t(int64_t(402));
  // stw r11,132(r1)
  guest::StoreU32(uint32_t(r1) + 132, uint32_t(r11));
  // li r11,420
  r11 = uint64_t(int64_t(420));
  // stw r11,136(r1)
  guest::StoreU32(uint32_t(r1) + 136, uint32_t(r11));
  // bl 0x8225fd78
  // (left out: dead in C++)
  r3 = f8225FD78(r3);
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // li r5,30
  r5 = uint64_t(int64_t(30));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // lwzx r6,r11,r10
  r6 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x822776EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8225fdd0
  // (left out: dead in C++)
  r3 = f8225FDD0(r3);
  // bl 0x822601b0
  // (left out: dead in C++)
  r3 = f822601B0(r3);
  // mr r11,r3
  r11 = r3;
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,31
  r5 = uint64_t(int64_t(31));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // addi r7,r11,1
  r7 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82277714, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8225ff68
  // (left out: dead in C++)
  r3 = f8225FF68(r3);
  // mr r11,r3
  r11 = r3;
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // addi r11,r11,0
  r11 = uint64_t(int64_t(int64_t(r11) + 0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // mr r3,r31
  r3 = r31;
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r6,r11,1
  r6 = r11 ^ 1;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::SetText(LIFTED_ARGS(0x82277740, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8227775C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r23)
  guest::StoreU32(uint32_t(r23) + 0, uint32_t(r11));
loc_82277764:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82277770, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82277780: lifted from generated/ by lift.py.
lifted::Result f82277780(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82277780 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r4
  r29 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,41
  r11 = uint64_t(int64_t(41));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822777A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82277814
  if (!cr6.eq) goto loc_82277814;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822777C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(LIFTED_ARGS(0x822777D0, r31)); r3 = o.r3; f1 = o.f1; }
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
  // lwz r11,24972(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24972);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x822777F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24972(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 24972);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // addi r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(LIFTED_ARGS(0x8227780C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  guest::StoreU32(uint32_t(r29) + 0, uint32_t(r11));
loc_82277814:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x82277820, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82277830: lifted from generated/ by lift.py.
lifted::Result f82277830(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82277830 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,41
  r11 = uint64_t(int64_t(41));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82277860, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82277894
  if (!cr6.eq) goto loc_82277894;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(LIFTED_ARGS(0x8227788C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
loc_82277894:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822778A0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_822778C0: lifted from generated/ by lift.py.
uint64_t f822778C0(uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822778C0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,41
  r11 = uint64_t(int64_t(41));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x822778EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9640(r31)
  r11 = schema::NetModeBlockAt86880(uint32_t(r31)).unknown_9640_bits();
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82277910
  if (cr6.eq) goto loc_82277910;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8211bc18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8211BC18, LIFTED_ARGS(0x82277904, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq 0x82277914
  if (cr0.eq) goto loc_82277914;
loc_82277910:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82277914:
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

// sub_82277928: lifted from generated/ by lift.py.
lifted::Result f82277928(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82277928 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24968(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24968);
  // lwz r11,0(r11)
  r11 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82277944
  if (cr6.lt) goto loc_82277944;
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x82277340
  r3 = f82277340(r1, r4);
  return {r3, f1};
loc_82277944:
  // b 0x82277178
  { const lifted::Result o = f82277178(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82277950: lifted from generated/ by lift.py.
lifted::Result f82277950(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82277950 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,41
  r11 = uint64_t(int64_t(41));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(LIFTED_ARGS(0x82277980, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // lwz r11,24968(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24968);
  // addi r4,r11,24
  r4 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwz r10,12(r11)
  r10 = schema::PageObject(uint32_t(r11)).step_bits();
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x822779a8
  if (!cr6.eq) goto loc_822779A8;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = guest::LoadU32(schema::UiMovieTableValue(uint32_t(r3)).value_address());
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(LIFTED_ARGS(0x822779A4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82277a40
  goto loc_82277A40;
loc_822779A8:
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x82277a1c
  if (cr6.lt) goto loc_82277A1C;
  // beq cr6,0x82277a10
  if (cr6.eq) goto loc_82277A10;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x822779e0
  if (cr6.lt) goto loc_822779E0;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82277a2c
  if (!cr6.eq) goto loc_82277A2C;
  // lwz r9,4(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 4);
  // stw r8,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x82277a3c
  goto loc_82277A3C;
loc_822779E0:
  // mr r3,r30
  r3 = r30;
  // bl 0x82277830
  // (left out: dead in C++)
  { const lifted::Result o = f82277830(r1, r13, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_822779E8:
  // lwz r11,24968(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24968);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82277a40
  if (cr0.eq) goto loc_82277A40;
  // lwz r9,16(r11)
  r9 = schema::PageObject(uint32_t(r11)).queued_step_bits();
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x82277a28
  if (!cr6.eq) goto loc_82277A28;
  // lwz r8,4(r11)
  r8 = schema::PageObject(uint32_t(r11)).next_page_bits();
  // stw r8,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r8));
  // b 0x82277a38
  goto loc_82277A38;
loc_82277A10:
  // mr r3,r30
  r3 = r30;
  // bl 0x82277780
  // (left out: dead in C++)
  { const lifted::Result o = f82277780(r1, r13, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822779e8
  goto loc_822779E8;
loc_82277A1C:
  // mr r3,r30
  r3 = r30;
  // bl 0x822773e8
  // (left out: dead in C++)
  { const lifted::Result o = f822773E8(r1, r13, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x822779e8
  goto loc_822779E8;
loc_82277A28:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_82277A2C:
  // lwz r9,16(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 16);
  // stw r10,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r10));
  // stw r8,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r8));
loc_82277A38:
  // stw r9,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r9));
loc_82277A3C:
  // stw r10,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r10));
loc_82277A40:
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

// sub_82277A58: lifted from generated/ by lift.py.
lifted::Result f82277A58(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82277A58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r11,r3
  r11 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // stw r31,9624(r11)
  guest::StoreU32(uint32_t(r11) + 9624, uint32_t(r31));
  // stw r31,9628(r11)
  guest::StoreU32(uint32_t(r11) + 9628, uint32_t(r31));
  // stw r31,9640(r11)
  guest::StoreU32(uint32_t(r11) + 9640, uint32_t(r31));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(LIFTED_ARGS(0x82277A84, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24968(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24968);
  // lwz r10,0(r11)
  r10 = schema::PageObject(uint32_t(r11)).page_bits();
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82277a9c
  if (cr0.eq) goto loc_82277A9C;
  // stw r10,8(r11)
  schema::PageObject(uint32_t(r11)).set_previous_page_bits(uint32_t(r10));
loc_82277A9C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r31,12(r11)
  schema::PageObject(uint32_t(r11)).set_step_bits(uint32_t(r31));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r31,4(r11)
  schema::PageObject(uint32_t(r11)).set_next_page_bits(uint32_t(r31));
  // stw r10,0(r11)
  schema::PageObject(uint32_t(r11)).set_page_bits(uint32_t(r10));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r9,16(r11)
  schema::PageObject(uint32_t(r11)).set_queued_step_bits(uint32_t(r9));
  // lwz r11,24972(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 24972);
  // stw r31,12(r11)
  guest::StoreU32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r9,16(r11)
  guest::StoreU32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r9,20(r11)
  guest::StoreU32(uint32_t(r11) + 20, uint32_t(r9));
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
  // lwz r11,24972(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 24972);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // stw r31,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
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

// sub_822785E0 game::ModeType12::Interrupt: lifted from generated/ by lift.py.
lifted::Result f822785E0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822785E0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // mr r27,r4
  r27 = r4;
  // mr r29,r28
  r29 = r28;
  // lwz r10,816(r31)
  r10 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // mr r3,r10
  r3 = r10;
  // bl 0x82277ee0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82277EE0, LIFTED_ARGS(0x82278608, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82278844
  if (cr0.eq) goto loc_82278844;
  // mr r3,r10
  r3 = r10;
  // bl 0x82277ff0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82277FF0, LIFTED_ARGS(0x82278618, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // ori r10,r11,38524
  r10 = r11 | 38524;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // lwz r11,24984(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24984);
  // stw r9,32(r11)
  schema::NetModeSelectData(uint32_t(r11)).set_sign_in_continues_bits(uint32_t(r9));
  // lwz r11,816(r31)
  r11 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // lwzx r10,r11,r10
  r10 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bne cr6,0x822786a8
  if (!cr6.eq) goto loc_822786A8;
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // lwz r10,25040(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 25040);
  // lwz r10,12(r10)
  r10 = schema::NetRankingData(uint32_t(r10)).unknown_12_bits();
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x822786a8
  if (!cr6.eq) goto loc_822786A8;
  // addi r11,r31,208
  r11 = uint64_t(int64_t(int64_t(r31) + 208));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,43
  r9 = uint64_t(int64_t(43));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = uint32_t(schema::ModeType12(uint32_t(r11) - 208).load_at(808, 4));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r6,604(r11)
  r6 = uint32_t(schema::ModeType12(uint32_t(r11) - 208).load_at(812, 4));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeType12(uint32_t(r11) - 208).store_at(808, 4, uint32_t(r10));
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
  // b 0x822786dc
  goto loc_822786DC;
loc_822786A8:
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,21300
  r10 = r10 | 21300;
  // lwzx r10,r11,r10
  r10 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bne cr6,0x822786e4
  if (!cr6.eq) goto loc_822786E4;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_822786C0:
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // lwz r3,816(r31)
  r3 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // ori r11,r11,4108
  r11 = r11 | 4108;
  // lwzx r4,r3,r11
  r4 = guest::LoadU32(uint32_t(r3) + uint32_t(r11));
  // bl 0x82277e10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82277E10, LIFTED_ARGS(0x822786D4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,9636(r3)
  guest::StoreU32(schema::NetMachine(uint32_t(r3)).interrupted_address(), uint32_t(r28));
loc_822786D8:
  // mr r3,r29
  r3 = r29;
loc_822786DC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_822786E4:
  // lis r10,2
  r10 = uint64_t(int64_t(131072));
  // ori r10,r10,4068
  r10 = r10 | 4068;
  // lwzx r10,r11,r10
  r10 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bne cr6,0x82278758
  if (!cr6.eq) goto loc_82278758;
  // addi r11,r31,208
  r11 = uint64_t(int64_t(int64_t(r31) + 208));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = uint32_t(schema::ModeType12(uint32_t(r11) - 208).load_at(808, 4));
  // lwz r6,604(r11)
  r6 = uint32_t(schema::ModeType12(uint32_t(r11) - 208).load_at(812, 4));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeType12(uint32_t(r11) - 208).store_at(808, 4, uint32_t(r10));
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
  // lwz r11,816(r31)
  r11 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // addis r3,r11,2
  r3 = uint64_t(int64_t(int64_t(r11) + 131072));
  // addi r3,r3,-5536
  r3 = uint64_t(int64_t(int64_t(r3) + -5536));
  // bl 0x8227e7c8
  // (left out: dead in C++)
  r3 = f8227E7C8(r1, r13, r3);
  // b 0x8227883c
  goto loc_8227883C;
loc_82278758:
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,30948
  r10 = r10 | 30948;
  // lwzx r10,r11,r10
  r10 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bne cr6,0x822787cc
  if (!cr6.eq) goto loc_822787CC;
  // addi r11,r31,208
  r11 = uint64_t(int64_t(int64_t(r31) + 208));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = uint32_t(schema::ModeType12(uint32_t(r11) - 208).load_at(808, 4));
  // lwz r6,604(r11)
  r6 = uint32_t(schema::ModeType12(uint32_t(r11) - 208).load_at(812, 4));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeType12(uint32_t(r11) - 208).store_at(808, 4, uint32_t(r10));
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
  // lwz r11,816(r31)
  r11 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // addis r3,r11,1
  r3 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r3,r3,21344
  r3 = uint64_t(int64_t(int64_t(r3) + 21344));
  // bl 0x822778c0
  // (left out: dead in C++)
  r3 = f822778C0(r1, r13, r3);
  // b 0x8227883c
  goto loc_8227883C;
loc_822787CC:
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,59956
  r10 = r10 | 59956;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // beq cr6,0x82278850
  if (cr6.eq) goto loc_82278850;
  // addi r11,r31,208
  r11 = uint64_t(int64_t(int64_t(r31) + 208));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,37
  r9 = uint64_t(int64_t(37));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = uint32_t(schema::ModeType12(uint32_t(r11) - 208).load_at(808, 4));
  // lwz r6,604(r11)
  r6 = uint32_t(schema::ModeType12(uint32_t(r11) - 208).load_at(812, 4));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  schema::ModeType12(uint32_t(r11) - 208).store_at(808, 4, uint32_t(r10));
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
  // lwz r11,816(r31)
  r11 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // addis r3,r11,2
  r3 = uint64_t(int64_t(int64_t(r11) + 131072));
  // addi r3,r3,-15184
  r3 = uint64_t(int64_t(int64_t(r3) + -15184));
  // bl 0x8227f480
  // (left out: dead in C++)
  r3 = f8227F480(r1, r13, r3);
loc_8227883C:
  // mr r29,r3
  r29 = r3;
  // b 0x82278848
  goto loc_82278848;
loc_82278844:
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_82278848:
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // bne cr6,0x822786c0
  if (!cr6.eq) goto loc_822786C0;
loc_82278850:
  // lis r10,2
  r10 = uint64_t(int64_t(131072));
  // lwz r11,816(r31)
  r11 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // ori r30,r10,4108
  r30 = r10 | 4108;
  // lwzx r11,r11,r30
  r11 = guest::LoadU32(uint32_t(r11) + uint32_t(r30));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x82278870
  if (cr6.eq) goto loc_82278870;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // bne cr6,0x82278874
  if (!cr6.eq) goto loc_82278874;
loc_82278870:
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82278874:
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // beq cr6,0x822788a0
  if (cr6.eq) goto loc_822788A0;
  // cmpwi cr6,r27,1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 1, so);
  // beq cr6,0x822788a0
  if (cr6.eq) goto loc_822788A0;
  // cmpwi cr6,r27,4
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 4, so);
  // beq cr6,0x822788a0
  if (cr6.eq) goto loc_822788A0;
  // lwz r3,816(r31)
  r3 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // lwzx r4,r3,r30
  r4 = guest::LoadU32(uint32_t(r3) + uint32_t(r30));
  // bl 0x82277e10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82277E10, LIFTED_ARGS(0x82278898, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,9632(r3)
  guest::StoreU32(schema::NetMachine(uint32_t(r3)).again_address(), uint32_t(r28));
  // b 0x822788c0
  goto loc_822788C0;
loc_822788A0:
  // lwz r3,816(r31)
  r3 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // lwzx r4,r3,r30
  r4 = guest::LoadU32(uint32_t(r3) + uint32_t(r30));
  // bl 0x82277e10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82277E10, LIFTED_ARGS(0x822788AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // stw r28,9624(r3)
  guest::StoreU32(schema::NetMachine(uint32_t(r3)).back_address(), uint32_t(r28));
  // lwz r10,816(r31)
  r10 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // ori r11,r11,4132
  r11 = r11 | 4132;
  // stwx r28,r10,r11
  guest::StoreU32(uint32_t(r10) + uint32_t(r11), uint32_t(r28));
loc_822788C0:
  // lwz r3,816(r31)
  r3 = uint32_t(schema::ModeType12(uint32_t(r31)).load_at(816, 4));
  // lwzx r4,r3,r30
  r4 = guest::LoadU32(uint32_t(r3) + uint32_t(r30));
  // bl 0x82277e10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82277E10, LIFTED_ARGS(0x822788CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9636(r3)
  guest::StoreU32(schema::NetMachine(uint32_t(r3)).interrupted_address(), uint32_t(r11));
  // b 0x822786d8
  goto loc_822786D8;
}

lifted::Result e8226E8B8(const lifted::Args& a) {
  return f8226E8B8(a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226EA88(const lifted::Args& a) {
  return f8226EA88(a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226EEC8(const lifted::Args& a) {
  return f8226EEC8(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226F470(const lifted::Args& a) {
  return f8226F470(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226F708(const lifted::Args& a) {
  return f8226F708(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8226FE68(const lifted::Args& a) {
  return f8226FE68(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82270118(const lifted::Args& a) {
  return f82270118(a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82270200(const lifted::Args& a) {
  return f82270200(a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822702C0(const lifted::Args& a) {
  return f822702C0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82270528(const lifted::Args& a) {
  return f82270528(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82270790(const lifted::Args& a) {
  return {f82270790(a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e822707A8(const lifted::Args& a) {
  return {f822707A8(a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e822707C0(const lifted::Args& a) {
  return {f822707C0(a.r[1]), a.f[0]};
}

lifted::Result e822707C8(const lifted::Args& a) {
  return {f822707C8(a.r1, a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82270828(const lifted::Args& a) {
  return {f82270828(a.r1, a.r[1]), a.f[0]};
}

lifted::Result e82270848(const lifted::Args& a) {
  return f82270848(a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e822708C0(const lifted::Args& a) {
  return {f822708C0(a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82270A20(const lifted::Args& a) {
  return {f82270A20(a.r1, a.r13), a.f[0]};
}

lifted::Result e82270A80(const lifted::Args& a) {
  return {f82270A80(a.r1, a.r13), a.f[0]};
}

lifted::Result e82270B28(const lifted::Args& a) {
  return {f82270B28(a.r1, a.r13), a.f[0]};
}

lifted::Result e82270BF8(const lifted::Args& a) {
  return {f82270BF8(a.r1, a.r13), a.f[0]};
}

lifted::Result e82270CC8(const lifted::Args& a) {
  return {f82270CC8(a.r1, a.r13), a.f[0]};
}

lifted::Result e82270DC0(const lifted::Args& a) {
  return f82270DC0(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82270F40(const lifted::Args& a) {
  return f82270F40(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822710C0(const lifted::Args& a) {
  return f822710C0(a.r1, a.r13, a.f[0]);
}

lifted::Result e822712B8(const lifted::Args& a) {
  return f822712B8(a.r1, a.r13, a.f[0]);
}

lifted::Result e82271460(const lifted::Args& a) {
  return f82271460(a.r1, a.r13, a.f[0]);
}

lifted::Result e82271630(const lifted::Args& a) {
  return f82271630(a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82271718(const lifted::Args& a) {
  return f82271718(a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e822717D0(const lifted::Args& a) {
  return f822717D0(a.r1, a.r13, a.f[0]);
}

lifted::Result e822718C0(const lifted::Args& a) {
  return f822718C0(a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82271B30(const lifted::Args& a) {
  return {f82271B30(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82271BC0(const lifted::Args& a) {
  return {f82271BC0(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82271C78(const lifted::Args& a) {
  f82271C78();
  return {a.r[0], a.f[0]};
}

lifted::Result e82271CB0(const lifted::Args& a) {
  f82271CB0();
  return {a.r[0], a.f[0]};
}

lifted::Result e82271D50(const lifted::Args& a) {
  f82271D50(a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82271E58(const lifted::Args& a) {
  return f82271E58(a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82272460(const lifted::Args& a) {
  f82272460(a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82272490(const lifted::Args& a) {
  f82272490(a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822724C0(const lifted::Args& a) {
  return f822724C0(a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82272520(const lifted::Args& a) {
  return {f82272520(a.r1, a.r13), a.f[0]};
}

lifted::Result e82272620(const lifted::Args& a) {
  return f82272620(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822726C8(const lifted::Args& a) {
  return {f822726C8(a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82272968(const lifted::Args& a) {
  return f82272968(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82272B80(const lifted::Args& a) {
  return {f82272B80(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82272CE8(const lifted::Args& a) {
  f82272CE8(a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82272D70(const lifted::Args& a) {
  return f82272D70(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82272EE8(const lifted::Args& a) {
  return f82272EE8(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82273080(const lifted::Args& a) {
  return {f82273080(a.r1, a.r13), a.f[0]};
}

lifted::Result e82273128(const lifted::Args& a) {
  return f82273128(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82273418(const lifted::Args& a) {
  return f82273418(a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822735A8(const lifted::Args& a) {
  return f822735A8(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822738A0(const lifted::Args& a) {
  return {f822738A0(a.r1, a.r13), a.f[0]};
}

lifted::Result e82273AC0(const lifted::Args& a) {
  return f82273AC0(a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82273BF0(const lifted::Args& a) {
  return {f82273BF0(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82273C70(const lifted::Args& a) {
  return f82273C70(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82273D80(const lifted::Args& a) {
  return f82273D80(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82273E58(const lifted::Args& a) {
  return f82273E58(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82274048(const lifted::Args& a) {
  return f82274048(a.r1, a.r13, a.r[0], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82274120(const lifted::Args& a) {
  return f82274120(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82274378(const lifted::Args& a) {
  return f82274378(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82274428(const lifted::Args& a) {
  return f82274428(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82274538(const lifted::Args& a) {
  return {f82274538(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822745B8(const lifted::Args& a) {
  return f822745B8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822746D8(const lifted::Args& a) {
  return f822746D8(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e822748E8(const lifted::Args& a) {
  return f822748E8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822749E0(const lifted::Args& a) {
  return f822749E0(a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e82274A78(const lifted::Args& a) {
  return f82274A78(a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e82274AF8(const lifted::Args& a) {
  return f82274AF8(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82274B60(const lifted::Args& a) {
  return f82274B60(a.r1, a.r13, a.f[0]);
}

lifted::Result e82274C50(const lifted::Args& a) {
  return {f82274C50(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82274CC0(const lifted::Args& a) {
  return {f82274CC0(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82274D30(const lifted::Args& a) {
  return {f82274D30(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82274DA8(const lifted::Args& a) {
  return f82274DA8(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82274FF0(const lifted::Args& a) {
  return f82274FF0(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822750A0(const lifted::Args& a) {
  return f822750A0(a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82275278(const lifted::Args& a) {
  return f82275278(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822752F8(const lifted::Args& a) {
  return f822752F8(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822755D8(const lifted::Args& a) {
  return f822755D8(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82275740(const lifted::Args& a) {
  return f82275740(a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e822758D8(const lifted::Args& a) {
  return f822758D8(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82275B88(const lifted::Args& a) {
  return f82275B88(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82275E38(const lifted::Args& a) {
  return f82275E38(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82275F58(const lifted::Args& a) {
  return f82275F58(a.r1, a.r13, a.r[0], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822760C0(const lifted::Args& a) {
  return f822760C0(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82276248(const lifted::Args& a) {
  return f82276248(a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82276330(const lifted::Args& a) {
  return f82276330(a.r1, a.r13, a.f[0]);
}

lifted::Result e82276E50(const lifted::Args& a) {
  f82276E50(a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82277018(const lifted::Args& a) {
  f82277018(a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82277078(const lifted::Args& a) {
  return f82277078(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82277178(const lifted::Args& a) {
  return f82277178(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82277340(const lifted::Args& a) {
  return {f82277340(a.r1, a.r[1]), a.f[0]};
}

lifted::Result e822773E8(const lifted::Args& a) {
  return f822773E8(a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82277780(const lifted::Args& a) {
  return f82277780(a.r1, a.r13, a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82277830(const lifted::Args& a) {
  return f82277830(a.r1, a.r13, a.r[1], a.r[5], a.f[0]);
}

lifted::Result e822778C0(const lifted::Args& a) {
  return {f822778C0(a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82277928(const lifted::Args& a) {
  return f82277928(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82277950(const lifted::Args& a) {
  return f82277950(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82277A58(const lifted::Args& a) {
  return f82277A58(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822785E0(const lifted::Args& a) {
  return f822785E0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x8226E8B8, e8226E8B8},
    {0x8226EA88, e8226EA88},
    {0x8226EEC8, e8226EEC8},
    {0x8226F470, e8226F470},
    {0x8226F708, e8226F708},
    {0x8226FE68, e8226FE68},
    {0x82270118, e82270118},
    {0x82270200, e82270200},
    {0x822702C0, e822702C0},
    {0x82270528, e82270528},
    {0x82270790, e82270790},
    {0x822707A8, e822707A8},
    {0x822707C0, e822707C0},
    {0x822707C8, e822707C8},
    {0x82270828, e82270828},
    {0x82270848, e82270848},
    {0x822708C0, e822708C0},
    {0x82270A20, e82270A20},
    {0x82270A80, e82270A80},
    {0x82270B28, e82270B28},
    {0x82270BF8, e82270BF8},
    {0x82270CC8, e82270CC8},
    {0x82270DC0, e82270DC0},
    {0x82270F40, e82270F40},
    {0x822710C0, e822710C0},
    {0x822712B8, e822712B8},
    {0x82271460, e82271460},
    {0x82271630, e82271630},
    {0x82271718, e82271718},
    {0x822717D0, e822717D0},
    {0x822718C0, e822718C0},
    {0x82271B30, e82271B30},
    {0x82271BC0, e82271BC0},
    {0x82271C78, e82271C78},
    {0x82271CB0, e82271CB0},
    {0x82271D50, e82271D50},
    {0x82271E58, e82271E58},
    {0x82272460, e82272460},
    {0x82272490, e82272490},
    {0x822724C0, e822724C0},
    {0x82272520, e82272520},
    {0x82272620, e82272620},
    {0x822726C8, e822726C8},
    {0x82272968, e82272968},
    {0x82272B80, e82272B80},
    {0x82272CE8, e82272CE8},
    {0x82272D70, e82272D70},
    {0x82272EE8, e82272EE8},
    {0x82273080, e82273080},
    {0x82273128, e82273128},
    {0x82273418, e82273418},
    {0x822735A8, e822735A8},
    {0x822738A0, e822738A0},
    {0x82273AC0, e82273AC0},
    {0x82273BF0, e82273BF0},
    {0x82273C70, e82273C70},
    {0x82273D80, e82273D80},
    {0x82273E58, e82273E58},
    {0x82274048, e82274048},
    {0x82274120, e82274120},
    {0x82274378, e82274378},
    {0x82274428, e82274428},
    {0x82274538, e82274538},
    {0x822745B8, e822745B8},
    {0x822746D8, e822746D8},
    {0x822748E8, e822748E8},
    {0x822749E0, e822749E0},
    {0x82274A78, e82274A78},
    {0x82274AF8, e82274AF8},
    {0x82274B60, e82274B60},
    {0x82274C50, e82274C50},
    {0x82274CC0, e82274CC0},
    {0x82274D30, e82274D30},
    {0x82274DA8, e82274DA8},
    {0x82274FF0, e82274FF0},
    {0x822750A0, e822750A0},
    {0x82275278, e82275278},
    {0x822752F8, e822752F8},
    {0x822755D8, e822755D8},
    {0x82275740, e82275740},
    {0x822758D8, e822758D8},
    {0x82275B88, e82275B88},
    {0x82275E38, e82275E38},
    {0x82275F58, e82275F58},
    {0x822760C0, e822760C0},
    {0x82276248, e82276248},
    {0x82276330, e82276330},
    {0x82276E50, e82276E50},
    {0x82277018, e82277018},
    {0x82277078, e82277078},
    {0x82277178, e82277178},
    {0x82277340, e82277340},
    {0x822773E8, e822773E8},
    {0x82277780, e82277780},
    {0x82277830, e82277830},
    {0x822778C0, e822778C0},
    {0x82277928, e82277928},
    {0x82277950, e82277950},
    {0x82277A58, e82277A58},
    {0x822785E0, e822785E0},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_039)), true);

}  // namespace

}  // namespace lifted
