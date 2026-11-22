// Lift sweep batch 002 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/action_mappers.h"
#include "schema/app_start_up.h"
#include "schema/app_states.h"
#include "schema/archive.h"
#include "schema/file_layer.h"
#include "schema/files_and_saves.h"
#include "schema/frame_driver.h"
#include "schema/frame_passes.h"
#include "schema/framework.h"
#include "schema/heap.h"
#include "schema/save_interface.h"
#include "schema/scene_target.h"
#include "schema/system_services.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_002, true, "ITC",
              "Run lift sweep batch 002's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_820949D0: lifted from generated/ by lift.py.
lifted::Result f820949D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_820949D0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r3,76
  r3 = uint64_t(int64_t(int64_t(r3) + 76));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::RunLoopHandlerKey(base, uint32_t(r11)).key_address());
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8209a930
  // (left out: dead in C++)
  { const lifted::Result o = game::RequestVector::PushBack(base, LIFTED_ARGS(0x820949FC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82094A70: lifted from generated/ by lift.py.
lifted::Result f82094A70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82094A70 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r3,76
  r3 = uint64_t(int64_t(int64_t(r3) + 76));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::RunLoopHandlerKey(base, uint32_t(r11)).key_address());
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8209a930
  // (left out: dead in C++)
  { const lifted::Result o = game::RequestVector::PushBack(base, LIFTED_ARGS(0x82094A9C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82094B68: lifted from generated/ by lift.py.
lifted::Result f82094B68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82094B68 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3768
  r11 = uint64_t(int64_t(int64_t(r11) + 3768));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::VtableObject(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x82094b94
  if (cr0.eq) goto loc_82094B94;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82094B94, r31)); r3 = o.r3; f1 = o.f1; }
loc_82094B94:
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

// sub_82094BB0: lifted from generated/ by lift.py.
lifted::Result f82094BB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82094BB0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r3,0(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82094BD0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82094BE8: lifted from generated/ by lift.py.
lifted::Result f82094BE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82094BE8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r5
  r28 = r5;
  // mr r30,r3
  r30 = r3;
  // mr r27,r4
  r27 = r4;
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82094c14
  if (cr0.eq) goto loc_82094C14;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82094c78
  if (!cr6.eq) goto loc_82094C78;
loc_82094C14:
  // addi r31,r30,8
  r31 = uint64_t(int64_t(int64_t(r30) + 8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82094C2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // addi r29,r30,256
  r29 = uint64_t(int64_t(int64_t(r30) + 256));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // mr r4,r29
  r4 = r29;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82094C4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x82094b10
  // (left out: dead in C++)
  { const lifted::Result o = game::Job::Wait(base, LIFTED_ARGS(0x82094C54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82094c78
  if (!cr6.eq) goto loc_82094C78;
  // lwz r11,0(r27)
  r11 = lifted::VtableWord(base, uint32_t(r27));
  // addi r4,r30,20
  r4 = uint64_t(int64_t(int64_t(r30) + 20));
  // mr r3,r27
  r3 = r27;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82094C78, r31)); r3 = o.r3; f1 = o.f1; }
loc_82094C78:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82094C80: lifted from generated/ by lift.py.
lifted::Result f82094C80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82094C80 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // b 0x82094cac
  goto loc_82094CAC;
loc_82094C98:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8258e540
  // (left out: dead in C++)
  { const lifted::Result o = f8258E540(base, r1, r13, r4, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_82094CAC:
  // lwz r11,248(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 248);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82094c98
  if (!cr6.eq) goto loc_82094C98;
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

// sub_82094CD0: lifted from generated/ by lift.py.
lifted::Result f82094CD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82094CD0 (lifted)");
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
  // bl 0x8258e530
  // (left out: dead in C++)
  f8258E530(base, r3);
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82094d00
  if (cr0.eq) goto loc_82094D00;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82094D00, r31)); r3 = o.r3; f1 = o.f1; }
loc_82094D00:
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

// sub_82094D20: lifted from generated/ by lift.py.
lifted::Result f82094D20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82094D20 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::ArchiveFunctorBase(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x82094d4c
  if (cr0.eq) goto loc_82094D4C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82094D4C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82094D4C:
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

// sub_82094D68: lifted from generated/ by lift.py.
lifted::Result f82094D68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82094D68 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3804
  r11 = uint64_t(int64_t(int64_t(r11) + 3804));
  // addi r30,r31,36
  r30 = uint64_t(int64_t(int64_t(r31) + 36));
  // mr r3,r30
  r3 = r30;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x82094D98, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82094DA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r11,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r11));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82094DB0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x8258dd60
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x82094DB8); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
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

// sub_82094DE0: lifted from generated/ by lift.py.
lifted::Result f82094DE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82094DE0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x82094d68
  // (left out: dead in C++)
  { const lifted::Result o = f82094D68(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82094e10
  if (cr0.eq) goto loc_82094E10;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82094E10, r31)); r3 = o.r3; f1 = o.f1; }
loc_82094E10:
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

// sub_82094E30: lifted from generated/ by lift.py.
lifted::Result f82094E30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82094E30 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r30,r31,8
  r30 = uint64_t(int64_t(int64_t(r31) + 8));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82094E4C); r3 = o.r3; f1 = o.f1; }
  // lwz r11,44(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 44);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82094e8c
  if (!cr0.eq) goto loc_82094E8C;
  // addi r29,r31,4
  r29 = uint64_t(int64_t(int64_t(r31) + 4));
loc_82094E5C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82094E64); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r29
  r3 = r29;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8258dd90
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Wait(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82094E80); r3 = o.r3; f1 = o.f1; }
  // lwz r11,44(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 44);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82094e5c
  if (cr0.eq) goto loc_82094E5C;
loc_82094E8C:
  // addi r4,r31,36
  r4 = uint64_t(int64_t(int64_t(r31) + 36));
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, uint32_t(r4) + 4);
  // rotlwi r10,r11,0
  r10 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82094ea8
  if (!cr6.eq) goto loc_82094EA8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82094EA8:
  // lwz r31,8(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 8);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, uint32_t(r4) + 4);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r4,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r4));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // ld r5,0(r10)
  r5 = guest::LoadU64(base, uint32_t(r10) + 0);
  // bl 0x82098e80
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Erase(base, LIFTED_ARGS(0x82094ECC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82094ED4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82094EE0: lifted from generated/ by lift.py.
lifted::Result f82094EE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82094EE0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3804
  r11 = uint64_t(int64_t(int64_t(r11) + 3804));
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::WorkerPoolAt28(base, uint32_t(r31)).unknown_0_address(), uint32_t(r11));
  // bl 0x8258dcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Create(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // bl 0x8258e548
  // (left out: dead in C++)
  { const lifted::Result o = game::InitializeCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,36
  r3 = uint64_t(int64_t(int64_t(r31) + 36));
  // bl 0x820ad058
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Ctor(base, LIFTED_ARGS(0x82094F18, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82094F30: lifted from generated/ by lift.py.
lifted::Result f82094F30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82094F30 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // mr r30,r3
  r30 = r3;
  // addi r11,r11,2740
  r11 = uint64_t(int64_t(int64_t(r11) + 2740));
  // addi r10,r10,3768
  r10 = uint64_t(int64_t(int64_t(r10) + 3768));
  // addi r9,r9,3852
  r9 = uint64_t(int64_t(int64_t(r9) + 3852));
  // addi r8,r8,3844
  r8 = uint64_t(int64_t(int64_t(r8) + 3844));
  // addi r7,r7,3816
  r7 = uint64_t(int64_t(int64_t(r7) + 3816));
  // mr r28,r4
  r28 = r4;
  // stw r11,4(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).vtable_4_address(), uint32_t(r11));
  // stw r10,8(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).vtable_8_address(), uint32_t(r10));
  // addi r27,r30,20
  r27 = uint64_t(int64_t(int64_t(r30) + 20));
  // stw r9,0(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).vtable_address(), uint32_t(r9));
  // addi r3,r30,24
  r3 = uint64_t(int64_t(int64_t(r30) + 24));
  // stw r8,4(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).vtable_4_address(), uint32_t(r8));
  // stw r7,8(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).vtable_8_address(), uint32_t(r7));
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, schema::WorkerPoolBlock(base, uint32_t(r28)).framework_interface_address());
  // stw r11,12(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).framework_interface_address(), uint32_t(r11));
  // lwz r11,8(r28)
  r11 = guest::LoadU32(base, schema::WorkerPoolBlock(base, uint32_t(r28)).unknown_8_address());
  // stw r11,16(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).unknown_16_address(), uint32_t(r11));
  // lwz r11,12(r28)
  r11 = guest::LoadU32(base, schema::WorkerPoolBlock(base, uint32_t(r28)).unknown_12_address());
  // stw r11,0(r27)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r27) - 20).unknown_20_address(), uint32_t(r11));
  // bl 0x8258dcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Create(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r30,28
  r3 = uint64_t(int64_t(int64_t(r30) + 28));
  // bl 0x82094ee0
  // (left out: dead in C++)
  { const lifted::Result o = f82094EE0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // addi r29,r30,76
  r29 = uint64_t(int64_t(int64_t(r30) + 76));
  // mr r31,r26
  r31 = r26;
loc_82094FB8:
  // mr r3,r29
  r3 = r29;
  // bl 0x820ad058
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Ctor(base, LIFTED_ARGS(0x82094FC0, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r29,r29,12
  r29 = uint64_t(int64_t(int64_t(r29) + 12));
  // bge 0x82094fb8
  if (!cr0.lt) goto loc_82094FB8;
  // addi r29,r30,100
  r29 = uint64_t(int64_t(int64_t(r30) + 100));
  // mr r31,r26
  r31 = r26;
loc_82094FD4:
  // mr r3,r29
  r3 = r29;
  // bl 0x8258dcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Create(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bge 0x82094fd4
  if (!cr0.lt) goto loc_82094FD4;
  // addi r29,r30,108
  r29 = uint64_t(int64_t(int64_t(r30) + 108));
  // mr r31,r26
  r31 = r26;
loc_82094FF0:
  // mr r3,r29
  r3 = r29;
  // bl 0x8258e548
  // (left out: dead in C++)
  { const lifted::Result o = game::InitializeCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r29,r29,28
  r29 = uint64_t(int64_t(int64_t(r29) + 28));
  // bge 0x82094ff0
  if (!cr0.lt) goto loc_82094FF0;
  // addi r25,r30,164
  r25 = uint64_t(int64_t(int64_t(r30) + 164));
  // li r31,3
  r31 = uint64_t(int64_t(3));
  // mr r29,r25
  r29 = r25;
loc_82095010:
  // mr r3,r29
  r3 = r29;
  // bl 0x8258de88
  // (left out: dead in C++)
  { const lifted::Result o = game::Thread::Ctor(base, LIFTED_ARGS(0x82095018, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r29,r29,20
  r29 = uint64_t(int64_t(int64_t(r29) + 20));
  // bge 0x82095010
  if (!cr0.lt) goto loc_82095010;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // stw r30,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r30));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r6,-32247
  r6 = uint64_t(int64_t(-2113339392));
  // lis r7,-32255
  r7 = uint64_t(int64_t(-2113863680));
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
  // lis r8,-32255
  r8 = uint64_t(int64_t(-2113863680));
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // addi r31,r30,256
  r31 = uint64_t(int64_t(int64_t(r30) + 256));
  // ld r3,80(r1)
  r3 = guest::LoadU64(base, uint32_t(r1) + 80);
  // addi r11,r11,2740
  r11 = uint64_t(int64_t(int64_t(r11) + 2740));
  // stw r29,248(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).unknown_248_address(), uint32_t(r29));
  // addi r7,r7,-31940
  r7 = uint64_t(int64_t(int64_t(r7) + -31940));
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // addi r10,r10,26332
  r10 = uint64_t(int64_t(int64_t(r10) + 26332));
  // addi r9,r9,10504
  r9 = uint64_t(int64_t(int64_t(r9) + 10504));
  // addi r6,r6,19432
  r6 = uint64_t(int64_t(int64_t(r6) + 19432));
  // addi r8,r8,-31940
  r8 = uint64_t(int64_t(int64_t(r8) + -31940));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_vtable_4_address(), uint32_t(r11));
  // stw r29,8(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_8_address(), uint32_t(r29));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r29,12(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_state_address(), uint32_t(r29));
  // mr r4,r27
  r4 = r27;
  // stb r29,16(r31)
  guest::StoreU8(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_16_address(), uint8_t(r29));
  // stb r26,17(r31)
  guest::StoreU8(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_17_address(), uint8_t(r26));
  // stb r29,18(r31)
  guest::StoreU8(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_18_address(), uint8_t(r29));
  // stw r10,0(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_vtable_address(), uint32_t(r10));
  // stw r9,4(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_vtable_4_address(), uint32_t(r9));
  // stw r7,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r7));
  // stw r6,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r6));
  // ld r7,80(r1)
  r7 = guest::LoadU64(base, uint32_t(r1) + 80);
  // stw r8,24(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_delegate_vtable_address(), uint32_t(r8));
  // stw r29,28(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_delegate_object_address(), uint32_t(r29));
  // std r3,32(r31)
  guest::StoreU64(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_delegate_function_address(), r3);
  // lwz r3,0(r28)
  r3 = guest::LoadU32(base, schema::WorkerPoolBlock(base, uint32_t(r28)).run_loop_address());
  // std r7,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r7);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::RunLoopVtable(base, uint32_t(r11)).add_listener_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820950CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32247
  r11 = uint64_t(int64_t(-2113339392));
  // lwz r10,20(r28)
  r10 = guest::LoadU32(base, schema::WorkerPoolBlock(base, uint32_t(r28)).thread_count_address());
  // mr r27,r29
  r27 = r29;
  // addi r11,r11,19584
  r11 = uint64_t(int64_t(int64_t(r11) + 19584));
  // lwz r9,132(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 132);
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // stw r10,244(r30)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r30)).thread_count_address(), uint32_t(r10));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // ld r8,80(r1)
  r8 = guest::LoadU64(base, uint32_t(r1) + 80);
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // stw r29,8(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_8_address(), uint32_t(r29));
  // stw r29,12(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_state_address(), uint32_t(r29));
  // stb r29,16(r31)
  guest::StoreU8(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_16_address(), uint8_t(r29));
  // stb r26,17(r31)
  guest::StoreU8(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_17_address(), uint8_t(r26));
  // stb r29,18(r31)
  guest::StoreU8(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_18_address(), uint8_t(r29));
  // stw r9,20(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_unknown_20_address(), uint32_t(r9));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // stw r30,28(r31)
  guest::StoreU32(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_delegate_object_address(), uint32_t(r30));
  // std r8,32(r31)
  guest::StoreU64(base, schema::WorkerPool(base, uint32_t(r31) - 256).job_delegate_function_address(), r8);
  // lwz r11,244(r30)
  r11 = guest::LoadU32(base, schema::WorkerPool(base, uint32_t(r30)).thread_count_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x8209515c
  if (!cr6.gt) goto loc_8209515C;
  // mr r31,r25
  r31 = r25;
loc_8209512C:
  // lwz r11,16(r28)
  r11 = guest::LoadU32(base, schema::WorkerPoolBlock(base, uint32_t(r28)).threads_address());
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // add r5,r29,r11
  r5 = r29 + r11;
  // addi r6,r5,4
  r6 = uint64_t(int64_t(int64_t(r5) + 4));
  // bl 0x8258df18
  // (left out: dead in C++)
  { const lifted::Result o = game::Thread::Begin(base, LIFTED_ARGS(0x82095144, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,244(r30)
  r11 = guest::LoadU32(base, schema::WorkerPool(base, uint32_t(r30)).thread_count_address());
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r29,r29,8
  r29 = uint64_t(int64_t(int64_t(r29) + 8));
  // addi r31,r31,20
  r31 = uint64_t(int64_t(int64_t(r31) + 20));
  // cmpw cr6,r27,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r11), so);
  // blt cr6,0x8209512c
  if (cr6.lt) goto loc_8209512C;
loc_8209515C:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82095168: lifted from generated/ by lift.py.
uint64_t f82095168(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82095168 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,240(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 240);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // bgt cr6,0x8209517c
  if (cr6.gt) goto loc_8209517C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8209517C:
  // clrlwi r3,r11,24
  r3 = uint32_t(r11) & 0xFF;
  // blr
  return r3;
}

// sub_82095188: lifted from generated/ by lift.py.
lifted::Result f82095188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82095188 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0;
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
  // addi r30,r31,100
  r30 = uint64_t(int64_t(int64_t(r31) + 100));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x820951AC); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r3,r30
  r3 = r30;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,244(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 244, temp.u32);
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x820951C0); r3 = o.r3; f1 = o.f1; }
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

// sub_82095208: lifted from generated/ by lift.py.
lifted::Result f82095208(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82095208 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82095fe0
  { const lifted::Result o = f82095FE0(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82095210: lifted from generated/ by lift.py.
lifted::Result f82095210(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82095210 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x82095fe0
  { const lifted::Result o = f82095FE0(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82095218: lifted from generated/ by lift.py.
void f82095218(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82095218 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // std r5,32(r1)
  guest::StoreU64(base, uint32_t(r1) + 32, r5);
  // std r4,24(r1)
  guest::StoreU64(base, uint32_t(r1) + 24, r4);
  // lwz r10,28(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 28);
  // lwz r11,36(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 36);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r9,r11,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // lwz r11,24(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 24);
loc_82095240:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82095254
  if (cr6.eq) goto loc_82095254;
  // lwz r8,32(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 32);
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // beq cr6,0x82095258
  if (cr6.eq) goto loc_82095258;
loc_82095254:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82095258:
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8209528c
  if (cr6.eq) goto loc_8209528C;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8209526c
  if (!cr6.eq) goto loc_8209526C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8209526C:
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // bne cr6,0x8209527c
  if (!cr6.eq) goto loc_8209527C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8209527C:
  // lwz r8,0(r6)
  r8 = guest::LoadU32(base, uint32_t(r6) + 0);
  // lwz r7,8(r10)
  r7 = guest::LoadU32(base, uint32_t(r10) + 8);
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // bne cr6,0x82095240
  if (!cr6.eq) goto loc_82095240;
loc_8209528C:
  // std r4,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r4);
  // blr
  return;
}

// sub_82095298: lifted from generated/ by lift.py.
lifted::Result f82095298(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82095298 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // addi r4,r31,8
  r4 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258e580
  // (left out: dead in C++)
  { const lifted::Result o = game::ScopedLock::Ctor(base, LIFTED_ARGS(0x820952C0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,36
  r31 = uint64_t(int64_t(int64_t(r31) + 36));
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r6,r30
  r6 = r30;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r31,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r31));
  // mr r8,r11
  r8 = r11;
  // stw r31,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r31));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r8,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r8));
  // ld r5,0(r10)
  r5 = guest::LoadU64(base, uint32_t(r10) + 0);
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // ld r4,0(r9)
  r4 = guest::LoadU64(base, uint32_t(r9) + 0);
  // bl 0x82095218
  // (left out: dead in C++)
  f82095218(base, r1, r3, r4, r5, r6);
  // lwz r11,88(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 88);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82095314
  if (cr6.eq) goto loc_82095314;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // beq cr6,0x82095318
  if (cr6.eq) goto loc_82095318;
loc_82095314:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82095318:
  // lwz r11,92(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 92);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82095354
  if (cr0.eq) goto loc_82095354;
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82095330:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258e5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScopedLock::Dtor(base, LIFTED_ARGS(0x82095338, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
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
loc_82095354:
  // mr r4,r31
  r4 = r31;
  // ld r5,88(r1)
  r5 = guest::LoadU64(base, uint32_t(r1) + 88);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82098e80
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Erase(base, LIFTED_ARGS(0x82095364, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // b 0x82095330
  goto loc_82095330;
}

// sub_82095370: lifted from generated/ by lift.py.
lifted::Result f82095370(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82095370 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // std r5,176(r1)
  guest::StoreU64(base, uint32_t(r1) + 176, r5);
  // mr r27,r6
  r27 = r6;
  // lwz r9,176(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 176);
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // std r27,184(r1)
  guest::StoreU64(base, uint32_t(r1) + 184, r27);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // beq cr6,0x820953ac
  if (cr6.eq) goto loc_820953AC;
  // cmplw cr6,r9,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r31), so);
  // beq cr6,0x820953b0
  if (cr6.eq) goto loc_820953B0;
loc_820953AC:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820953B0:
  // lwz r8,180(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r28,188(r1)
  r28 = guest::LoadU32(base, uint32_t(r1) + 188);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // lwz r29,184(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 184);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82095408
  if (cr0.eq) goto loc_82095408;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x820953dc
  if (cr6.eq) goto loc_820953DC;
  // cmplw cr6,r29,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r31), so);
  // beq cr6,0x820953e0
  if (cr6.eq) goto loc_820953E0;
loc_820953DC:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820953E0:
  // subf r11,r28,r10
  r11 = r10 - r28;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82095408
  if (cr0.eq) goto loc_82095408;
  // mr r3,r31
  r3 = r31;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x820953F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r31,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r31));
  // stw r11,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r11));
  // b 0x82095458
  goto loc_82095458;
loc_82095408:
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x82095418
  if (cr6.eq) goto loc_82095418;
  // cmplw cr6,r9,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r29), so);
  // beq cr6,0x8209541c
  if (cr6.eq) goto loc_8209541C;
loc_82095418:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8209541C:
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // subf r11,r11,r28
  r11 = r28 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82095454
  if (cr0.eq) goto loc_82095454;
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82098e80
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Erase(base, LIFTED_ARGS(0x82095444, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r5,0(r3)
  r5 = guest::LoadU64(base, uint32_t(r3) + 0);
  // std r5,176(r1)
  guest::StoreU64(base, uint32_t(r1) + 176, r5);
  // lwz r9,176(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 176);
  // b 0x82095408
  goto loc_82095408;
loc_82095454:
  // std r27,0(r30)
  guest::StoreU64(base, uint32_t(r30) + 0, r27);
loc_82095458:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82095468: lifted from generated/ by lift.py.
lifted::Result f82095468(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82095468 (lifted)");
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
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // mr r31,r4
  r31 = r4;
  // mr r30,r3
  r30 = r3;
  // lwz r11,776(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 776);
  // stw r31,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r31));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x820954c4
  if (!cr6.eq) goto loc_820954C4;
  // addi r4,r1,140
  r4 = uint64_t(int64_t(int64_t(r1) + 140));
  // addi r3,r30,20
  r3 = uint64_t(int64_t(int64_t(r30) + 20));
  // bl 0x82095298
  // (left out: dead in C++)
  { const lifted::Result o = f82095298(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820954B8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r30,16
  r3 = uint64_t(int64_t(int64_t(r30) + 16));
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Signal(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x820954e0
  goto loc_820954E0;
loc_820954C4:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x820954e0
  if (!cr6.eq) goto loc_820954E0;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820954E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820954E0:
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

// sub_820954F8: lifted from generated/ by lift.py.
lifted::Result f820954F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820954F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // addi r4,r31,8
  r4 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258e580
  // (left out: dead in C++)
  { const lifted::Result o = game::ScopedLock::Ctor(base, LIFTED_ARGS(0x82095518, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x820955d4
  if (cr6.eq) goto loc_820955D4;
  // addi r31,r31,36
  r31 = uint64_t(int64_t(int64_t(r31) + 36));
  // mr r29,r31
  r29 = r31;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r29,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r29));
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
loc_82095534:
  // stw r30,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r30));
loc_82095538:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8209554c
  if (cr6.eq) goto loc_8209554C;
  // cmplw cr6,r29,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r31), so);
  // beq cr6,0x82095550
  if (cr6.eq) goto loc_82095550;
loc_8209554C:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82095550:
  // subf r11,r30,r11
  r11 = r11 - r30;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8209560c
  if (cr0.eq) goto loc_8209560C;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x82095574
  if (!cr6.eq) goto loc_82095574;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82095574:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x82095584
  if (!cr6.eq) goto loc_82095584;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82095584:
  // addi r3,r30,8
  r3 = uint64_t(int64_t(int64_t(r30) + 8));
  // mtctr r28
  ctr = r28;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095590, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820955bc
  if (cr0.eq) goto loc_820955BC;
  // mr r4,r31
  r4 = r31;
  // ld r5,88(r1)
  r5 = guest::LoadU64(base, uint32_t(r1) + 88);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82098e80
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Erase(base, LIFTED_ARGS(0x820955A8, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r11,0(r3)
  r11 = guest::LoadU64(base, uint32_t(r3) + 0);
  // std r11,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r11);
  // lwz r30,92(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 92);
  // lwz r29,88(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 88);
  // b 0x82095538
  goto loc_82095538;
loc_820955BC:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x820955cc
  if (!cr6.eq) goto loc_820955CC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820955CC:
  // lwz r30,0(r30)
  r30 = guest::LoadU32(base, uint32_t(r30) + 0);
  // b 0x82095534
  goto loc_82095534;
loc_820955D4:
  // addi r4,r31,36
  r4 = uint64_t(int64_t(int64_t(r31) + 36));
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,4(r4)
  r11 = guest::LoadU32(base, uint32_t(r4) + 4);
  // stw r4,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r4));
  // mr r8,r11
  r8 = r11;
  // stw r4,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r4));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r8,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r8));
  // ld r6,0(r10)
  r6 = guest::LoadU64(base, uint32_t(r10) + 0);
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // ld r5,0(r9)
  r5 = guest::LoadU64(base, uint32_t(r9) + 0);
  // bl 0x82095370
  // (left out: dead in C++)
  { const lifted::Result o = f82095370(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_8209560C:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258e5b8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScopedLock::Dtor(base, LIFTED_ARGS(0x82095614, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82095620 game::WorkerPool::Create: lifted from generated/ by lift.py.
lifted::Result f82095620(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82095620 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // li r3,296
  r3 = uint64_t(int64_t(296));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8209563C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82095650
  if (cr0.eq) goto loc_82095650;
  // mr r4,r31
  r4 = r31;
  // bl 0x82094f30
  // (left out: dead in C++)
  { const lifted::Result o = f82094F30(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // b 0x82095654
  goto loc_82095654;
loc_82095650:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82095654:
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // addi r3,r3,8
  r3 = uint64_t(int64_t(int64_t(r3) + 8));
  // bne cr6,0x82095664
  if (!cr6.eq) goto loc_82095664;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82095664:
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

// sub_82095678: lifted from generated/ by lift.py.
void f82095678(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82095678 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // std r4,24(r1)
  guest::StoreU64(base, uint32_t(r1) + 24, r4);
  // lwz r11,28(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 28);
  // lwz r10,24(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 24);
  // std r5,32(r1)
  guest::StoreU64(base, uint32_t(r1) + 32, r5);
  // stw r11,-12(r1)
  guest::StoreU32(base, uint32_t(r1) + -12, uint32_t(r11));
  // stw r10,-16(r1)
  guest::StoreU32(base, uint32_t(r1) + -16, uint32_t(r10));
  // ld r11,-16(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + -16);
  // std r11,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r11);
  // lwz r11,-12(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + -12);
loc_8209569C:
  // lwz r9,36(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 36);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm. r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820956d0
  if (cr0.eq) goto loc_820956D0;
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lwz r8,0(r6)
  r8 = guest::LoadU32(base, uint32_t(r6) + 0);
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // beq cr6,0x820956d0
  if (cr6.eq) goto loc_820956D0;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // b 0x8209569c
  goto loc_8209569C;
loc_820956D0:
  // lwz r9,-16(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + -16);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x820956e0
  if (cr6.eq) goto loc_820956E0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820956E0:
  // stw r11,28(r1)
  guest::StoreU32(base, uint32_t(r1) + 28, uint32_t(r11));
  // ld r11,24(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 24);
  // std r11,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r11);
  // blr
  return;
}

// sub_820956F0: lifted from generated/ by lift.py.
lifted::Result f820956F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820956F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // mr r30,r3
  r30 = r3;
  // stw r4,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r4));
  // addi r27,r1,96
  r27 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r31,r30,84
  r31 = uint64_t(int64_t(int64_t(r30) + 84));
  // addi r26,r1,104
  r26 = uint64_t(int64_t(int64_t(r1) + 104));
  // addi r29,r30,132
  r29 = uint64_t(int64_t(int64_t(r30) + 132));
loc_82095714:
  // mr r3,r29
  r3 = r29;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8209571C); r3 = o.r3; f1 = o.f1; }
  // lwz r11,92(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 92);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8209575c
  if (!cr0.eq) goto loc_8209575C;
  // addi r28,r30,100
  r28 = uint64_t(int64_t(int64_t(r30) + 100));
loc_8209572C:
  // mr r3,r29
  r3 = r29;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095734); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8258dd90
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Wait(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095750); r3 = o.r3; f1 = o.f1; }
  // lwz r11,92(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 92);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8209572c
  if (cr0.eq) goto loc_8209572C;
loc_8209575C:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // addi r6,r1,204
  r6 = uint64_t(int64_t(int64_t(r1) + 204));
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r31,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r31));
  // mr r10,r11
  r10 = r11;
  // stw r31,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r31));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r10,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r10));
  // ld r5,0(r27)
  r5 = guest::LoadU64(base, uint32_t(r27) + 0);
  // stw r11,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r11));
  // ld r4,0(r26)
  r4 = guest::LoadU64(base, uint32_t(r26) + 0);
  // bl 0x82095678
  // (left out: dead in C++)
  f82095678(base, r1, r3, r4, r5, r6);
  // lwz r11,88(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 88);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820957a4
  if (cr6.eq) goto loc_820957A4;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // beq cr6,0x820957a8
  if (cr6.eq) goto loc_820957A8;
loc_820957A4:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820957A8:
  // lwz r11,92(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 92);
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
  // bne 0x820957d0
  if (!cr0.eq) goto loc_820957D0;
  // mr r3,r29
  r3 = r29;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x820957CC); r3 = o.r3; f1 = o.f1; }
  // b 0x82095714
  goto loc_82095714;
loc_820957D0:
  // mr r4,r31
  r4 = r31;
  // ld r5,88(r1)
  r5 = guest::LoadU64(base, uint32_t(r1) + 88);
  // addi r3,r1,104
  r3 = uint64_t(int64_t(int64_t(r1) + 104));
  // bl 0x82098e80
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Erase(base, LIFTED_ARGS(0x820957E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x820957E8); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820957F0: lifted from generated/ by lift.py.
lifted::Result f820957F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820957F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // mr r31,r3
  r31 = r3;
  // lwz r11,776(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 776);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82095834
  if (!cr6.eq) goto loc_82095834;
  // lis r11,-32247
  r11 = uint64_t(int64_t(-2113339392));
  // addi r3,r31,20
  r3 = uint64_t(int64_t(int64_t(r31) + 20));
  // addi r4,r11,19376
  r4 = uint64_t(int64_t(int64_t(r11) + 19376));
  // bl 0x820954f8
  // (left out: dead in C++)
  { const lifted::Result o = f820954F8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Signal(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8209589c
  goto loc_8209589C;
loc_82095834:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8209589c
  if (!cr6.eq) goto loc_8209589C;
  // addi r30,r31,100
  r30 = uint64_t(int64_t(int64_t(r31) + 100));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095848); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32245
  r10 = uint64_t(int64_t(-2113208320));
  // addi r11,r31,68
  r11 = uint64_t(int64_t(int64_t(r31) + 68));
  // addi r10,r10,-13888
  r10 = uint64_t(int64_t(int64_t(r10) + -13888));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r7,r1,88
  r7 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r6,0(r9)
  r6 = guest::LoadU32(base, uint32_t(r9) + 0);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // mr r9,r10
  r9 = r10;
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // ld r5,0(r8)
  r5 = guest::LoadU64(base, uint32_t(r8) + 0);
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // ld r4,0(r7)
  r4 = guest::LoadU64(base, uint32_t(r7) + 0);
  // bl 0x82118f50
  // (left out: dead in C++)
  { const lifted::Result o = f82118F50(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8209589C); r3 = o.r3; f1 = o.f1; }
loc_8209589C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
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

// sub_820959D0: lifted from generated/ by lift.py.
lifted::Result f820959D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820959D0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // addi r30,r31,8
  r30 = uint64_t(int64_t(int64_t(r31) + 8));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x820959F0); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,36
  r3 = uint64_t(int64_t(int64_t(r31) + 36));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r5,r29
  r5 = r29;
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 4);
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // ld r4,0(r11)
  r4 = guest::LoadU64(base, uint32_t(r11) + 0);
  // bl 0x82095958
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Insert(base, LIFTED_ARGS(0x82095A10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095A18); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Signal(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82095A28: lifted from generated/ by lift.py.
lifted::Result f82095A28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82095A28 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // mr r31,r3
  r31 = r3;
  // mr r3,r4
  r3 = r4;
  // lwz r11,776(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 776);
  // stw r3,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r3));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82095a74
  if (!cr6.eq) goto loc_82095A74;
  // addi r3,r31,20
  r3 = uint64_t(int64_t(int64_t(r31) + 20));
  // addi r4,r1,140
  r4 = uint64_t(int64_t(int64_t(r1) + 140));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095A70, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82095ad8
  goto loc_82095AD8;
loc_82095A74:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82095ad8
  if (!cr6.eq) goto loc_82095AD8;
  // addic. r11,r31,-8
  ca = uint32_t(r31) > 7;
  r11 = uint64_t(int64_t(int64_t(r31) + -8));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r4,r31,-4
  r4 = uint64_t(int64_t(int64_t(r31) + -4));
  // bne 0x82095a8c
  if (!cr0.eq) goto loc_82095A8C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_82095A8C:
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095A9C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,100
  r30 = uint64_t(int64_t(int64_t(r31) + 100));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095AA8); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,68
  r3 = uint64_t(int64_t(int64_t(r31) + 68));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r1,140
  r5 = uint64_t(int64_t(int64_t(r1) + 140));
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 4);
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // ld r4,0(r11)
  r4 = guest::LoadU64(base, uint32_t(r11) + 0);
  // bl 0x82095958
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Insert(base, LIFTED_ARGS(0x82095AC8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,92
  r3 = uint64_t(int64_t(int64_t(r31) + 92));
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Signal(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095AD8); r3 = o.r3; f1 = o.f1; }
loc_82095AD8:
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

// sub_82095DF8: lifted from generated/ by lift.py.
lifted::Result f82095DF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82095DF8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r25,r31,8
  r25 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r11,r11,3852
  r11 = uint64_t(int64_t(int64_t(r11) + 3852));
  // addi r10,r10,3844
  r10 = uint64_t(int64_t(int64_t(r10) + 3844));
  // addi r9,r9,3816
  r9 = uint64_t(int64_t(int64_t(r9) + 3816));
  // addi r30,r31,256
  r30 = uint64_t(int64_t(int64_t(r31) + 256));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r25
  r3 = r25;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // mr r4,r30
  r4 = r30;
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,0(r25)
  guest::StoreU32(base, uint32_t(r25) + 0, uint32_t(r9));
  // bl 0x82095a28
  // (left out: dead in C++)
  { const lifted::Result o = f82095A28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82094b10
  // (left out: dead in C++)
  { const lifted::Result o = game::Job::Wait(base, LIFTED_ARGS(0x82095E4C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r31,108
  r26 = uint64_t(int64_t(int64_t(r31) + 108));
  // mr r3,r26
  r3 = r26;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095E58); r3 = o.r3; f1 = o.f1; }
  // lwz r11,244(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 244);
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r29,r24
  r29 = r24;
  // ble cr6,0x82095eb0
  if (!cr6.gt) goto loc_82095EB0;
  // addi r28,r31,76
  r28 = uint64_t(int64_t(int64_t(r31) + 76));
  // stw r24,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r24));
  // addi r27,r31,100
  r27 = uint64_t(int64_t(int64_t(r31) + 100));
  // stw r28,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r28));
loc_82095E7C:
  // lwz r10,4(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 4);
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // ld r4,0(r11)
  r4 = guest::LoadU64(base, uint32_t(r11) + 0);
  // bl 0x82095958
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Insert(base, LIFTED_ARGS(0x82095E98, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Signal(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,244(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 244);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // blt cr6,0x82095e7c
  if (cr6.lt) goto loc_82095E7C;
loc_82095EB0:
  // mr r3,r26
  r3 = r26;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095EB8); r3 = o.r3; f1 = o.f1; }
  // lwz r11,244(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 244);
  // mr r29,r24
  r29 = r24;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x82095ef4
  if (!cr6.gt) goto loc_82095EF4;
  // addi r28,r31,164
  r28 = uint64_t(int64_t(int64_t(r31) + 164));
loc_82095ECC:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8258dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::Thread::Join(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,244(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 244);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r28,r28,20
  r28 = uint64_t(int64_t(int64_t(r28) + 20));
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // blt cr6,0x82095ecc
  if (cr6.lt) goto loc_82095ECC;
loc_82095EF4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // addi r10,r10,2984
  r10 = uint64_t(int64_t(int64_t(r10) + 2984));
  // addi r9,r9,2740
  r9 = uint64_t(int64_t(int64_t(r9) + 2740));
  // addi r8,r8,3768
  r8 = uint64_t(int64_t(int64_t(r8) + 3768));
  // addi r29,r31,244
  r29 = uint64_t(int64_t(int64_t(r31) + 244));
  // stw r11,24(r30)
  guest::StoreU32(base, uint32_t(r30) + 24, uint32_t(r11));
  // li r28,3
  r28 = uint64_t(int64_t(3));
  // stw r10,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r10));
  // stw r9,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r9));
  // stw r8,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r8));
loc_82095F2C:
  // addi r29,r29,-20
  r29 = uint64_t(int64_t(int64_t(r29) + -20));
  // mr r3,r29
  r3 = r29;
  // bl 0x8258ddd0
  // (left out: dead in C++)
  game::Thread::Dtor(base, r1, r13, r3);
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bge 0x82095f2c
  if (!cr0.lt) goto loc_82095F2C;
  // addi r30,r31,164
  r30 = uint64_t(int64_t(int64_t(r31) + 164));
  // li r29,1
  r29 = uint64_t(int64_t(1));
loc_82095F48:
  // addi r30,r30,-28
  r30 = uint64_t(int64_t(int64_t(r30) + -28));
  // mr r3,r30
  r3 = r30;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82095F54, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge 0x82095f48
  if (!cr0.lt) goto loc_82095F48;
  // mr r30,r26
  r30 = r26;
  // li r29,1
  r29 = uint64_t(int64_t(1));
loc_82095F64:
  // addi r30,r30,-4
  r30 = uint64_t(int64_t(int64_t(r30) + -4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8258dd60
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x82095F70); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge 0x82095f64
  if (!cr0.lt) goto loc_82095F64;
  // addi r28,r31,24
  r28 = uint64_t(int64_t(int64_t(r31) + 24));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // addi r30,r28,76
  r30 = uint64_t(int64_t(int64_t(r28) + 76));
loc_82095F84:
  // addi r30,r30,-12
  r30 = uint64_t(int64_t(int64_t(r30) + -12));
  // mr r3,r30
  r3 = r30;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x82095F90, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82095F98, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // stw r24,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r24));
  // bge 0x82095f84
  if (!cr0.lt) goto loc_82095F84;
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // bl 0x82094d68
  // (left out: dead in C++)
  { const lifted::Result o = f82094D68(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x8258dd60
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x82095FB4); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3768
  r11 = uint64_t(int64_t(int64_t(r11) + 3768));
  // addi r10,r10,2740
  r10 = uint64_t(int64_t(int64_t(r10) + 2740));
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r25)
  guest::StoreU32(base, uint32_t(r25) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // bl 0x8258e530
  // (left out: dead in C++)
  f8258E530(base, r3);
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82095FE0: lifted from generated/ by lift.py.
lifted::Result f82095FE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82095FE0 (lifted)");
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
  // bl 0x82095df8
  // (left out: dead in C++)
  { const lifted::Result o = f82095DF8(base, r1, r13, r3, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82096010
  if (cr0.eq) goto loc_82096010;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82096010, r31)); r3 = o.r3; f1 = o.f1; }
loc_82096010:
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

// sub_82096220 game::FramePasses::OnEvent: lifted from generated/ by lift.py.
lifted::Result f82096220(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82096220 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r5)
  r11 = guest::LoadU32(base, schema::FramePassesEvent(base, uint32_t(r5)).code_address());
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x820962a0
  if (!cr6.eq) goto loc_820962A0;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::FramePassesRunLoopVtable(base, uint32_t(r11)).slot_4_remove_tick_handler_address());
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82096260, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::FramePassesRunLoopVtable(base, uint32_t(r11)).slot_4_remove_tick_handler_address());
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82096280, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::FramePassesRunLoopVtable(base, uint32_t(r11)).slot_3_remove_listener_address());
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820962A0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820962A0:
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

// sub_82096320: lifted from generated/ by lift.py.
lifted::Result f82096320(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82096320 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r12,0(r3)
  r12 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r12)
  r11 = guest::LoadU32(base, schema::FramePassesDelegateObjectVtable(base, uint32_t(r12)).slot_3_address());
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820987E8 game::FramePasses::Dtor: lifted from generated/ by lift.py.
lifted::Result f820987E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_820987E8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r10,r10,3872
  r10 = uint64_t(int64_t(int64_t(r10) + 3872));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // addi r11,r31,8
  r11 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r10,0(r31)
  guest::StoreU32(base, schema::FramePasses(base, uint32_t(r31)).vtable_address(), uint32_t(r10));
  // addi r9,r9,2748
  r9 = uint64_t(int64_t(int64_t(r9) + 2748));
  // addi r8,r8,2984
  r8 = uint64_t(int64_t(int64_t(r8) + 2984));
  // addi r7,r7,2740
  r7 = uint64_t(int64_t(int64_t(r7) + 2740));
  // addi r6,r6,3768
  r6 = uint64_t(int64_t(int64_t(r6) + 3768));
  // stw r9,24(r11)
  guest::StoreU32(base, schema::FramePasses(base, uint32_t(r11) - 8).job_delegate_vtable_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::FramePasses(base, uint32_t(r11) - 8).job_interface_address(), uint32_t(r8));
  // stw r7,4(r11)
  guest::StoreU32(base, schema::FramePasses(base, uint32_t(r11) - 8).job_interface_address(), uint32_t(r7));
  // stw r6,0(r11)
  guest::StoreU32(base, schema::FramePasses(base, uint32_t(r11) - 8).job_vtable_address(), uint32_t(r6));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82098848
  if (cr0.eq) goto loc_82098848;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82098848, r31)); r3 = o.r3; f1 = o.f1; }
loc_82098848:
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

// sub_82098860 game::Scene::Dtor_82098860: lifted from generated/ by lift.py.
lifted::Result f82098860(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82098860 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3900
  r11 = uint64_t(int64_t(int64_t(r11) + 3900));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::AppScene(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // bl 0x82589650
  // (left out: dead in C++)
  { const lifted::Result o = f82589650(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // bl 0x82590de0
  // (left out: dead in C++)
  { const lifted::Result o = game::RenderQueue::Dtor_82590DE0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // bl 0x8258ccc8
  // (left out: dead in C++)
  { const lifted::Result o = f8258CCC8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::AppScene(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
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

// sub_820989A8 game::Scene::OnEvent5: lifted from generated/ by lift.py.
lifted::Result f820989A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820989A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r5)
  r11 = guest::LoadU32(base, schema::FrameDriverEventWord(base, uint32_t(r5)).value_address());
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82098a34
  if (!cr6.eq) goto loc_82098A34;
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, schema::AppScene(base, uint32_t(r3)).framework_interface_address());
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::AppSceneFrameworkInterfaceVtable(base, uint32_t(r11)).frame_driver_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820989E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820989F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,18
  r10 = uint64_t(int64_t(18));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::FrameDriverRunLoopVtable(base, uint32_t(r11)).remove_tick_handler_address());
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82098A14, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::FrameDriverRunLoopVtable(base, uint32_t(r11)).remove_listener_address());
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82098A34, r31)); r3 = o.r3; f1 = o.f1; }
loc_82098A34:
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

// sub_82098A48 game::Scene::Dtor_82098A48: lifted from generated/ by lift.py.
lifted::Result f82098A48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82098A48 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x82098860
  // (left out: dead in C++)
  { const lifted::Result o = game::Scene::Dtor_82098860(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82098a78
  if (cr0.eq) goto loc_82098A78;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82098A78, r31)); r3 = o.r3; f1 = o.f1; }
loc_82098A78:
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

// sub_82098C70 game::Scene::Create: lifted from generated/ by lift.py.
lifted::Result f82098C70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82098C70 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r3,80
  r3 = uint64_t(int64_t(80));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82098C8C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82098ca0
  if (cr0.eq) goto loc_82098CA0;
  // mr r4,r31
  r4 = r31;
  // bl 0x82098a98
  // (left out: dead in C++)
  { const lifted::Result o = game::Scene::Ctor(base, LIFTED_ARGS(0x82098C9C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82098ca4
  goto loc_82098CA4;
loc_82098CA0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82098CA4:
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

// sub_82098CE8: lifted from generated/ by lift.py.
lifted::Result f82098CE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82098CE8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 0);
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82098d48
  if (!cr6.eq) goto loc_82098D48;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82098D28, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82098D48, r31)); r3 = o.r3; f1 = o.f1; }
loc_82098D48:
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

// sub_82098F30: lifted from generated/ by lift.py.
lifted::Result f82098F30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82098F30 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3912
  r11 = uint64_t(int64_t(int64_t(r11) + 3912));
  // addi r30,r31,3980
  r30 = uint64_t(int64_t(int64_t(r31) + 3980));
  // li r29,4
  r29 = uint64_t(int64_t(4));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::InputManagerAtTeardown(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
loc_82098F54:
  // addi r30,r30,-12
  r30 = uint64_t(int64_t(int64_t(r30) + -12));
  // mr r3,r30
  r3 = r30;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x82098F60, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, schema::InputManagerAtTeardown(base, uint32_t(r30) - 3968).list_head_address());
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82098F68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // stw r11,4(r30)
  guest::StoreU32(base, schema::InputManagerAtTeardown(base, uint32_t(r30) - 3968).list_head_address(), uint32_t(r11));
  // bge 0x82098f54
  if (!cr0.lt) goto loc_82098F54;
  // addi r30,r31,3920
  r30 = uint64_t(int64_t(int64_t(r31) + 3920));
  // li r29,4
  r29 = uint64_t(int64_t(4));
loc_82098F80:
  // addi r30,r30,-416
  r30 = uint64_t(int64_t(int64_t(r30) + -416));
  // mr r3,r30
  r3 = r30;
  // bl 0x8258cea0
  // (left out: dead in C++)
  f8258CEA0(base, r1, r13, r3);
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge 0x82098f80
  if (!cr0.lt) goto loc_82098F80;
  // addi r30,r31,1836
  r30 = uint64_t(int64_t(int64_t(r31) + 1836));
  // li r29,3
  r29 = uint64_t(int64_t(3));
loc_82098F9C:
  // addi r30,r30,-212
  r30 = uint64_t(int64_t(int64_t(r30) + -212));
  // mr r3,r30
  r3 = r30;
  // bl 0x82591bb8
  // (left out: dead in C++)
  r3 = f82591BB8(base, r1, r13, r3);
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge 0x82098f9c
  if (!cr0.lt) goto loc_82098F9C;
  // addi r30,r31,988
  r30 = uint64_t(int64_t(int64_t(r31) + 988));
  // li r29,4
  r29 = uint64_t(int64_t(4));
loc_82098FB8:
  // addi r30,r30,-196
  r30 = uint64_t(int64_t(int64_t(r30) + -196));
  // mr r3,r30
  r3 = r30;
  // bl 0x825918e0
  // (left out: dead in C++)
  f825918E0(base, r3);
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge 0x82098fb8
  if (!cr0.lt) goto loc_82098FB8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2616
  r11 = uint64_t(int64_t(int64_t(r11) + 2616));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::InputManagerAtTeardown(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82098FF8: lifted from generated/ by lift.py.
lifted::Result f82098FF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82098FF8 (lifted)");
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
  // bl 0x82098f30
  // (left out: dead in C++)
  { const lifted::Result o = f82098F30(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82099028
  if (cr0.eq) goto loc_82099028;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82099028, r31)); r3 = o.r3; f1 = o.f1; }
loc_82099028:
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

// sub_82099048: lifted from generated/ by lift.py.
lifted::Result f82099048(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_82099048 (lifted)");
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
  // mr r30,r3
  r30 = r3;
  // stw r5,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r5));
  // mr r3,r5
  r3 = r5;
  // mr r31,r4
  r31 = r4;
  // bl 0x82592400
  // (left out: dead in C++)
  r3 = f82592400(base, r1, r13, r3);
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // addi r4,r1,148
  r4 = uint64_t(int64_t(int64_t(r1) + 148));
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r3,r11,3920
  r3 = uint64_t(int64_t(int64_t(r11) + 3920));
  // bl 0x82118790
  // (left out: dead in C++)
  { const lifted::Result o = game::HandleList::Remove(base, r1, r13, r3, r4, r6, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mulli r11,r10,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r9,3928(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 3928);
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm. r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x820990d8
  if (!cr0.eq) goto loc_820990D8;
  // addi r11,r11,3920
  r11 = uint64_t(int64_t(int64_t(r11) + 3920));
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // extsw r11,r9
  r11 = uint64_t(int64_t(int32_t(r9)));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x820990c4
  if (!cr6.eq) goto loc_820990C4;
  // twi 31,r0,22
  lifted::Trap(22);
  // twi 31,r0,22
  lifted::Trap(22);
loc_820990C4:
  // mulli r10,r10,416
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(416))));
  // lwz r3,8(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 8);
  // add r10,r10,r30
  r10 = r10 + r30;
  // addi r4,r10,1840
  r4 = uint64_t(int64_t(int64_t(r10) + 1840));
  // bl 0x82098d60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82098D60, LIFTED_ARGS(0x820990D8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820990D8:
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

// sub_820994E0: lifted from generated/ by lift.py.
lifted::Result f820994E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820994E0 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3960
  r11 = uint64_t(int64_t(int64_t(r11) + 3960));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x8209950c
  if (cr0.eq) goto loc_8209950C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8209950C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209950C:
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

// sub_82099528: lifted from generated/ by lift.py.
lifted::Result f82099528(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82099528 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,4000
  r11 = uint64_t(int64_t(int64_t(r11) + 4000));
  // lwz r3,240(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 240);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82099558
  if (cr0.eq) goto loc_82099558;
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x82099558, r31)); r3 = o.r3; f1 = o.f1; }
loc_82099558:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3960
  r11 = uint64_t(int64_t(int64_t(r11) + 3960));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
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

// sub_82099578: lifted from generated/ by lift.py.
uint64_t f82099578(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82099578 (lifted)");
  // lwz r3,244(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 244);
  // blr
  return r3;
}

// sub_82099580: lifted from generated/ by lift.py.
uint64_t f82099580(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82099580 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,252(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 252);
  // stw r11,0(r4)
  guest::StoreU32(base, uint32_t(r4) + 0, uint32_t(r11));
  // lwz r3,248(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 248);
  // blr
  return r3;
}

// sub_82099590: lifted from generated/ by lift.py.
lifted::Result f82099590(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82099590 (lifted)");
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
  // lwz r11,0(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x820995e8
  if (!cr6.eq) goto loc_820995E8;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // addi r4,r30,224
  r4 = uint64_t(int64_t(int64_t(r30) + 224));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820995D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // addi r4,r30,228
  r4 = uint64_t(int64_t(int64_t(r30) + 228));
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820995E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820995E8:
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

// sub_82099600 game::App::ReadUsers: lifted from generated/ by lift.py.
lifted::Result f82099600(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82099600 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // addi r29,r31,40
  r29 = uint64_t(int64_t(int64_t(r31) + 40));
  // mr r30,r26
  r30 = r26;
  // stw r26,216(r31)
  guest::StoreU32(base, uint32_t(r31) + 216, uint32_t(r26));
  // stw r26,220(r31)
  guest::StoreU32(base, uint32_t(r31) + 220, uint32_t(r26));
loc_82099624:
  // mr r3,r30
  r3 = r30;
  // bl 0x82295028
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserGetSigninState(base, LIFTED_ARGS(0x8209962C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r29,-32
  r28 = uint64_t(int64_t(int64_t(r29) + -32));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // stw r3,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r3));
  // beq 0x820996bc
  if (cr0.eq) goto loc_820996BC;
  // addi r27,r29,-24
  r27 = uint64_t(int64_t(int64_t(r29) + -24));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // mr r3,r30
  r3 = r30;
  // addi r4,r27,8
  r4 = uint64_t(int64_t(int64_t(r27) + 8));
  // bl 0x82295020
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserGetName(base, LIFTED_ARGS(0x82099650, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r27
  r4 = r27;
  // mr r3,r30
  r3 = r30;
  // bl 0x82295268
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserGetXUID(base, LIFTED_ARGS(0x8209965C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // stb r26,0(r29)
  guest::StoreU8(base, uint32_t(r29) + 0, uint8_t(r26));
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x820996a0
  if (!cr6.eq) goto loc_820996A0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,0(r29)
  guest::StoreU8(base, uint32_t(r29) + 0, uint8_t(r11));
  // lwz r11,220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 220);
  // addi r11,r11,21
  r11 = uint64_t(int64_t(int64_t(r11) + 21));
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // stwx r28,r11,r31
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r31), uint32_t(r28));
  // lwz r11,220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 220);
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r30,172(r11)
  guest::StoreU32(base, uint32_t(r11) + 172, uint32_t(r30));
  // lwz r11,220(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 220);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,220(r31)
  guest::StoreU32(base, uint32_t(r31) + 220, uint32_t(r11));
loc_820996A0:
  // lwz r11,216(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 216);
  // addi r11,r11,50
  r11 = uint64_t(int64_t(int64_t(r11) + 50));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r30,r11,r31
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r31), uint32_t(r30));
  // lwz r11,216(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 216);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,216(r31)
  guest::StoreU32(base, uint32_t(r31) + 216, uint32_t(r11));
loc_820996BC:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,40
  r29 = uint64_t(int64_t(int64_t(r29) + 40));
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // blt cr6,0x82099624
  if (cr6.lt) goto loc_82099624;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82099710: lifted from generated/ by lift.py.
uint64_t f82099710(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82099710 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,216(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 216);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // blt cr6,0x82099724
  if (cr6.lt) goto loc_82099724;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_82099724:
  // addi r11,r4,50
  r11 = uint64_t(int64_t(int64_t(r4) + 50));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r3
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r3));
  // mulli r11,r11,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(40))));
  // add r11,r11,r3
  r11 = r11 + r3;
  // ld r3,16(r11)
  r3 = guest::LoadU64(base, uint32_t(r11) + 16);
  // blr
  return r3;
}

// sub_82099740: lifted from generated/ by lift.py.
uint64_t f82099740(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82099740 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,216(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 216);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bge cr6,0x82099774
  if (!cr6.lt) goto loc_82099774;
  // addi r11,r4,50
  r11 = uint64_t(int64_t(int64_t(r4) + 50));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r3
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r3));
  // mulli r11,r11,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(40))));
  // add r11,r11,r3
  r11 = r11 + r3;
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82099774
  if (cr6.eq) goto loc_82099774;
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // blr
  return r3;
loc_82099774:
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lwz r3,796(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 796);
  // blr
  return r3;
}

// sub_82099780: lifted from generated/ by lift.py.
uint64_t f82099780(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82099780 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,216(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 216);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // blt cr6,0x82099794
  if (cr6.lt) goto loc_82099794;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_82099794:
  // addi r11,r4,50
  r11 = uint64_t(int64_t(int64_t(r4) + 50));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r3
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r3));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mulli r11,r11,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(40))));
  // lbzx r3,r11,r3
  r3 = guest::LoadU8(base, uint32_t(r11) + uint32_t(r3));
  // blr
  return r3;
}

// sub_820997B0: lifted from generated/ by lift.py.
uint64_t f820997B0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820997B0 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,220(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 220);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // blt cr6,0x820997c4
  if (cr6.lt) goto loc_820997C4;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // blr
  return r3;
loc_820997C4:
  // rlwinm r11,r4,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 3) & 0xFFFFFFF8;
  // add r11,r11,r3
  r11 = r11 + r3;
  // lwz r3,172(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 172);
  // blr
  return r3;
}

// sub_820997D8: lifted from generated/ by lift.py.
lifted::Result f820997D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820997D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x82099528
  // (left out: dead in C++)
  { const lifted::Result o = f82099528(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82099808
  if (cr0.eq) goto loc_82099808;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82099808, r31)); r3 = o.r3; f1 = o.f1; }
loc_82099808:
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

// sub_820998B8: lifted from generated/ by lift.py.
lifted::Result f820998B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_820998B8 (lifted)");
  uint64_t r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,4000
  r11 = uint64_t(int64_t(int64_t(r11) + 4000));
  // mr r30,r4
  r30 = r4;
  // addi r4,r31,224
  r4 = uint64_t(int64_t(int64_t(r31) + 224));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r29,r31,228
  r29 = uint64_t(int64_t(int64_t(r31) + 228));
  // stw r31,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r31));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // addi r10,r10,10320
  r10 = uint64_t(int64_t(int64_t(r10) + 10320));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::SysUsersSetUp(base, uint32_t(r30)).framework_interface_address());
  // lis r9,-32246
  r9 = uint64_t(int64_t(-2113273856));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // addi r10,r9,-26584
  r10 = uint64_t(int64_t(int64_t(r9) + -26584));
  // stw r11,232(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).framework_interface_address(), uint32_t(r11));
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, schema::SysUsersSetUp(base, uint32_t(r30)).tick_key_address());
  // stw r10,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r10));
  // stw r11,0(r4)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r4) - 224).tick_key_address(), uint32_t(r11));
  // lwz r11,12(r30)
  r11 = guest::LoadU32(base, schema::SysUsersSetUp(base, uint32_t(r30)).listener_key_address());
  // stw r11,0(r29)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r29) - 228).listener_key_address(), uint32_t(r11));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::SysUsersSetUp(base, uint32_t(r30)).run_loop_address());
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::FrameDriverRunLoopRef(base, uint32_t(r11)).add_tick_handler_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209992C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::SysUsersSetUp(base, uint32_t(r30)).run_loop_address());
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // stw r31,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r31));
  // lis r9,-32246
  r9 = uint64_t(int64_t(-2113273856));
  // addi r10,r10,10320
  r10 = uint64_t(int64_t(int64_t(r10) + 10320));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r4,r29
  r4 = r29;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // addi r11,r9,-27248
  r11 = uint64_t(int64_t(int64_t(r9) + -27248));
  // stw r10,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r10));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::FrameDriverRunLoopRef(base, uint32_t(r11)).add_listener_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82099970, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x822952b8
  // (left out: dead in C++)
  { const lifted::Result o = game::XNotifyCreateListener(base, LIFTED_ARGS(0x82099984, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,240(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).listener_address(), uint32_t(r3));
  // beq 0x82099998
  if (cr0.eq) goto loc_82099998;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // bne cr6,0x820999a0
  if (!cr6.eq) goto loc_820999A0;
loc_82099998:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,236(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).unknown_236_address(), uint32_t(r11));
loc_820999A0:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r9,r31,172
  r9 = uint64_t(int64_t(int64_t(r31) + 172));
  // addi r10,r31,16
  r10 = uint64_t(int64_t(int64_t(r31) + 16));
  // addi r7,r31,200
  r7 = uint64_t(int64_t(int64_t(r31) + 200));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r11,216(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).signed_in_count_address(), uint32_t(r11));
  // stw r11,220(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).live_count_address(), uint32_t(r11));
loc_820999BC:
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // stw r11,0(r7)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r7) - 200).signed_in_users_address(), uint32_t(r11));
  // stw r11,-8(r10)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r10) - 16).user0_signin_state_address(), uint32_t(r11));
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // std r11,0(r10)
  guest::StoreU64(base, schema::SysUsers(base, uint32_t(r10) - 16).user0_xuid_address(), r11);
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // stb r11,24(r10)
  guest::StoreU8(base, schema::SysUsers(base, uint32_t(r10) - 16).user0_on_live_address(), uint8_t(r11));
  // addi r10,r10,40
  r10 = uint64_t(int64_t(int64_t(r10) + 40));
  // stw r11,-4(r9)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r9) - 172).live_users_address(), uint32_t(r11));
  // stw r6,0(r9)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r9) - 172).live_user0_index_address(), uint32_t(r6));
  // addi r9,r9,8
  r9 = uint64_t(int64_t(int64_t(r9) + 8));
  // bne 0x820999bc
  if (!cr0.eq) goto loc_820999BC;
  // mr r3,r31
  r3 = r31;
  // stw r11,244(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).users_changed_address(), uint32_t(r11));
  // stw r11,248(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).live_connection_changed_address(), uint32_t(r11));
  // stw r11,252(r31)
  guest::StoreU32(base, schema::SysUsers(base, uint32_t(r31)).live_connection_state_address(), uint32_t(r11));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82099A08 game::Users::Create: lifted from generated/ by lift.py.
lifted::Result f82099A08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82099A08 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r3,256
  r3 = uint64_t(int64_t(256));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82099A24, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82099a38
  if (cr0.eq) goto loc_82099A38;
  // mr r4,r31
  r4 = r31;
  // bl 0x820998b8
  // (left out: dead in C++)
  { const lifted::Result o = f820998B8(base, r1, r13, r3, r4, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // b 0x82099a3c
  goto loc_82099A3C;
loc_82099A38:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82099A3C:
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

// sub_82099A50: lifted from generated/ by lift.py.
lifted::Result f82099A50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82099A50 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,4040
  r11 = uint64_t(int64_t(int64_t(r11) + 4040));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82099a7c
  if (cr0.eq) goto loc_82099A7C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82099A7C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82099A7C:
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

// sub_82099AD8: lifted from generated/ by lift.py.
lifted::Result f82099AD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82099AD8 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // std r4,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r4);
  // mr r11,r3
  r11 = r3;
  // addi r5,r11,44
  r5 = uint64_t(int64_t(int64_t(r11) + 44));
  // mr r4,r11
  r4 = r11;
  // lwz r3,124(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 124);
  // lwz r10,0(r3)
  r10 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82099B08, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82099BA8: lifted from generated/ by lift.py.
lifted::Result f82099BA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82099BA8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // b 0x82099be0
  goto loc_82099BE0;
loc_82099BC8:
  // mr r3,r30
  r3 = r30;
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, schema::HandlerMapNode(base, uint32_t(r31)).right_address());
  // bl 0x82099ba8
  // (left out: dead in C++)
  { const lifted::Result o = f82099BA8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // lwz r31,0(r31)
  r31 = guest::LoadU32(base, schema::HandlerMapNode(base, uint32_t(r31)).left_address());
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82099BE0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82099BE0:
  // lbz r11,21(r31)
  r11 = guest::LoadU8(base, schema::HandlerMapNode(base, uint32_t(r31)).is_nil_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82099bc8
  if (cr6.eq) goto loc_82099BC8;
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

// sub_82099CF8: lifted from generated/ by lift.py.
lifted::Result f82099CF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82099CF8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::HandlerMap(base, uint32_t(r31)).head_address());
  // lwz r4,4(r11)
  r4 = guest::LoadU32(base, schema::HandlerMapNode(base, uint32_t(r11)).parent_address());
  // bl 0x82099ba8
  // (left out: dead in C++)
  { const lifted::Result o = f82099BA8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::HandlerMap(base, uint32_t(r31)).head_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,4(r11)
  guest::StoreU32(base, schema::HandlerMapNode(base, uint32_t(r11)).parent_address(), uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::HandlerMap(base, uint32_t(r31)).head_address());
  // stw r10,8(r31)
  guest::StoreU32(base, schema::HandlerMap(base, uint32_t(r31)).size_address(), uint32_t(r10));
  // stw r11,0(r11)
  guest::StoreU32(base, schema::HandlerMapNode(base, uint32_t(r11)).left_address(), uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::HandlerMap(base, uint32_t(r31)).head_address());
  // stw r11,8(r11)
  guest::StoreU32(base, schema::HandlerMapNode(base, uint32_t(r11)).right_address(), uint32_t(r11));
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

// sub_8209A008: lifted from generated/ by lift.py.
lifted::Result f8209A008(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_8209A008 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r10,r3
  r10 = r3;
  // lwz r8,0(r4)
  r8 = guest::LoadU32(base, schema::RunLoopEvent(base, uint32_t(r4)).value_address());
  // lis r9,-32246
  r9 = uint64_t(int64_t(-2113273856));
  // addi r11,r10,16
  r11 = uint64_t(int64_t(int64_t(r10) + 16));
  // addi r9,r9,-25896
  r9 = uint64_t(int64_t(int64_t(r9) + -25896));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r8,44(r10)
  guest::StoreU32(base, schema::RunLoop(base, uint32_t(r10)).last_event_address(), uint32_t(r8));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::RunLoop(base, uint32_t(r11) - 16).listeners_address() + 4);
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // ld r6,0(r7)
  r6 = guest::LoadU64(base, uint32_t(r7) + 0);
  // mr r9,r10
  r9 = r10;
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::HandlerMapNode(base, uint32_t(r10)).left_address());
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // ld r5,0(r5)
  r5 = guest::LoadU64(base, uint32_t(r5) + 0);
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // ld r4,0(r4)
  r4 = guest::LoadU64(base, uint32_t(r4) + 0);
  // bl 0x82099d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82099D50, LIFTED_ARGS(0x8209A070, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8209A6E0: lifted from generated/ by lift.py.
lifted::Result f8209A6E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8209A6E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,4072
  r11 = uint64_t(int64_t(int64_t(r11) + 4072));
  // addi r30,r31,28
  r30 = uint64_t(int64_t(int64_t(r31) + 28));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::RunLoop(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, schema::RunLoop(base, uint32_t(r30) - 28).requests_address() + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8209a718
  if (cr0.eq) goto loc_8209A718;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8209A718, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209A718:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // stw r11,4(r30)
  guest::StoreU32(base, schema::RunLoop(base, uint32_t(r30) - 28).requests_address() + 4, uint32_t(r11));
  // stw r11,8(r30)
  guest::StoreU32(base, schema::RunLoop(base, uint32_t(r30) - 28).requests_address() + 8, uint32_t(r11));
  // stw r11,12(r30)
  guest::StoreU32(base, schema::RunLoop(base, uint32_t(r30) - 28).requests_address() + 12, uint32_t(r11));
  // bl 0x82093850
  // (left out: dead in C++)
  { const lifted::Result o = f82093850(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x82093850
  // (left out: dead in C++)
  { const lifted::Result o = f82093850(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,4040
  r11 = uint64_t(int64_t(int64_t(r11) + 4040));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::RunLoop(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
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

// sub_8209AA58: lifted from generated/ by lift.py.
lifted::Result f8209AA58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8209AA58 (lifted)");
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
  // bl 0x8209a6e0
  // (left out: dead in C++)
  { const lifted::Result o = f8209A6E0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8209aa88
  if (cr0.eq) goto loc_8209AA88;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8209AA88, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209AA88:
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

// sub_8209AB08: lifted from generated/ by lift.py.
lifted::Result f8209AB08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8209AB08 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r3,28
  r3 = uint64_t(int64_t(int64_t(r3) + 28));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::RunLoopHandlerKey(base, uint32_t(r11)).key_address());
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8209a930
  // (left out: dead in C++)
  { const lifted::Result o = game::RequestVector::PushBack(base, LIFTED_ARGS(0x8209AB34, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8209ABA8: lifted from generated/ by lift.py.
lifted::Result f8209ABA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8209ABA8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r3,28
  r3 = uint64_t(int64_t(int64_t(r3) + 28));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::RunLoopHandlerKey(base, uint32_t(r11)).key_address());
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8209a930
  // (left out: dead in C++)
  { const lifted::Result o = game::RequestVector::PushBack(base, LIFTED_ARGS(0x8209ABD4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8209AC40: lifted from generated/ by lift.py.
void f8209AC40(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209AC40 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r10,-32153
  r10 = uint64_t(int64_t(-2107179008));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,-12156(r10)
  guest::StoreU32(base, uint32_t(r10) + -12156, uint32_t(r11));
  // blr
  return;
}

// sub_8209AC60: lifted from generated/ by lift.py.
void f8209AC60(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209AC60 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,832
  r11 = uint64_t(int64_t(int64_t(r11) + 832));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // blr
  return;
}

// sub_8209AC78 game::ReportReadError: lifted from generated/ by lift.py.
lifted::Result f8209AC78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8209AC78 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x8258de88
  // (left out: dead in C++)
  { const lifted::Result o = game::Thread::Ctor(base, LIFTED_ARGS(0x8209AC8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,832
  r4 = uint64_t(int64_t(int64_t(r11) + 832));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x8258df18
  // (left out: dead in C++)
  { const lifted::Result o = game::Thread::Begin(base, LIFTED_ARGS(0x8209ACB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295310
  // (left out: dead in C++)
  { const lifted::Result o = f82295310(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8209ACB8: lifted from generated/ by lift.py.
lifted::Result f8209ACB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8209ACB8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f31 = 0.0;
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
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209ACE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209ACF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // bl 0x8258d8c8
  // (left out: dead in C++)
  { const lifted::Result o = game::VideoOutput::LockDevice(base, LIFTED_ARGS(0x8209ACF8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // bl 0x822a2a40
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::SetRenderState_PresentInterval(base, LIFTED_ARGS(0x8209AD04, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
loc_8209AD0C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // lis r7,-256
  r7 = uint64_t(int64_t(-16777216));
  // li r6,63
  r6 = uint64_t(int64_t(63));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822a04f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::Clear(base, r1, r13, r3, r4, r5, r6, r7, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8258d998
  // (left out: dead in C++)
  { const lifted::Result o = game::VideoOutput::ResolveFrontBuffer(base, LIFTED_ARGS(0x8209AD38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x822a0718
  // (left out: dead in C++)
  { const lifted::Result o = f822A0718(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8258d4a8
  // (left out: dead in C++)
  r3 = f8258D4A8(base, r3);
  // mr r4,r3
  r4 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x822a08a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::Swap(base, LIFTED_ARGS(0x8209AD58, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8258d4c0
  // (left out: dead in C++)
  f8258D4C0(base, r3);
  // b 0x8209ad0c
  goto loc_8209AD0C;
}

// sub_8209B2C0: lifted from generated/ by lift.py.
lifted::Result f8209B2C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8209B2C0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0;
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
  // lis r29,-32153
  r29 = uint64_t(int64_t(-2107179008));
  // mr r31,r3
  r31 = r3;
  // lwz r3,-12152(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + -12152);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209B2E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209B2F8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8258d8c8
  // (left out: dead in C++)
  { const lifted::Result o = game::VideoOutput::LockDevice(base, LIFTED_ARGS(0x8209B2FC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::SceneTarget(base, uint32_t(r31)).color_address());
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Resource::Release(base, LIFTED_ARGS(0x8209B308, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SceneTarget(base, uint32_t(r31)).depth_address());
  // stw r30,4(r31)
  guest::StoreU32(base, schema::SceneTarget(base, uint32_t(r31)).color_address(), uint32_t(r30));
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Resource::Release(base, LIFTED_ARGS(0x8209B318, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, schema::SceneTarget(base, uint32_t(r31)).tile_color_address());
  // stw r30,0(r31)
  guest::StoreU32(base, schema::SceneTarget(base, uint32_t(r31)).depth_address(), uint32_t(r30));
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Resource::Release(base, LIFTED_ARGS(0x8209B324, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, schema::SceneTarget(base, uint32_t(r31)).tile_depth_address());
  // stw r30,12(r31)
  guest::StoreU32(base, schema::SceneTarget(base, uint32_t(r31)).tile_color_address(), uint32_t(r30));
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Resource::Release(base, LIFTED_ARGS(0x8209B330, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,24(r31)
  r3 = guest::LoadU32(base, schema::SceneTarget(base, uint32_t(r31)).texture_address());
  // stw r30,8(r31)
  guest::StoreU32(base, schema::SceneTarget(base, uint32_t(r31)).tile_depth_address(), uint32_t(r30));
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Resource::Release(base, LIFTED_ARGS(0x8209B33C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,24(r31)
  guest::StoreU32(base, schema::SceneTarget(base, uint32_t(r31)).texture_address(), uint32_t(r30));
  // lwz r3,-12152(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + -12152);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209B354, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209B364, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258d908
  // (left out: dead in C++)
  { const lifted::Result o = game::VideoOutput::UnlockDevice(base, LIFTED_ARGS(0x8209B36C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8209BCB8: lifted from generated/ by lift.py.
lifted::Result f8209BCB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8209BCB8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32153
  r30 = uint64_t(int64_t(-2107179008));
  // lis r10,-32153
  r10 = uint64_t(int64_t(-2107179008));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r31,-12148(r30)
  r31 = guest::LoadU32(base, uint32_t(r30) + -12148);
  // stw r11,-12152(r10)
  guest::StoreU32(base, uint32_t(r10) + -12152, uint32_t(r11));
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8209bcf8
  if (cr6.eq) goto loc_8209BCF8;
  // mr r3,r31
  r3 = r31;
  // bl 0x8209b2c0
  // (left out: dead in C++)
  { const lifted::Result o = f8209B2C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8209BCF8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209BCF8:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,-12148(r30)
  guest::StoreU32(base, uint32_t(r30) + -12148, uint32_t(r11));
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

// sub_8209C158: lifted from generated/ by lift.py.
lifted::Result f8209C158(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8209C158 (lifted)");
  uint64_t r3 = 0, r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r3,20644(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 20644);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8209c178
  if (cr6.eq) goto loc_8209C178;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Resource::Release(base, LIFTED_ARGS(0x8209C178, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209C178:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r3,20640(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 20640);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8209c18c
  if (cr6.eq) goto loc_8209C18C;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = game::Resource::Release(base, LIFTED_ARGS(0x8209C18C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209C18C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8209D3B0 game::Archive::Shutdown: lifted from generated/ by lift.py.
lifted::Result f8209D3B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8209D3B0 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32150
  r31 = uint64_t(int64_t(-2106982400));
  // lwz r3,20788(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20788);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8209d3e4
  if (cr6.eq) goto loc_8209D3E4;
  // li r5,16384
  r5 = uint64_t(int64_t(16384));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x8209D3DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,20788(r31)
  guest::StoreU32(base, uint32_t(r31) + 20788, uint32_t(r11));
loc_8209D3E4:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r11,20748(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20748);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8209d418
  if (cr6.eq) goto loc_8209D418;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // li r5,16384
  r5 = uint64_t(int64_t(16384));
  // addi r31,r11,20756
  r31 = uint64_t(int64_t(int64_t(r11) + 20756));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x8209D40C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
loc_8209D418:
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lwz r3,2716(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 2716);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x8209D424, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8209d430
  if (!cr0.eq) goto loc_8209D430;
  // bl 0x8209ac78
  // (left out: dead in C++)
  { const lifted::Result o = game::ReportReadError(base, r1, r13, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_8209D430:
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lwz r3,2720(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 2720);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x8209D43C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8209d448
  if (!cr0.eq) goto loc_8209D448;
  // bl 0x8209ac78
  // (left out: dead in C++)
  { const lifted::Result o = game::ReportReadError(base, r1, r13, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_8209D448:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // li r5,16384
  r5 = uint64_t(int64_t(16384));
  // lwz r4,20772(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 20772);
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r3,20768(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 20768);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x8209D460, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // li r5,16384
  r5 = uint64_t(int64_t(16384));
  // lwz r4,20780(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 20780);
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r3,20776(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 20776);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x8209D478, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8209D888 game::RequestQueue::Dtor_8209D888: lifted from generated/ by lift.py.
lifted::Result f8209D888(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8209D888 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x82294d38
  // (left out: dead in C++)
  { const lifted::Result o = game::RequestQueue::Dtor_82294D38(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8209d8b8
  if (cr0.eq) goto loc_8209D8B8;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8209D8B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209D8B8:
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

// sub_8209DF50 game::LoaderFunctor::Clone: lifted from generated/ by lift.py.
lifted::Result f8209DF50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8209DF50 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8209DF6C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8209df8c
  if (cr0.eq) goto loc_8209DF8C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,7644
  r11 = uint64_t(int64_t(int64_t(r11) + 7644));
  // stw r11,0(r3)
  guest::StoreU32(base, schema::LoaderFunctor(base, uint32_t(r3)).vtable_address(), uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::LoaderFunctor(base, uint32_t(r31)).target_address());
  // stw r11,4(r3)
  guest::StoreU32(base, schema::LoaderFunctor(base, uint32_t(r3)).target_address(), uint32_t(r11));
  // b 0x8209df90
  goto loc_8209DF90;
loc_8209DF8C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8209DF90:
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

// sub_8209E7E8 game::SignInWatch::OnModeChange: lifted from generated/ by lift.py.
lifted::Result f8209E7E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8209E7E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r11,-21648(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -21648);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8209e818
  if (!cr6.eq) goto loc_8209E818;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821055d8
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Delete(base, LIFTED_ARGS(0x8209E810, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821057c8
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Create(base, LIFTED_ARGS(0x8209E818, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209E818:
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21696
  r11 = uint64_t(int64_t(int64_t(r11) + -21696));
  // stw r10,-21700(r9)
  guest::StoreU32(base, uint32_t(r9) + -21700, uint32_t(r10));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // mtctr r10
  ctr = r10;
loc_8209E838:
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x8209e838
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8209E838;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,-21648(r31)
  guest::StoreU32(base, uint32_t(r31) + -21648, uint32_t(r11));
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

// sub_8209F138: lifted from generated/ by lift.py.
lifted::Result f8209F138(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8209F138 (lifted)");
  uint64_t r3 = 0, r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // stw r11,-21640(r10)
  guest::StoreU32(base, uint32_t(r10) + -21640, uint32_t(r11));
  // lwz r3,-21636(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + -21636);
  // bl 0x8210afa0
  // (left out: dead in C++)
  { const lifted::Result o = f8210AFA0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,-21632
  r31 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8209f18c
  if (cr6.eq) goto loc_8209F18C;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209F18C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209F18C:
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8209f1ac
  if (cr6.eq) goto loc_8209F1AC;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209F1AC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209F1AC:
  // lwz r3,16(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 16);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8209f1cc
  if (cr6.eq) goto loc_8209F1CC;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209F1CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209F1CC:
  // lwz r3,24(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 24);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8209f1ec
  if (cr6.eq) goto loc_8209F1EC;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209F1EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209F1EC:
  // lwz r3,-21636(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + -21636);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8209f20c
  if (cr6.eq) goto loc_8209F20C;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209F20C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209F20C:
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

// sub_8209F590: lifted from generated/ by lift.py.
void f8209F590(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209F590 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,-21600(r10)
  guest::StoreU32(base, uint32_t(r10) + -21600, uint32_t(r11));
  // blr
  return;
}

// sub_8209FB80: lifted from generated/ by lift.py.
lifted::Result f8209FB80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8209FB80 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,7872
  r11 = uint64_t(int64_t(int64_t(r11) + 7872));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::VtableObject(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x8209fbac
  if (cr0.eq) goto loc_8209FBAC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8209FBAC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209FBAC:
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

// sub_8209FBC8: lifted from generated/ by lift.py.
lifted::Result f8209FBC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8209FBC8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // addi r31,r11,-21504
  r31 = uint64_t(int64_t(int64_t(r11) + -21504));
loc_8209FBE0:
  // mr r3,r31
  r3 = r31;
  // bl 0x820a2f38
  // (left out: dead in C++)
  { const lifted::Result o = f820A2F38(base, r1, r13, r3, r5, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, 0x8209FBE8); r3 = o.r3; f1 = o.f1; }
  // b 0x8209fbe0
  goto loc_8209FBE0;
}

// sub_820A0008 game::SaveInterface::Clear: lifted from generated/ by lift.py.
void f820A0008(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820A0008 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,-9072(r10)
  guest::StoreU32(base, uint32_t(r10) + -9072, uint32_t(r11));
  // blr
  return;
}

// sub_820A00F8 game::SaveInterface::Word992: lifted from generated/ by lift.py.
lifted::Result f820A00F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A00F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r3,-9072(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -9072);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820A0128, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r4,r31
  r4 = r31;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820A0140, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 1, so);
  // blt cr6,0x820a018c
  if (cr6.lt) goto loc_820A018C;
  // beq cr6,0x820a0184
  if (cr6.eq) goto loc_820A0184;
  // cmplwi cr6,r3,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 3, so);
  // blt cr6,0x820a017c
  if (cr6.lt) goto loc_820A017C;
  // beq cr6,0x820a0174
  if (cr6.eq) goto loc_820A0174;
  // cmplwi cr6,r3,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 5, so);
  // blt cr6,0x820a016c
  if (cr6.lt) goto loc_820A016C;
  // bne cr6,0x820a0190
  if (!cr6.eq) goto loc_820A0190;
  // li r30,5
  r30 = uint64_t(int64_t(5));
  // b 0x820a0190
  goto loc_820A0190;
loc_820A016C:
  // li r30,4
  r30 = uint64_t(int64_t(4));
  // b 0x820a0190
  goto loc_820A0190;
loc_820A0174:
  // li r30,3
  r30 = uint64_t(int64_t(3));
  // b 0x820a0190
  goto loc_820A0190;
loc_820A017C:
  // li r30,2
  r30 = uint64_t(int64_t(2));
  // b 0x820a0190
  goto loc_820A0190;
loc_820A0184:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x820a0190
  goto loc_820A0190;
loc_820A018C:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_820A0190:
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

// sub_820A01B0 game::SaveInterface::Request980: lifted from generated/ by lift.py.
lifted::Result f820A01B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A01B0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // lwz r3,-9072(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -9072);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820A01DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // mr r4,r31
  r4 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820A01F0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820A0260 game::SaveInterface::Word84: lifted from generated/ by lift.py.
lifted::Result f820A0260(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A0260 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r3,-9072(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -9072);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820A0294, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // mr r5,r30
  r5 = r30;
  // mr r4,r31
  r4 = r31;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820A02AC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820A2F38: lifted from generated/ by lift.py.
lifted::Result f820A2F38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820A2F38 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r31 = 0;
  // b 0x820a0c40
  { const lifted::Result o = game::CameraPass::ReleaseSurfaces(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A4398 game::Movies::Clear: lifted from generated/ by lift.py.
void f820A4398(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820A4398 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,7388(r10)
  guest::StoreU32(base, uint32_t(r10) + 7388, uint32_t(r11));
  // blr
  return;
}

// sub_820A44C0: lifted from generated/ by lift.py.
lifted::Result f820A44C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820A44C0 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,8520
  r11 = uint64_t(int64_t(int64_t(r11) + 8520));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::VtableObject(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x820a44ec
  if (cr0.eq) goto loc_820A44EC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820A44EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_820A44EC:
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

// sub_820A4508: lifted from generated/ by lift.py.
lifted::Result f820A4508(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A4508 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,8592
  r11 = uint64_t(int64_t(int64_t(r11) + 8592));
  // lwz r3,80(r31)
  r3 = guest::LoadU32(base, schema::SaveSlot(base, uint32_t(r31)).enumeration_buffer_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::SaveSlot(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x820a4538
  if (cr0.eq) goto loc_820A4538;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820A4538, r31)); r3 = o.r3; f1 = o.f1; }
loc_820A4538:
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, schema::SaveSlot(base, uint32_t(r31)).open_failed_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a4548
  if (cr0.eq) goto loc_820A4548;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820A4548, r31)); r3 = o.r3; f1 = o.f1; }
loc_820A4548:
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

// sub_820A54E8 game::SaveSlot::RequestDelete: lifted from generated/ by lift.py.
lifted::Result f820A54E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A54E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820A5514, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 3, so);
  // beq cr6,0x820a5524
  if (cr6.eq) goto loc_820A5524;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820a5578
  goto loc_820A5578;
loc_820A5524:
  // lwz r11,44(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 44);
  // mulli r30,r30,1000
  r30 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(1000))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // mr r4,r29
  r4 = r29;
  // addi r3,r11,504
  r3 = uint64_t(int64_t(int64_t(r11) + 504));
  // bl 0x82295d48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295D48, LIFTED_ARGS(0x820A553C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,44(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 44);
  // mr r10,r28
  r10 = r28;
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r11,r11,760
  r11 = uint64_t(int64_t(int64_t(r11) + 760));
loc_820A554C:
  // lbz r9,0(r10)
  r9 = guest::LoadU8(base, schema::SaveCString(base, uint32_t(r10)).first_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // stb r9,0(r11)
  guest::StoreU8(base, uint32_t(r11) + 0, uint8_t(r9));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // bne 0x820a554c
  if (!cr0.eq) goto loc_820A554C;
  // lwz r11,44(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 44);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // add r11,r11,r30
  r11 = r11 + r30;
  // stw r10,976(r11)
  guest::StoreU32(base, uint32_t(r11) + 976, uint32_t(r10));
loc_820A5578:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A55A8: lifted from generated/ by lift.py.
lifted::Result f820A55A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820A55A8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // lwz r10,44(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 44);
  // mr r4,r5
  r4 = r5;
  // mulli r11,r11,1000
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1000))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r6,r1,88
  r6 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,956(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 956);
  // bl 0x82295c00
  // (left out: dead in C++)
  { const lifted::Result o = game::XContentGetCreator(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x820A55DC); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820A5660: lifted from generated/ by lift.py.
lifted::Result f820A5660(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A5660 (lifted)");
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
  // beq 0x820a56c0
  if (cr0.eq) goto loc_820A56C0;
  // addi r29,r30,-8
  r29 = uint64_t(int64_t(int64_t(r30) + -8));
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::SaveSlot(base, uint32_t(r29) + 8).array_count_address());
  // mulli r10,r11,1000
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1000))));
  // addic. r31,r11,-1
  ca = uint32_t(r11) > 0;
  r31 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // add r30,r10,r30
  r30 = r10 + r30;
  // blt 0x820a56a8
  if (cr0.lt) goto loc_820A56A8;
loc_820A5694:
  // addi r30,r30,-1000
  r30 = uint64_t(int64_t(int64_t(r30) + -1000));
  // mr r3,r30
  r3 = r30;
  // bl 0x820a4508
  // (left out: dead in C++)
  { const lifted::Result o = f820A4508(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge 0x820a5694
  if (!cr0.lt) goto loc_820A5694;
loc_820A56A8:
  // clrlwi. r11,r28,31
  r11 = uint32_t(r28) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a56b8
  if (cr0.eq) goto loc_820A56B8;
  // mr r3,r29
  r3 = r29;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x820A56B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820A56B8:
  // mr r3,r29
  r3 = r29;
  // b 0x820a56dc
  goto loc_820A56DC;
loc_820A56C0:
  // mr r3,r30
  r3 = r30;
  // bl 0x820a4508
  // (left out: dead in C++)
  { const lifted::Result o = f820A4508(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r28,31
  r11 = uint32_t(r28) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a56d8
  if (cr0.eq) goto loc_820A56D8;
  // mr r3,r30
  r3 = r30;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820A56D8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820A56D8:
  // mr r3,r30
  r3 = r30;
loc_820A56DC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A57D0: lifted from generated/ by lift.py.
uint64_t f820A57D0(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820A57D0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r10,44(r3)
  r10 = guest::LoadU32(base, schema::SaveDevices(base, uint32_t(r3)).slots_address());
  // mulli r11,r4,1000
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(1000))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r10,r5,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,84(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 84);
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // blr
  return r3;
}

// sub_820A5848: lifted from generated/ by lift.py.
uint64_t f820A5848(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A5848 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r10,44(r3)
  r10 = guest::LoadU32(base, schema::SaveDevices(base, uint32_t(r3)).slots_address());
  // mulli r11,r4,1000
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(1000))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r3,992(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 992);
  // blr
  return r3;
}

// sub_820A5860: lifted from generated/ by lift.py.
void f820A5860(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A5860 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r10,44(r3)
  r10 = guest::LoadU32(base, schema::SaveDevices(base, uint32_t(r3)).slots_address());
  // mulli r11,r4,1000
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(1000))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,980(r11)
  guest::StoreU32(base, uint32_t(r11) + 980, uint32_t(r10));
  // blr
  return;
}

// sub_820A6420: lifted from generated/ by lift.py.
double f820A6420(uint8_t* base, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8) {
  ITC_NATIVE_RUNNING("sub_820A6420 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // fsubs f13,f2,f3
  lifted::ScalarFloatMode();
  f13 = double(float(f2 - f3));
  // fsubs f0,f2,f1
  f0 = double(float(f2 - f1));
  // fmuls f12,f13,f4
  f12 = double(float(f13 * f4));
  // fmr f13,f0
  f13 = f0;
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // ble cr6,0x820a643c
  if (!cr6.gt) goto loc_820A643C;
  // fmr f13,f8
  f13 = f8;
loc_820A643C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f11 = double(temp.f32);
  // fcmpu cr6,f13,f11
  cr6 = lifted::CompareFloat(f13, f11);
  // beqlr cr6
  if (cr6.eq) return f1;
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // fsubs f11,f6,f13
  f11 = double(float(f6 - f13));
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // fmuls f12,f11,f5
  f12 = double(float(f11 * f5));
  // fmadds f13,f13,f7,f12
  f13 = double(float(std::fma(f13, f7, f12)));
  // fmuls f13,f13,f4
  f13 = double(float(f13 * f4));
  // fmadds f1,f13,f0,f1
  f1 = double(float(std::fma(f13, f0, f1)));
  // blr
  return f1;
}

// sub_820A6A50 game::GameFile::OpenWrite: lifted from generated/ by lift.py.
lifted::Result f820A6A50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A6A50 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-640(r1)
  ea = -640 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // bl 0x82593c28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593C28, LIFTED_ARGS(0x820A6A70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82593ca0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593CA0, LIFTED_ARGS(0x820A6A80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r29
  r5 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8258c608
  // (left out: dead in C++)
  { const lifted::Result o = game::File::OpenWrite(base, r1, r13, r3, r4, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a6ab0
  if (cr0.eq) goto loc_820A6AB0;
  // li r31,1
  r31 = uint64_t(int64_t(1));
loc_820A6A9C:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82593c48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593C48, LIFTED_ARGS(0x820A6AA4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,640
  r1 = uint64_t(int64_t(int64_t(r1) + 640));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_820A6AB0:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x820a6a9c
  goto loc_820A6A9C;
}

// sub_820A6B20 game::GameFile::OpenAsync: lifted from generated/ by lift.py.
lifted::Result f820A6B20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A6B20 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-640(r1)
  ea = -640 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // bl 0x82593c28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593C28, LIFTED_ARGS(0x820A6B40, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82593ca0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593CA0, LIFTED_ARGS(0x820A6B50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r29
  r5 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x82593b10
  // (left out: dead in C++)
  { const lifted::Result o = f82593B10(base, r1, r13, r3, r4, r5, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a6b80
  if (cr0.eq) goto loc_820A6B80;
  // li r31,1
  r31 = uint64_t(int64_t(1));
loc_820A6B6C:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82593c48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593C48, LIFTED_ARGS(0x820A6B74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,640
  r1 = uint64_t(int64_t(int64_t(r1) + 640));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_820A6B80:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x820a6b6c
  goto loc_820A6B6C;
}

// sub_820A6B88 game::GameFile_820A6B88: lifted from generated/ by lift.py.
lifted::Result f820A6B88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A6B88 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,8816
  r11 = uint64_t(int64_t(int64_t(r11) + 8816));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::GameFile(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // bl 0x82593b00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593B00, LIFTED_ARGS(0x820A6BB4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a6bc4
  if (cr0.eq) goto loc_820A6BC4;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820A6BC4, r31)); r3 = o.r3; f1 = o.f1; }
loc_820A6BC4:
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

// sub_820A6C68 game::GamePath::Set: lifted from generated/ by lift.py.
lifted::Result f820A6C68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A6C68 (lifted)");
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
  // stwu r1,-624(r1)
  ea = -624 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // bl 0x82593c28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593C28, LIFTED_ARGS(0x820A6C8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82593ca0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593CA0, LIFTED_ARGS(0x820A6C9C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r11,r31
  r11 = r31;
loc_820A6CA4:
  // lbz r10,0(r3)
  r10 = guest::LoadU8(base, schema::PathText(base, uint32_t(r3)).first_char_address());
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stb r10,0(r11)
  guest::StoreU8(base, schema::GamePath(base, uint32_t(r11) - 4).text_address(), uint8_t(r10));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // bne 0x820a6ca4
  if (!cr0.eq) goto loc_820A6CA4;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82593c48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593C48, LIFTED_ARGS(0x820A6CC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,624
  r1 = uint64_t(int64_t(int64_t(r1) + 624));
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

// sub_820A6CE0 game::GamePath_820A6CE0: lifted from generated/ by lift.py.
lifted::Result f820A6CE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820A6CE0 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,8864
  r11 = uint64_t(int64_t(int64_t(r11) + 8864));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::GamePath(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x820a6d0c
  if (cr0.eq) goto loc_820A6D0C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820A6D0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820A6D0C:
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

// sub_820A6D28: lifted from generated/ by lift.py.
lifted::Result f820A6D28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A6D28 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r27,r7
  r27 = r7;
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bge cr6,0x820a6d58
  if (!cr6.lt) goto loc_820A6D58;
  // bl 0x825a6b18
  // (left out: dead in C++)
  { const lifted::Result o = f825A6B18(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_820A6D58:
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // subf r11,r30,r11
  r11 = r11 - r30;
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bge cr6,0x820a6d6c
  if (!cr6.lt) goto loc_820A6D6C;
  // mr r29,r11
  r29 = r11;
loc_820A6D6C:
  // cmplw cr6,r29,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r27), so);
  // mr r8,r29
  r8 = r29;
  // blt cr6,0x820a6d7c
  if (cr6.lt) goto loc_820A6D7C;
  // mr r8,r27
  r8 = r27;
loc_820A6D7C:
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x820a6d90
  if (cr6.lt) goto loc_820A6D90;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // b 0x820a6d94
  goto loc_820A6D94;
loc_820A6D90:
  // addi r11,r31,4
  r11 = uint64_t(int64_t(int64_t(r31) + 4));
loc_820A6D94:
  // mr r10,r28
  r10 = r28;
  // add r11,r11,r30
  r11 = r11 + r30;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi r8,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq 0x820a6dcc
  if (cr0.eq) goto loc_820A6DCC;
  // add r8,r11,r8
  r8 = r11 + r8;
loc_820A6DAC:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // lbz r7,0(r10)
  r7 = guest::LoadU8(base, uint32_t(r10) + 0);
  // subf. r9,r7,r9
  r9 = r9 - r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x820a6dcc
  if (!cr0.eq) goto loc_820A6DCC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r8), so);
  // bne cr6,0x820a6dac
  if (!cr6.eq) goto loc_820A6DAC;
loc_820A6DCC:
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne 0x820a6df4
  if (!cr0.eq) goto loc_820A6DF4;
  // cmplw cr6,r29,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r27), so);
  // bge cr6,0x820a6de4
  if (!cr6.lt) goto loc_820A6DE4;
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // b 0x820a6df4
  goto loc_820A6DF4;
loc_820A6DE4:
  // subf r11,r29,r27
  r11 = r27 - r29;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r9,r11,1
  r9 = r11 ^ 1;
loc_820A6DF4:
  // mr r3,r9
  r3 = r9;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A6E00: lifted from generated/ by lift.py.
lifted::Result f820A6E00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820A6E00 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
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
  // mr r6,r4
  r6 = r4;
  // mr r11,r6
  r11 = r6;
  // mr r10,r11
  r10 = r11;
loc_820A6E18:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x820a6e18
  if (!cr6.eq) goto loc_820A6E18;
  // subf r11,r10,r11
  r11 = r11 - r10;
  // lwz r5,20(r3)
  r5 = guest::LoadU32(base, uint32_t(r3) + 20);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // rotlwi r7,r11,0
  r7 = __builtin_rotateleft32(uint32_t(r11), 0);
  // bl 0x820a6d28
  // (left out: dead in C++)
  { const lifted::Result o = f820A6D28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // clrlwi r3,r11,24
  r3 = uint32_t(r11) & 0xFF;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

lifted::Result e820949D0(uint8_t* base, const lifted::Args& a) {
  return f820949D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e82094A70(uint8_t* base, const lifted::Args& a) {
  return f82094A70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e82094B68(uint8_t* base, const lifted::Args& a) {
  return f82094B68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82094BB0(uint8_t* base, const lifted::Args& a) {
  return f82094BB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82094BE8(uint8_t* base, const lifted::Args& a) {
  return f82094BE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82094C80(uint8_t* base, const lifted::Args& a) {
  return f82094C80(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82094CD0(uint8_t* base, const lifted::Args& a) {
  return f82094CD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82094D20(uint8_t* base, const lifted::Args& a) {
  return f82094D20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82094D68(uint8_t* base, const lifted::Args& a) {
  return f82094D68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82094DE0(uint8_t* base, const lifted::Args& a) {
  return f82094DE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82094E30(uint8_t* base, const lifted::Args& a) {
  return f82094E30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82094EE0(uint8_t* base, const lifted::Args& a) {
  return f82094EE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82094F30(uint8_t* base, const lifted::Args& a) {
  return f82094F30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e82095168(uint8_t* base, const lifted::Args& a) {
  return {f82095168(base, a.r[0]), a.f[0]};
}

lifted::Result e82095188(uint8_t* base, const lifted::Args& a) {
  return f82095188(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82095208(uint8_t* base, const lifted::Args& a) {
  return f82095208(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82095210(uint8_t* base, const lifted::Args& a) {
  return f82095210(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82095218(uint8_t* base, const lifted::Args& a) {
  f82095218(base, a.r1, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82095298(uint8_t* base, const lifted::Args& a) {
  return f82095298(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82095370(uint8_t* base, const lifted::Args& a) {
  return f82095370(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82095468(uint8_t* base, const lifted::Args& a) {
  return f82095468(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820954F8(uint8_t* base, const lifted::Args& a) {
  return f820954F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82095620(uint8_t* base, const lifted::Args& a) {
  return f82095620(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82095678(uint8_t* base, const lifted::Args& a) {
  f82095678(base, a.r1, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820956F0(uint8_t* base, const lifted::Args& a) {
  return f820956F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820957F0(uint8_t* base, const lifted::Args& a) {
  return f820957F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820959D0(uint8_t* base, const lifted::Args& a) {
  return f820959D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82095A28(uint8_t* base, const lifted::Args& a) {
  return f82095A28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82095DF8(uint8_t* base, const lifted::Args& a) {
  return f82095DF8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82095FE0(uint8_t* base, const lifted::Args& a) {
  return f82095FE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82096220(uint8_t* base, const lifted::Args& a) {
  return f82096220(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82096320(uint8_t* base, const lifted::Args& a) {
  return f82096320(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e820987E8(uint8_t* base, const lifted::Args& a) {
  return f820987E8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82098860(uint8_t* base, const lifted::Args& a) {
  return f82098860(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820989A8(uint8_t* base, const lifted::Args& a) {
  return f820989A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82098A48(uint8_t* base, const lifted::Args& a) {
  return f82098A48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82098C70(uint8_t* base, const lifted::Args& a) {
  return f82098C70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82098CE8(uint8_t* base, const lifted::Args& a) {
  return f82098CE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82098F30(uint8_t* base, const lifted::Args& a) {
  return f82098F30(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82098FF8(uint8_t* base, const lifted::Args& a) {
  return f82098FF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82099048(uint8_t* base, const lifted::Args& a) {
  return f82099048(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e820994E0(uint8_t* base, const lifted::Args& a) {
  return f820994E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82099528(uint8_t* base, const lifted::Args& a) {
  return f82099528(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82099578(uint8_t* base, const lifted::Args& a) {
  return {f82099578(base, a.r[0]), a.f[0]};
}

lifted::Result e82099580(uint8_t* base, const lifted::Args& a) {
  return {f82099580(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82099590(uint8_t* base, const lifted::Args& a) {
  return f82099590(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82099600(uint8_t* base, const lifted::Args& a) {
  return f82099600(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82099710(uint8_t* base, const lifted::Args& a) {
  return {f82099710(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82099740(uint8_t* base, const lifted::Args& a) {
  return {f82099740(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82099780(uint8_t* base, const lifted::Args& a) {
  return {f82099780(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820997B0(uint8_t* base, const lifted::Args& a) {
  return {f820997B0(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820997D8(uint8_t* base, const lifted::Args& a) {
  return f820997D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820998B8(uint8_t* base, const lifted::Args& a) {
  return f820998B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e82099A08(uint8_t* base, const lifted::Args& a) {
  return f82099A08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82099A50(uint8_t* base, const lifted::Args& a) {
  return f82099A50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82099AD8(uint8_t* base, const lifted::Args& a) {
  return f82099AD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e82099BA8(uint8_t* base, const lifted::Args& a) {
  return f82099BA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82099CF8(uint8_t* base, const lifted::Args& a) {
  return f82099CF8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8209A008(uint8_t* base, const lifted::Args& a) {
  return f8209A008(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e8209A6E0(uint8_t* base, const lifted::Args& a) {
  return f8209A6E0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8209AA58(uint8_t* base, const lifted::Args& a) {
  return f8209AA58(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8209AB08(uint8_t* base, const lifted::Args& a) {
  return f8209AB08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8209ABA8(uint8_t* base, const lifted::Args& a) {
  return f8209ABA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8209AC40(uint8_t* base, const lifted::Args& a) {
  f8209AC40(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8209AC60(uint8_t* base, const lifted::Args& a) {
  f8209AC60(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8209AC78(uint8_t* base, const lifted::Args& a) {
  return f8209AC78(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8209ACB8(uint8_t* base, const lifted::Args& a) {
  return f8209ACB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8209B2C0(uint8_t* base, const lifted::Args& a) {
  return f8209B2C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8209BCB8(uint8_t* base, const lifted::Args& a) {
  return f8209BCB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8209C158(uint8_t* base, const lifted::Args& a) {
  return f8209C158(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8209D3B0(uint8_t* base, const lifted::Args& a) {
  return f8209D3B0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8209D888(uint8_t* base, const lifted::Args& a) {
  return f8209D888(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8209DF50(uint8_t* base, const lifted::Args& a) {
  return f8209DF50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8209E7E8(uint8_t* base, const lifted::Args& a) {
  return f8209E7E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8209F138(uint8_t* base, const lifted::Args& a) {
  return f8209F138(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8209F590(uint8_t* base, const lifted::Args& a) {
  f8209F590(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8209FB80(uint8_t* base, const lifted::Args& a) {
  return f8209FB80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8209FBC8(uint8_t* base, const lifted::Args& a) {
  return f8209FBC8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820A0008(uint8_t* base, const lifted::Args& a) {
  f820A0008(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A00F8(uint8_t* base, const lifted::Args& a) {
  return f820A00F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A01B0(uint8_t* base, const lifted::Args& a) {
  return f820A01B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A0260(uint8_t* base, const lifted::Args& a) {
  return f820A0260(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A2F38(uint8_t* base, const lifted::Args& a) {
  return f820A2F38(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12], a.lr);
}

lifted::Result e820A4398(uint8_t* base, const lifted::Args& a) {
  f820A4398(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A44C0(uint8_t* base, const lifted::Args& a) {
  return f820A44C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820A4508(uint8_t* base, const lifted::Args& a) {
  return f820A4508(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A54E8(uint8_t* base, const lifted::Args& a) {
  return f820A54E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A55A8(uint8_t* base, const lifted::Args& a) {
  return f820A55A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[5], a.r[6]);
}

lifted::Result e820A5660(uint8_t* base, const lifted::Args& a) {
  return f820A5660(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A57D0(uint8_t* base, const lifted::Args& a) {
  return {f820A57D0(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820A5848(uint8_t* base, const lifted::Args& a) {
  return {f820A5848(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A5860(uint8_t* base, const lifted::Args& a) {
  f820A5860(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A6420(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A6420(base, a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7])};
}

lifted::Result e820A6A50(uint8_t* base, const lifted::Args& a) {
  return f820A6A50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A6B20(uint8_t* base, const lifted::Args& a) {
  return f820A6B20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A6B88(uint8_t* base, const lifted::Args& a) {
  return f820A6B88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A6C68(uint8_t* base, const lifted::Args& a) {
  return f820A6C68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A6CE0(uint8_t* base, const lifted::Args& a) {
  return f820A6CE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820A6D28(uint8_t* base, const lifted::Args& a) {
  return f820A6D28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A6E00(uint8_t* base, const lifted::Args& a) {
  return f820A6E00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x820949D0, e820949D0},
    {0x82094A70, e82094A70},
    {0x82094B68, e82094B68},
    {0x82094BB0, e82094BB0},
    {0x82094BE8, e82094BE8},
    {0x82094C80, e82094C80},
    {0x82094CD0, e82094CD0},
    {0x82094D20, e82094D20},
    {0x82094D68, e82094D68},
    {0x82094DE0, e82094DE0},
    {0x82094E30, e82094E30},
    {0x82094EE0, e82094EE0},
    {0x82094F30, e82094F30},
    {0x82095168, e82095168},
    {0x82095188, e82095188},
    {0x82095208, e82095208},
    {0x82095210, e82095210},
    {0x82095218, e82095218},
    {0x82095298, e82095298},
    {0x82095370, e82095370},
    {0x82095468, e82095468},
    {0x820954F8, e820954F8},
    {0x82095620, e82095620},
    {0x82095678, e82095678},
    {0x820956F0, e820956F0},
    {0x820957F0, e820957F0},
    {0x820959D0, e820959D0},
    {0x82095A28, e82095A28},
    {0x82095DF8, e82095DF8},
    {0x82095FE0, e82095FE0},
    {0x82096220, e82096220},
    {0x82096320, e82096320},
    {0x820987E8, e820987E8},
    {0x82098860, e82098860},
    {0x820989A8, e820989A8},
    {0x82098A48, e82098A48},
    {0x82098C70, e82098C70},
    {0x82098CE8, e82098CE8},
    {0x82098F30, e82098F30},
    {0x82098FF8, e82098FF8},
    {0x82099048, e82099048},
    {0x820994E0, e820994E0},
    {0x82099528, e82099528},
    {0x82099578, e82099578},
    {0x82099580, e82099580},
    {0x82099590, e82099590},
    {0x82099600, e82099600},
    {0x82099710, e82099710},
    {0x82099740, e82099740},
    {0x82099780, e82099780},
    {0x820997B0, e820997B0},
    {0x820997D8, e820997D8},
    {0x820998B8, e820998B8},
    {0x82099A08, e82099A08},
    {0x82099A50, e82099A50},
    {0x82099AD8, e82099AD8},
    {0x82099BA8, e82099BA8},
    {0x82099CF8, e82099CF8},
    {0x8209A008, e8209A008},
    {0x8209A6E0, e8209A6E0},
    {0x8209AA58, e8209AA58},
    {0x8209AB08, e8209AB08},
    {0x8209ABA8, e8209ABA8},
    {0x8209AC40, e8209AC40},
    {0x8209AC60, e8209AC60},
    {0x8209AC78, e8209AC78},
    {0x8209ACB8, e8209ACB8},
    {0x8209B2C0, e8209B2C0},
    {0x8209BCB8, e8209BCB8},
    {0x8209C158, e8209C158},
    {0x8209D3B0, e8209D3B0},
    {0x8209D888, e8209D888},
    {0x8209DF50, e8209DF50},
    {0x8209E7E8, e8209E7E8},
    {0x8209F138, e8209F138},
    {0x8209F590, e8209F590},
    {0x8209FB80, e8209FB80},
    {0x8209FBC8, e8209FBC8},
    {0x820A0008, e820A0008},
    {0x820A00F8, e820A00F8},
    {0x820A01B0, e820A01B0},
    {0x820A0260, e820A0260},
    {0x820A2F38, e820A2F38},
    {0x820A4398, e820A4398},
    {0x820A44C0, e820A44C0},
    {0x820A4508, e820A4508},
    {0x820A54E8, e820A54E8},
    {0x820A55A8, e820A55A8},
    {0x820A5660, e820A5660},
    {0x820A57D0, e820A57D0},
    {0x820A5848, e820A5848},
    {0x820A5860, e820A5860},
    {0x820A6420, e820A6420},
    {0x820A6A50, e820A6A50},
    {0x820A6B20, e820A6B20},
    {0x820A6B88, e820A6B88},
    {0x820A6C68, e820A6C68},
    {0x820A6CE0, e820A6CE0},
    {0x820A6D28, e820A6D28},
    {0x820A6E00, e820A6E00},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_002)), true);

}  // namespace

}  // namespace lifted
